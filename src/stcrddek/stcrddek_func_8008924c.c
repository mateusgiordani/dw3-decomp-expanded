/*
 * STCRDDEK:0x8008924c STCRDDEK_func_8008924c
 * 264 bytes at STCRDDEK.PRO offset 0x659c (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8008924c
 *  Symbols     D_80048D34=0x80048d34 F0x8001e7ec=0x8001e7ec
 *              F0x8001ebf8=0x8001ebf8 STCRDDEK_func_80089354=0x80089354
 *  Compare     264 bytes from 0x8008924c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008924c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Ghidra range [0x8008924c,0x80089354)
 *
 * Prologue: addiu sp,-0x90 ; save s0-s7,ra ; a0=sp+0x10 -> jal 0x8001ebf8 (EXE
 * work-buffer init)
 *
 * Body: 3 outer iters; per iter zero 6 words at state+off+0x74, then 40 iters:
 * h = *(short *)(0x80048D34 + 0x63E + roff) [eff. 0x80049372+roff];
 * callback(*(sp+0x3C))(h); b = **(u8 **)(sp+0x10); state[off + (b-1)*4 +
 * 0x74]++
 *
 * Caller: STCRDDEK_func_80089354 default case (jal at 0x800893a8).
 *
 * Callee: EXE F0x8001ebf8 (0x8001ebf8) fills 0x54-byte stack work buffer;
 * per-iter indirect callback is work+0x2C (EXE F0x8001e7ec: short-keyed table
 * update, global side effects).
 *
 * Data: s7 anchor 0x80048D34 + lh 0x63E => eff. short table at 0x80049372,
 * stride 0x66; sibling site (case 0x36) indexes 0x66-stride structs at
 * 0x8004935C, so our shorts sit at struct+0x16 with 40 shorts filling the 0x66
 * struct (0x16 + 80 = 0x66). Exact struct TBD.
 *
 * Semantic (unconfirmed): deck-menu candidate histogram -- 3 groups x 6 buckets
 * at state+0x74..0xB8, 40 card-key samples per group; bucket picked by
 * callback-updated byte (*work). Menu execution itself is NOT confirmed
 * (name-based role hypothesis only).
 *
 * Types preserve uncertainty: state layout unknown (byte-addressed); D_80048D34
 * blob TBD.
 */

typedef void (*StcrddekCb)(short v);

extern unsigned char D_80048D34[];
void F0x8001ebf8(void *buf);

void STCRDDEK_func_8008924c(char *state)
{
    unsigned char work[0x54];
    StcrddekCb fn;
    unsigned char *bp;
    unsigned char *dbase;
    short h;
    int i;
    int n;
    int offA;
    int offB;
    int q;
    int t;
    int roff;
    int idx;
    int a;
    int cnt;

    F0x8001ebf8(work);
    for (i = 0, dbase = D_80048D34, offA = 0, offB = 0; i < 3; i++) {
        for (n = 0, q = offA; n < 6; n++, q += 4) {
            *(int *)(state + q + 0x74) = 0;
        }
        for (n = 0, t = offA, roff = offB; n < 40; n++) {
            h = *(short *)(roff + (int)dbase + 0x63E);
            roff += 2;
            fn = *(StcrddekCb *)(work + 0x2C);
            fn(h);
            bp = *(unsigned char **)work;
            idx = (*bp - 1) * 4;
            a = idx + t;
            cnt = *(int *)(state + a + 0x74);
            *(int *)(state + a + 0x74) = cnt + 1;
        }
        offA += 0x18;
        offB += 0x66;
    }
}
