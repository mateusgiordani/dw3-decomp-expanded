// CARDGAME:0x80097508 (size 1000, 0x3e8)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x14858
// Framed function: prologue addiu sp,-0x38, saves s4/s3/s1/ra/s5/s2/s0;
// epilogue lw ra/s5/s4/s3/s2/s1/s0, jr ra/addiu sp,+0x38 at 0x800978cc..0x800978ec.
// Tables T1 at 0x80083700 (file-off 0xa50) and T2 at 0x80083718 (file-off 0xa68)
// sit before the function; next framed CARDGAME:0x800978f0 at +0x3e8 (gap 0).
// State byte *(p3+0x17): 0 -> return; 2 -> fixed-point reset path (A);
// 3 -> selector path (C); 1 or >=4 -> rate path (D). All nonzero states converge
// on tail CARDGAME_F0x80096e8c(p1,p2,p3).
// Path D: *(p3+4) = 0x1000 - ((*(p3+8) << 12) / *(p3+0xa)); u = DAT_8004df9c();
// *(p3+8) -= u; if ((short)diff > 0: *(p3+0x17) = 2 and switch T1[idx]:
// 0 -> F0x80097310(p1,p3,*(p2+0x20),DAT+0xf,1); 1,4 -> tail; 2 -> F0x80097310
// (p1,p3,*(p2+0x28),DAT+0x16,0); 3 -> F0x80097310(p1,p3,*(p2+0x24),DAT+0xf,0);
// 5 -> (*(p3+0xc)==5) ? F0x800973e4(p1,p2,p3,*(p2+0x18),DAT+0xf)
// : F0x80097310(p1,p3,*(p2+0x18),DAT+0xf,(*(p3+0x10)!=0x24)).
// Path A (state 2): *(p3+4) = 0x1000; idx==3 -> F0x80097310(...,*(p2+0x24),
// DAT+0xf,0); idx<4: idx==2 -> F0x80097310(...,*(p2+0x28),DAT+0x16,0);
// idx==4 -> 500-block (two (*(p2+4/8)+0x144)(.,0) unless *(p3+0x10)==500,
// then 0x2a/flag1 or 0x40+F0x80097238+F0x80097310(...,DAT+0xf,state)); else tail.
// Path C (state 3): switch T2[idx] selects *(p2+0x20/0x28/0x24/0x18/0x2c)
// (idx 4 first runs the two 0x144 calls; idx 1 and default skip the shared call);
// shared (*(t+0x144))(t,0); u = DAT_8004df9c(); *(p3+8) -= u;
// if ((short)diff <= 0: *(p3+0x17) = 0; *(p3+4) = ((*(p3+8) << 12) / *(p3+0xa)).
// Ghidra program CARDGAME read-only (project ddw3-pal-sles-03936, base 0x80082cb0):
// disasm 250 words match PAL word-for-word (250/250, first8 27bdffc8 afb40028
// 00808021 afb30024 00a09821 afb1001c 00b08821 afb70030; last8 8fbf0030
// 8fb5002c 8fb40028 8fb30024 8fb20020 8fb1001c 03e00008 27bd0038);
// decompile/x-ref to/from read-only, no mutation.
// Caller: CARDGAME_F0x800978f0 at 0x80097924 (6x (a0,a1,a0+off,i), off 0xe0c+i*0x18).
// Callees: CARDGAME_F0x800973e4 (0x80097644), F0x80097310 (shared site 0x80097768,
// direct 0x800976d0, 0x800977c0), F0x80097238 (0x80097748), F0x80096e8c tail
// (0x800978c4), DAT_8004df9c peril (0x80097590, 0x80097878), jump tables T1/T2
// (contents verified in reference bin: T1 975e0/978bc/976b8/97600/978bc/9761c,
// T2 977f4/9786c/97800/9780c/97818/97854).
// Upstream cardgame.s is GUIDE only, never copied as source.
// psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), --strip-div-guard,
// --rodata 0x80083700: exact_byte_match 1000/1000 (r7 o55). switch (st) with
// case 1 on the default block gives the ==2 / <3 / ==3 tree; st is copied after
// the ==0 test on the loaded byte; the timers are compound -= read back as int16_t.
#include "common/types.h"

extern int32_t (*DAT_8004df9c)(void);
extern uint32_t DAT_8005cca8;

extern void CARDGAME_F0x80097310(void *p1, void *p3, uint32_t w, void *d, int flag);
extern void CARDGAME_F0x800973e4(void *p1, void *p2, void *p3, uint32_t w, void *d);
extern void CARDGAME_F0x80097238(void *p1, void *p2, void *p3);
extern void CARDGAME_F0x80096e8c(void *p1, void *p2, void *p3);

