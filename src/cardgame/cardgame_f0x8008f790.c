/*
 * CARDGAME:0x8008f790 CARDGAME_F0x8008f790
 * 440 bytes at CARDGAME.PRO offset 0xcae0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008f790
 *  Symbols     CARDGAME_F0x80084320=0x80084320 CARDGAME_F0x8008d938=0x8008d938
 *              D_8004df9c=0x8004df9c
 *  Compare     440 bytes from 0x8008f790 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008f790
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * at verified base 0x80082cb0. File offset 0xcae0 = 0x8008f790 - 0x80082cb0
 * (RAW overlay, no EXE header). Prologue addiu sp,-0x28, saves s0-s3/ra;
 * epilogue jr ra at 0x8008f940. Next framed CARDGAME:0x8008f978 at +0x1b8
 * (contiguous). Callees: direct jal CARDGAME_F0x8008d938(p1,p2,(byte)0x575+10);
 * indirect jalr via *(p2+0xea0) twice: (p2,0,7,(short)(p1+0x5a2)) and
 * (p2,1,7,(short)(p1+0x66a)); EXE tick vector *0x8004df9c (lui 0x8005 + lw
 * -0x2064, jalr, no args). State byte p1+0x422: 1 = arm (d938 gate, stores
 * 10/2, *200 entry math, halfword copy 0x580->0x614, count bump, two table
 * callbacks, return 0); 2 = tick down (timer p1+0x428 minus tick; if still >0
 * return 0, else state=3, conditional halfword clear, 15-entry stride-0x4c byte
 * clear, return 0); 3 = return 1; else return 0. Forward-goto dispatch after
 * sibling CARDGAME:0x8008d938 (single return funnel, ret in s3, const-2 in s2,
 * PAL block order [dispatch][ge3][s1] [s2+loop][s3][epi]). Loop keeps (c+k)
 * inner add + p2 outer base-first addus, count-up i with slti 0xf, k += 0x4c in
 * the bne delay slot. Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2
 * -G0 with variant o2-g0-no-strength-reduce (base O2 induction/dbra rewrites
 * the loop to a countdown and folds (b-3)*8; no-sr keeps PAL shapes; all mults
 * still lower to the PAL shift chains).
 */

#include <stdint.h>

typedef int (*cardgame_f790_tick_t)(void);
extern cardgame_f790_tick_t D_8004df9c;
extern int CARDGAME_F0x8008d938(void *a0, void *a1, int a2);
typedef void (*cardgame_f790_ea0_t)(void *ctx, int a1, int a2, int a3);

int CARDGAME_F0x8008f790(unsigned char *p1, void *p2)
{
    int ret;
    int st;
    int tick;
    int left;
    int b;
    int c;
    int i;
    int k;
    unsigned char *slot;
    int cv;
    int idx;
    unsigned char *ent;
    int h;
    int off;
    unsigned int u;

    ret = 0;
    st = p1[0x422];
    if (st == 2)
        goto s2;
    if (st >= 3)
        goto ge3;
    if (st == 1)
        goto s1;
    goto epi;
ge3:
    if (st == 3)
        goto s3;
    goto epi;
s1:
    if (CARDGAME_F0x8008d938(p1, p2, ((int8_t *)p1)[0x575] + 10) == 0)
        goto epi;
    *(int32_t *)(p1 + 0x428) = 10;
    p1[0x422] = 2;
    slot = p1 + ((((int8_t *)p1)[0x575] - 2) * 8);
    cv = slot[0x584];
    idx = cv * 200;
    ent = p1 + idx;
    h = *(int16_t *)(ent + 0x5a2);
    off = h * 2 + idx;
    u = *(uint16_t *)(slot + 0x580);
    *(int16_t *)(p1 + off + 0x614) = (int16_t)u;
    *(uint16_t *)(ent + 0x5a2) = (uint16_t)(*(uint16_t *)(ent + 0x5a2) + 1);
    ((cardgame_f790_ea0_t)*(void **)((unsigned char *)p2 + 0xea0))(p2, 0, 7, (int)*(int16_t *)(p1 + 0x5a2));
    ((cardgame_f790_ea0_t)*(void **)((unsigned char *)p2 + 0xea0))(p2, 1, 7, (int)*(int16_t *)(p1 + 0x66a));
    goto epi;
s2:
    tick = D_8004df9c();
    left = *(int32_t *)(p1 + 0x428) - tick;
    *(int32_t *)(p1 + 0x428) = left;
    if (left > 0)
        goto epi;
    b = ((int8_t *)p1)[0x575];
    p1[0x422] = 3;
    c = b - 1;
    if (c >= 2)
        *(int16_t *)(p1 + (((b - 3) << 3)) + 0x582) = 0;
    for (i = 0, k = -1; i < 15; i += 1, k += 0x4c)
        *((unsigned char *)p2 + (c + k) + 0x146) = 0;
    goto epi;
s3:
    ret = 1;
epi:
    return ret;
}