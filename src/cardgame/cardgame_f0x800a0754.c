/*
 * CARDGAME:0x800a0754 CARDGAME_F0x800a0754
 * 240 bytes at CARDGAME.PRO offset 0x1daa4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a0754
 *  Symbols     CARDGAME_F0x800a0590=0x800a0590 CARDGAME_F0x800a0708=0x800a0708
 *  Compare     240 bytes from 0x800a0754 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a0754
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Verified base 0x80082cb0, REFERENCE_VERSION PAL-SLES-03936.
 *
 * Next function CARDGAME:0x800a0844 at +0xF0 (d0ffbd27), confirms size.
 *
 * Condition: *pb < *(uint8_t*)(p1+0x444) (unsigned, sltu) &&
 * *(int8_t*)(p1+i+0x46f)!=0 (signed lb). Body jal
 * 0x800a0708(p1,p2,*(int16*)(cur+100)), then cur+=2, i++. Tail jal
 * 0x800a0590(p1,p3). Xref DATA at 0x800a328c in FUN_800a3240 installs this as
 * function pointer at slot 0x818.
 */

#include <stdint.h>

extern void CARDGAME_F0x800a0708(int32_t a0, int32_t a1, int32_t a2);
extern void CARDGAME_F0x800a0590(int32_t a0, int32_t a1);

void CARDGAME_F0x800a0754(int32_t p1, int32_t p2)
{
    int32_t off1;
    int32_t off2;
    uint8_t *pb;
    int32_t p3;

    off1 = p2 * 0x72 + 0x72c;
    pb = (uint8_t *)(p1 + off1);
    off2 = p2 * 200 + 0x59c;
    p3 = p1 + off2;
    *pb = 0;
    if (*(int16_t *)(p3 + 10) > 0) {
        int32_t i = 0;
        int32_t cur = p3;
        do {
            if (*pb < *(uint8_t *)(p1 + 0x444) && *(int8_t *)(p1 + i + 0x46f) != 0) {
                CARDGAME_F0x800a0708(p1, p2, (int32_t)*(int16_t *)(cur + 100));
            }
            cur += 2;
            i++;
        } while (i < *(int16_t *)(p3 + 10));
    }
    CARDGAME_F0x800a0590(p1, p3);
}
