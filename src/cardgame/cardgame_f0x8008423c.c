/*
 * CARDGAME:0x8008423c CARDGAME_F0x8008423c
 * 228 bytes at CARDGAME.PRO offset 0x158c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008423c, jump table (.rodata) at 0x80082cc8
 *  Symbols     CARDGAME_F0x8008423c=0x8008423c CARDGAME_F0x8008f2ec=0x8008f2ec
 *  Compare     228 bytes from 0x8008423c and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008423c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * rodata 44/44 exact via pipeline. int-b fix this rev (sltiu->slti x3,
 * semantics kept: b in [0,255] compares identically signed): pipeline 41->38
 * diff bytes.
 *
 * Prev framed CARDGAME:0x80084188 size 0xb4 ends exactly at 0x8008423c (no
 * gap); next framed CARDGAME:0x80084320 at +0xe4 (no gap).
 *
 * Decompile hypothesis matches this C shape (switch 0x1e..0x28, two table
 * paths).
 *
 * No Ghidra state change.
 *
 * Callers: 1 direct jal (PAL bin scan for 0c02108f at file-off 0x2608 = vaddr
 * 0x800852b8).
 *
 * Callees: 1 direct jal CARDGAME_F0x8008f2ec(a0 passthrough, a1 passthrough, a2
 * = table byte or table byte ^ 1 or 0, a3 = switch value 0..4).
 *
 * Jump table: 11 words at 0x80082cc8 (values 30..40 -> a3
 * 0,1,2,2,3,4,0,1,2,3,4).
 *
 * Chunk-aware re-link of the same .obj is byte-exact.
 */

extern void CARDGAME_F0x8008f2ec(void *p, unsigned int q, unsigned int r, unsigned int s);

void CARDGAME_F0x8008423c(unsigned char *p, unsigned int q) {
    unsigned int ctl = 0;
    unsigned int sel = ctl;

    switch (p[0x420]) {
    case 30:
    case 36:
        sel = 0;
        break;
    case 31:
    case 37:
        sel = 1;
        break;
    case 32:
    case 33:
    case 38:
        sel = 2;
        break;
    case 34:
    case 39:
        sel = 3;
        break;
    case 35:
    case 40:
        sel = 4;
        break;
    }

    {
        int b = p[0x420];

        if (b >= 30) {
            if (b < 36) {
                ctl = *(unsigned char *)(p + ((((signed char *)p)[0x575] - 1) << 3) + 0x584);
            } else if (b < 41) {
                ctl = *(unsigned char *)(p + ((((signed char *)p)[0x575] - 1) << 3) + 0x584) ^ 1;
            }
        }
    }

    CARDGAME_F0x8008f2ec(p, q, ctl, sel);
}
