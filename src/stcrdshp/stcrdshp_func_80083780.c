/* STCRDSHP:0x80083780. Complete PAL selected-card display and hide paths.
 * exact_byte_match 1064/1064: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0, base.
 * Matching notes: the tables are addressed through their symbols (D_80048D34
 * byte table via tab = &D_80048D34[id], D_80044B38.fn414, D_8005CCA8); the slot
 * offset is a named local so self stays the first addend; CLEAR alone keeps a
 * do/while(0) (its loop notes weight the resources references so resources wins
 * s0 over id); the first DRAW(0x60) calls the +0x114 method through an int
 * return type, so jump2 does not cross-jump it with the id-list DRAW(0x60). */
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
