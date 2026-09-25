/*
 * CARDGAME:0x8008be04 CARDGAME_F0x8008be04
 * 576 bytes at CARDGAME.PRO offset 0x9154 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008be04
 *  Symbols     CARDGAME_F0x8008bc8c=0x8008bc8c D_8004B7D0=0x8004b7d0
 *  Compare     576 bytes from 0x8008be04 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008be04
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Absolute addresses do NOT reproduce the hoist (each access re-emits li+ori
 * and spills two extra saved regs, +16 B), so the source addresses the table
 * through the 0x8004b7d0 base pointer.
 *
 * Epilogue lw ra/s6/s5/s4/s3/s2/s1/s0 + jr ra + addiu sp,+0x30 at
 * 0x8008c01c-40.
 *
 * Next framed CARDGAME:0x8008c044 at +0x240, size contiguous, no overlap.
 *
 * Callers: 1 direct jal from CARDGAME_F0x8008c044 (0x8008c400).
 *
 * Callees: 2 direct jal to CARDGAME_F0x8008bc8c (0x8008bf14 a2=-1, 0x8008bfb0
 * a2=+1); 10 indirect jalr via EXE vectors (BBCC/BBC4/BBD8 slots, results
 * shuffled into s2/s1/s0 in subsequent jalr delay slots, last result shifted in
 * v0).
 *
 * Body: mark record idx=state[0x43c] active in table (stride 76: byte+0x150 |=
 * 1, half+0x12e = 1; same idx*76 sll/addu/sll/subu/sll address idiom as
 * 0x8008bc8c).
 *
 * Then bit-test chain: (BBCC(0) & 1<<BBD8(0,7)) | (BBC4(0) & 1<<BBD8(0,7)) set
 * and idx>0 -> step -1 via bc8c; else (BBCC(0) & 1<<BBD8(0,5)) | (BBC4(0) &
 * 1<<BBD8(0,5)) set and idx<max-1 (state[0x434]-1, signed slt) -> step +1 via
 * bc8c; else if ((BBC4(0)>>BBD8(0,13))&1 (srav) and state byte [0x446+idx] != 0
 * (lb), state[0x423] = 3. All paths set state[0x445] = 0 (delay-slot sb on
 * taken paths).
 *
 * Reference sha256 over the 576 B body @0x9154:
 * 35ff5737f586d24bef5fe003bff990888c46065369c0c2c5990ecefefbf47bfa.
 *
 * Exact pipeline attempted (all with --symbol CARDGAME_F0x8008bc8c=0x8008bc8c):
 * - base 2.8.1, absolute slot addresses: object 592 > 576 (each access re-emits
 * li 0x80040000 + ori and spills two extra saved regs; frame 0x38).
 *
 * - base 2.8.1, exe-base-pointer C (this file): object 560 != 576 (-16 B).
 *
 * - alt1 2.7.2-cygnus-970404 + aspsx-2.79, same C: object 564 != 576.
 *
 * History: r2 blocker (literal 0x8004b7d0 -> li/ori, 560/564 B) resolved in r3
 * by extern D_8004B7D0 (lui/addiu, 568 B) plus per-arm distinct result
 * variables (third live value -> 8th saved reg, 576 B exact). Prior cc1
 * microprobe of a bare `return (uint32_t *)0x8004b7d0` is not representative:
 * through the extern symbol the same cc1 emits PAL's lui/addiu pair.
 */

#include <stdint.h>

typedef uint32_t (*cardgame_exe0_t)(int32_t);
typedef uint32_t (*cardgame_exe1_t)(int32_t, int32_t);

extern void CARDGAME_F0x8008bc8c(int32_t p1, int32_t p2, int32_t p3);
extern uint32_t D_8004B7D0[];

void CARDGAME_F0x8008be04(int32_t p1, int32_t p2)
{
    uint32_t *exe;
    uint32_t s2;
    int32_t s0;
    uint8_t *t1, *t2;

    t1 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    t1[0x150] |= 1;
    t2 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    *(uint16_t *)(t2 + 0x12e) = 1;
    exe = D_8004B7D0;
    {
        uint32_t r0 = ((cardgame_exe0_t)exe[0xff])(0);
        uint32_t q1 = ((cardgame_exe1_t)exe[0x102])(0, 7);
        uint32_t r1 = ((cardgame_exe0_t)exe[0xfd])(0);
        uint32_t q2 = ((cardgame_exe1_t)exe[0x102])(0, 7);
        s2 = (r0 & (1u << q1)) | (r1 & (1u << q2));
    }
    if (s2 != 0) {
        if (*(int32_t *)(p1 + 0x43c) > 0)
            CARDGAME_F0x8008bc8c(p1, p2, -1);
    } else {
        uint32_t r0 = ((cardgame_exe0_t)exe[0xff])(0);
        uint32_t q1 = ((cardgame_exe1_t)exe[0x102])(0, 5);
        uint32_t r1 = ((cardgame_exe0_t)exe[0xfd])(0);
        uint32_t q2 = ((cardgame_exe1_t)exe[0x102])(0, 5);
        s2 = (r0 & (1u << q1)) | (r1 & (1u << q2));
        if (s2 != 0) {
            if (*(int32_t *)(p1 + 0x43c) < *(int32_t *)(p1 + 0x434) - 1)
                CARDGAME_F0x8008bc8c(p1, p2, 1);
        } else {
            int32_t r3 = (int32_t)((cardgame_exe0_t)exe[0xfd])(0);
            int32_t q3 = (int32_t)((cardgame_exe1_t)exe[0x102])(0, 13);
            s0 = (r3 >> q3) & 1;
            if ((s0 != 0) && (*(int8_t *)(p1 + *(int32_t *)(p1 + 0x43c) + 0x446) != 0))
                *(uint8_t *)(p1 + 0x423) = 3;
        }
    }
    *(uint8_t *)(p1 + 0x445) = 0;
}
