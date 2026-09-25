/*
 * CARDGAME:0x80096950 CARDGAME_F0x80096950
 * 324 bytes at CARDGAME.PRO offset 0x13ca0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80096950
 *  Symbols     CARDGAME_F0x80096c20=0x80096c20 CARDGAME_F0x8009bba8=0x8009bba8
 *              DAT_80044f5c=0x80044f5c DAT_8004df9c=0x8004df9c
 *              F0x8001f648=0x8001f648 exe_f0x8001f648=0x8001f648
 *  Compare     324 bytes from 0x80096950 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80096950
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdff40 addiu sp,-0xc0; epilogue jr ra / 27bd00c0
 * addiu sp,+0xc0.
 *
 * Prev ends at 0x80096950 (gap 0).
 *
 * (buf+0x7c)(0x100,2); (buf+0x74)(0x340,0); mode = byte at p1+0xE9E selects via
 * switch: 0 -> sel stays 0; 1 -> stepped path (if byte at p1+0xE9D >= 4: that
 * byte -= 4, byte at p1+0xE9C += 1 with 0xB clamp that also forces the mode
 * byte to 2; sel = byte at p1+0xE9C; byte at p1+0xE9D += DAT_8004df9c()); 2 ->
 * sel = 0xB; default -> sel stays 0.
 *
 * (buf+0x88)(sel); idx = (*(p1+0x58) >> 1) & 0x3F (srl, logical); svc =
 * DAT_80044f5c(0x25D0003); (buf+0x84)(svc,0,idx,idx). Caller passes only a0.
 *
 * Dispatch shape note: the three-case switch (0/1/2 + default) is what makes
 * gcc-2.8.1 -O2 emit PAL's exact dispatch head: beq-to-A with the signed slti
 * range check in its delay slot, bne-to-merge, then beq-forward to the
 * out-of-line 'li s1,0xB' select with j-to-merge. Two-case switch/if-else-if
 * spellings collapse to a fused bne select and lose the range check (316 B). No
 * Ghidra mutation.
 */

#include <stdint.h>

extern void F0x8001f648(void *buf);
extern int32_t (*DAT_8004df9c)(void);
extern uint32_t (*DAT_80044f5c)(uint32_t);

void CARDGAME_F0x80096950(int32_t p1)
{
    uint8_t buf[0xA0];
    int mode;
    int32_t elapsed;
    int32_t sel;
    uint32_t idx;
    uint32_t svc;

    F0x8001f648(buf);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x7C))(0x100, 2);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x74))(0x340, 0);
    mode = *(uint8_t *)(p1 + 0xE9E);
    sel = 0;
    switch (mode) {
    case 0:
        break;
    case 1:
        if (*(uint8_t *)(p1 + 0xE9D) >= 4) {
            uint8_t b;

            *(uint8_t *)(p1 + 0xE9D) -= 4;
            b = (uint8_t)(*(uint8_t *)(p1 + 0xE9C) + 1);
            *(uint8_t *)(p1 + 0xE9C) = b;
            if (b >= 0xB) {
                *(uint8_t *)(p1 + 0xE9C) = 0xB;
                *(uint8_t *)(p1 + 0xE9E) = 2;
            }
        }
        sel = *(uint8_t *)(p1 + 0xE9C);
        elapsed = DAT_8004df9c();
        *(uint8_t *)(p1 + 0xE9D) += (uint8_t)elapsed;
        break;
    case 2:
        sel = 0xB;
        break;
    default:
        break;
    }
    ((void (*)(int32_t))*(void **)(buf + 0x88))(sel);
    idx = (*(uint32_t *)(p1 + 0x58) >> 1) & 0x3F;
    svc = DAT_80044f5c(0x25D0003u);
    ((void (*)(uint32_t, int32_t, uint32_t, uint32_t))*(void **)(buf + 0x84))(svc, 0, idx, idx);
}
