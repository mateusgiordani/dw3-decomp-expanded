/* STCRDDEK:0x800868e8 (496 B, file off 0x3c38 = vaddr - 0x80082cb0).
 * PAL bytes (authority): reference/extracted/pro/stcrddek.bin off 0x3c38, 496 B,
 * seg sha256 318dba2511b9bb20afff95a802c04c033ee1b24586423c824deb209a164e4192;
 * prologue addiu sp,sp,-0x30 + sw s0-s5/ra, epilogue lw + jr ra + addiu sp,sp,0x30
 * at 0x80086ab4-0x80086ad4; next function STCRDDEK:0x80086ad8 contiguous, no overlap.
 * Ghidra STCRDDEK (project ddw3-pal-sles-03936) read-only: 124 insns disassembled
 * in 3 chunks, bytes match PAL head (d0ffbd27 1c00b3af ...) and tail (... 0800e003
 * 3000bd27); decompiler agrees (name STCRDDEK_func_800868e8, used as C symbol);
 * one direct caller: jal from STCRDDEK_func_800884e8 @ 0x800885ec (a0=s0, a1=s2).
 * Callee: EXE F0x8001ae38 (9 jal calls, constant (a1,a2,a3) triples) plus indirect
 * slot calls +0x138(obj,4), +0x140(obj,0x13,0 / obj,0xe,0x12), +0x15c(obj,mem58-1),
 * +0x160(obj,7). Loop runs 3x (slti/bne on counter, s1 0x2f0000+=0x4e0000, a2=s1>>16
 * = 0x2f/0x7d/0xcb); counter increment and +0x50 store sit in jal/bne delay slots,
 * folded into the loop body here (semantically identical).
 * Word 0x8008B6E4 stored to +0x50 of widget objects is overlay DATA (Ghidra:
 * no instruction there); no canonical data symbol exists, so it is kept as an
 * absolute address constant with uncertainty preserved -- coordinator follow-up
 * may promote a STCRDDEK data symbol. Deck-menu semantics UNCONFIRMED (campaign
 * deck-menu candidate; load contract verified only). Portable C, no register vars.
 * Upstream stcrddek.s used ONLY as --ref label locator (GUIDE only, never copied).
 * No Ghidra state change.
 * Rev B: locals declared in PAL saved-reg order (s0,s1,s2,s3,s4,s5); s0 is a byte
 * pointer bumped by 4 so stores/loads use PAL's 8(s0) shape instead of a folded
 * &s4[2] base; loop temporaries reloaded through s0 after each indirect call to
 * mirror PAL's post-call lw a0,8(s0).
 * Rev C (r6): s1 = 0x2f0000 moved to just before the loop (late init); sched1
 * emits its save/init after the arg triple and dbr can use jal delay slots,
 * matching PAL's lui s1,0x2f in the first jal delay (150 -> 137 diffs base).
 * r7 o55: exact_byte_match 496/496 (psyq-gcc-2.8.1 + aspsx-2.79, receipt variant
 * o2-g0-no-strength-reduce). The parameters are used directly (no s3/s4 copies:
 * the copies outranked s1/s2 in global allocation), s4[1] is stored through
 * directly, and s2 = 0 follows the first call (sched1 hoists it above the jal,
 * after the argument setup, as in PAL). */
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
