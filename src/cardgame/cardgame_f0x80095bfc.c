// CARDGAME:0x80095bfc (size 560, 0x230)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x12f4c
// Boundary: symbols/function_labels.csv CARDGAME,0x80095bfc,function,
// CARDGAME_F0x80095bfc,560 (cardgame-boundary-sweep). Prologue addiu sp,-0x60
// with s0/s1/s2/s3/ra saves; epilogue jr ra / addiu sp,+0x60. Next function
// CARDGAME_F0x80095e2c at +0x230 (registers this address as its EXE 0x80014504
// template, size 0x50, flag 4); no overlap.
// Overlay state callback. Dispatch on *(int32_t *)(p + 0xc): state 1 is the
// event gate (returns unless *(uint8_t *)(*q + 0x303) == 2, then
// (*(p+0x28))(p,2) and (*(*q+0x28))(*q,3)); state 2 is the polled path
// (no-op when *(p+0x10) == 1, else EXE vectors *0x8004B438/0x8004B440 with the
// 0x1500-or-*0x80048D68 select, then (*(p+0x3c))(p)); state 3 is a no-op;
// any other state runs init: EXE vectors *0x8004DF60(), *0x8004DF64(0xa000),
// *0x8004DF84(0x140,0xf0,0,0), filler F0x8001ffa8 over a 56-byte stack record
// (fn slots at buf+0x24 -> sp+0x34 and buf+0x2c -> sp+0x3c, same record shape
// as C_MATCHING sibling STCRDDEK_func_800884e8), slot(buf+0x24)(0x280,0),
// r = D_80044F5C(0x25d0000) -> slot(buf+0x2c)(r), slot(buf+0x24)(0x340,0),
// r = D_80044F5C(0x25d0001) -> slot(buf+0x2c)(r),
// r = D_8004DF7C(&D_800A5938,3,0x100) -> (*(r+0x12c))(r,0x1f,0x1f,0x1f),
// *q = CARDGAME_F0x800A3240(D_8004B43C()), then (*(p+0x38))(p).
// Shared-tail quirk (PAL): the init path loads (p+0x38) and jumps to the nop
// immediately before the shared jalr also used by the state-2 (p+0x3c) call
// (PAL j 0x80095e04); plain C leaves the tail merge to the compiler.
// Absolute accesses use array-base externs so the generator shares one base
// per region like PAL (s1 = 0x8004DE10 via lui 0x8005/addiu -0x21f0 with
// 0x150/0x154/0x16c/0x174 loads; s0 = 0x80044B38 via lui 0x8004/addiu +0x4b38
// with 0x424 loads; st2 s0 = 0x80048D34 via lui 0x8005/addiu -0x72cc with
// 0x2704/0x270c/0x34 accesses), mirroring the D_8008B848 array technique of
// C_MATCHING sibling STCRDDEK_func_800884e8. Indices: 0x150/4=0x54,
// 0x154/4=0x55, 0x16c/4=0x5b, 0x174/4=0x5d, 0x424/4=0x109, 0x2704/4=0x9c1,
// 0x270c/4=0x9c3, 0x34/4=0xd.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 140
// insns match the PAL words (first 16 words cross-checked against
// reference/extracted/pro/cardgame.bin at file-off 0x12f4c); decompile is
// hypothesis only, confirmed against disasm, never copied. Xref to 0x80095bfc:
// PARAM from CARDGAME_F0x80095e2c (0x80095e34); no direct jal (reached via the
// EXE 0x80014504 template registration). Direct EXE callees: 0x8001ffa8
// (filler) and 0x800a3240, which lies inside the CARDGAME range (prologue
// addiu sp,-0x20) so it is a CARDGAME-local callee, still unlabeled.
// Upstream cardgame.s is GUIDE only. No Ghidra state change.
// Dispatcher shape (beq ==1 / slti <2 / beq ==2 / beq ==3 with fallthrough to
// init) mirrors C_MATCHING sibling STCRDDEK_func_800884e8, hence the gotos.
// Toolchain base hypothesis: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0.
// Status: C_MATCHING (retry wave attempt 5: sel-in-a0 in-arg ternary; base
// toolchain psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 exact_byte_match,
// difference_count 0; prior C_NONMATCHING history retained below for provenance;
// fn_exact_pipeline result recorded in the
// handoff; attempt 1: single-symbol externs, object 552/560, s1 base not
// hoisted; attempt 2: array-base externs, object 552/560, st2/init tail order
// wrong; attempt 3: single tail(fp) + goto docall, object 556/560, PAL block
// order reproduced; attempt 4 (final): two-test st2 head (beq-zero + beq-one),
// object 568/560 with base 2.8.1: words 0-115 match modulo branch-target
// cascades; residue is three scheduler/allocator choices in the st2 region:
// (a) st2 base kept in $17 (shared with the DE10 base) instead of s0/$16
// (shared with the 44B38 base); (b) sel kept in $16 with two compensating
// moves instead of a0; (c) li sel,0x1500 sunk into the jalr delay
// slot instead of the bne delay slot (net +1 word from the extra move).
// Ranked alternate 1 (only compiler alternate tried):
// psyq-gcc-2.7.2-sn32-3.7-build-0002 + aspsx-2.79, object 576 bytes with
// 122/140 words differing (decisively worse codegen family). Blocker:
// scheduler/allocator build difference for the st2 region; needs a cc1 build
// whose earliest-ready heuristic leaves both st2 branch delays empty and
// shares the st2 base into s0 with sel in a0 (all other cardgame wins use
// base 2.8.1-sn32-4.0.0010, so no further alternates were burned).
#include <stdint.h>

