/*
 * CARDGAME:0x80098898 CARDGAME_F0x80098898
 * 2984 bytes at CARDGAME.PRO offset 0x15be8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80098898
 *  Symbols     CARDGAME_F0x80096a94=0x80096a94 DAT_80044f5c=0x80044f5c
 *              DAT_8004DF9C=0x8004df9c DAT_8005CCA8=0x8005cca8
 *              DAT_8005CCB0=0x8005ccb0 DAT_800A59D0=0x800a59d0
 *              DAT_800A5A68=0x800a5a68 DAT_800A5A70=0x800a5a70
 *              DAT_800A5A80=0x800a5a80 DAT_800A5A88=0x800a5a88
 *              DAT_800A5A90=0x800a5a90 DAT_800A5A98=0x800a5a98
 *              DAT_800A5AA0=0x800a5aa0 D_80044B38=0x80044b38
 *              func_0x8001f648=0x8001f648
 *  Compare     2984 bytes from 0x80098898 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80098898
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936, [0x80098898, 0x80099440).
 *
 * 2984 text bytes; no rodata. PsyQ GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79.
 *
 * Row records have verified stride 0x4c; the service slot is base + 0x424.
 *
 * Callback buffers are 0xa0 bytes and the numeric packet is 0x18 bytes.
 *
 * See the r9 Astra submission for PAL/Ghidra evidence and exact reruns.
 */

#include "common/types.h"

extern void func_0x8001f648(void *buf);
extern uint8_t D_80044B38[];
extern uint32_t (*DAT_80044f5c)(uint32_t);
extern int32_t (*DAT_8004DF9C)(void);
extern void CARDGAME_F0x80096a94(void *pkt, int32_t flag);

extern int32_t DAT_8005CCB0;
extern int32_t DAT_8005CCA8;
extern uint8_t DAT_800A5A68[];
extern int16_t DAT_800A59D0[];
extern uint8_t DAT_800A5A70[];
extern uint8_t DAT_800A5A80[];
extern uint8_t DAT_800A5A88[];
extern uint8_t DAT_800A5A90[];
extern uint8_t DAT_800A5A98[];
extern uint8_t DAT_800A5AA0[];

typedef struct { uint8_t pad[0x30]; uint8_t enabled[5]; } work_flags_t;
typedef struct { uint8_t pad[0x18]; uint8_t values[5]; } work_values_t;
typedef struct { uint8_t left, right; } selection_pair_t;
typedef void (*cb2_t)(int32_t a0, int32_t a1);
typedef void (*cb1_t)(int32_t a0);
typedef uint32_t (*cvec_t)(uint32_t a0);
typedef void (*cb4_t)(uint32_t a0, int32_t a1, int32_t a2, int32_t a3);

typedef struct {
    uint8_t pad[0x74];
    cb2_t f74;
    uint8_t pad2[4];
    cb2_t f7c;
    uint8_t pad3[4];
    cb4_t f84;
    cb1_t f88;
    cb2_t f8c;
    uint8_t pad6[0x10];
} cardgame_cb_t;

typedef struct {
    uint16_t value;
    uint16_t x;
    uint16_t y;
    uint8_t unknown06[8];
    uint8_t digits;
    uint8_t style;
    uint8_t enabled;
    uint8_t unknown11[7];
} cardgame_pkt_t;

