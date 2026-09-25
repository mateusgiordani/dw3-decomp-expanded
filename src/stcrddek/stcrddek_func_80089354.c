#include "common/types.h"

/* STCRDDEK:0x80089354 - deck-menu candidate state machine (2884 B, 0xB44).
 * PAL base 0x80082cb0; file off 0x66a4; body sha 5fda69eead9fc985d39e77c8986e4839f614a3efda969570b740bda8d10dcac4.
 * Boundary: [0x80089354, 0x80089e98); tail jr ra at 0x80089e90, next prologue at 0x80089e98.
 * Switch on *(p1 + 0x10) via table at 0x80083058, bound 0x66; default target 0x800893a8.
 * Caller: STCRDDEK_func_80089e98 (+0xd4 jal at 0x80089f6c, a0=s0, a1=s1).
 * Ghidra program STCRDDEK read-only; no mutation; reference words match (see handoff).
 * Every case stores its own next state and returns; case 0x65 falls through with its store, so
 * jump2 redirects the other stores to that one and keeps the per-case loads (see strategy-r6). */

extern void STCRDDEK_func_8008924c(int32_t p1);
extern void STCRDDEK_func_80088d24(int32_t p1, uint32_t *p2, int32_t a2, int32_t a3);
extern int32_t STCRDDEK_func_80083594(void);
extern uint32_t STCRDDEK_func_8008660c(int32_t p1, int32_t idx);
extern uint32_t STCRDDEK_func_800887d4(int32_t addr);

/* 0x8008B830: fade/window helper table (PAL builds the base with lui/addiu). */
typedef struct {
    int32_t unk0;
    int32_t unk4;
    void (*start)(int32_t a0, int32_t a1);  /* +0x8 (0x8008B838) */
    int32_t (*done)(int32_t a0);            /* +0xC (0x8008B83C) */
} Stcrddek8008B830;

/* 0x8004B7D0: EXE input helper table. */
typedef struct {
    uint8_t unk0[0x3F4];
    int32_t (*held)(int32_t a0);                /* +0x3F4 (0x8004BBC4) */
    int32_t unk3F8;
    int32_t (*pressed)(int32_t a0);             /* +0x3FC (0x8004BBCC) */
    int32_t unk400;
    int32_t unk404;
    uint32_t (*button)(int32_t a0, int32_t a1); /* +0x408 (0x8004BBD8) */
} Exe8004B7D0;

/* 0x80044B38: EXE text/string helper table. */
typedef struct {
    uint8_t unk0[0x414];
    uint32_t (*text)(int32_t a0); /* +0x414 (0x80044F4C) */
} Exe80044B38;

extern Stcrddek8008B830 D_8008B830;
extern Exe8004B7D0 D_8004B7D0;
extern Exe80044B38 D_80044B38;
extern int32_t D_8005CCA8;
extern uint8_t D_8004935C[]; /* deck table, 0x66-byte records */
extern void (*D_80055C48)(int32_t a0);

