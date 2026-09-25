/*
 * CARDGAME:0x8009c214 CARDGAME_F0x8009c214
 * 72 bytes at CARDGAME.PRO offset 0x19564 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c214
 *  Symbols     CARDGAME_F0x8009d310=0x8009d310 DAT_80055c48=0x80055c48
 *  Compare     72 bytes from 0x8009c214 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c214
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffe8 addiu sp,-0x18 ; sw s0,0x10(sp) ; addu s0,a0 ; lui
 * a0,0x8004 ; lui v0,0x8005 ; lw v0,0x5c48(v0) ; sw ra,0x14(sp) ; jalr v0 ; ori
 * a0,0x503c delay
 *
 * Body: addiu v0,10 ; sh v0,0xe02(s0) ; sh v0,0xe00(s0) ; addiu v0,4 ; sh
 * v0,0xe0a(s0) ; lw ra,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,+0x18
 *
 * Next bytes CARDGAME:0x8009c25c at +0x48 (dispatch-table LAB slot +0xf00 in
 * CARDGAME_F0x8009d310), confirms size 0x48 is self-contained
 * (prologue/epilogue pair).
 *
 * DATA x-ref: table installer CARDGAME_F0x8009d310 stores &CARDGAME_F0x8009c214
 * at slot +0xefc (callback family with 0x8009c054/0x8009c0dc/0x8009c1cc).
 *
 * Callee: indirect jalr via word at 0x80055c48 (EXE-resident vector, -G0
 * absolute lui/lw); arg 0x8004503c is a 32-bit constant (lui 0x8004 + ori
 * delay).
 */

#include "common/types.h"

typedef void (*cardgame_svc_fn)(uint32_t arg);
extern cardgame_svc_fn DAT_80055c48;

void CARDGAME_F0x8009c214(uint8_t *work)
{
    DAT_80055c48(0x8004503cu);
    *(int16_t *)(work + 0xe02) = 10;
    *(int16_t *)(work + 0xe00) = 10;
    *(int16_t *)(work + 0xe0a) = 4;
}
