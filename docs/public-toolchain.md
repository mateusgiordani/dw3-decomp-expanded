# Open-source toolchain equivalence

Question: can the Card Cage matches be reproduced without the proprietary
PsyQ SDK?

## Result (2026-09-25)

All 318 functions reproduce the PAL bytes with open-source tools only.

| Stage | Result |
| --- | --- |
| Compiler: open-source `cc1` vs PsyQ `CC1PSX.EXE` 2.8.1 SN32 4.0.0010, same flags | identical assembly for 318/318 |
| Assembler + link: maspsx `--aspsx-version=2.79` + GNU `as`, ELF linked at the PAL address | `exact_byte_match` 318/318 |

Per-function results: `reports/experiments/public-toolchain-2026-09-25.json`.

## Tools

| Tool | Source | License |
| --- | --- | --- |
| `gcc-2.8.1-psx` (`cc1`) | decompals/old-gcc release 0.12, archive SHA-256 `e5a4fad3b93a16683fb16d1920074be5a81446563348ba759c37aced85c4bdbf` | GPL (GNU GCC) |
| maspsx | mkst/maspsx, revision `874855c53f65f8fa57447e1da6bde6236dbef9d5` | MIT |
| GNU `as` | `binutils-mipsel-linux-gnu` (Ubuntu), 2.45.90 | GPL |
| clang | preprocessor only (`-E -nostdinc`) | Apache-2.0 with LLVM exception |

Assembler flags: `-EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0`.

## Zero-divisor guard

The Card Cage overlays have no zero-divisor guard after their divisions.

- ASPSX 2.79 inserts one after every `div`/`divu`. The PsyQ path must remove it
  from the compiler output before assembling. Measured on 2026-09-25: 21 of the
  318 functions contain a division; without the removal three of them assemble
  larger than PAL (CARDGAME `0x80093710` +36 bytes, `0x80097508` +72,
  `0x8009a1a0` +180). With the removal applied to every function (a no-op when
  there is no division) all 318 match.
- maspsx inserts the guard only with `--expand-div`. The public path does not
  pass it, so it needs no removal.

## Controls

The ELF comparator rejects a symbol moved by 4 bytes, a function linked 8 bytes
off and a jump table moved by 4 bytes, and refuses to link one function's
object with another function's symbols.

## Limits

- Measured for these 318 functions and these three flag sets (`base`,
  `-fno-strength-reduce`, `-fno-strength-reduce -mno-split-addresses`). It is
  not a claim about other overlays, other compiler versions or other ASPSX
  behaviour.
- `tools/public_toolchain_check.py` uses PsyQ `CC1PSX.EXE` for the
  compiler-stage comparison only. `tools/card_verify.py` needs only the public
  tools, which are the default.
