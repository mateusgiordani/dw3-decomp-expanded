/* CARDGAME countdown-lerp with EXE vector tail.
 *
 * Verified entry: 0x80099ffc (420 bytes, 0x1a4). The eight bytes at
 * 0x80099ffc (lui v0,0x8005; lw v0,-0x2064(v0)) load the tick callback
 * from 0x8004df9c into v0; the framed body at 0x8009a004 opens with
 * stack adjustment then jalr v0 (delay slot move s0,a1). Same corrected
 * entry family as CARDGAME:0x8009a540 (former internal label 0x8009a548).
 * Prev CARDGAME:0x80099f60 (size 0x9c) ends at 0x80099ffc exactly, so the
 * prefix is an unclaimed gap; next CARDGAME:0x8009a1a0 confirms the tail.
 * Coordinator revision 8 covers the complete 0x80099ffc/420 function;
 * 0x8009a004 is an internal prologue. See boundary-review-r8.md.
 *
 * Body: rem = *(s0+0x28) - tick(); sw rem,(s0+0x28) in blez delay slot.
 * Live branch (rem > 0): word countdown-lerp on +0x00/+0x04
 * (guarded by two word compares) then half countdown-lerp on +0x18/+0x1a
 * from signed s16 deltas, subtracted from zero-extended (lhu) targets
 * (guarded by ONE word compare +0x18 vs +0x1c). Terminal branch
 * (rem <= 0): if lbu(s0+0x42) != 3 call EXE vector (*DAT_80055c48)
 * with a0 = 0x800460bd (lui in beq delay slot, ori in jalr delay slot);
 * then state byte = 1, +0x28/+0x2c = 0, snap words + halves (lhu) to
 * targets, and sh 0,(s0+0x26) only when lbu(s0+0x48) == 0.
 *
 * Matching notes (all load/store widths and reloads are load-bearing):
 * - tick via extern pointer object D_8004df9c (no incoming-v0 ABI).
 * - returned elapsed kept separate from the loaded counter
 *   (PAL subu a0,v1,v0); rem is NOT reused after the first mult --
 *   later mults read s->f28 fresh. Each fresh read follows a store to
 *   the same struct, which kills the CSE quantity and forces a real
 *   reload (word-Y reload needs the f00 store sunk above it by the
 *   scheduler, which also restores PAL store order).
 * - quotients are compiler temps (inlined into the stores), which puts
 *   each div triple (num,total,quot) in PAL registers with total/quot
 *   sharing; d0/d1 compiler stores f00/f04 early and the scheduler
 *   sinks them below the second div (PAL order).
 * - half stores are likewise sunk below the second half div (PAL order).
 * - divisions are bare (PAL has 0 guards over 4 divs): build with
 *   --strip-div-guard (tools/div_guard.py; division_scan 0/196 guarded).
 *
 * Evidence: docs/c-matching-guide/submissions/cardgame-8009a004/
 *   attempts-r7.json + strategy-r7.md; report build/match manifest
 *   CARDGAME_F0x80099ffc_W5r7final (exact_byte_match, full 420 bytes).
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0,
 *   variant base, --strip-div-guard, symbols D_8004df9c=0x8004df9c,
 *   DAT_80055c48=0x80055c48.
 */
typedef int (*CardTick9a004)(void);
extern CardTick9a004 D_8004df9c;
extern void (*DAT_80055c48)(unsigned int arg);

typedef struct {
    int f00;                  /* +0x00 */
    int f04;                  /* +0x04 */
    int f08;                  /* +0x08 target0 */
    int f0c;                  /* +0x0c target1 */
    int f10;                  /* +0x10 start0 */
    int f14;                  /* +0x14 start1 */
    short f18;                /* +0x18 current X (word-compared vs +0x1c) */
    short f1a;                /* +0x1a current Y */
    short f1c;                /* +0x1c target X */
    short f1e;                /* +0x1e target Y */
    short f20;                /* +0x20 start X */
    short f22;                /* +0x22 start Y */
    short f24;                /* +0x24 untouched here */
    short f26;                /* +0x26 cleared when byte +0x48 is 0 */
    int f28;                  /* +0x28 remaining */
    int f2c;                  /* +0x2c total */
    unsigned char pad30[18];  /* +0x30..0x41 untouched here */
    unsigned char f42;        /* +0x42 state (lbu/sb; 3 = done, set to 1) */
    unsigned char pad43[5];   /* +0x43..0x47 untouched here */
    unsigned char f48;        /* +0x48 flag (lbu) */
} cardgame_8009a004_ctx_t;

void CARDGAME_F0x80099ffc(int unused, cardgame_8009a004_ctx_t *s)
{
    int rem;
    int elapsed;

    (void)unused;
    elapsed = D_8004df9c();
    rem = s->f28 - elapsed;
    s->f28 = rem;
    if (rem > 0) {
        if ((s->f00 != s->f08) || (s->f04 != s->f0c)) {
            s->f00 = s->f08 - ((s->f08 - s->f10) * rem) / s->f2c;
            s->f04 = s->f0c - ((s->f0c - s->f14) * s->f28) / s->f2c;
        }
        if (*(int *)&s->f18 != *(int *)&s->f1c) {
            s->f18 = (short)((unsigned short)s->f1c - (unsigned int)(((int)s->f1c - (int)s->f20) * s->f28 / s->f2c));
            s->f1a = (short)((unsigned short)s->f1e - (unsigned int)(((int)s->f1e - (int)s->f22) * s->f28 / s->f2c));
        }
    } else {
        if (s->f42 != 3)
            DAT_80055c48(0x800460bd);
        s->f42 = 1;
        s->f28 = 0;
        s->f2c = 0;
        s->f00 = s->f08;
        s->f04 = s->f0c;
        s->f18 = (short)(unsigned short)s->f1c;
        s->f1a = (short)(unsigned short)s->f1e;
        if (s->f48 == 0)
            s->f26 = 0;
    }
}
