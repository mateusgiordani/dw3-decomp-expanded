"""Compare the open-source GCC 2.8.1 + maspsx path with the PsyQ toolchain.

Stage 1 (compiler): every function is preprocessed once, then compiled by the
PsyQ ``CC1PSX.EXE`` 2.8.1 and by the open-source ``cc1`` from
decompals/old-gcc ``gcc-2.8.1-psx`` with the same flags. The two assembly
outputs are compared line by line after dropping comments, blank lines and
identification directives that do not reach the object.

Stage 2 (assembler + link): the open-source assembly goes through maspsx
(``--aspsx-version``) and GNU ``mipsel-linux-gnu-as``. The ELF ``.text`` (and
``.rodata`` jump table) is linked at the function's PAL address with the recipe
symbols and compared with the PAL bytes, exactly as the PsyQ path is.

The open-source tools are Linux programs. On Windows they run through WSL.

Usage:
    python tools/public_toolchain_check.py --gcc-dir PATH --maspsx PATH \\
        --out build/public-toolchain.json
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import shlex
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import card_verify  # noqa: E402
from elf_link import link_elf, read_elf  # noqa: E402


DROP = re.compile(r"^\s*(#.*)?$|^\s*\.(ident|file)\b|^gcc2_compiled\.:|^__gnu_compiled_c:")


def normalize_asm(text: str) -> list[str]:
    lines = []
    for raw in text.replace("\r\n", "\n").split("\n"):
        line = raw.split("#", 1)[0].rstrip() if not raw.lstrip().startswith(".ascii") else raw.rstrip()
        if DROP.match(line):
            continue
        lines.append(re.sub(r"\s+", " ", line.strip()))
    return lines


def wsl_path(path: Path) -> str:
    resolved = path.resolve()
    if os.name != "nt":
        return str(resolved)
    drive, rest = resolved.drive.rstrip(":").lower(), resolved.as_posix()[2:]
    return f"/mnt/{drive}{rest}"


def run_linux(script: str) -> subprocess.CompletedProcess:
    cmd = ["wsl", "-d", os.environ.get("DW3_WSL_DISTRO", "Ubuntu"), "--", "bash", "-lc", script] \
        if os.name == "nt" else ["bash", "-lc", script]
    return subprocess.run(cmd, capture_output=True, timeout=3600)


# ---------------------------------------------------------------- main

def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--gcc-dir", type=Path, required=True, help="extracted gcc-2.8.1-psx")
    parser.add_argument("--maspsx", type=Path, required=True, help="maspsx checkout (maspsx.py)")
    parser.add_argument("--aspsx-version", default="2.79")
    parser.add_argument("--pal-dir", default=os.environ.get("DW3_PAL_DIR"))
    parser.add_argument("--cc1psx", default=os.environ.get("DW3_PSYQ_CC1PSX"))
    parser.add_argument("--clang", default=os.environ.get("DW3_CLANG", "clang"))
    parser.add_argument("--only", action="append", default=[])
    parser.add_argument("--keep", type=Path, help="keep work files here")
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    toolchain = json.loads((ROOT / "config/toolchain.json").read_text(encoding="utf-8"))
    hashes = json.loads((ROOT / "reference/hashes.json").read_text(encoding="utf-8"))
    recipes = json.loads((ROOT / "recipes/card_cage.json").read_text(encoding="utf-8"))["functions"]
    if args.only:
        wanted = {w.lower() for w in args.only}
        recipes = [r for r in recipes if f"{r['module']}:{r['address']}".lower() in wanted]

    pal = {m: (Path(args.pal_dir) / f"{m.lower()}.bin") if (Path(args.pal_dir) / f"{m.lower()}.bin").is_file()
           else Path(args.pal_dir) / f"{m}.PRO" for m in hashes["overlays"]}
    pal_bytes = {m: p.read_bytes() for m, p in pal.items()}
    for m, data in pal_bytes.items():
        if hashlib.sha256(data).hexdigest() != hashes["overlays"][m]["sha256"]:
            raise SystemExit(f"{pal[m]} is not PAL {m}")

    work = Path(args.keep) if args.keep else Path(tempfile.mkdtemp(prefix="dw3-public-"))
    work.mkdir(parents=True, exist_ok=True)

    # Preprocess and compile with PsyQ on the host.
    def psyq(recipe: dict) -> None:
        tag = f"{recipe['module']}_{recipe['address']}"
        pre = subprocess.run([args.clang, "-E", "-nostdinc", "-include", "include/ps1_types.h", "-I", "include",
                              str(ROOT / recipe["source"])], cwd=ROOT, capture_output=True)
        (work / f"{tag}.i").write_bytes(pre.stdout.replace(b"\r\n", b"\n"))
        flags = card_verify.cc1_flags(toolchain, recipe["variant"], recipe["opt_level"])
        subprocess.run([args.cc1psx, f"{tag}.i", *flags, "-dumpbase", f"{tag}.c", "-o", f"{tag}.psyq.s"],
                       cwd=work, capture_output=True, env={**os.environ, "TMP": ".", "TEMP": "."})

    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
        list(pool.map(psyq, recipes))

    # Compile, maspsx and assemble with the open-source path in one Linux batch.
    lines = ["set -u", f"cd {shlex.quote(wsl_path(work))}"]
    cc1 = wsl_path(args.gcc_dir / "cc1")
    maspsx = wsl_path(args.maspsx / "maspsx.py")
    for recipe in recipes:
        tag = f"{recipe['module']}_{recipe['address']}"
        flags = " ".join(shlex.quote(f) for f in card_verify.cc1_flags(toolchain, recipe["variant"], recipe["opt_level"]))
        lines.append(f"{shlex.quote(cc1)} {tag}.i {flags} -o {tag}.open.s 2>{tag}.open.err || echo cc1-failed > {tag}.open.err")
        # ASPSX needs its zero-divisor guard stripped from the source; maspsx only emits
        # the guard with --expand-div, which is not passed, so it takes the raw output.
        src = f"{tag}.open.s"
        lines.append(f"python3 {shlex.quote(maspsx)} --aspsx-version={args.aspsx_version} {src} < /dev/null > {tag}.masp.s 2>{tag}.masp.err")
        lines.append(f"mipsel-linux-gnu-as -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0 -o {tag}.o {tag}.masp.s 2>{tag}.as.err")
    (work / "build.sh").write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    done = run_linux(f"bash {shlex.quote(wsl_path(work / 'build.sh'))}")
    if done.returncode != 0:
        raise SystemExit(done.stderr.decode("utf-8", "replace")[-800:])

    results, counts = [], {}
    for recipe in recipes:
        tag = f"{recipe['module']}_{recipe['address']}"
        item = {"function": f"{recipe['module']}:{recipe['address']}", "variant": recipe["variant"]}
        psyq_s, open_s = work / f"{tag}.psyq.s", work / f"{tag}.open.s"
        if not psyq_s.is_file() or not open_s.is_file():
            item["compiler"] = "compile_failed"
        else:
            a = normalize_asm(psyq_s.read_text(encoding="latin1"))
            b = normalize_asm(open_s.read_text(encoding="latin1"))
            item["compiler"] = "identical" if a == b else "different"
            if a != b:
                first = next((i for i, (x, y) in enumerate(zip(a, b)) if x != y), min(len(a), len(b)))
                item["first_difference"] = {"line": first, "psyq": a[first] if first < len(a) else None,
                                            "open": b[first] if first < len(b) else None}
        obj = work / f"{tag}.o"
        if not obj.is_file():
            item["link"] = "assemble_failed"
            item["detail"] = ((work / f"{tag}.masp.err").read_text(errors="replace")
                              + (work / f"{tag}.as.err").read_text(errors="replace"))[-400:]
        else:
            try:
                symbols = {k: int(v, 16) for k, v in recipe["symbols"].items()}
                text, ro = link_elf(read_elf(obj.read_bytes()), int(recipe["address"], 16),
                                    int(recipe["rodata"], 16) if recipe["rodata"] else None, symbols)
                base = int(recipe["base"], 16)
                blob = pal_bytes[recipe["module"]]
                start = int(recipe["address"], 16) - base
                reference = blob[start:start + recipe["size"]]
                body = text[:recipe["size"]]
                ok = len(text) >= recipe["body_size"] and body == reference
                if ok and ro:
                    ro_start = int(recipe["rodata"], 16) - base
                    ok = blob[ro_start:ro_start + len(ro)] == ro
                item["link"] = "exact_byte_match" if ok else "mismatch"
                if not ok:
                    item["text_size"] = len(text)
                    diffs = [i for i in range(0, min(len(body), len(reference)), 4) if body[i:i + 4] != reference[i:i + 4]]
                    item["differing_words"] = len(diffs)
            except (ValueError, StopIteration, KeyError) as exc:
                item["link"] = "link_error"
                item["detail"] = str(exc)
        for key in ("compiler", "link"):
            counts.setdefault(key, {}).setdefault(item[key], 0)
            counts[key][item[key]] += 1
        results.append(item)

    summary = {"aspsx_version_mode": args.aspsx_version, "functions": len(results), "counts": counts,
               "results": results}
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(summary, indent=1) + "\n", encoding="utf-8")
    print(json.dumps(counts))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
