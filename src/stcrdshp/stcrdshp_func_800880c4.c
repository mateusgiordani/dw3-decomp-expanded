/*
 * STCRDSHP:0x800880c4 STCRDSHP_func_800880c4
 * 712 bytes at STCRDSHP.PRO offset 0x5414 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800880c4
 *  Symbols     CARDGAME_F0x80097958=0x80097958 DAT_80044f5c=0x80044f5c
 *              D_80044b38=0x80044b38 D_80044b38_2=0x80044b38
 *              EXE_F0x8001f648=0x8001f648 STCRDSHP_func_8008838c=0x8008838c
 *              STCRDSHP_func_80088b50=0x80088b50
 *  Compare     712 bytes from 0x800880c4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x800880c4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Caller: 1 direct jal from STCRDSHP_func_80088b50 at 0x80088c20 (case-1 path:
 * STCRDSHP_func_8008838c(arg0,arg1) then this function).
 *
 * Callees: 1 direct jal EXE 0x8001f648 (table init, buf at sp+0x10); 5
 * stack-table indirect jalr (lw v0,off(sp)+nop+jalr, MIPS-I load delay) at
 * sp+0x84/0x8c/0x94/0x9c/0xa4; EXE-vector jalr via *(0x80044f5c) (lui 0x8004 /
 * s1-base 0x80044b38 + lw 0x424(s1), recomputed once; final block uses lui/lw
 * 0x4f5c(v0) form) with a0=0x63e0000.
 *
 * Body order: init(buf) ; f8c(*(arg0+0x50),7) ; f84(0x280,0) ; 0x5c==0 ?
 * (0x5c=1) : (0x58+=1 with 0x60 wrap, 0x5c=0) ; r=DAT(0x63e0000) ;
 * f94(r,8,*(0x58),*(0x58)) ; f8c(*(0x50),*(0x54)-1) ; three guarded slot blocks
 * at arg0+0x3a0/0x3b0 (each: nonzero?/0x1000-scaled draw + f94 triple +
 * conditional f-a4 + second f94 triple) and arg0+0x3c0 (scaled draw + single
 * f94 triple).
 *
 * The 0x58 counter (mod 0x60) feeds both a2/a3 of the first f94 call; 0x5c is
 * its companion toggle. Semantics of the drawn triples and the EXE vector
 * constant stay unknown; no struct promoted beyond offsets.
 */

#include "common/types.h"

typedef void (*stcrdshp_cb2_t)(int32_t a0, int32_t a1);
typedef void (*stcrdshp_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*stcrdshp_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*stcrdshp_svc_t)(int32_t a0);

void EXE_F0x8001f648(void *buf);
extern uint8_t D_80044b38[];
extern uint8_t D_80044b38_2[];
extern stcrdshp_svc_t DAT_80044f5c;

typedef struct {
    uint32_t w[40];
} stcrdshp_880c4_buf_t;

void STCRDSHP_func_800880c4(int32_t arg0)
{
    stcrdshp_880c4_buf_t buf;
    uint8_t *exebase1;
    uint8_t *exebase2;
    int32_t counted;
    int32_t kept;
    int32_t slot;
    int32_t svc;

    EXE_F0x8001f648(buf.w);
    ((stcrdshp_cb2_t)buf.w[31])(*(int32_t *)(arg0 + 0x50), 7);
    ((stcrdshp_cb2_t)buf.w[29])(0x280, 0);
    if (*(int32_t *)(arg0 + 0x5c) != 0) {
        counted = *(int32_t *)(arg0 + 0x58) + 1;
        *(int32_t *)(arg0 + 0x58) = counted;
        kept = 0;
        if (counted < 0x60) {
            kept = counted;
        }
        *(int32_t *)(arg0 + 0x58) = kept;
        *(int32_t *)(arg0 + 0x5c) = 0;
    } else {
        *(int32_t *)(arg0 + 0x5c) = 1;
    }
    exebase1 = D_80044b38;
    svc = ((stcrdshp_svc_t) * (int32_t *)(exebase1 + 0x424))(0x63E0000);
    ((stcrdshp_cb4_t)buf.w[33])(svc, 8, *(int32_t *)(arg0 + 0x58), *(int32_t *)(arg0 + 0x58));
    ((stcrdshp_cb2_t)buf.w[31])(*(int32_t *)(arg0 + 0x50), *(int32_t *)(arg0 + 0x54) - 1);
    slot = *(int32_t *)(arg0 + 0x3a0);
    if (slot != 0) {
        if (slot != 0x1000) {
            ((stcrdshp_cb3_t)buf.w[35])(slot, 0x1000, 0x1000);
            ((stcrdshp_cb2_t)buf.w[37])(0x57, 0x1b);
        }
        svc = ((stcrdshp_svc_t) * (int32_t *)(exebase1 + 0x424))(0x63E0000);
        ((stcrdshp_cb4_t)buf.w[33])(svc, 0x28, 0x16, 0x14);
        if (*(int32_t *)(arg0 + 0x3a0) != 0x1000) {
            ((stcrdshp_cb2_t)buf.w[37])(0x140, 0x1d);
        }
        svc = ((stcrdshp_svc_t) * (int32_t *)(exebase1 + 0x424))(0x63E0000);
        ((stcrdshp_cb4_t)buf.w[33])(svc, 0x29, 0xd6, 0x15);
    }
    slot = *(int32_t *)(arg0 + 0x3b0);
    if (slot != 0) {
        if (slot != 0x1000) {
            ((stcrdshp_cb3_t)buf.w[35])(slot, 0x1000, 0x1000);
            ((stcrdshp_cb2_t)buf.w[37])(0x140, 0x49);
        }
        exebase2 = D_80044b38_2;
        svc = ((stcrdshp_svc_t) * (int32_t *)(exebase2 + 0x424))(0x63E0000);
        ((stcrdshp_cb4_t)buf.w[33])(svc, 0x2f, 0x92, 0x2e);
        if (*(int32_t *)(arg0 + 0x3b0) != 0x1000) {
            ((stcrdshp_cb2_t)buf.w[37])(0x140, 0xd2);
        }
        svc = ((stcrdshp_svc_t) * (int32_t *)(exebase2 + 0x424))(0x63E0000);
        ((stcrdshp_cb4_t)buf.w[33])(svc, 0x0f, 0xc6, 0xc4);
    }
    slot = *(int32_t *)(arg0 + 0x3c0);
    if (slot != 0) {
        if (slot != 0x1000) {
            ((stcrdshp_cb3_t)buf.w[35])(slot, 0x1000, 0x1000);
            ((stcrdshp_cb2_t)buf.w[37])(0x140, 0x77);
        }
        svc = DAT_80044f5c(0x63E0000);
        ((stcrdshp_cb4_t)buf.w[33])(svc, 0x2c, 0x7b, 0x6b);
    }
}