/*
 * STCRDDEK:0x800834e0 STCRDDEK_func_800834e0
 * 180 bytes at STCRDDEK.PRO offset 0x830 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800834e0
 *  Symbols     STCRDDEK_func_8008339c=0x8008339c
 *  Compare     180 bytes from 0x800834e0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800834e0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Behavior (conservative): dispatch on word at +0xc. State 1: if word at +0x10
 * is zero return; else add word at +0x60 into word at +0x5c, store the sum
 * unconditionally, then clamp by direction word at +0x58: direction zero with
 * sum > 0xff00 sets state 2 and stores 0xff00; nonzero direction with sum < 0
 * sets state 2 and stores 0; the state word is written once at the join through
 * the state local (PAL duplicates li v0,2 into both branch delay slots with a
 * single shared sw at the join); then call STCRDDEK_func_8008339c. State 2:
 * call STCRDDEK_func_8008339c. State 3: return without calling. Any other
 * state: indirect call *(arg0+0x38)(arg0), then return. Struct layout beyond
 * these six words is unrecovered, so the object stays int32_t *. Source uses
 * forward gotos so the generator keeps the PAL block order (dispatch inline,
 * indirect block, state-1 block, shared tail call) including the slti <2 test
 * (PAL 28620002 in the first beq delay slot).
 */

#include "common/types.h"

extern void STCRDDEK_func_8008339c(int32_t *arg0);

void STCRDDEK_func_800834e0(int32_t *arg0) {
    int32_t state;
    int32_t sum;
    int32_t next;

    state = arg0[3];
    if (state == 1) {
        goto state1;
    }
    if (state < 2) {
        goto indirect;
    }
    if (state == 2) {
        goto tail;
    }
    if (state == 3) {
        return;
    }
indirect:
    ((void (*)(int32_t *))arg0[14])(arg0);
    return;
state1:
    if (arg0[4] == 0) {
        return;
    }
    sum = arg0[23] + arg0[24];
    arg0[23] = sum;
    if (arg0[22] != 0) {
        goto negdir;
    }
    if (sum <= 0xFF00) {
        goto tail;
    }
    next = 2;
    arg0[23] = 0xFF00;
    goto set2;
negdir:
    if (sum >= 0) {
        goto tail;
    }
    next = 2;
    arg0[23] = 0;
set2:
    arg0[3] = next;
tail:
    STCRDDEK_func_8008339c(arg0);
}
