/*
 * CARDGAME:0x80086d78 CARDGAME_F0x80086d78
 * 312 bytes at CARDGAME.PRO offset 0x40c8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086d78
 *  Symbols     CARDGAME_F0x80085fd0=0x80085fd0 CARDGAME_F0x80086a18=0x80086a18
 *  Compare     312 bytes from 0x80086d78 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80086d78
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prev CARDGAME:0x80086d28 size 0x50 ends exactly at 0x80086d78; next
 * CARDGAME:0x80086eb0 at +0x138.
 *
 * Signature: void (a0 base, a1 engine-iface, a2 record); void confirmed by both
 * callers in CARDGAME:0x80084320 ignoring v0; epilogue li v0,-1 feeds sw only.
 *
 * a2 layout from PAL accesses: +0xa int16 count, +0xc byte table for 85fd0,
 * +0x11 flag byte, +0x64 40-entry int16 index array (sliding pointer +2/iter).
 *
 * a1 layout: +0xecc/0xec0 indirect slots called with (a1) / (a1, flag).
 *
 * Codegen notes (each verified by pipeline diff): outer if uses != 0 with the
 * loop as fallthrough (beq-to-skip, no extra j); explicit int one = 1 declared
 * between i and q reproduces li s5,1 before move s2,s3; sliding byte pointer q
 * (+2/iter, lh 0x64(q)) reproduces the index loads without shift/addu.
 */

#include "common/types.h"

extern unsigned int CARDGAME_F0x80085fd0(void *p, unsigned char *k, int idx);
extern void CARDGAME_F0x80086a18(void *a0, void *a1);

void CARDGAME_F0x80086d78(void *a0, void *a1, void *a2)
{
    if (*(int16_t *)((char *)a2 + 0xa) != 0) {
        int32_t i = 0;
        int32_t one = 1;
        unsigned char *q = (unsigned char *)a2;
        do {
            unsigned char *row = (unsigned char *)a0 + i;
            row[0x446] = 0;
            row[0x46f] = 0;
            if (i < *(int16_t *)((char *)a2 + 0xa)) {
                if (CARDGAME_F0x80085fd0(a0, (unsigned char *)a2 + 0xc, *(int16_t *)(q + 0x64)) != 0) {
                    row[0x446] = one;
                    row[0x49e] = 0;
                } else {
                    row[0x49e] = one;
                }
            }
            i++;
            q += 2;
        } while (i < 0x28);
    } else {
        ((unsigned char *)a0)[0x49e] = 0;
    }
    (*(void (**)(void *))((char *)a1 + 0xecc))(a1);
    if (*(unsigned char *)((char *)a2 + 0x11) == 0) {
        ((unsigned char *)a0)[0x499] = 5;
        (*(void (**)(void *, int))((char *)a1 + 0xec0))(a1, *(unsigned char *)((char *)a2 + 0x11));
    } else {
        ((unsigned char *)a0)[0x499] = 0xb;
    }
    ((unsigned char *)a0)[0x423] = 1;
    CARDGAME_F0x80086a18(a0, a1);
    ((int32_t *)((char *)a0 + 0x43c))[0] = 0;
    ((int32_t *)((char *)a0 + 0x440))[0] = -1;
}
