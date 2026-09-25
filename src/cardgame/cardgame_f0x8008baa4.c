// CARDGAME:0x8008baa4 (size 456, 0x1c8)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x08df4
// Boundary: sweep reports/handoffs/cardgame-boundary-sweep.md #37; prologue
// 27bdffe0 (addiu sp,-0x20) to epilogue jr ra + addiu sp,+0x20; frame -0x20;
// prev CARDGAME:0x8008ad98 ends exactly at 0x8008baa4, next 0x8008bc6c at +0x1c8.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 114
// words match PAL; decompile gives mode dispatch + fill loop + tail worker call.
// Callers: 4 direct jal from CARDGAME_F0x80084320 (modes 0/3/2/1), 1 from
// CARDGAME_F0x8008bc6c (mode 0 wrapper). Callees: jal CARDGAME:0x80086a18 +
// 2 indirect jalr via slot +0x814 (modes 1/2 setup calls).
// Signature: void (a0 struct ptr s0, a1 opaque s2 -> 0x80086a18 2nd arg,
// a2 index s1, a3 mode). Mode 0: 0x438 = s1?11:5, count = half(s1*200+0x5a6).
// Modes 1/2: 0x438 = 7 (s1==0, setup call a1=+0x5b0/a3=2) or 13 (2nd setup call
// a1=+0x678/a3=3 unless a3==2); count = half(s1*200+0x5a4). Mode 3:
// 0x438 = s1?15:9, count = half(s1*200+0x5a2). Out-of-range a3 skips setup.
// Tail: 40-entry loop (count reloaded each iter): if (i<count)
// { byte+0x46f=0; byte+0x49e = (byte+0x446!=0) ? 0 : fill }; then
// 0x80086a18(s0,s2) with sb 0x423=1 in jal delay (call-first idiom per
// 0x80086eb0), 0x43c=0, 0x440=-1.
// Codegen notes (all confirmed by byte-exact build, see handoff): flat
// dispatch in PAL block order (outer, bgtz, site1, site2, then bodies) keeps
// beq-forward + j per out-of-range path; nested if/else gets inverted and
// threaded to bne by jump.c (verified in RTL jump2 dumps). Params a0/a1
// double as loop index/fill (a0=(char*)0, a1=1); struct/index/opaque held in
// s0/s1/s2 copies. `a2 = 0` is dead (deleted, zero bytes) but blocks
// copy-propagation of the s1 copy into a2-reg, keeping index reads in s1
// (verified: without it the index uses a2). Value/index share one temp so it
// lands in v0, which pins the 0x438 store above the index chain via
// anti-dependence. Explicit sll/addu index chain (base-first addu per lesson
// 008). int8_t byte read (PAL lb). Tail store s0[0x423]=1 precedes the call;
// the scheduler sinks it into the jal delay slot (call-first idiom per
// 0x80086eb0). Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0.
#include <stdint.h>

extern void CARDGAME_F0x80086a18(char *a0, int32_t a1);

typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

void CARDGAME_F0x8008baa4(char *a0, int32_t a1, int32_t a2, int32_t a3)
{
    char *s0 = a0;
    int32_t s2 = a1;
    int32_t s1 = a2;
    int32_t t;

    a2 = 0;

    if (a3 >= 3)
        goto ge3;
    if (a3 > 0)
        goto gt0;
    a0 = (char *)0;
    if (a3 == 0)
        goto mode0;
    a1 = 1;
    goto loop;
ge3:
    a0 = (char *)0;
    if (a3 == 3)
        goto mode3;
    a1 = 1;
    goto loop;
mode0:
    if (s1 == 0)
        t = 5;
    else
        t = 11;
    *(int32_t *)(s0 + 0x438) = t;
    t = s1 << 1;
    t += s1;
    t <<= 3;
    t += s1;
    t <<= 3;
    *(int32_t *)(s0 + 0x434) = *(int16_t *)(s0 + t + 0x5a6);
    goto merge;
gt0:
    if (s1 == 0) {
        *(int32_t *)(s0 + 0x438) = 7;
        ((cardgame_cb4_t)*(uint32_t *)(s0 + 0x814))(s0, s0 + 0x5b0, (int32_t)*(int16_t *)(s0 + 0x5a0) | 0x280000, 2);
    } else {
        *(int32_t *)(s0 + 0x438) = 13;
        if (a3 != 2)
            ((cardgame_cb4_t)*(uint32_t *)(s0 + 0x814))(s0, s0 + 0x678, (int32_t)*(int16_t *)(s0 + 0x668) | 0x280000, 3);
    }
    t = s1 << 1;
    t += s1;
    t <<= 3;
    t += s1;
    t <<= 3;
    *(int32_t *)(s0 + 0x434) = *(int16_t *)(s0 + t + 0x5a4);
    goto merge;
mode3:
    if (s1 == 0)
        t = 9;
    else
        t = 15;
    *(int32_t *)(s0 + 0x438) = t;
    t = s1 << 1;
    t += s1;
    t <<= 3;
    t += s1;
    t <<= 3;
    *(int32_t *)(s0 + 0x434) = *(int16_t *)(s0 + t + 0x5a2);
merge:
    a0 = (char *)0;
    a1 = 1;
loop:
    do {
        char *p = s0 + (int)a0;
        if ((int)a0 < *(int32_t *)(s0 + 0x434)) {
            p[0x46f] = 0;
            if (*(int8_t *)(p + 0x446) != 0)
                p[0x49e] = 0;
            else
                p[0x49e] = (char)a1;
        }
        a0++;
    } while ((int)a0 < 40);
    s0[0x423] = 1;
    CARDGAME_F0x80086a18(s0, s2);
    *(int32_t *)(s0 + 0x43c) = 0;
    *(int32_t *)(s0 + 0x440) = -1;
}
