/*
 * CARDGAME:0x8009d540 CARDGAME_F0x8009d540
 * 372 bytes at CARDGAME.PRO offset 0x1a890 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009d540
 *  Symbols     CARDGAME_F0x8009d540=0x8009d540 D_80044f3c=0x80044f3c
 *              D_80044f44=0x80044f44 D_8005cca8=0x8005cca8
 *              D_800a5bf0=0x800a5bf0
 *  Compare     372 bytes from 0x8009d540 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009d540
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Candidate status: EXACT (372B/372B, 0 diffs) - r5 S4, pending parent
 * reproduction r5 mechanism: integer table base + scaled offset before base
 * (offset-first addu), fresh idx re-read defeating CSE (reload lh + hazard
 * nop), pointer [1] second access (lh +2 offset, no ori).
 */

#include <stdint.h>

extern int16_t D_800a5bf0[];
extern uint16_t D_8005cca8;
extern int32_t D_80044f44;
extern int32_t D_80044f3c;

void CARDGAME_F0x8009d540(int32_t arg0)
{
    int32_t state;
    int32_t v1;

    state = *(int32_t *)(arg0 + 0xc);
    if (state == 1)
        goto body;
    if (state < 2)
        goto else_body;
    if (state < 4)
        goto out;
else_body:
    *(int16_t *)(arg0 + 0x54) = 0;
    ((void (*)(int32_t, int32_t))*(void **)(arg0 + 0x28))(arg0, 1);
    *(int32_t *)(arg0 + 0x50) = 0;
    goto out;
body:
    v1 = *(int32_t *)(arg0 + 0x10);
    if (v1 == 0)
        goto do_block;
    if (v1 == 1)
        goto skip_block;
do_block:
    {
        int16_t idx = *(int16_t *)(arg0 + 0x54);
        int32_t tbl = (int32_t)D_800a5bf0;
        int32_t off = (int32_t)idx * 4;
        int16_t val = *(int16_t *)(off + tbl);
        *(int16_t *)(arg0 + 0x56) = val;
        if (*(int16_t *)((int32_t)(*(int16_t *)(arg0 + 0x54)) * 4 + tbl + 2) != 0) {
            *(int16_t *)(arg0 + 0x56) = val + (int16_t)D_8005cca8;
        }
        ((void (*)(int32_t))D_80044f44)(*(int16_t *)(arg0 + 0x56));
        *(int32_t *)(arg0 + 0x10) = *(int32_t *)(arg0 + 0x10) + 1;
    }
skip_block:
    if (((int32_t (*)(int32_t))D_80044f3c)(*(int16_t *)(arg0 + 0x56)) != 0)
        goto out;
    {
        int16_t cur = *(int16_t *)(arg0 + 0x54);
        int16_t nxt = cur + 1;
        int32_t t;
        *(int16_t *)(arg0 + 0x54) = nxt;
        t = (int32_t)nxt * 4;
        if (D_800a5bf0[t >> 1] == -2) {
            *(int16_t *)(arg0 + 0x54) = cur + 2;
            *(int32_t *)(arg0 + 0x50) = 1;
        }
        if (D_800a5bf0[*(int16_t *)(arg0 + 0x54) * 2] == -1) {
            ((void (*)(int32_t, int32_t))*(void **)(arg0 + 0x28))(arg0, 3);
        }
        *(int32_t *)(arg0 + 0x10) = 0;
    }
out:
    return;
}
