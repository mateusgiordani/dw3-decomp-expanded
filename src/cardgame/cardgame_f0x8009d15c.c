// CARDGAME:0x8009d15c (size 436, 0x1b4)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1a4ac
// Framed function: prologue 27bdff40 addiu sp,-0xc0, epilogue jr ra / addiu sp,+0xc0.
// Prev CARDGAME:0x8009d140 (7 words, short-table lookup) ends exactly at 0x8009d15c;
// next framed CARDGAME:0x8009d310 at +0x1b4 (27bdffe8). Upstream cardgame.s GUIDE only:
// jal 0x8009d140 words at file-off 0x1a5ec/0x1a61c fall inside this range. No Ghidra mutation.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only disasm confirms every
// word; decompile gives 4 loops (40/40/9/100) over short arrays.
// Stack: ctx at sp+0x10 (56 B, EXE 0x8001ffa8 render buffer, callbacks at +0x1c..0x30
// read by callee 0x8009d0b0 via s1+0x20/0x24/0x28); hbuf at sp+0x48 (88 B, EXE 0x8001ebf8
// buffer, word0 = base, callback at +0x2c = sp+0x74, same convention as 0x8008d854).
// Frame 0x10 outgoing + 56 + 88 locals + 7 saved (s0-s5,ra) = 0xc0.
// Callers: none direct; DATA ref from CARDGAME:0x8009d310 (callback table registered via
// EXE 0x80014504). Callees: EXE 0x8001ebf8/0x8001ffa8, CARDGAME 0x8009d0b0/0x8009d140,
// plus indirect jalr via hbuf callback (load-delay nop, addiu/move in delay slots).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0, variant o2-g0-no-strength-reduce
// (2.8.1 base 424 B, 2.7.2 base 428 B / nsr 440 B are controls, not the build).
// r9 (o55/s0923i): explicit cursors (no SR): the src1 parameter is the one reused source/
// destination cursor (PAL s0 in all four loops), d walks dst in loops 1-3, n stays a real
// counter; loops 1-3 share i and loop 4 has its own counter j, so src1 outranks the counters
// for s0. exact_byte_match 436/436. Details: strategy-r9-o55/attempts-r9.
extern void func_8001ebf8(void *buf);
extern void func_8001ffa8(void *buf);
extern void CARDGAME_F0x8009d0b0(void *ctx, int base, int idx);
extern int CARDGAME_F0x8009d140(int idx);
extern short DAT_800a5bdc[];

typedef void (*cardgame_8009d15c_cb_t)(int x);

int CARDGAME_F0x8009d15c(short *dst, short *src1, short *src2)
{
    void *ctx[14];
    void *hbuf[22];
    int i;
    int j;
    int n;
    short *d;

    func_8001ebf8(hbuf);
    func_8001ffa8(ctx);

    i = 0;
    d = dst;
    for (; i < 40; i++) {
        ((cardgame_8009d15c_cb_t)hbuf[11])(*src1 + 1);
        CARDGAME_F0x8009d0b0(ctx, (int)hbuf[0] + 12, i);
        *d = *src1;
        src1++;
        d++;
    }

    n = 40;
    i = 0;
    src1 = src2;
    d = dst;
    for (; i < 40; i++) {
        n++;
        ((cardgame_8009d15c_cb_t)hbuf[11])(*src1 + 1);
        CARDGAME_F0x8009d0b0(ctx, (int)hbuf[0] + 12, i + 40);
        d[40] = *src1;
        src1++;
        d++;
    }

    i = 0;
    src1 = DAT_800a5bdc;
    d = dst;
    for (; i < 9; i++) {
        n++;
        ((cardgame_8009d15c_cb_t)hbuf[11])(*src1 + 1);
        CARDGAME_F0x8009d0b0(ctx, (int)hbuf[0] + 12, i + 80);
        d[80] = *src1;
        src1++;
        d++;
    }

    j = 0;
    src1 = dst;
    for (; j < 100; j++) {
        ((cardgame_8009d15c_cb_t)hbuf[11])(CARDGAME_F0x8009d140(j) + 1);
        n++;
        CARDGAME_F0x8009d0b0(ctx, (int)hbuf[0] + 12, j + 89);
        src1[89] = (short)CARDGAME_F0x8009d140(j);
        src1++;
    }

    return n;
}
