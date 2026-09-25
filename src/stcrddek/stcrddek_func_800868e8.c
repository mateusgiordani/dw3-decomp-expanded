/*
 * STCRDDEK:0x800868e8 STCRDDEK_func_800868e8
 * 496 bytes at STCRDDEK.PRO offset 0x3c38 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800868e8
 *  Symbols     D_8008B6E4=0x8008b6e4 F0x8001ae38=0x8001ae38
 *  Compare     496 bytes from 0x800868e8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800868e8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Callee: EXE F0x8001ae38 (9 jal calls, constant (a1,a2,a3) triples) plus
 * indirect slot calls +0x138(obj,4), +0x140(obj,0x13,0 / obj,0xe,0x12),
 * +0x15c(obj,mem58-1), +0x160(obj,7). Loop runs 3x (slti/bne on counter, s1
 * 0x2f0000+=0x4e0000, a2=s1>>16 = 0x2f/0x7d/0xcb); counter increment and +0x50
 * store sit in jal/bne delay slots, folded into the loop body here
 * (semantically identical).
 *
 * Deck-menu semantics UNCONFIRMED (campaign deck-menu candidate; load contract
 * verified only). Portable C, no register vars.
 *
 * No Ghidra state change.
 *
 * Rev B: locals declared in PAL saved-reg order (s0,s1,s2,s3,s4,s5); s0 is a
 * byte pointer bumped by 4 so stores/loads use PAL's 8(s0) shape instead of a
 * folded &s4[2] base; loop temporaries reloaded through s0 after each indirect
 * call to mirror PAL's post-call lw a0,8(s0).
 *
 * Rev C (r6): s1 = 0x2f0000 moved to just before the loop (late init); sched1
 * emits its save/init after the arg triple and dbr can use jal delay slots,
 * matching PAL's lui s1,0x2f in the first jal delay (150 -> 137 diffs base).
 *
 * The parameters are used directly (no s3/s4 copies: the copies outranked s1/s2
 * in global allocation), s4[1] is stored through directly, and s2 = 0 follows
 * the first call (sched1 hoists it above the jal, after the argument setup, as
 * in PAL).
 */

#include "common/types.h"

extern void *F0x8001ae38(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
extern unsigned char D_8008B6E4[];

void STCRDDEK_func_800868e8(void *s3, void **s4)
{
    char *s0;
    int32_t s1;
    int32_t s2;
    void *s5;
    void *v0;
    void *w;
    void *v1;

    v0 = F0x8001ae38((int32_t)*(int16_t *)((char *)s3 + 0x54), 1, 0x20, 0x1a);
    s4[0] = v0;
    ((void (*)(void *, int32_t))*(void **)((char *)v0 + 0x138))(v0, 4);
    s2 = 0;
    v0 = F0x8001ae38((int32_t)*(int16_t *)((char *)s3 + 0x54), 1, 0x4b, 0x40);
    s4[1] = v0;
    ((void (*)(void *, int32_t, int32_t))*(void **)((char *)v0 + 0x140))(v0, 0x13, s2);
    v0 = (void *)D_8008B6E4;
    s5 = v0;
    s0 = (char *)s4;
    *(void **)((char *)s4[1] + 0x50) = s5;
    s1 = 0x2f0000;
    for (; s2 < 3; s2++) {
        v0 = F0x8001ae38((int32_t)*(int16_t *)((char *)s3 + 0x54), 1, s1 >> 16, 0x5b);
        *(void **)(s0 + 8) = v0;
        w = v0;
        ((void (*)(void *, int32_t))*(void **)((char *)w + 0x15c))(w, *(int32_t *)((char *)s3 + 0x58) - 1);
        w = *(void **)(s0 + 8);
        ((void (*)(void *, int32_t))*(void **)((char *)w + 0x160))(w, 7);
        w = *(void **)(s0 + 8);
        ((void (*)(void *, int32_t, int32_t))*(void **)((char *)w + 0x140))(w, 0xe, 0x12);
        v1 = *(void **)(s0 + 8);
        s0 += 4;
        s1 += 0x4e0000;
        *(void **)((char *)v1 + 0x50) = s5;
    }
    v0 = F0x8001ae38((int32_t)*(int16_t *)((char *)s3 + 0x54), 1, 0xce, 0xc6);
    s4[8] = v0;
    v0 = F0x8001ae38((int32_t)*(int16_t *)((char *)s3 + 0x54), 1, 0xe1, 0xc6);
    s4[9] = v0;
    v0 = F0x8001ae38((int32_t)*(int16_t *)((char *)s3 + 0x54), 1, 0x13, 0x62);
    s4[10] = v0;
    ((void (*)(void *, int32_t))*(void **)((char *)v0 + 0x15c))(v0, *(int32_t *)((char *)s3 + 0x58) - 1);
    v0 = F0x8001ae38((int32_t)*(int16_t *)((char *)s3 + 0x54), 1, 0x123, 0x62);
    s4[11] = v0;
    ((void (*)(void *, int32_t))*(void **)((char *)v0 + 0x15c))(v0, *(int32_t *)((char *)s3 + 0x58) - 1);
    v0 = F0x8001ae38((int32_t)*(int16_t *)((char *)s3 + 0x54), 1, 0x3e, 0x72);
    s4[12] = v0;
}
