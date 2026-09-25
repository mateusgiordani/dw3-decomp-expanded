/*
 * CARDGAME:0x8009af28 CARDGAME_F0x8009af28
 * 256 bytes at CARDGAME.PRO offset 0x18278 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009af28
 *  Symbols     CARDGAME_F0x8009ba3c=0x8009ba3c D_80044f5c=0x80044f5c
 *              F0x8001f648=0x8001f648 FIGHTSTG_F0x80094878=0x80094878
 *              func_0x8001f648=0x8001f648
 *  Compare     256 bytes from 0x8009af28 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009af28
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Xref: 1 caller CARDGAME_F0x8009ba3c via 0x8009ba88 jal; 1 direct callee EXE
 * 0x8001f648 plus 6 computed jalr (5 stack-table slots + EXE data word).
 *
 * No Ghidra state change.
 *
 * Stack table family matches FIGHTSTG_F0x80094878 portable pattern:
 *
 * F0x8001f648(buf) fills uint8_t buf[0xa0] at sp+0x10, fn slots at
 * buf+0x74/0x7c/0x84/0x8c/0x94 (sp+0x84/0x8c/0x94/0x9c/0xa4).
 */

#include <stdint.h>

extern int32_t (*D_80044f5c)(int32_t a0);
extern void F0x8001f648(void *a0);

void CARDGAME_F0x8009af28(int32_t p1, int32_t p2)
{
    uint8_t buf[0xa0];
    int32_t r;

    (void)p1;
    if (*(uint8_t *)(p2 + 0x45) == 0) {
        return;
    }
    if (*(uint8_t *)(p2 + 0x46) == 0) {
        return;
    }
    F0x8001f648(buf);
    if (*(int32_t *)(p2 + 0x18) != 0x10001000) {
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x94))((*(int32_t *)p2 >> 8) + 0x14, (*(int32_t *)(p2 + 4) >> 8) + 0x17);
        ((void (*)(int32_t, int32_t, int32_t))*(void **)(buf + 0x8c))(*(int16_t *)(p2 + 0x18), *(int16_t *)(p2 + 0x1a), 0x1000);
    }
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x7c))(0x100, 1);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x74))(0x280, 0);
    r = D_80044f5c(0x25d0002);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(buf + 0x84))(r, *(uint8_t *)(p2 + 0x46) + 0x33, (*(int32_t *)p2 >> 8) + 3, (*(int32_t *)(p2 + 4) >> 8) + 2);
}
