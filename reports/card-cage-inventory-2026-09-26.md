# Card Cage inventory correction — 2026-09-26

The 407 published C matches remain valid. The catalog was incomplete: independent PAL/Ghidra review confirmed 28 additional functions, totaling 2,512 bytes. Milestone 1 is reopened.

The earlier scanner found framed functions and leaves reached by an internal `jal`. It missed callbacks stored as pointers, including functions already mentioned by existing C. The revised coverage gate independently flags unowned `jr ra` instructions and their delay slots. It failed on 45 return sites before reconciliation and passes after the 28 reviewed ranges are added. Return sites are not function counts.

Comparison with DW3-Port-PC supplied leads, not source authority. Ten of its recipe entries at `CARDGAME:0x8009cb08..0x8009cc28` are internal cases of the single function `CARDGAME:0x8009cae0..0x8009cc4c`: the PAL switch reads ten targets from `0x80083778` and dispatches through `jr v0` at `0x8009cb00`. They are not counted as ten independent functions.

No external C was imported. By maintainer policy, 24 functions with existing external C and the switch with external C fragments remain blocked. Three other bodies are eligible for original PAL recovery; their names already occur as external labels, but neither project has their C bodies. That recovery is currently blocked by the CARDGAME Ghidra analysis/save gate (`Unable to lock due to active transaction`).

The table below records the newly reviewed boundaries; it does not claim matching C or add verification recipes for them.

| Module | Start | Exclusive end | Bytes | Recovery |
| --- | --- | --- | ---: | --- |
| CARDGAME | `0x80096144` | `0x80096150` | 12 | blocked_peer_existing_c |
| CARDGAME | `0x80096150` | `0x80096160` | 16 | blocked_peer_existing_c |
| CARDGAME | `0x80096160` | `0x80096180` | 32 | blocked_peer_existing_c |
| CARDGAME | `0x800967f8` | `0x80096808` | 16 | blocked_peer_existing_c |
| CARDGAME | `0x80096808` | `0x80096828` | 32 | blocked_peer_existing_c |
| CARDGAME | `0x8009be40` | `0x8009be78` | 56 | blocked_peer_existing_c |
| CARDGAME | `0x8009be78` | `0x8009beac` | 52 | blocked_peer_existing_c |
| CARDGAME | `0x8009bfcc` | `0x8009c010` | 68 | blocked_peer_existing_c |
| CARDGAME | `0x8009c010` | `0x8009c054` | 68 | blocked_peer_existing_c |
| CARDGAME | `0x8009c16c` | `0x8009c174` | 8 | blocked_peer_existing_c |
| CARDGAME | `0x8009c25c` | `0x8009c264` | 8 | blocked_peer_existing_c |
| CARDGAME | `0x8009c264` | `0x8009c2d0` | 108 | blocked_peer_existing_c |
| CARDGAME | `0x8009c2d0` | `0x8009c350` | 128 | eligible_original_pal_recovery; blocked Ghidra save |
| CARDGAME | `0x8009c350` | `0x8009c3c0` | 112 | blocked_peer_existing_c |
| CARDGAME | `0x8009c3c0` | `0x8009c438` | 120 | blocked_peer_existing_c |
| CARDGAME | `0x8009c438` | `0x8009c4a4` | 108 | blocked_peer_existing_c |
| CARDGAME | `0x8009c4a4` | `0x8009c4d8` | 52 | blocked_peer_existing_c |
| CARDGAME | `0x8009c4d8` | `0x8009c510` | 56 | blocked_peer_existing_c |
| CARDGAME | `0x8009c7f8` | `0x8009c898` | 160 | eligible_original_pal_recovery; blocked Ghidra save |
| CARDGAME | `0x8009ca80` | `0x8009cae0` | 96 | blocked_peer_existing_c |
| CARDGAME | `0x8009cae0` | `0x8009cc4c` | 364 | blocked_peer_partial_c |
| CARDGAME | `0x8009cc4c` | `0x8009cda4` | 344 | eligible_original_pal_recovery; blocked Ghidra save |
| CARDGAME | `0x8009cdfc` | `0x8009ce28` | 44 | blocked_peer_existing_c |
| STCRDABM | `0x800859a0` | `0x800859e0` | 64 | blocked_peer_existing_c |
| STCRDABM | `0x800859e0` | `0x80085a4c` | 108 | blocked_peer_existing_c |
| STCRDDEK | `0x8008a3d8` | `0x8008a418` | 64 | blocked_peer_existing_c |
| STCRDDEK | `0x8008a418` | `0x8008a484` | 108 | blocked_peer_existing_c |
| STCRDSHP | `0x800890f4` | `0x80089160` | 108 | blocked_peer_existing_c |

All four overlays use verified PAL base `0x80082cb0`. Every instruction in these ranges was checked against the extracted PAL bytes; indirect entry xrefs and full return paths established the boundaries.

| Module | PAL SHA-256 |
| --- | --- |
| CARDGAME | `2886ada962c4893d321bead7db4294e09cb586212db013bb69ce14e64f770498` |
| STCRDABM | `6293e8c1379258dc9607c26a732fa1f3e6c96b3257b0b68db0a3263a88853722` |
| STCRDDEK | `07a853a5774abf4b9f4e3cf57d93bb3f144dd1bde509b8472e4ff569d81c3d73` |
| STCRDSHP | `f8daf77958e9ff5ffe9c108e4c1cdb46fd4826f44881eec6a71972ccd64b3230` |

The revised inventory is 435 functions (407 matching, 28 pending). A clean coverage scan is still not proof that every remaining byte is classified: tail calls, non-returning code, data and overall overlay layout require their own evidence. Historical v0.2.0 certificates remain unchanged and attest only to their 407-function snapshot.
