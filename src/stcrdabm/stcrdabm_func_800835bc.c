/*
 * STCRDABM:0x800835bc STCRDABM_func_800835bc
 * 376 bytes at STCRDABM.PRO offset 0x90c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800835bc
 *  Symbols     D_80044B38=0x80044b38 D_80048D34=0x80048d34
 *              F0x8001f648=0x8001f648
 *  Compare     376 bytes from 0x800835bc against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x800835bc
 */

#include "common/types.h"

extern void F0x8001f648(uint8_t *work);
extern uint8_t D_80048D34[];
extern int32_t D_80044B38[];

typedef struct {
    uint8_t pad[0x74];
    void (*fn_ecbc)(int32_t, int32_t);
    void (*fn_ecd8)(void);
    void (*fn_ed34)(int32_t, int32_t);
    void (*fn_ecf8)(void);
    void (*fn_ed6c)(int32_t, int32_t, int32_t, int32_t);
    void (*fn_ecec)(int32_t);
    void (*fn_f5d0)(void);
    void (*fn_f5ec)(void);
    void (*fn_f608)(void);
    void (*fn_f61c)(void);
    void (*fn_f628)(void);
} DrawWork;

/* STCRDABM:0x800835bc (376 bytes, 94 instructions); PAL reviewed body.
 * Draws the 6-column card album grid: per card, icon call when the album
 * flag byte is set or card >= 0x13b (315), else the blank call; then blit
 * at x = col*42+35, y = row*54+50.
 * Col-first order steers GCC 2.8.1 -O2 allocation to the PAL homes
 * (card->v1, row->s0). Duplicated ecec calls per arm are required: the
 * scheduler keeps the callee load inside each arm (icon: lw a0, lw v0, j;
 * blank: lw v0, clear a0) and cross-jumping shares only the single jalr.
 * A single post-diamond call instead tail-merges the load (13-byte delta).
 */
void STCRDABM_func_800835bc(int32_t object) {
    DrawWork work;
    int32_t i;
    int32_t row;
    int32_t col;
    int32_t card;

    F0x8001f648((uint8_t *)&work);
    work.fn_ed34(*(int32_t *)(object + 0x54), *(int32_t *)(object + 0x58) - 1);
    work.fn_ecbc(0x280, 0);
    for (i = 0; i < *(int32_t *)(object + 0x68); i++) {
        col = i % 6;
        card = *(int32_t *)(object + 0x5c) + i;
        row = i / 6;
        if (D_80048D34[card + 0x4df] != 0 || card >= 0x13b)
            work.fn_ecec(*(int32_t *)(object + 0x70));
        else
            work.fn_ecec(0);
        work.fn_ed6c(((int32_t (*)(int32_t))D_80044B38[0x424 / 4])(0x6050000), 6, col * 0x2a + 0x23, row * 0x36 + 0x32);
    }
}
