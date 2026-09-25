/*
 * CARDGAME:0x8009b678 CARDGAME_F0x8009b678
 * 244 bytes at CARDGAME.PRO offset 0x189c8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009b678
 *  Symbols     CARDGAME_F0x8009ba3c=0x8009ba3c DAT_80044f5c=0x80044f5c
 *              F0x8001f648=0x8001f648
 *  Compare     244 bytes from 0x8009b678 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009b678
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdffb8 addiu sp,-0xb8 ; sw s0,0xb0(sp) ; move
 * s0,a1 ; sw ra,0xb4(sp) ; epilogue lw ra,0xb4(sp) ; lw s0,0xb0(sp) ; jr ra ;
 * addiu sp,+0xb8. Next function CARDGAME:0x8009b76c at +0xF4 confirms size 0xF4
 * contiguous (prologue/prologue pair).
 *
 * Sibling CARDGAME:0x8009b76c shares the prologue/frame idiom with an extra
 * slot call and last-arg 9 vs 10 here; guards differ (0x49 vs 0x42==11).
 */

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
