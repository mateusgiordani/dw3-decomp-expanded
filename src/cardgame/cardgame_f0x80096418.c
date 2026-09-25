/*
 * CARDGAME:0x80096418 CARDGAME_F0x80096418
 * 992 bytes at CARDGAME.PRO offset 0x13768 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80096418
 *  Symbols     CARDGAME_F0x800961f0=0x800961f0 DAT_8004935C=0x8004935c
 *              DAT_8004995C=0x8004995c DAT_8004DF9C=0x8004df9c
 *              DAT_800A5940=0x800a5940 EXE_F0x8001ae38=0x8001ae38
 *  Compare     992 bytes from 0x80096418 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80096418
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue addiu sp,-0x28 (saves s0-s4,ra), epilogue jr ra /
 * addiu sp,+0x28.
 *
 * Next body at 0x800967F8 (+0x3E0): leaf handlers at
 * 0x800967F8/0x80096808/0x80096814, then framed CARDGAME_F0x80096828; confirms
 * size 992 contiguous, no gap.
 *
 * Reference words @ 0x13768: first8 27bdffd8 afb3001c 00809821 afb40020
 * afbf0024 afb20018 afb10014 afb00010; last8 8fbf0024 8fb40020 8fb3001c
 * 8fb20018 8fb10014 8fb00010 03e00008 27bd0028. End off 0x13B48.
 *
 * Direct jal EXE 0x8001ae38 x2 is cross-program so absent from intra-program
 * graph; recorded here from disasm.
 *
 * Dispatch (PAL order): state=*(obj+0x0C); if (state==1) -> S1 region
 * @0x800965bc; else if (state<2) -> INIT @0x80096464; else if (state==2) -> S2
 * @0x8009665c; else if (state==3) -> TAIL @0x800966bc; else -> INIT.
 *
 * INIT: (*(obj+0x38))(obj); halves +0x6A/+0x68=12, byte +0x66=0, halves
 * +0x5E=0x1000/+0x5C=0; buf0=EXE_0x8001ae38(0x100,1,0,0); out[0]=buf0;
 * (buf0+0x114)(buf0, idx*0x66+DAT_8004935C, -1) with idx=*(obj+0x50);
 * (buf0+0x134)(buf0, h58+5, h5A+3); 6x alloc/fill loop over DAT_800A5940 pairs
 * with (buf+0x134)(buf,h58+u0,h5A+u1), (slot+0x118)(slot,0,byte(obj+s2+0x60)),
 * (slot+0x148)(slot,1).
 *
 * S1 (state==1): b=obj[0x66]; b==1 -> TAIL; b<2: b!=0 -> TAIL, b==0 progress
 * (0x5C=0x1000-((h68<<12)/h6A); h68-=DAT_8004DF9C(); if ((int16)h68>0 -> TAIL
 * else 0x5C=0x1000,b=1); b==2 -> (*(obj+0x28))(obj,2); b>2 -> TAIL.
 *
 * S2 (state==2): 0x5C=(h68<<12)/h6A; h68-=DAT_8004DF9C(); if ((int16)h68>0 ->
 * TAIL else 0x5C=0, (*(obj+0x28))(obj,3).
 *
 * TAIL: b==1 -> (out0+0x134)(out0,h58+5,h5A+3); (out0+0x144)(out0,1); 6x
 * ((next+0x144)(next,1); (next+0x134)(next,h58+u0,h5A+u1)); else ->
 * (out0+0x144)(out0,0); 6x ((next+0x144)(next,0)). Then
 * CARDGAME_F0x800961f0(obj); return.
 *
 * Portable C recovery (campaign cardgame-human-v1 rev 2): no __asm__, no words,
 * no register variables. lh vs lhu preserved via int16_t/uint16_t loads; sh/sb
 * stores; 16-bit sign wrap via (int32_t)(int16_t)(u+const); table halves via
 * uint16_t pair walk.
 *
 * r7 (o55/s0923e): S1 is a switch (cases 0/1/2, root 1); one allocation local
 * b0 for both allocations spans blocks, so global alloc gives it a0 after local
 * alloc gave u0 v0.
 *
 * Details: strategy-r7-o55/attempts-r7.
 */

#include <stdint.h>

