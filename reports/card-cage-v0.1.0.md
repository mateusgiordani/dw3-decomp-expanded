# Card Cage v0.1.0: 318/318 functions, exact per-function match

All functions identified in the four Card Cage overlays of *Digimon World 2003*
(PAL `SLES-03936`) are reconstructed in C. Each one was rebuilt with the original
PsyQ toolchain and compared, over its full address range, with the bytes of the
PAL overlay. This proves exact code generation per function; it is not a
byte-identical rebuild of the `.PRO` files (headers, data and layout are not
reconstructed).

## Audited snapshot

- Repository commit: `35b1fab22606fe5fbf3f71fc5aef8df1ec35de52` (clean tree)
- Exported from private working repository commit `51e21ea1031bf789a6159464d455783a3e9aa463`
- Verification run: 2026-09-25T01:38:14+00:00 to 2026-09-25T01:38:19+00:00 (UTC)
- Reference: `PAL-SLES-03936`, disc SHA-256 `fb70dc9a995aed628cf515cabc87c7b14e5142559076ec394dfe793ec3e26a04`

## Result

| Overlay | PAL overlay SHA-256 | Functions | Exact |
| --- | --- | ---: | ---: |
| CARDGAME | `2886ada962c4893d321bead7db4294e09cb586212db013bb69ce14e64f770498` | 199 | 199 |
| STCRDABM | `6293e8c1379258dc9607c26a732fa1f3e6c96b3257b0b68db0a3263a88853722` | 27 | 27 |
| STCRDDEK | `07a853a5774abf4b9f4e3cf57d93bb3f144dd1bde509b8472e4ff569d81c3d73` | 48 | 48 |
| STCRDSHP | `f8daf77958e9ff5ffe9c108e4c1cdb46fd4826f44881eec6a71972ccd64b3230` | 44 | 44 |
| **Total** | | **318** | **318** |

- Expected functions: 318
- Rebuilt: 318
- `exact_byte_match`: 318
- Not matching: 0
- Digest of all (function, matched-bytes SHA-256) pairs: `6152e5110d04d504d36f235068167e321bc39412e2c1bd4639313cdc8c10de8a`

## Toolchain

- cc1psx: `psyq-gcc-2.8.1-sn32-4.0.0010` (2.8.1 SN32 BUILD 4.0.0010), SHA-256 `26eb8259fa3e077d1980eb1e0c942006752135953dd03173200bf99ef6f5b6c9`
- aspsx: `aspsx-2.79` (2.79), SHA-256 `fcb987495ba18f9d8b5dc29696f7e6650091d3d86471c60b2a8aa00396888992`
- Base CC1 flags: `-quiet -O2 -G0 -mips1 -msoft-float`
- Variant `base` (extra flags: `(none)`): 292 functions
- Variant `o2-g0-no-strength-reduce` (extra flags: `-fno-strength-reduce`): 25 functions
- Variant `o2-g0-nosr-nosplit` (extra flags: `-fno-strength-reduce -mno-split-addresses`): 1 function

## Reproduce

```
python tools/card_verify.py --pal-dir PATH/TO/PRO --cc1psx CC1PSX.EXE --aspsx ASPSX.EXE \
    --out build/card-verify.json
python tools/card_certificate.py build/card-verify.json --version v0.1.0
```

The per-function results (source hash, variant, matched-bytes hash) are in
`reports/card-cage-v0.1.0.json`. This certificate and that file contain
hashes and counts only; no byte of the game, and no PsyQ binary, is included.
