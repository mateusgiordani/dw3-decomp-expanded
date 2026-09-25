// CARDGAME:0x8009d310 (size 560, 0x230)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1a660
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #178
// Prologue 27bdffe8 addiu sp,-0x18 ; sw s0,0x10(sp) ; move s0,a0 ; lui a0,0x800a ; addiu a0,-0x4458 (=0x8009bba8) ; li a1,0xf48 ; sw ra,0x14(sp) ; jal EXE:0x80014504 ; ori a2,0x38 (delay)
// Body: 42x (lui v1,0x800a ; addiu v1,neg ; sw v1,off(v0)) table stores plus sw s0,0x50(v0) after the 0xf04 store; v0 is the EXE allocator result reused directly
// Epilogue lw ra,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,+0x18
// Next function CARDGAME:0x8009d540 at +0x230 (prologue addiu sp,-0x18) confirms size 0x230 is self-contained.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 140 words, decompile, x-ref to/from; no state changed.
// Caller: one direct jal from FUN_800a2f00 at 0x800a2f94 (in-overlay init path); registered callbacks are reached via this table (DATA xrefs), not direct calls.
// Callee: direct jal EXE:0x80014504 (canonical EXE_F0x80014504, C_MATCHING, 32B); a0=CARDGAME:0x8009bba8 template, a1=0xf48, a2=0x38.
// Slot map (offset -> CARDGAME pointer): ea0->cae0 ea4->be40 ea8->be78 eac->beac eb0->bf5c eb8->cc4c eb4->cda4 ee4->c054 ee8->c0dc ef0->c16c eec->c124 ef4->c174 ef8->c1cc efc->c214 f00->c25c ed8->8864 edc->bfcc ee0->c010 f08->c9a0 f0c->ca3c f10->ca80 f2c->c654 f30->c6e8 f34->c770 f38->c7f8 f28->c5d8 f14->c510 f18->cdfc f1c->ce28 f20->c904 f24->c938 f04->ce94 050->arg ec4->c350 ec8->c2d0 ebc->c438 ed0->c4a4 ed4->c4d8 ec0->c3c0 ecc->c264 f3c->cfbc f40->cf60 f44->d15c (all 0x8009xxxx; 0x80098864 is the only out-of-family slot).
// Codegen note: every slot uses lui 0x800a + addiu (negative), i.e. the
// assembler %hi/%lo expansion of a symbolic address. Numeric C constants go
// through cc1's HIGH/LO_SUM pre-split (lui 0x8009 + ori) on every pinned
// cc1, so the slots must reference extern symbols (resolved to absolute
// addresses at link) to reproduce the observed expansion. Function slots use
// their Ghidra canonical names; LAB_ slots use positional externs (address
// encoded in the name, not promoted symbols, no CSV changes).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base variant), portable C exact_byte_match, no inline asm.

extern void *EXE_F0x80014504(void *a0, int a1, int a2);

extern void CARDGAME_F0x8009bba8(void);
extern void CARDGAME_F0x8009beac(void);
extern void CARDGAME_F0x8009bf5c(void);
extern void CARDGAME_F0x8009c054(void);
extern void CARDGAME_F0x8009c0dc(void);
extern void CARDGAME_F0x8009c124(void);
extern void CARDGAME_F0x8009c174(void);
extern void CARDGAME_F0x8009c1cc(void);
extern void CARDGAME_F0x8009c214(void);
extern void CARDGAME_F0x8009c510(void);
extern void CARDGAME_F0x8009c5d8(void);
extern void CARDGAME_F0x8009c654(void);
extern void CARDGAME_F0x8009c6e8(void);
extern void CARDGAME_F0x8009c770(void);
extern void CARDGAME_F0x8009c904(void);
extern void CARDGAME_F0x8009c938(void);
extern void CARDGAME_F0x8009c9a0(void);
extern void CARDGAME_F0x8009ca3c(void);
extern void CARDGAME_F0x8009cda4(void);
extern void CARDGAME_F0x8009ce28(void);
extern void CARDGAME_F0x8009ce94(void);
extern void CARDGAME_F0x8009cf60(void);
extern void CARDGAME_F0x8009cfbc(void);
extern void CARDGAME_F0x8009d15c(void);
extern void FUN_80098864(void);

/* Positional code-label placeholders (Ghidra LAB_, no sweep function start,
   no project symbol; names encode the address, semantics unknown). */
extern char CARDGAME_LAB_8009be40[];
extern char CARDGAME_LAB_8009be78[];
extern char CARDGAME_LAB_8009bfcc[];
extern char CARDGAME_LAB_8009c010[];
extern char CARDGAME_LAB_8009c16c[];
extern char CARDGAME_LAB_8009c25c[];
extern char CARDGAME_LAB_8009c264[];
extern char CARDGAME_LAB_8009c2d0[];
extern char CARDGAME_LAB_8009c350[];
extern char CARDGAME_LAB_8009c3c0[];
extern char CARDGAME_LAB_8009c438[];
extern char CARDGAME_LAB_8009c4a4[];
extern char CARDGAME_LAB_8009c4d8[];
extern char CARDGAME_LAB_8009ca80[];
extern char CARDGAME_LAB_8009cae0[];
extern char CARDGAME_LAB_8009cc4c[];
extern char CARDGAME_LAB_8009cdfc[];
extern char CARDGAME_LAB_8009c7f8[];

