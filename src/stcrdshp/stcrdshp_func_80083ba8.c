#include "common/types.h"
/* STCRDSHP:0x80083ba8, PAL body 2072 bytes.
 * Keep panel/loop locals distinct: their lifetimes determine callback saves. */
extern void F0x8001f648(void *);
extern void F0x8001ebf8(void *);
typedef void (*cb1)(int32_t);
typedef void (*cb2)(int32_t,int32_t);
typedef void (*cb3)(int32_t,int32_t,int32_t);
typedef void (*cb4)(int32_t,int32_t,int32_t,int32_t);
typedef int32_t (*ret0)(void);
typedef int32_t (*ret1)(int32_t);
#define W(o) (*(int32_t *)(self+(o)))
#define A1(o,x) (*(cb1 *)(api+(o)))(x)
#define A2(o,x,y) (*(cb2 *)(api+(o)))(x,y)
#define A3(o,x,y,z) (*(cb3 *)(api+(o)))(x,y,z)
#define A4(x,y,z,w) (*(cb4 *)(api+0x84))(x,y,z,w)
extern struct { char pad[0x424]; ret1 fn424; } D_80044B38;
#define RESOURCE(id) (D_80044B38.fn424(id))
extern struct { ret1 unknown; ret1 fn4; } D_80042B98;
extern struct { char pad[0x188]; ret0 fn188; } D_8004DE10;
#define CLOCK() D_8004DE10.fn188()
extern int32_t D_8008CAF8[];
#define DRAW(id,x,y) do { resource=RESOURCE(0x063e0000); A4(resource,id,x,y); } while (0)
void STCRDSHP_func_80083ba8(uint8_t *self) {
 uint8_t api[0xa0], menu[0x58];
 int32_t i,index,card,resource,label,x,y,scale,now,choice,aux;
 F0x8001f648(api);
 A2(0x74,0x280,0); A2(0x7c,W(0x54),W(0x58));
 scale=W(0x6f8);
 if(scale!=0) {
  if(scale!=0x1000) { A3(0x8c,0x1000,scale,0x1000); A2(0x94,0xa0,0x5c); } else {
   for(i=0;i<8;) {
    int32_t card, resource, label, x, y;
    index=W(0x60)*8+i;
    { int32_t k=index*2; card=*(int16_t *)(self+k+0x98); }
    if(index>=W(0x3c0)||card<1) break;
    A2(0x74,0x140,0);
    resource=RESOURCE(0x02860000);
    label=D_80042B98.fn4(card);
    x=(i%2)*0x83+0x28; y=(i/2)*0xe + 0x39;
    A4(resource,label,x,y);
    /* PAL advances in this draw call's delay slot, before the icon draw. */
    i++;
    A2(0x74,0x280,0); DRAW(0x31,x,y);
   }
   A2(0x74,0x280,0);
   if(W(0x64)>1) {
    now=CLOCK();
    if(now-W(0x6c)>10) { W(0x68)++; if(W(0x68)>3) W(0x68)=0; }
    A1(0x88,W(0x68));
    if(W(0x60)>0) DRAW(0x48,0x1e,0x74);
    if(W(0x60)<W(0x64)-1) DRAW(0x47,0xfd,0x74);
    A1(0x88,0);
   }
  }
  DRAW(0x2b,0,0x32);
 }
 { int32_t scale; scale=W(0x708);
 if(scale!=0) {
  if(scale!=0x1000) { A3(0x8c,scale,0x1000,0x1000); A2(0x94,0x140,0x8f); }
  else {
   int32_t card, resource, label;
   A2(0x74,0x140,0);
   { int32_t k=W(0x5c)*2; card=*(int16_t *)(self+k+0x98); }
   resource=RESOURCE(0x02860000);
   label=D_80042B98.fn4(card);
   A4(resource,label,0x80,0x8a);
  }
  A2(0x74,0x280,0); DRAW(0x2e,0x79,0x83);
 }
 }
 if(W(0x718)!=0) {
  A3(0x8c,0x1000,W(0x718),0x1000);
  if(W(0x718)!=0x1000) A2(0x94,0xa0,0xcf);
  DRAW(0x2a,0,0xbd);
 }
 if(W(0x728)!=0) {
  { int32_t k=W(0x88)*4; card=*(int32_t *)(self+k+0x70); }
  F0x8001ebf8(menu); (*(cb1 *)(menu+0x2c))(card);
  A3(0x8c,W(0x728),0x1000,0x1000);
  if(W(0x728)!=0x1000) A2(0x94,0x140,0x87);
  choice=(*(ret0 *)(menu+0x50))();
  if(choice==1) aux=0x12;
  else if(choice==2) aux=0x13;
  else aux=**(uint8_t **)menu+0x13;
  DRAW(aux,0x103,0x7e); DRAW(0xd,0xfc,0x7c);
  if(W(0x728)!=0x1000) A2(0x94,0x140,0x87);
  DRAW(10,0x82,0x7c);
  if(W(0x728)!=0x1000) A2(0x94,0x140,0xaf);
  DRAW(0x10,0x103,0xa4); DRAW(0xd,0xfc,0xa2);
  if(W(0x728)!=0x1000) A2(0x94,0x140,0xa5);
  if(choice!=0) { DRAW(0xb,0x4a,0x92); }
  else if(card==0x45||card==0x70||card==0x9b||card==0xc6||card==0xf1) { DRAW(0xb,0x4a,0x92); } else { DRAW(0xc,0xc7,0x92); }
  if(W(0x8c)!=0) {
   now=CLOCK();
   if(now-W(0x94)>4) { now=CLOCK(); W(0x94)=now; W(0x90)++; if(W(0x90)>5) W(0x90)=0; }
   A2(0x7c,W(0x54),W(0x58)-2);
   A1(0x88,D_8008CAF8[W(0x90)]);
   DRAW(7,W(0x88)*0x2a+0x24,0x44);
  }
 }
}
