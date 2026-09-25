// STCRDDEK:0x80084890 (size 2432, 0x980; file off 0x1be0 = vaddr - base 0x80082cb0)
// PAL: reference/extracted/pro/stcrddek.bin (RAW, no header; 35748 B)
// Boundary: prologue addiu sp,sp,-0x120 + saves s0-s4/ra at 0x80084890-0x848b4;
// epilogue restores + jr ra + addiu sp,sp,+0x120 at 0x800851f0-0x8520c.
// Next framed STCRDDEK:0x80085210 at +0x980, size 2432 contiguous, no overlap.
// Prev reviewed STCRDDEK:0x800847c0 size 208 ends exactly at entry.
// Ghidra program STCRDDEK (project ddw3-pal-sles-03936) read-only: disasm 608
// words match PAL word-for-word (func sha256
// 2361353532786e24aaac9bcade1759071a4a3a391c7eea4db3a85a7ccaa441ca);
// decompile + full-disasm walkthrough in reports/handoffs/stcrddek-80084890-c-recovery.md.
// Callers: 1 direct jal from STCRDDEK 0x800865f0 (in STCRDDEK_func_80086574,
// UNCONDITIONAL_CALL); no other intra-overlay refs (table-driven/cross-overlay
// callers possible, as with sibling tiny setters).
// Callees: 2 direct EXE helpers (0x8001f648 table-fill x2, 0x8001ebf8 card-decode
// x3); all other calls are indirect jalr via the stack table below or via EXE
// RAM vectors 0x8004df98 (tick) and 0x80044f5c (gfx ctx factory, arg 0x63e0000).
// stcrddek.s / recomp are GUIDES only; nothing copied as source.
// No Ghidra state change. Deck-menu role still unconfirmed beyond name hint.
// Toolchain hypothesis: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Full-body exact candidate in revision 6; coordinator review pending (see handoff).
#include "common/types.h"

extern void EXE_F0x8001f648(void *tbl);
extern void EXE_F0x8001ebf8(void *out);
/* Family B (symbolic-absolute-base): PAL keeps EXE table bases in s-regs
   (tick 0x8004de10: lui 0x8005 + addiu -8688, vector at +392; gfx 0x80044b38:
   lui 0x8004 + addiu 19256, vector at +1060). Same idiom as the EXACT-matching
   sibling STCRDDEK:0x800835d8 (D_80044B38.fn424(0x63E0000)) and
   STCRDABM:0x80083848 (D0x8004DE10[0x62] tick). */
extern int32_t D0x8004DE10[];   /* EXE table; tick vector at [0x62] */
extern uint8_t D_80048D34[];    /* EXE card LUT base; cardmap entry = base+idx*2+0x63E */
extern struct {
    uint8_t pad[0x424];
    int32_t (*fn424)(int32_t a0);
} D_80044B38;                   /* EXE table; gfx factory at +0x424 */
/* STCRDDEK_cardmap (0x80049372) retired: PAL addresses the LUT as
   D_80048D34 + idx*2 + 0x63E (cf. sibling 835d8, EXACT). */
extern int32_t STCRDDEK_anim_tbl[]; /* in-overlay word table, entry 0x8008a484 */

/* Stack table filled by EXE_F0x8001f648 over sp+0x10; slots at absolute sp
   offsets (rel = sp - 0x10): fn_clear2 @+0x84, fn_xy @+0x8c, draw @+0x94,
   mark1 @+0x98, fn_scale3 @+0x9c, fn_pair2 @+0xa4, cardbuf @+0xb0 (11 words),
   fn_card @+0xdc, fn_query @+0x100. */
typedef struct {
    uint8_t _00[0x74];
    void (*fn_clear2)(int32_t a0, int32_t a1);
    uint8_t _78[4];
    void (*fn_xy)(int32_t a0, int32_t a1);
    uint8_t _80[4];
    void (*draw)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
    void (*mark1)(int32_t a0);
    void (*fn_scale3)(int32_t a0, int32_t a1, int32_t a2);
    uint8_t _90[4];
    void (*fn_pair2)(int32_t a0, int32_t a1);
    uint8_t _98[8];
    uint8_t *card[11];
    void (*fn_card)(int32_t a0);
    uint8_t _d0[0x20];
    int32_t (*fn_query)(void);
} GfxTbl;

/* Caller block; only offsets touched by this body are named. +0x88 reads are
   16-bit card ids; elements past the named head are unrecovered. */
