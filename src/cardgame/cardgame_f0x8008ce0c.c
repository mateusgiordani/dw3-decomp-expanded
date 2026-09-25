/*
 * CARDGAME:0x8008ce0c CARDGAME_F0x8008ce0c
 * 464 bytes at CARDGAME.PRO offset 0xa15c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008ce0c
 *  Symbols     F0x8001ebf8=0x8001ebf8
 *  Compare     464 bytes from 0x8008ce0c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008ce0c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next framed CARDGAME:0x8008cfdc at +0x1D0 (27bdffc8 prologue), size 0x1D0
 * contiguous, no overlap.
 *
 * No Ghidra state change.
 *
 * Callers: 2 direct jal from CARDGAME_F0x80084320 (0x80084d8c with a2=0,
 * 0x80084da4 with a2=1, both jal word 0c023383); 1 direct callee EXE
 *
 * Reg note: the two static `flag = 1` sets (p3==0 inline + p3==1 gated)
 * tail-merge into one `li s2,1`; the extra static set raises flag above p2 in
 * the global pick order, yielding p1->s1, p2->s3, p3->s0, flag->s2 per PAL.
 */

typedef void (*cardgame_8ce0c_cb1_t)(int32_t);
typedef void (*cardgame_8ce0c_cb3_t)(int32_t, int32_t, int32_t);
typedef void (*cardgame_8ce0c_cb4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_8ce0c_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_8ce0c_fn_t)(int32_t);

void F0x8001ebf8(void *buf);

void CARDGAME_F0x8008ce0c(int32_t p1, int32_t p2, int32_t p3)
{
    int i;
    int p;
    int flag;
    int h;
    int ha;
    int one;
    int k;
    uint8_t *u;
    uint8_t buf[0x54];

    ((cardgame_8ce0c_cb1_t)*(uint32_t *)(p2 + 0xecc))(p2);
    *(uint32_t *)(p1 + 0x440) = 0;
    *(uint8_t *)(p1 + 0x422) = 1;
    *(uint32_t *)(p1 + 0x438) = 0;
    ((cardgame_8ce0c_cb4_t)*(uint32_t *)(p2 + 0xf14))(p2, 0xf, 0xe500, 0x6100);
    ((cardgame_8ce0c_cb3_t)*(uint32_t *)(p2 + 0xf3c))(p2, 0xf, *(int16_t *)(p1 + (((int8_t *)p1)[0x575] << 3) + 0x580));
    *(uint16_t *)(p2 + 0x594) = 0;
    ((cardgame_8ce0c_cb5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 8, 0x1000, 0x1000);
    ((cardgame_8ce0c_cb1_t)*(uint32_t *)(p2 + 0xec8))(p2);
    one = 1;
    i = 0xe;
    p = p1 + i;
    do {
        *(uint8_t *)(p + 0x49e) = one;
        p--;
        i--;
    } while (i >= 0);
    *(uint8_t *)(p1 + 0x4ad) = 0;
    i = 0xe;
    p = p1 + i;
    do {
        *(uint8_t *)(p + 0x46f) = 0;
        p--;
        i--;
    } while (i >= 0);
    if (*(int8_t *)(p1 + 0x575) == 0)
        goto done;
    flag = 0;
    if (p3 == 0) {
        flag = 1;
        goto check;
    }
    if (p3 != 1)
        goto check;
    h = *(int16_t *)(p1 + ((*(int8_t *)(p1 + 0x575) - 1) << 3) + 0x580);
    F0x8001ebf8(buf);
    ha = p1 + h * 2;
    ((cardgame_8ce0c_fn_t)*(uint32_t *)(buf + 0x2c))(*(int16_t *)(ha + 0x50) + 1);
    if (**(uint8_t **)buf != 6)
        goto check;
    flag = 1;
check:
    if (flag == 0)
        goto done;
    k = *(int8_t *)(p1 + 0x575) + 11;
    u = (uint8_t *)(p2 + k * 76);
    u[0x150] |= 1;
    *(uint8_t *)(p1 + k + 0x49e) = 0;
    *(uint32_t *)(p1 + 0x438) = 1;
done:
    *(uint8_t *)(p1 + 0x499) = 1;
}



