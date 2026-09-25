# dw3-decomp-expanded

A matching decompilation of *Digimon World 2003* / *Digimon World 3*
(PAL, `SLES-03936`), starting with the **Card Cage**.

## Milestone: Card Cage (v0.1.0)

All 318 functions identified in the four Card Cage overlays are reconstructed in
C. Each one was rebuilt with the original PsyQ toolchain and verified to match,
over its full address range, the bytes of the PAL reference.

| Overlay | Functions |
| --- | ---: |
| `CARDGAME` | 199 |
| `STCRDABM` | 27 |
| `STCRDDEK` | 48 |
| `STCRDSHP` | 44 |
| **Total** | **318** |

The reproducible certificate is in [`reports/`](reports/). It proves exact code
generation for each function; it is not a byte-identical rebuild of the `.PRO`
files, whose headers, data and layout are not reconstructed here.

## Layout

| Path | Contents |
| --- | --- |
| `src/<overlay>/` | the 318 recovered C functions |
| `include/` | the headers they use |
| `symbols/card_cage_functions.csv` | inventory: address, size, name, source hash |
| `recipes/card_cage.json` | per-function build and link recipe (base, size, symbols, jump table, compiler variant) |
| `config/toolchain.json` | pinned compiler flags and tool hashes |
| `reference/hashes.json` | hashes of the PAL disc and of the four overlays |
| `tools/card_verify.py` | rebuild every function and compare it with PAL |
| `tools/card_certificate.py` | write the release certificate from a verification run |
| `tools/export/` | the maintainer script that exported this snapshot |

Source headers keep the working notes written during recovery (evidence,
measurements, hypotheses). They are historical; the recipes and the
certificate are the authoritative record.

## What is not here

This repository contains no game data and no proprietary tools. To verify the
matches you need:

- your own copy of the PAL disc, with `CARDGAME.PRO`, `STCRDABM.PRO`,
  `STCRDDEK.PRO` and `STCRDSHP.PRO` extracted from `AAA/PRO/` (their SHA-256
  values are in `reference/hashes.json`);
- the PsyQ `CC1PSX.EXE` (GCC 2.8.1, SN32 4.0.0010) and `ASPSX.EXE` 2.79, matching
  the hashes in `config/toolchain.json`;
- clang, used only as the C preprocessor;
- Python 3.10 or newer.

## Verify

```
python tools/card_verify.py --pal-dir PATH/TO/PRO --cc1psx PATH/TO/CC1PSX.EXE \
    --aspsx PATH/TO/ASPSX.EXE --out build/card-verify.json
```

The tools can also be given through `DW3_PAL_DIR`, `DW3_PSYQ_CC1PSX`,
`DW3_PSYQ_ASPSX` and `DW3_CLANG`. The PsyQ tools are Win32 programs; on other
systems pass a wrapper command. A run checks every hash before building and
exits non-zero unless all 318 functions match.

## Legal

*Digimon* and *Digimon World* are trademarks of their respective owners. This
project is not affiliated with or endorsed by them. See [`LICENSE.md`](LICENSE.md)
for what the project license covers, and what it does not.
