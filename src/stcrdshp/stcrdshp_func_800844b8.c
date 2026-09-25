#include "common/types.h"

/* STCRDSHP:0x800844b8..0x800852cc, PAL-SLES-03936 (3604 bytes).
 * Recovered from Ghidra ddw3-pal-sles-03936/STCRDSHP at verified base
 * 0x80082cb0 and cross-checked against the complete PAL binary range.
 * The switch table at 0x80082cb0 has 61 entries (244 bytes).
 * See submissions/stcrdshp-800844b8/strategy-r7-astra.md for exact evidence.
 */

extern int32_t STCRDSHP_func_800832c8(void *p, void *q, int32_t mode);
extern int32_t STCRDSHP_func_800835b8(void *p, void *q, int32_t mode);
extern int32_t STCRDSHP_func_80083780(void *p, void *q, int32_t mode);
extern void STCRDSHP_func_800843c0(void *p);
extern int32_t STCRDSHP_func_80085fdc(void *a, void *b);

/* RAM-resident callback slots (loaded with lw, invoked with jalr). */
extern void *D_8008CB88;
extern void *D_8008CB8C;
/* Overlay callback page base (0x8008CB80; slots D_8008CB88/D_8008CB8C at
 * +8/+12). PAL keeps this page in a saved reg per multi-use case block
 * (e.g. s0 at 0x80084580: lui 0x8009/addiu -0x3480). */
extern char O_8008CB80[];

/* EXE helper page base (0x8004B7D0; slots E_8004BBC4/BBC8/BBCC/BBD8 at
 * +0x3f4/+0x3f8/+0x3fc/+0x408). PAL keeps this page in a saved reg per
 * case block (e.g. s1 at 0x80084644: lui 0x8005/addiu -0x4830). */
extern void *X_8004B7D0[];
extern char M_80044B38[];
extern void *R_8004DC04[];

/* EXE input/message helpers reachable through fixed RAM slots. */
extern void *E_80055C48;

/* Fixed tables/pools. */
extern int32_t T_8008C708[];
extern uint8_t B_80048D34[];
extern int32_t P_8005CCA8;

/* Seven-word group: key followed by six choice-list pointers. */
typedef struct {
    int32_t key;
    void *items[6];
} CardGroup;

/* Partial layout only; the full object size/card capacity is not established. */
typedef struct {
    char _unknown00[0x70];
    int32_t selected[6];
    char _unknown88[0x10];
    int16_t cards[0x100];
} ShopCards;
typedef void (*cb_w2_t)(void *, int32_t);
typedef int32_t (*cb_poll0_t)(int32_t);
typedef int32_t (*cb_status_t)(void *);
typedef int32_t (*cb_poll1_t)(int32_t, int32_t);
typedef uint32_t (*cb_rnd_t)(void);

