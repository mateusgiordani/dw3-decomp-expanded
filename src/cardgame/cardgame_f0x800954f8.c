/*
 * CARDGAME:0x800954f8 CARDGAME_F0x800954f8
 * 1796 bytes at CARDGAME.PRO offset 0x12848 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800954f8, jump table (.rodata) at 0x800836a0
 *  Symbols     F0x8001ebf8=0x8001ebf8
 *  Compare     1796 bytes from 0x800954f8 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800954f8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * complete body/range 1796 bytes (0x704).
 *
 * PAL-SLES-03936: PRO/CARDGAME.BIN, base 0x80082cb0, file offset 0x12848.
 *
 * Entry 27bdfe68; return at 0x80095bf4 with 27bd0198 at 0x80095bf8.
 *
 * Next function starts at 0x80095bfc. Frame 0x198; saves s0-s7 and ra.
 *
 * Code SHA-256:
 * f3a736132b5767f3136823d299d79aa97042b9f937828aa09263233647fac9f7.
 *
 * Both native jump tables and their alignment word: 80 bytes at 0x800836a0.
 *
 * Native recipe: PsyQ GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79, -O2 -G0 -mips1
 * -msoft-float, variant base; no code normalization/padding.
 *
 * E29 reproduces every code and rodata byte.
 *
 * The worker's matching evidence does not itself promote the shared catalog.
 *
 * Caller 0x800a22e0 uses a0=context, a1=card and a2=mode; the integer result is
 * consumed.
 *
 * Four 88-byte stack work areas retain their +0x2c callback slots. The card
 * callback remains indirect through card+0xf40; its full target set is unknown.
 *
 * Offset views below describe only this function's observed accesses and
 * strides, not globally recovered object capacities or semantic type names.
 */

#include <stdint.h>

/* Local access view: PAL records have stride 8 and fields at +4,+5,+6.
 * The four-row view spans 0x580..0x59f; no global semantic naming is implied. */
typedef struct {
    unsigned char unknown[4];
    unsigned char side, selector, argument, unknown7;
} TurnRecord954f8;
typedef struct {
    unsigned char prefix[0x580];
    TurnRecord954f8 records[4];
} TurnView954f8;

/* Offset views, not canonical object/capacity declarations. */
typedef struct {
    short cursor, check2, check4, count;
    unsigned char unknown8[8];
    short deck[40];
    short hand[6];
    unsigned char unknown6c[0x5c];
} SideRecord954f8;
typedef struct {
    unsigned char prefix[0x5a0];
    SideRecord954f8 sides[2];
} SideView954f8;
typedef struct {
    unsigned char prefix[0x50];
    short values[1]; 
} ValuesView954f8;
typedef char SideStride954f8[(sizeof(SideRecord954f8) == 200) ? 1 : -1];

typedef struct {
    unsigned char prefix[0x46f];
    unsigned char flags[12];
} FlagsView954f8;

typedef struct {
    short value;
    unsigned char unknown2[12];
} Row14_954f8;
typedef struct {
    unsigned char prefix[0x72e];
    Row14_954f8 rows[6];
} FirstRows954f8;
typedef struct {
    unsigned char prefix[0x7a0];
    Row14_954f8 rows[6];
} SecondRows954f8;
typedef char RowStride954f8[(sizeof(Row14_954f8) == 14) ? 1 : -1];

extern void F0x8001ebf8(void *);

typedef void (*cardgame_54f8_cb_t)(int);
typedef int (*cardgame_54f8_cardfn_t)(void *, int);

