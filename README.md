# dw3-decomp-expanded

A matching decompilation of *Digimon World 2003* / *Digimon World 3*
(PAL, `SLES-03936`), starting with the **Card Cage**.

## Purpose

The game shipped only as compiled MIPS machine code. This project rebuilds its
program logic as C source that anyone can read, study and modify. A *matching*
decompilation holds that C to a strict test: compiled with the same kind of
compiler the developers used, it must give back exactly the bytes on the CD.
Every function here passes that test. A plausible reading of the code is not
enough.

## What the C files deliver

Each file in `src/` is the C for one function of the game:

- **It compiles back to the original bytes.** With the compiler, flags and link
  addresses given in its header, it produces the exact machine code of that
  function, every byte of it, plus any jump table it uses. The comparison is
  against the overlay extracted from your own copy of the disc.
- **It carries its own recipe.** The comment at the top of every file says how
  it becomes a byte match: the compiler with its flags and variant, the
  assembler for both toolchains, how the zero-divisor guard is handled, where
  code and jump table are linked, the addresses of the symbols it references,
  and the command that checks it. The same data is in `recipes/card_cage.json`,
  and `tools/recipe_headers.py --check` keeps the two identical.
- **It is plain C.** No inline assembly, no hard-pinned registers, no padding
  or hand-placed instruction words.

Below the recipe, some files keep *recovery notes*: callers, data layouts, and
why an odd-looking construct is needed for the match. These are historical
observations from the recovery work, not verified claims.

### A byte match is not the original source

A byte match does not mean these are the files the developers wrote. Their
source code is lost, and many different C programs compile to the same machine
code. Names, types, variable splits, loop shapes and even statement order were
chosen here because they reproduce the bytes, not because anyone knows the
original was written that way. Most functions keep neutral names such as
`CARDGAME_F0x80084090` because nothing yet proves their purpose.

What *is* guaranteed: compiled as described in its header, each file generates
code that is byte-for-byte identical to the corresponding function extracted
from the CD.

## How a function reaches C matching

1. **Start from the PAL bytes.** The function's address range is read from the
   hashed PAL overlay and disassembled. The binary is the authority. Decompiler
   output and other projects' listings are hints only.
2. **Write C.** The C is shaped to express what the machine code does: widths,
   signedness, the order of memory accesses and calls.
3. **Compile with the period toolchain.** This is GCC 2.8.1 as shipped in the
   PsyQ SDK (`CC1PSX` 2.8.1 SN32 BUILD 4.0.0010), at `-O2 -G0 -mips1
   -msoft-float`. 25 functions also need `-fno-strength-reduce`, and one of
   those also needs `-mno-split-addresses`.
4. **Link at the real address.** The object is placed at the function's PAL
   address, and its jump table (40 functions have one) at the table's PAL
   address. External calls and data resolve to their PAL addresses.
5. **Compare every byte.** The whole function range and jump table are compared
   with the PAL overlay. Any difference means the C form is changed and step 3
   runs again, until zero bytes differ.
6. **Record the recipe.** The exact inputs of the match go into
   `recipes/card_cage.json` and into the header of the file.

One detail does not come from the compiler. The PsyQ assembler (ASPSX) inserts a
zero-divisor check after every division, but the game's overlays contain none.
The original build must have assembled divisions without it. On the PsyQ path
the check is therefore removed before assembling (`tools/div_guard.py`). Of the
318 functions, 21 divide. The public toolchain does not insert the check in the
first place.

## Milestone: Card Cage (v0.1.0)

All 318 functions identified in the four Card Cage overlays are recovered in C
and match byte for byte.

| Overlay | Functions |
| --- | ---: |
| `CARDGAME` | 199 |
| `STCRDABM` | 27 |
| `STCRDDEK` | 48 |
| `STCRDSHP` | 44 |
| **Total** | **318** |

The reproducible certificate is in [`reports/`](reports/). It proves exact code
generation for each function. It is not a byte-identical rebuild of the `.PRO`
files: their headers, data and layout are not reconstructed here.

## Layout

