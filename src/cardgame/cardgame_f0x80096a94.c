/*
 * CARDGAME:0x80096a94 CARDGAME_F0x80096a94
 * 396 bytes at CARDGAME.PRO offset 0x13de4 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80096a94
 *  Symbols     CARDGAME_F0x80096a94=0x80096a94 D0x80044b38=0x80044b38
 *              F0x8001f648=0x8001f648
 *  Compare     396 bytes from 0x80096a94 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80096a94
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdff30 addiu sp,-0xd0 ; epilogue jr ra / 27bd00d0 addiu sp,+0xd0
 *
 * Prev CARDGAME:0x80096950 (324B) ends at 0x80096a94; next CARDGAME:0x80096c20
 * at +0x18c.
 *
 * Data: EXE function-pointer word at 0x80044f5c, reached in PAL as lui
 * s5,0x8004 / addiu s5,0x4b38 / lw v0,0x424(s5) with the lui/addiu hoisted
 * above the digit loop; expressed here via an EXE base local so the pinned
 * psyq-gcc-2.8.1 -O2 hoist reproduces the PAL schedule. Digit rendering via
 * signed /10 and %10 (magic 0x66666667, mult/mfhi/sra/subu).
 */

#include <stdint.h>

extern void F0x8001f648(void *buf);
extern uint8_t D0x80044b38[];

typedef void (*CardCb2)(int32_t a0, int32_t a1);
typedef void (*CardCb3)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*CardCb4)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*ExeSlotFn)(int32_t a0);

typedef struct {
    uint8_t pad[0x74];
    CardCb2 cb74;
    CardCb2 cb78;
    CardCb2 cb7c;
    CardCb2 cb80;
    CardCb4 cb84;
    CardCb2 cb88;
    CardCb3 cb8c;
    CardCb2 cb90;
    CardCb2 cb94;
    CardCb2 cb98;
    CardCb2 cb9c;
} CardCtx96a94;

void CARDGAME_F0x80096a94(int16_t *a0, int32_t a1)
{
    CardCtx96a94 ctx;
    uint8_t *exebase;
    int32_t val;
    int32_t i;
    int32_t rem;
    int32_t k;
    int32_t r;

    F0x8001f648(&ctx);
    ctx.cb7c(0x100, ((uint8_t *)a0)[0x10]);
    ctx.cb74(0x340, 0);
    if (a1 != 0) {
        ctx.cb94(a0[5], a0[6]);
        ctx.cb8c(a0[3], a0[4], 0x1000);
    }
    val = a0[0];
    i = 0;
    if (((uint8_t *)a0)[0x0e] != 0) {
        do {
            rem = val % 10;
            if (i == 0 || ((uint8_t *)a0)[0x0f] != 0 || rem != 0 || val / 10 != 0) {
                exebase = D0x80044b38;
                r = ((ExeSlotFn)*(void **)(exebase + 0x424))(0x25d0003);
                k = i + 1;
                ctx.cb84(r, rem + 0x14, a0[1] + (((uint8_t *)a0)[0x0e] - k) * 7, a0[2]);
            }
            i += 1;
            val /= 10;
        } while (i < ((uint8_t *)a0)[0x0e]);
    }
}
