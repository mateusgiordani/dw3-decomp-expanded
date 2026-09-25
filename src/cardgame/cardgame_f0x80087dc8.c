/*
 * CARDGAME:0x80087dc8 CARDGAME_F0x80087dc8
 * 276 bytes at CARDGAME.PRO offset 0x5118 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087dc8
 *  Symbols     D0x8004df9c=0x8004df9c
 *  Compare     276 bytes from 0x80087dc8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80087dc8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdffd8 addiu sp,-0x28 (sw s0/s1/ra), epilogue jr
 * ra / addiu sp,+0x28. Next function CARDGAME:0x80087edc (addiu sp,-0x40) at
 * +0x114 confirms size 0x114 contiguous.
 *
 * x-ref to: callers CARDGAME_F0x8008c044@0x8008c47c,
 * CARDGAME_F0x80089974@0x80089c58, CARDGAME_F0x80087edc@0x800886a4 (all
 * UNCONDITIONAL_CALL, jal with delay move a1,sX: 2-arg calls).
 *
 * x-ref from: no direct jal (2 computed jalr: EXE global *0x8004df9c via lui
 * 0x8005 + lw -0x2064, and *(param_2+0xf24)); rest are intra branches.
 *
 * State machine on *(p1+0x424) with accumulator *(p1+0x428): - state==1: acc +=
 * tick(); if acc<5 return 0; state 1->2, return 0.
 *
 * - state==3: acc += tick(); if acc<5 return 0; return 1 (no state change on
 * either state==3 exit).
 *
 * - otherwise (incl. state==2): fn = *(p2+0xf24); fn(p2, *(p1+0x43c), 5, X, X)
 * with X=0x1000 if state==2 else X=0x1400; then acc=0, state+=1, return 0.
 *
 * Only the state==3 success path returns nonzero. All paths converge on one
 * epilogue returning ret (s1 in asm: cleared in the entry branch delay slot,
 * set to 1 only on the state==3 success path), hence the shared tail/out labels
 * below.
 *
 * Physical reg notes: entry move a0,a1 serves the indirect calls' a0=p2; a2=5
 * (branch delay slot) likewise serves the indirect calls' a2=5, so it holds on
 * every path except the state==1 early branch. The EXE tick calls take no C
 * args (sibling tick idiom: cardgame_f0x8008642c/0x80092638/ 0x800942dc call
 * *0x8004df9c with ()); a1/a2/a3 on those paths are caller/hoist leftovers,
 * never set up (no move/a2/a3 write dominates the tick blocks). The 5-arg
 * indirect call places its 5th arg at sp+0x10, matching the observed sw/a3
 * setup.
 *
 * The state reloads use a dedicated temp (fresh) so the entry-loaded state
 * stays in v1 while reloads allocate v0; accumulator updates are memory-direct
 * (addu v1,v1,v0) with the compare forwarded from v1.
 */

extern int D0x8004df9c;

typedef int (*CardgameTick)(void);
typedef void (*CardgameIndirect5)(int a0, int a1, int a2, int a3, int a4);

int CARDGAME_F0x80087dc8(int p1, int p2)
{
    int state;
    int fresh;
    int ret;

    ret = 0;
    state = *(int *)(p1 + 0x424);
    if (state == 1)
        goto st1;
    if (state < 2)
        goto def;
    if (state == 2)
        goto st2;
    if (state == 3)
        goto st3;
def:
    (*(CardgameIndirect5 *)(p2 + 0xF24))(p2, *(int *)(p1 + 0x43C), 5, 0x1400, 0x1400);
    goto tail;
st1:
    *(int *)(p1 + 0x428) = *(int *)(p1 + 0x428) + ((CardgameTick)D0x8004df9c)();
    if (*(int *)(p1 + 0x428) < 5)
        goto out;
    fresh = *(int *)(p1 + 0x424);
    *(int *)(p1 + 0x424) = fresh + 1;
    goto out;
st2:
    (*(CardgameIndirect5 *)(p2 + 0xF24))(p2, *(int *)(p1 + 0x43C), 5, 0x1000, 0x1000);
tail:
    fresh = *(int *)(p1 + 0x424);
    *(int *)(p1 + 0x428) = 0;
    *(int *)(p1 + 0x424) = fresh + 1;
    goto out;
st3:
    *(int *)(p1 + 0x428) = *(int *)(p1 + 0x428) + ((CardgameTick)D0x8004df9c)();
    if (*(int *)(p1 + 0x428) < 5)
        goto out;
    ret = 1;
out:
    return ret;
}
