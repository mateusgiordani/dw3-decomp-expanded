// STCRDDEK:0x800886c8 (size 256, 0x100)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5a18 (RAW, no header)
// Boundary: prologue 27bdffd0 addiu sp,sp,-0x30; saves s1/s2/ra/s0 at 0x24/0x28/0x2c/0x20(sp);
// s1=a0, s2=a1; epilogue lw ra/s2/s1/s0 + jr ra + addiu sp,sp,+0x30 (0x800887b0-0x800887c4).
// 64/64 Ghidra disasm words equal PAL words at file-off 0x5a18; body ends jr ra, no padding.
// Ghidra project ddw3-pal-sles-03936, program STCRDDEK, read-only: disasm 64 instr before
// decompile; decompile STCRDDEK_func_800886c8; x-ref to empty; graph callers/callees empty.
// No Ghidra state change. Upstream ddw3 stcrddek.s not checked out in this worktree: guide
// unavailable, not used. EXE helper 0x8001fcc0 (read-only EXE decompile) inits the 12-byte
// stack object: param_1[0..2] = {F0x8001f744, FUN_8001f770, F0x8001fa20}; slot[2] (sp+0x18)
// is the indirect target below. Callers inside STCRDDEK: none found (empty xrefs; table or
// overlay-entry dispatch not excluded). No loader/relocation semantics in this body.
// Behavior (conservative, disasm + decompiler hypothesis): *(int *)(p1+0x94) is an entry
// count for the halfword array at p1+0x78; 0x4081 is an empty-slot sentinel. Zero count*2
// bytes at p2; clear trailing 0x4081 entries scanning down; count leading 0x4081 entries
// scanning up (n); init stack callback via EXE 0x8001fcc0; call slot2(p2, &arr[n], 1).
// No semantic names promoted: p1/p2 plus offsets only.
// Domain pack: none generated. The body touches no battle/camera, skill, record, dialogue,
// field/map, sprite/rendering, disc-I/O, or overlay-load domain: only a local byte buffer,
// one struct halfword array with a sentinel, and an EXE callback init. Rendering-pack
// evidence from the campaign inspection covers menu context, not this body's accesses.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 -mips1 -msoft-float (base),
// no alternates needed.
// Pipeline: python tools/fn_exact_pipeline.py --name STCRDDEK_F0x800886c8
//   --ref upstream/ddw3/asm/dw2003/pro/stcrddek.s
//   --c decomp/src/overlays/stcrddek/stcrddek_func_800886c8.c
//   --reference-bin reference/extracted/pro/stcrddek.bin --base 0x80082cb0
//   --address 0x800886c8 --size 256 --opt-level O2 --symbol F0x8001fcc0=0x8001fcc0
// Result: exact_byte_match, difference_count 0, relocation_count 1 (jal F0x8001fcc0).
// sha256 aab1ef076509c5fe2707b4106d47958c64b2fe8fa0006a3aac18605194660747 (256B).
// Key idioms: both scan loops are single-head goto loops (no rotation/peel); the strip
// pointer is precomputed once and decremented on the back edge (kept live, so sh stays
// put and the decrement fills the bgez delay slot); sentinels are pre-set so their li
// fills the branch delay slots; the lead count is hoisted once; lead pointer/flag inits
// sit past the guard so the scheduler cannot hoist them into the strip branch delay.
// The strip pointer init is integer-domain (i * 2 + (int32_t)p1): pointer-typed addition
// is canonicalized base-first by the frontend, which emits addu v1,s1,v0 and misses by
// one word; integer-domain preserves the scaled-first order (addu v1,v0,s1).
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
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
