// CARDGAME:0x8009cfbc (size 244, 0xf4)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1a30c
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #175
// Prologue 27bdff88 addiu sp,-0x78 ; sw s2,0x70(sp) ; move s2,a0 ; sw s0,0x68(sp) ; move s0,a1 ; sw s1,0x6c(sp) ; move s1,a2 ; sw ra,0x74(sp) ; jal 0x8001ebf8 ; addiu a0,sp,0x10
// Body: lw v1,0x50(s2) ; sll v0,s1,1 ; addu v0,v1 ; lh a0,0(v0) ; lw v0,0x3c(sp) ; nop ; jalr v0 ; addiu a0,a0,1 ; v0=s0*0x4c via sll/addu/subu chain ; addu s2,v0 ; sh s1,0x142(s2) ; 5x (lw v0,0x10(sp) ; lbu ; sb) to +0x14b/+0x14c/+0x149 ; lbu 0(v0) ; addiu -1 ; sh +0x140 ; lbu v1,3(v0) vs 0x10 -> sh 1/0 to +0x144 (then-branch store in j delay slot)
// Epilogue lw ra,0x74(sp) ; lw s2,0x70(sp) ; lw s1,0x6c(sp) ; lw s0,0x68(sp) ; jr ra ; addiu sp,+0x78
// Next function CARDGAME:0x8009d0b0 at +0xf4 (contiguous: 0x8009cfbc+0xf4=0x8009d0b0), prev CARDGAME:0x8009cf60 size 0x5c ends exactly at 0x8009cfbc; size 0xf4 is self-contained (prologue/epilogue pair).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 61 words plus next-function prologue, decompile confirms semantics; no state changed.
// Callers: 0 direct jal (word 0x0c0273ef occurs 0 times in cardgame.bin); one DATA xref from CARDGAME_F0x8009d310 at 0x8009d514 (table-store sw, indirect/table-referenced, kept conservative per R15); callee jal EXE:0x8001ebf8 plus indirect jalr via stack slot; frame -0x78/+0x78 verified.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match with --symbol func_8001ebf8=0x8001ebf8.
// Semantic: EXE helper fills stack buffer at sp+0x10; h=base[a2] signed halfword from *(a0+0x50); cb=*(buf+0x2c); cb(h+1); row=(char *)a0+a1*0x4c; row[0x142]=(short)a2; row[0x14b/0x14c/0x149]=buf0[1/2/5]; row[0x140]=(short)(buf0[0]-1); if (buf0[3]==0x10) row[0x144]=1 else row[0x144]=0.
// Buffer void *buf[22] (88 B, frame 0x78 with buffer at sp+0x10, callback at sp+0x3c = buf+0x2c, result pointer at sp+0x10 = buf[0]): only words 0 and 11 are read here; the rest is written by the EXE helper and untouched.
// Matching note: the helper result pointer at buf[0] is re-fetched from the
// stack before every byte access in the original codegen (5x lw v0,0x10(sp));
// CSE would otherwise fold the next-to-last fetch into the last across the
// halfword store, so the final access uses a volatile-qualified fetch to
// preserve the observed reload. Standard C, no inline asm.

extern void func_8001ebf8(void *buf);

void CARDGAME_F0x8009cfbc(void *a0, int a1, int a2)
{
    void *buf[22];
    short *base;
    short h;
    void (*cb)(int);
    char *p;

    func_8001ebf8(buf);
    base = *(short **)((char *)a0 + 0x50);
    h = base[a2];
    cb = *(void (**)(int))((char *)buf + 0x2c);
    cb(h + 1);
    p = (char *)a0 + a1 * 0x4c;
    *(short *)(p + 0x142) = (short)a2;
    *(p + 0x14b) = (*(unsigned char **)buf)[1];
    *(p + 0x14c) = (*(unsigned char **)buf)[2];
    *(p + 0x149) = (*(unsigned char **)buf)[5];
    *(short *)(p + 0x140) = (short)(*(unsigned char **)buf)[0] - 1;
    if ((*(unsigned char *volatile *)buf)[3] == 0x10)
        *(short *)(p + 0x144) = 1;
    else
        *(short *)(p + 0x144) = 0;
}
