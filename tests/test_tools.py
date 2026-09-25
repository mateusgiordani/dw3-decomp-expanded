"""Unit tests for the verification tools. No game data, compiler or assembler is needed.

Run with ``python -m unittest discover tests``.
"""

from __future__ import annotations

import json
import re
import struct
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import card_certificate  # noqa: E402
import card_verify  # noqa: E402
import div_guard  # noqa: E402
import elf_link  # noqa: E402
import recipe_headers  # noqa: E402


def build_elf(text: bytes, relocs: list[tuple[int, int, int]], symbols: list[tuple[str, int, int]],
              rodata: bytes | None = None) -> bytes:
    """A 32-bit little-endian REL object: .text, optional .rodata, .rel.text and a symtab.

    ``symbols`` are (name, value, section index); the null symbol is added first.
    ``relocs`` are (offset, type, symbol index) applied to .text.
    """
    names = [".text", ".rel.text", ".symtab", ".strtab", ".shstrtab"] + ([".rodata"] if rodata else [])
    shstr, name_off = b"\0", {}
    for name in names:
        name_off[name] = len(shstr)
        shstr += name.encode() + b"\0"
    strtab, symtab = b"\0", b"\0" * 16
    for name, value, shndx in symbols:
        symtab += struct.pack("<IIIBBH", len(strtab), value, 0, 0x10, 0, shndx)
        strtab += name.encode() + b"\0"
    rel = b"".join(struct.pack("<II", off, (idx << 8) | kind) for off, kind, idx in relocs)
    # section indices: 1 .text, 2 .rel.text, 3 .symtab, 4 .strtab, 5 .shstrtab, 6 .rodata
    blobs = [text, rel, symtab, strtab, shstr] + ([rodata] if rodata else [])
    body, offsets = b"", []
    for blob in blobs:
        offsets.append(52 + len(body))
        body += blob + b"\0" * (-len(blob) % 4)
    header = b"\x7fELF" + bytes([1, 1, 1]) + b"\0" * 9
    header += struct.pack("<HHIIIIIHHHHHH", 1, 8, 1, 0, 0, 52 + len(body), 0, 52, 0, 0, 40, len(blobs) + 1, 5)
    types, links, infos = [1, 9, 2, 3, 3, 1], [0, 3, 4, 0, 0, 0], [0, 1, 1, 0, 0, 0]
    shdrs = b"\0" * 40
    for i, blob in enumerate(blobs):
        shdrs += struct.pack("<IIIIIIIIII", name_off[names[i]], types[i], 0, 0, offsets[i], len(blob),
                             links[i], infos[i], 4, 16 if types[i] == 2 else 0)
    return header + body + shdrs


def words(*values: int) -> bytes:
    return b"".join(struct.pack("<I", v) for v in values)


