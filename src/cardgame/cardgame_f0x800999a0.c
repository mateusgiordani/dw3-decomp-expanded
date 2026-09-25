// CARDGAME:0x800999a0 (size 580, 0x244)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x16cf0
// Prologue 27bdff28 addiu sp,-0xd8; saves fp/s7/s1/s4/ra/s6/s5/s3/s2/s0 at 0xd0/0xcc/0xb4/0xc0/0xd4/0xc8/0xc4/0xbc/0xb8/0xb0(sp);
//   fp=a0 (byte base for stride-84 table), s7=a1 (slot-pointer table), s1=a2 (index), s4=a3 (status).
// Body: s2/s3/s5/s6 = signed table pair from (DAT_800a5aa8 + DAT_8005ccb0*16), half selected by (s1!=0)
//   (+0/+2/+8/+0xa when s1==0, +4/+6/+0xc/+0xe when s1!=0); stride-84 row = fp+s1*84 with int16 pair at +0x6c/+0x6e.
// If s4[8]!=0: jal EXE 0x8001f648(buf sp+0x10) then callbacks (sp+0x8c)(0x100,1); (sp+0x84)(0x280,0);
//   (sp+0xa4)(row6c+s2,row6e+s3); (sp+0x9c)(s4[0],0x1000,0x1000); v=(*0x80044f5c)(0x25d0002);
//   (sp+0x94)(v, s1?0x43:0x44, row6c+s2, row6e+s3).
// If s4[8]==2: obj=*(s7+s1*4+0x10); (*(obj+0x134))(obj,(int16)(row6c+s5),(int16)(row6e+s6));
//   v=(*0x80044f4c)(DAT_8005cca8+15); obj=*(s7+s1*4+0x10); (*(obj+0x114))(obj,v,0x3f);
// else: obj=*(s7+s1*4+0x10); (*(obj+0x144))(obj,0). Epilogue restores, jr ra, sp+0xd8; next fn 0x80099be4.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only disasm/decompile/xref: name CARDGAME_F0x800999a0
//   already present; decompile hypothesis cross-checked field-by-field against independent PAL LE capstone decode
//   (145 words); Ghidra DB never mutated. Upstream asm/dw2003/pro/cardgame.s is a raw .word dump used as GUIDE only.
// Caller: CARDGAME 0x80099cf4 (in fn at 0x80099be4) jal 0x800999a0, PAL file-off 0x17044 (word 0x0c026668),
//   confirmed independently in PAL bytes and in upstream guide label .L0x00017044.
// Callee: direct EXE 0x8001f648; indirect cb slots sp+0x84/0x8c/0x94/0x9c/0xa4, *0x80044f5c, *0x80044f4c,
//   *(obj+0x114/0x134/0x144). Data: 0x8005ccb0, 0x800a5aa8, 0x8005cca8.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (variant base).
// Status: exact_byte_match, 580 bytes, sha256 7f525113a119b05289560493dc0e2685c9f4dcb97f973f3402fe50867006b449.
// O delay do bne (f8==2) e sllv v0,s1,v0: o shift idx<<2 fica dentro do braco
// tomado, com v0 ainda vivo contendo 2. O else recomputa idx<<2 como ponteiro+off
// (addu com a base primeiro). No 2.7.2-cygnus o mesmo texto dobra o count para sll imediato.
#include "common/types.h"

extern void func_0x8001f648(void *buf);
extern uint32_t (*DAT_80044f5c)(uint32_t);
extern uint32_t (*DAT_80044f4c)(uint32_t);
extern int32_t DAT_8005ccb0;
extern int32_t DAT_8005cca8;
extern int16_t DAT_800a5aa8[1];

typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(uint32_t a0, int32_t a1, int32_t a2, int32_t a3);

typedef struct {
    uint8_t pad_00[0x74];
    cardgame_cb2_t f84;
    uint8_t pad_78[4];
    cardgame_cb2_t f8c;
    uint8_t pad_80[4];
    cardgame_cb4_t f94;
    uint8_t pad_88[4];
    cardgame_cb3_t f9c;
    uint8_t pad_90[4];
    cardgame_cb2_t fa4;
    uint8_t pad_98[8];
} cardgame_99a0_cb_t;

