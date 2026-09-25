/*
 * CARDGAME:0x8008cfdc CARDGAME_F0x8008cfdc
 * 664 bytes at CARDGAME.PRO offset 0xa32c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008cfdc
 *  Symbols     CARDGAME_F0x80084320=0x80084320 DAT_8004B7D0=0x8004b7d0
 *              D_80055C48=0x80055c48
 *  Compare     664 bytes from 0x8008cfdc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008cfdc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Sysbit tests use unnamed call temps (sys0(0)>>sys1(0,imm))&1 so the save
 * lands in the next jalr delay slot (r10 S); ret = 0 sits between the m2 and m1
 * calls (r10 C3).
 *
 * EXE sys base is an extern array (DAT_8004B7D0): plain constant would emit
 * li+ori, extern yields lui+addiu. idx*76 stride is base-first addu (p2+off).
 *
 * Lineage: A2 funnel (r9) + S/C3 (r10) + shared-fail-return R-a/R-b (r6).
 */

#include <stdint.h>

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
extern void (*D_80055C48)(uint32_t);
typedef void (*cardgame_m1_t)(int32_t);
typedef void (*cardgame_m2_t)(int32_t, int32_t);
typedef void (*cardgame_m5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

extern int32_t DAT_8004B7D0[];

int32_t CARDGAME_F0x8008cfdc(int32_t p1, int32_t p2)
{
    int32_t ret = -1;
    int32_t st;
    int32_t k;
    int8_t idx;
    uint8_t idxu;

    st = *(uint8_t *)(p1 + 0x422);
    if (st == 2)
        goto Ldispatch;
    if (st < 3) {
        if (st == 1)
            goto Lcase1;
        goto Lheadfail;
    } else {
        if (st == 3)
            goto Lcase3;
    }
Lheadfail:
    return ret;
Lcase1:
    if (*(int16_t *)(p2 + 0x64) != 2)
        goto Lheadfail;
    if (*(uint8_t *)(p1 + 0x498) != 0)
        goto Lheadfail;
    *(uint8_t *)(p1 + 0x422) = 2;
    idx = *(int8_t *)(p1 + 0x575);
    if (idx == 0)
        goto Lfun;
    {
        uint8_t *u;
        u = (uint8_t *)(p2 + (idx + 11) * 76);
        u[0x150] |= 1;
    }
    goto Lfun;
Ldispatch:
    if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) != 0) {
            uint8_t *u;
            if (*(int32_t *)(p1 + 0x438) != 1)
                goto Lheadfail;
            D_80055C48(0x4001c);
            idx = *(int8_t *)(p1 + 0x575);
            k = idx + 11;
            u = (uint8_t *)(p2 + k * 76);
            u[0x150] &= 0xfe;
            idxu = *(uint8_t *)(p1 + 0x575);
            *(int8_t *)(p1 + k + 0x46f) = idxu + 1;
            idx = *(int8_t *)(p1 + 0x575);
            *(int16_t *)(p1 + ((idx - 1) << 3) + 0x582) = idx;
            ret = 1;
            goto Lfun;
        } else {
            if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 14)) & 1) != 0) {
                D_80055C48(0x800450bd);
                ((cardgame_m1_t)*(uint32_t *)(p2 + 0xec4))(p2);
                *(int32_t *)(p1 + 0x440) = 1;
                *(uint8_t *)(p1 + 0x422) = 3;
                ((cardgame_m5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 4, 0, 0x1000);
                *(uint8_t *)(p1 + 0x499) = st;
                goto Lfun;
            }
            goto Lfun;
        }
    return ret;
Lcase3:
    if (*(int16_t *)(p2 + 0x64) != 0)
        goto Lheadfail;
    if (*(uint8_t *)(p1 + 0x498) != 0)
        goto Lheadfail;
    ((cardgame_m2_t)*(uint32_t *)(p2 + 0xf18))(p2, 0xf);
    ret = 0;
    ((cardgame_m1_t)*(uint32_t *)(p2 + 0xeb4))(p2);
Lfun:
    return ret;
}