extern void F0x8001ffa8(void *buf);
extern int32_t CARDGAME_F0x800A3240(int32_t a0);
extern int32_t D_800A5938;
extern void *D_8004DE10[];
extern int32_t (*D_80044B38[])(int32_t a0);
extern int32_t (*D_8004B43C)(void);
extern void *D_80048D34[];

void CARDGAME_F0x80095bfc(int32_t p, int32_t *q)
{
    uint8_t buf[56];
    int32_t state;
    int32_t r;
    void (*tail)(int32_t);

    state = *(int32_t *)(p + 0xc);
    if (state == 1) {
        goto st1;
    }
    if (state < 2) {
        goto other;
    }
    if (state == 2) {
        goto st2;
    }
    if (state == 3) {
        goto out;
    }
other:
    ((void (*)(void))D_8004DE10[0x54])();
    ((void (*)(int32_t))D_8004DE10[0x55])(0xa000);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))D_8004DE10[0x5d])(0x140, 0xf0, 0, 0);
    F0x8001ffa8(buf);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x24))(0x280, 0);
    r = D_80044B38[0x109](0x25d0000);
    ((void (*)(int32_t))*(void **)(buf + 0x2c))(r);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x24))(0x340, 0);
    r = D_80044B38[0x109](0x25d0001);
    ((void (*)(int32_t))*(void **)(buf + 0x2c))(r);
    r = ((int32_t (*)(int32_t, int32_t, int32_t))D_8004DE10[0x5b])((int32_t)&D_800A5938, 3, 0x100);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(r + 0x12c))(r, 0x1f, 0x1f, 0x1f);
    *q = CARDGAME_F0x800A3240(D_8004B43C());
    tail = *(void (**)(int32_t))(p + 0x38);
    goto docall;
st1:
    if (*(uint8_t *)(*q + 0x303) != 2) {
        goto out;
    }
    ((void (*)(int32_t, int32_t))*(void **)(p + 0x28))(p, 2);
    ((void (*)(int32_t, int32_t))*(void **)(*q + 0x28))(*q, 3);
    goto out;
st2:
    if (*(int32_t *)(p + 0x10) == 0) {
        goto work;
    }
    if (*(int32_t *)(p + 0x10) == 1) {
        goto out;
    }
work:
    ((void (*)(int32_t, int32_t))D_80048D34[0x9c3])(((((int32_t (*)(void))D_80048D34[0x9c1])() & 0xf) != 0) ? 0x1500 : (int32_t)D_80048D34[0xd], 0);
    tail = *(void (**)(int32_t))(p + 0x3c);
docall:
    tail(p);
out:
    return;
}