class ElfLinkTests(unittest.TestCase):
    def link(self, obj: bytes, symbols: dict, rodata: int | None = None):
        return elf_link.link_elf(elf_link.read_elf(obj), 0x80090000, rodata, symbols)

    def test_hi16_lo16_pair_carries_the_sign_of_the_low_half(self):
        obj = build_elf(words(0x3C020000, 0x24420000),  # lui v0,0 ; addiu v0,v0,0
                        [(0, elf_link.R_MIPS_HI16, 1), (4, elf_link.R_MIPS_LO16, 1)], [("D", 0, 0)])
        text, _ = self.link(obj, {"D": 0x800A8000})
        self.assertEqual(struct.unpack("<II", text), (0x3C02800B, 0x24428000))

    def test_r_mips_26_external_and_local_targets(self):
        obj = build_elf(words(0x0C000000, 0, 0x0C000000),
                        [(0, elf_link.R_MIPS_26, 1), (8, elf_link.R_MIPS_26, 2)],
                        [("callee", 0, 0), ("local", 4, 1)])
        first, _, second = struct.unpack("<III", self.link(obj, {"callee": 0x80082CB0})[0])
        self.assertEqual(first, 0x0C000000 | (0x80082CB0 >> 2 & 0x03FFFFFF))
        self.assertEqual(second, 0x0C000000 | (0x80090004 >> 2 & 0x03FFFFFF))

    def test_r_mips_32_adds_the_addend(self):
        obj = build_elf(words(8), [(0, elf_link.R_MIPS_32, 1)], [("table", 0, 0)])
        self.assertEqual(struct.unpack("<I", self.link(obj, {"table": 0x80083000})[0])[0], 0x80083008)

    def test_unresolved_symbol_is_an_error(self):
        obj = build_elf(words(0x0C000000), [(0, elf_link.R_MIPS_26, 1)], [("missing", 0, 0)])
        with self.assertRaisesRegex(ValueError, "unresolved symbol 'missing'"):
            self.link(obj, {})

    def test_unknown_relocation_is_an_error(self):
        obj = build_elf(words(0), [(0, 7, 1)], [("gp_thing", 0, 0)])  # R_MIPS_GPREL16
        with self.assertRaisesRegex(ValueError, "unsupported relocation type 7"):
            self.link(obj, {"gp_thing": 0x80090000})

    def test_jump_table_needs_a_rodata_address(self):
        with self.assertRaisesRegex(ValueError, "no rodata address"):
            self.link(build_elf(words(0), [], [], rodata=words(0, 0)), {})

    def test_big_endian_is_rejected(self):
        obj = bytearray(build_elf(words(0), [], []))
        obj[5] = 2
        with self.assertRaisesRegex(ValueError, "little-endian"):
            elf_link.read_elf(bytes(obj))


class DivGuardTests(unittest.TestCase):
    def test_three_operand_div_becomes_bare_div_and_mflo(self):
        out = div_guard.strip_div_guard(b"\tdiv\t$2,$4,$5\n\tsw\t$2,0($6)\n").replace(b"\r\n", b"\n")
        self.assertIn(b"\tdiv\t$4,$5\n\tmflo\t$2\n\tnop\n", out)
        self.assertIn(b".set\tnomacro", out)

    def test_no_gap_when_the_next_instruction_does_not_read_the_result(self):
        out = div_guard.strip_div_guard(b"\tdivu\t$3,$4,$5\n\taddu\t$7,$8,$9\n").replace(b"\r\n", b"\n")
        self.assertIn(b"\tdivu\t$4,$5\n\tmflo\t$3\n\t.set\tmacro", out)

    def test_code_without_division_is_unchanged(self):
        source = b"\taddu\t$2,$3,$4\n\tjr\t$31\n"
        self.assertEqual(div_guard.strip_div_guard(source), source)


class RecipeHeaderTests(unittest.TestCase):
    def setUp(self):
        self.data, self.toolchain, self.open_cfg = recipe_headers.load()

    def recipe(self, **changes) -> dict:
        return {**self.data["functions"][0], **changes}

    def test_header_states_every_input_of_the_match(self):
        recipe = self.recipe(rodata="0x8008326c", symbols={"D_1": "0x80010000", "F_2": "0x80020000"},
                             variant="o2-g0-no-strength-reduce")
        header = recipe_headers.render_recipe(recipe, self.toolchain, self.open_cfg)
        for needed in ("CC1PSX 2.8.1 SN32 BUILD 4.0.0010", "ASPSX 2.79", "gcc-2.8.1-psx", "maspsx",
                       "-fno-strength-reduce", "zero-divisor guard", "jump table (.rodata) at 0x8008326c",
                       "D_1=0x80010000", "F_2=0x80020000", "card_verify.py --only"):
            self.assertIn(needed, header)
        self.assertTrue(all(len(line) <= recipe_headers.WIDTH for line in header.splitlines()))

    def test_private_references_are_dropped_from_notes(self):
        raw = ("/* CARDGAME:0x80084090 (size 64)\n"
               " * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0\n"
               " * Evidence: reports/handoffs/x.md.\n"
               " * The goto keeps beq and bltz separate. Measured by the coordinator.\n"
               " */")
        notes = recipe_headers.sanitize_notes(raw)
        self.assertEqual(notes, ["The goto keeps beq and bltz separate."])

    def test_rebuild_is_idempotent_and_drops_a_byte_order_mark(self):
        recipe = self.recipe()
        source = "﻿// CARDGAME:0x80084090 (size 64)\n// Callers: one jal.\n#include <x.h>\nint f(void);\n"
        once = recipe_headers.rebuild(recipe, source, self.toolchain, self.open_cfg)
        self.assertNotIn("﻿", once)
        self.assertIn("Callers: one jal.", once)
        self.assertTrue(once.endswith("#include <x.h>\nint f(void);\n"))
        self.assertEqual(recipe_headers.rebuild(recipe, once, self.toolchain, self.open_cfg), once)

    def test_every_source_carries_its_current_recipe(self):
        for recipe in self.data["functions"]:
            text = (ROOT / recipe["source"]).read_text(encoding="utf-8").replace("\r\n", "\n")
            with self.subTest(source=recipe["source"]):
                self.assertEqual(recipe_headers.rebuild(recipe, text, self.toolchain, self.open_cfg), text)
                self.assertEqual(recipe_headers.lf_sha256(text.encode()), recipe["source_sha256"])


