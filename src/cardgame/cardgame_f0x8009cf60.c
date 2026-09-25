// CARDGAME:0x8009cf60 (size 92, 0x5c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1a2b0
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #174
// Prologue 27bdff88 addiu sp,-0x78 ; sw s1,0x6c(sp) ; addu s1,a0 ; sw s0,0x68(sp) ; addu s0,a1 ; sw ra,0x70(sp) ; jal 0x8001ebf8 ; addiu a0,sp,0x10
// Body: lw v0,0x50(s1) ; sll s0,s0,1 ; addu s0,s0,v0 ; lh a0,0(s0) ; lw v0,0x3c(sp) ; nop ; jalr ra,v0 ; addiu a0,a0,1 ; lw v0,0x10(sp) ; lw ra,0x70(sp) ; lw s1,0x6c(sp) ; lw s0,0x68(sp) ; lbu v0,0(v0) ; jr ra ; addiu sp,+0x78
// Next function CARDGAME:0x8009cfbc at +0x5c, prev CARDGAME:0x8009ce94 ends exactly at 0x8009cf60; size 0x5c is self-contained (prologue/epilogue pair).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm and decompile confirm semantics; no state changed.
// Callers 0 in overlay (no 0x0c0273d8 jal to here); callee jal EXE:0x8001ebf8 plus indirect jalr via stack slot; frame -0x78/+0x78 verified.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match with --symbol func_8001ebf8=0x8001ebf8.
// Semantic: EXE helper fills stack buffer at sp+0x10; base=*(a0+0x50); h=base[a1] signed halfword; cb=*(buf+0x2c); cb(h+1); return byte at *buf.
// Buffer void *buf[22] (88 B, vars=88 -> frame 0x78): only words 0 (result pointer) and 11 (callback) are read here; the rest is written by the EXE helper and untouched.

extern void func_8001ebf8(void *buf);

unsigned char CARDGAME_F0x8009cf60(void *a0, int a1)
{
    void *buf[22];
    short *base;
    short h;
    void (*cb)(int);

    func_8001ebf8(buf);
    base = *(short **)((char *)a0 + 0x50);
    h = base[a1];
    cb = *(void (**)(int))((char *)buf + 0x2c);
    cb(h + 1);
    return *(unsigned char *)*(void **)buf;
}
