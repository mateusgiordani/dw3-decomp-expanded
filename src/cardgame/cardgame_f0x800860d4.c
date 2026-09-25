/*
 * CARDGAME:0x800860d4 CARDGAME_F0x800860d4
 * 604 bytes at CARDGAME.PRO offset 0x3424 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800860d4
 *  Symbols     CARDGAME_F0x80086090=0x80086090 D0x800a58b4=0x800a58b4
 *              D0x800a58d4=0x800a58d4 F0x80087edc=0x80087edc
 *              F0x8008a068=0x8008a068 F0x8008ad98=0x8008ad98
 *              F0x8008c044=0x8008c044 F0x80092638=0x80092638
 *  Compare     604 bytes from 0x800860d4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800860d4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffc8 addiu sp,-0x38 ; sw s5,0x2c(sp) ; move s5,a0 ; sw
 * s2,0x20(sp) ; move s2,a1 ; sw s1,0x1c(sp) ; move s1,a2 ; sw s3,0x24(sp) ; lw
 * s3,0x4c(sp) ; sw ra,0x30(sp) ; sw s4,0x28(sp) ; sw s0,0x18(sp)
 *
 * Body: lui v0,0x800a + addiu 0x58b4 -> table 0x800a58b4 ; sll v1,s3,2 ; addu
 * a0,v1,v0 ; lw v1,0x48(sp) ; lh v0,0(a0) ; slt v0,v0,v1 ; beq -> early-return
 * if entry.w0 >= lim (delay move s4,a3 always) ; lh v1,2(a0) kind dispatch: ==1
 * case1, <2 then ==0 case0 else tail, >=2 then ==2 case2 else tail ; 8x jal
 * CARDGAME:0x80086090 + 5x jalr *(q+0xeac) ; tail slti idx<3, cond increment,
 * return idx Epilogue lw ra,0x30(sp) ; lw s5,0x2c(sp) ; lw s4,0x28(sp) ; lw
 * s3,0x24(sp) ; lw s2,0x20(sp) ; lw s1,0x1c(sp) ; lw s0,0x18(sp) ; jr ra +
 * addiu sp,+0x38 at 0x80086328/0x8008632c; size 0x25c = distance to 0x80086330
 * (functions.csv end). Prev gap CARDGAME:0x80086090 size 0x44 leaf ends at
 * 0x800860d4 exactly.
 *
 * DATA 0x800a58b4).
 *
 * Table 0x800a58b4: 4-byte entries {short w0; short kind;} (sibling 0x800896f0
 * uses same extern-D idiom for 0x800a58b4+0x20 region D0x800a58d4).
 */

#include <stdint.h>

typedef struct {
    int16_t w0;
    int16_t kind;
} CardEnt60d4;

extern CardEnt60d4 D0x800a58b4[];
/* Callee binary returns lh (already sign-extended), but caller-side codegen
   shows assignment-time extension (sll s0 early + sra in jal delay), so the
   original declaration must have been int-returning: the caller extends. */
extern int32_t CARDGAME_F0x80086090(void *, int32_t, int32_t);

typedef void (*CardCb60d4)(void *, int32_t, int32_t, int32_t, int32_t, int32_t);

int32_t CARDGAME_F0x800860d4(void *p, void *q, void *r, int32_t flag, int32_t lim, int32_t idx) {
    CardEnt60d4 *e = D0x800a58b4 + idx;
    int16_t kind;

    if (e->w0 >= lim)
        return idx;
    kind = e->kind;
    switch (kind) {
    case 0:
        (*(CardCb60d4 *)((char *)q + 0xeac))(q, 2, 1, 0, (int16_t)CARDGAME_F0x80086090(r, 0, 0), (int16_t)CARDGAME_F0x80086090(r, 0, 1));
        (*(CardCb60d4 *)((char *)q + 0xeac))(q, 4, 2, 0, (int16_t)CARDGAME_F0x80086090(r, 1, 0), (int16_t)CARDGAME_F0x80086090(r, 1, 1));
        if (flag != 0)
            (*(CardCb60d4 *)((char *)q + 0xeac))(q, 0, 0, flag, 0, 0x42);
        if (*(uint8_t *)((char *)p + 0x420) == 0x9a)
            (*(CardCb60d4 *)((char *)q + 0xeac))(q, 5, 4, 0x24, 0, 0x14);
        break;
    case 1:
        (*(CardCb60d4 *)((char *)q + 0xeac))(q, 3, 1, 0, (int16_t)CARDGAME_F0x80086090(r, 2, 0), (int16_t)CARDGAME_F0x80086090(r, 2, 1));
        break;
    case 2:
        (*(CardCb60d4 *)((char *)q + 0xeac))(q, 1, 3, 0, (int16_t)CARDGAME_F0x80086090(r, 3, 0), (int16_t)CARDGAME_F0x80086090(r, 3, 1));
        break;
    }
    if (idx < 3)
        idx++;
    return idx;
}
