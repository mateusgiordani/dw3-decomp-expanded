/*
 * STCRDDEK:0x8008339c STCRDDEK_func_8008339c
 * 324 bytes at STCRDDEK.PRO offset 0x6ec (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008339c
 *  Symbols     STCRDDEK_func_8008339c=0x8008339c
 *              stcrddek_tbl_8004DE10=0x8004de10
 *  Compare     324 bytes from 0x8008339c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008339c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL jr ra plus delay slot".
 *
 * /* Body: h = tbl.make(*(arg+0x50)); obj = *(h+0x138)(h, *(arg+0x54)); ent =
 * tbl.alloc() (no args; PAL delay-slot move s1,v0 saves the method result
 * before the call). Init ent bytes/halves: [3]=5, [7]=0x2a, +0x14/+0x0c=0x140,
 * +0x10/+0x08/+0x0e/+0x0a=0, +0x16/+0x12=0x100, craw=*(arg+0x5c) at PAL load
 * point, [6]=[5]=[4]=(signed craw >> 8, PAL sra). Swap low 24 bits between
 * ent[0] and obj[0] under masks 0xff000000/0x00ffffff; ent[0x1b]=1;
 * ent+0x1c=0xe1000245; swap low 24 bits between ent[0x18] and obj[0] where the
 * obj side receives (ent+0x18); tbl.done(ent+0x20). Returns void (caller
 * ignores v0).
 *
 * Externals: single RAM table base 0x8004DE10 (PAL lui s2,0x8005 + addiu
 * -0x21f0, kept in callee-saved s2) with slots +0x158/+0x15c/+0x17c.
 *
 * Names conservative. Toolchain base hypothesis: psyq-gcc-2.8.1-sn32-4.0.0010 +
 * aspsx-2.79 -O2 -G0. Portable C only.
 *
 * Measured on this function's -da .sched trace: sched1 splits 0xe1000245 and
 * 0x00ffffff into two sets of one pseudo each, so their lui never gets the
 * ready-list boost a once-set pseudo gets; li 5 / li 0x2a did get it and won
 * every tie, leaving the lui hoisted. Routing 5 and 0x2a through one byte local
 * (b) removes that boost, so the lui is picked right after sb 7 on insn order;
 * setting the mask local (m) right after the alloc keeps its split early in
 * insn order so it stays above li 5. Controls: b alone moves lui and mask
 * together below li 0x2a; m alone reproduces the rev5 object; m after ent[7]
 * behaves like b alone. Logic verified insn-by-insn vs PAL.
 */

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
