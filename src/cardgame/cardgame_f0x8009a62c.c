/*
 * CARDGAME:0x8009a62c CARDGAME_F0x8009a62c
 * 304 bytes at CARDGAME.PRO offset 0x1797c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009a62c
 *  Symbols     CARDGAME_F0x8009b890=0x8009b890 CARDGAME_TBL_800A5AD0=0x800a5ad0
 *              EXE_CB_8004DC0C=0x8004dc0c EXE_CB_8004DF9C=0x8004df9c
 *              F0x8002abcc=0x8002abcc
 *  Compare     304 bytes from 0x8009a62c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009a62c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936, base 0x80082cb0, file-off 0x1797c. Prologo addiu sp,-0x20;
 * s1=ctx, s0=tabela depois flag, s2=0x1000. Epilogo lw ra/s2/s1/s0; jr ra;
 * addiu sp,+0x20.
 *
 * Contigua: prev 0x8009a5b0 termina aqui; next 0x8009a75c.
 *
 * Um caller: 0x8009b95c em CARDGAME_F0x8009b890 (caso 6 de *(a1+0x42)).
 */

typedef int (*CardTick9a62c)(void);
extern short CARDGAME_TBL_800A5AD0[8];
extern CardTick9a62c EXE_CB_8004DC0C;
extern CardTick9a62c EXE_CB_8004DF9C;
extern int F0x8002abcc(int arg);

int CARDGAME_F0x8009a62c(void *param_1, unsigned char *ctx) {
    int r;
    int q;
    int n;
    int flag;

    (void)param_1;
    r = *(int *)(ctx + 0x10) + CARDGAME_TBL_800A5AD0[*(int *)(ctx + 0x28) & 7];
    *(int *)ctx = r;
    r = EXE_CB_8004DC0C();
    r = *(int *)(ctx + 0x14) + CARDGAME_TBL_800A5AD0[r & 7];
    flag = 0;
    *(int *)(ctx + 4) = r;
    q = F0x8002abcc((*(int *)(ctx + 0x28) << 12) / 24);
    *(short *)(ctx + 0x1a) = (short)(0x1000 - q / 8);
    r = EXE_CB_8004DF9C();
    n = *(int *)(ctx + 0x28) + r;
    *(int *)(ctx + 0x28) = n;
    if (n >= 12) {
        ctx[0x42] = 1;
        flag = 1;
        *(short *)(ctx + 0x26) = 0;
        *(short *)(ctx + 0x1a) = 0x1000;
        ctx[0x47] = 0;
        *(int *)ctx = *(int *)(ctx + 0x10);
        *(int *)(ctx + 4) = *(int *)(ctx + 0x14);
    }
    return flag;
}
