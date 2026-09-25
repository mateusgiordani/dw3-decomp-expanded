/*
 * STCRDDEK:0x80084440 STCRDDEK_func_80084440
 * 896 bytes at STCRDDEK.PRO offset 0x1790 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80084440
 *  Symbols     D_80044B38=0x80044b38 D_8005CCA8=0x8005cca8
 *              F0x8001ebf8=0x8001ebf8 STCRDDEK_func_80084440=0x80084440
 *  Compare     896 bytes from 0x80084440 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80084440
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * D_8005CCA8 (int at 0x8005cca8, s3 base via lui 0x8006/-0x3358) and
 * D_80044B38.fn414 (slot at 0x80044f4c = base+0x414, s2 base) mirror the
 * STCRDDEK_func_8008a100 TU idiom (D_80044B38/D_8005CCA8 externs).
 *
 * p2 is 7 object pointers at +0x44..0x60; each object dispatches via slots
 * +0x114 (set3), +0x118 (set2i), +0x144 (set1i), +0x148 (set1i) -- indices
 * 68/70/81/82. p1 carries two counts at +0x74/+0x78 selecting one int16 from
 * the table at +0x88. Deck/album role unconfirmed; names conservative.
 *
 * No Ghidra state change.
 *
 * Matching notes: s is an int loaded from the int16 table (lh, no
 * re-extension); the ret != 0 DRAW(0x60) calls +0x114 through an int return
 * type so jump2 does not cross-jump it with the id-list DRAW(0x60) (same shape
 * as STCRDSHP:0x80083780).
 */

extern void F0x8001ebf8(void *);
extern int D_8005CCA8;
extern struct {
    char pad[0x414];
    int (*fn414)(int);
} D_80044B38;

void STCRDDEK_func_80084440(void *p1, void *p2, int flag)
{
    unsigned int buf[22];
    int s;
    int u;
    int ret;
    void *o;

    if (flag != 0) {
        int n;

        n = *(int *)((char *)p1 + 0x78) + *(int *)((char *)p1 + 0x74);
        s = *(short *)((char *)p1 + (n << 1) + 0x88);
        u = D_80044B38.fn414(D_8005CCA8 + 0x16);
        o = *(void **)((char *)p2 + 0x44);
        ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, s);
        F0x8001ebf8(buf);
        ((void (*)(int))buf[11])(s);
        ret = ((int (*)(void))buf[20])();
        if (ret != 0) {
            o = *(void **)((char *)p2 + 0x48);
            ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
            o = *(void **)((char *)p2 + 0x4c);
            ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
            u = D_80044B38.fn414(D_8005CCA8 + 0x1d);
            o = *(void **)((char *)p2 + 0x60);
            ((int (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, s);
        } else {
            u = D_80044B38.fn414(D_8005CCA8 + 0x32);
            o = *(void **)((char *)p2 + 0x48);
            ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, 8);
            o = *(void **)((char *)p2 + 0x4c);
            ((void (*)(void *, int, int))*(void **)((char *)o + 0x118))(o, 0, ((unsigned char *)buf[0])[5]);
            o = *(void **)((char *)p2 + 0x4c);
            ((void (*)(void *, int))*(void **)((char *)o + 0x148))(o, 1);
            if (s == 0x45 || s == 0x70 || s == 0x9b || s == 0xc6 || s == 0xf1) {
                u = D_80044B38.fn414(D_8005CCA8 + 0x1d);
                o = *(void **)((char *)p2 + 0x60);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, s);
            } else {
                o = *(void **)((char *)p2 + 0x60);
                ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
                u = D_80044B38.fn414(D_8005CCA8 + 0x32);
                o = *(void **)((char *)p2 + 0x50);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, 0x11);
                o = *(void **)((char *)p2 + 0x54);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x118))(o, 0, ((unsigned char *)buf[0])[1]);
                o = *(void **)((char *)p2 + 0x54);
                ((void (*)(void *, int))*(void **)((char *)o + 0x148))(o, 1);
                u = D_80044B38.fn414(D_8005CCA8 + 0x32);
                o = *(void **)((char *)p2 + 0x58);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, 0x12);
                o = *(void **)((char *)p2 + 0x5c);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x118))(o, 0, ((unsigned char *)buf[0])[2]);
                o = *(void **)((char *)p2 + 0x5c);
                ((void (*)(void *, int))*(void **)((char *)o + 0x148))(o, 1);
                return;
            }
        }
    } else {
        o = *(void **)((char *)p2 + 0x44);
        ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
        o = *(void **)((char *)p2 + 0x48);
        ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
        o = *(void **)((char *)p2 + 0x4c);
        ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
        o = *(void **)((char *)p2 + 0x60);
        ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
    }
    o = *(void **)((char *)p2 + 0x50);
    ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
    o = *(void **)((char *)p2 + 0x54);
    ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
    o = *(void **)((char *)p2 + 0x58);
    ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
    o = *(void **)((char *)p2 + 0x5c);
    ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
}