extern void *EXE_F0x8001ae38(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
extern void CARDGAME_F0x800961f0(uint8_t *obj);
extern int32_t (*DAT_8004DF9C)(void);
extern int16_t DAT_800A5940;
extern uint8_t DAT_8004935C;

typedef void (*cardgame_m0_t)(uint8_t *obj);
typedef void (*cardgame_m1_t)(uint8_t *obj, int32_t a1);
typedef void (*cardgame_s0_t)(void *buf, void *ptr, int32_t a2);
typedef void (*cardgame_s2_t)(void *buf, int32_t a1, int32_t a2);
typedef void (*cardgame_s3_t)(void *buf, int32_t a1, int32_t a2, int32_t a3);
typedef void (*cardgame_t2_t)(void *buf, int32_t a1, uint8_t a2);
typedef void (*cardgame_t1_t)(void *buf, int32_t a1);

void CARDGAME_F0x80096418(uint8_t *obj, int32_t *out) {
    int16_t *tbl;
    int32_t *walk;
    int32_t i;
    int32_t st;

    st = *(int32_t *)(obj + 0x0C);
    if (st == 1) {
        goto s1;
    }
    if (st < 2) {
        goto init;
    }
    if (st == 2) {
        goto s2;
    }
    if (st != 3) {
        goto init;
    }
    goto tail;
init: {
    void *b0;
    int32_t idx;

    ((cardgame_m0_t)*(uint32_t *)(obj + 0x38))(obj);
    *(int16_t *)(obj + 0x6A) = 12;
    *(int16_t *)(obj + 0x68) = 12;
    *(obj + 0x66) = 0;
    *(int16_t *)(obj + 0x5E) = 0x1000;
    *(int16_t *)(obj + 0x5C) = 0;
    b0 = EXE_F0x8001ae38(0x100, 1, 0, 0);
    out[0] = (int32_t)b0;
    idx = *(int32_t *)(obj + 0x50);
    ((cardgame_s0_t)*(uint32_t *)((uint8_t *)b0 + 0x114))(b0, &DAT_8004935C + idx * 0x66, -1);
    ((cardgame_s2_t)*(uint32_t *)(out[0] + 0x134))((void *)out[0],
        (int32_t)(int16_t)(*(uint16_t *)(obj + 0x58) + 5),
        (int32_t)(int16_t)(*(uint16_t *)(obj + 0x5A) + 3));
    tbl = &DAT_800A5940;
    walk = out;
    i = 0;
    do {
        uint16_t u0;
        uint16_t u1;

        b0 = EXE_F0x8001ae38(0x100, 1, 0, 0);
        walk[1] = (int32_t)b0;
        u0 = ((uint16_t *)tbl)[0];
        u1 = ((uint16_t *)tbl)[1];
        tbl += 2;
        ((cardgame_s2_t)*(uint32_t *)((uint8_t *)b0 + 0x134))(b0,
            (int32_t)(int16_t)(*(uint16_t *)(obj + 0x58) + u0),
            (int32_t)(int16_t)(*(uint16_t *)(obj + 0x5A) + u1));
        ((cardgame_t2_t)*(uint32_t *)(walk[1] + 0x118))((void *)walk[1], 0, *(obj + i + 0x60));
        ((cardgame_t1_t)*(uint32_t *)(walk[1] + 0x148))((void *)walk[1], 1);
        i++;
        walk++;
    } while (i < 6);
    goto tail;
}
s1:
    switch (*(uint8_t *)(obj + 0x66)) {
    case 0: {
        int32_t q;
        int32_t rem;

        q = (*(int16_t *)(obj + 0x68) << 12) / *(int16_t *)(obj + 0x6A);
        *(int16_t *)(obj + 0x5C) = (int16_t)(0x1000 - q);
        rem = DAT_8004DF9C();
        rem = (int32_t)*(uint16_t *)(obj + 0x68) - rem;
        *(int16_t *)(obj + 0x68) = (int16_t)rem;
        if ((int16_t)rem > 0) {
            break;
        }
        *(int16_t *)(obj + 0x5C) = 0x1000;
        *(obj + 0x66) = (uint8_t)st;
        break;
    }
    case 1:
        break;
    case 2:
        ((cardgame_m1_t)*(uint32_t *)(obj + 0x28))(obj, 2);
        break;
    }
    goto tail;
s2: {
    int32_t q;
    int32_t rem;

    q = (*(int16_t *)(obj + 0x68) << 12) / *(int16_t *)(obj + 0x6A);
    *(int16_t *)(obj + 0x5C) = (int16_t)q;
    rem = DAT_8004DF9C();
        rem = (int32_t)*(uint16_t *)(obj + 0x68) - rem;
    *(int16_t *)(obj + 0x68) = (int16_t)rem;
    if ((int16_t)rem > 0) {
        goto tail;
    }
    *(int16_t *)(obj + 0x5C) = 0;
    ((cardgame_m1_t)*(uint32_t *)(obj + 0x28))(obj, 3);
}
tail: {
    int32_t b;

    b = *(uint8_t *)(obj + 0x66);
    if (b == 1) {
        void *s0;

        s0 = (void *)out[0];
        ((cardgame_s2_t)*(uint32_t *)((uint8_t *)s0 + 0x134))(s0,
            (int32_t)(int16_t)(*(uint16_t *)(obj + 0x58) + 5),
            (int32_t)(int16_t)(*(uint16_t *)(obj + 0x5A) + 3));
        s0 = (void *)out[0];
        ((cardgame_t1_t)*(uint32_t *)((uint8_t *)s0 + 0x144))(s0, 1);
        tbl = &DAT_800A5940;
        walk = out;
        for (i = 0; i < 6;) {
            void *slot;
            uint16_t u0;
            uint16_t u1;

            ((cardgame_t1_t)*(uint32_t *)(walk[1] + 0x144))((void *)walk[1], 1);
            i++;
            u0 = ((uint16_t *)tbl)[0];
            u1 = ((uint16_t *)tbl)[1];
            tbl += 2;
            slot = (void *)walk[1];
            walk++;
            ((cardgame_s2_t)*(uint32_t *)((uint8_t *)slot + 0x134))(slot,
                (int32_t)(int16_t)(*(uint16_t *)(obj + 0x58) + u0),
                (int32_t)(int16_t)(*(uint16_t *)(obj + 0x5A) + u1));
        }
    } else {
        void *s0;

        s0 = (void *)out[0];
        ((cardgame_t1_t)*(uint32_t *)((uint8_t *)s0 + 0x144))(s0, 0);
        walk = out;
        i = 0;
        do {
            void *slot;

            slot = (void *)walk[1];
            ((cardgame_t1_t)*(uint32_t *)((uint8_t *)slot + 0x144))(slot, 0);
            walk++;
            i++;
        } while (i < 6);
    }
    CARDGAME_F0x800961f0(obj);
}}
