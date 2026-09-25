/*
 * CARDGAME:0x8008e5bc CARDGAME_F0x8008e5bc
 * 1672 bytes at CARDGAME.PRO offset 0xb90c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008e5bc
 *  Symbols     DAT_8004DF9C=0x8004df9c DAT_8005CCB0=0x8005ccb0
 *              DAT_800A5958=0x800a5958 DAT_800A5DA8=0x800a5da8
 *              DAT_800A5DC0=0x800a5dc0 DAT_800A5DC2=0x800a5dc2
 *  Compare     1672 bytes from 0x8008e5bc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008e5bc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Dispatch on *(p1+0x422): 2 = countdown via EXE slot *(0x8004df9c); 0 = return
 * 0; 1 = pair census + draws via slot *(p2+0xf08); 3 = record resolve + marks
 * via slot *(p2+0xf18); 4 = return 1.
 *
 * Caller: CARDGAME_F0x80084320 via 0x80085708 (jal 0c02396f).
 */

#include <stdint.h>
typedef struct { int16_t words[7]; } Tail14;

typedef void (*cardgame_draw_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_mark_t)(int32_t, int32_t);

typedef struct { uint8_t first, second; } PairE5bc;
typedef struct { PairE5bc rows[2][6]; } PairTableE5bc;
extern PairTableE5bc DAT_800A5DA8;
extern uint8_t DAT_800A5DC0[2];
extern uint8_t DAT_800A5DC2[2];
extern int32_t DAT_800A5958[][4];
extern int32_t DAT_8005CCB0;
extern int32_t DAT_8004DF9C;

typedef struct { int32_t words[19]; } Record76;

typedef struct {
    uint8_t prefix[0x46f];
    int8_t flags[6][2];
} FlagViewE5bc;

typedef struct {
    uint8_t count;
    uint8_t unknown1;
    Tail14 records[6];
    uint8_t unknown56[0x1c];
} HandRowE5bc;
typedef struct {
    uint8_t prefix[0x72c];
    HandRowE5bc hands[2];
} HandViewE5bc;
typedef char HandStrideE5bc[(sizeof(HandRowE5bc) == 0x72) ? 1 : -1];

typedef struct {
    uint8_t prefix[0x108];
    Record76 records[12];
} RecordViewE5bc;

typedef struct {
    uint8_t prefix[0x120];
    int16_t markers[12][38];
} MarkViewE5bc;

typedef struct {
    uint8_t prefix[0x72e];
    Tail14 value;
} TailAccessE5bc;

static __inline__ int32_t flag_half_index_e5bc(int32_t player) {
    return player * 3;
}

