/*
 * STCRDDEK:0x800837b4 STCRDDEK_func_800837b4
 * 272 bytes at STCRDDEK.PRO offset 0xb04 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800837b4
 *  Symbols     D0x80048D34=0x80048d34 F0x8001ebf8=0x8001ebf8
 *              STCRDDEK_func_800837b4=0x800837b4
 *  Compare     272 bytes from 0x800837b4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800837b4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Decompile hypothesis + disasm: counter at +0x5c pre-incremented; if >= 0x29,
 * state at +0xc = 2 and counter reset to 0x28, else one deck-grid step: EXE
 * helper fills a stack callback block, then 5 indirect calls driven by an int16
 * card id from an EXE table, two (0x140,0x100)/(0x300,0x100) pairs,
 * (counter-1)%9, flush.
 *
 * Callee: 1 direct jal to EXE 0x8001ebf8.
 *
 * Table: EXE halfword entries at 0x80048d34+0x63e (int16 card ids); the
 * lui/addiu pair is a relocated extern address (linker high-adjusted form), not
 * an immediate.
 *
 * No Ghidra state change.
 */

#include <stdint.h>

extern void F0x8001ebf8(void *);
extern int16_t D0x80048D34[];

void STCRDDEK_func_800837b4(int32_t *p)
{
    uint32_t cb[22];
    int16_t *tbase;
    int32_t *sub;
    int32_t idx;
    int32_t stride;
    int32_t col;
    int16_t card;
    void (*cb0)(int32_t);
    void (*cb1)(void);
    void (*cb2)(int32_t, int32_t);
    void (*cb3)(int32_t, int32_t);
    void (*cb4)(int32_t, int32_t);

    if (++p[23] >= 0x29) {
        p[3] = 2;
        p[23] = 0x28;
    } else {
        F0x8001ebf8(cb);
        tbase = D0x80048D34;
        sub = (int32_t *)p[20];
        idx = p[23] - 1;
        stride = sub[23];
        card = *(int16_t *)((uint8_t *)tbase + (stride * 51 + idx) * 2 + 0x63e);
        cb0 = (void (*)(int32_t))cb[11];
        cb0(card);
        cb2 = (void (*)(int32_t, int32_t))cb[15];
        cb2(0x140, 0x100);
        cb3 = (void (*)(int32_t, int32_t))cb[16];
        cb3(0x300, 0x100);
        col = p[23] - 1;
        cb4 = (void (*)(int32_t, int32_t))cb[17];
        cb4(col % 9, col / 9);
        cb1 = (void (*)(void))cb[12];
        cb1();
    }
}
