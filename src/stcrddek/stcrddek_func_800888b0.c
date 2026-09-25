/*
 * STCRDDEK:0x800888b0 STCRDDEK_func_800888b0
 * 536 bytes at STCRDDEK.PRO offset 0x5c00 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800888b0
 *  Symbols     D0x8004de10=0x8004de10 D0x8004df6c=0x8004df6c
 *  Compare     536 bytes from 0x800888b0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800888b0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next framed STCRDDEK:0x80088ac8 at +0x218 (27bdffe8 prologue), size 0x218
 * contiguous, no overlap.
 *
 * No Ghidra state change.
 *
 * Callers: none recorded (indirect/table dispatch suspected; campaign notes EXE
 * 0x80014504 entry pattern).
 *
 * Callees: 3 indirect jalr in mode-1 path (EXE alloc/method/cursor vectors
 * reached through a single hoisted 0x8004de10 base: lui 0x8005 + addiu -0x21f0
 * into s0, then +0x17c/+0x158/+0x0) plus one jalr shared by cross-jumping
 * (mode-1: submit vector *0x8004df6c, shared with FIELDSTG; other modes: p[0xe]
 * with arg p; each path sets up a0 itself, delay slot is nop).
 *
 * The lui 0x8005 + addiu -0x21f0 shape is the symbolic %hi/%lo(adjusted)
 * expansion of an address load (la), i.e. the base is address-taken into a
 * register once; per-use folded lui+lw (as an extern array with constant
 * indices produces) does not match. The submit vector keeps the shared
 * D0x8004df6c extern convention with FIELDSTG.
 *
 * Dispatch layout (PAL order: guards/div first, mode-1 block second, shared
 * tail) matches if (p[3] != 1) {...} else {...} with the mode-2/3 early return
 * tested inline at the top.
 *
 * Quotient roles (r5): PAL computes the mult-bound quotient in the FIRST div
 * (mflo a0 feeds mult) and the store-bound quotient in the SECOND div (divisor
 * reloaded via lw 0x70(s1), mflo v1 feeds the p[0x22] store); writing p[0x19]
 * before p[0x22] reproduces this exactly.
 *
 * Tail calls are direct per path (per-case-callback-call); cross-jumping merges
 * the two jalr sites into PAL's single shared jalr with per-path a0 setup and
 * nop delay.
 *
 * Clamp temp u and halfword temp v are distinct locals; v is uint16_t (its
 * halfword-width uses), which homes it in v0 as in PAL instead of sharing u's
 * a1 home.
 */

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
