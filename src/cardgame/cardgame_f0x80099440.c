/*
 * CARDGAME:0x80099440 CARDGAME_F0x80099440
 * 1376 bytes at CARDGAME.PRO offset 0x16790 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80099440, jump table (.rodata) at 0x80083730
 *  Symbols     CARDGAME_F0x80099440=0x80099440 DAT_8004DF9C=0x8004df9c
 *  Compare     1376 bytes from 0x80099440 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80099440
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936, base0x80082cb0.
 *
 * Exact1376 text bytes plus24-byte jump table at0x80083730.
 *
 * GCC2.8.1 SN32 4.0.0010 / ASPSX2.79, O2base, --strip-div-guard.
 *
 * Keep all twelve real endpoints distinct across the selector branches.
 *
 * Equal coordinates still have separate PAL subtraction/multiply/divide paths.
 *
 * Raw PS1 addresses keep halfword accesses outside the old compiler array alias
 * class, preserving stores before the tick-pointer load in cases4/5.
 *
 * Case3/4/5 resets intentionally repeat dx2 in the final field, as PAL does.
 */

#include <stdint.h>
typedef int32_t (*cardgame_tick_fn_t)(void);
extern int32_t DAT_8004DF9C;

void CARDGAME_F0x80099440(int32_t p, int32_t unused, int32_t sel)
{
    int32_t x0, y0, x1, y1, x2, y2;
    int32_t dx0, dy0, dx1, dy1, dx2, dy2;
    if (sel == 0) {
        x0=0; y0=0x8d; dx0=0; dy0=0xf1;
        x1=0x120; y1=0x8f; dx1=0x147; dy1=0x8f;
        x2=0x113; y2=0xbd; dx2=0x13a; dy2=0xbd;
    } else {
        x0=0; y0=0; dx0=0; dy0=-100;
        x1=0x120; y1=0x50; dx1=0x147; dy1=0x50;
        x2=0x113; y2=0x26; dx2=0x13a; dy2=0x26;
    }
    switch((*(int16_t *)(p + 6))) {
    case 0: break;
    case 1: (*(int16_t *)(p + 6))=0; break;
    case 2: (*(int16_t *)(p + 6))=0; break;
    }
    switch((*(int16_t *)(p + 4))) {
    case 1: {
        int32_t cnt;
        cnt = (*(int16_t *)(p + 0)) -= ((cardgame_tick_fn_t)DAT_8004DF9C)();
        if (cnt>0) {
            (*(int16_t *)(p + 12))=(int16_t)(x0-((x0-dx0)*cnt)/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 14))=(int16_t)(y0-((y0-dy0)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 32))=(int16_t)(x1-((x1-dx1)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 34))=(int16_t)(y1-((y1-dy1)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 60))=(int16_t)(x2-((x2-dx2)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 62))=(int16_t)(y2-((y2-dy2)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
        } else {
            (*(int16_t *)(p + 4))=2;
            (*(int16_t *)(p + 0))=0; (*(int16_t *)(p + 2))=0;
            (*(int16_t *)(p + 12))=(int16_t)x0;
            (*(int16_t *)(p + 14))=(int16_t)y0;
            (*(int16_t *)(p + 32))=(int16_t)x1;
            (*(int16_t *)(p + 34))=(int16_t)y1;
            (*(int16_t *)(p + 60))=(int16_t)x2;
            (*(int16_t *)(p + 62))=(int16_t)y2;
        }
        break;
    }
    case 3: {
        int32_t cnt;
        cnt = (*(int16_t *)(p + 0)) -= ((cardgame_tick_fn_t)DAT_8004DF9C)();
        if (cnt>0) {
            (*(int16_t *)(p + 12))=(int16_t)(dx0-((dx0-x0)*cnt)/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 14))=(int16_t)(dy0-((dy0-y0)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 32))=(int16_t)(dx1-((dx1-x1)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 34))=(int16_t)(dy1-((dy1-y1)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 60))=(int16_t)(dx2-((dx2-x2)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 62))=(int16_t)(dy2-((dy2-y2)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
        } else {
            (*(int16_t *)(p + 4))=0; (*(int16_t *)(p + 6))=1;
            (*(int16_t *)(p + 0))=0; (*(int16_t *)(p + 2))=0;
            (*(int16_t *)(p + 12))=(int16_t)dx0;
            (*(int16_t *)(p + 14))=(int16_t)dy0;
            (*(int16_t *)(p + 32))=(int16_t)dx1;
            (*(int16_t *)(p + 34))=(int16_t)dy1;
            (*(int16_t *)(p + 60))=(int16_t)dx2;
            (*(int16_t *)(p + 62))=(int16_t)dx2;
        }
        break;
    }
    case 4: {
        int32_t cnt;
        (*(int16_t *)(p + 32))=(int16_t)dx1; (*(int16_t *)(p + 34))=(int16_t)dy1;
        (*(int16_t *)(p + 60))=(int16_t)dx2; (*(int16_t *)(p + 62))=(int16_t)dx2;
        cnt = (*(int16_t *)(p + 0)) -= ((cardgame_tick_fn_t)DAT_8004DF9C)();
        if (cnt>0) {
            (*(int16_t *)(p + 12))=(int16_t)(x0-((x0-dx0)*cnt)/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 14))=(int16_t)(y0-((y0-dy0)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
        } else {
            (*(int16_t *)(p + 4))=2;
            (*(int16_t *)(p + 0))=0; (*(int16_t *)(p + 2))=0;
            (*(int16_t *)(p + 12))=(int16_t)x0;
            (*(int16_t *)(p + 14))=(int16_t)y0;
        }
        break;
    }
    case 5: {
        int32_t cnt;
        (*(int16_t *)(p + 32))=(int16_t)dx1; (*(int16_t *)(p + 34))=(int16_t)dy1;
        (*(int16_t *)(p + 60))=(int16_t)dx2; (*(int16_t *)(p + 62))=(int16_t)dx2;
        cnt = (*(int16_t *)(p + 0)) -= ((cardgame_tick_fn_t)DAT_8004DF9C)();
        if (cnt>0) {
            (*(int16_t *)(p + 12))=(int16_t)(dx0-((dx0-x0)*cnt)/(int32_t)(*(int16_t *)(p + 2)));
            (*(int16_t *)(p + 14))=(int16_t)(dy0-((dy0-y0)*(int32_t)(*(int16_t *)(p + 0)))/(int32_t)(*(int16_t *)(p + 2)));
        } else {
            (*(int16_t *)(p + 4))=0; (*(int16_t *)(p + 6))=1;
            (*(int16_t *)(p + 0))=0; (*(int16_t *)(p + 2))=0;
            (*(int16_t *)(p + 12))=(int16_t)dx0;
            (*(int16_t *)(p + 14))=(int16_t)dy0;
        }
        break;
    }
    case 0:
    case 2:
    default:
        break;
    }
}
