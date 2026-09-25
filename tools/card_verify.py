"""Rebuild every Card Cage function and compare it with the PAL reference.

For each recipe in ``recipes/card_cage.json`` this tool:

1. checks the SHA-256 of the C source, of the two PsyQ tools and of the
   extracted PAL overlay against the pinned values in this repository;
2. preprocesses the source with clang (``-E -nostdinc``, ``include/``);
3. compiles it with the pinned CC1PSX flags and variant;
4. assembles it with ASPSX;
5. links the function at its PAL address with the recipe's symbols and
   compares the full function range (and any jump table) with the PAL bytes.

No game data and no proprietary tool is part of this repository. You supply:

* the four overlays extracted from your own PAL SLES-03936 disc, named
  ``CARDGAME.PRO``, ``STCRDABM.PRO``, ``STCRDDEK.PRO``, ``STCRDSHP.PRO``
  (``--pal-dir`` or ``DW3_PAL_DIR``);
* ``CC1PSX.EXE`` (PsyQ GCC 2.8.1 SN32 4.0.0010) and ``ASPSX.EXE`` 2.79
  (``--cc1psx``/``--aspsx`` or ``DW3_PSYQ_CC1PSX``/``DW3_PSYQ_ASPSX``);
* clang (``--clang``, ``DW3_CLANG`` or ``clang`` on PATH), used only as a
  C preprocessor.

On non-Windows hosts the DOS/Win32 PsyQ tools need a wrapper; pass the wrapper
command as the tool path.

Usage:
    python tools/card_verify.py --out build/card-verify.json
    python tools/card_verify.py --only CARDGAME:0x8008def8 -v
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import div_guard  # noqa: E402
import verify_match  # noqa: E402


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


def find_tool(explicit: str | None, env: str, fallback: str | None = None) -> Path:
    value = explicit or os.environ.get(env) or (shutil.which(fallback) if fallback else None)
    if not value:
        raise SystemExit(f"missing tool: pass it explicitly or set {env}")
    path = Path(value)
    if not path.is_file():
        raise SystemExit(f"tool not found: {path}")
    return path


def cc1_flags(toolchain: dict, variant: str, opt_level: str) -> list[str]:
    flags = [f for f in toolchain["cc1_flags"] if not (f.startswith("-O") and len(f) == 3)]
    flags.insert(1 if flags and flags[0] == "-quiet" else 0, f"-{opt_level}")
    return flags + list(toolchain["variants"][variant])


def run(cmd: list[str], cwd: Path, env: dict | None = None) -> subprocess.CompletedProcess:
    child = os.environ.copy()
    child.update(env or {})
    return subprocess.run(cmd, cwd=str(cwd), env=child, capture_output=True, timeout=180)


def build_one(recipe: dict, ctx: dict) -> dict:
    ident = f"{recipe['module']}:{recipe['address']}"
    result = {"function": ident, "name": recipe["name"], "source": recipe["source"],
              "size": recipe["size"], "variant": recipe["variant"]}
    source = ROOT / recipe["source"]
    # Hash the LF form so a CRLF checkout (core.autocrlf) verifies the same source.
    source_sha = sha256_bytes(source.read_bytes().replace(b"\r\n", b"\n"))
    if source_sha != recipe["source_sha256"]:
        return {**result, "status": "source_hash_mismatch", "source_sha256": source_sha}
    with tempfile.TemporaryDirectory(prefix="dw3-verify-") as temp:
        work = Path(temp)
        pre = run([str(ctx["clang"]), "-E", "-nostdinc", "-include", "include/ps1_types.h",
                   "-I", "include", str(source)], ROOT)
        if pre.returncode != 0:
            return {**result, "status": "preprocess_failed",
                    "detail": pre.stderr.decode("utf-8", "replace")[-600:]}
        (work / "unit.i").write_bytes(pre.stdout.replace(b"\r\n", b"\n"))
        flags = cc1_flags(ctx["toolchain"], recipe["variant"], recipe["opt_level"])
        cc1 = run([str(ctx["cc1psx"]), "unit.i", *flags, "-dumpbase", "unit.c", "-o", "unit.s"], work,
                  {"GCCTMP": ".", "TMP": ".", "TEMP": ".", "TMPDIR": "."})
        if cc1.returncode != 0 or not (work / "unit.s").is_file():
            return {**result, "status": "compile_failed",
                    "detail": (cc1.stdout + cc1.stderr).decode("latin1", "replace")[-600:]}
        assembly = (work / "unit.s").read_bytes()
        if recipe["strip_div_guard"]:
            assembly = div_guard.strip_div_guard(assembly)
        (work / "input.s").write_bytes(assembly)
        # ASPSX reports its warning count as the exit code; the object file is the only
        # reliable success signal.
        asm = run([str(ctx["aspsx"]), "-o", "output.obj", *recipe["aspsx_flags"], "input.s"], work)
        obj = work / "output.obj"
        if not obj.is_file():
            return {**result, "status": "assemble_failed",
                    "detail": (asm.stdout + asm.stderr).decode("latin1", "replace")[-600:]}
        symbols = {name: int(value, 16) for name, value in recipe["symbols"].items()}
        try:
            report = verify_match.verify(
                obj, ctx["pal"][recipe["module"]], int(recipe["base"], 16),
                int(recipe["address"], 16), recipe["size"], symbols,
                expected_body_size=recipe["body_size"],
                rodata_address=int(recipe["rodata"], 16) if recipe["rodata"] else None,
                gp_address=int(recipe["gp"], 16) if recipe["gp"] else None,
            )
        except verify_match.SizeMismatchError as exc:
            return {**result, "status": "size_mismatch", "detail": str(exc)}
        except (ValueError, KeyError) as exc:
            return {**result, "status": "verify_error", "detail": str(exc)}
    return {**result, "status": report["status"],
            "difference_count": report["difference_count"],
            "candidate_sha256": report["candidate_sha256"],
            "reference_sha256": report["reference_sha256"],
            "rodata_difference_count": report.get("rodata_difference_count", 0),
            "relocation_count": report.get("relocation_count")}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--pal-dir", default=os.environ.get("DW3_PAL_DIR"))
    parser.add_argument("--cc1psx")
    parser.add_argument("--aspsx")
    parser.add_argument("--clang")
    parser.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) // 2))
    parser.add_argument("--only", action="append", default=[], help="MODULE:0xADDRESS")
    parser.add_argument("--out", type=Path)
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()

    toolchain = json.loads((ROOT / "config/toolchain.json").read_text(encoding="utf-8"))
    hashes = json.loads((ROOT / "reference/hashes.json").read_text(encoding="utf-8"))
    recipes = json.loads((ROOT / "recipes/card_cage.json").read_text(encoding="utf-8"))["functions"]
    if args.only:
        wanted = {w.upper().replace("0X", "0x") for w in args.only}
        recipes = [r for r in recipes if f"{r['module']}:{r['address']}".upper().replace("0X", "0x") in wanted]
    if not args.pal_dir:
        raise SystemExit("missing PAL overlays: pass --pal-dir or set DW3_PAL_DIR")

    pal = {}
    for module, entry in hashes["overlays"].items():
        path = Path(args.pal_dir) / f"{module}.PRO"
        if not path.is_file():
            path = Path(args.pal_dir) / f"{module.lower()}.bin"
        digest = sha256_file(path)
        if digest != entry["sha256"]:
            raise SystemExit(f"{path} is not PAL {module} ({digest})")
        pal[module] = path

    pinned = {t["id"]: t["sha256"] for kind in toolchain["tools"].values() for t in kind}
    ctx = {"toolchain": toolchain, "pal": pal, "clang": find_tool(args.clang, "DW3_CLANG", "clang")}
    for kind, env in (("cc1psx", "DW3_PSYQ_CC1PSX"), ("aspsx", "DW3_PSYQ_ASPSX")):
        tool = find_tool(getattr(args, kind), env)
        needed = {r[kind] for r in recipes}
        digest = sha256_file(tool)
        if len(needed) != 1 or pinned[needed.pop()] != digest:
            raise SystemExit(f"{tool} does not match the pinned {kind} ({digest})")
        ctx[kind] = tool

    started = datetime.now(timezone.utc).isoformat(timespec="seconds")
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        results = list(pool.map(lambda r: build_one(r, ctx), recipes))
    counts: dict[str, int] = {}
    for item in results:
        counts[item["status"]] = counts.get(item["status"], 0) + 1
        if args.verbose or item["status"] != "exact_byte_match":
            print(f"{item['function']:22} {item['status']} {item.get('difference_count', '')}")
    commit = subprocess.run(["git", "-C", str(ROOT), "rev-parse", "HEAD"],
                            capture_output=True, text=True).stdout.strip() or None
    dirty = bool(subprocess.run(["git", "-C", str(ROOT), "status", "--porcelain", "--", "src", "include",
                                 "recipes", "config", "tools"], capture_output=True, text=True).stdout.strip())
    summary = {
        "schema_version": 1,
        "reference_version": hashes["reference_version"],
        "repository_commit": commit,
        "tree_dirty": dirty,
        "started_at": started,
        "finished_at": datetime.now(timezone.utc).isoformat(timespec="seconds"),
        "tools": {k: {"path_name": ctx[k].name, "sha256": sha256_file(ctx[k])} for k in ("cc1psx", "aspsx")},
        "overlays": {m: hashes["overlays"][m]["sha256"] for m in pal},
        "functions": len(results),
        "status_counts": counts,
        "results": results,
    }
    print(json.dumps({k: summary[k] for k in ("functions", "status_counts")}))
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(json.dumps(summary, indent=1) + "\n", encoding="utf-8")
    return 0 if counts.get("exact_byte_match", 0) == len(results) else 1


if __name__ == "__main__":
    raise SystemExit(main())
