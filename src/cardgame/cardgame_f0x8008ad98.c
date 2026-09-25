/*
 * CARDGAME:0x8008ad98 CARDGAME_F0x8008ad98
 * 3340 bytes at CARDGAME.PRO offset 0x80e8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008ad98, jump table (.rodata) at 0x800833fc
 *  Symbols     CARDGAME_F0x800860d4=0x800860d4 CARDGAME_F0x80086a30=0x80086a30
 *              CARDGAME_F0x80086d28=0x80086d28 CARDGAME_F0x80089f18=0x80089f18
 *              CARDGAME_F0x8008ad58=0x8008ad58 CARDGAME_F0x8008ad78=0x8008ad78
 *              DAT_8004B7D0=0x8004b7d0 D_80055c48=0x80055c48
 *              D_800a58a4=0x800a58a4 FUN_80086a18=0x80086a18
 *  Compare     3340 bytes from 0x8008ad98 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008ad98
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 0x8008baa4, PAL-SLES-03936.
 *
 * Both 14-entry dispatch tables start at 0x800833fc. Callback names retain
 * offsets; broader semantics and full object capacities remain uncertain.
 *
 * Partial member-array views preserve the observed address-expression trees.
 *
 * Explicit backedges preserve PAL ascending loops without GCC loop reversal.
 *
 * Consecutive loops reuse their real counter; byte values and offsets retain
 * distinct lifetimes. See the r9 Astra submission for measured controls.
 */

