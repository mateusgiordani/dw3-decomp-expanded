/*
 * CARDGAME:0x80095fb4 CARDGAME_F0x80095fb4
 * 400 bytes at CARDGAME.PRO offset 0x13304 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80095fb4
 *  Symbols     CARDGAME_F0x80095e58=0x80095e58 CARDGAME_F0x80095fb4=0x80095fb4
 *              EXE_D_8004DF9C=0x8004df9c
 *  Compare     400 bytes from 0x80095fb4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80095fb4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next CARDGAME:0x80096144 (gap leaves), prev CARDGAME_F0x80095e58 ends at
 * 0x80095fb4. No overlap.
 *
 * Decompile shows state dispatch on *(a0+0xc) with cases 1/2/3/default,
 * sub-dispatch on byte *(a0+0x62), two ((lh +0x64 << 12) / lh +0x66) div
 * blocks, two tick calls through EXE data word 0x8004df9c (lui 0x8005 + lw
 * -0x2064), indirect slots +0x28(obj,int)/+0x38(obj), and tail jal
 * CARDGAME_F0x80095e58 (hypothesis only; confirmed against disasm, not copied
 * as source).
 *
 * EXE data word 0x8004df9c also read by EXE FUN_80019224 (tick callback slot).
 *
 * Object layout stays untyped raw offsets (same 0x74-byte object as the
 * 0x80096180 allocator: +0xc state, +0x54/+0x56 params, +0x58/+0x5a fixed,
 * +0x62 mode, +0x63 flag, +0x64/+0x66 counters, +0x68/+0x6c/+0x70 handlers).
 *
 * The 0x1000 constant is 1.0 in 12-bit fractional fixed point: remaining =
 * 0x1000 - ((cur << 12) / max); init uses 10/10; completion sets 0x5a.
 *
 * Register evidence kept as control flow: PAL compares the mode byte against
 * the state register (beq v1,s1, no li) and stores the state register into the
 * mode byte (sb s1), so the source reads mode == state / stores state; the
 * value is provably 1 on that path (state == 1 dominates). The two slot-0x28
 * calls share one jalr site (second jumps in with its own a1), hence the fn/cmd
 * + goto docall shape.
 *
 * Block layout mirrors PAL address order (dispatch, init, case-1 with
 * chk2/calcA/cb2, case-2, shared jalr, tail) so branch senses match; mode is a
 * signed int local (PAL uses slti, not sltiu, on the lbu value).
 *
 * PAL div sites are bare div/mflo with no break guards: build with the pinned
 * psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 variant base with
 * --strip-div-guard (supported div normalization: 0/196 overlay division sites
 * guarded). The two slot-0x28 calls are one fn(obj, N) call per path (commands
 * 2 and 3); cross-jumping merges them into the single shared jalr with PAL
 * delay-slot scheduling.
 */

#include <stdint.h>

extern void CARDGAME_F0x80095e58(void *obj);
extern int (*EXE_D_8004DF9C)(void);

void CARDGAME_F0x80095fb4(uint8_t *obj) {
    int32_t state = *(int32_t *)(obj + 0x0c);
    void (*fn)(uint8_t *, int32_t);

    if (state == 1)
        goto doCase1;
    if (state < 2)
        goto doInit;
    if (state == 2)
        goto doCase2;
    if (state == 3)
        goto tail;
doInit:
    (*(void (**)(uint8_t *))(obj + 0x38))(obj);
    *(int16_t *)(obj + 0x66) = 10;
    *(int16_t *)(obj + 0x64) = 10;
    *(obj + 0x62) = 0;
    *(int16_t *)(obj + 0x58) = 0x1000;
    *(int16_t *)(obj + 0x5a) = 0;
    goto tail;
doCase1: {
        int mode = *(obj + 0x62);

        if (mode == state)
            goto tail;
        if (mode >= 2)
            goto chk2;
        if (mode == 0)
            goto doA;
        goto tail;
    chk2:
        if (mode == 2)
            goto docb2;
        goto tail;
    doA: {
            int32_t q = ((int32_t)*(int16_t *)(obj + 0x64) << 12) / (int32_t)*(int16_t *)(obj + 0x66);
            int32_t left;

            *(int16_t *)(obj + 0x5a) = (int16_t)(0x1000 - q);
            left = EXE_D_8004DF9C();
            left = (int32_t)*(uint16_t *)(obj + 0x64) - left;
            *(int16_t *)(obj + 0x64) = (int16_t)left;
            if (left * 0x10000 > 0)
                goto tail;
            *(int16_t *)(obj + 0x5a) = 0x1000;
            *(obj + 0x62) = (uint8_t)state;
            goto tail;
        }
    docb2:
        fn = *(void (**)(uint8_t *, int32_t))(obj + 0x28);
        fn(obj, 2);
        goto tail;
    }
doCase2: {
        int32_t q = ((int32_t)*(int16_t *)(obj + 0x64) << 12) / (int32_t)*(int16_t *)(obj + 0x66);
        int32_t left;

        *(int16_t *)(obj + 0x5a) = (int16_t)q;
        left = EXE_D_8004DF9C();
            left = (int32_t)*(uint16_t *)(obj + 0x64) - left;
        *(int16_t *)(obj + 0x64) = (int16_t)left;
        if (left * 0x10000 > 0)
            goto tail;
        fn = *(void (**)(uint8_t *, int32_t))(obj + 0x28);
        *(int16_t *)(obj + 0x5a) = 0;
        fn(obj, 3);
    }
tail:
    CARDGAME_F0x80095e58(obj);
}
