/* STCRDDEK:0x80088634 (size 148, 0x94; 37 words) */
/* PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5984 (RAW, no header) */
/* Boundary: prologue 27bdffd0 addiu sp,-0x30 at 0x80088634; saves s2/s1/ra/s0 at */
/* 0x28/0x24/0x2c/0x20(sp) with s2=a0, s1=a1; epilogue lw ra/s2/s1/s0 + jr ra + */
/* addiu sp,+0x30 at 0x800886b0-0x800886c4. Previous function ends with jr ra at */
/* 0x8008862c; next prologue 27bdffd0 at 0x800886c8: size 0x94 contiguous, no overlap. */
/* Ghidra project ddw3-pal-sles-03936, program STCRDDEK, read-only: disasm 37 words */
/* matches PAL above; jal targets 0x8001FCC0 (word 7) and 0x8002525C (word 16) are */
/* EXE-resident helpers; one indirect jalr ra,v0 through the word at sp+0x18 (word 14). */
/* Caller: STCRDDEK 0x800887d4 region references this function (inventory adjacency; */
/* exact caller list unconfirmed). No Ghidra state change. Upstream ddw3 is GUIDE only. */
/* Behavior (conservative, disasm-verified): EXE helper fills a 16-byte stack buffer; */
/* word +8 of the buffer is an indirect target called as fn(arg0+0x78, arg1, 0); EXE */
/* strlen-like helper on arg0+0x78 gives a byte length whose half (sra 1) is the start */
/* index; halfwords at arg0+0x78 are filled with 0x4081 up to the int count at arg0+0x94. */
/* The count at +0x94 is reloaded every loop iteration (lw inside the loop, not hoisted). */
/* Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 -mips1 -msoft-float (base). */
/* Status: C_MATCHING (portable C, no asm, no explicit register variables). */
/* sha256 9103041d1ebf2020b0b6fbadf282884393a50d47e086f5f7ed9779a77e1dd0c4 (148B). */
/* Matching idioms (load-bearing, do not "clean up"): arg0 is cast to int at each use */
/* (no base local) -- this orders the prologue s2-pair before the s1-pair and emits */
/* addu v1,v0,s2 for the pointer; the do-while body stores first (sh), then p += 2, */
/* then idx += 1 with the count reloaded in the condition, which places addiu a0,a0,1 */
/* between lw and slt (no load-delay nop) and p += 2 in the bne delay slot. */

/* EXE-resident helpers (resolved via pipeline --symbol, not committed): */
extern void stcrddek_exe_8001FCC0(void *buf);
extern int stcrddek_exe_8002525C(const char *s);

void STCRDDEK_func_80088634(void *arg0, int arg1)
{
    unsigned int buf[4];
    int (*fn)(void *, int, int);
    char *s0;
    int tmp;
    int count;
    int idx;

    stcrddek_exe_8001FCC0(buf);
    s0 = (char *)((int)arg0 + 0x78);
    fn = (int (*)(void *, int, int))buf[2];
    fn(s0, arg1, 0);
    tmp = stcrddek_exe_8002525C(s0);
    count = *(int *)((int)arg0 + 0x94);
    idx = tmp >> 1;
    if (idx < count) {
        short fill = 0x4081;
        char *p = (char *)((idx << 1) + (int)arg0);
        do {
            *(short *)(p + 0x78) = fill;
            p += 2;
            idx += 1;
        } while (idx < *(int *)((int)arg0 + 0x94));
    }
}
