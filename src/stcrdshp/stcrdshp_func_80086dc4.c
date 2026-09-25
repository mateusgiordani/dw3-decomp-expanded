/* STCRDSHP:0x80086dc4..0x80087a2c, PAL-SLES-03936, 3176 bytes.
 * The reviewed full CFG exceeds Ghidra's 184-byte automatic function.
 * Table: 52 entries at 0x80082da4. All price-vector loads use 0x80044f4c.
 * Polls and callback receivers retain local lifetimes. Successful paths own
 * their real state increment; compiler suffix merging reproduces PAL.
 * Money is at global base+0x6c; card flag at base+card+0x3a2. The final address
 * first takes the card index, then adds the base. See r7 Astra evidence. */
#include "common/types.h"

extern void STCRDSHP_func_8008627c(void *ws, void *ctx, int32_t flag);
extern void F0x8001ebf8(void *buf);

extern uint8_t D_8008CB80[];
extern void *D_8008CB88;
extern void *D_8008CB8C;
extern void *D_8008CB9C;

extern void *D_80044F4C;

extern uint8_t D_80044B38[];

extern uint8_t D_8004B7D0[];
extern void *D_80055C48;
extern int32_t D_8005CCA8;
extern uint8_t D_80048D34[];
typedef struct { uint8_t unknown[0x6c]; int32_t money; } money_view;

typedef struct {
    uint8_t unk[0x2C];
    void (*cb)(int32_t);
    uint8_t tail[0x24];
} STCRDSHP_1ebf8_out;

#define POLL(base,off,k) ({ int32_t _b=((int32_t (*)(int32_t))*(void **)((base)+(off)))(0); int32_t _s=((int32_t (*)(int32_t,int32_t))*(void **)((base)+0x408))(0,k); (_b >> _s)&1; })