class VerifyReportTests(unittest.TestCase):
    @staticmethod
    def report(toolchain: str, *results: tuple[str, str, str]) -> dict:
        return {"toolchain": toolchain, "results": [
            {"function": f, "status": s, "candidate_sha256": h} for f, s, h in results]}

    def test_toolchains_agree_on_status_and_bytes(self):
        a = self.report("open", ("CARDGAME:0x1", "exact_byte_match", "aa"))
        self.assertEqual(card_verify.disagreements(a, self.report("psyq", ("CARDGAME:0x1", "exact_byte_match", "aa"))), [])
        self.assertEqual(len(card_verify.disagreements(a, self.report("psyq", ("CARDGAME:0x1", "exact_byte_match", "bb")))), 1)
        self.assertEqual(len(card_verify.disagreements(a, self.report("psyq", ("CARDGAME:0x1", "mismatch", "aa")))), 1)
        self.assertEqual(len(card_verify.disagreements(a, self.report("psyq"))), 1)

    def test_both_writes_one_report_per_toolchain(self):
        out = Path("build/verify.json")
        self.assertEqual(card_verify.output_path(out, "psyq", True), Path("build/verify-psyq.json"))
        self.assertEqual(card_verify.output_path(out, "open", False), out)

    def test_certificate_digest_ignores_result_order(self):
        a = self.report("open", ("B", "exact_byte_match", "2"), ("A", "exact_byte_match", "1"))
        b = self.report("open", ("A", "exact_byte_match", "1"), ("B", "exact_byte_match", "2"))
        self.assertEqual(card_certificate.digest_of(a), card_certificate.digest_of(b))


class RepositoryTests(unittest.TestCase):
    def test_inventory_and_recipes_agree(self):
        recipes = json.loads((ROOT / "recipes/card_cage.json").read_text(encoding="utf-8"))["functions"]
        inventory = (ROOT / "symbols/card_cage_functions.csv").read_text(encoding="utf-8").splitlines()[1:]
        self.assertEqual(len(recipes), 318)
        self.assertEqual(len(inventory), 318)
        hashes = {line.split(",")[5]: line.split(",")[6] for line in inventory}
        for recipe in recipes:
            self.assertEqual(hashes[recipe["source"]], recipe["source_sha256"])

    def test_no_game_or_tool_binaries_are_tracked(self):
        import subprocess
        tracked = subprocess.run(["git", "-C", str(ROOT), "ls-files"], capture_output=True, text=True).stdout
        banned = re.compile(r"\.(pro|bin|iso|cue|img|chd|exe|obj|o|zip|7z)$", re.IGNORECASE)
        self.assertEqual([p for p in tracked.splitlines() if banned.search(p)], [])


if __name__ == "__main__":
    unittest.main()
