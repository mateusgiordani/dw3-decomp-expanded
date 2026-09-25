/*
 * CARDGAME:0x8008db7c CARDGAME_F0x8008db7c
 * 244 bytes at CARDGAME.PRO offset 0xaecc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008db7c
 *  Symbols     (none)
 *  Compare     244 bytes from 0x8008db7c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008db7c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Guard: two separate if-gotos, NOT (v1 == 0 || v1 != 1). The || form collapses
 * to a single beq v1,1 under O2, while PAL keeps the redundant branch-to-next
 * (beq v1,zero->store, ret=0 in its delay slot), an explicit load-delay nop,
 * and the late li v0,1 + beq v1,v0->check (first sll of the p3*76 chain in its
 * delay slot). ret = 0 is initialized before the load.
 *
 * Path-A address: fresh dst (addrA = p2 + off, base first) emits addu v0,s1,v0.
 * Reusing the addend (t = p2 + t) or the inline (p2 + p3 * 76) form emits addu
 * v0,v0,s1 instead (cf. lesson 008).
 *
 * Path B reuses the loaded guard value as the constant: bne v0,v1, since v1 ==
 * 1 on that edge; p1 stays parked in a3 (move a3,a0 at entry).
 *
 * Xref: 1 caller CARDGAME_F0x8008dd38 via 0x8008dd78 UNCONDITIONAL_CALL (arity
 * 3 confirmed by that caller's extern decl). 0 direct callees; 2 indirect jalr
 * via table slots p2+0xf08 (5 args, a3=-0x5000, 0x6100 at 0x10(sp)) and
 * p2+0xf20 (4 args, a3=a2=0x1200). Raw lbu at +0x14a; sh stores at
 * +0x12e/+0x120. Stride 0x4c=76. Offsets stay raw: no CARDGAME slot mapping in
 * the advisory rendering pack.
 */

#include <stdint.h>

typedef void (*cardgame_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_cb4_t)(int32_t, int32_t, int32_t, int32_t);

int32_t CARDGAME_F0x8008db7c(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t ret = 0;
    int32_t v1 = *(int32_t *)(p1 + 0x424);

    if (v1 == 0)
        goto store;
    if (v1 == 1)
        goto check;
store:
    {
        int32_t off = p3 * 76;
        int32_t addrA = p2 + off;

        *(int32_t *)(p1 + 0x424) = 1;
        *(int16_t *)(addrA + 0x12e) = 1;
        ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xf08))(p2, p3, 15, -0x5000, 0x6100);
        ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xf20))(p2, p3, 0x1200, 0x1200);
    }
    goto done;
check:
    {
        int32_t addr = p2 + p3 * 76;

        if (*(uint8_t *)(addr + 0x14a) == 1) {
            *(int16_t *)(addr + 0x12e) = 0;
            *(int16_t *)(addr + 0x120) = 0;
            ret = 1;
        }
    }
done:
    return ret;
}
