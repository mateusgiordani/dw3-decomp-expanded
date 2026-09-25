/* STCRDDEK:0x80086e04, PAL-SLES-03936; 2436-byte body, no padding.
 * Raw overlay base 0x80082cb0; offset 0x4154; next entry 0x80087788.
 * The 34-entry jump table is at 0x80082db0 (136 bytes).
 * Read-only Ghidra disassembly and caller 0x80088608 were checked against PAL.
 * Exact with PsyQ GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79, -O2 -G0 (base).
 * Evidence: docs/c-matching-guide/submissions/stcrddek-80086e04/strategy-r7-astra.md.
 * Offset names retain uncertainty about the complete work/callback layouts.
 */
#include "common/types.h"

typedef void (*stcrddek_cb1_t)(int32_t a0);
typedef void (*stcrddek_cb2_t)(int32_t a0, int32_t a1);
typedef void (*stcrddek_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*stcrddek_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*stcrddek_svc_t)(uint32_t arg);
typedef int32_t (*stcrddek_tick_t)(void);

void EXE_F0x8001f648(void *buf);
extern stcrddek_svc_t STCRDDEK_DAT_80044f5c;
extern uint8_t D_8004de10[];
extern uint8_t D_80044b38[];
extern int32_t STCRDDEK_DAT_8005cca8;
extern int32_t STCRDDEK_DAT_8008b848;
extern int32_t STCRDDEK_DAT_8008b850;
extern int32_t STCRDDEK_DAT_8008b7f4;
extern int32_t STCRDDEK_DAT_8008b800;
extern int32_t STCRDDEK_DAT_8008b80c;
extern int32_t STCRDDEK_TBL_8008b7dc[];
extern int32_t STCRDDEK_TBL_8008b6fc[];
extern int32_t STCRDDEK_TBL_8008b818[];

typedef struct {
    uint8_t pad_00[0x50];
    int32_t f50; /* +0x50 */
    int32_t f54; /* +0x54 */
    int32_t f58; /* +0x58 */
    int32_t f5c; /* +0x5c */
    int32_t f60; /* +0x60 */
    int32_t f64; /* +0x64 */
    int32_t f68; /* +0x68 */
    int32_t f6c; /* +0x6c */
    int32_t f70; /* +0x70 */
    int32_t f74; /* +0x74 */
    uint8_t pad_78[0x18]; /* +0x78..+0x8f */
    int32_t f90; /* +0x90 */
    uint8_t pad_94[4]; /* +0x94 (never accessed) */
    int32_t f98; /* +0x98 */
    int32_t f9c; /* +0x9c */
    int32_t fa0; /* +0xa0 */
    int32_t fa4; /* +0xa4 */
    int32_t fa8; /* +0xa8 */
    int32_t fac; /* +0xac */
    int32_t fb0; /* +0xb0 */
    int32_t fb4; /* +0xb4 */
    uint8_t pad_b8[0x1c]; /* +0xb8..+0xd3 */
    int32_t fd4; /* +0xd4 */
    uint8_t pad_d8[0x0c]; /* +0xd8..+0xe3 */
    int32_t fe4; /* +0xe4 */
} stcrddek_6e04_work_t;

typedef struct {
    uint8_t pad_00[0x74];
    stcrddek_cb2_t f84; /* +0x74 -> sp+0x84 */
    int32_t d78; /* +0x78 paired data */
    stcrddek_cb2_t f8c; /* +0x7c -> sp+0x8c */
    int32_t d80; /* +0x80 paired data */
    stcrddek_cb4_t f94; /* +0x84 -> sp+0x94 */
    stcrddek_cb1_t f98; /* +0x88 -> sp+0x98 */
    stcrddek_cb3_t f9c; /* +0x8c -> sp+0x9c */
    int32_t d90; /* +0x90 paired data */
    stcrddek_cb2_t fa4; /* +0x94 -> sp+0xa4 */
    uint8_t pad_98[8]; /* +0x98..+0x9f */
} stcrddek_6e04_cb_t;

/* Keep the actual horizontal components in this inline scope. */
static inline int32_t horizontal_6e04(int32_t sx)
{
    int32_t delta = sx * 14;
    int32_t bias = (sx / 5) * 8 + 0x2f;
    return delta + bias;
}