void CARDGAME_F0x80098898(void *work, int32_t unused, int32_t row)
{
    cardgame_cb_t cbA;
    cardgame_cb_t cbB;
    cardgame_pkt_t pkt;
    cardgame_cb_t cbC;
    uint8_t *pb = (uint8_t *)work;
    int16_t ybase = *(int16_t *)(pb + 0x0E);
    int32_t yb = ybase;
    int32_t y_secondary = *(int16_t *)(pb + 0x3E);
    int32_t phase_b;
    int32_t row_address_or_phase;
    int32_t slot;
    int32_t x_offset;
    int32_t selector;
    int32_t frame_index;
    int32_t value_or_index;
    int32_t delta;
    int16_t *tab;
    uint32_t resource;

    (void)unused;
    if (DAT_8005CCB0 != 0) {
        if (row == 0) {
            yb += 12;
            y_secondary += 12;
        } else {
            yb -= 12;
            y_secondary -= 12;
        }
    }
    frame_index = *(int32_t *)(pb + 8) % 0x24;
    func_0x8001f648(&cbA);
    cbA.f7c(0x100, 1);
    cbA.f74(0x340, 0);
    selector = 0;
    x_offset = 0;
    cbA.f88(DAT_800A5A68[frame_index / 6]);
    { uint8_t *service=D_80044B38;
        int16_t *base = DAT_800A59D0;
        int16_t *tab = (int16_t *)(row * 76 + (int32_t)base);
        do {
            if (((work_flags_t *)pb)->enabled[selector] != 0) {
                resource = (*(cvec_t *)(service+0x424))(0x25D0003u);
                cbA.f84(resource, 13,
                (int32_t)*(int16_t *)(pb + 12) + (int32_t)tab[0x34 / 2] + x_offset,
                (int32_t)yb + (int32_t)tab[0x36 / 2]);
            }
            selector++;
            x_offset += 0x2A;
        } while (selector < 5);
    }
    if (pb[0x38] != 0) {
        int16_t *base, *tab;
        resource = DAT_80044f5c(0x25D0003u);
        base = DAT_800A59D0;
        tab = (int16_t *)(row * 76 + (int32_t)base);
        cbA.f84(resource, 12,
        (int32_t)*(int16_t *)(pb + 12) + (int32_t)tab[0x44 / 2],
        (int32_t)yb + (int32_t)tab[0x46 / 2]);
    }
    if (pb[0x39] != 0) {
        int16_t *base, *tab;
        resource = DAT_80044f5c(0x25D0003u);
        base = DAT_800A59D0;
        tab = (int16_t *)(row * 76 + (int32_t)base);
        cbA.f84(resource, 12,
        (int32_t)*(int16_t *)(pb + 12) + (int32_t)tab[0x48 / 2],
        (int32_t)yb + (int32_t)tab[0x4A / 2]);
    }
    if (pb[0x36] != 0) {
        int16_t *base, *tab;
        resource = DAT_80044f5c(0x25D0003u);
        base = DAT_800A59D0;
        tab = (int16_t *)(row * 76 + (int32_t)base);
        cbA.f84(resource, 14,
        (int32_t)*(int16_t *)(pb + 12) + (int32_t)tab[0x38 / 2],
        (int32_t)yb + (int32_t)tab[0x3A / 2]);
    }
    if (pb[0x37] != 0) {
        int16_t *base, *tab;
        resource = DAT_80044f5c(0x25D0003u);
        base = DAT_800A59D0;
        tab = (int16_t *)(row * 76 + (int32_t)base);
        cbA.f84(resource, 14,
        (int32_t)*(int16_t *)(pb + 12) + (int32_t)tab[0x3C / 2],
        (int32_t)yb + (int32_t)tab[0x3E / 2]);
    }
    if (pb[0x35] != 0) {
        int16_t *base, *tab;
        resource = DAT_80044f5c(0x25D0003u);
        base = DAT_800A59D0;
        tab = (int16_t *)(row * 76 + (int32_t)base);
        cbA.f84(resource, 15,
        (int32_t)*(int16_t *)(pb + 0x20) + (int32_t)tab[0x40 / 2],
        (int32_t)*(int16_t *)(pb + 0x22) + (int32_t)tab[0x42 / 2]);
    }
    phase_b = *(int32_t *)(pb + 8) % 0x18;
    func_0x8001f648(&cbB);
    cbB.f7c(0x100, 1);
    cbB.f74(0x340, 0);
    if (pb[0x38] != 0) cbB.f88(phase_b / 6 + 1);
    else cbB.f88(0);
    { int16_t *base,*tab;
        resource = DAT_80044f5c(0x25D0003u);
        base=DAT_800A59D0;
        tab=(int16_t *)(row*76+(int32_t)base);
        cbB.f84(resource, DAT_800A5A70[DAT_8005CCA8 * 2],
        (int32_t)*(int16_t *)(pb + 12) + (int32_t)tab[0x18 / 2],
        (int32_t)yb + (int32_t)tab[0x1A / 2]);
    }
    {
        if (pb[0x39] != 0) cbB.f88(phase_b / 6 + 1);
        else cbB.f88(0);
        { int16_t *base,*saved_row;
            resource = DAT_80044f5c(0x25D0003u);
            base=DAT_800A59D0;
            saved_row=(int16_t *)(row*76+(int32_t)base);
            cbB.f84(resource, ((selection_pair_t *)DAT_800A5A70)[DAT_8005CCA8].right,
            (int32_t)*(int16_t *)(pb + 12) + (int32_t)saved_row[0x20 / 2],
            (int32_t)yb + (int32_t)saved_row[0x22 / 2]);
            pkt.enabled = 1;
            pkt.x = *(int16_t *)(pb + 12) + saved_row[0x0C / 2];
            pkt.y = saved_row[0x0E / 2] + yb;
            pkt.digits = 2;
            pkt.style = 1;
            pkt.value = (uint16_t)pb[0x1D];
            CARDGAME_F0x80096a94(&pkt, 0);
            pkt.x = *(int16_t *)(pb + 12) + saved_row[0x10 / 2];
            pkt.y = saved_row[0x12 / 2] + yb;
            pkt.digits = 2;
            pkt.style = 1;
            pkt.value = (uint16_t)pb[0x1E];
            CARDGAME_F0x80096a94(&pkt, 0);
            pkt.x = *(int16_t *)(pb + 12) + saved_row[0x1C / 2];
            pkt.y = saved_row[0x1E / 2] + yb;
            pkt.digits = 3;
            pkt.style = 0;
            pkt.value = *(uint16_t *)(pb + 0x10);
            CARDGAME_F0x80096a94(&pkt, 0);
            pkt.x = *(int16_t *)(pb + 12) + saved_row[0x24 / 2];
            pkt.y = saved_row[0x26 / 2] + yb;
            pkt.digits = 3;
            pkt.style = 0;
            pkt.value = *(uint16_t *)(pb + 0x12);
            CARDGAME_F0x80096a94(&pkt, 0);
            /* Index-derived x coordinates preserve the PAL loop scheduling. */
            for(value_or_index=0;value_or_index<5;value_or_index++) {
                /* This scratch holds the actual row address until numeric drawing ends. */
                row_address_or_phase=row*76+(int32_t)DAT_800A59D0;
                pkt.x = *(int16_t *)(pb + 12) + ((int16_t *)row_address_or_phase)[0x08 / 2] + value_or_index*42;
                pkt.y = ((int16_t *)row_address_or_phase)[0x0A / 2] + yb;
                pkt.digits = 2;
                pkt.style = 1;
                pkt.value = (uint16_t)((work_values_t *)pb)->values[value_or_index];
                CARDGAME_F0x80096a94(&pkt, 0);
            }
        }
    }
    { int16_t *base=DAT_800A59D0; int16_t *saved_row=(int16_t *)(row*76+(int32_t)base);
        pkt.x = *(int16_t *)(pb + 0x20) + saved_row[0x14 / 2];
        pkt.y = *(int16_t *)(pb + 0x22) + saved_row[0x16 / 2];
        pkt.digits = 2;
        pkt.style = 1;
        pkt.value = *(uint16_t *)(pb + 0x28);
        CARDGAME_F0x80096a94(&pkt, 0);
        func_0x8001f648(&cbC);
        cbC.f7c(0x100, 1);
        cbC.f74(0x280, 0);
        value_or_index = DAT_80044f5c(0x25D0002u);
        cbC.f84(value_or_index, (uint32_t)((uint8_t *)saved_row)[0x02] + (uint32_t)*(int32_t *)(pb + 0x44),
        (int32_t)*(int16_t *)(pb + 0x3C), y_secondary);
    }
    { int16_t *base,*tab;
        { int32_t remainder=*(int32_t *)(pb+8)%0x1e;
            selector=pb[0x37] ? remainder/6 : 0; }
        value_or_index = DAT_80044f5c(0x25D0002u);
        { uint8_t *selected=DAT_800A5A80+selector;
            base=DAT_800A59D0; tab=(int16_t *)(row*76+(int32_t)base);
            cbC.f84(value_or_index, (uint32_t)*selected + 9,
            (int32_t)*(int16_t *)(pb + 12) + (int32_t)tab[0x2C / 2],
            (int32_t)yb + (int32_t)tab[0x2E / 2]);
    }}
    { int16_t *base,*tab;
        { int32_t remainder=*(int32_t *)(pb+8)%0x1e;
            selector=pb[0x36] ? remainder/6 : 0; }
        value_or_index = DAT_80044f5c(0x25D0002u);
        { uint8_t *selected=DAT_800A5A88+selector;
            base=DAT_800A59D0; tab=(int16_t *)(row*76+(int32_t)base);
            cbC.f84(value_or_index, (uint32_t)*selected + 6,
            (int32_t)*(int16_t *)(pb + 12) + (int32_t)tab[0x28 / 2],
            (int32_t)yb + (int32_t)tab[0x2A / 2]);
    }}
    { int32_t remainder=*(int32_t *)(pb+8)%0x1e;
        frame_index=pb[0x35] ? remainder/6 : 0; }
    { uint8_t *service=D_80044B38;
        value_or_index=(*(cvec_t *)(service+0x424))(0x25D0002u);
        cbC.f84(value_or_index, (uint32_t)DAT_800A5A90[frame_index] + 0x31,
        (int32_t)*(int16_t *)(pb + 0x20), (int32_t)*(int16_t *)(pb + 0x22));
    }
    {
        int32_t selection, row_offset;
        uint8_t *service;
        int16_t *base;
        /* The same scratch now holds the captured signed animation remainder. */
        row_address_or_phase=*(int32_t *)(pb+8)%0x1e;
        slot=0;
        do {
            selection=((work_flags_t *)pb)->enabled[slot] ? row_address_or_phase/6 : 0;
            { uint8_t *base_service=D_80044B38;
                resource=(*(cvec_t *)(base_service+0x424))(0x25D0002u);
                { uint8_t *first=DAT_800A5A98+slot;
                    uint8_t *second=DAT_800A5AA0+selection;
                    int16_t *base=DAT_800A59D0;
                    int16_t *row_ptr;
                    row_offset=row*76;
                    row_ptr=(int16_t *)(row_offset+(int32_t)base);
                    cbC.f84(resource, (uint32_t)*first + (uint32_t)*second,
                    (int32_t)*(int16_t *)(pb+12)+(int32_t)row_ptr[2]+slot*42,
                    yb+(int32_t)row_ptr[3]);
                }
            }
            slot++;
        } while(slot<5);
        service=D_80044B38;
        resource=(*(cvec_t *)(service+0x424))(0x25D0002u);
        base=DAT_800A59D0;
        tab=(int16_t *)(row_offset+(int32_t)base);
        cbC.f84(resource, (uint32_t)((uint8_t *)tab)[3],
        (int32_t)*(int16_t *)(pb+0x20)+(int32_t)tab[0x30/2],
        (int32_t)*(int16_t *)(pb+0x22)+(int32_t)tab[0x32/2]);
        resource=(*(cvec_t *)(service+0x424))(0x25D0002u);
        cbC.f84(resource, (uint32_t)((uint8_t *)tab)[1], (int32_t)*(int16_t *)(pb+12),yb);
    }
    delta = DAT_8004DF9C();
    *(int32_t *)(pb + 8) = (*(int32_t *)(pb + 8) + delta) & 0xFFFF;
}
