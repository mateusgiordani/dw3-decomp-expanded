/*
 * STCRDSHP:0x80088b50 STCRDSHP_func_80088b50
 * 336 bytes at STCRDSHP.PRO offset 0x5ea0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088b50
 *  Symbols     D_80048D34=0x80048d34 D_8008CB80=0x8008cb80
 *              D_8008CB84=0x8008cb84 STCRDSHP_func_80087d00=0x80087d00
 *              STCRDSHP_func_800880c4=0x800880c4
 *              STCRDSHP_func_8008838c=0x8008838c
 *              STCRDSHP_func_80088b50=0x80088b50
 *              StcrdshpWork_80088b50=0x80088b50
 *  Compare     336 bytes from 0x80088b50 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80088b50
 */

#include "common/types.h"

/*
 * (0x80088b50-0x80088ca0).
 *
 * PAL bytes recovered from Ghidra program
 *
 * In-program xrefs to this function: none; it is reached via the overlay's
 * dispatch path (same pattern as the other STCRDSHP callbacks registered
 * through EXE 0x80014504).
 *
 * Behaviour: - kind == 1: run the shop update pair (0x8008838c, 0x800880c4).
 *
 * - kind == 2: nothing (return).
 *
 * - kind == 3: EXE sys-vector branch gated on flag +0x68.
 *
 * - otherwise (kind < 2, i.e. 0/negative, or kind > 3): phase gate on field
 * +0x10 against the EXE-resident vectors at 0x8008CB80/0x8008CB84.
 *
 * Match-critical source shape (psyq-gcc-2.8.1-sn32-4.0.0010, aspsx-2.79,
 *
 * O2/G0, base variant): the goto layout reproduces PAL block order (phase gate
 * inline first, kind==1/kind==3 bodies out of line at the end, so the poll
 * block needs no extra lui and both trailing jumps share the epilogue); the
 * kind dispatch keeps the `slti`+`bne` range test (`kind < 2` after excluding
 * kind == 1); the phase gate keeps the dual `beq` test (phase == 0, then phase
 * == 1 against the preserved `a0 == 1`) with a shared call block; the kind==3
 * vectors are read through one `&D_80048D34` base per branch so the compiler
 * shares a single `lui`/`addiu` base in `s0` (taken path) / `v0` (else path)
 * with the PAL offsets +0x34/+0x2708/+0x270c.
 *
 * Externs name fixed RAM slots; link addresses are supplied by the exact
 * pipeline (--symbol), never by guesswork in shared headers.
 */

extern void (*D_8008CB80)(void);
extern int32_t (*D_8008CB84)(void);
extern uint8_t D_80048D34;

void STCRDSHP_func_8008838c(void *work, void *arg);
void STCRDSHP_func_800880c4(void *work);
void STCRDSHP_func_80087d00(void *work, void *arg);

typedef struct {
    uint8_t unk_00[0x0c];   /* +0x00 */
    int32_t kind;           /* +0x0C state selector */
    int32_t phase;          /* +0x10 progress gate */
    uint8_t pad_14[0x24];   /* +0x14..0x37 */
    void (*draw)(void *);     /* +0x38 */
    uint8_t pad_3c[0x2c];   /* +0x3C..0x67 */
    int32_t flag_68;        /* +0x68 */
    uint8_t pad_6c[0x32c];  /* +0x6C..0x397 */
    int32_t val_398;        /* +0x398 */
    uint8_t pad_39c[0x0c];  /* +0x39C..0x3A7 */
    int32_t val_3a8;        /* +0x3A8 */
    uint8_t pad_3ac[0x0c];  /* +0x3AC..0x3B7 */
    int32_t val_3b8;        /* +0x3B8 */
} StcrdshpWork_80088b50;

void STCRDSHP_func_80088b50(StcrdshpWork_80088b50 *work, void *arg) {
    if (work->kind == 1) {
        goto do_kind1;
    }
    if (work->kind < 2) {
        goto phase_gate;
    }
    if (work->kind == 2) {
        return;
    }
    if (work->kind == 3) {
        goto do_kind3;
    }
phase_gate:
    if (work->phase == 0) {
        goto do_phase_call;
    }
    if (work->phase == 1) {
        goto do_phase_poll;
    }
do_phase_call:
    D_8008CB80();
    work->phase++;
    goto phase_done;
do_phase_poll:
    if (D_8008CB84() == 0) {
        STCRDSHP_func_80087d00(work, arg);
        work->val_398 = 10;
        work->val_3a8 = 10;
        work->val_3b8 = 10;
        work->draw(work);
    }
phase_done:
    return;
do_kind1:
    STCRDSHP_func_8008838c(work, arg);
    STCRDSHP_func_800880c4(work);
    return;
do_kind3:
    if (work->flag_68 != 0) {
        uint8_t *exe = &D_80048D34;
        uint32_t res = ((uint32_t (*)(void))*(uint32_t *)(exe + 0x2708))();
        ((void (*)(uint32_t, uint32_t))*(uint32_t *)(exe + 0x270c))(0x400, res);
    } else {
        uint8_t *exe = &D_80048D34;
        ((void (*)(uint32_t, uint32_t))*(uint32_t *)(exe + 0x270c))(*(uint32_t *)(exe + 0x34), 0);
    }
}
