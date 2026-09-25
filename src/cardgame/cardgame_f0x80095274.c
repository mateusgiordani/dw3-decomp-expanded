/*
 * CARDGAME:0x80095274 CARDGAME_F0x80095274
 * 128 bytes at CARDGAME.PRO offset 0x125c4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80095274
 *  Symbols     (none)
 *  Compare     128 bytes from 0x80095274 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80095274
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next framed CARDGAME:0x8009535c at +0xe8 (gap 0x68 leaf/data), prev
 * CARDGAME:0x8009518c size 0x7c ends 0x80095208 (gap 0x6c to this start); size
 * 0x80 contiguous, no overlap.
 *
 * No Ghidra state change.
 *
 * Callers: 2 direct jal (PAL bin scan for 0x0c02549d at file-off
 * 0x18cc/0x18e4).
 *
 * Callee: 1 indirect jalr via lw v0,0xec8(a0) after move a0,a1 (a0=q at call).
 *
 * Signedness duality at +0x575 is compiler-observed and required for matching:
 * first read is lb (signed index math), post-call decrement reload is lbu
 * (unsigned) followed by addiu -1 and sb. Both views kept; no struct invented.
 *
 * Delay slots (move a0,a1 / li v0,2 / sb v1,0x575(s0)) are compiler-scheduled
 * from this exact C shape; passing q to the indirect callee forces the early
 * move.
 *
 * Indirect callee stored at q+0xec8; called with q as first argument (a0).
 */

typedef void (*cardgame_80095274_cb_t)(void *arg);

void CARDGAME_F0x80095274(unsigned char *p, unsigned char *q, int r) {
    signed char idx = ((signed char *)p)[0x575];
    unsigned char t = *(unsigned char *)(p + ((idx - 1) << 3) + 0x584);

    if (t == 0 || r != 0) {
        ((cardgame_80095274_cb_t *)(q + 0xec8))[0](q);
        p[0x49d] = 1;
        p[0x499] = 1;
        p[0x422] = 1;
        p[0x575]--;
    } else {
        p[0x422] = 2;
    }
}
