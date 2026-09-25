// CARDGAME:0x8009b678 (size 244, 0xF4)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x189c8
// Framed function: prologue 27bdffb8 addiu sp,-0xb8 ; sw s0,0xb0(sp) ;
// move s0,a1 ; sw ra,0xb4(sp) ; epilogue lw ra,0xb4(sp) ; lw s0,0xb0(sp) ;
// jr ra ; addiu sp,+0xb8. Next function CARDGAME:0x8009b76c at +0xF4
// confirms size 0xF4 contiguous (prologue/prologue pair).
// Ghidra program CARDGAME read-only (project ddw3-pal-sles-03936, base 0x80082cb0):
// disasm 61 words match PAL word-for-word; decompile is a guarded 5-callback
// sequence through a stack work area filled by EXE helper 0x8001f648;
// x-ref to 0x8009b678: caller 0x8009baa0 (CARDGAME_F0x8009ba3c) UNCONDITIONAL_CALL.
// Upstream cardgame.s GUIDE only (.L0x00018df0 jal 0x8009b678 corroborates caller).
// Sibling CARDGAME:0x8009b76c shares the prologue/frame idiom with an extra
// slot call and last-arg 9 vs 10 here; guards differ (0x49 vs 0x42==11).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).

typedef struct {
    void (*fn)();
    int unk;
} CardWorkSlot;

typedef struct {
    unsigned char data[116];
    CardWorkSlot slot[5];
} CardWork;

typedef struct {
    int w0;         /* +0x00 */
    int w1;         /* +0x04 */
    char pad08[16]; /* +0x08..0x17 */
    int w6;         /* +0x18 */
    char pad1c[41]; /* +0x1c..0x44 */
    char b45;       /* +0x45 */
    char pad46[3];  /* +0x46..0x48 */
    char b49;       /* +0x49 */
} CardCtx;

extern void F0x8001f648(void *work);
extern int (*DAT_80044f5c)(int arg);

void CARDGAME_F0x8009b678(int a0, CardCtx *s)
{
    CardWork w;

    if (s->b45 != 0 && s->b49 != 0) {
        F0x8001f648(&w);
        if (s->w6 != 0x10001000) {
            w.slot[4].fn((s->w0 >> 8) + 0x14, (s->w1 >> 8) + 0x17);
            w.slot[3].fn((short)s->w6, *(short *)((char *)s + 0x1a), 0x1000);
        }
        w.slot[1].fn(0x100, 1);
        w.slot[0].fn(0x340, 0);
        w.slot[2].fn(DAT_80044f5c(0x25d0003), 10, s->w0 >> 8, s->w1 >> 8);
    }
}
