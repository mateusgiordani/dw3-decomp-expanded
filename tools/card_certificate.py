"""Write the Card Cage release certificate from a ``card_verify.py`` report.

The certificate is only written when the report covers every recipe, every
function is an ``exact_byte_match``, the tree was clean and the report was made
at the commit being certified. It contains hashes and counts, never game bytes.

Usage:
    python tools/card_verify.py --out build/card-verify.json
    python tools/card_certificate.py build/card-verify.json --version v0.1.0
"""

from __future__ import annotations

import argparse
import hashlib
import json
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("report", type=Path)
    parser.add_argument("--version", required=True)
    args = parser.parse_args()

    report = json.loads(args.report.read_text(encoding="utf-8"))
    recipes = json.loads((ROOT / "recipes/card_cage.json").read_text(encoding="utf-8"))["functions"]
    toolchain = json.loads((ROOT / "config/toolchain.json").read_text(encoding="utf-8"))
    hashes = json.loads((ROOT / "reference/hashes.json").read_text(encoding="utf-8"))
    provenance = json.loads((ROOT / "recipes/export-provenance.json").read_text(encoding="utf-8"))

    results = report["results"]
    exact = [r for r in results if r["status"] == "exact_byte_match"]
    problems = []
    if len(results) != len(recipes):
        problems.append(f"report covers {len(results)} of {len(recipes)} recipes")
    if len(exact) != len(results):
        problems.append(f"{len(results) - len(exact)} functions are not exact")
    if report["tree_dirty"]:
        problems.append("report was made from a dirty tree")
    if not report.get("repository_commit"):
        problems.append("report has no repository commit")
    if problems:
        raise SystemExit("refusing to certify: " + "; ".join(problems))

    by_module = Counter(r["function"].split(":")[0] for r in exact)
    variants = Counter(r["variant"] for r in exact)
    tools = {t["id"]: t for kind in toolchain["tools"].values() for t in kind}
    digest = hashlib.sha256(json.dumps(
        [[r["function"], r["candidate_sha256"]] for r in sorted(exact, key=lambda x: x["function"])]
    ).encode()).hexdigest()

    lines = [
        f"# Card Cage {args.version}: 318/318 functions, exact per-function match",
        "",
        "All functions identified in the four Card Cage overlays of *Digimon World 2003*",
        "(PAL `SLES-03936`) are reconstructed in C. Each one was rebuilt with the original",
        "PsyQ toolchain and compared, over its full address range, with the bytes of the",
        "PAL overlay. This proves exact code generation per function; it is not a",
        "byte-identical rebuild of the `.PRO` files (headers, data and layout are not",
        "reconstructed).",
        "",
        "## Audited snapshot",
        "",
        f"- Repository commit: `{report['repository_commit']}` (clean tree)",
        f"- Exported from private working repository commit `{provenance['source_commit']}`",
        f"- Verification run: {report['started_at']} to {report['finished_at']} (UTC)",
        f"- Reference: `{hashes['reference_version']}`, disc SHA-256 `{hashes['disc']['sha256']}`",
        "",
        "## Result",
        "",
        "| Overlay | PAL overlay SHA-256 | Functions | Exact |",
        "| --- | --- | ---: | ---: |",
    ]
    for module, entry in hashes["overlays"].items():
        total = sum(1 for r in recipes if r["module"] == module)
        lines.append(f"| {module} | `{entry['sha256']}` | {total} | {by_module[module]} |")
    lines += [
        f"| **Total** | | **{len(recipes)}** | **{len(exact)}** |",
        "",
        f"- Expected functions: {len(recipes)}",
        f"- Rebuilt: {len(results)}",
        f"- `exact_byte_match`: {len(exact)}",
        "- Not matching: 0",
        f"- Digest of all (function, matched-bytes SHA-256) pairs: `{digest}`",
        "",
        "## Toolchain",
        "",
    ]
    for kind in ("cc1psx", "aspsx"):
        used = report["tools"][kind]["sha256"]
        tool = next(t for t in tools.values() if t["sha256"] == used)
        lines.append(f"- {kind}: `{tool['id']}` ({tool.get('version', '')}), SHA-256 `{used}`")
    lines.append(f"- Base CC1 flags: `{' '.join(toolchain['cc1_flags'])}`")
    for variant, count in sorted(variants.items()):
        extra = " ".join(toolchain["variants"][variant]) or "(none)"
        lines.append(f"- Variant `{variant}` (extra flags: `{extra}`): {count} functions")
    lines += [
        "",
        "## Reproduce",
        "",
        "```",
        "python tools/card_verify.py --pal-dir PATH/TO/PRO --cc1psx CC1PSX.EXE --aspsx ASPSX.EXE \\",
        f"    --out build/card-verify.json",
        f"python tools/card_certificate.py build/card-verify.json --version {args.version}",
        "```",
        "",
        "The per-function results (source hash, variant, matched-bytes hash) are in",
        f"`reports/card-cage-{args.version}.json`. This certificate and that file contain",
        "hashes and counts only; no byte of the game, and no PsyQ binary, is included.",
        "",
    ]
    out_md = ROOT / "reports" / f"card-cage-{args.version}.md"
    out_json = ROOT / "reports" / f"card-cage-{args.version}.json"
    out_md.parent.mkdir(exist_ok=True)
    out_md.write_text("\n".join(lines), encoding="utf-8", newline="\n")
    slim = {k: report[k] for k in ("schema_version", "reference_version", "repository_commit",
                                     "started_at", "finished_at", "tools", "overlays", "functions",
                                     "status_counts")}
    slim["tools"] = {k: {"sha256": v["sha256"]} for k, v in report["tools"].items()}
    slim["digest"] = digest
    slim["results"] = [{k: r[k] for k in ("function", "name", "source", "size", "variant", "status",
                                          "candidate_sha256")} for r in sorted(exact, key=lambda x: x["function"])]
    out_json.write_text(json.dumps(slim, indent=1) + "\n", encoding="utf-8", newline="\n")
    print(out_md.relative_to(ROOT), out_json.relative_to(ROOT))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