void STCRDSHP_func_80086dc4(void *a0, void *a1) {
    uint8_t *ws = (uint8_t *)a0;
    uint8_t *ctx = (uint8_t *)a1;
    uint8_t *obj;
    int16_t *table;
    int32_t cur;
    int32_t idx;
    int32_t card;
    int32_t price;
    int32_t st;
    int32_t i;
    uint8_t *p;
    STCRDSHP_1ebf8_out tmp;

    obj = *(uint8_t **)(ctx + 0x48);
    if (*(int32_t *)(obj + 0x0C) != 1) {
        return;
    }

    switch (*(int32_t *)(ws + 0x10)) {
    case 0:
    default:
        ((void (*)(void *, int32_t))D_8008CB88)(ws + 0xAC, 1);
        (*(int32_t *)(ws + 0x10))++;
        return;
    case 1:
        if (((int32_t (*)(void *))D_8008CB8C)(ws + 0xAC) == 0) {
            return;
        }
        *(int32_t *)(ws + 0x60) = 1;
        STCRDSHP_func_8008627c(ws, ctx, 1);
        if (*(int32_t *)(ws + 0x70) >= 2) {
            if (*(int32_t *)(ws + 0x6C) < *(int32_t *)(ws + 0x70) - 1) {
                price = ((int32_t (*)(int32_t))D_80044F4C)(D_8005CCA8 + 0x32);
                { uint8_t *obj = *(uint8_t **)(ctx + 0x34);
                ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x114)))(obj, price, 0x0B);
                }
            } else {
                { uint8_t *obj = *(uint8_t **)(ctx + 0x34);
                ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
                }
            }
        }
        (*(int32_t *)(ws + 0x10))++;
        return;
    case 2:
        cur = *(int32_t *)(ws + 0x5C);
        {
            uint8_t *padbase=D_8004B7D0;
            if (POLL(padbase,0x3F4,7) || POLL(padbase,0x3FC,7)) {
                idx=*(int32_t *)(ws+0x5C)-1;
                *(int32_t *)(ws+0x5C)=idx;
                if(idx<0) *(int32_t *)(ws+0x5C)=0;
            } else if (POLL(padbase,0x3F4,5) || POLL(padbase,0x3FC,5)) {
                idx=*(int32_t *)(ws+0x5C)+1;
                *(int32_t *)(ws+0x5C)=idx;
                if(idx>=6) *(int32_t *)(ws+0x5C)=5;
            }
        }
        if (cur != *(int32_t *)(ws + 0x5C)) {
            int32_t index=*(int32_t *)(ws + 0x6C)*6 + *(int32_t *)(ws+0x5C);
            int16_t *entries=*(int16_t **)(*(uint8_t **)(ws+0xA8)+8);
            if (entries[index] != 0) {
                ((void (*)(int32_t))D_80055C48)(0x4001B);
                STCRDSHP_func_8008627c(ws,ctx,1);
            } else {
                *(int32_t *)(ws+0x5C)=cur;
            }
        }
        cur = *(int32_t *)(ws + 0x6C);
        {
            uint8_t *padbase=D_8004B7D0;
            if (!POLL(padbase,0x3F8,0x0B) && POLL(padbase,0x3F4,0x0A)) {
                idx=*(int32_t *)(ws+0x6C)-1;
                *(int32_t *)(ws+0x6C)=idx;
                if(idx<0) *(int32_t *)(ws+0x6C)=0;
            } else {
                uint8_t *padbase=D_8004B7D0;
                if (!POLL(padbase,0x3F8,0x0A) && POLL(padbase,0x3F4,0x0B)) {
                    idx=*(int32_t *)(ws+0x6C)+1;
                    st=*(int32_t *)(ws+0x70)-1;
                    *(int32_t *)(ws+0x6C)=idx;
                    if(st<idx) *(int32_t *)(ws+0x6C)=st;
                }
            }
        }
        if (cur != *(int32_t *)(ws + 0x6C)) {
            ((void (*)(int32_t))D_80055C48)(0x4001B);
            *(int32_t *)(ws + 0x60) = 0;
            *(int32_t *)(ws + 0x5C) = 0;
            i = 0;
            p = ws;
            do {
                int32_t index=*(int32_t *)(ws + 0x6C)*6+i;
                int16_t *entries=*(int16_t **)(*(uint8_t **)(ws+0xA8)+8);
                *(int32_t *)(p+0x74)=entries[index];
                p += 4;
                i++;
            } while (i < 6);
            { uint8_t *obj = *(uint8_t **)(ctx + 0x48);
            ((void (*)(void *, void *))(*(void (**)(void))(obj + 0xBC)))(obj, ws + 0x74);
            }
            if (*(int32_t *)(ws + 0x70) > 1) {
                if (*(int32_t *)(ws + 0x6C) > 0) {
                    price = ((int32_t (*)(int32_t))D_80044F4C)(D_8005CCA8 + 0x32);
                    { uint8_t *obj = *(uint8_t **)(ctx + 0x30);
                    ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x114)))(obj, price, 10);
                    }
                } else {
                    { uint8_t *obj = *(uint8_t **)(ctx + 0x30);
                    ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
                    }
                }
                if (*(int32_t *)(ws + 0x6C) < *(int32_t *)(ws + 0x70) - 1) {
                    price = ((int32_t (*)(int32_t))D_80044F4C)(D_8005CCA8 + 0x32);
                    { uint8_t *obj = *(uint8_t **)(ctx + 0x34);
                    ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x114)))(obj, price, 0x0B);
                    }
                } else {
                    { uint8_t *obj = *(uint8_t **)(ctx + 0x34);
                    ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
                    }
                }
            }
            *(int32_t *)(ws + 0x10) = 1;
        }
        {
            uint8_t *padbase = D_8004B7D0;
            if (POLL(padbase,0x3F4,0x0D) != 0) {
        F0x8001ebf8(&tmp);
        table = *(int16_t **)(*(uint8_t **)(ws + 0xA8) + 8);
        card = table[*(int32_t *)(ws + 0x6C) * 6 + *(int32_t *)(ws + 0x5C)];
        *(int32_t *)(ws + 0x98) = card;
        tmp.cb(card);
        price = (int16_t)((int32_t (*)(int32_t))D_8008CB9C)(*(int32_t *)(ws + 0x98));
        *(int32_t *)(ws + 0x9C) = price;
        ((void (*)(int32_t))D_80055C48)(0x4001C);
        if (((money_view *)D_80048D34)->money < *(int32_t *)(ws + 0x9C)) {
            *(int32_t *)(ws + 0x10) = 10;
            *(int32_t *)(ws + 0x14) = 0x13;
            return;
        }
        { uint32_t record=*(int32_t *)(ws + 0x98);
        record+=(uint32_t)D_80048D34;
        if (*(int8_t *)(record + 0x3A2) == 9) {
            *(int32_t *)(ws + 0x10) = 10;
            *(int32_t *)(ws + 0x14) = 0x14;
            return;
        }
        *(int32_t *)(ws + 0x10) = 5;
        return;
        }
            } else {
                if (POLL(padbase,0x3F4,0x0E) == 0) {
                    return;
                }
                ((void (*)(int32_t))D_80055C48)(0x800450BD);
                *(int32_t *)(ws + 0x10) = 0x32;
                return;
            }
        }
    case 5: {
        uint8_t *base=D_8008CB80;
        ((void (*)(void *, int32_t))*(void **)(base+8))(ws + 0xBC, 1);
        ((void (*)(void *, int32_t))*(void **)(base+8))(ws + 0xCC, 1);
        (*(int32_t *)(ws + 0x10))++;
        return;
    }
    case 6: {
        uint8_t *base=D_8008CB80;
        ((int32_t (*)(void *))*(void **)(base+12))(ws + 0xCC);
        if (((int32_t (*)(void *))*(void **)(base+12))(ws + 0xBC) == 0) {
            return;
        }
        {
            uint8_t *svc = D_80044B38;
            price = ((int32_t (*)(int32_t))*(void **)(svc + 0x414))(D_8005CCA8 + 0x32);
            { uint8_t *obj = *(uint8_t **)(ctx + 0x38);
            ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x114)))(obj, price, 0x0C);
            }
            { uint8_t *obj = *(uint8_t **)(ctx + 0x38);
            ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x118)))(obj, 1, *(int32_t *)(ws + 0x9C));
            }
            price = ((int32_t (*)(int32_t))*(void **)(svc + 0x414))(D_8005CCA8 + 0x32);
            { uint8_t *obj = *(uint8_t **)(ctx + 0x3C);
            ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x114)))(obj, price, 0x0D);
            }
            price = ((int32_t (*)(int32_t))*(void **)(svc + 0x414))(D_8005CCA8 + 0x32);
            { uint8_t *obj = *(uint8_t **)(ctx + 0x40);
            ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x114)))(obj, price, 0x0E);
            }
        }
        { uint8_t *obj = *(uint8_t **)(ctx + 0x44);
        ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x84)))(obj, 0xB8, *(int32_t *)(ws + 0x94) * 0x10 + 0x56);
        }
        { uint8_t *obj = *(uint8_t **)(ctx + 0x44);
        ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x80)))(obj, 1);
        }
        (*(int32_t *)(ws + 0x10))++;
        return;
    }
    case 7:
        cur = *(int32_t *)(ws + 0x94);
        {
            uint8_t *padbase = D_8004B7D0;
            if (POLL(padbase,0x3F4,4) != 0) {
                *(int32_t *)(ws + 0x94) = 0;
            } else {
                if (POLL(padbase,0x3F4,6) != 0) {
                    *(int32_t *)(ws + 0x94) = 1;
                }
            }
        }
        if (cur != *(int32_t *)(ws + 0x94)) {
            ((void (*)(int32_t))D_80055C48)(0x8004513E);
            { uint8_t *obj = *(uint8_t **)(ctx + 0x44);
            ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x84)))(obj, 0xB8, *(int32_t *)(ws + 0x94) * 0x10 + 0x56);
            }
        }
        {
            uint8_t *padbase = D_8004B7D0;
            if (POLL(padbase,0x3F4,0x0D) != 0) {
                ((void (*)(int32_t))D_80055C48)(0x8004503C);
                if (*(int32_t *)(ws + 0x94) == 0) {
                    uint8_t *base=D_80048D34;
                    ((void (*)(int32_t, int32_t))*(void **)(base+0x2720))(*(int32_t *)(ws + 0x98), 1);
                    *(int32_t *)(base+0x6C) -= *(int32_t *)(ws + 0x9C);
                    { uint8_t *obj = *(uint8_t **)(ws + 0x50);
                    ((void (*)(void *))(*(void (**)(void))(obj + 0x3C8)))(obj);
                    }
                }
                (*(int32_t *)(ws + 0x10))++;
                return;
            } else {
                if (POLL(padbase,0x3F4,0x0E) == 0) {
                    return;
                }
                ((void (*)(int32_t))D_80055C48)(0x800450BD);
                (*(int32_t *)(ws + 0x10))++;
                return;
            }
        }
    case 8:
        { uint8_t *obj = *(uint8_t **)(ctx + 0x38);
        ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
        }
        { uint8_t *obj = *(uint8_t **)(ctx + 0x3C);
        ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
        }
        { uint8_t *obj = *(uint8_t **)(ctx + 0x40);
        ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
        }
        { uint8_t *obj = *(uint8_t **)(ctx + 0x44);
        ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x80)))(obj, 0);
        }
        *(int32_t *)(ws + 0x94) = 0;
        ((void (*)(void *, int32_t))D_8008CB88)(ws + 0xBC, 0);
        (*(int32_t *)(ws + 0x10))++;
        return;
    case 10:
        ((void (*)(void *, int32_t))D_8008CB88)(ws + 0xBC, 1);
        *(int32_t *)(ws + 0xD4) = 0;
        (*(int32_t *)(ws + 0x10))++;
        return;
    case 11:
        if (((int32_t (*)(void *))D_8008CB8C)(ws + 0xBC) == 0) {
            return;
        }
        price = ((int32_t (*)(int32_t))D_80044F4C)(D_8005CCA8 + 0x32);
        { uint8_t *obj = *(uint8_t **)(ctx + 0x38);
        ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))(obj + 0x114)))(obj, price, *(int32_t *)(ws + 0x14));
        }
        (*(int32_t *)(ws + 0x10))++;
        return;
    case 12: {
        uint8_t *padbase = D_8004B7D0;
        if (POLL(padbase,0x3F4,0x0D) == 0) {
            if (POLL(padbase,0x3F4,0x0E) == 0) {
                return;
            }
        }
        ((void (*)(int32_t))D_80055C48)(0x4001C);
        { uint8_t *obj = *(uint8_t **)(ctx + 0x38);
        ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
        }
        ((void (*)(void *, int32_t))D_8008CB88)(ws + 0xBC, 0);
        (*(int32_t *)(ws + 0x10))++;
        return;
    }
    case 9:
    case 0x0D:
        if (((int32_t (*)(void *))D_8008CB8C)(ws + 0xBC) == 0) {
            return;
        }
        *(int32_t *)(ws + 0x10) = 1;
        return;
    case 0x32:
        *(int32_t *)(ws + 0x60) = 0;
        { uint8_t *obj = *(uint8_t **)(ctx + 0x30);
        ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
        }
        { uint8_t *obj = *(uint8_t **)(ctx + 0x34);
        ((void (*)(void *, int32_t))(*(void (**)(void))(obj + 0x144)))(obj, 0);
        }
        STCRDSHP_func_8008627c(ws, ctx, 0);
        ((void (*)(void *, int32_t))D_8008CB88)(ws + 0xAC, 0);
        (*(int32_t *)(ws + 0x10))++;
        return;
    case 0x33:
        if (((int32_t (*)(void *))D_8008CB8C)(ws + 0xAC) == 0) {
            return;
        }
        { uint8_t *obj = *(uint8_t **)(ctx + 0x48);
        ((void (*)(void *))(*(void (**)(void))(obj + 0xC0)))(obj);
        }
        *(int32_t *)(ws + 0x0C) = 2;
        return;
    }
}
