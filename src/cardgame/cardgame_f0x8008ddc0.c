#include "common/types.h"

/* CARDGAME:0x8008ddc0 (236B, 0xEC) -- portable C recovery from PAL bytes.
 * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xb110.
 * Boundary sweep #58: prologue 27bdffd8 addiu sp,-0x28, epilogue jr ra +
 * 27bd0028 addiu sp,+0x28; prev CARDGAME:0x8008dd38 size 0x88 ends exactly
 * at 0x8008ddc0, next CARDGAME:0x8008deb4 at +0xEC (+8 caller-setup gap).
 * Ghidra ddw3-pal-sles-03936/CARDGAME read-only: loop over 12 entries gated
 * on the signed byte at ctx+i+0x46f and the unsigned limits at ctx+0x72c
 * (first 6) / ctx+0x79e (last 6); indirect call *(p2+0xf38)(p2,i,side) per
 * passing entry; then, if any entry passed, the EXE vector *0x80055c48 with
 * 0x9c0000 for side 1 and 0x9c0001 otherwise; finally ctx word +0x424 = 0.
 * x-ref to: 2 jal sites in CARDGAME_F0x80084320 (side 0 and side 1).
 * Revision 7 (GCC 2.8.1 SN32 / ASPSX 2.79 O2, exact): the vector call is a
 * switch whose default shares the case 0 body, written first, so the tests
 * fall into the 0x9c0001 call and case 1 jumps to the 0x9c0000 call; and the
 * vector is referenced through the symbol D_80055c48 so its lui can occupy
 * the case 1 branch delay slot (--symbol D_80055c48=0x80055c48). */
typedef void (*CardSlotFn_ddc0)(void *p2, int32_t idx, int32_t side);
typedef void (*ExeVecFn_ddc0)(uint32_t arg);

extern ExeVecFn_ddc0 D_80055c48;

void CARDGAME_F0x8008ddc0(void *ctx, void *p2, int32_t side)
{
    int32_t hit = 0;
    int32_t i;

    for (i = 0; i < 12; i++) {
        if (*(int8_t *)((char *)ctx + i + 0x46f) != 0) {
            if (i < 6) {
                if (i < *(uint8_t *)((char *)ctx + 0x72c)) {
                    (*(CardSlotFn_ddc0 *)((char *)p2 + 0xf38))(p2, i, side);
                    hit = 1;
                }
            } else {
                if ((i - 6) < *(uint8_t *)((char *)ctx + 0x79e)) {
                    (*(CardSlotFn_ddc0 *)((char *)p2 + 0xf38))(p2, i, side);
                    hit = 1;
                }
            }
        }
    }
    if (hit != 0) {
        switch (side) {
        case 0:
        default:
            D_80055c48(0x9c0001);
            break;
        case 1:
            D_80055c48(0x9c0000);
            break;
        }
    }
    *(int32_t *)((char *)ctx + 0x424) = 0;
}
