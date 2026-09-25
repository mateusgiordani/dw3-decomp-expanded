// CARDGAME:0x8009a75c (size 208, 0xd0)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x17aac
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #141
// Prologue 27bdffe0 addiu sp,-0x20 ; sw s0,0x10(sp) ; move s0,a1 ; lui v0,0x2aaa ;
// sw ra,0x18(sp) ; sw s1,0x14(sp) ; lw a0,0x28(s0) ; ori v0,0xaaab ; sll a0,0xc ;
// mult a0,v0 ; sra a0,0x1f ; mfhi a2 ; sra v0,a2,2 ; jal 0x8002abcc (EXE F0x8002abcc) ;
// subu a0,v0,a0 (delay) ; li v1,0x1000 ; subu v1,v1,v0 ; lui v0,0x8005 ;
// sh v1,0x18(s0) ; lw v0,-0x2064(v0) (= *0x8004df9c, EXE vector) ; nop ;
// jalr v0 ; clear s1 (delay) ; lw v1,0x28(s0) ; lw a0,0x2c(s0) ; addu v0,v1,v0 ;
// bne a0,zero -> L1 ; sw v0,0x28(s0) (delay) ; slti v0,7 ; bne v0,zero -> L1 ;
// li v1,1 (delay) ; lbu v0,0x45(s0) ; sw v1,0x2c(s0) ; xori v0,3 ; sb v0,0x45(s0) ;
// L1: lw v0,0x28(s0) ; nop ; slti v0,0xc ; bne v0,zero -> ret ; move v0,s1 (delay) ;
// li s1,1 ; move v0,s1 ; sb v0,0x42(s0) ; li v0,0x1000 ; sh zero,0x26(s0) ;
// sh v0,0x18(s0) ; move v0,s1 ; ret: lw ra,0x18(sp) ; lw s1,0x14(sp) ;
// lw s0,0x10(sp) ; jr ra ; addiu sp,+0x20
// Prev CARDGAME:0x8009a62c (304B) ends exactly at 0x8009a75c ; next framed
// CARDGAME:0x8009a82c at +0xd0 (contiguous) confirms size 0xd0 self-contained.
// Ghidra program CARDGAME read-only (never import); cardgame.s GUIDE only
// (.L0x00017aac file-off label matches PAL first word 27bdffe0).
// Caller: 1 direct jal from CARDGAME_F0x8009b890+0x80 (0x8009b910), same
// ori-v0/flag idiom as sibling calls to 0x8009a1a0/0x8009a62c in guide.
// Callee 1: EXE F0x8002abcc (DISASSEMBLED, symbols/functions.csv) via direct jal,
// takes ((*(s0+0x28) << 12) / 24), returns int stored as 0x1000 - ret at +0x18.
// Callee 2: indirect jalr via v0 loaded from *0x8004df9c (lui 0x8005 + lw -0x2064),
// takes void, return added to *(s0+0x28); same EXE-vector idiom as the
// 0x80055c48 family (0x8009bf5c/0x8009c0dc C_MATCHING precedent).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base hypothesis).
// Signature conservative: a0 incoming value never read (first use of a0 is a lw
// destination), a1 is the context pointer, s1 flag (0/1) returned.
// flag lives in callee-saved s1 (cleared in the jalr delay slot, set in the
// tail) which forces the -0x20 frame with s0/s1/ra saves. Known 1-word
// divergence: PAL forwards the just-set flag through v0 for the +0x42 store
// (li s1,1 ; move v0,s1 ; sb v0), while gcc-2.8.1 CSE folds the constant and
// stores from s1 directly, leaving the object one word short (see blocker).
// No struct invented: char * + explicit offsets preserve the L1 reload of
// *(ctx+0x28) (char * may alias, forcing the observed second lw).

extern int F0x8002abcc(int arg);

/* EXE-resident vector slot: Ghidra names the dword _DAT_8004df9c and shows
   (*_DAT_8004df9c)() ; PAL loads it with lui v0,0x8005 + lw v0,-0x2064(v0).
   Declared as an extern pointer (resolved with --symbol DAT_8004df9c=0x8004df9c)
   so the compiler emits a relocatable lui+lw pair the scheduler can split
   around the +0x18 store, as in the original; same extern-pointer precedent
   as cardgame_f0x8008ad58.c (func_80089ed4 + --symbol). */
extern int (*DAT_8004df9c)(void);

int CARDGAME_F0x8009a75c(void *p1, char *ctx)
{
    int acc;
    int nv;
    int flag;

    (void)p1;
    *(short *)(ctx + 0x18) = (short)(0x1000 - F0x8002abcc(((*(int *)(ctx + 0x28)) << 12) / 24));
    flag = 0;
    acc = DAT_8004df9c();
    nv = *(int *)(ctx + 0x28) + acc;
    *(int *)(ctx + 0x28) = nv;
    if (*(int *)(ctx + 0x2c) == 0 && nv >= 7) {
        *(int *)(ctx + 0x2c) = 1;
        *(char *)(ctx + 0x45) ^= 3;
    }
    if (*(int *)(ctx + 0x28) >= 12) {
        /* PAL emits li s1,1 ; move v0,s1 ; sb v0,0x42(s0) here: the sb
           constant and flag need separate li copies. Setting flag after the
           stores reproduces the 208B body exactly (rev6 A4); setting it
           first lets CSE fold both into one li s1,1 stored from s1 (204B).
           Order is unobservable: flag is local, stores hit distinct
           addresses. */
        *(char *)(ctx + 0x42) = 1;
        *(short *)(ctx + 0x26) = 0;
        *(short *)(ctx + 0x18) = 0x1000;
        flag = 1;
    }
    return flag;
}
