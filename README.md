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
| `reference/hashes.json` | hashes of the PAL disc and of the four overlays |
| `tools/card_verify.py` | rebuild every function and compare it with PAL (public toolchain by default) |
| `tools/fetch_toolchain.py` | download and check the pinned public toolchain |
| `tools/card_certificate.py` | write the release certificate from verification runs |
| `tools/public_toolchain_check.py` | research: compare the public and PsyQ compilers stage by stage |
| `tools/export/` | the maintainer script that exported this snapshot |
| `config/open_toolchain.json` | pinned public toolchain (default) |
| `config/toolchain.json` | compiler flags and the pinned PsyQ binaries (optional cross-check) |

Source headers keep the working notes written during recovery (evidence,
measurements, hypotheses). They are historical; the recipes and the
certificate are the authoritative record.

## What you need

This repository contains no game data and no proprietary tools.

**Always:** your own copy of the PAL disc (`SLES-03936`), with `CARDGAME.PRO`,
`STCRDABM.PRO`, `STCRDDEK.PRO` and `STCRDSHP.PRO` extracted from `AAA/PRO/`
(their SHA-256 values are in `reference/hashes.json`), Python 3.10 or newer, and
clang (used only as the C preprocessor).

**Default toolchain, all public** (Linux; on Windows through WSL):

| Tool | Version | License |
| --- | --- | --- |
| GCC 2.8.1 `cc1` | [decompals/old-gcc](https://github.com/decompals/old-gcc) release 0.12, `gcc-2.8.1-psx` | GPL |
| [maspsx](https://github.com/mkst/maspsx) | revision `874855c`, run as `--aspsx-version=2.79` | MIT |
| GNU `mipsel-linux-gnu-as` | binutils (e.g. `apt install binutils-mipsel-linux-gnu`) | GPL |

`python tools/fetch_toolchain.py` downloads GCC and maspsx into `toolchains/` and
checks every hash. With this toolchain all 318 functions match; its compiler
produces the same assembly as the PsyQ one for every function.

**Optional cross-check with the original PsyQ tools** (not distributed; supply
your own, identified by SHA-256 in `config/toolchain.json`):

| Tool | Version | Found in |
| --- | --- | --- |
| `CC1PSX.EXE` | GCC 2.8.1 SN32 BUILD 4.0.0010 | PsyQ Runtime Library 4.3 / 4.4 |
| `ASPSX.EXE` | 2.79 | PsyQ Runtime Library 4.4 |

ASPSX inserts a zero-divisor guard (`bnez`/`break`) after every division, which
the Card Cage overlays do not have. The PsyQ path therefore always removes that
guard from the compiler output before assembling (a no-op in functions without
division; 21 of the 318 have one). The public path needs no removal: maspsx adds
the guard only when given `--expand-div`, which is not passed.

## Verify

```
python tools/fetch_toolchain.py
python tools/card_verify.py --pal-dir PATH/TO/PRO --out build/verify-open.json

# optional cross-check with PsyQ
python tools/card_verify.py --toolchain psyq --pal-dir PATH/TO/PRO \
    --cc1psx PATH/TO/CC1PSX.EXE --aspsx PATH/TO/ASPSX.EXE --out build/verify-psyq.json
```

Paths can also be given through `DW3_PAL_DIR`, `DW3_CLANG`, `DW3_GNU_AS`,
`DW3_PSYQ_CC1PSX` and `DW3_PSYQ_ASPSX`. A run checks every hash before building
and exits non-zero unless all 318 functions match.

## Legal

*Digimon* and *Digimon World* are trademarks of their respective owners. This
project is not affiliated with or endorsed by them. See [`LICENSE.md`](LICENSE.md)
for what the project license covers, and what it does not.
