# Card Cage v0.1.0: 318/318 functions, exact per-function match

All functions identified in the four Card Cage overlays of *Digimon World 2003*
(PAL `SLES-03936`) are reconstructed in C. Each one was rebuilt and compared,
over its full address range and any jump table, with the bytes of the PAL
overlay. This proves exact code generation per function; it is not a
byte-identical rebuild of the `.PRO` files (headers, data and layout are not
reconstructed).

## Audited snapshot

- Repository commit: `24beb19a1011af45c04a94129ba44e89ed5f414c` (clean tree)
- Exported from private working repository commit `51b751f6d796338019510280273f45b6c70eda4d`
- Reference: `PAL-SLES-03936`, disc SHA-256 `fb70dc9a995aed628cf515cabc87c7b14e5142559076ec394dfe793ec3e26a04`

## Result

| Overlay | PAL overlay SHA-256 | Functions | Exact (open) | Exact (PsyQ) |
| --- | --- | ---: | ---: | ---: |
| CARDGAME | `2886ada962c4893d321bead7db4294e09cb586212db013bb69ce14e64f770498` | 199 | 199 | 199 |
| STCRDABM | `6293e8c1379258dc9607c26a732fa1f3e6c96b3257b0b68db0a3263a88853722` | 27 | 27 | 27 |
| STCRDDEK | `07a853a5774abf4b9f4e3cf57d93bb3f144dd1bde509b8472e4ff569d81c3d73` | 48 | 48 | 48 |
| STCRDSHP | `f8daf77958e9ff5ffe9c108e4c1cdb46fd4826f44881eec6a71972ccd64b3230` | 44 | 44 | 44 |
| **Total** | | **318** | **318** | **318** |

- Digest of all (function, matched-bytes SHA-256) pairs: `6152e5110d04d504d36f235068167e321bc39412e2c1bd4639313cdc8c10de8a` (identical for both toolchains)

## Default toolchain (public)

Verification run 2026-09-25T02:17:31+00:00 to 2026-09-25T02:19:18+00:00 (UTC).

- GCC: `gcc-2.8.1-psx` (decompals/old-gcc (GNU GCC 2.8.1 built for PlayStation)), archive SHA-256 `e5a4fad3b93a16683fb16d1920074be5a81446563348ba759c37aced85c4bdbf`, `cc1` SHA-256 `60d886cd75bbd7855fc7909224a15401de76bff21af8a629c2060290a073f5fd`
- maspsx: revision `874855c53f65f8fa57447e1da6bde6236dbef9d5`, arguments `--aspsx-version=2.79`
- GNU as: `GNU assembler (GNU Binutils for Ubuntu) 2.45.90.20260125`, flags `-EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0`
- Zero-divisor guard: maspsx runs without `--expand-div` and adds none.

## Cross-check (original PsyQ toolchain)

Verification run 2026-09-25T02:19:18+00:00 to 2026-09-25T02:19:23+00:00 (UTC).

- cc1psx: `psyq-gcc-2.8.1-sn32-4.0.0010` (2.8.1 SN32 BUILD 4.0.0010, PsyQ Runtime Library 4.3 or 4.4), SHA-256 `26eb8259fa3e077d1980eb1e0c942006752135953dd03173200bf99ef6f5b6c9`
- aspsx: `aspsx-2.79` (2.79, PsyQ Runtime Library 4.4), SHA-256 `fcb987495ba18f9d8b5dc29696f7e6650091d3d86471c60b2a8aa00396888992`
- Zero-divisor guard: ASPSX would add one after every division; the path removes it before ASPSX, as the overlays have none.

## Compiler flags

- Base: `-quiet -O2 -G0 -mips1 -msoft-float`
- Variant `base` (extra: `(none)`): 293 functions
- Variant `o2-g0-no-strength-reduce` (extra: `-fno-strength-reduce`): 24 functions
- Variant `o2-g0-nosr-nosplit` (extra: `-fno-strength-reduce -mno-split-addresses`): 1 function

## Reproduce

```
python tools/fetch_toolchain.py
python tools/card_verify.py --pal-dir PATH/TO/PRO --out build/verify-open.json
```

Per-function results are in `reports/card-cage-v0.1.0.json`. This certificate
and that file contain hashes and counts only; no byte of the game and no PsyQ
binary is included.
