/*
 * CARDGAME:0x80096180 CARDGAME_F0x80096180
 * 112 bytes at CARDGAME.PRO offset 0x134d0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80096180
 *  Symbols     CARDGAME_F0x80095fb4=0x80095fb4 CARDGAME_F0x80096144=0x80096144
 *              CARDGAME_F0x80096150=0x80096150 CARDGAME_F0x80096160=0x80096160
 *              CARDGAME_F0x8009f110=0x8009f110 EXE_F0x80014504=0x80014504
 *  Compare     112 bytes from 0x80096180 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80096180
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue addiu sp,-0x20, epilogue jr ra / addiu sp,+0x20.
 *
 * Next CARDGAME:0x800961f0 at +0x70; prev CARDGAME_F0x80095fb4 ends at
 * 0x80096144 with a 0x3C gap holding leaf handlers at
 * 0x80096144/0x80096150/0x80096160.
 *
 * Stored here as handler function pointers.
 *
 * Portable C recovery (campaign cardgame-human-v1 rev 2): replaces the rev-1
 *
 * __asm__ noreorder wrapper, which is not portable C. Store order below follows
 * PAL bytes: +0x68, +0x6c, +0x70, +0x54, +0x56, +0x63.
 *
 * Arg widths: PAL uses sh for both args, so only the low half is stored;
 * signedness is unproven, int16_t is the conservative halfword reading.
 */

#include <stdint.h>

extern void CARDGAME_F0x80095fb4(void);
extern void CARDGAME_F0x80096144(void);
extern void CARDGAME_F0x80096150(void);
extern void CARDGAME_F0x80096160(void);
extern void *EXE_F0x80014504(void *tpl, int32_t size, int32_t flag);

void CARDGAME_F0x80096180(int16_t a0, int16_t a1) {
    uint8_t *obj;

    obj = (uint8_t *)EXE_F0x80014504((void *)CARDGAME_F0x80095fb4, 0x74, 0);
    *(void **)(obj + 0x68) = (void *)CARDGAME_F0x80096144;
    *(void **)(obj + 0x6c) = (void *)CARDGAME_F0x80096160;
    *(void **)(obj + 0x70) = (void *)CARDGAME_F0x80096150;
    *(uint16_t *)(obj + 0x54) = (uint16_t)a0;
    *(uint16_t *)(obj + 0x56) = (uint16_t)a1;
    *(obj + 0x63) = 0;
}
