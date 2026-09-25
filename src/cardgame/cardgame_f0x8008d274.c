/*
 * CARDGAME:0x8008d274 CARDGAME_F0x8008d274
 * 800 bytes at CARDGAME.PRO offset 0xa5c4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008d274, jump table (.rodata) at 0x800834fc
 *  Symbols     (none)
 *  Compare     800 bytes from 0x8008d274 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008d274
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffb8 addiu sp,-0x48 ; sw s5,0x34(sp) ; move s5,a0 ; sw
 * s3,0x2c(sp) ; move s3,a1 ; sw s0,0x20(sp) ; move s0,a3 ; ... ; sw ra,0x44(sp)
 * ; ... ; epilogue lw ra,0x44(sp) ; ... ; jr ra ; addiu sp,+0x48. Next framed
 * CARDGAME:0x8008d594 at +0x320 (contiguous, first word 27bdffc8 addiu
 * sp,-0x38).
 *
 * No Ghidra state change.
 *
 * Callers: 8 direct jal (0x0c02349d) from CARDGAME_F0x80084320 at
 * 0x80084dd8/0x80084df4/0x80084e10/0x80084e2c/0x80084e48/0x80084e64/0x80084e80/
 * 0x80084e9c, passing (s1, s0, s3, 1..8 in a3 delay slot); each site then jumps
 * to 0x80084ebc. So a0 = slot array, a1 = callback-table object, a2 = flag, a3
 * = mode 1..8. Signature: void f(u8 *p, void *q, int flag, int mode).
 *
 * Callees: 6 indirect jalr through q: *(q+0xecc)(q);
 * *(q+0xf14)(q,15,0xe500,0x6100); *(q+0xf3c)(q,15,*(short *)(p + (signed
 * char)p[0x575]*8 + 0x580)); *(q+0xf24)(q,15,8,0x1000,0x1000) (5 args, 5th on
 * stack via sw 0x10(sp)); *(q+0xec8)(q); *(q+0xf40)(q[, sVar1]) per mode below.
 * Load-delay nops before each jalr; li/move in delay slots. No direct jal:
 * relocation_count 0.
 *
 * Mode dispatch is a PAL jump table at 0x800834fc (data word row, Ghidra
 * reports "No instruction at address"): lui v0,0x8008 ; addiu v0,0x34fc ; sll
 * v1,(mode-1),2 ; addu v1,v0 ; lw v0,(v1) ; jr v0, guarded by sltiu (mode-1)<8
 * else skip. Table entries (LE, from reference bin): 8008d3fc 8008d42c 8008d4fc
 * 8008d45c 8008d480 8008d4a4 8008d4bc 8008d4e0 for modes 1..8; mode 0 or >8
 * skips the switch (set flag stays 0).
 *
 * Loop: 12 iterations (idx 0..11). Per-iter pointers: r0 = p+idx (stride 1,
 * bytes +0x46f/+0x49e), r6 = p+0xe*idx (stride 0xe, short +0x72e for idx<6), r4
 * = q+0x4c*idx (stride 0x4c, byte +0x150), k = -0x54+0xe*idx (short at
 * p+k+0x7a0 for idx>=6). Per iter: r0[0x46f]=0; r0[0x49e]=1; r4[0x150] &= ~1;
 * if idx<6: skip unless idx < p[0x72c] (lbu), sVar1 = r6[0x72e] (lh); else skip
 * unless idx-6 < p[0x79e] (lbu), sVar1 = *(p+k+0x7a0) (lh).
 *
 * Mode semantics (set -> r0[0x46f]=1, r0[0x49e]=0, p[0x438]=1 word): 1: flag==0
 * ? idx<6 : idx>=6; 2: flag==0 ? idx>=6 : idx<6; 3: always; 4: cb40(q)!=1; 5:
 * cb40(q)!=2; 6: cb40(q)==3; 7: cb40(q)!=4; 8: cb40(q,sVar1)==6. Epilogue:
 * p[0x4aa..0x4ac]=1 (3-iter sb loop), p[0x4ad]=0, p[0x499]=1.
 *
 * Matching-critical notes (all portable C, no register variables): a1 already
 * holds sVar1 at every *(q+0xf40) jalr (loaded before the switch, never
 * clobbered), so modes 4-7 are modelled 1-arg and mode 8 2-arg exactly as the
 * decompiler prints; if the callee reads a1 uniformly, unifying to 2-arg adds
 * an explicit a1 setup (blocker). sb is used for byte stores, sh for the
 * q+0x594 halfword clear, sw for the p+0x438/p+0x440 word stores; the lb at
 * p+0x575 is signed (signed char index), lh is signed (short). The epilogue
 * 3-store loop is kept as a loop (addu+sb shape), not unrolled.
 *
 * Conservative names: no globals, no overlay table, no invented struct; q stays
 * opaque with explicit offset loads, same convention as matched sibling
 * CARDGAME:0x8008d854.
 *
 * r7 (o55/s0923g): the per-slot cursors are loop.c strength-reduction givs of
 * idx (p + idx, p + idx*14, q + idx*76, and k = (idx-6)*14 as a separate giv),
 * so their inits land after the hoisted switch index as in PAL; idx/set are
 * assigned just before the loop; the final loop starts from set (0 there), so
 * it is not reversed.
 *
 * Details: strategy-r7-o55/attempts-r7.
 */

