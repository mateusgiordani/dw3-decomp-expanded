"""Export the Card Cage milestone from the private working repository.

This is the maintainer script that produced this repository's snapshot. It
reads a checkout of the private working repository (``--source``) and writes
only material that can be published:

* the 318 recovered C sources of CARDGAME, STCRDABM, STCRDDEK and STCRDSHP,
  byte for byte, plus the four headers they include;
* one verification recipe per function (base, address, size, symbols,
  rodata, compiler variant), with toolchains identified only by SHA-256;
* the function inventory with the SHA-256 of every exported source.

It never copies game data, PsyQ binaries, generated objects, private
listings, queue configuration or local paths. Recipes are taken, in order,
from the function's accepted manifest, from an ``exact_byte_match`` attempt in
its matching ledger whose source hash equals the exported source, from the
exact byte-report of its acceptance review or integration manifest, or from the
command the source repository infers. ``tools/card_verify.py`` then proves
every recipe against the PAL reference; the recipe origin is informational.

Usage:
    python tools/export/export_card_cage.py --source "PATH/TO/private/repo" --out .
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

MODULES = ("CARDGAME", "STCRDABM", "STCRDDEK", "STCRDSHP")
HEADERS = {
    "decomp/experiments/compiler/ps1_types.h": "include/ps1_types.h",
    "decomp/experiments/compiler/include/stdint.h": "include/stdint.h",
    "decomp/include/common/types.h": "include/common/types.h",
    "decomp/include/exe/exe.h": "include/exe/exe.h",
}
VALUE_OPTIONS = {
    "--c", "--reference-bin", "--base", "--address", "--size", "--body-size",
    "--opt-level", "--variant", "--rodata", "--gp", "--auto-symbols",
    "--cc1", "--aspsx", "--cc1-id", "--aspsx-id", "--name", "--ref",
}
MULTI_OPTIONS = {"--symbol", "--aspsx-flag"}
FLAG_OPTIONS = {"--strip-div-guard"}


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def blob(source: Path, rev: str, rel: str) -> bytes:
    """Committed (LF) content of ``rel`` at ``rev``; never the working-tree bytes."""
    return subprocess.run(
        ["git", "-C", str(source), "show", f"{rev}:{rel}"],
        check=True, capture_output=True,
    ).stdout


def parse_argv(argv: list[str]) -> dict:
    start = next(i for i, a in enumerate(argv) if a.endswith("fn_exact_pipeline.py")) + 1
    out: dict = {"--symbol": [], "--aspsx-flag": []}
    i = start
    while i < len(argv):
        opt = argv[i]
        if opt in FLAG_OPTIONS:
            out[opt] = True
            i += 1
        elif opt in MULTI_OPTIONS:
            out[opt].append(argv[i + 1])
            i += 2
        elif opt in VALUE_OPTIONS:
            out[opt] = argv[i + 1]
            i += 2
        else:
            raise ValueError(f"unsupported pipeline option {opt!r} in {argv}")
    return out


def tool_ids(config: dict) -> tuple[dict[str, str], dict[str, str]]:
    by_hash = {}
    for kind in ("cc1psx", "aspsx"):
        for tool in config["tools"][kind]:
            by_hash[tool["sha256"]] = tool["id"]
    return by_hash, config["default_tools"]


def resolve_tool(opts: dict, kind: str, source: Path, by_hash: dict, defaults: dict) -> str:
    id_opt, path_opt = ("--cc1-id", "--cc1") if kind == "cc1psx" else ("--aspsx-id", "--aspsx")
    if opts.get(id_opt):
        return opts[id_opt]
    if opts.get(path_opt):
        path = Path(opts[path_opt])
        if not path.is_absolute():
            path = source / path
        digest = sha256(path)
        if digest not in by_hash:
            raise ValueError(f"unpinned {kind} {path} ({digest})")
        return by_hash[digest]
    return defaults[kind]


def ledger_command(source: Path, module: str, address: str, src_sha: str) -> list[str] | None:
    slug = f"{module.lower()}-{address[2:].lower()}"
    folder = source / "docs" / "c-matching-guide" / "submissions" / slug
    best = None
    for ledger in sorted(folder.glob("attempts-*.json")):
        try:
            data = json.loads(ledger.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            continue
        for attempt in data.get("attempts", []):
            if attempt.get("outcome") != "exact_byte_match":
                continue
            if attempt.get("source_sha256") != src_sha:
                continue
            command = attempt.get("command")
            if command and any(a.endswith("fn_exact_pipeline.py") for a in command):
                best = command
    return best


def report_opts(report: dict) -> dict:
    """Pipeline options recorded by an exact byte-report."""
    opts = {
        "--symbol": [f"{k}={v}" for k, v in report.get("symbols", {}).items()],
        "--aspsx-flag": list(report.get("assembler", {}).get("flags", [])),
        "--base": report["base"], "--address": report["address"],
        "--size": str(report["size"]), "--body-size": str(report["body_size"]),
        "--variant": report.get("variant", "base"),
        "--cc1-id": report["compiler"]["id"], "--aspsx-id": report["assembler"]["id"],
    }
    if report.get("rodata_address"):
        opts["--rodata"] = report["rodata_address"]
    if report.get("gp_address"):
        opts["--gp"] = report["gp_address"]
    if report.get("strip_div_guard"):
        opts["--strip-div-guard"] = True
    return opts


def review_opts(source: Path, row: dict, src_sha: str) -> dict | None:
    """Recipe from an accepted review whose exact byte-report replayed this source."""
    raw = (row.get("evidence_path") or "").strip()
    if not raw.startswith("reports/acceptance/reviews/"):
        return None
    review = json.loads((source / raw).read_text(encoding="utf-8"))
    if review.get("decision") != "accepted" or review.get("source_sha256_lf") != src_sha:
        return None
    for rel in review.get("evidence", []):
        if not rel.endswith("byte-report.json"):
            continue
        report = json.loads((source / rel).read_text(encoding="utf-8"))
        if report.get("status") == "exact_byte_match":
            return report_opts(report)
    return None


def integration_report_opts(source: Path, row: dict) -> dict | None:
    """Recipe from the exact byte-report next to the catalog's integration manifest."""
    raw = (row.get("evidence_path") or "").strip()
    if not raw.startswith("reports/acceptance/") or raw.startswith("reports/acceptance/reviews/"):
        return None
    folder = (source / raw).parent
    address = row["address"].lower()
    for candidate in sorted(folder.glob("*byte-report*.json")):
        try:
            report = json.loads(candidate.read_text(encoding="utf-8"))
        except ValueError:
            continue
        if report.get("address", "").lower() == address and report.get("status") == "exact_byte_match":
            return report_opts(report)
    return None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--rev", default="HEAD", help="source commit to export")
    args = parser.parse_args()
    source, out = args.source.resolve(), args.out.resolve()
    rev = subprocess.run(["git", "-C", str(source), "rev-parse", args.rev],
                         check=True, capture_output=True, text=True).stdout.strip()

    sys.path.insert(0, str(source / "tools"))
    import card_retest  # noqa: E402  (source repository tooling)
    import fn_exact_pipeline  # noqa: E402

    config = json.loads((source / "decomp/config/psyq_oracle.json").read_text(encoding="utf-8"))
    by_hash, defaults = tool_ids(config)
    variants = config["variants"]
    bases = card_retest.load_bases()

    rows = [
        r for r in csv.DictReader((source / "symbols/functions.csv").open(encoding="utf-8"))
        if r["module"] in MODULES
    ]
    table = card_retest.symbol_table(rows)
    for src, dst in HEADERS.items():
        target = out / dst
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(blob(source, rev, src))

    recipes, inventory, origins = [], [], {}
    for row in rows:
        module, address = row["module"], row["address"].lower()
        src_path = source / row["source_path"]
        src_bytes = blob(source, rev, row["source_path"])
        src_sha = hashlib.sha256(src_bytes).hexdigest()
        overlay = row["overlay"].strip().lower()
        dst_rel = f"src/{overlay}/{src_path.name}"
        (out / dst_rel).parent.mkdir(parents=True, exist_ok=True)
        (out / dst_rel).write_bytes(src_bytes)

        command, origin = None, None
        resolution = card_retest.resolve_manifest(module, address, row=row)
        if resolution.status == card_retest.STATUS_RESOLVED and resolution.entry is not None:
            command, origin = card_retest.accepted_replay_command(resolution.entry, "export"), "accepted_manifest"
            manifest_c = parse_argv(command).get("--c")
            if manifest_c and hashlib.sha256(blob(source, rev, Path(manifest_c).as_posix())).hexdigest() != src_sha:
                command, origin = None, None
        if command is None:
            command = ledger_command(source, module, address, src_sha)
            origin = "matching_ledger" if command else None
        opts = parse_argv(command) if command else None
        if opts is None:
            opts = review_opts(source, row, src_sha)
            origin = "acceptance_review" if opts else None
        if opts is None:
            opts = integration_report_opts(source, row)
            origin = "integration_byte_report" if opts else None
        if opts is None:
            base = bases[overlay.upper()]
            command = card_retest.build_command(row, base, table, "export", None)
            origin = "inferred"
            opts = parse_argv(command) if command else None
        if opts is None:
            raise SystemExit(f"no recipe for {module}:{address}")

        symbols = {}
        if opts.get("--auto-symbols"):
            text = src_bytes.decode("utf-8", errors="replace")
            names = set(re.findall(r"[A-Za-z_][A-Za-z0-9_]*", text))
            for name, value in fn_exact_pipeline.load_module_data_symbols(opts["--auto-symbols"]).items():
                if name in names:
                    symbols[name] = f"0x{value:08x}"
        for definition in opts["--symbol"]:
            name, value = definition.split("=", 1)
            symbols[name] = f"0x{int(value, 0):08x}"

        variant = opts.get("--variant", "base")
        if variant not in variants:
            raise SystemExit(f"unknown variant {variant} for {module}:{address}")
        recipe = {
            "module": module,
            "address": address,
            "name": row["current_name"],
            "source": dst_rel,
            "source_sha256": src_sha,
            "overlay_file": f"{overlay.upper()}.PRO",
            "base": f"0x{int(opts['--base'], 0):08x}",
            "size": int(opts["--size"], 0),
            "body_size": int(opts.get("--body-size") or opts["--size"], 0),
            "opt_level": opts.get("--opt-level", "O2"),
            "variant": variant,
            "cc1psx": resolve_tool(opts, "cc1psx", source, by_hash, defaults),
            "aspsx": resolve_tool(opts, "aspsx", source, by_hash, defaults),
            "rodata": f"0x{int(opts['--rodata'], 0):08x}" if opts.get("--rodata") else None,
            "gp": f"0x{int(opts['--gp'], 0):08x}" if opts.get("--gp") else None,
            "strip_div_guard": bool(opts.get("--strip-div-guard")),
            "aspsx_flags": opts["--aspsx-flag"],
            "symbols": dict(sorted(symbols.items())),
            "recipe_origin": origin,
        }
        recipes.append(recipe)
        origins[origin] = origins.get(origin, 0) + 1
        inventory.append({
            "module": module, "address": address, "end_address": row["end_address"].lower(),
            "size": int(row["size"]), "name": row["current_name"], "source": dst_rel,
            "source_sha256": src_sha, "catalog_status": row["status"],
        })

    used = sorted({(r["cc1psx"], r["aspsx"], r["variant"]) for r in recipes})
    tools = {
        kind: [t for t in config["tools"][kind] if t["id"] in {r[kind] for r in recipes}]
        for kind in ("cc1psx", "aspsx")
    }
    toolchain = {
        "schema_version": 1,
        "note": "Proprietary PsyQ binaries are not distributed; they are identified by SHA-256 only.",
        "cc1_flags": config["cc1_flags"],
        "variants": {v: variants[v] for v in sorted({r["variant"] for r in recipes})},
        "tools": {k: [{f: t[f] for f in ("id", "sha256", "version") if f in t} for t in v] for k, v in tools.items()},
        "combinations_used": [list(u) for u in used],
    }
    (out / "config").mkdir(exist_ok=True)
    (out / "config" / "toolchain.json").write_text(json.dumps(toolchain, indent=2) + "\n", encoding="utf-8")
    (out / "recipes").mkdir(exist_ok=True)
    (out / "recipes" / "card_cage.json").write_text(
        json.dumps({"schema_version": 1, "functions": recipes}, indent=1) + "\n", encoding="utf-8")
    (out / "symbols").mkdir(exist_ok=True)
    with (out / "symbols" / "card_cage_functions.csv").open("w", encoding="utf-8", newline="\n") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(inventory[0]), lineterminator="\n")
        writer.writeheader()
        writer.writerows(inventory)
    (out / "recipes" / "export-provenance.json").write_text(json.dumps(
        {"source_commit": rev, "functions": len(recipes), "recipe_origin": origins}, indent=2) + "\n",
        encoding="utf-8")
    print(json.dumps({"source_commit": rev, "functions": len(recipes), "recipe_origin": origins,
                      "combinations_used": toolchain["combinations_used"]}, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
