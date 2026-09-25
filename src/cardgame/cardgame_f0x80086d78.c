// CARDGAME:0x80086d78 (size 312, 0x138)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x40c8
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #13
// prologue 27bdffd0 (addiu sp,-0x30) to epilogue jr ra + addiu sp,+0x30.
// Prev CARDGAME:0x80086d28 size 0x50 ends exactly at 0x80086d78;
// next CARDGAME:0x80086eb0 at +0x138.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 78
// insns word-equal vs PAL; decompile reviewed; 2 callers in
// CARDGAME:0x80084320 (0x80084a08, 0x80084a58, return value ignored -> void);
// 2 callees: CARDGAME:0x80085fd0(a0, a2+0xc, halfword) and CARDGAME:0x80086a18.
// Signature: void (a0 base, a1 engine-iface, a2 record); void confirmed by both
// callers in CARDGAME:0x80084320 ignoring v0; epilogue li v0,-1 feeds sw only.
// a2 layout from PAL accesses: +0xa int16 count, +0xc byte table for 85fd0,
// +0x11 flag byte, +0x64 40-entry int16 index array (sliding pointer +2/iter).
// a1 layout: +0xecc/0xec0 indirect slots called with (a1) / (a1, flag).
// Tail (+0x423=1, call 86a18, +0x43c=0, +0x440=-1) mirrors the C_MATCHING
// sibling CARDGAME:0x80086eb0, except store-BEFORE-call order here: the sb sits
// in the jal delay slot with li v0,1 hoisted above the jal (proven by H4 diff).
// Codegen notes (each verified by pipeline diff): outer if uses != 0 with the
// loop as fallthrough (beq-to-skip, no extra j); explicit int one = 1 declared
// between i and q reproduces li s5,1 before move s2,s3; sliding byte pointer q
// (+2/iter, lh 0x64(q)) reproduces the index loads without shift/addu.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 base variant.
// Pipeline: --name CARDGAME_F0x80086d78 --base 0x80082cb0 --address 0x80086d78
// --size 312 --body-size 312 --opt-level O2 --symbol CARDGAME_F0x80085fd0=0x80085fd0
// --symbol CARDGAME_F0x80086a18=0x80086a18 (reference-bin absolute cardgame.bin).
// Result: exact_byte_match, difference_count 0,
// sha256 8c704fb3c8657b17c737911d7204acf7c7445b514638e32d23bfdc2366c9e35e.

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
