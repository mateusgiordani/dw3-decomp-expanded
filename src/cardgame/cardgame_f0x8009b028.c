/*
 * CARDGAME:0x8009b028 CARDGAME_F0x8009b028
 * 320 bytes at CARDGAME.PRO offset 0x18378 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009b028
 *  Symbols     EXE_80044b38=0x80044b38 F0x8001f648=0x8001f648
 *  Compare     320 bytes from 0x8009b028 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009b028
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 320-byte PAL body at verified base 0x80082cb0.
 *
 * The spacing temporary with o2-g0-no-strength-reduce keeps k*8+3 grouped
 * before the coordinate load.
 */

typedef struct {
    void (*fn)();
    int unk;
} CardWorkSlot;

typedef struct {
    unsigned char data[116];
    CardWorkSlot slot[5];
    unsigned char tail[4];
} CardWork;

typedef struct {
    int w0;         /* +0x00 */
    int w1;         /* +0x04 */
    char pad08[16]; /* +0x08..0x17 */
    int w6;         /* +0x18 */
    char pad1c[34]; /* +0x1c..0x3d */
    char b3e[3];    /* +0x3e..0x40 */
    char pad41[4];  /* +0x41..0x44 */
    char b45;       /* +0x45 */
} CardCtx;

typedef struct {
    char pad[0x424];    /* +0x000..0x423 (contents unknown) */
    int (*cb)(int arg); /* +0x424 */
} ExeCbBlock;

extern void F0x8001f648(void *work);
extern ExeCbBlock EXE_80044b38;

void CARDGAME_F0x8009b028(int a0, CardCtx *s)
{
    CardWork w;
    int i;
    int k;
    int resource;
    int spacing;

    if (s->b45 != 0) {
        i = 0;
        k = 0;
        for (; i < 3; i++) {
            if (s->b3e[i] != 0) {
                F0x8001f648(&w);
                if (s->w6 != 0x10001000) {
                    w.slot[4].fn((s->w0 >> 8) + 0x14, (s->w1 >> 8) + 0x17);
                    w.slot[3].fn((short)s->w6, *(short *)((char *)s + 0x1a), 0x1000);
                }
                w.slot[1].fn(0x100, 1);
                w.slot[0].fn(0x280, 0);
                resource = EXE_80044b38.cb(0x25d0002);
                spacing = k * 8 + 3;
                w.slot[2].fn(resource, i + 0x37,
                             (s->w0 >> 8) + spacing, (s->w1 >> 8) + 0x15);
                k++;
            }
        }
    }
}
