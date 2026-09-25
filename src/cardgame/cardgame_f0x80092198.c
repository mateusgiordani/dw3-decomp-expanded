/*
 * CARDGAME:0x80092198 CARDGAME_F0x80092198
 * 672 bytes at CARDGAME.PRO offset 0xf4e8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80092198, jump table (.rodata) at 0x8008359c
 *  Symbols     DAT_8004DF9C=0x8004df9c DAT_8005CCB0=0x8005ccb0
 *              DAT_800A5958=0x800a5958
 *  Compare     672 bytes from 0x80092198 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80092198
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * jump table at 0x8008359c (20 bytes).
 */

#include <stdint.h>

extern int32_t DAT_8005CCB0;

typedef struct {
    int32_t w0;
    int32_t w1;
    int32_t w2;
    int32_t w3;
} cardgame_5958_ent_t;

extern cardgame_5958_ent_t DAT_800A5958[];
extern int32_t (*DAT_8004DF9C)(void);

typedef void (*cardgame_f24_t)(void *obj, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
typedef void (*cardgame_f1c_t)(void *obj, int32_t a1);
typedef void (*cardgame_f08_t)(void *obj, int32_t a1, int32_t a2, int32_t a3, int32_t a4);

int32_t CARDGAME_F0x80092198(uint8_t *st, uint8_t *obj, int32_t idx)
{
    int32_t slot;
    int32_t tbl_b;
    int32_t a3v;
    int32_t off;
    uint8_t *pb;
    int32_t ret = 0;

    off = idx * 0x72 + 0x72c;
    pb = st + off;

    slot = (int32_t)(*pb);
    /* Adjustment in its own if before the selection: jump threading gives the
       PAL beq to the idx == 0 arm and j to the else arm with slot += 6 in the
       delay slot. Each arm then builds a3v whole (table word + (x * 41) << 8). */
    if (idx != 0)
        slot = slot + 6;
    if (idx == 0) {
        a3v = DAT_800A5958[DAT_8005CCB0].w0 + ((slot * 41) << 8);
    } else {
        a3v = DAT_800A5958[DAT_8005CCB0].w2 + (((int32_t)(*pb) * 41) << 8);
    }
    if (idx == 0) {
        tbl_b = DAT_800A5958[DAT_8005CCB0].w1;
    } else {
        tbl_b = DAT_800A5958[DAT_8005CCB0].w3;
    }

    switch (*(st + 0x422)) {
    case 1: {
        int32_t v = DAT_8004DF9C();
        v = *(int32_t *)(st + 0x424) - v;
        *(int32_t *)(st + 0x424) = v;
        if (v < 1) {
            ((cardgame_f24_t)*(void **)(obj + 0xf24))(obj, slot, 5, 0x1000, 0x1000);
            *(st + 0x422) = 2;
        }
        break;
    }
    case 2: {
        uint8_t *s = obj + slot * 0x4c;
        if (*(s + 0x14a) == 1) {
            ((cardgame_f1c_t)*(void **)(obj + 0xf1c))(obj, slot);
            *(st + 0x422) = 3;
        }
        break;
    }
    case 3: {
        uint8_t *s = obj + slot * 0x4c;
        if (*(s + 0x14a) == 1) {
            ((cardgame_f08_t)*(void **)(obj + 0xf08))(obj, slot, 0x14, a3v, tbl_b);
            *(int16_t *)(s + 0x12e) = 1;
            *(st + 0x422) = 4;
        }
        break;
    }
    case 4: {
        uint8_t *s = obj + slot * 0x4c;
        if (*(s + 0x14a) == 1) {
            *(st + 0x422) = 5;
            *(int16_t *)(s + 0x12e) = 0;
            *pb = (uint8_t)(*pb + 1);
        }
        break;
    }
    case 5:
        ret = 1;
        break;
    default:
        break;
    }
    return ret;
}