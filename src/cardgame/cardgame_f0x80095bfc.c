/*
 * CARDGAME:0x80095bfc CARDGAME_F0x80095bfc
 * 560 bytes at CARDGAME.PRO offset 0x12f4c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80095bfc
 *  Symbols     CARDGAME_F0x80095bfc=0x80095bfc CARDGAME_F0x800A3240=0x800a3240
 *              D_80044B38=0x80044b38 D_80048D34=0x80048d34
 *              D_80048D68=0x80048d68 D_8004B43C=0x8004b43c
 *              D_8004DE10=0x8004de10 D_800A5938=0x800a5938
 *              F0x8001ffa8=0x8001ffa8
 *  Compare     560 bytes from 0x80095bfc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80095bfc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Overlay state callback.
 *
 * Shared-tail quirk (PAL): the init path loads (p+0x38) and jumps to the nop
 * immediately before the shared jalr also used by the state-2 (p+0x3c) call
 * (PAL j 0x80095e04); plain C leaves the tail merge to the compiler.
 *
 * Indices: 0x150/4=0x54, 0x154/4=0x55, 0x16c/4=0x5b, 0x174/4=0x5d,
 * 0x424/4=0x109, 0x2704/4=0x9c1, 0x270c/4=0x9c3, 0x34/4=0xd.
 *
 * Xref to 0x80095bfc:
 *
 * PARAM from CARDGAME_F0x80095e2c (0x80095e34); no direct jal (reached via the
 * EXE 0x80014504 template registration). Direct EXE callees: 0x8001ffa8
 * (filler) and 0x800a3240, which lies inside the CARDGAME range (prologue addiu
 * sp,-0x20) so it is a CARDGAME-local callee, still unlabeled.
 *
 * No Ghidra state change.
 *
 * Ranked alternate 1 (only compiler alternate tried):
 * psyq-gcc-2.7.2-sn32-3.7-build-0002 + aspsx-2.79, object 576 bytes with
 * 122/140 words differing (decisively worse codegen family). Blocker:
 * scheduler/allocator build difference for the st2 region; needs a cc1 build
 * whose earliest-ready heuristic leaves both st2 branch delays empty and shares
 * the st2 base into s0 with sel in a0 (all other cardgame wins use base
 * 2.8.1-sn32-4.0.0010, so no further alternates were burned).
 */

#include <stdint.h>

extern void F0x8001ffa8(void *buf);
extern int32_t CARDGAME_F0x800A3240(int32_t a0);
extern int32_t D_800A5938;
extern void *D_8004DE10[];
extern int32_t (*D_80044B38[])(int32_t a0);
extern int32_t (*D_8004B43C)(void);
extern void *D_80048D34[];

void CARDGAME_F0x80095bfc(int32_t p, int32_t *q)
{
    uint8_t buf[56];
    int32_t state;
    int32_t r;
    void (*tail)(int32_t);

    state = *(int32_t *)(p + 0xc);
    if (state == 1) {
        goto st1;
    }
    if (state < 2) {
        goto other;
    }
    if (state == 2) {
        goto st2;
    }
    if (state == 3) {
        goto out;
    }
other:
    ((void (*)(void))D_8004DE10[0x54])();
    ((void (*)(int32_t))D_8004DE10[0x55])(0xa000);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))D_8004DE10[0x5d])(0x140, 0xf0, 0, 0);
    F0x8001ffa8(buf);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x24))(0x280, 0);
    r = D_80044B38[0x109](0x25d0000);
    ((void (*)(int32_t))*(void **)(buf + 0x2c))(r);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x24))(0x340, 0);
    r = D_80044B38[0x109](0x25d0001);
    ((void (*)(int32_t))*(void **)(buf + 0x2c))(r);
    r = ((int32_t (*)(int32_t, int32_t, int32_t))D_8004DE10[0x5b])((int32_t)&D_800A5938, 3, 0x100);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(r + 0x12c))(r, 0x1f, 0x1f, 0x1f);
    *q = CARDGAME_F0x800A3240(D_8004B43C());
    tail = *(void (**)(int32_t))(p + 0x38);
    goto docall;
st1:
    if (*(uint8_t *)(*q + 0x303) != 2) {
        goto out;
    }
    ((void (*)(int32_t, int32_t))*(void **)(p + 0x28))(p, 2);
    ((void (*)(int32_t, int32_t))*(void **)(*q + 0x28))(*q, 3);
    goto out;
st2:
    if (*(int32_t *)(p + 0x10) == 0) {
        goto work;
    }
    if (*(int32_t *)(p + 0x10) == 1) {
        goto out;
    }
work:
    ((void (*)(int32_t, int32_t))D_80048D34[0x9c3])(((((int32_t (*)(void))D_80048D34[0x9c1])() & 0xf) != 0) ? 0x1500 : (int32_t)D_80048D34[0xd], 0);
    tail = *(void (**)(int32_t))(p + 0x3c);
docall:
    tail(p);
out:
    return;
}
