// CARDGAME:0x80086eb0 (size 116, 0x74) -- C_MATCHING
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x4200
// Boundary: sweep reports/handoffs/cardgame-boundary-sweep.md #14; prologue
// 27bdffe0 (addiu sp,-0x20) to epilogue jr ra + addiu sp,+0x20; frame -0x20;
// prev CARDGAME:0x80086d78 ends exactly at 0x80086eb0, next 0x80086f24 at +0x74.
// Ghidra program CARDGAME read-only (unanalyzed at this address per sweep);
// cardgame.s GUIDE only; PAL words decoded directly (file off = vaddr - base).
// Signature: 4 args (a0 pointer s0, a1 unused, a2 signed count, a3 byte s1).
// a1 ($5) is never read; a2 drives blez; a3 is preserved across the jal in s1.
// Semantics: if (a2 <= 0) clear one byte at +0x49E, else clear 40 bytes at
// +0x49E..+0x4C5 descending (i 39..0, pointer sliding); then call the 0x80086a18
// worker on a0 and set +0x423=1, +0x499=a3, +0x43C=0, +0x440=-1.
// The explicit goto pins blez/j/nop/single order: plain if/else lets gcc 2.8.1
// invert to bgtz with the single sb folded into j's delay slot (112 B object);
// the goto form keeps fallthrough loop + j/nop + single (116 B, exact).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 base, no alternates.
// Pipeline: --name CARDGAME_F0x80086eb0 --base 0x80082cb0 --address 0x80086eb0
// --size 116 --body-size 116 --opt-level O2 --symbol CARDGAME_F0x80086a18=0x80086a18
// Result: exact_byte_match, diff 0, sha bc23baa13bc39b0f2de14b0d8a2024d2c271ee75527548eb90c902bf7dde6ff2.

extern void CARDGAME_F0x80086a18(unsigned char *a0);

void CARDGAME_F0x80086eb0(unsigned char *a0, int a1, int a2, unsigned char a3)
{
    (void)a1;
    if (a2 <= 0)
        goto single;
    {
        int i = 39;
        unsigned char *p = a0 + 39;
        do {
            p[0x49E] = 0;
            i--;
            p--;
        } while (i >= 0);
    }
    goto cont;
single:
    a0[0x49E] = 0;
cont:
    CARDGAME_F0x80086a18(a0);
    a0[0x423] = 1;
    a0[0x499] = a3;
    ((int *)(a0 + 0x43C))[0] = 0;
    ((int *)(a0 + 0x440))[0] = -1;
}
