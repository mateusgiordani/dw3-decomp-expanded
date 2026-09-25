/*
 * CARDGAME:0x800870a4 CARDGAME_F0x800870a4
 * 392 bytes at CARDGAME.PRO offset 0x43f4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800870a4
 *  Symbols     D_80055C48=0x80055c48
 *  Compare     392 bytes from 0x800870a4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800870a4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Funcao emoldurada: prologo 27bdffd0 (sp,-0x30, salva s0-s3/ra) em 0x800870a4,
 * epilogo jr ra + 27bd0030 em 0x80087224; anterior termina em 0x800870a0 (jr +
 * sp,+0x30), proximo prologo 27bdffd0 em 0x8008722c: contiguo, sem vao.
 *
 * 4 callers jal no overlay: 0x8008776c, 0x8008780c, 0x80087a54, 0x80087af4
 * (palavra jal 0x0c021c29). Ambos os callers matching (875ac/878b4) declaram
 * void.
 *
 * Ghidra CARDGAME somente leitura: decompila void e (*D_80055C48)(0x4001b).
 *
 * Layout: par+0x43c int idx; rec+0xed8/+0xf0c ponteiros de fn, records stride
 * 76 com byte de flag +0x150 e halfword +0x12e; hdr+10 halfword com sinal.
 *
 * Fase A: global-fn(0x4001b) -> f(hdr.h10, idx) -> g(rec, idx, 5, r+0x1800,
 * 0x6100); limpa bit0 em rec[idx*76]+0x150; halfword 0 em +0x12e; idx += delta.
 *
 * Fase B: f de novo -> g(rec, idx, 1, r+0x1800, 0x5c00); seta bit0; rec +=
 * idx*76; halfword 1 no novo rec+0x12e.
 *
 * Ponteiros de tabela e idx recarregados da memoria a cada uso.
 *
 * O return 1 (int) fazia o const-set de retreg e o sched1 icava li v0,1 no
 * delay do ultimo lw de idx (20 B na cauda w81-90).
 */

typedef int (*cardgame_fnG)(int cmd, void *rec, void *hdr, int delta);
typedef int (*cardgame_fn2)(int a0, int a1);
typedef int (*cardgame_fn5)(void *a0, int a1, int a2, int a3, int a4);

extern cardgame_fnG D_80055C48;

void CARDGAME_F0x800870a4(void *par, unsigned char *rec, void *hdr, int delta) {
    int r;
    int off;

    D_80055C48(0x4001B, rec, hdr, delta);
    r = (*(cardgame_fn2 *)(rec + 0xED8))(*(short *)((char *)hdr + 10), *(int *)((char *)par + 0x43C));
    (*(cardgame_fn5 *)(rec + 0xF0C))(rec, *(int *)((char *)par + 0x43C), 5, r + 0x1800, 0x6100);
    off = (*(int *)((char *)par + 0x43C)) * 76;
    *(off + rec + 0x150) &= 0xFE;
    off = (*(int *)((char *)par + 0x43C)) * 76;
    *(short *)(off + rec + 0x12E) = 0;
    *(int *)((char *)par + 0x43C) += delta;
    r = (*(cardgame_fn2 *)(rec + 0xED8))(*(short *)((char *)hdr + 10), *(int *)((char *)par + 0x43C));
    (*(cardgame_fn5 *)(rec + 0xF0C))(rec, *(int *)((char *)par + 0x43C), 1, r + 0x1800, 0x5C00);
    off = (*(int *)((char *)par + 0x43C)) * 76;
    *(off + rec + 0x150) |= 1;
    off = (*(int *)((char *)par + 0x43C)) * 76;
    rec += off;
    *(short *)(rec + 0x12E) = 1;
}