typedef void (*tbl144_t)(void *t, int flag);

#define U8(p, o)  (*(unsigned char *)((unsigned char *)(p) + (o)))
#define S16(p, o) (*(int16_t *)((unsigned char *)(p) + (o)))
#define U16(p, o) (*(uint16_t *)((unsigned char *)(p) + (o)))
#define S32(p, o) (*(int32_t *)((unsigned char *)(p) + (o)))
#define U32(p, o) (*(uint32_t *)((unsigned char *)(p) + (o)))
#define PTR(p, o) (*(void **)((unsigned char *)(p) + (o)))
#define CALL144(t) ((tbl144_t)U32((t), 0x144))((t), 0)

void CARDGAME_F0x80097508(void *p1, void *p2, void *p3, int idx)
{
    int st;
    int16_t diff;
    void *t;

    if (U8(p3, 0x17) == 0)
        return;
    st = U8(p3, 0x17);
    switch (st) {
    case 1:
    default:
        S16(p3, 4) = (int16_t)(0x1000 - ((S16(p3, 8) << 12) / S16(p3, 10)));
        U16(p3, 8) -= DAT_8004df9c();
        diff = U16(p3, 8);
        if ((int16_t)diff <= 0) {
            U8(p3, 0x17) = 2;
            switch (idx) {
            case 0:
                CARDGAME_F0x80097310(p1, p3, U32(p2, 0x20), (void *)(DAT_8005cca8 + 0x0f), 1);
                break;
            case 2:
                CARDGAME_F0x80097310(p1, p3, U32(p2, 0x28), (void *)(DAT_8005cca8 + 0x16), 0);
                break;
            case 3:
                CARDGAME_F0x80097310(p1, p3, U32(p2, 0x24), (void *)(DAT_8005cca8 + 0x0f), 0);
                break;
            case 5:
                if (S16(p3, 0x0c) == 5)
                    CARDGAME_F0x800973e4(p1, p2, p3, U32(p2, 0x18), (void *)(DAT_8005cca8 + 0x0f));
                else
                    CARDGAME_F0x80097310(p1, p3, U32(p2, 0x18), (void *)(DAT_8005cca8 + 0x0f),
                                         S32(p3, 0x10) != 0x24);
                break;
            case 1:
            case 4:
                break;
            }
        }
        break;
    case 2:
        S16(p3, 4) = 0x1000;
        switch (idx) {
        case 2:
            CARDGAME_F0x80097310(p1, p3, U32(p2, 0x28), (void *)(DAT_8005cca8 + 0x16), 0);
            break;
        case 3:
            CARDGAME_F0x80097310(p1, p3, U32(p2, 0x24), (void *)(DAT_8005cca8 + 0x0f), 0);
            break;
        case 4:
            if (S32(p3, 0x10) == 500) {
                if (U8(p3, 0x16) == 0) {
                    S32(p3, 0x10) = 0x2a;
                    CARDGAME_F0x80097310(p1, p3, U32(p2, 0x2c), (void *)(DAT_8005cca8 + 0x0f), 1);
                } else {
                    S32(p3, 0x10) = 0x40;
                    CARDGAME_F0x80097238(p1, p2, p3);
                    CARDGAME_F0x80097310(p1, p3, U32(p2, 0x2c), (void *)(DAT_8005cca8 + 0x0f), (int)st);
                }
            } else {
                t = PTR(p2, 4);
                CALL144(t);
                t = PTR(p2, 8);
                CALL144(t);
                CARDGAME_F0x80097310(p1, p3, U32(p2, 0x2c), (void *)(DAT_8005cca8 + 0x1d), 0);
            }
            break;
        }
        break;
    case 3:
        switch (idx) {
        case 0:
            t = PTR(p2, 0x20);
            CALL144(t);
            break;
        case 2:
            t = PTR(p2, 0x28);
            CALL144(t);
            break;
        case 3:
            t = PTR(p2, 0x24);
            CALL144(t);
            break;
        case 4:
            t = PTR(p2, 4);
            CALL144(t);
            t = PTR(p2, 8);
            CALL144(t);
            t = PTR(p2, 0x2c);
            CALL144(t);
            break;
        case 5:
            t = PTR(p2, 0x18);
            CALL144(t);
            break;
        }
        U16(p3, 8) -= DAT_8004df9c();
        diff = U16(p3, 8);
        if ((int16_t)diff <= 0)
            U8(p3, 0x17) = 0;
        S16(p3, 4) = (int16_t)((S16(p3, 8) << 12) / S16(p3, 10));
        break;
    }
    CARDGAME_F0x80096e8c(p1, p2, p3);
}
