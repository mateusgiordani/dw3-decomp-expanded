/*
 * CARDGAME:0x80097238 CARDGAME_F0x80097238
 * 216 bytes at CARDGAME.PRO offset 0x14588 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80097238
 *  Symbols     CARDGAME_F0x80097508=0x80097508
 *  Compare     216 bytes from 0x80097238 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80097238
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next function CARDGAME:0x80097310 (addiu sp,-0x20) confirms size 0xD8
 * contiguous.
 *
 * Callers: CARDGAME_F0x80097508 -> jal 0x80097238 at 0x80097748
 * (UNCONDITIONAL_CALL). Callees: 3 indirect jalr per iteration (vtable at
 * +0x134/+0x118/+0x148 from object at *(cursor+4)).
 *
 * Exact toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0
 * -fno-strength-reduce.
 *
 * The late local cursor initialization reproduces PAL prologue order while the
 * pinned no-strength-reduce variant preserves the base+4 loads. Full 216-byte
 * match.
 */

#include <stdint.h>

typedef void (*cardgame_cb3_t)(int32_t, int32_t, int32_t);
typedef void (*cardgame_cb2_t)(int32_t, int32_t);

void CARDGAME_F0x80097238(int32_t arg0, int32_t *input, uint8_t *rec)
{
    uint32_t slots[2];
    int32_t i;
    int32_t k;
    int32_t *cursor;
    (void)arg0;
    i = 0;
    slots[0] = rec[0x14];
    k = 4;
    slots[1] = rec[0x15];
    cursor = input;
    do {
        int32_t off;
        int32_t a1v;
        int32_t a2v;
        int32_t o0;
        int32_t o1;
        int32_t o2;
        off = i << 2;
        i += 1;
        o0 = cursor[1];
        a1v = (int32_t)(int16_t)(*(uint16_t *)(rec + 0) + 0x67);
        a2v = (int32_t)(int16_t)(*(uint16_t *)(rec + 2) + k);
        ((cardgame_cb3_t)*(int32_t *)(o0 + 0x134))(o0, a1v, a2v);
        k += 13;
        o1 = cursor[1];
        ((cardgame_cb3_t)*(int32_t *)(o1 + 0x118))(o1, 0, *(int32_t *)((uint8_t *)slots + off));
        o2 = cursor[1];
        ((cardgame_cb2_t)*(int32_t *)(o2 + 0x148))(o2, 1);
        cursor += 1;
    } while (i < 2);
}