| Path | Contents |
| --- | --- |
| `src/<overlay>/` | the 318 recovered C functions, each with its byte-match recipe |
| `include/` | the headers they use |
| `recipes/card_cage.json` | the recipes in machine-readable form (base, size, flags, variant, symbols, jump table) |
| `symbols/card_cage_functions.csv` | inventory: address, size, name, source hash |
| `reference/hashes.json` | hashes of the PAL disc and of the four overlays |
| `tools/card_verify.py` | rebuild every function and compare it with PAL (`--toolchain open`, `psyq` or `both`) |
| `tools/fetch_toolchain.py` | download and check the pinned public toolchain |
| `tools/recipe_headers.py` | write or check the recipe header of every C file |
| `tools/card_certificate.py` | write the release certificate from verification runs |
| `tools/public_toolchain_check.py` | research: compare the public and PsyQ compilers stage by stage |
| `tools/export/` | the maintainer script that exported this snapshot |
| `tests/` | unit tests for the tools (no game data needed) |
| `config/open_toolchain.json` | pinned public toolchain (default) |
| `config/toolchain.json` | compiler flags and the pinned PsyQ binaries (optional cross-check) |

## What you need

This repository contains no game data and no proprietary tools.

**Always:**
- your own copy of the PAL disc (`SLES-03936`), with `CARDGAME.PRO`,
  `STCRDABM.PRO`, `STCRDDEK.PRO` and `STCRDSHP.PRO` extracted from `AAA/PRO/`
  (their SHA-256 values are in `reference/hashes.json`);
- Python 3.10 or newer;
- clang, used only as the C preprocessor.

**Default toolchain, all public** (Linux; on Windows through WSL):

| Tool | Version | License |
| --- | --- | --- |
| GCC 2.8.1 `cc1` | [decompals/old-gcc](https://github.com/decompals/old-gcc) release 0.12, `gcc-2.8.1-psx` | GPL |
| [maspsx](https://github.com/mkst/maspsx) | revision `874855c`, run as `--aspsx-version=2.79` | MIT |
| GNU `mipsel-linux-gnu-as` | binutils (e.g. `apt install binutils-mipsel-linux-gnu`) | GPL |

`python tools/fetch_toolchain.py` downloads GCC and maspsx into `toolchains/` and
checks every hash. With this toolchain all 318 functions match. Its compiler also
produces the same assembly as the PsyQ one for every function.

**Optional cross-check with the original PsyQ tools.** These are not
distributed; supply your own. `config/toolchain.json` identifies them by SHA-256.

| Tool | Version | Found in |
| --- | --- | --- |
| `CC1PSX.EXE` | GCC 2.8.1 SN32 BUILD 4.0.0010 | PsyQ Runtime Library 4.3 / 4.4 |
| `ASPSX.EXE` | 2.79 | PsyQ Runtime Library 4.4 |

## Verify

```
python tools/fetch_toolchain.py
python tools/card_verify.py --pal-dir PATH/TO/PRO --out build/verify-open.json

# one function
python tools/card_verify.py --pal-dir PATH/TO/PRO --only CARDGAME:0x80084090 -v

# both toolchains; fails if they disagree on any function's bytes
python tools/card_verify.py --toolchain both --pal-dir PATH/TO/PRO \
    --cc1psx PATH/TO/CC1PSX.EXE --aspsx PATH/TO/ASPSX.EXE --out build/verify.json
```

Paths can also be given through `DW3_PAL_DIR`, `DW3_CLANG`, `DW3_GNU_AS`,
`DW3_PSYQ_CC1PSX` and `DW3_PSYQ_ASPSX`. A run checks every hash before building,
and exits non-zero unless all 318 functions match.

The checks that need neither the game nor a compiler run in CI:

```
python -m unittest discover tests
python tools/recipe_headers.py --check
```

## Legal

*Digimon* and *Digimon World* are trademarks of their respective owners. This
project is not affiliated with or endorsed by them. See [`LICENSE.md`](LICENSE.md)
for what the project license covers, and what it does not.
