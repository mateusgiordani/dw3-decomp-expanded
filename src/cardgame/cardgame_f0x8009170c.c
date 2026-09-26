/*
 * CARDGAME:0x8009170c CARDGAME_F0x8009170c
 * 332 bytes at CARDGAME.PRO offset 0xea5c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009170c, jump table (.rodata) at 0x8008357c
 *  Symbols     (none)
 *  Compare     332 bytes from 0x8009170c and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009170c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Marks which of the slots before the active one (count p1+0x575 - 1, 8-byte
 * records at p1+0x580) disable something for card `idx`: for each slot the flag
 * at p2 + idx*0x4c + slot + 0x146 is cleared, then set by the slot's kind byte
 * (+0x585, jump table 0x8008357c): kinds 1/2 compare the slot's side byte
 * (+0x584) with idx < 6, 3 always, 4..8 compare the card's value (p2 + idx*0x4c
 * + 0x140, plus one) with 1, 2, 3, 4 and 6. p3 is unused.
 *
 * Matching note: every store writes the slot offset as p2 + (off + i); the
 * identical per-site givs are combined and strength-reduced into one register
 * while p2 is added at each site, as in PAL.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

void CARDGAME_F0x8009170c(int32_t p1, int32_t p2, int32_t p3, int32_t idx)
{
    int32_t i;
    int32_t off;
    int32_t n;
    int32_t v;

    off = idx * 0x4c;
    n = S8(p1, 0x575) - 1;
    v = S16(p2 + off, 0x140) + 1;
    for (i = 0; i < n; i++) {
        U8(p2 + (off + i), 0x146) = 0;
        switch (U8(p1 + i * 8, 0x585)) {
        case 1:
            if (U8(p1 + i * 8, 0x584) == 0) {
                if (idx < 6)
                    U8(p2 + (off + i), 0x146) = 1;
            } else if (idx >= 6) {
                U8(p2 + (off + i), 0x146) = 1;
            }
            break;
        case 2:
            if (U8(p1 + i * 8, 0x584) == 0) {
                if (idx >= 6)
                    U8(p2 + (off + i), 0x146) = 1;
            } else if (idx < 6) {
                U8(p2 + (off + i), 0x146) = 1;
            }
            break;
        case 3:
            U8(p2 + (off + i), 0x146) = 1;
            break;
        case 4:
            if (v != 1)
                U8(p2 + (off + i), 0x146) = 1;
            break;
        case 5:
            if (v != 2)
                U8(p2 + (off + i), 0x146) = 1;
            break;
        case 6:
            if (v == 3)
                U8(p2 + (off + i), 0x146) = 1;
            break;
        case 7:
            if (v != 4)
                U8(p2 + (off + i), 0x146) = 1;
            break;
        case 8:
            if (v == 6)
                U8(p2 + (off + i), 0x146) = 1;
            break;
        }
    }
}
