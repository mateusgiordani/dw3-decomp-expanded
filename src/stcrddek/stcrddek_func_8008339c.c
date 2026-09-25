/* STCRDDEK:0x8008339c (size 324, 0x144; file-off 0x6ec, base 0x80082cb0) */
/* PAL: reference/extracted/pro/stcrddek.bin. Entry addiu sp,sp,-0x20 at
 * 0x8008339c; epilogue lw ra/s2/s1/s0 + jr ra / addiu sp,sp,+0x20 ending at
 * 0x800834dc; next prologue at 0x800834e0. All 81 words match PAL (LE words
 * equal Ghidra disasm bytes; e.g. off 0x6ec 27bdffe0, off 0x6f0 afb00010).
 * Reviewed inventory: start_offset 1772 end_offset 2096, boundary evidence
 * "complete Ghidra CFG including switch cases; PAL jr ra plus delay slot".
 * Ghidra ddw3-pal-sles-03936 program STCRDDEK read-only: disasm 81 insns +
 * decompile STCRDDEK_func_8008339c (hypothesis only, confirmed against disasm)
 * + x-ref to (single direct jal from STCRDDEK_func_800834e0 at 0x8008357c,
 * delay slot move a0,a1; sibling state 2 of an a1+0xc dispatch also seen at
 * 0x800834f4/0x80083504). Upstream stcrddek.s .L0x000006ec GUIDE only.
/* Body: h = tbl.make(*(arg+0x50)); obj = *(h+0x138)(h, *(arg+0x54));
 * ent = tbl.alloc() (no args; PAL delay-slot move s1,v0 saves the method
 * result before the call). Init ent bytes/halves: [3]=5, [7]=0x2a,
 * +0x14/+0x0c=0x140, +0x10/+0x08/+0x0e/+0x0a=0, +0x16/+0x12=0x100,
 * craw=*(arg+0x5c) at PAL load point, [6]=[5]=[4]=(signed craw >> 8,
 * PAL sra). Swap low 24 bits between
 * ent[0] and obj[0] under masks 0xff000000/0x00ffffff; ent[0x1b]=1;
 * ent+0x1c=0xe1000245; swap low 24 bits between ent[0x18] and obj[0] where
 * the obj side receives (ent+0x18); tbl.done(ent+0x20). Returns void (caller
 * ignores v0).
 * Externals: single RAM table base 0x8004DE10 (PAL lui s2,0x8005 +
 * addiu -0x21f0, kept in callee-saved s2) with slots +0x158/+0x15c/+0x17c.
 * Owning module of 0x8004DE10 unproven (writable-data region, likely EXE;
 * not claimed); object at h+0x138 is an untyped method slot. Names
 * conservative. Toolchain base hypothesis:
 * psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0. Portable C only.
 * Status: exact candidate rev6 (C_NONMATCHING until coordinator promotion):
 * g281/aspsx-2.79 -O2 -G0 full-range exact_byte_match 324/324, candidate sha
 * 7c71a5af... = PAL body (rev5 H2 form was 14/324, cand 5b3e2c43...).
 * Measured on this function's -da .sched trace: sched1 splits 0xe1000245 and
 * 0x00ffffff into two sets of one pseudo each, so their lui never gets the
 * ready-list boost a once-set pseudo gets; li 5 / li 0x2a did get it and won
 * every tie, leaving the lui hoisted. Routing 5 and 0x2a through one byte local
 * (b) removes that boost, so the lui is picked right after sb 7 on insn order;
 * setting the mask local (m) right after the alloc keeps its split early in
 * insn order so it stays above li 5. Controls: b alone moves lui and mask
 * together below li 0x2a; m alone reproduces the rev5 object; m after ent[7]
 * behaves like b alone. Logic verified insn-by-insn vs PAL. */
#include <stdint.h>

typedef struct {
    uint8_t pad[0x158];
    void *(*alloc)(void);
    void (*done)(void *p);
    uint8_t pad2[0x1c];
    void *(*make)(int32_t a0);
} stcrddek_tbl_8004DE10_t;

/* RAM-resident vector (resolved via pipeline --symbol, not committed). */
extern stcrddek_tbl_8004DE10_t stcrddek_tbl_8004DE10;

void STCRDDEK_func_8008339c(int32_t arg0)
{
    void *h;
    uint32_t *obj;
    uint8_t *ent;
    uint8_t *f;
    int32_t col;
    int32_t craw;
    uint32_t m;
    uint8_t b;

    h = stcrddek_tbl_8004DE10.make(*(int32_t *)(arg0 + 0x50));
    obj = (*(void *(**)(void *, int32_t))((uint8_t *)h + 0x138))(h, *(int32_t *)(arg0 + 0x54));
    ent = (uint8_t *)stcrddek_tbl_8004DE10.alloc();
    m = 0x00ffffff;
    b = 5;
    ent[3] = b;
    b = 0x2a;
    ent[7] = b;
    craw = *(int32_t *)(arg0 + 0x5c);
    f = ent + 0x18;
    *(uint16_t *)(ent + 0x14) = 0x140;
    *(uint16_t *)(ent + 0x0c) = 0x140;
    *(uint16_t *)(ent + 0x10) = 0;
    *(uint16_t *)(ent + 0x08) = 0;
    *(uint16_t *)(ent + 0x0e) = 0;
    *(uint16_t *)(ent + 0x0a) = 0;
    *(uint16_t *)(ent + 0x16) = 0x100;
    *(uint16_t *)(ent + 0x12) = 0x100;
    col = craw >> 8;
    ent[6] = (uint8_t)col;
    ent[5] = (uint8_t)col;
    ent[4] = (uint8_t)col;
    *(uint32_t *)ent = (*(uint32_t *)ent & 0xff000000) | (*(uint32_t *)obj & m);
    *(uint32_t *)obj = (*(uint32_t *)obj & 0xff000000) | ((uint32_t)ent & m);
    f[3] = 1;
    *(uint32_t *)(f + 4) = 0xe1000245;
    *(uint32_t *)f = (*(uint32_t *)f & 0xff000000) | (*(uint32_t *)obj & m);
    *(uint32_t *)obj = (*(uint32_t *)obj & 0xff000000) | ((uint32_t)f & m);
    stcrddek_tbl_8004DE10.done(ent + 0x20);
}
