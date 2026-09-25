/*
 * CARDGAME:0x8009d0b0 CARDGAME_F0x8009d0b0
 * 144 bytes at CARDGAME.PRO offset 0x1a400 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009d0b0
 *  Symbols     (none)
 *  Compare     144 bytes from 0x8009d0b0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009d0b0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Semantics: q = idx / 8; r = idx % 8; f24(q*16+320, r*32+256); f28(768,
 * idx+256); f20(a1). Third slot takes a single arg (a0=s2); its a1 is the stale
 * t from the second call, so the C prototype is 1-arg.
 *
 * That single-arg reading is the match key: a 2-arg f20 keeps t live across the
 * second jal and grows the object to 148 B; the 1-arg form reuses a1 and yields
 * 144 B exact.
 */

typedef void (*cardgame_fn2_t)(int a0, int a1);
typedef void (*cardgame_fn1_t)(int a0);

typedef struct {
    char pad[0x20];
    cardgame_fn1_t f20;
    cardgame_fn2_t f24;
    cardgame_fn2_t f28;
} cardgame_ctx_t;

void CARDGAME_F0x8009d0b0(cardgame_ctx_t *ctx, int a1, int idx)
{
    int q = idx / 8;
    int r = idx % 8;

    ctx->f24(q * 16 + 320, r * 32 + 256);
    ctx->f28(768, idx + 256);
    ctx->f20(a1);
}