void CARDGAME_F0x8009d310(void *arg)
{
    void *tbl;

    tbl = EXE_F0x80014504((void *)CARDGAME_F0x8009bba8, 0xf48, 0x38);
    *(void **)((char *)tbl + 0xea0) = (void *)CARDGAME_LAB_8009cae0;
    *(void **)((char *)tbl + 0xea4) = (void *)CARDGAME_LAB_8009be40;
    *(void **)((char *)tbl + 0xea8) = (void *)CARDGAME_LAB_8009be78;
    *(void **)((char *)tbl + 0xeac) = (void *)CARDGAME_F0x8009beac;
    *(void **)((char *)tbl + 0xeb0) = (void *)CARDGAME_F0x8009bf5c;
    *(void **)((char *)tbl + 0xeb8) = (void *)CARDGAME_LAB_8009cc4c;
    *(void **)((char *)tbl + 0xeb4) = (void *)CARDGAME_F0x8009cda4;
    *(void **)((char *)tbl + 0xee4) = (void *)CARDGAME_F0x8009c054;
    *(void **)((char *)tbl + 0xee8) = (void *)CARDGAME_F0x8009c0dc;
    *(void **)((char *)tbl + 0xef0) = (void *)CARDGAME_LAB_8009c16c;
    *(void **)((char *)tbl + 0xeec) = (void *)CARDGAME_F0x8009c124;
    *(void **)((char *)tbl + 0xef4) = (void *)CARDGAME_F0x8009c174;
    *(void **)((char *)tbl + 0xef8) = (void *)CARDGAME_F0x8009c1cc;
    *(void **)((char *)tbl + 0xefc) = (void *)CARDGAME_F0x8009c214;
    *(void **)((char *)tbl + 0xf00) = (void *)CARDGAME_LAB_8009c25c;
    *(void **)((char *)tbl + 0xed8) = (void *)FUN_80098864;
    *(void **)((char *)tbl + 0xedc) = (void *)CARDGAME_LAB_8009bfcc;
    *(void **)((char *)tbl + 0xee0) = (void *)CARDGAME_LAB_8009c010;
    *(void **)((char *)tbl + 0xf08) = (void *)CARDGAME_F0x8009c9a0;
    *(void **)((char *)tbl + 0xf0c) = (void *)CARDGAME_F0x8009ca3c;
    *(void **)((char *)tbl + 0xf10) = (void *)CARDGAME_LAB_8009ca80;
    *(void **)((char *)tbl + 0xf2c) = (void *)CARDGAME_F0x8009c654;
    *(void **)((char *)tbl + 0xf30) = (void *)CARDGAME_F0x8009c6e8;
    *(void **)((char *)tbl + 0xf34) = (void *)CARDGAME_F0x8009c770;
    *(void **)((char *)tbl + 0xf38) = (void *)CARDGAME_LAB_8009c7f8;
    *(void **)((char *)tbl + 0xf28) = (void *)CARDGAME_F0x8009c5d8;
    *(void **)((char *)tbl + 0xf14) = (void *)CARDGAME_F0x8009c510;
    *(void **)((char *)tbl + 0xf18) = (void *)CARDGAME_LAB_8009cdfc;
    *(void **)((char *)tbl + 0xf1c) = (void *)CARDGAME_F0x8009ce28;
    *(void **)((char *)tbl + 0xf20) = (void *)CARDGAME_F0x8009c904;
    *(void **)((char *)tbl + 0xf24) = (void *)CARDGAME_F0x8009c938;
    *(void **)((char *)tbl + 0xf04) = (void *)CARDGAME_F0x8009ce94;
    *(void **)((char *)tbl + 0x50) = arg;
    *(void **)((char *)tbl + 0xec4) = (void *)CARDGAME_LAB_8009c350;
    *(void **)((char *)tbl + 0xec8) = (void *)CARDGAME_LAB_8009c2d0;
    *(void **)((char *)tbl + 0xebc) = (void *)CARDGAME_LAB_8009c438;
    *(void **)((char *)tbl + 0xed0) = (void *)CARDGAME_LAB_8009c4a4;
    *(void **)((char *)tbl + 0xed4) = (void *)CARDGAME_LAB_8009c4d8;
    *(void **)((char *)tbl + 0xec0) = (void *)CARDGAME_LAB_8009c3c0;
    *(void **)((char *)tbl + 0xecc) = (void *)CARDGAME_LAB_8009c264;
    *(void **)((char *)tbl + 0xf3c) = (void *)CARDGAME_F0x8009cfbc;
    *(void **)((char *)tbl + 0xf40) = (void *)CARDGAME_F0x8009cf60;
    *(void **)((char *)tbl + 0xf44) = (void *)CARDGAME_F0x8009d15c;
}
