/*
 * CARDGAME:0x80099de8 CARDGAME_F0x80099de8
 * 236 bytes at CARDGAME.PRO offset 0x17138 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80099de8
 *  Symbols     EXE_D_80044f5c=0x80044f5c EXE_F0x8001f648=0x8001f648
 *  Compare     236 bytes from 0x80099de8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80099de8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * EXE_F0x8001f648=0x8001f648 EXE_D_80044f5c=0x80044f5c.
 *
 * Boundary (sweep #132, HIGH): prologue 27bdff48 addiu sp,-0xb8, epilogue
 * 03e00008 jr ra + 27bd00b8 addiu sp,+0xb8; prev CARDGAME:0x80099d20 ends
 * exactly at 0x80099de8 (0x80099d20+0xc8), next CARDGAME:0x80099ed4 starts
 * exactly at 0x80099de8+0xec. Frame -0xb8/+0xb8 verified.
 *
 * Recovery notes: the 0xa0-byte stack block (blk at sp+0x10) reproduces the PAL
 * 184-byte frame with callback slots at sp+0x84/0x8c/0x94/0x98. The select
 * logic is spelled with gotos (set3/mid/set1/funnel) to reproduce PAL's block
 * layout: q==1 -> sel 3; q==2 -> sel 1; else sel 0 with param[6] = 0 unless q
 * == 0. Semantics identical to the if-chain form; the goto spelling exists only
 * to match the original's branch lowering.
 */

#include <stdint.h>

extern void EXE_F0x8001f648(void *work);
extern void *EXE_D_80044f5c;

typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb1_t)(int32_t a0);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*exe_svc_t)(int32_t a0);

/* Stack block hypothesis: 0x74-byte work area filled by EXE_F0x8001f648, */
/* followed by callback slots. Exact inter-slot gaps (0x84/0x8c/0x94/0x98) */

typedef struct {
    uint8_t work[0x74];
    cardgame_cb2_t cb340;
    uint32_t slot_78;
    cardgame_cb2_t cb100;
    uint8_t pad[4];
    cardgame_cb4_t cb_last;
    cardgame_cb1_t cb_mid;
    uint8_t tail[0x14];
} cardgame_99de8_block_t;

void CARDGAME_F0x80099de8(int16_t *param) {
    cardgame_99de8_block_t blk;
    int32_t sel;
    int32_t q;
    int16_t prev;
    int16_t next;

    EXE_F0x8001f648(blk.work);
    blk.cb100(0x100, 1);
    blk.cb340(0x340, 0);

    prev = param[6];
    next = (int16_t)(prev + 1);
    param[6] = next;
    q = ((int32_t)(next << 16)) >> 18;

    if (q == 1)
        goto set3;
    if (q < 2) {
        if (q == 0)
            goto mid;
        param[6] = 0;
        goto mid;
    }
    if (q == 2)
        goto set1;
    param[6] = 0;
mid:
    sel = 0;
    goto funnel;
set3:
    sel = 3;
    goto funnel;
set1:
    sel = 1;
funnel:
    blk.cb_mid(sel);
    sel = ((exe_svc_t)EXE_D_80044f5c)(0x025D0003);
    blk.cb_last(sel, 8, (int32_t)param[0], (int32_t)param[1]);
}
