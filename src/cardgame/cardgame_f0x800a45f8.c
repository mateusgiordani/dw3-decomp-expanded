/*
 * CARDGAME:0x800a45f8 CARDGAME_F0x800a45f8
 * 812 bytes at CARDGAME.PRO offset 0x21948 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a45f8, jump table (.rodata) at 0x80083e14
 *  Symbols     CARDGAME_F0x800a3d90=0x800a3d90 CARDGAME_F0x800a3e9c=0x800a3e9c
 *              CARDGAME_F0x800a3f48=0x800a3f48 CARDGAME_F0x800a4078=0x800a4078
 *              CARDGAME_F0x800a41a8=0x800a41a8 CARDGAME_F0x800a41f8=0x800a41f8
 *              CARDGAME_F0x800a4440=0x800a4440 CARDGAME_F0x800a4494=0x800a4494
 *  Compare     812 bytes from 0x800a45f8 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a45f8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x30 (s0-s6, ra).
 *
 * Resolves the effect of the active slot (1-based byte at p1+0x575, 8-byte
 * records at p1+0x580) by its kind byte at +0x585 (8-entry jump table at
 * 0x80083e14). p4 selects the side: it swaps which of p2/p3 feeds
 * CARDGAME_F0x800a4440 and CARDGAME_F0x800a4494. Returns 1 when applied.
 *
 * Matching notes: every case keeps its own `if (v) ret = 1` tail and leaves
 * through the single `return ret` (per-case tails stop the identical case 1 /
 * case 3 bodies from being cross-jumped; one return keeps ret's priority below
 * idx and a). Offsets are block-local and added base-first.
 */

#include <stdint.h>

int32_t CARDGAME_F0x800a3d90(int32_t p1, int32_t side);
int32_t CARDGAME_F0x800a3e9c(int32_t p1, int32_t side);
int32_t CARDGAME_F0x800a3f48(int32_t p1, int32_t side);
int32_t CARDGAME_F0x800a4078(int32_t p1, int32_t side);
int32_t CARDGAME_F0x800a41a8(int32_t p1, int32_t side, int32_t idx, int32_t value);
int32_t CARDGAME_F0x800a41f8(int32_t p1, int32_t side, int32_t value, int32_t arg);
int32_t CARDGAME_F0x800a4440(int32_t p1, int32_t card);
int32_t CARDGAME_F0x800a4494(int32_t p1, int32_t card);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define SLOT_U8(o)   ({ int32_t k_ = S8(p1, 0x575) - 1; int32_t s_ = k_ * 8; U8(p1 + s_, (o)); })

int32_t CARDGAME_F0x800a45f8(int32_t p1, int32_t p2, int32_t p3, int32_t p4)
{
    int32_t ret;
    int32_t idx;
    int32_t a;
    int32_t b;
    int32_t v;
    int32_t i;

    idx = 0;
    ret = 0;
    if (p4 == 0) {
        a = CARDGAME_F0x800a4440(p1, p3);
        b = CARDGAME_F0x800a4494(p1, p2);
    } else {
        a = CARDGAME_F0x800a4440(p1, p2);
        b = CARDGAME_F0x800a4494(p1, p3);
    }
    switch (SLOT_U8(0x585)) {
    case 0:
        for (i = 0; i < 12; i++) {
            if (i < 6) {
                if (SLOT_U8(0x586) == U8(p1 + i * 0xe, 0x73a)) {
                    idx = i;
                    break;
                }
            } else {
                int32_t o = (i - 6) * 0xe;

                if (SLOT_U8(0x586) == U8(p1 + o, 0x7ac)) {
                    idx = i - 6;
                    break;
                }
            }
        }
        if (p4 == 0) {
            if (CARDGAME_F0x800a41a8(p1, 0, idx, a) == 0)
                break;
            ret = 1;
            {
                int32_t d = S8(p1, 0x575) * 8;
                int32_t o = idx * 0xe;

                U8(p1 + d, 0x586) = U8(p1 + o, 0x73a);
            }
        } else {
            if (CARDGAME_F0x800a41a8(p1, 1, idx, a) == 0)
                break;
            if (CARDGAME_F0x800a41a8(p1, 1, idx, a - b) != 0)
                break;
            ret = 1;
            {
                int32_t d = S8(p1, 0x575) * 8;
                int32_t o = idx * 0xe;

                U8(p1 + d, 0x586) = U8(p1 + o, 0x7ac);
            }
        }
        break;
    case 1:
        if (p4 != 0)
            break;
        CARDGAME_F0x800a41f8(p1, 0, a, 0);
        CARDGAME_F0x800a3d90(p1, 0);
        v = CARDGAME_F0x800a3e9c(p1, 0);
        if (v != 0)
            ret = 1;
        break;
    default:
        break;
    case 3:
        if (p4 == 0) {
            CARDGAME_F0x800a41f8(p1, 0, a, 0);
            CARDGAME_F0x800a3d90(p1, 0);
            v = CARDGAME_F0x800a3e9c(p1, 0);
            if (v != 0)
                ret = 1;
            break;
        }
        if (CARDGAME_F0x800a41f8(p1, 1, a, 0) == 0)
            break;
        v = CARDGAME_F0x800a3e9c(p1, 1);
        if (v != 0)
            ret = 1;
        break;
    case 6:
        if (p4 != 0)
            break;
        CARDGAME_F0x800a41f8(p1, 0, a, 0);
        CARDGAME_F0x800a3d90(p1, 0);
        v = CARDGAME_F0x800a3f48(p1, 0);
        if (v != 0)
            ret = 1;
        break;
    case 7:
        if (p4 == 0)
            break;
        CARDGAME_F0x800a3d90(p1, 1);
        if (CARDGAME_F0x800a41f8(p1, 1, a, 0) == 0)
            break;
        v = CARDGAME_F0x800a4078(p1, 1);
        if (v != 0)
            ret = 1;
        break;
    }
    return ret;
}
