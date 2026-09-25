/*
 * CARDGAME:0x8009b168 CARDGAME_F0x8009b168
 * 1296 bytes at CARDGAME.PRO offset 0x184b8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009b168
 *  Symbols     CARDGAME_F0x80096a94=0x80096a94 CARDGAME_F0x8009a82c=0x8009a82c
 *              CARDGAME_F0x8009b168=0x8009b168 DAT_80044f5c=0x80044f5c
 *              EXE_F0x8001f648=0x8001f648
 *  Compare     1296 bytes from 0x8009b168 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009b168
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936, 1296-byte body.
 *
 * Measured GCC 2.8.1 / ASPSX 2.79, O2 base.
 */

#include "common/types.h"

typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*cardgame_svc_t)(uint32_t arg);

void EXE_F0x8001f648(void *buf);
extern cardgame_svc_t DAT_80044f5c;
void CARDGAME_F0x80096a94(void *desc, int32_t flag);
void CARDGAME_F0x8009a82c(void *rec);

typedef struct {
    uint32_t w[40];
} cardgame_b168_drawbuf_t;

typedef struct {
    uint16_t id;
    int16_t x;
    int16_t y;
    uint16_t w;
    uint16_t h;
    int16_t x2;
    int16_t y2;
    uint8_t f1;
    uint8_t f2;
    uint8_t kind;
} cardgame_b168_desc_t;

