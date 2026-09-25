/*
 * STCRDSHP:0x80083780 STCRDSHP_func_80083780
 * 1064 bytes at STCRDSHP.PRO offset 0xad0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083780
 *  Symbols     D_80044B38=0x80044b38 D_80048D34=0x80048d34
 *              D_8005CCA8=0x8005cca8 F0x8001ebf8=0x8001ebf8
 *              STCRDSHP_func_80083780=0x80083780
 *  Compare     1064 bytes from 0x80083780 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80083780
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Complete PAL selected-card display and hide paths.
 *
 * Matching notes: the tables are addressed through their symbols (D_80048D34
 * byte table via tab = &D_80048D34[id], D_80044B38.fn414, D_8005CCA8); the slot
 * offset is a named local so self stays the first addend; CLEAR alone keeps a
 * do/while(0) (its loop notes weight the resources references so resources wins
 * s0 over id); the first DRAW(0x60) calls the +0x114 method through an int
 * return type, so jump2 does not cross-jump it with the id-list DRAW(0x60).
 */

#include "common/types.h"
extern void F0x8001ebf8(void *);
extern uint8_t D_80048D34[];
extern struct {
    uint8_t pad[0x414];
    int32_t (*fn414)(int32_t);
} D_80044B38;
extern int32_t D_8005CCA8;
typedef void (*M2)(void *, int32_t);
typedef void (*M3)(void *, int32_t, int32_t);
typedef int32_t (*M3R)(void *, int32_t, int32_t);
#define CLEAR(slot) do {o=resources[(slot)/4]; (*(M2 *)((uint8_t *)o+0x144))(o,0);} while(0)
#define DRAW(slot,key,arg) {r=D_80044B38.fn414(D_8005CCA8+(key)); o=resources[(slot)/4]; (*(M3 *)((uint8_t *)o+0x114))(o,r,(arg));}
#define DRAWR(slot,key,arg) {r=D_80044B38.fn414(D_8005CCA8+(key)); o=resources[(slot)/4]; (*(M3R *)((uint8_t *)o+0x114))(o,r,(arg));}
#define VALUE(slot,arg) {o=resources[(slot)/4]; (*(M3 *)((uint8_t *)o+0x118))(o,0,(arg)); o=resources[(slot)/4]; (*(M2 *)((uint8_t *)o+0x148))(o,1);}
void STCRDSHP_func_80083780(uint8_t *self, void **resources, int32_t enabled)
{
    uint32_t work[22];
    int32_t off=*(int32_t *)(self+0x88)*4;
    int32_t id=*(int32_t *)(self+off+0x70);
    uint8_t *tab=&D_80048D34[id];
    int32_t r;
    void *o;
    if (tab[0x4df] && enabled) {
        F0x8001ebf8(work);
        ((void (*)(int32_t))work[11])(id);
        DRAW(0x4c,0x16,id);
        DRAW(0x58,0x32,8);
        VALUE(0x5c,(int8_t)tab[0x3a2]);
        if (((int32_t (*)(void))work[20])()) {
            CLEAR(0x50); CLEAR(0x54); DRAWR(0x60,0x1d,id);
        } else {
            DRAW(0x50,0x32,8);
            VALUE(0x54,((uint8_t *)work[0])[5]);
            if (id==0x45 || id==0x70 || id==0x9b || id==0xc6 || id==0xf1) {
                DRAW(0x60,0x1d,id);
            } else {
                CLEAR(0x60);
                DRAW(0x64,0x32,0x11);
                VALUE(0x68,((uint8_t *)work[0])[1]);
                DRAW(0x6c,0x32,0x12);
                VALUE(0x70,((uint8_t *)work[0])[2]);
                return;
            }
        }
    } else {
        CLEAR(0x4c); CLEAR(0x58); CLEAR(0x5c);
        CLEAR(0x50); CLEAR(0x54); CLEAR(0x60);
    }
    CLEAR(0x64); CLEAR(0x68); CLEAR(0x6c); CLEAR(0x70);
}
