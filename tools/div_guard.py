"""Strip the ASPSX zero-divisor guard the game's own overlay code never emits.

Evidence (see tools/division_scan.py and reports/handoffs derived from it):
every one of the 196 division sites in this game's overlays uses a bare
2-operand `div`/`divu` + `mflo`/`mfhi`, with zero canonical zero-divisor
guards. ASPSX, however, always expands the compiler's 3-operand
`div $rd,$rs,$rt` pseudo-op into a 5-word `bnez`/`break 7`/`break 6` guard
sequence, inflating the object beyond the PAL body size (blocker
CODEGEN_SIZE). No pinned ASPSX version exposes a flag to suppress this; only
`-mno-check-zero-division` on cc1 (psyq-gcc-2.95.2) avoids it at the source
level, and even that still leaves other toolchains blocked.

This reproduces, at the assembly-text level (between cc1 and ASPSX), exactly
what the original build's assembler did: the bare 2-operand `div`/`divu`
instruction (the real R3000 hardware form, which writes only HI/LO -- no
destination register field) followed by mflo/mfhi, wrapped in
`.set noreorder`/`.set nomacro` so ASPSX does not re-expand or reschedule
it. Measured directly: the 3-operand form with `$0` as a placeholder
destination (`div $0,$rs,$rt`) still triggers ASPSX 2.79's guard expansion
even under `.set nomacro`; only the true 2-operand form suppresses it. It
also reproduces one further original-assembler
quirk: a `nop` is inserted after `mflo`/`mfhi` if and only if the next real
instruction reads that destination register -- and never when that next
instruction is itself a `mult`/`div`-family op, which needs a *different*
two-word hazard gap that ASPSX still inserts on its own in reorder mode.

Ported from a sister decompilation project's independently measured rule
(63/66 exact matches on functions containing a division, and a from-scratch
binary verifier confirming zero unsafe exceptions across ~200 real division
sites): the gap fires if and only if the following real instruction reads
the mflo/mfhi destination register, and never before a mult/div-family op.

Placement: when one or more labels separate the mflo/mfhi from that reading
instruction, the gap is emitted after the labels, immediately before the
reader, so a branch to the label lands on the nop. Measured on CARDGAME
0x80096cf0: its PAL `j 0x80096de8` targets the nop that follows `mflo v0` and
precedes the labelled reader `sh v0,2`; placing the gap before the label
changed exactly that jump target, placing it after the label reproduced the PAL
body. A read-only census of the hashed PAL images (tools/div_guard_audit.py)
found no site where the instruction after such a gap is a branch target while
the gap is not. Without an intervening label the emitted text is unchanged.
"""

from __future__ import annotations

import re

_DIV_RE = re.compile(rb"\t(div|divu|rem|remu)\t(\$\w+),(\$\w+),(\$\w+)\r?\n")

_RESULT_MOVE = {b"div": b"mflo", b"divu": b"mflo", b"rem": b"mfhi", b"remu": b"mfhi"}
_BARE_OP = {b"div": b"div", b"divu": b"divu", b"rem": b"div", b"remu": b"divu"}

# Mnemonics whose first operand is written, not read (all remaining operands
# of these instructions are reads).
_WRITES_FIRST_OPERAND = frozenset(
    b"""
    lw lh lhu lb lbu lwc1 ldc1 la li lui move mov
    add addu addi addiu sub subu neg negu
    and andi or ori xor xori nor not
    sll srl sra sllv srlv srav
    slt slti sltu sltiu seq sne sle sleu sge sgeu sgt sgtu
    mul mulo mulou div divu rem remu
    mfhi mflo mfc0 mfc1 mfc2 cfc1 cfc2 abs
    """.split()
)

# Mnemonics that write HI/LO instead of a GPR operand; a mflo/mfhi followed
# by one of these needs ASPSX's own separate two-word hazard gap, not ours.
_WRITES_HI_LO = frozenset(
    b"mult multu div divu rem remu madd maddu msub msubu mthi mtlo".split()
)

