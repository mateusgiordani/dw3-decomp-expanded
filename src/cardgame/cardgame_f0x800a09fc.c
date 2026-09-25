/*
 * CARDGAME:0x800a09fc CARDGAME_F0x800a09fc
 * 208 bytes at CARDGAME.PRO offset 0x1dd4c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a09fc
 *  Symbols     CARDGAME_F0x800a0844=0x800a0844
 *  Compare     208 bytes from 0x800a09fc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a09fc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Revision 6 source shape (GCC 2.8.1 SN32 / ASPSX 2.79 O2, exact): - the record
 * loop is a for loop: GCC copies its test to the entry (beqz on *pb) and the
 * loop optimizer places result = 1 and the stepped record offset (li s1,2) in
 * the pre-header. In the measured dumps inner then has 18 loop-weighted
 * references and the offset is a loop-created register with 15, so inner is
 * allocated first and takes s0; - the record address is written &pb[inner * 14
 * + 2] so the stepped register holds the offset (addu a1,s2,s1), not a pointer.
 */

#include <stdint.h>

extern void CARDGAME_F0x800a0844(void *p, void *q, int a2, int a3);

int CARDGAME_F0x800a09fc(void *a0)
{
    unsigned char *p = (unsigned char *)a0;
    int result = 0;
    int outer = 0;
    int middle;
    int off;
    unsigned char *pb;
    int inner;

    result = 0;
    outer = 0;
    do {
        middle = 0;
        off = 0x72c;
        do {
            pb = p + off;
            for (inner = 0; inner < *pb; inner++) {
                result = 1;
                CARDGAME_F0x800a0844(p, &pb[inner * 14 + 2], middle, outer);
            }
            middle++;
            off += 0x72;
        } while (middle < 2);
        outer++;
    } while (outer < 5);
    return result;
}