int32_t CARDGAME_F0x8008e5bc(int32_t p1, int32_t p2)
{
    uint8_t state;
    int32_t result;
    int32_t outer;
    int32_t base6;
    int32_t base12;
    int32_t count;
    int32_t i;
    int32_t j;
    int8_t *pc;
    uint8_t *pairs;
    uint8_t tmp;
    Record76 swap;

    result = 0;
    state = *(uint8_t *)(p1 + 0x422);
    switch (state) {
    case 1:
        outer = 0;
        pairs = (uint8_t *)&DAT_800A5DA8;
        DAT_800A5DC2[0] = DAT_800A5DC2[1] = 0;
        DAT_800A5DC0[0] = DAT_800A5DC0[1] = 0;
        do {
            base12 = outer * 12;
            base6 = flag_half_index_e5bc(outer);
            pc = (int8_t *)&((FlagViewE5bc *)p1)->flags + base6 * 2;
            for (i = 0; i < ((HandViewE5bc *)p1)->hands[outer].count; i++) {
                if (pc[i] == 1)
                    DAT_800A5DC2[outer] += 1;
            }
            count = 0;
            for (i = 0; i < ((HandViewE5bc *)p1)->hands[outer].count - 1; i++) {
                if (pc[i] == 1) {
                    for (j = i + 1; j < ((HandViewE5bc *)p1)->hands[outer].count; j++) {
                        if (pc[j] == 0) {
                            tmp = (uint8_t)pc[i];
                            pc[i] = (uint8_t)pc[j];
                            pc[j] = tmp;
                            *(uint8_t *)(count * 2 + base12 + (int32_t)pairs) = (uint8_t)i;
                            *(uint8_t *)(count * 2 + base12 + (int32_t)pairs + 1) = (uint8_t)j;
                            count++;
                            break;
                        }
                    }
                }
            }
            DAT_800A5DC0[outer] = (uint8_t)count;
            if (outer == 0) {
                for (i = 0; i < count; i++) {
                    ((cardgame_draw_t)*(void **)(p2 + 0xf08))(
                        p2, pairs[i * 2 + 1], 10,
                        DAT_800A5958[DAT_8005CCB0][0] + pairs[i * 2] * 0x2900,
                        DAT_800A5958[DAT_8005CCB0][1]);
                }
            } else {
                for (i = 0; i < count; i++) {
                    ((cardgame_draw_t)*(void **)(p2 + 0xf08))(
                        p2, pairs[i * 2 + 0xd] + 6, 10,
                        DAT_800A5958[DAT_8005CCB0][2] + pairs[i * 2 + 0xc] * 0x2900,
                        DAT_800A5958[DAT_8005CCB0][3]);
                }
            }
            outer++;
        } while (outer < 2);
        if ((uint32_t)DAT_800A5DC2[0] + (uint32_t)DAT_800A5DC2[1] != 0) {
            *(uint8_t *)(p1 + 0x422) = 2;
            *(int32_t *)(p1 + 0x424) = 20;
            break;
        }
        *(uint8_t *)(p1 + 0x422) = 4;
        break;
    case 2: {
        int32_t elapsed;
        int32_t remaining;
        elapsed = ((int32_t (*)(void))DAT_8004DF9C)();
        remaining = *(int32_t *)(p1 + 0x424) - elapsed;
        *(int32_t *)(p1 + 0x424) = remaining;
        if (remaining <= 0)
            *(uint8_t *)(p1 + 0x422) = 3;
        break;
    }
    case 3: {
        int32_t outer3, i3;
        outer3 = 0;
        do {
            for (i3 = 0; i3 < DAT_800A5DC0[outer3]; i3++) {
                    int32_t first, second;
                    TailAccessE5bc *to, *from;
                    first = DAT_800A5DA8.rows[outer3][i3].first;
                    second = DAT_800A5DA8.rows[outer3][i3].second;
                    to = (TailAccessE5bc *)(p1 + (first * 14 + outer3 * 0x72));
                    from = (TailAccessE5bc *)(p1 + (second * 14 + outer3 * 0x72));
                    first += outer3 * 6;
                    second += outer3 * 6;
                    to->value = from->value;
                    swap = ((RecordViewE5bc *)p2)->records[first];
                    ((RecordViewE5bc *)p2)->records[first] = ((RecordViewE5bc *)p2)->records[second];
                    ((RecordViewE5bc *)p2)->records[second] = swap;
            }
            i3 = 0;
            ((HandViewE5bc *)p1)->hands[outer3].count -= DAT_800A5DC2[outer3];
            do {
                if ((int32_t)(uint32_t)((HandViewE5bc *)p1)->hands[outer3].count <= i3) {
                    ((cardgame_mark_t)*(void **)(p2 + 0xf18))(p2, (outer3 * 6) + i3);
                    ((MarkViewE5bc *)p2)->markers[(outer3 * 6) + i3][0] = 0;
                }
                i3++;
            } while (i3 < 6);
            outer3++;
        } while (outer3 < 2);
        *(uint8_t *)(p1 + 0x422) = 4;
        break;
    }
    case 4:
        result = 1;
        break;
    }
    return result;
}
