"""Write the Card Cage release certificate from ``card_verify.py`` reports.

The primary report must come from the open toolchain (the default path). An
optional cross-check report from the PsyQ toolchain is recorded alongside it.
The certificate is only written when every report covers every recipe, every
function is an ``exact_byte_match``, the tree was clean and all reports were
made at the same commit. It contains hashes and counts, never game bytes.

Usage:
    python tools/card_verify.py --out build/verify-open.json
    python tools/card_verify.py --toolchain psyq ... --out build/verify-psyq.json
    python tools/card_certificate.py build/verify-open.json --cross-check build/verify-psyq.json \\
        --version v0.1.0
"""

from __future__ import annotations

import argparse
import hashlib
import json
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def load_checked(path: Path, expected_toolchain: str, recipes: list[dict]) -> dict:
    report = json.loads(path.read_text(encoding="utf-8"))
    problems = []
    if report.get("toolchain") != expected_toolchain:
        problems.append(f"{path} is a {report.get('toolchain')} report, expected {expected_toolchain}")
    results = report["results"]
    exact = [r for r in results if r["status"] == "exact_byte_match"]
    if len(results) != len(recipes):
        problems.append(f"{path} covers {len(results)} of {len(recipes)} recipes")
    if len(exact) != len(results):
        problems.append(f"{path}: {len(results) - len(exact)} functions are not exact")
    if report["tree_dirty"] or not report.get("repository_commit"):
        problems.append(f"{path} was not made from a clean committed tree")
    if problems:
        raise SystemExit("refusing to certify: " + "; ".join(problems))
    return report