_INSTRUCTION_RE = re.compile(rb"^[ \t]*([a-z][a-z0-9.]*)[ \t]+(.*)$")
_REGISTER_RE = re.compile(rb"\$\w+")


def _reads_register(line: bytes, register: bytes) -> bool:
    line = line.split(b"#", 1)[0]
    match = _INSTRUCTION_RE.match(line)
    if match is None:
        return False
    mnemonic, operands = match.group(1), match.group(2)
    if mnemonic in _WRITES_HI_LO:
        return False
    parts = operands.split(b",")
    if mnemonic in _WRITES_FIRST_OPERAND and parts:
        parts = parts[1:]
    for part in parts:
        if register in _REGISTER_RE.findall(part):
            return True
    return False


def _gap_site(assembly: bytes, start: int, register: bytes) -> tuple[bool, int | None, bool]:
    """Look past blank lines, comments, `.set` directives and labels to the next
    real instruction.

    Returns ``(needs_gap, reader_offset, noreorder)``: whether that instruction
    reads ``register``; the byte offset of the reader's line when at least one
    label was crossed (``None`` otherwise); and whether `.set noreorder` is in
    effect at the reader (the replacement block itself ends in reorder mode).
    Any other directive, unknown syntax or end of text means no gap.
    """
    offset, crossed_label, noreorder = start, False, False
    while offset < len(assembly):
        newline = assembly.find(b"\n", offset)
        line_end = len(assembly) if newline < 0 else newline + 1
        line = assembly[offset:line_end].rstrip(b"\r\n")
        stripped = line.strip()
        if not stripped or stripped.startswith(b"#"):
            pass
        elif stripped.startswith(b"."):
            if not stripped.startswith(b".set"):
                return False, None, False
            words = stripped.split()
            if len(words) > 1 and words[1] in (b"noreorder", b"reorder"):
                noreorder = words[1] == b"noreorder"
        elif stripped.endswith(b":"):
            crossed_label = True
        else:
            reads = _reads_register(line, register)
            return reads, offset if crossed_label else None, noreorder
        offset = line_end
    return False, None, False


def _needs_gap(assembly: bytes, start: int, register: bytes) -> bool:
    return _gap_site(assembly, start, register)[0]


def strip_div_guard(assembly: bytes) -> bytes:
    """Replace every macro-form div/divu/rem/remu with the bare instruction
    pair the game's own overlay code always uses, including the original
    assembler's conditional post-mflo/mfhi nop (after any intervening labels)."""
    pieces: list[bytes] = []
    end = 0
    for match in _DIV_RE.finditer(assembly):
        pieces.append(assembly[end : match.start()])
        opcode, dest, rs, rt = match.group(1), match.group(2), match.group(3), match.group(4)
        move = _RESULT_MOVE[opcode]
        needs_gap, reader_offset, noreorder = _gap_site(assembly, match.end(), dest)
        inline_gap = b"\tnop\r\n" if needs_gap and reader_offset is None else b""
        pieces.append(
            b"\t.set\tnoreorder\r\n\t.set\tnomacro\r\n"
            b"\t%s\t%s,%s\r\n\t%s\t%s\r\n%s"
            b"\t.set\tmacro\r\n\t.set\treorder\r\n"
            % (_BARE_OP[opcode], rs, rt, move, dest, inline_gap)
        )
        end = match.end()
        if needs_gap and reader_offset is not None:
            # Labels, comments and .set lines between the division and its
            # reader stay in place; only the gap is written just before the
            # reader, so every crossed label addresses the nop.
            pieces.append(assembly[end:reader_offset])
            eol = b"\r\n" if assembly[:reader_offset].endswith(b"\r\n") else b"\n"
            gap = b"\tnop" + eol
            if not noreorder:
                gap = b"\t.set\tnoreorder" + eol + gap + b"\t.set\treorder" + eol
            pieces.append(gap)
            end = reader_offset
    pieces.append(assembly[end:])
    return b"".join(pieces)
