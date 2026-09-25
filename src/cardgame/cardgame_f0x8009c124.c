// CARDGAME:0x8009c124 (size 72, 0x48)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x19474
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #158
// Prologue 27bdffe8 addiu sp,-0x18 ; sw s0,0x10(sp) ; move s0,a0 ; lui a0,0x8004 ; lui v0,0x8005 ; lw v0,0x5c48(v0) ; sw ra,0x14(sp) ; jalr v0 ; ori a0,0x503c delay
// Body: li v0,10 ; sh v0,0xdf2(s0) ; sh v0,0xdf0(s0) ; li v0,4 ; sb v0,0xdfa(s0) ; lw ra,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,+0x18
// Next function CARDGAME:0x8009c174 at +0x50 (8 B gap), prev CARDGAME:0x8009c0dc at -0x48 confirms size 0x48 self-contained.
// Ghidra: CARDGAME_F0x8009c124 decompiled as (*_DAT_80055c48)(0x8004503c); *(short*)(param+0xdf2)=10; *(short*)(param+0xdf0)=10; *(char*)(param+0xdfa)=4;
// Caller: DATA XREF from CARDGAME_F0x8009d310 (0x8009d3b4); no direct jal callers (table-driven wrapper)
// Callee: indirect jalr via v0 loaded from *0x80055c48 (EXE global)
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0, portable C exact_byte_match (0 relocations)

void CARDGAME_F0x8009c124(void *arg) {
    (*(void (**)(char *))0x80055c48)((char *)0x8004503c);
    *(short *)((char *)arg + 0xdf2) = 10;
    *(short *)((char *)arg + 0xdf0) = 10;
    *(char *)((char *)arg + 0xdfa) = 4;
}