void STCRDDEK_func_80089354(int32_t p1, uint32_t *p2) {
    switch (*(int32_t *)(p1 + 0x10)) {
    case 0:
    default:
        STCRDDEK_func_8008924c(p1);
        D_8008B830.start(p1 + 0xBC, 1);
        D_8008B830.start(p1 + 0xEC, 1);
        *(int32_t *)(p1 + 100) = 0;
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 1: {
        uint32_t r3;
        D_8008B830.done(p1 + 0xBC);
        if (D_8008B830.done(p1 + 0xEC) == 0) {
            return;
        }
        r3 = D_80044B38.text(D_8005CCA8 + 0x32);
        ((void (*)(uint32_t, uint32_t, int32_t))*(uint32_t *)(p2[1] + 0x114))(p2[1], r3, 0x19);
        STCRDDEK_func_80088d24(p1, p2, 0, 1);
        D_8008B830.start(p1 + 0xFC, 1);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    }
    case 2:
        if (D_8008B830.done(p1 + 0xFC) == 0) {
            return;
        }
        STCRDDEK_func_80088d24(p1, p2, 1, 1);
        D_8008B830.start(p1 + 0x10C, 1);
        D_8008B830.start(p1 + 0xDC, 1);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 3:
        D_8008B830.done(p1 + 0xDC);
        if (D_8008B830.done(p1 + 0x10C) == 0) {
            return;
        }
        STCRDDEK_func_80088d24(p1, p2, 2, 1);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 4: {
        int32_t old;
        int32_t v;
        old = *(int32_t *)(p1 + 0x60);
        if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0, 4)) & 1U) != 0
            || ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0, 4)) & 1U) != 0) {
            v = *(int32_t *)(p1 + 0x60) - 1;
            *(int32_t *)(p1 + 0x60) = v;
            if (v < 0) {
                *(int32_t *)(p1 + 0x60) = 0;
            }
        } else if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0, 6)) & 1U) != 0
                   || ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0, 6)) & 1U) != 0) {
            v = *(int32_t *)(p1 + 0x60) + 1;
            *(int32_t *)(p1 + 0x60) = v;
            if (2 < v) {
                *(int32_t *)(p1 + 0x60) = 2;
            }
        }
        if (old != *(int32_t *)(p1 + 0x60)) {
            D_80055C48(0x4001B);
            return;
        }
        if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0, 0xD)) & 1U) != 0) {
            D_80055C48(0x4001C);
            *(int32_t *)(p1 + 0x10) = 10;
            return;
        }
        if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0, 0xE)) & 1U) == 0) {
            return;
        }
        D_80055C48((int32_t)0x800450BD);
        ((void (*)(int32_t, int32_t))*(uint32_t *)(p1 + 0x2C))(p1, 100);
        return;
    }
    case 10:
        *(int32_t *)(p1 + 100) = 1;
        ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[1] + 0x144))(p2[1], 0);
        D_8008B830.start(p1 + 0xBC, 0);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 0xB:
        if (D_8008B830.done(p1 + 0xBC) == 0) {
            return;
        }
        ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[1] + 0x144))(p2[1], 0);
        D_8008B830.start(p1 + 0xCC, 1);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 0xC: {
        uint32_t r3;
        if (D_8008B830.done(p1 + 0xCC) == 0) {
            return;
        }
        *(int32_t *)(p1 + 0x70) = 0;
        ((void (*)(uint32_t, int32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x84))(p2[0x19], 0x9A, 0x23);
        ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x80))(p2[0x19], 1);
        r3 = D_80044B38.text(D_8005CCA8 + 0x32);
        ((void (*)(uint32_t, uint32_t, int32_t))*(uint32_t *)(p2[0x17] + 0x114))(p2[0x17], r3, 0x1A);
        r3 = D_80044B38.text(D_8005CCA8 + 0x32);
        ((void (*)(uint32_t, uint32_t, int32_t))*(uint32_t *)(p2[0x18] + 0x114))(p2[0x18], r3, 0x1B);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    }
    case 0xD: {
        int32_t old;
        old = *(int32_t *)(p1 + 0x70);
        if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0, 4)) & 1U) != 0) {
            *(int32_t *)(p1 + 0x70) = 0;
        } else if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0, 6)) & 1U) != 0) {
            *(int32_t *)(p1 + 0x70) = 1;
        }
        if (old != *(int32_t *)(p1 + 0x70)) {
            D_80055C48((int32_t)0x8004513E);
            ((void (*)(uint32_t, int32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x84))(p2[0x19], 0x9A, *(int32_t *)(p1 + 0x70) * 0xE + 0x23);
            return;
        }
        if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0, 0xD)) & 1U) != 0) {
            D_80055C48((int32_t)0x8004503C);
            ((void (*)(int32_t, int32_t))*(uint32_t *)(p1 + 0x2C))(p1, 0x32);
            *(int32_t *)(p1 + 0x14) = 1;
            return;
        }
        if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0, 0xE)) & 1U) == 0) {
            return;
        }
        D_80055C48((int32_t)0x800450BD);
        *(int32_t *)(p1 + 0x10) = 0xF;
        return;
    }
    case 0xF:
        ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x80))(p2[0x19], 0);
        ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x17] + 0x144))(p2[0x17], 0);
        ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x18] + 0x144))(p2[0x18], 0);
        D_8008B830.start(p1 + 0xCC, 0);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 0x10:
        if (D_8008B830.done(p1 + 0xCC) == 0) {
            return;
        }
        D_8008B830.start(p1 + 0xBC, 1);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 0x11: {
        uint32_t r3;
        if (D_8008B830.done(p1 + 0xBC) == 0) {
            return;
        }
        r3 = D_80044B38.text(D_8005CCA8 + 0x32);
        ((void (*)(uint32_t, uint32_t, int32_t))*(uint32_t *)(p2[1] + 0x114))(p2[1], r3, 0x19);
        *(int32_t *)(p1 + 100) = 0;
        ((void (*)(int32_t, int32_t))*(uint32_t *)(p1 + 0x2C))(p1, 4);
        return;
    }
    case 0x32:
        if (*(int32_t *)(p1 + 0x14) != 0) {
            ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x94))(p2[0x19], 1);
            ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x88))(p2[0x19], 7);
        }
        D_8008B830.start(p1 + 0xDC, 0);
        D_8008B830.start(p1 + 0x10C, 0);
        STCRDDEK_func_80088d24(p1, p2, 2, 0);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 0x33:
        D_8008B830.done(p1 + 0xDC);
        if (D_8008B830.done(p1 + 0x10C) == 0) {
            return;
        }
        D_8008B830.start(p1 + 0xFC, 0);
        STCRDDEK_func_80088d24(p1, p2, 1, 0);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 0x34:
        if (D_8008B830.done(p1 + 0xFC) == 0) {
            return;
        }
        if (*(int32_t *)(p1 + 0x14) != 0) {
            ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x94))(p2[0x19], 0);
            ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x88))(p2[0x19], 0);
            ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x19] + 0x80))(p2[0x19], 0);
            ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x17] + 0x144))(p2[0x17], 0);
            ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[0x18] + 0x144))(p2[0x18], 0);
            D_8008B830.start(p1 + 0xCC, 0);
        } else {
            ((void (*)(uint32_t, int32_t))*(uint32_t *)(p2[1] + 0x144))(p2[1], 0);
            D_8008B830.start(p1 + 0xBC, 0);
        }
        D_8008B830.start(p1 + 0xEC, 0);
        STCRDDEK_func_80088d24(p1, p2, 0, 0);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 0x35:
        if (*(int32_t *)(p1 + 0x14) != 0) {
            D_8008B830.done(p1 + 0xCC);
        } else {
            D_8008B830.done(p1 + 0xBC);
        }
        if (D_8008B830.done(p1 + 0xEC) == 0) {
            return;
        }
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    case 0x36:
        if (*(int32_t *)(p1 + 0x14) != 0) {
            if (*(int32_t *)(p1 + 0x70) == 0) {
                *p2 = STCRDDEK_func_8008660c(p1, *(int32_t *)(p1 + 0x60));
            } else {
                *p2 = STCRDDEK_func_800887d4(*(int32_t *)(p1 + 0x60) * 0x66 + (int32_t)&D_8004935C);
            }
            ((void (*)(int32_t, int32_t))*(uint32_t *)(p1 + 0x28))(p1, 2);
            return;
        }
        *(int32_t *)(p1 + 0xC) = 3;
        return;
    case 100: {
        int32_t id;
        id = STCRDDEK_func_80083594();
        p2[0x1A] = (uint32_t)id;
        ((void (*)(int32_t, int32_t, int32_t))*(uint32_t *)(id + 100))(id, 0, 10);
        *(int32_t *)(p1 + 0x10) = *(int32_t *)(p1 + 0x10) + 1;
        return;
    }
    case 0x65:
        if (*(int32_t *)(p2[0x1A] + 0xC) != 2) {
            return;
        }
        *(int32_t *)(p1 + 0x10) = 0x36;
    }
}
