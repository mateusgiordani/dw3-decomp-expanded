"""Write or check the byte-match recipe header at the top of every C file.

Each source in ``src/`` starts with a comment generated from its entry in
``recipes/card_cage.json``: identity, compiler and flags for both toolchains,
assembler, the zero-divisor guard rule, link addresses, symbols and the command
that verifies it. The recipe file stays the machine-readable authority; the
header is its readable copy, and ``--check`` fails when the two drift.

Below the recipe comes an optional ``Recovery notes`` comment: observations
kept from the recovery work (callers, data layout, why an odd C form is needed
for the match). They are historical, not verified claims.

A freshly exported source has no recipe header yet. ``--write`` then turns its
leading comments into recovery notes, drops what only made sense in the private
working repository (paths, task bookkeeping, tool status lines that the recipe
now states), and inserts the recipe. Comments are removed by the preprocessor,
so none of this changes the compiled bytes; ``tools/card_verify.py`` proves it.

``--write`` also refreshes ``source_sha256`` in ``recipes/card_cage.json`` and
``symbols/card_cage_functions.csv`` (LF form).

Usage:
    python tools/recipe_headers.py --check
    python tools/recipe_headers.py --write
    python tools/recipe_headers.py --show CARDGAME:0x80084090
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import io
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RECIPES = ROOT / "recipes" / "card_cage.json"
INVENTORY = ROOT / "symbols" / "card_cage_functions.csv"
MARK = " * Byte-match recipe"
NOTES_TITLE = " * Recovery notes"
WIDTH = 80

# Labeled note items restated by the generated recipe or only meaningful in the
# private working repository.
DROP_LABELS = {
    "pal", "toolchain", "toolchain base", "status", "boundary", "evidence", "domain pack",
    "pipeline", "result", "word check", "match", "size",
    "reference", "recipe", "compiler", "flags", "measured", "measurement", "verification",
    "validated", "validation", "provenance", "claim", "task", "handoff", "baseline",
    "matching recipe", "recipe used", "command",
}
# Any sentence mentioning one of these is dropped from the notes.
PRIVATE = re.compile(
    r"reports/|docs/|symbols/|build/|tools/|asm/|decomp/|reference/|upstream|worktree|"
    r"\.json\b|\.md\b|\.s\b|markisha|ddw3|domain pack|handoff|ledger|manifest|claim|coordinator|"
    r"fn_exact|card_retest|batch_audit|work_packet|agent|dispatcher|queue|pocketbase|"
    r"claude|codex|gpt-|gemini|glm|opus|luna|sonnet|muse|qwen|task\b|C_MATCHING|C_NONMATCHING|"
    r"ASM_MATCHING|exact_byte_match|difference_count|inventory|state changed|symbols added|read-only",
    re.IGNORECASE,
)
IDENTITY = re.compile(
    r"^(CARDGAME|STCRDABM|STCRDDEK|STCRDSHP)[:_ ]*(0x)?[0-9a-fA-F]{8}\b"
    r"(\s*\([^)]*\)|,?\s*(size\s*)?(0x[0-9a-fA-F]+|\d+)\s*(PAL\s*)?(bytes|B)\b)?[\s,.:;-]*",
)


def lf_sha256(data: bytes) -> str:
    return hashlib.sha256(data.replace(b"\r\n", b"\n")).hexdigest()


# ------------------------------------------------------------------ recipe block

def wrap(prefix: str, text: str, indent: str) -> list[str]:
    """Wrap ``text`` after ``prefix``; continuation lines start with ``indent``."""
    lines, current = [], prefix
    for word in text.split():
        candidate = f"{current}{word}" if current.endswith(" ") or current == prefix else f"{current} {word}"
        if len(candidate) > WIDTH and current.strip() not in ("", "*") and current != prefix:
            lines.append(current.rstrip())
            current = f"{indent}{word}"
        else:
            current = candidate if current != prefix else f"{prefix}{word}"
    lines.append(current.rstrip())
    return lines


def render_recipe(recipe: dict, toolchain: dict, open_cfg: dict) -> str:
    pinned = {t["id"]: t for kind in toolchain["tools"].values() for t in kind}
    cc1psx, aspsx = pinned[recipe["cc1psx"]], pinned[recipe["aspsx"]]
    base = [f for f in toolchain["cc1_flags"] if not (f.startswith("-O") and len(f) == 3)]
    base.insert(1 if base and base[0] == "-quiet" else 0, f"-{recipe['opt_level']}")
    flags = " ".join(base + list(toolchain["variants"][recipe["variant"]]))
    address, module = int(recipe["address"], 16), recipe["module"]
    offset = address - int(recipe["base"], 16)
    hang = " *              "
    out = [
        "/*",
        f" * {module}:{recipe['address']} {recipe['name']}",
        f" * {recipe['size']} bytes at {recipe['overlay_file']} offset 0x{offset:x}"
        f" (overlay loaded at {recipe['base']}).",
        " *",
        f"{MARK} (generated from recipes/card_cage.json by",
        " * tools/recipe_headers.py). Compiling this file as below reproduces the PAL",
        " * bytes of the function.",
        " *",
        " *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include",
    ]
    out += wrap(" *  Compile     ", f"cc1 {flags}", hang)
    out += [
        f" *  Variant     {recipe['variant']}",
        " *  Toolchain A (public, default)",
        f" *    cc1       {open_cfg['gcc']['id']} ({open_cfg['gcc']['project'].split(' (')[0]})",
    ]
    out += wrap(f" *    assemble  ", f"maspsx {open_cfg['maspsx']['revision'][:7]} "
                + " ".join(open_cfg["maspsx"]["args"]) + ", then " + open_cfg["gnu_as"]["command"] + " "
                + " ".join(open_cfg["gnu_as"]["flags"]), " *              ")
    out += [
        " *  Toolchain B (original PsyQ, optional)",
        f" *    cc1       CC1PSX {cc1psx['version']}",
    ]
    aspsx_flags = " ".join(recipe["aspsx_flags"])
    out += wrap(" *    assemble  ", f"ASPSX {aspsx['version']}" + (f" {aspsx_flags}" if aspsx_flags else "")
                + ", after removing the zero-divisor guard (tools/div_guard.py); the overlays"
                " have none and ASPSX would insert one after every division.", " *              ")
    link = f".text at {recipe['address']}"
    if recipe.get("rodata"):
        link += f", jump table (.rodata) at {recipe['rodata']}"
    if recipe.get("gp"):
        link += f", $gp = {recipe['gp']}"
    out.append(f" *  Link        {link}")
    symbols = [f"{name}={value}" for name, value in sorted(recipe["symbols"].items())]
    if symbols:
        out += wrap(" *  Symbols     ", " ".join(symbols), " *              ")
    else:
        out.append(" *  Symbols     (none)")
    out += wrap(" *  Compare     ", f"{recipe['size']} bytes from {recipe['address']}"
                + (" and the jump table" if recipe.get("rodata") else "") + " against the PAL overlay", hang)
    out += [
        f" *  Verify      python tools/card_verify.py --only {module}:{recipe['address']}",
        " */",
    ]
    return "\n".join(out) + "\n"


# ------------------------------------------------------------------ splitting

LEADING = re.compile(r"\A(?:[ \t]*(?://[^\n]*|/\*.*?\*/)[ \t]*\n|[ \t]*\n)*", re.S)
COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


def split_source(text: str) -> tuple[str | None, str | None, str]:
    """Return (recipe block, notes block, rest) of a source already carrying a header,
    or (None, raw leading comments, rest) of a fresh export."""
    lead = LEADING.match(text).group(0)
    rest = text[len(lead):]
    comments = COMMENT.findall(lead)
    if comments and MARK in comments[0]:
        notes = comments[1] if len(comments) > 1 and NOTES_TITLE in comments[1] else None
        return comments[0] + "\n", (notes + "\n") if notes else None, rest
    return None, "\n".join(comments), rest


def comment_lines(raw: str) -> list[str]:
    lines = []
    for chunk in COMMENT.findall(raw):
        if chunk.startswith("//"):
            lines.append(chunk[2:])
            continue
        body = chunk[2:-2]
        for line in body.split("\n"):
            lines.append(re.sub(r"^\s*\*(?!/)", "", line))
    return lines


LABEL = re.compile(r"^([A-Za-z][A-Za-z0-9 /()+.,-]{0,44}?)\s*:\s")


def sanitize_notes(raw: str) -> list[str]:
    """Turn exported leading comments into publishable note paragraphs."""
    items: list[tuple[str | None, list[str]]] = []
    item_indent = 0
    for line in comment_lines(raw):
        stripped = line.strip()
        if not stripped:
            items.append((None, []))
            continue
        indent = len(line) - len(line.lstrip())
        open_item = bool(items and items[-1][1])
        label = LABEL.match(stripped)
        # A line continues the current item when it is indented under it, or when
        # it carries on a wrapped sentence (lower case or punctuation first, and
        # the previous line did not end one).
        previous = items[-1][1][-1] if open_item else ""
        wrapped = open_item and not previous.endswith(".") and (
            re.match(r"^[a-z0-9(\[{*/+=<>&|~^%,;-]", stripped) is not None
            or re.search(r"(?:\b[a-z]{2,}|[,(=+\-/&|])$", previous) is not None
        )
        if label and wrapped and not stripped[:1].isupper():
            label = None
        if open_item and (indent > item_indent + 1 or (wrapped and not label)):
            items[-1][1].append(stripped)
            continue
        item_indent = indent
        if label:
            items.append((label.group(1).strip().lower(), [stripped]))
        else:
            items.append((None, [stripped]))
    paragraphs = []
    for index, (label, lines) in enumerate(items):
        if not lines:
            continue
        text = " ".join(lines)
        if label and (label in DROP_LABELS or label.split()[0] in DROP_LABELS):
            continue
        if index == 0 or not paragraphs:
            text = IDENTITY.sub("", text).strip()
        sentences = re.split(r"(?<=[.;])\s+(?=[A-Z(])", text)
        kept = [s for s in sentences if s and not PRIVATE.search(s)]
        text = " ".join(kept).strip()
        if len(re.sub(r"[^A-Za-z0-9]", "", text)) < 12:
            continue
        paragraphs.append(text)
    return paragraphs


def render_notes(paragraphs: list[str]) -> str:
    if not paragraphs:
        return ""
    out = ["/*", f"{NOTES_TITLE} (kept from the recovery work; historical, not re-verified)."]
    for text in paragraphs:
        out.append(" *")
        out += wrap(" * ", text.replace("*/", "* /"), " * ")
    out.append(" */")
    return "\n".join(out) + "\n"


# ------------------------------------------------------------------ main

def load() -> tuple[dict, dict, dict]:
    data = json.loads(RECIPES.read_text(encoding="utf-8"))
    toolchain = json.loads((ROOT / "config/toolchain.json").read_text(encoding="utf-8"))
    open_cfg = json.loads((ROOT / "config/open_toolchain.json").read_text(encoding="utf-8"))
    return data, toolchain, open_cfg


def clean_body_comment(match: re.Match) -> str:
    """Rewrite a comment below the header only if it mentions the private repository."""
    chunk = match.group(0)
    if not PRIVATE.search(chunk):
        return chunk
    paragraphs = sanitize_notes(chunk)
    if not paragraphs:
        return ""
    text = match.string
    line_start = text.rfind("\n", 0, match.start()) + 1
    before = text[line_start:match.start()]
    if before.strip() or "\n" not in chunk:
        return "/* " + " ".join(paragraphs).replace("*/", "* /") + " */"
    lead = " " * len(before.expandtabs())
    out = ["/*"]
    for index, paragraph in enumerate(paragraphs):
        if index:
            out.append(f"{lead} *")
        out += wrap(f"{lead} * ", paragraph.replace("*/", "* /"), f"{lead} * ")
    out.append(f"{lead} */")
    return "\n".join(out)


def rebuild(recipe: dict, text: str, toolchain: dict, open_cfg: dict) -> str:
    # A byte-order mark would end up after the header, inside the C.
    block, notes, rest = split_source(text.lstrip("﻿"))
    if block is None:
        notes = render_notes(sanitize_notes(notes or ""))
    rest = COMMENT.sub(clean_body_comment, rest)
    header = render_recipe(recipe, toolchain, open_cfg) + (notes or "")
    return header + "\n" + rest.lstrip("\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--check", action="store_true")
    mode.add_argument("--write", action="store_true")
    mode.add_argument("--show", metavar="MODULE:0xADDRESS")
    args = parser.parse_args()

    data, toolchain, open_cfg = load()
    recipes = data["functions"]
    if args.show:
        recipe = next(r for r in recipes if f"{r['module']}:{r['address']}".lower() == args.show.lower())
        text = (ROOT / recipe["source"]).read_text(encoding="utf-8").replace("\r\n", "\n")
        sys.stdout.write(rebuild(recipe, text, toolchain, open_cfg))
        return 0

    problems, changed = [], 0
    for recipe in recipes:
        path = ROOT / recipe["source"]
        text = path.read_text(encoding="utf-8").replace("\r\n", "\n")
        new = rebuild(recipe, text, toolchain, open_cfg)
        if args.check:
            if new != text:
                problems.append(f"{recipe['source']}: header does not match the recipe")
            if lf_sha256(text.encode()) != recipe["source_sha256"]:
                problems.append(f"{recipe['source']}: source_sha256 in the recipe is stale")
            leak = PRIVATE.search("\n".join(COMMENT.findall(text)).split(MARK, 1)[-1].split("*/", 1)[-1])
            if leak:
                problems.append(f"{recipe['source']}: note mentions {leak.group(0)!r}")
            continue
        if new != text:
            path.write_bytes(new.encode("utf-8"))
            changed += 1
        recipe["source_sha256"] = lf_sha256(new.encode())
    if args.check:
        for problem in problems:
            print(problem)
        print(f"{len(recipes)} sources checked, {len(problems)} problem(s)")
        return 1 if problems else 0

    RECIPES.write_text(json.dumps(data, indent=1) + "\n", encoding="utf-8", newline="\n")
    by_source = {r["source"]: r["source_sha256"] for r in recipes}
    rows = list(csv.DictReader(io.StringIO(INVENTORY.read_text(encoding="utf-8"))))
    for row in rows:
        row["source_sha256"] = by_source[row["source"]]
    buffer = io.StringIO()
    writer = csv.DictWriter(buffer, fieldnames=list(rows[0]), lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    INVENTORY.write_text(buffer.getvalue(), encoding="utf-8", newline="\n")
    print(f"{changed} sources rewritten; recipe and inventory hashes refreshed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
