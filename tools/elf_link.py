"""Minimal ELF reader and single-function linker for maspsx/GNU as objects.

Places ``.text`` at the function's PAL address and ``.rodata`` (jump table) at
its PAL address, resolves external symbols from the recipe, applies the MIPS
REL relocations GNU ``as`` emits for this code (``R_MIPS_32``, ``R_MIPS_26``,
``R_MIPS_HI16``/``R_MIPS_LO16``), and returns the linked bytes. Any other
relocation or an unresolved symbol is an error, never a silent pass.
"""

from __future__ import annotations

import struct


R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6


def read_elf(data: bytes) -> dict:
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        raise ValueError("expected 32-bit little-endian ELF")
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
    sections = []
    for i in range(shnum):
        name, typ, flags, addr, off, size, link, info, align, entsize = struct.unpack_from(
            "<IIIIIIIIII", data, shoff + i * shentsize)
        sections.append(dict(name=name, type=typ, off=off, size=size, link=link, info=info, entsize=entsize))
    strtab = sections[shstrndx]
    for sec in sections:
        end = data.index(b"\0", strtab["off"] + sec["name"])
        sec["name"] = data[strtab["off"] + sec["name"]:end].decode()
        sec["data"] = bytearray(data[sec["off"]:sec["off"] + sec["size"]]) if sec["type"] != 8 else bytearray(sec["size"])
    symtab = next(s for s in sections if s["type"] == 2)
    symstr = sections[symtab["link"]]
    symbols = []
    for i in range(symtab["size"] // 16):
        name, value, size, info, other, shndx = struct.unpack_from("<IIIBBH", data, symtab["off"] + i * 16)
        end = data.index(b"\0", symstr["off"] + name)
        symbols.append(dict(name=data[symstr["off"] + name:end].decode(), value=value, shndx=shndx))
    relocs = {}
    for sec in sections:
        if sec["type"] == 9:  # SHT_REL
            entries = []
            for i in range(sec["size"] // 8):
                offset, info = struct.unpack_from("<II", data, sec["off"] + i * 8)
                entries.append((offset, info & 0xFF, info >> 8))
            relocs[sections[sec["info"]]["name"]] = entries
    return dict(sections=sections, symbols=symbols, relocs=relocs)


def link_elf(elf: dict, address: int, rodata: int | None, symbols: dict[str, int]) -> tuple[bytes, bytes]:
    by_name = {s["name"]: s for s in elf["sections"]}
    text = by_name[".text"]
    ro = by_name.get(".rodata") or by_name.get(".rdata")
    placed = {elf["sections"].index(text): address}
    if ro is not None and ro["size"]:
        if rodata is None:
            raise ValueError("object has a jump table but the recipe has no rodata address")
        placed[elf["sections"].index(ro)] = rodata

    def resolve(index: int) -> int:
        sym = elf["symbols"][index]
        if sym["shndx"] in placed:
            return placed[sym["shndx"]] + sym["value"]
        if sym["name"] in symbols:
            return symbols[sym["name"]]
        raise ValueError(f"unresolved symbol {sym['name']!r}")

    for sec_name in (".text", ".rodata", ".rdata"):
        if sec_name not in by_name or sec_name not in elf["relocs"]:
            continue
        buf = by_name[sec_name]["data"]
        base = placed[elf["sections"].index(by_name[sec_name])]
        pending_hi = []
        for offset, kind, index in elf["relocs"][sec_name]:
            word, = struct.unpack_from("<I", buf, offset)
            target = resolve(index)
            if kind == R_MIPS_32:
                word = (word + target) & 0xFFFFFFFF
            elif kind == R_MIPS_26:
                addend = (word & 0x03FFFFFF) << 2
                word = (word & 0xFC000000) | (((addend + target) >> 2) & 0x03FFFFFF)
            elif kind == R_MIPS_HI16:
                pending_hi.append((offset, word, target))
                continue
            elif kind == R_MIPS_LO16:
                lo = word & 0xFFFF
                lo_signed = lo - 0x10000 if lo & 0x8000 else lo
                for hi_off, hi_word, hi_target in pending_hi:
                    full = ((hi_word & 0xFFFF) << 16) + lo_signed + hi_target
                    hi = ((full + 0x8000) >> 16) & 0xFFFF
                    struct.pack_into("<I", buf, hi_off, (hi_word & 0xFFFF0000) | hi)
                pending_hi.clear()
                word = (word & 0xFFFF0000) | ((lo_signed + target) & 0xFFFF)
            else:
                raise ValueError(f"unsupported relocation type {kind} in {sec_name}")
            struct.pack_into("<I", buf, offset, word)
        if pending_hi:
            raise ValueError("HI16 without LO16")
    return bytes(text["data"]), bytes(ro["data"]) if ro is not None else b""
