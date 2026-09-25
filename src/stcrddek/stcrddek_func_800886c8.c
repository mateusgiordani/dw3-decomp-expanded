/*
 * STCRDDEK:0x800886c8 STCRDDEK_F0x800886c8
 * 256 bytes at STCRDDEK.PRO offset 0x5a18 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800886c8
 *  Symbols     F0x8001f744=0x8001f744 F0x8001fa20=0x8001fa20
 *              F0x8001fcc0=0x8001fcc0 FUN_8001f770=0x8001f770
 *              stcrddek_func_800886c8=0x800886c8
 *  Compare     256 bytes from 0x800886c8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800886c8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 64/64 Ghidra disasm words equal PAL words at file-off 0x5a18; body ends jr
 * ra, no padding.
 *
 * No Ghidra state change. Callers inside STCRDDEK: none found (empty xrefs;
 * table or overlay-entry dispatch not excluded). No loader/relocation semantics
 * in this body.
 *
 * Behavior (conservative, disasm + decompiler hypothesis): *(int *)(p1+0x94) is
 * an entry count for the halfword array at p1+0x78; 0x4081 is an empty-slot
 * sentinel. Zero count*2 bytes at p2; clear trailing 0x4081 entries scanning
 * down; count leading 0x4081 entries scanning up (n); init stack callback via
 * EXE 0x8001fcc0; call slot2(p2, &arr[n], 1).
 *
 * No semantic names promoted: p1/p2 plus offsets only.
 *
 * sha256 aab1ef076509c5fe2707b4106d47958c64b2fe8fa0006a3aac18605194660747
 * (256B).
 *
 * Key idioms: both scan loops are single-head goto loops (no rotation/peel);
 * the strip pointer is precomputed once and decremented on the back edge (kept
 * live, so sh stays put and the decrement fills the bgez delay slot); sentinels
 * are pre-set so their li fills the branch delay slots; the lead count is
 * hoisted once; lead pointer/flag inits sit past the guard so the scheduler
 * cannot hoist them into the strip branch delay.
 *
 * The strip pointer init is integer-domain (i * 2 + (int32_t)p1): pointer-typed
 * addition is canonicalized base-first by the frontend, which emits addu
 * v1,s1,v0 and misses by one word; integer-domain preserves the scaled-first
 * order (addu v1,v0,s1).
 */

#include <stdint.h>

typedef void (*stcrddek_886c8_fn_t)(void *dst, void *src, int32_t arg);

typedef struct {
    void *slot0;
    void *slot1;
    stcrddek_886c8_fn_t fn;
} stcrddek_886c8_cb_t;

extern void F0x8001fcc0(void *cb);

void stcrddek_func_800886c8(void *p1, uint8_t *p2)
{
    int32_t i;
    int32_t n;
    int32_t sent;
    int32_t sent2;
    uint8_t *q;
    uint8_t *w;
    stcrddek_886c8_cb_t cb;

    i = 0;
    if (((*(int32_t *)((uint8_t *)p1 + 0x94)) << 1) > 0) {
        do {
            *(p2 + i) = 0;
            i += 1;
        } while (i < ((*(int32_t *)((uint8_t *)p1 + 0x94)) << 1));
    }
    /* Trailing-sentinel strip: index + byte pointer walked together. The
       pointer init runs once (sll+addu); the back edge decrements it, so it
       stays live and the scheduler keeps sh put with the decrement filling
       the bgez delay slot. Sentinel pre-set so its li fills bltz delay. */
    sent = 0x4081;
    i = (*(int32_t *)((uint8_t *)p1 + 0x94)) - 1;
    if (i < 0)
        goto strip_done;
    q = (uint8_t *)(i * 2 + (int32_t)p1);
strip_head:
    if (*(uint16_t *)(q + 0x78) != sent)
        goto strip_done;
    *(uint16_t *)(q + 0x78) = 0;
    i -= 1;
    q -= 2;
    if (i >= 0)
        goto strip_head;
strip_done:
    /* Leading-sentinel count: count hoisted once, sentinel pre-set, byte
       pointer from the struct base, element at +0x78, pointer bumped by 2
       alongside the counter. */
    n = *(int32_t *)((uint8_t *)p1 + 0x94);
    i = 0;
    if (n <= 0)
        goto lead_done;
    sent2 = 0x4081;
    w = (uint8_t *)p1;
lead_head:
    if (*(uint16_t *)(w + 0x78) != sent2)
        goto lead_done;
    i += 1;
    w += 2;
    if (i < n)
        goto lead_head;
lead_done:
    F0x8001fcc0(&cb);
    cb.fn(p2, (uint16_t *)((uint8_t *)p1 + 0x78) + i, 1);
}
