"""Link one PsyQ object section and verify it against reference bytes.

The parser supports the LNK v2 records and MIPS relocation kinds emitted by
ASPSX for ordinary C functions.  Every section and external symbol used by a
relocation must have an explicit address; unresolved inputs fail closed.

Jump-table data (`.rdata`/`.rodata`) is supported only with an explicitly
recorded `rodata_address`: the address is project evidence (read from the PAL
reference), never invented by the linker.  Without it, non-empty data
sections keep failing exactly as before.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import sys
from dataclasses import dataclass
from pathlib import Path


class ObjectFormatError(ValueError):
    pass


class SizeMismatchError(ValueError):
    """Linked .text length disagrees with the canonical PAL range."""

    def __init__(
        self,
        message: str,
        *,
        candidate: bytes,
        reference: bytes,
        body_size: int,
        expected_size: int,
    ) -> None:
        super().__init__(message)
        self.candidate = candidate
        self.reference = reference
        self.body_size = body_size
        self.expected_size = expected_size


@dataclass(frozen=True)
class Expression:
    opcode: int
    value: int | None = None
    left: "Expression | None" = None
    right: "Expression | None" = None


@dataclass(frozen=True)
class Relocation:
    section: int
    kind: int
    offset: int
    expression: Expression


def _u16(data: bytes, offset: int) -> tuple[int, int]:
    if offset + 2 > len(data):
        raise ObjectFormatError("truncated u16")
    return struct.unpack_from("<H", data, offset)[0], offset + 2


def _u32(data: bytes, offset: int) -> tuple[int, int]:
    if offset + 4 > len(data):
        raise ObjectFormatError("truncated u32")
    return struct.unpack_from("<I", data, offset)[0], offset + 4


def _pascal(data: bytes, offset: int) -> tuple[bytes, int]:
    if offset >= len(data):
        raise ObjectFormatError("truncated string length")
    size = data[offset]
    offset += 1
    end = offset + size
    if end > len(data):
        raise ObjectFormatError("truncated string")
    return data[offset:end], end


def _expression(data: bytes, offset: int) -> tuple[Expression, int]:
    if offset >= len(data):
        raise ObjectFormatError("truncated relocation expression")
    opcode = data[offset]
    offset += 1
    if opcode == 0x00:  # literal value
        value, offset = _u32(data, offset)
        return Expression(opcode, value=value), offset
    if opcode in (0x02, 0x04, 0x0C, 0x16):
        value, offset = _u16(data, offset)
        return Expression(opcode, value=value), offset
    if opcode in (0x2C, 0x2E, 0x32, 0x36):
        left, offset = _expression(data, offset)
        right, offset = _expression(data, offset)
        return Expression(opcode, left=left, right=right), offset
    raise ObjectFormatError(f"unsupported relocation expression 0x{opcode:02x}")


def _parse_object(data: bytes) -> tuple[
    dict[int, bytes], dict[int, bytes], dict[int, tuple[int, int]],
    dict[int, bytes], list[Relocation]
]:
    if not data.startswith(b"LNK\x02"):
        raise ObjectFormatError("expected PsyQ LNK version 2 object")

    offset = 4
    current_section: int | None = None
    section_names: dict[int, bytes] = {}
    section_data: dict[int, bytearray] = {}
    # LNK relocation patch positions are relative to the most recent BYTES
    # record in that section. Symbol values and expressions stay section-relative.
    chunk_bases: dict[int, int] = {}
    definitions: dict[int, tuple[int, int]] = {}
    references: dict[int, bytes] = {}
    relocations: list[Relocation] = []
    debug_positions: list[tuple[int, int]] = []

    while offset < len(data):
        record = data[offset]
        offset += 1
        if record == 0x00:
            break
        if record == 0x2E:
            offset += 1
        elif record == 0x10:
            section, offset = _u16(data, offset)
            _, offset = _u16(data, offset)
            if offset >= len(data):
                raise ObjectFormatError("truncated section alignment")
            offset += 1
            name, offset = _pascal(data, offset)
            section_names[section] = name
            section_data.setdefault(section, bytearray())
            chunk_bases.setdefault(section, 0)
        elif record == 0x1C:
            _, offset = _u16(data, offset)
            _, offset = _pascal(data, offset)
        elif record == 0x06:
            current_section, offset = _u16(data, offset)
            if current_section not in section_names:
                raise ObjectFormatError("switch to undeclared section")
        elif record == 0x02:
            if current_section is None:
                raise ObjectFormatError("data record without current section")
            size, offset = _u16(data, offset)
            end = offset + size
            if end > len(data):
                raise ObjectFormatError("truncated code record")
            chunk_bases[current_section] = len(section_data[current_section])
            section_data[current_section].extend(data[offset:end])
            offset = end
        elif record == 0x08:  # ZEROES; does not replace the last BYTES base
            if current_section is None:
                raise ObjectFormatError("BSS record without current section")
            size, offset = _u32(data, offset)
            section_data[current_section].extend(bytes(size))
        elif record == 0x0A:
            if current_section is None:
                raise ObjectFormatError("relocation without current section")
            if offset >= len(data):
                raise ObjectFormatError("truncated relocation kind")
            kind = data[offset]
            offset += 1
            patch_offset, offset = _u16(data, offset)
            expression, offset = _expression(data, offset)
            relocations.append(
                Relocation(
                    current_section, kind, chunk_bases[current_section] + patch_offset, expression
                )
            )
        elif record in (0x0C, 0x30):
            symbol, offset = _u16(data, offset)
            section, offset = _u16(data, offset)
            value, offset = _u32(data, offset)
            _, offset = _pascal(data, offset)
            definitions[symbol] = (section, value)
        elif record in (0x4A, 0x4C):
            # PsyQ FUNCTION / FUNCTION_END debug metadata, also emitted by
            # ASPSX 2.81 for .ent/.frame/.mask/.end without -g. These are
            # records, distinct from relocation kind 0x4A. Layout reference:
            # pcsx-redux 0de8b9c5, tools/psyq-obj-parser/psyq-obj-parser.cc.
            layout = "<HIHIHIHII" if record == 0x4A else "<HII"
            size = struct.calcsize(layout)
            if offset + size > len(data):
                raise ObjectFormatError(f"truncated LNK record 0x{record:02x}")
            fields = struct.unpack_from(layout, data, offset)
            offset += size
            debug_positions.append((fields[0], fields[1]))
            if record == 0x4A:
                _, offset = _pascal(data, offset)
        elif record == 0x3C:
            # aspsx-2.81 trailer seen exactly once per object, always zero
            # in every locally generated shape; anything else fails loudly
            # rather than risk a stream desync.
            if len(data) < offset + 2:
                raise ObjectFormatError("truncated LNK record 0x3c")
            (payload,) = struct.unpack_from("<H", data, offset)
            if payload != 0:
                raise ObjectFormatError(
                    f"unsupported LNK record 0x3c payload 0x{payload:04x}"
                )
            offset += 2
        elif record == 0x0E:
            symbol, offset = _u16(data, offset)
            name, offset = _pascal(data, offset)
            references[symbol] = name
        elif record == 0x12:
            _, offset = _u16(data, offset)
            _, offset = _u32(data, offset)
            _, offset = _pascal(data, offset)
        else:
            raise ObjectFormatError(f"unsupported LNK record 0x{record:02x}")
        if offset > len(data):
            raise ObjectFormatError("truncated object record")

    # Debug offsets describe section positions, never executable payload.
    # Check after parsing so metadata may precede a later data record.
    for section, position in debug_positions:
        if section not in section_names:
            raise ObjectFormatError("function debug record references undeclared section")
        if position > len(section_data[section]):
            raise ObjectFormatError("function debug offset exceeds section size")

    return (
        section_names,
        {key: bytes(value) for key, value in section_data.items()},
        definitions,
        references,
        relocations,
    )


def _evaluate(
    expression: Expression,
    section_addresses: dict[int, int],
    section_sizes: dict[int, int],
    definitions: dict[int, tuple[int, int]],
    references: dict[int, bytes],
    symbols: dict[str, int],
) -> int:
    opcode = expression.opcode
    value = expression.value
    if opcode == 0x00:
        assert value is not None
        return value
    if opcode == 0x02:
        assert value is not None
        if value in definitions:
            section, relative = definitions[value]
            if section not in section_addresses:
                raise ObjectFormatError(f"no address for section {section}")
            return section_addresses[section] + relative
        if value not in references:
            raise ObjectFormatError(f"unknown symbol id {value}")
        name = references[value].decode("ascii", "strict")
        if name not in symbols:
            raise ObjectFormatError(f"unresolved external symbol {name}")
        return symbols[name]
    if opcode in (0x04, 0x0C):
        assert value is not None
        if value not in section_addresses:
            raise ObjectFormatError(f"no address for section {value}")
        return section_addresses[value]
    if opcode == 0x16:
        assert value is not None
        if value not in section_addresses:
            raise ObjectFormatError(f"no address for section {value}")
        return section_addresses[value] + section_sizes[value]
    assert expression.left is not None and expression.right is not None
    left = _evaluate(
        expression.left, section_addresses, section_sizes,
        definitions, references, symbols
    )
    right = _evaluate(
        expression.right, section_addresses, section_sizes,
        definitions, references, symbols
    )
    if opcode == 0x2C:
        return (left + right) & 0xFFFFFFFF
    if opcode == 0x2E:
        return (left - right) & 0xFFFFFFFF
    if opcode == 0x32:
        if right == 0:
            raise ObjectFormatError("division by zero in relocation expression")
        return (left // right) & 0xFFFFFFFF
    raise ObjectFormatError("EXEC relocation expressions are not linkable")


RODATA_SECTION_NAMES = (b".rdata", b".rodata")


def _link(
    data: bytes,
    text_address: int,
    symbols: dict[str, int] | None,
    rodata_address: int | None,
    gp_address: int | None = None,
    stats: dict[str, int] | None = None,
) -> tuple[bytes, list[tuple[int, bytes]], int]:
    names, sections, definitions, references, relocations = _parse_object(data)
    text_ids = [section for section, name in names.items() if name == b".text"]
    if len(text_ids) != 1:
        raise ObjectFormatError("object must have exactly one .text section")
    text_section = text_ids[0]
    if not sections[text_section]:
        raise ObjectFormatError("object has no .text bytes")

    def _has_content(content: bytes) -> bool:
        # Zero-filled sections (BSS-style) carry no bytes to compare.
        return bool(bytes(content).strip(b"\x00"))
    # Jump tables assemble to zero-filled .rdata whose every word is a
    # relocation placeholder. Content alone cannot distinguish those from
    # BSS, so a zero-filled rodata section that relocations actually target
    # is allocated at the recorded base -- but only when the caller passed
    # an explicit rodata_address (fail-closed otherwise).
    targeted = {relocation.section for relocation in relocations}
    data_ids = [
        section for section, name in names.items()
        if name in RODATA_SECTION_NAMES and (
            _has_content(sections.get(section, b""))
            or (rodata_address is not None and section in targeted)
        )
    ]
    nonempty_other = [
        section for section, content in sections.items()
        if section != text_section
        and section not in data_ids and _has_content(content)
    ]
    if nonempty_other:
        raise ObjectFormatError("object has unsupported non-empty data sections")
    if data_ids and rodata_address is None:
        raise ObjectFormatError(
            "object has unsupported non-empty data sections; "
            "pass an explicit rodata_address recorded from the reference"
        )
    section_addresses = {text_section: text_address}
    data_spans: list[tuple[int, bytes]] = []
    cursor = rodata_address if rodata_address is not None else 0
    for section in data_ids:
        if cursor % 4:
            raise ObjectFormatError("rodata section is not word aligned")
        section_addresses[section] = cursor
        data_spans.append((cursor, bytes(sections[section])))
        cursor += len(sections[section])
    section_sizes = {section: len(content) for section, content in sections.items()}
    linked = bytearray(sections[text_section])
    linked_data = {section: bytearray(content) for section, content in sections.items()}
    resolved_symbols = symbols or {}
    relocation_targets: dict[Relocation, int] = {}

    for relocation in relocations:
        if relocation.section != text_section and relocation.section not in data_ids:
            if _has_content(sections[relocation.section]):
                raise ObjectFormatError("relocation targets unsupported section")
            continue
        target_section = linked if relocation.section == text_section else linked_data[relocation.section]
        patch = relocation.offset
        if patch + 4 > len(target_section):
            raise ObjectFormatError("relocation patch is outside its section")
        target = _evaluate(
            relocation.expression, section_addresses, section_sizes,
            definitions, references, resolved_symbols
        )
        relocation_targets[relocation] = target
        instruction = struct.unpack_from("<I", target_section, patch)[0]
        if relocation.kind == 0x10:
            instruction = target
        elif relocation.kind == 0x4A:
            instruction = (instruction & 0xFC000000) | ((target >> 2) & 0x03FFFFFF)
        elif relocation.kind == 0x52:
            instruction = (instruction & 0xFFFF0000) | (((target + 0x8000) >> 16) & 0xFFFF)
        elif relocation.kind in (0x1A, 0x1E, 0x54):
            instruction = (instruction & 0xFFFF0000) | (target & 0xFFFF)
        else:
            raise ObjectFormatError(
                f"unsupported MIPS relocation 0x{relocation.kind:02x}"
            )
        struct.pack_into("<I", target_section, patch, instruction)
    gp_relaxations = 0
    if gp_address is not None:
        linked, gp_relaxations = _relax_gp_text(
            bytes(linked),
            text_address,
            [item for item in relocations if item.section == text_section],
            relocation_targets,
            gp_address,
        )
    if stats is not None:
        stats["gp_relaxation_count"] = gp_relaxations
    linked_spans = [
        (address, bytes(linked_data[section]))
        for address, section in [
            (section_addresses[section], section) for section in data_ids
        ]
    ]
    return bytes(linked), linked_spans, len(relocations)


BRANCH_OPCODES = {0x01, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17}
GP_RELAX_LOW_KINDS = {0x1A, 0x1E, 0x54}
GP_RELAX_OPCODES = {
    0x09,  # addiu
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,  # loads
    0x28, 0x29, 0x2A, 0x2B, 0x2E,  # stores
}


def _relax_gp_text(
    linked: bytes,
    text_address: int,
    relocations: list[Relocation],
    targets: dict[Relocation, int],
    gp_address: int,
) -> tuple[bytes, int]:
    """Collapse an ASPSX absolute macro into one PAL `$gp` instruction.

    This is deliberately narrower than a normal linker relaxation: the HI16
    and LO16 relocations must be adjacent, reference the identical expression,
    and form ``lui base`` followed by a supported I-type instruction using
    that base. The resolved symbol must fit a signed 16-bit displacement from
    the explicitly supplied `$gp`. Internal branches/jumps are then remapped;
    any control-flow edge into a removed word fails closed.
    """
    by_offset: dict[int, list[Relocation]] = {}
    for relocation in relocations:
        by_offset.setdefault(relocation.offset, []).append(relocation)

    removed: set[int] = set()
    replacements: dict[int, int] = {}
    for high in relocations:
        if high.kind != 0x52 or high.offset + 8 > len(linked):
            continue
        lows = [
            low for low in by_offset.get(high.offset + 4, [])
            if low.kind in GP_RELAX_LOW_KINDS
            and low.expression == high.expression
            and targets.get(low) == targets.get(high)
        ]
        if len(lows) != 1:
            continue
        low = lows[0]
        high_word = struct.unpack_from("<I", linked, high.offset)[0]
        low_word = struct.unpack_from("<I", linked, low.offset)[0]
        high_opcode = (high_word >> 26) & 0x3F
        base_register = (high_word >> 16) & 0x1F
        low_opcode = (low_word >> 26) & 0x3F
        low_base = (low_word >> 21) & 0x1F
        if high_opcode != 0x0F or base_register == 0 or low_base != base_register:
            continue
        if low_opcode not in GP_RELAX_OPCODES:
            continue
        displacement = int(targets[low]) - gp_address
        if not -0x8000 <= displacement <= 0x7FFF:
            continue
        removed.add(high.offset)
        replacements[low.offset] = (
            (low_word & ~(0x1F << 21) & 0xFFFF0000)
            | (28 << 21)
            | (displacement & 0xFFFF)
        )

    if not removed:
        raise ObjectFormatError(
            "explicit gp address produced no unambiguous HI16/LO16 relaxation"
        )

    def remap(offset: int) -> int:
        return offset - 4 * sum(item < offset for item in removed)

    def remap_target(offset: int) -> int:
        if offset in removed:
            if offset + 4 not in replacements:
                raise ObjectFormatError("control flow targets removed gp macro word")
            offset += 4
        return remap(offset)

    output = bytearray()
    for old_offset in range(0, len(linked), 4):
        if old_offset in removed:
            continue
        word = replacements.get(
            old_offset, struct.unpack_from("<I", linked, old_offset)[0]
        )
        opcode = (word >> 26) & 0x3F
        if opcode in BRANCH_OPCODES:
            immediate = word & 0xFFFF
            displacement = immediate - 0x10000 if immediate & 0x8000 else immediate
            target = old_offset + 4 + displacement * 4
            if 0 <= target <= len(linked):
                new_displacement = (
                    remap_target(target) - (remap(old_offset) + 4)
                ) // 4
                if not -0x8000 <= new_displacement <= 0x7FFF:
                    raise ObjectFormatError("relaxed branch displacement overflow")
                word = (word & 0xFFFF0000) | (new_displacement & 0xFFFF)
        elif opcode in (0x02, 0x03):
            pc = text_address + old_offset
            target_address = ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
            target = target_address - text_address
            if 0 <= target <= len(linked):
                word = (word & 0xFC000000) | (
                    ((text_address + remap_target(target)) >> 2) & 0x03FFFFFF
                )
        output.extend(struct.pack("<I", word))
    return bytes(output), len(removed)


def link_text(
    data: bytes,
    text_address: int,
    symbols: dict[str, int] | None = None,
    gp_address: int | None = None,
    stats: dict[str, int] | None = None,
) -> tuple[bytes, int]:
    """Link `.text` only; data sections keep failing exactly as before."""
    linked, spans, count = _link(
        data, text_address, symbols, None, gp_address, stats
    )
    assert not spans
    return linked, count


def link_with_rodata(
    data: bytes,
    text_address: int,
    rodata_address: int,
    symbols: dict[str, int] | None = None,
    gp_address: int | None = None,
    stats: dict[str, int] | None = None,
) -> tuple[bytes, list[tuple[int, bytes]], int]:
    """Link `.text` plus `.rdata`/`.rodata` at an explicitly recorded base."""
    return _link(data, text_address, symbols, rodata_address, gp_address, stats)


def extract_relocation_free_text(data: bytes) -> bytes:
    names, sections, _, _, relocations = _parse_object(data)
    if relocations:
        raise ObjectFormatError(
            "object contains relocations; link-aware comparison is required"
        )
    text_ids = [section for section, name in names.items() if name == b".text"]
    if len(text_ids) != 1:
        raise ObjectFormatError("object must have exactly one .text section")
    text = sections[text_ids[0]]
    if not text:
        raise ObjectFormatError("object has no .text bytes")
    return text


PSX_EXE_MAGIC = b"PS-X EXE"
PSX_EXE_HEADER_SIZE = 0x800


def reference_file_offset(image: bytes, base: int, address: int) -> int:
    """Map a virtual address onto a reference image, skipping a PS-X EXE header.

    Overlay dumps are raw payloads loaded at `base`. The EXE dump is a PS-X
    EXE whose 2048-byte header is followed by the payload at the header load
    address. `reference/` is never modified; the header is skipped in memory.
    """
    payload_base = 0
    if image[:8] == PSX_EXE_MAGIC:
        if len(image) < PSX_EXE_HEADER_SIZE:
            raise ValueError("truncated PS-X EXE")
        load_address = struct.unpack_from("<I", image, 0x18)[0]
        if load_address != base:
            raise ValueError(
                "PS-X EXE load address 0x{:08x} != comparison base 0x{:08x}".format(
                    load_address, base
                )
            )
        payload_base = PSX_EXE_HEADER_SIZE
    if address < base:
        raise ValueError("function address is below module base")
    return payload_base + (address - base)


def linked_image(
    object_path: Path,
    address: int,
    symbols: dict[str, int] | None = None,
    *,
    rodata_address: int | None = None,
    gp_address: int | None = None,
    stats: dict[str, int] | None = None,
) -> tuple[bytes, list[tuple[int, bytes]], int]:
    """Link one object exactly the way ``verify`` does.

    Analysis tools need the same bytes ``verify`` compares, including the
    ``--rodata`` path; without a shared entry point each caller re-implements
    the link and they drift.
    """
    data = object_path.read_bytes()
    if rodata_address is None:
        candidate, relocation_count = link_text(
            data, address, symbols, gp_address, stats
        )
        return candidate, [], relocation_count
    return link_with_rodata(
        data, address, rodata_address, symbols, gp_address, stats
    )


def verify(
    object_path: Path,
    reference_path: Path,
    base: int,
    address: int,
    expected_size: int | None = None,
    symbols: dict[str, int] | None = None,
    *,
    expected_body_size: int | None = None,
    rodata_address: int | None = None,
    expected_rodata_size: int | None = None,
    gp_address: int | None = None,
) -> dict:
    link_stats: dict[str, int] = {}
    candidate, data_spans, relocation_count = linked_image(
        object_path, address, symbols,
        rodata_address=rodata_address, gp_address=gp_address, stats=link_stats,
    )

    body_size = len(candidate)
    range_size = expected_size if expected_size is not None else body_size
    padding_size = 0
    padding_classification = "none"

    reference_image = reference_path.read_bytes()
    offset = reference_file_offset(reference_image, base, address)

    if expected_body_size is not None and expected_size is None:
        raise ValueError("body size requires an expected range size")

    pal_range = b""
    if expected_size is not None:
        pal_end = offset + expected_size
        if pal_end > len(reference_image):
            raise ValueError("function range is outside reference image")
        pal_range = reference_image[offset:pal_end]
        if body_size > expected_size:
            raise SizeMismatchError(
                f"object text size {body_size} > expected size {expected_size}",
                candidate=candidate,
                reference=pal_range,
                body_size=body_size,
                expected_size=expected_size,
            )
        if expected_body_size is None:
            if body_size != expected_size:
                raise SizeMismatchError(
                    f"object text size {body_size} != expected size {expected_size}; "
                    "an explicit body size is required to classify padding",
                    candidate=candidate,
                    reference=pal_range,
                    body_size=body_size,
                    expected_size=expected_size,
                )
        else:
            if expected_body_size <= 0 or expected_body_size % 4:
                raise ValueError("body size must be a positive multiple of 4")
            if expected_body_size > expected_size:
                raise ValueError("body size exceeds expected range size")
            if body_size != expected_body_size:
                raise SizeMismatchError(
                    f"object text size {body_size} != expected body size "
                    f"{expected_body_size}",
                    candidate=candidate,
                    reference=pal_range,
                    body_size=body_size,
                    expected_size=expected_size,
                )

        if body_size < expected_size:
            padding_size = expected_size - body_size
            padding_offset = offset + body_size
            padding_end = offset + expected_size
            if padding_end > len(reference_image):
                raise ValueError("function range is outside reference image")
            padding_bytes = reference_image[padding_offset:padding_end]

            is_nop_padding = len(padding_bytes) % 4 == 0 and all(
                struct.unpack_from("<I", padding_bytes, index)[0] == 0
                for index in range(0, len(padding_bytes), 4)
            )
            padding_classification = (
                "trailing_nop" if is_nop_padding else "unclassified"
            )

            if padding_classification == "unclassified":
                raise SizeMismatchError(
                    f"object text size {body_size} != expected size {expected_size} "
                    f"and trailing bytes are not recognized padding",
                    candidate=candidate,
                    reference=pal_range,
                    body_size=body_size,
                    expected_size=expected_size,
                )

    end = offset + body_size
    if end > len(reference_image):
        raise ValueError("function range is outside reference image")
    reference = reference_image[offset:end]
    differences = [
        index
        for index, (candidate_byte, reference_byte) in enumerate(
            zip(candidate, reference)
        )
        if candidate_byte != reference_byte
    ]
    rodata_spans: list[dict] = []
    rodata_difference_count = 0
    for span_address, span_bytes in data_spans:
        if expected_rodata_size is not None and len(span_bytes) != expected_rodata_size:
            raise ValueError(
                f"linked rodata size {len(span_bytes)} != expected rodata size "
                f"{expected_rodata_size}"
            )
        span_offset = reference_file_offset(reference_image, base, span_address)
        span_end = span_offset + len(span_bytes)
        if span_end > len(reference_image):
            raise ValueError("rodata range is outside reference image")
        span_reference = reference_image[span_offset:span_end]
        span_diffs = sum(
            1 for candidate_byte, reference_byte in zip(span_bytes, span_reference)
            if candidate_byte != reference_byte
        )
        rodata_difference_count += span_diffs
        rodata_spans.append({
            "address": f"0x{span_address:08x}",
            "size": len(span_bytes),
            "candidate_sha256": hashlib.sha256(span_bytes).hexdigest(),
            "reference_sha256": hashlib.sha256(span_reference).hexdigest(),
            "difference_count": span_diffs,
        })
    exact = not differences and not rodata_difference_count
    return {
        "schema_version": 1,
        "status": "exact_byte_match" if exact else "mismatch",
        "object": str(object_path),
        "reference": str(reference_path),
        "base": f"0x{base:08x}",
        "address": f"0x{address:08x}",
        "size": body_size,
        "body_size": body_size,
        "range_size": range_size,
        "padding_size": padding_size,
        "padding_classification": padding_classification,
        "relocation_count": relocation_count,
        "gp_address": f"0x{gp_address:08x}" if gp_address is not None else None,
        "gp_relaxation_count": link_stats.get("gp_relaxation_count", 0),
        "candidate_sha256": hashlib.sha256(candidate).hexdigest(),
        "reference_sha256": hashlib.sha256(reference).hexdigest(),
        "difference_count": len(differences),
        "first_difference_offsets": differences[:16],
        "rodata_address": f"0x{rodata_address:08x}" if rodata_address is not None else None,
        "rodata_spans": rodata_spans,
        "rodata_difference_count": rodata_difference_count,
    }


def integer(value: str) -> int:
    return int(value, 0)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--obj", required=True, type=Path)
    parser.add_argument("--reference", required=True, type=Path)
    parser.add_argument("--base", required=True, type=integer)
    parser.add_argument("--address", required=True, type=integer)
    parser.add_argument("--size", type=integer)
    parser.add_argument(
        "--body-size",
        type=integer,
        help="explicit logical body size; required when --size includes padding",
    )
    parser.add_argument(
        "--symbol", action="append", default=[], metavar="NAME=ADDRESS",
        help="resolve one external symbol (repeatable)",
    )
    parser.add_argument(
        "--rodata", type=integer, default=None,
        help="explicit recorded base address for .rdata/.rodata jump tables",
    )
    parser.add_argument(
        "--rodata-size", type=integer, default=None,
        help="expected linked rodata size in bytes",
    )
    parser.add_argument(
        "--gp", type=integer, default=None,
        help="explicit PAL $gp address for fail-closed HI16/LO16 relaxation",
    )
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()

    try:
        symbols: dict[str, int] = {}
        for definition in args.symbol:
            if "=" not in definition:
                raise ValueError(f"invalid symbol definition: {definition}")
            name, value = definition.split("=", 1)
            if not name or name in symbols:
                raise ValueError(f"invalid or duplicate symbol: {name}")
            symbols[name] = integer(value)
        report = verify(
            args.obj,
            args.reference,
            args.base,
            args.address,
            args.size,
            symbols,
            expected_body_size=args.body_size,
            rodata_address=args.rodata,
            expected_rodata_size=args.rodata_size,
            gp_address=args.gp,
        )
    except (OSError, ValueError) as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2

    encoded = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(encoded, encoding="utf-8")
    print(encoded, end="")
    return 0 if report["status"] == "exact_byte_match" else 1


if __name__ == "__main__":
    sys.exit(main())
