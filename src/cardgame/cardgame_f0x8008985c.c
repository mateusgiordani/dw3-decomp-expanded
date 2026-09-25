/*
 * CARDGAME:0x8008985c CARDGAME_F0x8008985c
 * 280 bytes at CARDGAME.PRO offset 0x6bac (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float
 *  Variant     base
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8008985c
 *  Symbols     D_8004dc0c=0x8004dc0c
 *  Compare     280 bytes from 0x8008985c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008985c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Direct PAL callers target the global callback load eight bytes before the
 * former framed entry.
 *
 * The callback is loaded from 0x8004dc0c; no incoming-v0 ABI.
 */

#include <stdint.h>

typedef int32_t (*cardgame_flag_cb_t)(void);
extern cardgame_flag_cb_t D_8004dc0c;
typedef void (*cardgame_m4_t)(void *p, int32_t a1, int32_t a2, int32_t a3);
typedef void (*cardgame_m3_t)(void *p, int32_t a1, int32_t a2);

void CARDGAME_F0x8008985c(void *p1, void *p2)
{
    int32_t flag = D_8004dc0c() & 1;

    *(int32_t *)((uint8_t *)p1 + 0x434) = flag;
    *(int32_t *)((uint8_t *)p1 + 0x430) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x42c) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x428) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x424) = 0;
    ((cardgame_m4_t)*(uint32_t *)((uint8_t *)p2 + 0xf14))(p2, 0, 0x7400, 0x6100);
    ((cardgame_m4_t)*(uint32_t *)((uint8_t *)p2 + 0xf14))(p2, 1, 0xa400, 0x6100);
    if (flag != 0) {
        ((cardgame_m3_t)*(uint32_t *)((uint8_t *)p2 + 0xf3c))(p2, 0, 0x57);
        ((cardgame_m3_t)*(uint32_t *)((uint8_t *)p2 + 0xf3c))(p2, 1, 0x58);
    } else {
        ((cardgame_m3_t)*(uint32_t *)((uint8_t *)p2 + 0xf3c))(p2, 1, 0x57);
        ((cardgame_m3_t)*(uint32_t *)((uint8_t *)p2 + 0xf3c))(p2, 0, 0x58);
    }
    *(uint8_t *)((uint8_t *)p2 + 0x14d) = 2;
    *(uint8_t *)((uint8_t *)p2 + 0x199) = 2;
    *(uint16_t *)((uint8_t *)p2 + 0x120) = 0;
    *(uint8_t *)((uint8_t *)p2 + 0x151) = 0;
    *(uint8_t *)((uint8_t *)p2 + 0x150) = 0;
    *(uint16_t *)((uint8_t *)p2 + 0x16c) = 0;
    *(uint8_t *)((uint8_t *)p2 + 0x19d) = 0;
    *(uint8_t *)((uint8_t *)p2 + 0x19c) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x43c) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x440) = 0;
    *(uint8_t *)((uint8_t *)p1 + 0x422) = 1;
}
