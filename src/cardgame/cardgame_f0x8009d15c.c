/*
 * CARDGAME:0x8009d15c CARDGAME_F0x8009d15c
 * 436 bytes at CARDGAME.PRO offset 0x1a4ac (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8009d15c
 *  Symbols     CARDGAME_F0x8009d0b0=0x8009d0b0 CARDGAME_F0x8009d140=0x8009d140
 *              CARDGAME_F0x8009d15c=0x8009d15c DAT_800a5bdc=0x800a5bdc
 *              func_8001ebf8=0x8001ebf8 func_8001ffa8=0x8001ffa8
 *  Compare     436 bytes from 0x8009d15c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009d15c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdff40 addiu sp,-0xc0, epilogue jr ra / addiu
 * sp,+0xc0.
 *
 * Prev CARDGAME:0x8009d140 (7 words, short-table lookup) ends exactly at
 * 0x8009d15c; next framed CARDGAME:0x8009d310 at +0x1b4 (27bdffe8). No Ghidra
 * mutation.
 *
 * Stack: ctx at sp+0x10 (56 B, EXE 0x8001ffa8 render buffer, callbacks at
 * +0x1c..0x30 read by callee 0x8009d0b0 via s1+0x20/0x24/0x28); hbuf at sp+0x48
 * (88 B, EXE 0x8001ebf8 buffer, word0 = base, callback at +0x2c = sp+0x74, same
 * convention as 0x8008d854).
 *
 * Frame 0x10 outgoing + 56 + 88 locals + 7 saved (s0-s5,ra) = 0xc0.
 *
 * Callers: none direct; DATA ref from CARDGAME:0x8009d310 (callback table
 * registered via
 *
 * EXE 0x80014504). Callees: EXE 0x8001ebf8/0x8001ffa8, CARDGAME
 * 0x8009d0b0/0x8009d140, plus indirect jalr via hbuf callback (load-delay nop,
 * addiu/move in delay slots).
 *
 * Details: strategy-r9-o55/attempts-r9.
 */

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