typedef struct {
    uint8_t _00[0x54];
    int32_t f54;
    int32_t f58;
    int32_t f5c;
    int32_t f60;
    int32_t f64;
    int32_t f68;
    int32_t f6c;
    int32_t f70;
    int32_t f74;
    int32_t f78;
    int32_t f7c;
    int32_t f80;
    int32_t f84;
    int16_t cards_88[482]; /* 0x88..0x44c */
    int32_t f44c;
    uint8_t _450[0xc];
    int32_t f45c;
    uint8_t _460[0xc];
    int32_t f46c;
    uint8_t _470[0xc];
    int32_t f47c;
    uint8_t _480[0xc];
    int32_t f48c;
} DeckCtx;

/* Family A (indirect-domain): PAL issues vector-indirect calls inline (lw+jalr),
   no direct jal to a local wrapper. Macros emit the cast-call at each site. */
/* Family C9 (per-region anchor locals): PAL materializes each EXE table base
   per region (tick s0 at entry + D-blink; gfx s0/s2/s4 per v-path; cardmap a1
   in B) via lui+addiu with small-offset uses. Function-scope sharing folded.
   Each region takes its anchor into a narrow-scope local; single-use regions
   (A/C/F/arrows/RARE) use the direct member spelling. */
#define TICK() (((int32_t (*)(void))tickbase[0x62])())

#define GFXCTX() ((*(int32_t (**)(int32_t))((uint8_t *)g + 0x424))(0x63e0000))
/* Family C5: the RARE case uses the direct member spelling (single-use symbol
   load, v0-direct like PAL 0x80084b7c/0x800850c8); the ELSE case keeps the
   b38-local (s-cached 1060(sX) like PAL). Different address pseudos keep the
   two GFXCTX calls alive through jump2 cross_jump; only the draw tails merge.
   Both spellings have project precedent (835bc index vs 835d8 local). */
#define DRAW_D(id,x,y) do { resource = D_80044B38.fn424(0x63e0000); tbl.draw(resource,id,x,y); } while (0)

#define DRAW(id,x,y) do { resource = (int32_t)GFXCTX(); tbl.draw(resource,id,x,y); } while (0)

