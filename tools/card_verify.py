"""Rebuild every Card Cage function and compare it with the PAL reference.

Two toolchains are supported; both must reproduce the same PAL bytes.

``--toolchain open`` (default) uses public tools only:
  GCC 2.8.1 ``cc1`` from decompals/old-gcc, maspsx (ASPSX 2.79 mode) and GNU
  ``mipsel-linux-gnu-as``. These are Linux programs; on Windows they run in WSL.
  ``tools/fetch_toolchain.py`` downloads and checks GCC and maspsx; GNU ``as``
  comes from your distribution (``binutils-mipsel-linux-gnu``).

``--toolchain psyq`` uses the original, proprietary PsyQ tools, which you must
  supply: ``CC1PSX.EXE`` 2.8.1 SN32 BUILD 4.0.0010 and ``ASPSX.EXE`` 2.79 (see
  ``config/toolchain.json`` for their SHA-256 and SDK releases). ASPSX inserts a
  zero-divisor guard after every division that the Card Cage overlays do not
  have, so this path always removes it from the compiler output before ASPSX
  (a no-op when a function has no division). maspsx emits no guard because
  ``--expand-div`` is not passed, so the open path needs no removal.

``--toolchain both`` runs the two and also fails when they disagree on any
  function's status or matched bytes.

Both paths preprocess with clang (``-E -nostdinc``, ``include/``), check every
input hash (sources, tools, PAL overlays) before building, and compare the full
function range and any jump table with the PAL bytes of your own disc.

Usage:
    python tools/card_verify.py --pal-dir PATH/TO/PRO --out build/card-verify.json
    python tools/card_verify.py --toolchain psyq --cc1psx CC1PSX.EXE --aspsx ASPSX.EXE ...
    python tools/card_verify.py --toolchain both ... --out build/verify.json  (verify-open/-psyq.json)
    python tools/card_verify.py --only CARDGAME:0x8008def8 -v
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import os
import shlex
import shutil
import subprocess
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import div_guard  # noqa: E402
import elf_link  # noqa: E402
import verify_match  # noqa: E402


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


def find_file(explicit: str | None, env: str, fallback: str | None = None) -> Path:
    value = explicit or os.environ.get(env) or (shutil.which(fallback) if fallback else None)
    if not value:
        raise SystemExit(f"missing tool: pass it explicitly or set {env}")
    path = Path(value)
    if not path.exists():
        raise SystemExit(f"not found: {path}")
    return path


def cc1_flags(toolchain: dict, variant: str, opt_level: str) -> list[str]:
    flags = [f for f in toolchain["cc1_flags"] if not (f.startswith("-O") and len(f) == 3)]
    flags.insert(1 if flags and flags[0] == "-quiet" else 0, f"-{opt_level}")
    return flags + list(toolchain["variants"][variant])


def run(cmd: list[str], cwd: Path, env: dict | None = None) -> subprocess.CompletedProcess:
    child = os.environ.copy()
    child.update(env or {})
    return subprocess.run(cmd, cwd=str(cwd), env=child, capture_output=True, timeout=180)


def linux_path(path: Path) -> str:
    resolved = path.resolve()
    if os.name != "nt":
        return str(resolved)
    return f"/mnt/{resolved.drive.rstrip(':').lower()}{resolved.as_posix()[2:]}"


def run_linux(script: Path) -> subprocess.CompletedProcess:
    inner = f"bash {shlex.quote(linux_path(script))}"
    if os.name == "nt":
        cmd = ["wsl", "-d", os.environ.get("DW3_WSL_DISTRO", "Ubuntu"), "--", "bash", "-lc", inner]
    else:
        cmd = ["bash", "-lc", inner]
    return subprocess.run(cmd, capture_output=True, timeout=3600)


def tag_of(recipe: dict) -> str:
    return f"{recipe['module']}_{recipe['address']}"


def base_result(recipe: dict) -> dict:
    return {"function": f"{recipe['module']}:{recipe['address']}", "name": recipe["name"],
            "source": recipe["source"], "size": recipe["size"], "variant": recipe["variant"]}


def preprocess(recipe: dict, ctx: dict, work: Path) -> str | None:
    """Write ``<tag>.i``; return an error string or None."""
    source = ROOT / recipe["source"]
    # Hash the LF form so a CRLF checkout (core.autocrlf) verifies the same source.
    if sha256_bytes(source.read_bytes().replace(b"\r\n", b"\n")) != recipe["source_sha256"]:
        return "source_hash_mismatch"
    pre = run([str(ctx["clang"]), "-E", "-nostdinc", "-include", "include/ps1_types.h",
               "-I", "include", str(source)], ROOT)
    if pre.returncode != 0:
        return "preprocess_failed: " + pre.stderr.decode("utf-8", "replace")[-400:]
    (work / f"{tag_of(recipe)}.i").write_bytes(pre.stdout.replace(b"\r\n", b"\n"))
    return None


def compare(recipe: dict, ctx: dict, text: bytes, rodata: bytes) -> dict:
    """Compare a linked function (and jump table) with the PAL overlay."""
    base = int(recipe["base"], 16)
    blob = ctx["pal_bytes"][recipe["module"]]
    start = int(recipe["address"], 16) - base
    reference = blob[start:start + recipe["size"]]
    candidate = text[:recipe["size"]]
    words = [i for i in range(0, recipe["size"], 4) if candidate[i:i + 4] != reference[i:i + 4]]
    ro_diff = 0
    if rodata:
        ro_start = int(recipe["rodata"], 16) - base
        ro_ref = blob[ro_start:ro_start + len(rodata)]
        ro_diff = sum(1 for i in range(0, len(rodata), 4) if rodata[i:i + 4] != ro_ref[i:i + 4])
    if len(text) < recipe["body_size"]:
        return {"status": "size_mismatch", "detail": f"text {len(text)} < body {recipe['body_size']}"}
    exact = not words and not ro_diff
    return {"status": "exact_byte_match" if exact else "mismatch", "difference_count": len(words),
            "rodata_difference_count": ro_diff, "candidate_sha256": sha256_bytes(candidate),
            "reference_sha256": sha256_bytes(reference)}


# ------------------------------------------------------------------ open path

def verify_open(recipes: list[dict], ctx: dict, work: Path) -> list[dict]:
    open_cfg = ctx["open"]
    lines = ["set -u", f"cd {shlex.quote(linux_path(work))}"]
    as_flags = " ".join(shlex.quote(f) for f in open_cfg["gnu_as"]["flags"])
    masp_args = " ".join(shlex.quote(a) for a in open_cfg["maspsx"]["args"])
    cc1 = shlex.quote(linux_path(ctx["gcc_dir"] / "cc1"))
    maspsx = shlex.quote(linux_path(ctx["maspsx_dir"] / "maspsx.py"))
    gas = shlex.quote(ctx["gnu_as"])
    lines.append(f"{gas} --version | head -1 > gnu-as.version")
    results = {}
    for recipe in recipes:
        tag = tag_of(recipe)
        error = preprocess(recipe, ctx, work)
        if error:
            results[tag] = {**base_result(recipe), "status": error.split(":")[0], "detail": error}
            continue
        flags = " ".join(shlex.quote(f) for f in cc1_flags(ctx["toolchain"], recipe["variant"], recipe["opt_level"]))
        lines.append(f"{cc1} {tag}.i {flags} -o {tag}.s 2>{tag}.cc1.err"
                     f" && python3 {maspsx} {masp_args} {tag}.s < /dev/null > {tag}.masp.s 2>{tag}.masp.err"
                     f" && {gas} {as_flags} -o {tag}.o {tag}.masp.s 2>{tag}.as.err")
    script = work / "build.sh"
    script.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    done = run_linux(script)
    if done.returncode not in (0, 1):
        raise SystemExit("Linux build step failed: " + done.stderr.decode("utf-8", "replace")[-800:])
    version = work / "gnu-as.version"
    ctx["gnu_as_version"] = version.read_text(errors="replace").strip() if version.is_file() else None
    out = []
    for recipe in recipes:
        tag = tag_of(recipe)
        if tag in results:
            out.append(results[tag])
            continue
        obj = work / f"{tag}.o"
        if not obj.is_file():
            detail = "".join((work / f"{tag}.{s}.err").read_text(errors="replace")
                             for s in ("cc1", "masp", "as") if (work / f"{tag}.{s}.err").is_file())
            out.append({**base_result(recipe), "status": "build_failed", "detail": detail[-600:]})
            continue
        try:
            symbols = {k: int(v, 16) for k, v in recipe["symbols"].items()}
            text, rodata = elf_link.link_elf(elf_link.read_elf(obj.read_bytes()), int(recipe["address"], 16),
                                             int(recipe["rodata"], 16) if recipe["rodata"] else None, symbols)
            out.append({**base_result(recipe), **compare(recipe, ctx, text, rodata)})
        except (ValueError, KeyError, StopIteration) as exc:
            out.append({**base_result(recipe), "status": "link_error", "detail": str(exc)})
    return out


# ------------------------------------------------------------------ PsyQ path

def verify_psyq_one(recipe: dict, ctx: dict, work: Path) -> dict:
    tag = tag_of(recipe)
    error = preprocess(recipe, ctx, work)
    if error:
        return {**base_result(recipe), "status": error.split(":")[0], "detail": error}
    unit = work / tag
    unit.mkdir(exist_ok=True)
    shutil.move(str(work / f"{tag}.i"), unit / "unit.i")
    flags = cc1_flags(ctx["toolchain"], recipe["variant"], recipe["opt_level"])
    cc1 = run([str(ctx["cc1psx"]), "unit.i", *flags, "-dumpbase", "unit.c", "-o", "unit.s"], unit,
              {"GCCTMP": ".", "TMP": ".", "TEMP": ".", "TMPDIR": "."})
    if cc1.returncode != 0 or not (unit / "unit.s").is_file():
        return {**base_result(recipe), "status": "build_failed",
                "detail": (cc1.stdout + cc1.stderr).decode("latin1", "replace")[-600:]}
    # Required on this path: ASPSX would insert a zero-divisor guard the overlays lack.
    assembly = div_guard.strip_div_guard((unit / "unit.s").read_bytes())
    (unit / "input.s").write_bytes(assembly)
    # ASPSX reports its warning count as the exit code; the object file is the only
    # reliable success signal.
    asm = run([str(ctx["aspsx"]), "-o", "output.obj", *recipe["aspsx_flags"], "input.s"], unit)
    obj = unit / "output.obj"
    if not obj.is_file():
        return {**base_result(recipe), "status": "build_failed",
                "detail": (asm.stdout + asm.stderr).decode("latin1", "replace")[-600:]}
    symbols = {name: int(value, 16) for name, value in recipe["symbols"].items()}
    try:
        report = verify_match.verify(
            obj, ctx["pal"][recipe["module"]], int(recipe["base"], 16), int(recipe["address"], 16),
            recipe["size"], symbols, expected_body_size=recipe["body_size"],
            rodata_address=int(recipe["rodata"], 16) if recipe["rodata"] else None,
            gp_address=int(recipe["gp"], 16) if recipe["gp"] else None)
    except verify_match.SizeMismatchError as exc:
        return {**base_result(recipe), "status": "size_mismatch", "detail": str(exc)}
    except (ValueError, KeyError) as exc:
        return {**base_result(recipe), "status": "link_error", "detail": str(exc)}
    return {**base_result(recipe), "status": report["status"], "difference_count": report["difference_count"],
            "rodata_difference_count": report.get("rodata_difference_count", 0),
            "candidate_sha256": report["candidate_sha256"], "reference_sha256": report["reference_sha256"]}


# ------------------------------------------------------------------ main

def prepare_tools(name: str, args: argparse.Namespace, ctx: dict, recipes: list[dict]) -> dict:
    """Check the pinned tools of one toolchain; put them in ``ctx`` and describe them."""
    toolchain, open_cfg = ctx["toolchain"], ctx["open"]
    if name == "open":
        gcc_dir, maspsx_dir = Path(args.gcc_dir), Path(args.maspsx_dir)
        checks = {gcc_dir / "cc1": open_cfg["gcc"]["cc1_sha256"]}
        checks.update({maspsx_dir / rel: digest for rel, digest in open_cfg["maspsx"]["files_sha256"].items()})
        for path, digest in checks.items():
            if not path.is_file() or sha256_file(path) != digest:
                raise SystemExit(f"{path} is missing or does not match the pinned hash; run tools/fetch_toolchain.py")
        ctx.update(gcc_dir=gcc_dir, maspsx_dir=maspsx_dir,
                   gnu_as=args.gnu_as or open_cfg["gnu_as"]["command"])
        return {"gcc": {"id": open_cfg["gcc"]["id"], "cc1_sha256": open_cfg["gcc"]["cc1_sha256"]},
                "maspsx": {"revision": open_cfg["maspsx"]["revision"], "args": open_cfg["maspsx"]["args"]},
                "gnu_as": {"command": ctx["gnu_as"], "flags": open_cfg["gnu_as"]["flags"]}}
    tools_used = {}
    pinned = {t["id"]: t for kind in toolchain["tools"].values() for t in kind}
    for kind, env in (("cc1psx", "DW3_PSYQ_CC1PSX"), ("aspsx", "DW3_PSYQ_ASPSX")):
        tool = find_file(getattr(args, kind), env)
        needed = {r[kind] for r in recipes}
        digest = sha256_file(tool)
        if len(needed) != 1 or pinned[next(iter(needed))]["sha256"] != digest:
            raise SystemExit(f"{tool} does not match the pinned {kind} ({digest})")
        ctx[kind] = tool
        tools_used[kind] = {k: pinned[next(iter(needed))][k] for k in ("id", "sha256", "version")}
    return tools_used


def run_toolchain(name: str, args: argparse.Namespace, ctx: dict, recipes: list[dict],
                  tools_used: dict, hashes: dict) -> dict:
    work = (args.keep / name) if args.keep else Path(tempfile.mkdtemp(prefix=f"dw3-verify-{name}-"))
    work.mkdir(parents=True, exist_ok=True)
    started = datetime.now(timezone.utc).isoformat(timespec="seconds")
    if name == "open":
        results = verify_open(recipes, ctx, work)
        tools_used["gnu_as"]["version"] = ctx.get("gnu_as_version")
    else:
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
            results = list(pool.map(lambda r: verify_psyq_one(r, ctx, work), recipes))
    if not args.keep:
        shutil.rmtree(work, ignore_errors=True)

    counts: dict[str, int] = {}
    for item in results:
        counts[item["status"]] = counts.get(item["status"], 0) + 1
        if args.verbose or item["status"] != "exact_byte_match":
            print(f"{name:5} {item['function']:22} {item['status']} {item.get('difference_count', '')}")
    commit = subprocess.run(["git", "-C", str(ROOT), "rev-parse", "HEAD"],
                            capture_output=True, text=True).stdout.strip() or None
    dirty = bool(subprocess.run(["git", "-C", str(ROOT), "status", "--porcelain", "--", "src", "include",
                                 "recipes", "config", "tools"], capture_output=True, text=True).stdout.strip())
    summary = {
        "schema_version": 2,
        "toolchain": name,
        "reference_version": hashes["reference_version"],
        "repository_commit": commit,
        "tree_dirty": dirty,
        "started_at": started,
        "finished_at": datetime.now(timezone.utc).isoformat(timespec="seconds"),
        "tools": tools_used,
        "overlays": {m: hashes["overlays"][m]["sha256"] for m in ctx["pal"]},
        "functions": len(results),
        "status_counts": counts,
        "results": results,
    }
    print(json.dumps({"toolchain": name, "functions": len(results), "status_counts": counts}))
    return summary


def disagreements(first: dict, second: dict) -> list[str]:
    """Functions whose status or matched bytes differ between two reports."""
    other = {r["function"]: r for r in second["results"]}
    problems = []
    for result in first["results"]:
        twin = other.get(result["function"])
        if twin is None:
            problems.append(f"{result['function']}: missing from the {second['toolchain']} report")
        elif (result["status"], result.get("candidate_sha256")) != (twin["status"], twin.get("candidate_sha256")):
            problems.append(f"{result['function']}: {first['toolchain']} {result['status']}, "
                            f"{second['toolchain']} {twin['status']}")
    return problems


def output_path(out: Path | None, name: str, both: bool) -> Path | None:
    if out is None or not both:
        return out
    return out.with_name(f"{out.stem}-{name}{out.suffix}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--toolchain", choices=("open", "psyq", "both"), default="open")
    parser.add_argument("--pal-dir", default=os.environ.get("DW3_PAL_DIR"))
    parser.add_argument("--gcc-dir", default=os.environ.get("DW3_GCC_DIR", str(ROOT / "toolchains/gcc-2.8.1-psx")))
    parser.add_argument("--maspsx-dir", default=os.environ.get("DW3_MASPSX_DIR", str(ROOT / "toolchains/maspsx")))
    parser.add_argument("--gnu-as", default=os.environ.get("DW3_GNU_AS"))
    parser.add_argument("--cc1psx")
    parser.add_argument("--aspsx")
    parser.add_argument("--clang")
    parser.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) // 2))
    parser.add_argument("--only", action="append", default=[], help="MODULE:0xADDRESS")
    parser.add_argument("--keep", type=Path, help="keep intermediate files in this directory")
    parser.add_argument("--out", type=Path, help="report path; with --toolchain both, NAME-open and NAME-psyq")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()

    toolchain = json.loads((ROOT / "config/toolchain.json").read_text(encoding="utf-8"))
    open_cfg = json.loads((ROOT / "config/open_toolchain.json").read_text(encoding="utf-8"))
    hashes = json.loads((ROOT / "reference/hashes.json").read_text(encoding="utf-8"))
    recipes = json.loads((ROOT / "recipes/card_cage.json").read_text(encoding="utf-8"))["functions"]
    if args.only:
        wanted = {w.lower() for w in args.only}
        recipes = [r for r in recipes if f"{r['module']}:{r['address']}".lower() in wanted]
    if not args.pal_dir:
        raise SystemExit("missing PAL overlays: pass --pal-dir or set DW3_PAL_DIR")

    pal = {}
    for module, entry in hashes["overlays"].items():
        path = Path(args.pal_dir) / f"{module}.PRO"
        if not path.is_file():
            path = Path(args.pal_dir) / f"{module.lower()}.bin"
        if not path.is_file() or sha256_file(path) != entry["sha256"]:
            raise SystemExit(f"{path} is missing or is not PAL {module}")
        pal[module] = path
    ctx = {"toolchain": toolchain, "open": open_cfg, "pal": pal,
           "pal_bytes": {m: p.read_bytes() for m, p in pal.items()},
           "clang": find_file(args.clang, "DW3_CLANG", "clang")}

    names = ["open", "psyq"] if args.toolchain == "both" else [args.toolchain]
    tools = {name: prepare_tools(name, args, ctx, recipes) for name in names}
    reports = {}
    for name in names:
        reports[name] = run_toolchain(name, args, ctx, recipes, tools[name], hashes)
        path = output_path(args.out, name, len(names) > 1)
        if path:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(json.dumps(reports[name], indent=1) + "\n", encoding="utf-8")
    ok = all(r["status_counts"].get("exact_byte_match", 0) == len(recipes) for r in reports.values())
    if len(names) > 1:
        split = disagreements(reports["open"], reports["psyq"])
        for line in split:
            print(line)
        print(json.dumps({"toolchains_agree": not split, "disagreements": len(split)}))
        ok = ok and not split
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
