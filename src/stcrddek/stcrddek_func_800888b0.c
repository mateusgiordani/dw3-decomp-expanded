// STCRDDEK:0x800888b0 (size 536, 0x218)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5c00 (RAW, no header)
// Boundary: prologue 27bdffe0 addiu sp,-0x20, saves s1/ra/s0 at 0x14/0x18/0x10(sp);
// s1=a0; epilogue lw ra/s1/s0 + jr ra + addiu sp,+0x20 at 0x80088ab4-0x80088ac4.
// Next framed STCRDDEK:0x80088ac8 at +0x218 (27bdffe8 prologue), size 0x218 contiguous, no overlap.
// Ghidra program STCRDDEK (project ddw3-pal-sles-03936) read-only: disasm 134 words matches PAL;
// decompile hypothesis only; x-ref/callers/callees empty (thin analysis DB, no recorded edges).
// Upstream stcrddek.s GUIDE only; cardgame/FIELDSTG siblings guide EXE-vector idiom only.
// No Ghidra state change.
// Callers: none recorded (indirect/table dispatch suspected; campaign notes EXE 0x80014504 entry pattern).
// Callees: 3 indirect jalr in mode-1 path (EXE alloc/method/cursor vectors reached through a single
// hoisted 0x8004de10 base: lui 0x8005 + addiu -0x21f0 into s0, then +0x17c/+0x158/+0x0) plus one
// jalr shared by cross-jumping (mode-1: submit vector *0x8004df6c, shared with FIELDSTG;
// other modes: p[0xe] with arg p; each path sets up a0 itself, delay slot is nop).
// The lui 0x8005 + addiu -0x21f0 shape is the symbolic %hi/%lo(adjusted) expansion of an address
// load (la), i.e. the base is address-taken into a register once; per-use folded lui+lw (as an
// extern array with constant indices produces) does not match. The submit vector keeps the shared
// D0x8004df6c extern convention with FIELDSTG.
// Dispatch layout (PAL order: guards/div first, mode-1 block second, shared tail) matches
// if (p[3] != 1) {...} else {...} with the mode-2/3 early return tested inline at the top.
// Toolchain (r5 exact): psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0 base variant,
// --strip-div-guard (PAL divs are bare: 0 breaks in 134 words; every pinned aspsx otherwise
// expands the cc1 3-operand div pseudo with guards). No volatile needed under 2.8.1: the
// p[0x19] store between the divisions blocks merging, so both div events survive without it
// (2.95.2 GCSE needed the volatile crutch in r1). r5 controls: 2.95.2-4.0.0030/2.86/no-div-check
// gives 540 B with rotated temps; 2.7.2-cygnus/2.79/strip gives 544 B. Both rejected.
// Quotient roles (r5): PAL computes the mult-bound quotient in the FIRST div (mflo a0 feeds
// mult) and the store-bound quotient in the SECOND div (divisor reloaded via lw 0x70(s1),
// mflo v1 feeds the p[0x22] store); writing p[0x19] before p[0x22] reproduces this exactly.
// Tail calls are direct per path (per-case-callback-call); cross-jumping merges the two jalr
// sites into PAL's single shared jalr with per-path a0 setup and nop delay.
// Clamp temp u and halfword temp v are distinct locals; v is uint16_t (its halfword-width
// uses), which homes it in v0 as in PAL instead of sharing u's a1 home.
#include <stdint.h>

extern uint8_t D0x8004de10[];
extern void (*D0x8004df6c)(uint8_t *);

typedef uint32_t *(*stc_mkobj_fn)(int32_t arg);
typedef uint32_t *(*stc_cursor_fn)(void);
typedef uint32_t *(*stc_method_fn)(uint32_t *obj, int32_t arg);
typedef void (*stc_submit_fn)(uint8_t *arg);

void STCRDDEK_func_800888b0(int32_t *p)
{
    uint32_t *base;
    uint32_t *r0;
    uint32_t *r1;
    uint32_t *r2;
    int32_t t;
    int32_t u;
    int32_t w;
    uint16_t v;

    if (p[3] != 1) {
        if (p[3] >= 2) {
            if (p[3] < 4)
                return;
        }
        if (p[0x1e] == 0 || p[0x1a] == 0)
            return;
        t = (p[0x20] - p[0x1f]) << 8;
        p[0x19] = (t / p[0x1c]) * p[0x1b];
        p[0x22] = t / p[0x1c];
        ((stc_submit_fn)p[0x0e])((uint8_t *)p);
        return;
    }
    base = (uint32_t *)D0x8004de10;
    r0 = ((stc_mkobj_fn)base[0x5f])(p[0x14]);
    r1 = ((stc_method_fn)*(uint32_t **)((uint8_t *)r0 + 0x138))(r0, p[0x15]);
    r2 = ((stc_cursor_fn)base[0x56])();
    if (p[0x1d] < p[0x1c] - 1) {
        u = p[0x1f] + ((p[0x1d] * p[0x22]) >> 8);
        w = p[0x20] - (p[0x19] >> 8);
        p[0x17] = u;
        if (w < u)
            p[0x17] = w;
    } else {
        p[0x17] = p[0x20] - (p[0x19] >> 8);
    }
    ((uint8_t *)r2)[3] = 5;
    ((uint8_t *)r2)[7] = 0x28;
    ((uint8_t *)r2)[6] = 0xff;
    ((uint8_t *)r2)[5] = 0xff;
    ((uint8_t *)r2)[4] = 0xff;
    v = *(uint16_t *)((uint8_t *)p + 0x58);
    *(uint16_t *)((uint8_t *)r2 + 0x10) = (uint16_t)v;
    *(uint16_t *)((uint8_t *)r2 + 8) = (uint16_t)v;
    v = *(uint16_t *)((uint8_t *)p + 0x58) + *(uint16_t *)((uint8_t *)p + 0x60);
    *(uint16_t *)((uint8_t *)r2 + 0x14) = (uint16_t)v;
    *(uint16_t *)((uint8_t *)r2 + 0xc) = (uint16_t)v;
    v = *(uint16_t *)((uint8_t *)p + 0x5c);
    *(uint16_t *)((uint8_t *)r2 + 0xe) = (uint16_t)v;
    *(uint16_t *)((uint8_t *)r2 + 0xa) = (uint16_t)v;
    v = *(uint16_t *)((uint8_t *)p + 0x5c) + (p[0x19] >> 8);
    *(uint16_t *)((uint8_t *)r2 + 0x16) = (uint16_t)v;
    *(uint16_t *)((uint8_t *)r2 + 0x12) = (uint16_t)v;
    r2[0] = (r2[0] & 0xff000000) | (r1[0] & 0xffffff);
    r1[0] = (r1[0] & 0xff000000) | ((uint32_t)r2 & 0xffffff);
    D0x8004df6c((uint8_t *)r2 + 0x18);
}