#include "common/types.h"
typedef struct { uint8_t unknown[0x446]; int8_t fields[1]; } flags_view;
typedef struct { uint8_t unknown[0x49e]; uint8_t fields[1]; } markers_view;
typedef struct { uint8_t unknown[0x580]; int16_t fields[1][4]; } values_view;
typedef struct { uint8_t unknown[0x151]; uint8_t fields[1][76]; } enabled_view;
typedef struct { uint8_t unknown[0x150]; uint8_t fields[1][76]; } selected_view;
typedef struct { uint8_t unknown[0x12e]; int16_t fields[1][38]; } half_view;
typedef struct { uint8_t unknown[0x14a]; uint8_t fields[1][76]; } status_view;
typedef struct { uint8_t unknown[0x46f]; uint8_t fields[1]; } mark_view;
#define B(p,o) (*(uint8_t *)((uint8_t *)(p)+(o)))
#define S(p,o) (*(int8_t *)((uint8_t *)(p)+(o)))
#define H(p,o) (*(int16_t *)((uint8_t *)(p)+(o)))
#define W(p,o) (*(int32_t *)((uint8_t *)(p)+(o)))
#define C1(o) ((void (*)(void *))W(c,o))(c)
#define C2(o,a) ((void (*)(void *,int32_t))W(c,o))(c,a)
#define C3(o,a,b) ((void (*)(void *,int32_t,int32_t))W(c,o))(c,a,b)
#define C4(o,a,b,d) ((void (*)(void *,int32_t,int32_t,int32_t))W(c,o))(c,a,b,d)
#define C5(o,a,b,d,e) ((void (*)(void *,int32_t,int32_t,int32_t,int32_t))W(c,o))(c,a,b,d,e)
extern int32_t D_800a58a4[];
extern uint8_t DAT_8004B7D0[];
#define POLL(k) ({ uint8_t *_sv = DAT_8004B7D0; int32_t _b = (*(int32_t (**)(int32_t))(_sv+0x3f4))(0); int32_t _s = (*(int32_t (**)(int32_t,int32_t))(_sv+0x408))(0,k); (_b >> _s)&1; })
#define REPEAT(k) ({ uint8_t *_sv = DAT_8004B7D0; int32_t _b1 = (*(int32_t (**)(int32_t))(_sv+0x3f4))(0); int32_t _s1 = (*(int32_t (**)(int32_t,int32_t))(_sv+0x408))(0,k); int32_t _b2 = (*(int32_t (**)(int32_t))(_sv+0x3fc))(0); int32_t _s2 = (*(int32_t (**)(int32_t,int32_t))(_sv+0x408))(0,k); (_b1 & (1u << _s1)) | (_b2 & (1u << _s2)); })
extern void (*D_80055c48)(uint32_t);
#define EVENT(v) D_80055c48(v)
extern int32_t CARDGAME_F0x8008ad58(void *,void *);
extern void FUN_80086a18(void *,void *);
extern int32_t CARDGAME_F0x800860d4(void *,void *,int32_t,int32_t,int32_t,int32_t);
extern void CARDGAME_F0x80086a30(void *,void *,int32_t);
extern void CARDGAME_F0x8008ad78(void *,void *,int32_t,int32_t);
extern void CARDGAME_F0x80089f18(void *,void *,int32_t,int32_t);
extern void CARDGAME_F0x80086d28(void *,void *,int32_t);
int32_t CARDGAME_F0x8008ad98(void *s,void *c)
{
    int32_t i,n,rv;
    rv = 0;
    if (B(s,0x423)) {
        switch(B(s,0x423)) {
        case 1:
            if(W(s,0x438)) {
                { int32_t count=0; uint8_t one=1;
loop_s3:
                *((uint8_t *)(count+(uint32_t)s) + 0x4aa)=one;
                count++; if(count<3) goto loop_s3;
                for(count=0;count<12;count++) {
                    if (((flags_view *)s)->fields[count]) ((markers_view *)s)->fields[count]=0;
                    else ((markers_view *)s)->fields[count]=1;
                }
                } B(s,0x499)=1; W(s,0x43c)=0;
                C1(0xecc); C1(0xec8);
                C4(0xf14,15,0xe500,0x6100);
                C3(0xf3c,15,((values_view *)s)->fields[S(s,0x575)][0]);
                H(c,0x594)=0; C5(0xf24,15,8,0x1000,0x1000);
            } else {
                { int32_t one,k; i=0; one=1; k=12*76;
loop_c3:
                    B((uint32_t)k+(uint8_t *)c,0x151)=one; i++; k+=76; if(i<3) goto loop_c3; }
                { for(i=0;i<12;i++) {
                    if (((flags_view *)s)->fields[i]) ((enabled_view *)c)->fields[i][0]=0;
                    else ((enabled_view *)c)->fields[i][0]=1;
                }
            }}
            W(s,0x42c)=CARDGAME_F0x8008ad58(s,c);
            FUN_80086a18(s,c); break;
        case 2:
            FUN_80086a18(s,c); W(s,0x428)=0; W(s,0x424)=0; W(s,0x434)=0; break;
        case 3:
            FUN_80086a18(s,c); W(s,0x428)=0; W(s,0x424)=0; W(s,0x434)=1; break;
        case 7: { int32_t i;
            i=W(s,0x43c);
            ((selected_view *)c)->fields[i][0]&=0xfe;
            i=W(s,0x43c);
            ((half_view *)c)->fields[i][0]=0;
            C2(0xeb0,4); C2(0xeb0,1); C2(0xeb0,2); C2(0xeb0,3);
            W(s,0x428)=0; W(s,0x424)=0; break; }
        case 8: { int32_t i;
            i=W(s,0x43c); i+=6;
            ((selected_view *)c)->fields[i][0]&=0xfe;
            i=W(s,0x43c); i+=6;
            ((half_view *)c)->fields[i][0]=0;
            C2(0xeb0,4); C2(0xeb0,1); C2(0xeb0,2); C2(0xeb0,3);
            W(s,0x428)=0; W(s,0x424)=0; break; }
        case 6:
            C2(0xeb0,4); C2(0xeb0,1); C2(0xeb0,2); C2(0xeb0,3);
            C2(0xf1c,W(s,0x440)); EVENT(0x4001c); break;
        case 9:
            C1(0xec4); C2(0xeb0,4); C2(0xeb0,1); C2(0xeb0,2); C2(0xeb0,3);
            B(s,0x499)=2; C5(0xf24,15,8,0,0x1000); break;
        case 10:
            C1(0xec4); B(s,0x499)=2; C5(0xf24,15,8,0,0x1000); break;
        case 11: case 12: case 13: case 14: break;
        }
        { int32_t pending=B(s,0x423); B(s,0x423)=0; B(s,0x422)=pending; }
    }
    switch(B(s,0x422)) {
    case 1:
        if(W(s,0x438) && (H(c,0x64)!=2 || B(s,0x498))) goto done;
        if(B(s,0x445)&1) {
            if(B(s,0x72c)) { B(s,0x423)=2; W(s,0x434)=0; }
            else B(s,0x445)&=0xfe;
        }
        if(B(s,0x445)&2) {
            if(B(s,0x79e)) { B(s,0x423)=3; W(s,0x434)=1; }
            else B(s,0x445)&=0xfd;
        }
        if(!B(s,0x445)) { B(s,0x423)=12; W(s,0x434)=3; goto done; }
        goto done;
    case 2: case 3:
        W(s,0x428)=CARDGAME_F0x800860d4(s,c,W(s,0x434),0,W(s,0x424),W(s,0x428));
        CARDGAME_F0x80086a30(s,c,D_800a58a4[W(s,0x434)]);
        W(s,0x424)++;
        if(W(s,0x424)<11) goto done;
        switch(W(s,0x434)) {
        case 0: CARDGAME_F0x8008ad78(s,c,0,0); B(s,0x423)=4; goto done;
        case 1: CARDGAME_F0x8008ad78(s,c,1,0); B(s,0x423)=5; goto done;
        default: goto done;
        }
    case 4:
        if((B(s,0x445)&2) && POLL(4) && B(s,0x79e)) B(s,0x423)=7;
        if(REPEAT(5) && W(s,0x43c)<B(s,0x72c)-1) CARDGAME_F0x80089f18(s,c,0,1);
        if(REPEAT(7) && W(s,0x43c)>0) CARDGAME_F0x80089f18(s,c,0,-1);
        CARDGAME_F0x80086d28(s,c,0);
        if(W(s,0x438) && POLL(14)) { EVENT(0x800450bd); B(s,0x423)=9; }
        if(!POLL(13)) goto done;
        if(!((flags_view *)s)->fields[W(s,0x43c)]) goto done;
        B(s,0x423)=6; W(s,0x440)=W(s,0x43c); goto done;
    case 5:
        if((B(s,0x445)&1) && POLL(6) && B(s,0x72c)) B(s,0x423)=8;
        if(REPEAT(5) && W(s,0x43c)<B(s,0x79e)-1) CARDGAME_F0x80089f18(s,c,1,1);
        if(REPEAT(7) && W(s,0x43c)>0) CARDGAME_F0x80089f18(s,c,1,-1);
        CARDGAME_F0x80086d28(s,c,1);
        if(W(s,0x438) && POLL(14)) { EVENT(0x800450bd); B(s,0x423)=9; goto done; }
        if(!POLL(13)) goto done;
        if(!S(s,0x44c + W(s,0x43c))) goto done;
        B(s,0x423)=6; W(s,0x440)=W(s,0x43c)+6; goto done;
    case 6:
        if(((status_view *)c)->fields[W(s,0x440)][0]!=1) goto done;
        if(!W(s,0x438)) {
            { int32_t k; i=0; k=12*76;
clear_c3:
                B((uint32_t)k+(uint8_t *)c,0x151)=0; i++; k+=76; if(i<3) goto clear_c3; }
            for(i=11;i>=0;i--) B(c,0x151 + i*76)=0;
        }
        B(s,0x423)=14; goto done;
    case 7:
        W(s,0x424)++; if(W(s,0x424)<11) goto done;
        n=B(s,0x79e)-1; B(s,0x423)=3;
        if(n<W(s,0x43c)) W(s,0x43c)=n; break;
    case 8:
        W(s,0x424)++; if(W(s,0x424)<11) goto done;
        n=B(s,0x72c)-1; B(s,0x423)=2;
        if(n<W(s,0x43c)) W(s,0x43c)=n; break;
    case 11: B(s,0x423)=14; goto done;
    case 9: case 10:
        if(H(c,0x64)!=0) goto done;
        if(B(s,0x498)!=0) goto done;
        B(s,0x423)=13; goto done;
    case 12:
        if(!POLL(14)) goto done;
        EVENT(0x800450bd); B(s,0x423)=10; goto done;
    case 13: rv = 1; break;
    case 14:
        { uint8_t *cursor; int32_t count=14; cursor=(uint8_t *)s+count;
clear_marks:
            B(cursor,0x46f)=0; count--; cursor--; if(count>=0) goto clear_marks; }
        ((mark_view *)s)->fields[W(s,0x440)]=1; rv = 2; break;
    }
done:
    return rv;
}