void STCRDDEK_func_80086e04(stcrddek_6e04_work_t *work)
{
    stcrddek_6e04_cb_t cb;
    int32_t now;
    int32_t svc;
    int32_t a1;
    int32_t a2;
    int32_t a3;
    int32_t idx;
    int32_t mode;
    int32_t x;
    int32_t y;
    uint8_t *svcBase1, *svcBase2, *svcBase3, *svcBase4;

    EXE_F0x8001f648(cb.pad_00);
    cb.f84(work->f5c, work->f60);
    cb.f8c(work->f54, work->f58);
    if (work->fd4 == 0)
        goto tail;
    if (work->fa8 != 0) {
        {
            uint8_t *tickBase = D_8004de10;
            now = (*(stcrddek_tick_t *)(tickBase + 0x188))();
            if (now - work->fa4 > 4) {
                work->fa4 = (*(stcrddek_tick_t *)(tickBase + 0x188))();
                work->fa0 += 1;
                if (work->fa0 > 3)
                    work->fa0 = 0;
            }
        }
        cb.f98(work->fa0);
        x = work->f98;
        if (x < 10)
            goto svc_path;
        idx = 0;
        y = work->f9c;
        if (y >= 3)
            goto scan_begin;
svc_path:
        {
            int32_t sx, sy;
            svc = STCRDDEK_DAT_80044f5c(0x07700000u);
            sx = work->f98;
            sy = work->f9c;
            a1 = 0x27;
            /* The scan column is dead on this service path. Reuse its storage
             * for the real 8*y and 9*y intermediates of the vertical coordinate. */
            x = sy * 8;
            x += sy;
            a3 = x * 2 + 0x5a;
            a2 = horizontal_6e04(sx);
            cb.f94(svc, a1, a2, a3);
            goto after_dispatch;
        }
scan_begin:
        while (*(int8_t *)(work->fac * 210 + STCRDDEK_DAT_8008b850
                          + (work->f9c * 30 + (x + idx) * 2)) != 1) {
            idx--;
        }
        switch (work->f98 + idx + work->f9c * 15) {
        /* Explicit default destinations preserve the verified dense table. */
        default:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x4b:
        case 0x4c:
        case 0x4d:
        case 0x4e:
        case 0x4f:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5a:
        case 0x5b:
        case 0x5c:
        case 0x5d:
        case 0x5e:
        case 0x5f:
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x66:
            mode = 0;
            break;
        case 0x65:
            mode = 1;
            break;
        case 0x46:
            mode = 2;
            STCRDDEK_DAT_8008b7f4 = STCRDDEK_DAT_8005cca8 + 0x3c;
            break;
        case 0x55:
            mode = 3;
            STCRDDEK_DAT_8008b800 = STCRDDEK_DAT_8005cca8 + 0x44;
            break;
        case 0x67:
            mode = 4;
            STCRDDEK_DAT_8008b80c = STCRDDEK_DAT_8005cca8 + 0x4c;
            break;
        }
        svc = STCRDDEK_DAT_80044f5c(0x07700000u);
        {
            int32_t *base = STCRDDEK_TBL_8008b7dc;
            int32_t *row = (int32_t *)((int32_t)base + mode * 12);
            cb.f94(svc, row[0], row[1], row[2]);
        }
after_dispatch:
        svc = STCRDDEK_DAT_80044f5c(0x07700000u);
        cb.f94(svc, 0x28, work->f90 * 19 + 0x4b, 0x40);
        cb.f98(0);
    }
    if (work->fd4 != 0x1000) {
        cb.f9c(work->fd4, 0x1000, 0x1000);
        if (work->fd4 != 0x1000)
            cb.fa4(0x18, 0x20);
    }
    svcBase1 = D_80044b38;
    svc = (*(stcrddek_svc_t *)(svcBase1 + 0x424))(0x07700000u);
    cb.f94(svc, 0x1d, 0x18, 0x15);
    if (work->f50 != 2) {
        if (work->fd4 != 0x1000)
            cb.fa4(0x20, 0x3f);
        if (work->f64 != -1) {
            {
                uint8_t *tickBase = D_8004de10;
                now = (*(stcrddek_tick_t *)(tickBase + 0x188))();
                if (now - work->f6c > 12) {
                    work->f6c = (*(stcrddek_tick_t *)(tickBase + 0x188))();
                    a1 = work->f68 + 1;
                    work->f68 = a1;
                    if (a1 < 7) {
                        if (STCRDDEK_TBL_8008b6fc[work->f64 * 7 + a1] != -1)
                            goto timer2_done;
                    }
                    work->f68 = 0;
timer2_done:;
                }
            }
            svc = STCRDDEK_DAT_80044f5c(0x07700000u);
            cb.f94(svc, STCRDDEK_TBL_8008b6fc[work->f64 * 7 + work->f68], 0x22, 0x30);
        } else {
            svc = (*(stcrddek_svc_t *)(svcBase1 + 0x424))(0x07700000u);
            cb.f94(svc, 0x36, 0x20, 0x2e);
        }
        {
            uint8_t *tickBase = D_8004de10;
            now = (*(stcrddek_tick_t *)(tickBase + 0x188))();
            if (now - work->f74 > 4) {
                work->f74 = (*(stcrddek_tick_t *)(tickBase + 0x188))();
                work->f70 += 1;
                if (work->f70 > 13)
                    work->f70 = 0;
            }
        }
        cb.f8c(work->f54, work->f58 - 1);
        cb.f98(work->f70);
        svcBase2 = D_80044b38;
        svc = (*(stcrddek_svc_t *)(svcBase2 + 0x424))(0x07700000u);
        cb.f94(svc, 0x1f, 0x20, 0x2e);
        cb.f98(0);
        cb.f8c(work->f54, work->f58);
        svc = (*(stcrddek_svc_t *)(svcBase2 + 0x424))(0x07700000u);
        cb.f94(svc, 0x1e, 0x20, 0x2e);
    }
    if (work->fd4 != 0x1000)
        cb.fa4(0x20, 0x49);
    if (work->f50 != 2) {
        if (STCRDDEK_DAT_8005cca8 == 0) {
            svc = STCRDDEK_DAT_80044f5c(0x07700000u);
            cb.f94(svc, 0x34, 0x4b, 0x40);
        } else {
            svc = STCRDDEK_DAT_80044f5c(0x07700000u);
            cb.f94(svc, 0x37, 0x4b, 0x40);
        }
    } else {
        svc = STCRDDEK_DAT_80044f5c(0x07700000u);
        cb.f94(svc, 0x35, 0x4b, 0x40);
    }
    if (work->fd4 != 0x1000)
        cb.f9c(0x1000, work->fd4, 0x1000);
    if (STCRDDEK_DAT_8008b848 > 1) {
        {
            uint8_t *tickBase = D_8004de10;
            now = (*(stcrddek_tick_t *)(tickBase + 0x188))();
            if (now - work->fb4 > 6) {
                work->fb4 = (*(stcrddek_tick_t *)(tickBase + 0x188))();
                work->fb0 += 1;
                if (work->fb0 > 5)
                    work->fb0 = 0;
            }
        }
        cb.f98(STCRDDEK_TBL_8008b818[work->fb0]);
        svcBase3 = D_80044b38;
        svc = (*(stcrddek_svc_t *)(svcBase3 + 0x424))(0x07700000u);
        cb.f94(svc, 0x32, 0x0a, 0x5e);
        svc = (*(stcrddek_svc_t *)(svcBase3 + 0x424))(0x07700000u);
        cb.f94(svc, 0x33, 0x0119, 0x5e);
        cb.f98(0);
    }
    cb.f98(4);
    svcBase4 = D_80044b38;
    svc = (*(stcrddek_svc_t *)(svcBase4 + 0x424))(0x07700000u);
    cb.f94(svc, 0x2c, 0xcb, 0xc3);
    svc = (*(stcrddek_svc_t *)(svcBase4 + 0x424))(0x07700000u);
    cb.f94(svc, 0x2d, 0xde, 0xc3);
    svc = (*(stcrddek_svc_t *)(svcBase4 + 0x424))(0x07700000u);
    cb.f94(svc, STCRDDEK_DAT_8005cca8 + 0x3c, 0xcb, 0x99);
    svc = (*(stcrddek_svc_t *)(svcBase4 + 0x424))(0x07700000u);
    cb.f94(svc, STCRDDEK_DAT_8005cca8 + 0x44, 0xcb, 0xae);
    svc = (*(stcrddek_svc_t *)(svcBase4 + 0x424))(0x07700000u);
    cb.f94(svc, STCRDDEK_DAT_8005cca8 + 0x4c, 0xf6, 0xc3);
    cb.f98(0);
    svc = (*(stcrddek_svc_t *)(svcBase4 + 0x424))(0x07700000u);
    cb.f94(svc, 0x25, 0x1d, 0x54);
tail:
    cb.f8c(work->f54, work->f58 - 1);
    if (work->fe4 != 0) {
        if (work->fe4 != 0x1000) {
            cb.f9c(0x1000, work->fe4, 0x1000);
            cb.fa4(0, 0x78);
        }
        svc = STCRDDEK_DAT_80044f5c(0x07700000u);
        cb.f94(svc, 0x26, 0, 100);
    }
}
