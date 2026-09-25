// CARDGAME:0x80092198 (672 bytes), jump table at 0x8008359c (20 bytes).
// psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), --rodata 0x8008359c:
// exact_byte_match 672/672 (r11 o55).
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