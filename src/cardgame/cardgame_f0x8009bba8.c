/*
 * CARDGAME:0x8009bba8 CARDGAME_F0x8009bba8
 * 664 bytes at CARDGAME.PRO offset 0x18ef8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009bba8
 *  Symbols     CARDGAME_F0x80096950=0x80096950 CARDGAME_F0x80096e24=0x80096e24
 *              CARDGAME_F0x800978f0=0x800978f0 CARDGAME_F0x80097acc=0x80097acc
 *              CARDGAME_F0x80098558=0x80098558 CARDGAME_F0x80099ed4=0x80099ed4
 *              CARDGAME_F0x80099f60=0x80099f60 CARDGAME_F0x8009bac8=0x8009bac8
 *              CARDGAME_F0x8009d310=0x8009d310 DAT_8004df9c=0x8004df9c
 *              D_8004df9c=0x8004df9c func_0x8001ae38=0x8001ae38
 *              func_0x8001b364=0x8001b364
 *  Compare     664 bytes from 0x8009bba8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009bba8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdffe0 addiu sp,-0x20 ; sw s0,0x10(sp) ; move
 * s0,a0 ; sw s1,0x14(sp) ; sw ra,0x18(sp) at 0x8009bba8-0x8009bbb8.
 *
 * Epilogue lw ra,0x18(sp) ; lw s1,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu
 * sp,+0x20 at 0x8009be2c-0x8009be3c. Next function CARDGAME:0x8009be40 (sll
 * v0,a1,1 ...) starts exactly at +0x298, confirming size 664.
 *
 * (mode<2)||(mode!=3) -> alloc/build path, mode==3 -> immediate return. x-ref
 * to 0x8009bba8: 1 PARAM ref from CARDGAME_F0x8009d310 @ 0x8009d320 (lui
 * a0,0x800a + addiu a0,-0x4458 materializes 0x8009bba8 as arg0 of EXE jal
 * 0x80014504 with 0xf48,0x38: overlay callback registration; same file then
 * installs the vtable at +0xea0...+0xf44). x-ref from: 1x jal 0x8001b364, 13x
 * jal 0x8001ae38 (EXE-resident alloc wrappers), 8x jal CARDGAME helpers
 * (0x80099ed4, 0x80098558, 0x80097acc, 0x800978f0, 0x8009bac8, 0x80096e24,
 * 0x80096950 single-arg, 0x80099f60), READ of EXE vector 0x8004df9c, 3
 * intra-function branches, stack only.
 *
 * Mode dispatch (from disassembly): mode==1 -> helper chain; mode==3 -> return
 * with no stores; else -> indirect (*(st+0x38))(st) then 1x
 * func_0x8001b364(0x100,0,0,0) into out[0] + method *(tmp+0x80)(tmp,0), 13x
 * func_0x8001ae38(0x100,1,0,0) into out[1..13] with method *(tmp+0x160) kinds 3
 * (out[7]), 3 (out[11]), 5 (out[12]), 2 (out[13]).
 *
 * r5: dispatch is spelled as a goto chain (mode==1 -> helpers; mode<2 -> build;
 * mode==3 -> done; build falls through) because the equivalent if/else
 * short-circuit-OR form folds slti+bne away under -O2 (measured obj 656 vs
 * 664). The goto chain keeps slti v0,mode,2 + bne -> build over the mode==3
 * test with build as fallthrough: exact 664 B.
 *
 * Signatures conservative: widths from lw/sw (32-bit), offsets raw, no invented
 * structs for st/out slots. Indirect slots +0x38/+0x80/+0x160 kept as raw
 * offsets; EXE vector kept as data-word function pointer.
 */

#include <stdint.h>

extern int (*D_8004df9c)(void);
extern int func_0x8001b364(int, int, int, int);
extern int func_0x8001ae38(int, int, int, int);
extern void CARDGAME_F0x80099ed4(void *, int *);
extern void CARDGAME_F0x80098558(void *, int *);
extern void CARDGAME_F0x80097acc(void *, int *);
extern void CARDGAME_F0x800978f0(void *, int *);
extern void CARDGAME_F0x8009bac8(void *, int *);
extern void CARDGAME_F0x80096e24(void *, int *);
extern void CARDGAME_F0x80096950(void *);
extern void CARDGAME_F0x80099f60(void *, int *);

typedef void (*CardObjFn2)(int, int);
typedef void (*CardObjFn1)(int);

void CARDGAME_F0x8009bba8(void *st, int *out) {
    int mode;
    int v;

    mode = *(int *)((char *)st + 0xc);
    if (mode == 1)
        goto helpers;
    if (mode < 2)
        goto build;
    if (mode == 3)
        goto done;
build:
        ((CardObjFn1)*(int *)((char *)st + 0x38))((int)st);
        v = func_0x8001b364(0x100, 0, 0, 0);
        out[0] = v;
        ((CardObjFn2)*(int *)(v + 0x80))(v, 0);
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[1] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[2] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[3] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[4] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[5] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[6] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[7] = v;
        ((CardObjFn2)*(int *)(v + 0x160))(v, 3);
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[8] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[9] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[10] = v;
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[11] = v;
        ((CardObjFn2)*(int *)(v + 0x160))(v, 3);
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[12] = v;
        ((CardObjFn2)*(int *)(v + 0x160))(v, 5);
        v = func_0x8001ae38(0x100, 1, 0, 0);
        out[13] = v;
        ((CardObjFn2)*(int *)(v + 0x160))(v, 2);
    goto done;
helpers:
        v = D_8004df9c();
        *(int *)((char *)st + 0x54) = 0;
        *(int *)((char *)st + 0x58) += v;
        CARDGAME_F0x80099ed4(st, out);
        CARDGAME_F0x80098558(st, out);
        CARDGAME_F0x80097acc(st, out);
        CARDGAME_F0x800978f0(st, out);
        CARDGAME_F0x8009bac8(st, out);
        CARDGAME_F0x80096e24(st, out);
        CARDGAME_F0x80096950(st);
        CARDGAME_F0x80099f60(st, out);
done:
    return;
}
