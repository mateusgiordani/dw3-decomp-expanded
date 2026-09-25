/*
 * CARDGAME:0x8009b76c CARDGAME_F0x8009b76c
 * 292 bytes at CARDGAME.PRO offset 0x18abc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009b76c
 *  Symbols     CARDGAME_F0x8009bac8=0x8009bac8 DAT_80044f5c=0x80044f5c
 *              D_80044f5c=0x80044f5c F0x8001f648=0x8001f648
 *  Compare     292 bytes from 0x8009b76c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009b76c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdff48 addiu sp,-0xb8 ; sw s0,0xb0(sp) ; move
 * s0,a1 ; sw ra,0xb4(sp) ; epilogue lw ra,0xb4(sp) ; lw s0,0xb0(sp) ; jr ra ;
 * addiu sp,+0xb8. Next function CARDGAME:0x8009b890 at +0x124 confirms size
 * 0x124 contiguous (prologue/prologue pair).
 *
 * The guard stays combined && (reproduces beq/li-delay/lbu/bne). There is no
 * w6==0x10001000 interior guard here, so all six callbacks run.
 */

#include <stdint.h>

extern int32_t (*D_80044f5c)(int32_t a0);
extern void F0x8001f648(void *a0);

void CARDGAME_F0x8009b76c(int32_t p1, int32_t p2)
{
    uint8_t buf[0xa0];
    int32_t r;

    (void)p1;
    if (*(uint8_t *)(p2 + 0x45) != 0 && *(uint8_t *)(p2 + 0x42) == 11) {
        F0x8001f648(buf);
        ((void (*)(int32_t))*(void **)(buf + 0x88))((*(int32_t *)(p2 + 0x2c) / 2) % 5);
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x94))((*(int32_t *)p2 >> 8) + 0x14, (*(int32_t *)(p2 + 4) >> 8) + 0x17);
        ((void (*)(int32_t, int32_t, int32_t))*(void **)(buf + 0x8c))(*(int16_t *)(p2 + 0x18), *(int16_t *)(p2 + 0x1a), 0x1000);
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x7c))(0x100, 1);
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x74))(0x340, 0);
        r = D_80044f5c(0x25d0003);
        ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(buf + 0x84))(r, 9, *(int32_t *)p2 >> 8, *(int32_t *)(p2 + 4) >> 8);
    }
}