typedef void (*cardgame_8008d274_cb_ecc_t)(void *q);
typedef void (*cardgame_8008d274_cb_f14_t)(void *q, int a, int b, int c);
typedef void (*cardgame_8008d274_cb_f3c_t)(void *q, int a, int b);
typedef void (*cardgame_8008d274_cb_f24_t)(void *q, int a, int b, int c, int d);
typedef void (*cardgame_8008d274_cb_ec8_t)(void *q);
typedef int (*cardgame_8008d274_cb_f40_t)(void *q, int v);
typedef int (*cardgame_8008d274_cb_f40_2_t)(void *q, int v);

void CARDGAME_F0x8008d274(unsigned char *p, void *q, int flag, int mode)
{
    int idx;
    int set;
    int k;
    int sVar1;
    int r;
    int i;
    int t;

    (*(cardgame_8008d274_cb_ecc_t *)((unsigned char *)q + 0xecc))(q);
    *(int *)(p + 0x440) = 0;
    p[0x422] = 1;
    (*(cardgame_8008d274_cb_f14_t *)((unsigned char *)q + 0xf14))(q, 15, 0xe500, 0x6100);
    t = *(signed char *)(p + 0x575) * 8;
    (*(cardgame_8008d274_cb_f3c_t *)((unsigned char *)q + 0xf3c))(q, 15, *(short *)(p + t + 0x580));
    *(short *)((unsigned char *)q + 0x594) = 0;
    (*(cardgame_8008d274_cb_f24_t *)((unsigned char *)q + 0xf24))(q, 15, 8, 0x1000, 0x1000);
    (*(cardgame_8008d274_cb_ec8_t *)((unsigned char *)q + 0xec8))(q);
    *(int *)(p + 0x438) = 0;
    idx = 0;
    set = 0;

    do {
        p[idx + 0x46f] = 0;
        p[idx + 0x49e] = 1;
        ((unsigned char *)q)[idx * 0x4c + 0x150] = (unsigned char)(((unsigned char *)q)[idx * 0x4c + 0x150] & 0xfe);
        if (idx < 6) {
            if (idx >= p[0x72c])
                goto next;
            sVar1 = *(short *)(p + idx * 0xe + 0x72e);
        } else {
            if (idx - 6 >= p[0x79e])
                goto next;
            k = (idx - 6) * 0xe;
            sVar1 = *(short *)(p + k + 0x7a0);
        }
        switch (mode) {
        case 1:
            if (flag == 0) {
                if (idx < 6)
                    set = 1;
            } else if (idx >= 6) {
                set = 1;
            }
            break;
        case 2:
            if (flag == 0) {
                if (idx >= 6)
                    set = 1;
            } else if (idx < 6) {
                set = 1;
            }
            break;
        case 3:
            set = 1;
            break;
        case 4:
            r = (*(cardgame_8008d274_cb_f40_t *)((unsigned char *)q + 0xf40))(q, sVar1);
            if (r != 1)
                set = 1;
            break;
        case 5:
            r = (*(cardgame_8008d274_cb_f40_t *)((unsigned char *)q + 0xf40))(q, sVar1);
            if (r != 2)
                set = 1;
            break;
        case 6:
            r = (*(cardgame_8008d274_cb_f40_t *)((unsigned char *)q + 0xf40))(q, sVar1);
            if (r == 3)
                set = 1;
            break;
        case 7:
            r = (*(cardgame_8008d274_cb_f40_t *)((unsigned char *)q + 0xf40))(q, sVar1);
            if (r != 4)
                set = 1;
            break;
        case 8:
            r = (*(cardgame_8008d274_cb_f40_2_t *)((unsigned char *)q + 0xf40))(q, sVar1);
            if (r == 6)
                set = 1;
            break;
        }
        if (set) {
            p[idx + 0x46f] = 1;
            p[idx + 0x49e] = 0;
            *(int *)(p + 0x438) = 1;
        }
next:
        idx += 1;
        set = 0;
    } while (idx < 12);

    for (i = set; i < 3; i++)
        p[0x4aa + i] = 1;
    p[0x4ad] = 0;
    p[0x499] = 1;
}
