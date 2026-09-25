// CARDGAME:0x8008423c (size 228, 0xe4) -- portable C, C_NONMATCHING (blocked exact)
// Status rev5: C shape PROVEN complete by diagnostic chunk-aware link (228/228 + rodata
// 44/44 exact, 0 diff bytes); configured pipeline shows 38 diff bytes left, all from
// six post-.rdata .text relocs applied flat instead of chunk-relative (see blocker).
// rodata 44/44 exact via pipeline. int-b fix this rev (sltiu->slti x3, semantics kept:
// b in [0,255] compares identically signed): pipeline 41->38 diff bytes.
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x158c
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #3:
// prologue 27bdffe8 addiu sp,-0x18 ; clear a2 ; sw ra,0x10(sp),
// epilogue lw ra,0x10(sp) ; nop ; jr ra ; addiu sp,+0x18 (frame -0x18/+0x18).
// Prev framed CARDGAME:0x80084188 size 0xb4 ends exactly at 0x8008423c (no gap);
// next framed CARDGAME:0x80084320 at +0xe4 (no gap).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm confirms
// lbu/addiu-30/sltiu-11 range check, lui 0x8008 + addiu 0x2cc8 table base,
// sll 2 + jr dispatch, j-with-li-delay case stubs, lbu/slti/slti-delay branch
// pair, lb/addiu-1/sll-3/addu/lbu table reads, xori 1, jal 0x8008f2ec.
// Decompile hypothesis matches this C shape (switch 0x1e..0x28, two table paths).
// cardgame.s is GUIDE only, never copied. No Ghidra state change.
// Callers: 1 direct jal (PAL bin scan for 0c02108f at file-off 0x2608 = vaddr 0x800852b8).
// Callees: 1 direct jal CARDGAME_F0x8008f2ec(a0 passthrough, a1 passthrough,
// a2 = table byte or table byte ^ 1 or 0, a3 = switch value 0..4).
// Jump table: 11 words at 0x80082cc8 (values 30..40 -> a3 0,1,2,2,3,4,0,1,2,3,4).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0, --rodata 0x80082cc8.
// BLOCKER pipeline-linker chunk awareness (aspsx EXONERATED): all pinned aspsx emit
// post-.rdata-switch .text relocs chunk-relative (here -68B = pre-switch chunk size),
// a format-wide convention, not a per-version bug (2.56 cannot even assemble this .s).
// tools/verify_match._link applies them flat, corrupting 4 j-stub targets, j $L16
// and jal (38 diff bytes). Chunk-aware re-link of the same .obj is byte-exact.
// Coordinator fix: teach _link (or link_with_rodata) mid-function island replay;
// then rerun the configured pipeline for exact_byte_match. See handoff rev5 +
// docs/c-matching-guide/submissions/cardgame-8008423c/strategy-r5.md (int-b C fix applied this rev).

extern void CARDGAME_F0x8008f2ec(void *p, unsigned int q, unsigned int r, unsigned int s);

void CARDGAME_F0x8008423c(unsigned char *p, unsigned int q) {
    unsigned int ctl = 0;
    unsigned int sel = ctl;

    switch (p[0x420]) {
    case 30:
    case 36:
        sel = 0;
        break;
    case 31:
    case 37:
        sel = 1;
        break;
    case 32:
    case 33:
    case 38:
        sel = 2;
        break;
    case 34:
    case 39:
        sel = 3;
        break;
    case 35:
    case 40:
        sel = 4;
        break;
    }

    {
        int b = p[0x420];

        if (b >= 30) {
            if (b < 36) {
                ctl = *(unsigned char *)(p + ((((signed char *)p)[0x575] - 1) << 3) + 0x584);
            } else if (b < 41) {
                ctl = *(unsigned char *)(p + ((((signed char *)p)[0x575] - 1) << 3) + 0x584) ^ 1;
            }
        }
    }

    CARDGAME_F0x8008f2ec(p, q, ctl, sel);
}
