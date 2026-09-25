/*
 * CARDGAME:0x8009a82c CARDGAME_F0x8009a82c
 * 364 bytes at CARDGAME.PRO offset 0x17b7c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009a82c
 *  Symbols     CARDGAME_F0x8009a82c=0x8009a82c D_800A5AD8=0x800a5ad8
 *              F0x8001f648=0x8001f648
 *  Compare     364 bytes from 0x8009a82c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009a82c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next CARDGAME:0x8009a998 contiguous, size 0x16c exact.
 *
 * Words compared with reference bin at off 0x17b7c: head 27bdff40 afb000b0
 * 00808021 ... tail 8fbf00b8 8fb100b4 8fb000b0 03e00008 27bd00c0, all equal.
 *
 * No Ghidra state change.
 *
 * F0x8001f648(buf) fills uint8_t buf[0xa0] at sp+0x10, fn slots at
 * buf+0x74/0x78/0x7c/0x84/0x8c/0x94 (sp+0x84/0x88/0x8c/0x94/0x9c/0xa4).
 *
 * Symbols: F0x8001f648=0x8001f648, CARDGAME_F0x8009a82c=0x8009a82c,
 */

#include <stdint.h>

extern uint8_t D_800A5AD8[];

extern void F0x8001f648(void *a0);

void CARDGAME_F0x8009a82c(int32_t p)
{
    uint8_t buf[0xA0];
    int32_t t0;
    int32_t t1;
    uint8_t *base;
    int16_t *P;
    uint8_t *Q;
    int32_t off0;
    int32_t off1;
    int32_t q;
    int32_t q32;
    int32_t rem;
    uint16_t u;

    t0 = *(int16_t *)(p + 0x3A);
    t1 = t0;
    if (t1 < 0) {
        t1 += 7;
    }
    base = D_800A5AD8;
    off1 = *(int32_t *)(base + 8);
    P = (int16_t *)(base + off1);
    P[0] = 1;
    P[2] = -1;
    u = *(uint16_t *)(p + 0x3A);
    P[4] = 4;
    P[5] = 2;
    off0 = *(int32_t *)base;
    P[3] = 0;
    P[1] = u;
    q = t1 >> 3;
    q32 = q * 32;
    *(int16_t *)(base + off0) = (int16_t)q32;
    Q = base + off0;
    rem = t0 - q * 8;
    *(int16_t *)(Q + 2) = (int16_t)rem * 32;
    *(uint16_t *)(Q + 4) = 0x20;
    *(uint16_t *)(Q + 6) = 0x20;
    *(uint16_t *)(Q + 8) = 0;
    *(uint16_t *)(Q + 10) = 0x100;
    *(uint16_t *)(Q + 12) = 1;
    F0x8001f648(buf);
    if (*(int32_t *)(p + 0x18) != 0x10001000) {
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x94))(((*(int32_t *)p) >> 8) + 0x14, ((*(int32_t *)(p + 4)) >> 8) + 0x17);
        ((void (*)(int32_t, int32_t, int32_t))*(void **)(buf + 0x8c))(*(int16_t *)(p + 0x18), *(int16_t *)(p + 0x1a), 0x1000);
    }
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x7C))(0x100, 1);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x74))(0x140, 0x100);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x78))(0x300, 0x100);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(buf + 0x84))((int32_t)base, 0, (*(int32_t *)p) >> 8, (*(int32_t *)(p + 4)) >> 8);
}