int CARDGAME_F0x800954f8(void *ctx, void *card, int mode)
{
    unsigned int buf2[22];
    unsigned int buf3[22];
    unsigned int buf7[22];
    unsigned int buf8[22];
    signed char turn;
    unsigned int side, oside;
    int ret = 0;

    turn = ((signed char *)ctx)[0x575];
    side = ((TurnView954f8 *)ctx)->records[turn - 1].side;
    oside = side ^ 1;
    switch (mode) {
    case 1: {
        short n = ((SideView954f8 *)ctx)->sides[oside].count;

        if (n != 0)
            break;
        ret = 1;
        break;
    }
    case 2: {
        int i;
        unsigned char *q;

        F0x8001ebf8(buf2);
        ret = 1;
        for (i = 0; i < ((SideView954f8 *)ctx)->sides[oside].count; i++) {
            short e = ((SideView954f8 *)ctx)->sides[oside].hand[i];

            ((cardgame_54f8_cb_t)buf2[11])((((ValuesView954f8 *)ctx)->values[e]) + 1);
            q = (unsigned char *)buf2[0];
            if (q[0] == 6 && q[3] == 0x10)
                goto reject954f8;
        }
        break;
    }
    case 3: {
        int i;
        unsigned char *q;

        F0x8001ebf8(buf3);
        ret = 1;
        for (i = 0; i < ((SideView954f8 *)ctx)->sides[oside].count; i++) {
            short e = ((SideView954f8 *)ctx)->sides[oside].hand[i];

            ((cardgame_54f8_cb_t)buf3[11])((((ValuesView954f8 *)ctx)->values[e]) + 1);
            q = (unsigned char *)buf3[0];
            if (q[0] != 5)
                goto reject954f8;
        }
        break;
    }
    case 4: {
        short n = ((SideView954f8 *)ctx)->sides[side].check2;

        if (n != 0)
            break;
        ret = 1;
        break;
    }
    case 5: {
        short n = ((SideView954f8 *)ctx)->sides[side].check4;

        if (n != 0)
            break;
        ret = 1;
        break;
    }
    case 6: {
        short n = ((SideView954f8 *)ctx)->sides[oside].check4;

        if (n != 0)
            break;
        ret = 1;
        break;
    }
    case 7: {
        int c;
        int off;

        F0x8001ebf8(buf7);
        c = ((SideView954f8 *)ctx)->sides[side].cursor;

        ret = 1;
        if (c < 0x28) {
            off = c * 2 + side * 200;
        loop7:
            {
                short e = *(short *)((char *)ctx + off + 0x5b0);

                ((cardgame_54f8_cb_t)buf7[11])((((ValuesView954f8 *)ctx)->values[e]) + 1);
                c++;
                if (((unsigned char *)buf7[0])[3] == 0x10)
                    goto reject954f8;
                off += 2;
            }
            if (c < 0x28) goto loop7;
        }
        break;
    }
    case 8: {
        int c;
        int off;

        F0x8001ebf8(buf8);
        c = ((SideView954f8 *)ctx)->sides[side].cursor;

        ret = 1;
        if (c < 0x28) {
            off = c * 2 + side * 200;
        loop8:
            {
                short e = *(short *)((char *)ctx + off + 0x5b0);

                ((cardgame_54f8_cb_t)buf8[11])((((ValuesView954f8 *)ctx)->values[e]) + 1);
                c++;
                if (((unsigned char *)buf8[0])[3] != 0x10)
                    goto reject954f8;
                off += 2;
            }
            if (c < 0x28) goto loop8;
        }
        break;
    }
    case 9: {
        int i;
        int off2;
        int off1;
        unsigned char *base;
        unsigned char *selected;

        ret = 1;
        i = 0;
        off2 = 0x74c;
        off1 = 0x72e;
        turn = ((signed char *)ctx)[0x575];
        base = (unsigned char *)ctx + (turn - 1) * 8;
        do {
            ((FlagsView954f8 *)ctx)->flags[i] = 0;
            if (i < 6) {
                if (i >= ((unsigned char *)ctx)[0x72c])
                    goto cont9;
                selected = (unsigned char *)ctx + off1;
            } else {
                if (i - 6 >= ((unsigned char *)ctx)[0x79e])
                    goto cont9;
                selected = (unsigned char *)ctx + off2;
            }
            if (selected[0xc] == base[0x586])
                goto reject954f8;
        cont9:
            off2 += 0xe;
            i++;
            off1 += 0xe;
        } while (i < 12);
        break;
    reject954f8:
        ret = 0;
        goto done954f8;
    }
    case 10: {
        int i;
        ret = 1;
        for (i = 0; ; ) {
            unsigned char sel;
            int h;
            int r;

            if (ret != 1)
                goto done954f8;
            if (i < 6) {
                if (i >= ((unsigned char *)ctx)[0x72c])
                    goto cont10;
            } else {
                if (i - 6 >= ((unsigned char *)ctx)[0x79e])
                    goto cont10;
            }
            turn = ((signed char *)ctx)[0x575];
            sel = ((TurnView954f8 *)ctx)->records[turn - 1].selector;
            switch (sel) {
            case 1:
                if (side == 0) {
                    if (i < 6)
                        ret = 0;
                } else if (i >= 6) {
                    ret = 0;
                }
                break;
            case 2:
                if (side == 0) {
                    if (i > 5)
                        ret = 0;
                } else if (i <= 5) {
                    ret = 0;
                }
                break;
            case 3:
                ret = 0;
                break;
            case 4:
                if (i < 6)
                    h = ((FirstRows954f8 *)ctx)->rows[i].value;
                else
                    h = ((SecondRows954f8 *)ctx)->rows[i - 6].value;
                r = ((cardgame_54f8_cardfn_t)*(void **)((char *)card + 0xf40))(card, h);
                if (r != 1)
                    ret = 0;
                break;
            case 5:
                if (i < 6)
                    h = ((FirstRows954f8 *)ctx)->rows[i].value;
                else
                    h = ((SecondRows954f8 *)ctx)->rows[i - 6].value;
                r = ((cardgame_54f8_cardfn_t)*(void **)((char *)card + 0xf40))(card, h);
                if (r != 2)
                    ret = 0;
                break;
            case 6:
                if (i < 6)
                    h = ((FirstRows954f8 *)ctx)->rows[i].value;
                else
                    h = ((SecondRows954f8 *)ctx)->rows[i - 6].value;
                r = ((cardgame_54f8_cardfn_t)*(void **)((char *)card + 0xf40))(card, h);
                if (r == 3)
                    ret = 0;
                break;
            case 7:
                if (i < 6)
                    h = ((FirstRows954f8 *)ctx)->rows[i].value;
                else
                    h = ((SecondRows954f8 *)ctx)->rows[i - 6].value;
                r = ((cardgame_54f8_cardfn_t)*(void **)((char *)card + 0xf40))(card, h);
                if (r != 4)
                    ret = 0;
                break;
            case 8:
                if (i < 6)
                    h = ((FirstRows954f8 *)ctx)->rows[i].value;
                else
                    h = ((SecondRows954f8 *)ctx)->rows[i - 6].value;
                r = ((cardgame_54f8_cardfn_t)*(void **)((char *)card + 0xf40))(card, h);
                if (r == 6)
                    ret = 0;
                break;
            default:
                break;
            }
        cont10:
            i++;
            if (i >= 12)
                goto done954f8;
        }
    }
    case 11: {
        int i;
        ret = 1;
        for (i = 0; i < ((SideView954f8 *)ctx)->sides[side].count; i++) {
            turn = ((signed char *)ctx)[0x575];
            if (((TurnView954f8 *)ctx)->records[turn - 1].argument ==
                ((SideView954f8 *)ctx)->sides[side].hand[i])
                goto reject954f8;
        }
        break;
    }
    default:
        break;
    }
 done954f8:
    return ret;
}