void CARDGAME_F0x8009b168(int32_t unused_a0, uint8_t *rec)
{
    int32_t kind;
    int32_t svc;
    /* PAL reuses [sp+0x10,sp+0xc8): descriptor at +0, then a 160-byte
     * draw context at +24; other phases initialize the same workspace at +0. */
    union {
        uint32_t words[46];
        cardgame_b168_desc_t desc;
    } phase_work;
    cardgame_b168_drawbuf_t case2_work;
    cardgame_b168_drawbuf_t case3_work;
    if (rec[0x45] == 0) {
        return;
    }
    kind = rec[0x45];
    switch (kind) {
    case 0:
        return;
    case 1:
        if (*(int16_t *)(rec + 0x3c) != 0) {

            phase_work.desc.kind = kind;
            phase_work.desc.f1 = 2;
            phase_work.desc.f2 = 0;
            phase_work.desc.x = (int16_t)((*(int32_t *)(rec + 0x00) >> 8) + 4);
            phase_work.desc.y = (int16_t)((*(int32_t *)(rec + 0x04) >> 8) + 0x21);
            phase_work.desc.id = *(rec + 0x43);
            phase_work.desc.x2 = (int16_t)((*(int32_t *)(rec + 0x00) >> 8) + 0x14);
            phase_work.desc.y2 = (int16_t)((*(int32_t *)(rec + 0x04) >> 8) + 0x17);
            phase_work.desc.w = *(uint16_t *)(rec + 0x18);
            phase_work.desc.h = *(uint16_t *)(rec + 0x1a);
            CARDGAME_F0x80096a94(&phase_work.desc, 1);
            phase_work.desc.x = (int16_t)((*(int32_t *)(rec + 0x00) >> 8) + 0x17);
            phase_work.desc.y = (int16_t)((*(int32_t *)(rec + 0x04) >> 8) + 0x21);
            phase_work.desc.id = *(rec + 0x44);
            phase_work.desc.x2 = (int16_t)((*(int32_t *)(rec + 0x00) >> 8) + 0x14);
            phase_work.desc.y2 = (int16_t)((*(int32_t *)(rec + 0x04) >> 8) + 0x17);
            phase_work.desc.w = *(uint16_t *)(rec + 0x18);
            phase_work.desc.h = *(uint16_t *)(rec + 0x1a);
            CARDGAME_F0x80096a94(&phase_work.desc, 1);
            EXE_F0x8001f648(phase_work.words + 6);
            if (*(int32_t *)(rec + 0x18) != 0x10001000) {
                ((cardgame_cb2_t)(phase_work.words + 6)[37])((*(int32_t *)(rec + 0x00) >> 8) + 0x14, (*(int32_t *)(rec + 0x04) >> 8) + 0x17);
                ((cardgame_cb3_t)(phase_work.words + 6)[35])(*(int16_t *)(rec + 0x18), *(int16_t *)(rec + 0x1a), 0x1000);
            }
            ((cardgame_cb2_t)(phase_work.words + 6)[31])(0x100, 1);
            ((cardgame_cb2_t)(phase_work.words + 6)[29])(0x340, 0);
            svc = DAT_80044f5c(0x025D0003u);
            ((cardgame_cb4_t)(phase_work.words + 6)[33])(svc, 0x1f, (*(int32_t *)(rec + 0x00) >> 8) + 0x12, (*(int32_t *)(rec + 0x04) >> 8) + 0x21);
        } else {
            EXE_F0x8001f648(phase_work.words);
            if (*(int32_t *)(rec + 0x18) != 0x10001000) {
                ((cardgame_cb2_t)phase_work.words[37])((*(int32_t *)(rec + 0x00) >> 8) + 0x14, (*(int32_t *)(rec + 0x04) >> 8) + 0x17);
                ((cardgame_cb3_t)phase_work.words[35])(*(int16_t *)(rec + 0x18), *(int16_t *)(rec + 0x1a), 0x1000);
            }
            ((cardgame_cb2_t)phase_work.words[31])(0x100, 1);
            ((cardgame_cb2_t)phase_work.words[29])(0x340, 0);
            svc = DAT_80044f5c(0x025D0003u);
            ((cardgame_cb4_t)phase_work.words[33])(svc, 0x1e, (*(int32_t *)(rec + 0x00) >> 8) + 4, (*(int32_t *)(rec + 0x04) >> 8) + 0x21);
        }
        {
            EXE_F0x8001f648(phase_work.words);
            if (*(int32_t *)(rec + 0x18) != 0x10001000) {
                int32_t x = ((*(int32_t *)(rec + 0x00)) >> 8) + 0x14;
                int32_t y = ((*(int32_t *)(rec + 0x04)) >> 8) + 0x17;
                ((cardgame_cb2_t)phase_work.words[37])(x, y);
                ((cardgame_cb3_t)phase_work.words[35])(*(int16_t *)(rec + 0x18),
                                       *(int16_t *)(rec + 0x1a), 0x1000);
            }
            ((cardgame_cb2_t)phase_work.words[31])(0x100, 1);
            ((cardgame_cb2_t)phase_work.words[29])(0x280, 0);
            svc = DAT_80044f5c(0x025D0002u);
            {
                int32_t sub = *(int16_t *)(rec + 0x38);
                if (*(rec + 0x49) == 1) {
                    sub += 0x3c;
                }
                ((cardgame_cb4_t)phase_work.words[33])(svc, sub, *(int32_t *)(rec + 0x00) >> 8, *(int32_t *)(rec + 0x04) >> 8);
            }
            CARDGAME_F0x8009a82c(rec);
        }
        break;
    case 2:
        {
            EXE_F0x8001f648(case2_work.w);
            if (*(int32_t *)(rec + 0x18) != 0x10001000) {
                ((cardgame_cb2_t)case2_work.w[37])((*(int32_t *)(rec + 0x00) >> 8) + 0x14, (*(int32_t *)(rec + 0x04) >> 8) + 0x17);
                ((cardgame_cb3_t)case2_work.w[35])(*(int16_t *)(rec + 0x18), *(int16_t *)(rec + 0x1a), 0x1000);
            }
            ((cardgame_cb2_t)case2_work.w[31])(0x100, 1);
            ((cardgame_cb2_t)case2_work.w[29])(0x280, 0);
            svc = DAT_80044f5c(0x025D0002u);
            ((cardgame_cb4_t)case2_work.w[33])(svc, 0x10, (*(int32_t *)(rec + 0x00) >> 8), (*(int32_t *)(rec + 0x04) >> 8));
            break;
        }
    case 3:
        {
            EXE_F0x8001f648(case3_work.w);
            if (*(int32_t *)(rec + 0x18) != 0x10001000) {
                ((cardgame_cb2_t)case3_work.w[37])((*(int32_t *)(rec + 0x00) >> 8) + 0x14, (*(int32_t *)(rec + 0x04) >> 8) + 0x17);
                ((cardgame_cb3_t)case3_work.w[35])(*(int16_t *)(rec + 0x18), *(int16_t *)(rec + 0x1a), 0x1000);
            }
            ((cardgame_cb2_t)case3_work.w[31])(0x100, 1);
            ((cardgame_cb2_t)case3_work.w[29])(0x280, 0);
            svc = DAT_80044f5c(0x025D0002u);
            ((cardgame_cb4_t)case3_work.w[33])(svc, 0x0c, (*(int32_t *)(rec + 0x00) >> 8), (*(int32_t *)(rec + 0x04) >> 8));
            break;
        }
    }
}