void STCRDSHP_func_800844b8(char *P, char *Q)
{
    int32_t state = *(int32_t *)(P + 0x10);
    /* Shared scalar scratch: horizontal coordinate or unscaled page index. */
    int32_t x;
    int32_t row;
    void *obj;

#define K4E(ex,slot,k) (key = ((cb_poll0_t)(ex)[(slot)/4])(0) >> ((cb_poll1_t)(ex)[0x102])(0,(k)), key &= 1)
    switch (state) {
    case 0:
    default:
        ((cb_w2_t)D_8008CB88)(P + 0x6f0, 1);
        goto advance_state;
    case 1: {
        char *cb = O_8008CB80;
        if (((cb_status_t)*(void **)(cb + 12))(P + 0x6f0) == 0)
            return;
        STCRDSHP_func_800832c8(P, Q, 1);
        ((cb_w2_t)*(void **)(cb + 8))(P + 0x700, 1);
        ((cb_w2_t)*(void **)(cb + 8))(P + 0x710, 1);
        goto advance_state;
    }
    case 2: {
        int32_t cursor;
        char *cb = O_8008CB80;
        ((cb_status_t)*(void **)(cb + 12))(P + 0x710);
        if (((cb_status_t)*(void **)(cb + 12))(P + 0x700) == 0)
            return;
        STCRDSHP_func_800835b8(P, Q, 1);
        cursor = *(int32_t *)(P + 0x5c);
        obj = *(void **)(Q + 0x20);
        ((void (*)(void *, int32_t, int32_t))*(void **)((char *)obj + 0x84))(
            obj, ((cursor % 2) * 0x83 + 0x1d), (((cursor % 8) / 2) * 0xe + 0x39));
        obj = *(void **)(Q + 0x20);
        ((cb_w2_t)*(void **)((char *)obj + 0x80))(obj, 1);
        goto advance_state;
    }
    case 3:
        goto advance_state;
    case 4: {
        int32_t cursor, cnt;
        int32_t key;
        int32_t newcursor;
        uint32_t lo;
        uint32_t hi;
        row = *(int32_t *)(P + 0x60);

        if (!K4E(X_8004B7D0,0x3f8,11) && K4E(X_8004B7D0,0x3f4,10)) {
            *(int32_t *)(P + 0x60) -= 1;
            if (*(int32_t *)(P + 0x60) < 0) *(int32_t *)(P + 0x60) = 0;
        } else if (!K4E(X_8004B7D0,0x3f8,10) && K4E(X_8004B7D0,0x3f4,11)) {
            *(int32_t *)(P + 0x60) += 1;
            cnt = *(int32_t *)(P + 0x64) - 1;
            if (*(int32_t *)(P + 0x60) > cnt) *(int32_t *)(P + 0x60) = cnt;
        }
        if (row != *(int32_t *)(P + 0x60)) {
            int32_t scaled_row;
            ((void (*)(uint32_t))E_80055C48)(0x8004513e);
            *(int32_t *)(P + 0x5c) = *(int32_t *)(P + 0x60) * 8;
            scaled_row = *(int32_t *)(P + 0x5c);
            obj = *(void **)(Q + 0x20);
            ((void (*)(void *, int32_t, int32_t))*(void **)((char *)obj + 0x84))(
                obj, (x = scaled_row, x = ((x % 2) * 0x83 + 0x1d)), (((scaled_row % 8) / 2) * 0xe + 0x39));
            STCRDSHP_func_800832c8(P, Q, 1);
            STCRDSHP_func_800835b8(P, Q, 1);
            return;
        }
        lo = (uint32_t)((uint32_t)row << 3) | 7u;
        cursor = *(int32_t *)(P + 0x5c);
        hi = (uint32_t)(*(int32_t *)(P + 0x3c0) - 1);
        /* Separate assignments retain the PAL lower-bound value lifetime. */
        row <<= 1;
        row <<= 2;
        if ((int32_t)hi < (int32_t)lo)
            lo = hi;
        if (K4E(X_8004B7D0,0x3f4,4) || K4E(X_8004B7D0,0x3fc,4)) {
            *(int32_t *)(P + 0x5c) -= 2;
            if (*(int32_t *)(P + 0x5c) < row) *(int32_t *)(P + 0x5c) = row;
        } else if (K4E(X_8004B7D0,0x3f4,6) || K4E(X_8004B7D0,0x3fc,6)) {
            *(int32_t *)(P + 0x5c) += 2;
            if (*(int32_t *)(P + 0x5c) > (int32_t)lo) *(int32_t *)(P + 0x5c) = (int32_t)lo;
        }
        if (K4E(X_8004B7D0,0x3f4,7) || K4E(X_8004B7D0,0x3fc,7)) {
            *(int32_t *)(P + 0x5c) -= 1;
            if (*(int32_t *)(P + 0x5c) < row) *(int32_t *)(P + 0x5c) = row;
        } else if (K4E(X_8004B7D0,0x3f4,5) || K4E(X_8004B7D0,0x3fc,5)) {
            *(int32_t *)(P + 0x5c) += 1;
            if (*(int32_t *)(P + 0x5c) > (int32_t)lo) *(int32_t *)(P + 0x5c) = (int32_t)lo;
        }
        if (cursor != *(int32_t *)(P + 0x5c)) {
            ((void (*)(uint32_t))E_80055C48)(0x8004513e);
            newcursor = *(int32_t *)(P + 0x5c);
            obj = *(void **)(Q + 0x20);
            ((void (*)(void *, int32_t, int32_t))*(void **)((char *)obj + 0x84))(
                obj, ((newcursor % 2) * 0x83 + 0x1d), (((newcursor % 8) / 2) * 0xe + 0x39));
            STCRDSHP_func_800835b8(P, Q, 1);
            return;
        }
        if (K4E(X_8004B7D0,0x3f4,13) != 0) {
            ((void (*)(uint32_t))E_80055C48)(0x8004503c);
            *(int32_t *)(P + 0x10) = 10;
            return;
        }
        if (K4E(X_8004B7D0,0x3f4,14) == 0)
            return;
        ((void (*)(uint32_t))E_80055C48)(0x800450bd);
        *(int32_t *)(P + 0x10) = 0x32;
        *(int32_t *)(P + 0x14) = 0;
        return;
    }
    case 10: {
        uint32_t r;
        char *msg;
        char *cb = O_8008CB80;
        ((cb_w2_t)*(void **)(cb + 8))(P + 0x6f0, 0);
        STCRDSHP_func_800832c8(P, Q, 0);
        ((cb_w2_t)*(void **)(cb + 8))(P + 0x700, 0);
        STCRDSHP_func_800835b8(P, Q, 0);
        obj = *(void **)(Q + 0x20);
        ((cb_w2_t)*(void **)((char *)obj + 0x80))(obj, 0);
        msg = M_80044B38;
        r = ((uint32_t (*)(int32_t))*(void **)(msg + 0x414))(P_8005CCA8 + 0x32);
        obj = *(void **)(Q + 0x30);
        ((void (*)(void *, uint32_t, int32_t))*(void **)((char *)obj + 0x114))(obj, r, 0x10);
        r = ((uint32_t (*)(int32_t))*(void **)(msg + 0x414))(P_8005CCA8 + 0x32);
        obj = *(void **)(Q + 0x34);
        ((void (*)(void *, uint32_t, int32_t))*(void **)((char *)obj + 0x114))(obj, r, 4);
        goto advance_state;
    }
    case 11: {
        int32_t card, tmp;
        char *pool;
        int32_t idx;
        int32_t n;
        int32_t r;
        int32_t offset;
        int32_t *dst;
        void *choices;
        char *cb = O_8008CB80;

        ((cb_status_t)*(void **)(cb + 12))(P + 0x6f0);
        if (((cb_status_t)*(void **)(cb + 12))(P + 0x700) == 0)
            return;
        card = ((ShopCards *)P)->cards[*(int32_t *)(P + 0x5c)];
        if (*(void **)(Q + 0x74) != 0) {
            *(int32_t *)(*(char **)(Q + 0x74) + 0xc) = 3;
            return;
        }
        idx = 0;
        for (n = 0; T_8008C708[n * 7] != 0; n++) {
            if (T_8008C708[n * 7] == card) idx = n;
        }
        n = 0;
        offset = 0;
        /* The destination advances before notification; the table/count after it. */
        do {
            dst = (int32_t *)P + offset;
            r = ((cb_rnd_t)R_8004DC04[2])();
            tmp = r % 16;
            choices = ((CardGroup *)((char *)T_8008C708 + idx * 28 + n * 4))->items[0];
            tmp = *(int32_t *)((char *)choices + tmp * 4);
            dst[28] = tmp;
            offset++;
            pool = (char *)B_80048D34;
            ((void (*)(int32_t,int32_t))*(void **)(pool + 0x2720))(tmp, 1);
            n++;
        } while (n < 6);
        ((uint8_t *)pool)[card + 0x7c]--;
        tmp = STCRDSHP_func_80085fdc(*(void **)(P + 0x50), P + 0x70);
        *(void **)(Q + 0x74) = (void *)tmp;
        *(int32_t *)(P + 0x88) = 0;
        ((cb_w2_t)D_8008CB88)(P + 0x720, 1);
        goto advance_state;
    }
    case 12:
        if (((cb_status_t)D_8008CB8C)(P + 0x720) == 0)
            return;
        STCRDSHP_func_80083780(P, Q, 1);
        if (*(int32_t *)(*(char **)(Q + 0x74) + 0xc) != 1)
            return;
        *(int32_t *)(P + 0x8c) = 1;
        *(int32_t *)(P + 0x10) += 1;
        return;
    case 13: {
        int32_t key;
        row = *(int32_t *)(P + 0x88);

        if (K4E(X_8004B7D0,0x3f4,7) || K4E(X_8004B7D0,0x3fc,7)) {
            *(int32_t *)(P + 0x88) -= 1;
            if (*(int32_t *)(P + 0x88) < 0) *(int32_t *)(P + 0x88) = 0;
        } else if (K4E(X_8004B7D0,0x3f4,5) || K4E(X_8004B7D0,0x3fc,5)) {
            *(int32_t *)(P + 0x88) += 1;
            if (*(int32_t *)(P + 0x88) > 5) *(int32_t *)(P + 0x88) = 5;
        }
        if (row != *(int32_t *)(P + 0x88)) {
            ((void (*)(uint32_t))E_80055C48)(0x4001b);
            STCRDSHP_func_80083780(P, Q, 1);
            return;
        }
        if (K4E(X_8004B7D0,0x3f4,14) == 0)
            return;
        ((void (*)(uint32_t))E_80055C48)(0x800450bd);
        *(int32_t *)(P + 0x10) = 0x32;
        *(int32_t *)(P + 0x8c) = 0;
        *(int32_t *)(P + 0x14) = 1;
        return;
    }
    case 50:
        ((cb_w2_t)D_8008CB88)(P + 0x710, 0);
        obj = *(void **)(Q + 0x30);
        ((cb_w2_t)*(void **)((char *)obj + 0x144))(obj, 0);
        obj = *(void **)(Q + 0x34);
        ((cb_w2_t)*(void **)((char *)obj + 0x144))(obj, 0);
        if (*(int32_t *)(P + 0x14) == 0) {
            *(int32_t *)(P + 0x10) = 0x37;
            return;
        }
        goto advance_state;
    case 51: {
        char *cb = O_8008CB80;
        if (((cb_status_t)*(void **)(cb + 12))(P + 0x710) == 0)
            return;
        ((cb_w2_t)*(void **)(cb + 8))(P + 0x720, 0);
        STCRDSHP_func_80083780(P, Q, 0);
        obj = *(void **)(Q + 0x74);
        ((void (*)(void *))*(void **)((char *)obj + 0xc0))(obj);
        goto advance_state;
    }
    case 52: {
        int32_t card, cnt;
        int32_t lim;

        if (((cb_status_t)D_8008CB8C)(P + 0x720) == 0) {
            if (*(void **)(Q + 0x74) != 0)
                return;
        }
        *(int32_t *)(P + 0x10) = 0;
        x = *(int32_t *)(P + 0x60);
        lim = (x << 3) | 7;
        row = x << 3;
        card = ((ShopCards *)P)->cards[*(int32_t *)(P + 0x5c)];
        STCRDSHP_func_800843c0(P);
        cnt = *(int32_t *)(P + 0x3c0) - 1;
        if (cnt < lim)
            lim = cnt;
        if ((int8_t)B_80048D34[card + 0x7c] > 0)
            return;
        card = ((ShopCards *)P)->cards[*(int32_t *)(P + 0x5c)];
        if ((int8_t)B_80048D34[card + 0x7c] > 0)
            return;
        *(int32_t *)(P + 0x5c) -= 1;
        if (row <= *(int32_t *)(P + 0x5c))
            return;
        *(int32_t *)(P + 0x60) -= 1;
        if (*(int32_t *)(P + 0x60) >= 0) {
            *(uint32_t *)(P + 0x5c) = (uint32_t)lim;
            return;
        }
        *(int32_t *)(P + 0xc) = 3;
        return;
    }
    case 55: {
        char *cb = O_8008CB80;
        if (((cb_status_t)*(void **)(cb + 12))(P + 0x710) == 0)
            return;
        ((cb_w2_t)*(void **)(cb + 8))(P + 0x6f0, 0);
        STCRDSHP_func_800832c8(P, Q, 0);
        ((cb_w2_t)*(void **)(cb + 8))(P + 0x700, 0);
        STCRDSHP_func_800835b8(P, Q, 0);
        obj = *(void **)(Q + 0x20);
        ((cb_w2_t)*(void **)((char *)obj + 0x80))(obj, 0);
advance_state:
        *(int32_t *)(P + 0x10) += 1;
        return;
    }
    case 56: {
        char *cb = O_8008CB80;
        ((cb_status_t)*(void **)(cb + 12))(P + 0x6f0);
        if (((cb_status_t)*(void **)(cb + 12))(P + 0x700) == 0)
            return;
        *(int32_t *)(P + 0x10) = 0x3c;
        return;
    }
    case 60:
        *(int32_t *)(P + 0xc) = 3;
        return;
    }
}

#undef K4E
