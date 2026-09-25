/* CARDGAME:0x8009a62c (304 B, 0x130). PAL-SLES-03936, base 0x80082cb0,
 * file-off 0x1797c. Prologo addiu sp,-0x20; s1=ctx, s0=tabela depois flag,
 * s2=0x1000. Epilogo lw ra/s2/s1/s0; jr ra; addiu sp,+0x20.
 * Contigua: prev 0x8009a5b0 termina aqui; next 0x8009a75c.
 * Um caller: 0x8009b95c em CARDGAME_F0x8009b890 (caso 6 de *(a1+0x42)).
 *
 * r12: exact_byte_match com ctx unsigned char* (alias C89 com o global do
 * callback impede o hoist do lui/lw para dentro da soma) e local q so para
 * o retorno de F0x8002abcc (r atravessa CB1 e o .greg poe em v1, o que
 * copiava v0->v1 no delay do bgez do /8). Ver strategy-r12.md.
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 base.
 * Status: C_MATCHING candidato (pai confirma). */

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