void STCRDDEK_F0x80084890(DeckCtx *ctx)
{
    GfxTbl tbl;
    int32_t resource;
    int32_t t;
    int32_t v;
    int32_t id;
    int32_t i;
    int32_t y;

    EXE_F0x8001f648(&tbl);
    tbl.fn_clear2(0x280, 0);
    tbl.fn_xy(ctx->f54, ctx->f58 - 2);

    if (ctx->f70 != 0) {
        int32_t *tickbase = D0x8004DE10;
        t = TICK();
        if (t - ctx->f6c > 4) {
            ctx->f6c = TICK();
            ctx->f68 += 1;
            if (ctx->f68 > 5)
                ctx->f68 = 0;
        }
        tbl.mark1(STCRDDEK_anim_tbl[ctx->f68]);
        DRAW_D( 0x43, (ctx->f60 << 5) | 0x10, ctx->f64 * 0x20 + 0x3a);
        tbl.mark1(0);
    }

    if (ctx->f45c != 0) {
        if (ctx->f45c != 0x1000) {
            tbl.fn_scale3(ctx->f45c, 0x1000, 0x1000);
            tbl.fn_pair2(0x140, 0x25);
        }
        /* a1 at this call site is stale (0x25); callee arity unresolved. */
        EXE_F0x8001ebf8(tbl.card);
        {
            uint8_t *deckbase = D_80048D34;
            int32_t cell = ctx->f60 + ctx->f64 * 9;
            id = *(int16_t *)((((ctx->f5c * 51) + cell) << 1) + (int32_t)deckbase + 0x63E);
        }
        tbl.fn_card(id);
        v = tbl.fn_query();
        /* Family C6 (branch-sense fallthrough): PAL dispatches v!=0 as the
           fall-through (beq v,zero -> v==0 target) in both phases. */
        if (v != 0) {
            void *g = &D_80044B38;
            DRAW( v + 0x11, 0x103, 0x24);
            DRAW( 0xd, 0xfc, 0x22);
            DRAW( 0xb, 0x4a, 0x12);
        } else {
            void *g = &D_80044B38;
            /* Keep the promoted byte in v across the resource callback. The
               separate addition preserves PAL's draw-argument delay slot. */
            v = tbl.card[0][0];
            resource = (int32_t)GFXCTX();
            v += 0x13;
            tbl.draw(resource, v, 0x103, 0x24);
            DRAW( 0xd, 0xfc, 0x22);
            /* Family C2 (per-case-callback-call, switch form): PAL keeps one
               GFXCTX+draw per case and cross-jumps only the draw tail.
               The if/else diamond let GCC merge the identical GFXCTX prefix
               (single GFXCTX after the join); separate switch cases keep two
               call_insn alive until jump2, as in CARDGAME:0x8008dc70. */
            /* Family C7: if/else-chain with RARE as fall-through (PAL beq
               layout: compares fall into RARE, bne to ELSE, ELSE falls into
               the shared draw). Same per-case call split as C5/C6. */
            if (id == 0x45 || id == 0x70 || id == 0x9b || id == 0xc6 ||
                id == 0xf1)
                DRAW_D( 0xb, 0x4a, 0x12);
            else
                DRAW( 0xc, 0xc7, 0x12);
        }
    }

    if (ctx->f44c != 0) {
        tbl.fn_scale3(0x1000, ctx->f44c, 0x1000);
        if (ctx->f44c != 0x1000)
            tbl.fn_pair2(0xa0, 0x74);
        tbl.fn_xy(ctx->f54, ctx->f58);
        DRAW_D( 0x40, 0x10, 0x15);
    }

    EXE_F0x8001f648(&tbl);
    tbl.fn_clear2(0x280, 0);
    tbl.fn_xy(ctx->f54, ctx->f58);

    if (ctx->f46c != 0) {
        /* Branch-sense: PAL beq-equal falls into the loop path. */
        if (ctx->f46c != 0x1000) {
            tbl.fn_scale3(ctx->f46c, 0x1000, 0x1000);
            tbl.fn_pair2(0x140, 0x5c);
        } else {
            void *g;
            /* a1 here holds 0x1000 from the compare prologue above. */
            i = 0;
            EXE_F0x8001ebf8(tbl.card);
            g = &D_80044B38;
            y = 0x27;
            do {
                id = ctx->cards_88[ctx->f78 + i];
                if (id != 0) {
                    tbl.fn_card(id);
                    v = tbl.fn_query();
                    /* PAL falls through on nonzero; reload the card byte after
                       the resource callback on the zero path. */
                    if (v != 0) {
                        tbl.mark1(tbl.card[0][0] - 1);
                        resource = (int32_t)GFXCTX();
                        tbl.draw(resource, 0x4b, 0x94, y);
                    } else {
                        tbl.mark1(0);
                        resource = (int32_t)GFXCTX();
                        tbl.draw(resource, tbl.card[0][0] + 0x4b, 0x94, y);
                    }
                    tbl.mark1(0);
                    DRAW( 0x31, 0x94, y);
                }
                i += 1;
                y += 0xe;
            } while (i < 8);
            if (ctx->f7c > 8) {
                int32_t *tickbase = D0x8004DE10;
                t = TICK();
                if (t - ctx->f84 > 8) {
                    ctx->f84 = TICK();
                    ctx->f80 = 1 - ctx->f80;
                }
                if (ctx->f80 != 0) {
                    if (ctx->f78 > 0)
                        DRAW_D( 0x45, 0x126, 0x22);
                    if (ctx->f78 < ctx->f7c - 8)
                        DRAW_D( 0x46, 0x126, 0x8f);
                }
            }
        }
        DRAW_D( 0x3f, 0x81, 0x1e);
    }

    if (ctx->f47c != 0) {
        if (ctx->f47c != 0x1000) {
            tbl.fn_scale3(ctx->f47c, 0x1000, 0x1000);
            tbl.fn_pair2(0x140, 0x25);
        }
        /* a1 at this call site is stale (0x25); callee arity unresolved. */
        EXE_F0x8001ebf8(tbl.card);
        id = ctx->cards_88[ctx->f78 + ctx->f74];
        tbl.fn_card(id);
        v = tbl.fn_query();
        /* Same branch-sense as B-phase: v!=0 falls through. */
        if (v != 0) {
            void *g = &D_80044B38;
            DRAW( v + 0x11, 0x103, 0x9f);
            DRAW( 0xd, 0xfc, 0x9d);
            DRAW( 0xb, 0x4a, 0xb3);
        } else {
            void *g = &D_80044B38;
            v = tbl.card[0][0];
            resource = (int32_t)GFXCTX();
            v += 0x13;
            tbl.draw(resource, v, 0x103, 0x9f);
            DRAW( 0xd, 0xfc, 0x9d);
            if (id == 0x45 || id == 0x70 || id == 0x9b || id == 0xc6 ||
                id == 0xf1)
                DRAW_D( 0xb, 0x4a, 0xb3);
            else
                DRAW( 0xc, 0xc7, 0xb3);
        }
        if (ctx->f47c != 0x1000)
            tbl.fn_pair2(0x140, 0xa8);
        DRAW_D( 10, 0x82, 0x9d);
    }

    if (ctx->f48c != 0) {
        if (ctx->f48c != 0x1000) {
            tbl.fn_scale3(0x1000, ctx->f48c, 0x1000);
            tbl.fn_pair2(0, 0x78);
        }
        tbl.fn_xy(ctx->f54, ctx->f58 - 3);
        DRAW_D( 0x36, 0, 100);
    }
}