typedef struct {
    int16_t f0;
    uint8_t pad_02[6];
    uint8_t f8;
} cardgame_99a0_st_t;

typedef struct {
    uint8_t pad_00[0x114];
    void (*m114)();
    uint8_t pad_118[0x1c];
    void (*m134)();
    uint8_t pad_138[0xc];
    void (*m144)();
} cardgame_99a0_obj_t;

/* slots: array of 4-byte units; object pointer is unit [idx + 4] (lw +0x10 after s7+idx*4). */
typedef cardgame_99a0_obj_t cardgame_99a0_slot_t;

void CARDGAME_F0x800999a0(int32_t p0, int32_t p1, int32_t p2, cardgame_99a0_st_t *st)
{
    uint8_t cbb[0xa0];
    int16_t *tab;
    int32_t s2, s3, s5, s6;
    uint8_t *work;
    cardgame_99a0_slot_t **slots;
    int32_t idx;
    uint32_t v;
    int32_t a1;
    cardgame_99a0_obj_t *obj;
    cardgame_99a0_st_t *status;

    work = (uint8_t *)p0;
    slots = (cardgame_99a0_slot_t **)p1;
    idx = p2;
    status = st;
    if (idx == 0) {
        int32_t base;
        int32_t off;

        base = (int32_t)DAT_800a5aa8;
        off = DAT_8005ccb0 << 4;
        tab = (int16_t *)(off + base);
        s2 = tab[0];
        s3 = tab[1];
        s5 = tab[4];
        s6 = tab[5];
    } else {
        int32_t base;
        int32_t off;

        base = (int32_t)DAT_800a5aa8;
        off = DAT_8005ccb0 << 4;
        tab = (int16_t *)(off + base);
        s2 = tab[2];
        s3 = tab[3];
        s5 = tab[6];
        s6 = tab[7];
    }
    if (status->f8 != 0) {
        uint8_t *row;

        func_0x8001f648(cbb);
        ((cardgame_cb2_t)*(void **)(cbb + 0x7c))(0x100, 1);
        ((cardgame_cb2_t)*(void **)(cbb + 0x74))(0x280, 0);
        row = work + idx * 84;
        ((cardgame_cb2_t)*(void **)(cbb + 0x94))(*(int16_t *)(row + 0x6c) + s2, *(int16_t *)(row + 0x6e) + s3);
        ((cardgame_cb3_t)*(void **)(cbb + 0x8c))(status->f0, 0x1000, 0x1000);
        v = DAT_80044f5c(0x25d0002u);
        a1 = 0x43;
        if (idx == 0)
            a1 = 0x44;
        ((cardgame_cb4_t)*(void **)(cbb + 0x84))(v, a1, *(int16_t *)(row + 0x6c) + s2, *(int16_t *)(row + 0x6e) + s3);
    }
    {
        int t;

        if (status->f8 == 2) {
            uint8_t *row;
            uint8_t *slotb;

            t = idx << 2;
            slotb = (uint8_t *)slots + t;
            t = t + idx;
            t = t << 2;
            t = t + idx;
            t = t << 2;
            row = work + t;
            obj = *(cardgame_99a0_obj_t **)(slotb + 0x10);
            obj->m134(obj, (int16_t)(*(uint16_t *)(row + 0x6c) + s5), (int16_t)(*(uint16_t *)(row + 0x6e) + s6));
            v = DAT_80044f4c((uint32_t)(DAT_8005cca8 + 15));
            obj = *(cardgame_99a0_obj_t **)(slotb + 0x10);
            obj->m114(obj, v, 0x3f);
        } else {
            int off;

            off = idx << 2;
            obj = *(cardgame_99a0_obj_t **)((uint8_t *)slots + off + 0x10);
            obj->m144(obj, 0);
        }
    }
}

