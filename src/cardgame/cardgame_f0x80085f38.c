/*
 * CARDGAME:0x80085f38 CARDGAME_F0x80085f38
 * 152 bytes at CARDGAME.PRO offset 0x3288 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085f38
 *  Symbols     CARDGAME_F0x80085f38=0x80085f38 func_0x8001ebf8=0x8001ebf8
 *  Compare     152 bytes from 0x80085f38 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80085f38
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 152-byte PAL body at verified base 0x80082cb0.
 *
 * Explicit index scaling and a pointer intermediate preserve original
 * allocation.
 */

#include <stdint.h>

/* EXE-resident helper (address outside the overlay range): fills the caller's
   stack buffer, including the two callback slots below. Conservative name. */
extern void func_0x8001ebf8(void *buf);

/* Callback slots recovered from PAL: lw v0,60(sp)/lw v0,96(sp) then jalr v0,
   with the buffer passed at sp+0x10, so offsets 0x2c and 0x50. */
typedef int32_t (*cardgame_5f38_cb_t)(int32_t arg);

typedef struct {
    uint8_t unk_00[0x2c];
    cardgame_5f38_cb_t fn_first; /* +0x2c (sp+0x3c) */
    uint8_t unk_30[0x20];
    int32_t (*fn_second)(void); /* +0x50 (sp+0x60) */
} cardgame_5f38_work_t;

int32_t CARDGAME_F0x80085f38(uint8_t *base, int32_t index) {
    cardgame_5f38_work_t work;
    int16_t hv;
    int32_t value;
    uint8_t *element;

    func_0x8001ebf8(&work);
    index *= 2;
    element = base + index;
    index = (int32_t)element;
    hv = *(int16_t *)(index + 0x50);
    index = hv + 1;
    work.fn_first(index);
    value = work.fn_second();
    /* Reuse index as the 0/1 result, mirroring PAL s0 reuse. */
    index = 0;
    if (value != 0) {
        if (base[0x2f8] == 7 || (base[0x2f8] == 5 && value == 2)) {
            index = 1;
        }
    }
    return index;
}