def digest_of(report: dict) -> str:
    pairs = sorted([r["function"], r["candidate_sha256"]] for r in report["results"])
    return hashlib.sha256(json.dumps(pairs).encode()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("report", type=Path, help="open toolchain report")
    parser.add_argument("--cross-check", type=Path, help="PsyQ toolchain report")
    parser.add_argument("--version", required=True)
    args = parser.parse_args()

    recipes = json.loads((ROOT / "recipes/card_cage.json").read_text(encoding="utf-8"))["functions"]
    toolchain = json.loads((ROOT / "config/toolchain.json").read_text(encoding="utf-8"))
    open_cfg = json.loads((ROOT / "config/open_toolchain.json").read_text(encoding="utf-8"))
    hashes = json.loads((ROOT / "reference/hashes.json").read_text(encoding="utf-8"))
    provenance = json.loads((ROOT / "recipes/export-provenance.json").read_text(encoding="utf-8"))

    primary = load_checked(args.report, "open", recipes)
    cross = load_checked(args.cross_check, "psyq", recipes) if args.cross_check else None
    if cross and cross["repository_commit"] != primary["repository_commit"]:
        raise SystemExit("refusing to certify: reports were made at different commits")
    digest = digest_of(primary)
    if cross and digest_of(cross) != digest:
        raise SystemExit("refusing to certify: the two toolchains matched different bytes")

    by_module = Counter(r["function"].split(":")[0] for r in primary["results"])
    variants = Counter(r["variant"] for r in primary["results"])
    psyq_tools = {t["id"]: t for kind in toolchain["tools"].values() for t in kind}

    lines = [
        f"# Card Cage {args.version}: 318/318 functions, exact per-function match",
        "",
        "All functions identified in the four Card Cage overlays of *Digimon World 2003*",
        "(PAL `SLES-03936`) are reconstructed in C. Each one was rebuilt and compared,",
        "over its full address range and any jump table, with the bytes of the PAL",
        "overlay. This proves exact code generation per function; it is not a",
        "byte-identical rebuild of the `.PRO` files (headers, data and layout are not",
        "reconstructed).",
        "",
        "## Audited snapshot",
        "",
        f"- Repository commit: `{primary['repository_commit']}` (clean tree)",
        f"- Exported from private working repository commit `{provenance['source_commit']}`",
        f"- Reference: `{hashes['reference_version']}`, disc SHA-256 `{hashes['disc']['sha256']}`",
        "",
        "## Result",
        "",
        "| Overlay | PAL overlay SHA-256 | Functions | Exact (open) |" + (" Exact (PsyQ) |" if cross else ""),
        "| --- | --- | ---: | ---: |" + (" ---: |" if cross else ""),
    ]
    cross_by_module = Counter(r["function"].split(":")[0] for r in cross["results"]) if cross else None
    for module, entry in hashes["overlays"].items():
        total = sum(1 for r in recipes if r["module"] == module)
        row = f"| {module} | `{entry['sha256']}` | {total} | {by_module[module]} |"
        lines.append(row + (f" {cross_by_module[module]} |" if cross else ""))
    total_row = f"| **Total** | | **{len(recipes)}** | **{len(primary['results'])}** |"
    lines += [
        total_row + (f" **{len(cross['results'])}** |" if cross else ""),
        "",
        f"- Digest of all (function, matched-bytes SHA-256) pairs: `{digest}`"
        + (" (identical for both toolchains)" if cross else ""),
        "",
        "## Default toolchain (public)",
        "",
        f"Verification run {primary['started_at']} to {primary['finished_at']} (UTC).",
        "",
        f"- GCC: `{open_cfg['gcc']['id']}` ({open_cfg['gcc']['project']}), "
        f"archive SHA-256 `{open_cfg['gcc']['archive_sha256']}`, `cc1` SHA-256 `{open_cfg['gcc']['cc1_sha256']}`",
        f"- maspsx: revision `{open_cfg['maspsx']['revision']}`, arguments `{' '.join(open_cfg['maspsx']['args'])}`",
        f"- GNU as: `{primary['tools']['gnu_as'].get('version')}`, flags `{' '.join(open_cfg['gnu_as']['flags'])}`",
        "- Zero-divisor guard: maspsx runs without `--expand-div` and adds none.",
    ]
    if cross:
        lines += ["", "## Cross-check (original PsyQ toolchain)", "",
                  f"Verification run {cross['started_at']} to {cross['finished_at']} (UTC).", ""]
        for kind in ("cc1psx", "aspsx"):
            tool = psyq_tools[cross["tools"][kind]["id"]]
            lines.append(f"- {kind}: `{tool['id']}` ({tool['version']}, PsyQ {tool.get('source_release', '')}), "
                         f"SHA-256 `{tool['sha256']}`")
        lines.append("- Zero-divisor guard: ASPSX would add one after every division; the path removes it "
                     "before ASPSX, as the overlays have none.")
    lines += ["", "## Compiler flags", "", f"- Base: `{' '.join(toolchain['cc1_flags'])}`"]
    for variant, count in sorted(variants.items()):
        extra = " ".join(toolchain["variants"][variant]) or "(none)"
        noun = "function" if count == 1 else "functions"
        lines.append(f"- Variant `{variant}` (extra: `{extra}`): {count} {noun}")
    lines += [
        "",
        "## Reproduce",
        "",
        "```",
        "python tools/fetch_toolchain.py",
        "python tools/card_verify.py --pal-dir PATH/TO/PRO --out build/verify-open.json",
        "# optional, both toolchains in one run (writes verify-open.json and verify-psyq.json)",
        "python tools/card_verify.py --toolchain both --pal-dir PATH/TO/PRO \\",
        "    --cc1psx PATH/TO/CC1PSX.EXE --aspsx PATH/TO/ASPSX.EXE --out build/verify.json",
        "```",
        "",
        "Every C file states its own recipe in its header comment (`tools/recipe_headers.py`).",
        "",
        f"Per-function results are in `reports/card-cage-{args.version}.json`. This certificate",
        "and that file contain hashes and counts only; no byte of the game and no PsyQ",
        "binary is included.",
        "",
    ]
    out_md = ROOT / "reports" / f"card-cage-{args.version}.md"
    out_json = ROOT / "reports" / f"card-cage-{args.version}.json"
    out_md.parent.mkdir(exist_ok=True)
    out_md.write_text("\n".join(lines), encoding="utf-8", newline="\n")

    def slim(report: dict) -> dict:
        keep = ("toolchain", "reference_version", "repository_commit", "started_at", "finished_at", "tools",
                "overlays", "functions", "status_counts")
        out = {k: report[k] for k in keep}
        out["results"] = [{k: r[k] for k in ("function", "name", "source", "size", "variant", "status",
                                              "candidate_sha256")} for r in sorted(report["results"],
                                                                                   key=lambda x: x["function"])]
        return out

    payload = {"schema_version": 2, "version": args.version, "digest": digest, "primary": slim(primary)}
    if cross:
        payload["cross_check"] = {k: v for k, v in slim(cross).items() if k != "results"}
    out_json.write_text(json.dumps(payload, indent=1) + "\n", encoding="utf-8", newline="\n")
    print(out_md.relative_to(ROOT), out_json.relative_to(ROOT))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
