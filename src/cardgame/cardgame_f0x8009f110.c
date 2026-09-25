/*
 * CARDGAME:0x8009f110 CARDGAME_F0x8009f110
 * 1968 bytes at CARDGAME.PRO offset 0x1c460 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009f110, jump table (.rodata) at 0x80083890
 *  Symbols     CARDGAME_F0x80096180=0x80096180 CARDGAME_F0x80096828=0x80096828
 *              CARDGAME_F0x8009d9e4=0x8009d9e4 CARDGAME_F0x8009edcc=0x8009edcc
 *              CARDGAME_F0x8009f110=0x8009f110 DAT_80048D34=0x80048d34
 *              DAT_8004B7D0=0x8004b7d0 DAT_8004df9c=0x8004df9c
 *              DAT_800A5C74=0x800a5c74 DAT_800A5D48=0x800a5d48
 *              D_80055c48=0x80055c48
 *  Compare     1968 bytes from 0x8009f110 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009f110
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936; complete body 1968 bytes (0x7b0).
 *
 * Text SHA-256:
 * 237fe4739c286fe8e939838637d7e51771ceaa099fae15c170732ce129a6335b.
 *
 * Jump table: 22 entries / 88 bytes at 0x80083890, included in exact
 * validation.
 *
 * Native recipe: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, O2/G0, base.
 *
 * Phase byte st+0x2f9 controls object setup, timed transitions, controller
 * input, and the final 40-card copy / index-array initialization. The caller at
 * 0x800a1c80 consumes the 0/1 return. Frame: 0x40; 6 direct and 37 indirect
 * calls.
 *
 * The EXE sound vector at 0x80055c48 points to 0x8002006c, whose PAL paths
 * return an integer. Ignoring that real return must not become a void
 * prototype.
 *
 * The selected deck uses 51 halfwords per row (0x66 bytes); its index at
 * st+0x2ea is reloaded after each destination store, as in the PAL loop.
 *
 * Views below describe only the accessed offsets, not complete runtime objects.
 *
 * Keep per-case phase stores, sequential counter lifetimes, and the local
 * method load before selecting the shared receiver: factoring them changes
 * codegen.
 */

#include <stdint.h>

typedef struct {
    int32_t prefix[3];
    int32_t slots[3];
} cardgame_context_slots_view;

typedef struct {
    uint8_t prefix[0x63e];
    int16_t cards[3 * 0x33];
} cardgame_deck_table_view;

typedef struct {
    uint8_t prefix[0x5b0];
    int16_t first[0x28];
    uint8_t gap[0x78];
    int16_t second[0x28];
} cardgame_order_arrays_view;

typedef struct {
    int16_t prefix[2];
    int16_t start;
    int16_t gap;
    int16_t count;
} cardgame_range_view;

typedef int32_t (*cardgame_exe_vec_t)(uint32_t id);
typedef int32_t (*cardgame_get_t)(void);
typedef int32_t (*cardgame_pad0_t)(int32_t a0);
typedef int32_t (*cardgame_pad1_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_obj_fn_t)(void *);
typedef int32_t (*cardgame_obj_arg_fn_t)(void *obj, int32_t a1);
typedef void (*cardgame_st_fn_t)(void *a0, int32_t a1, int32_t a2);
typedef void (*cardgame_pair6_fn_t)(void *a0, int32_t a1, int32_t a2,
                                    int32_t a3, int32_t a4, int32_t a5);
typedef int32_t (*cardgame_deck_fn_t)(void *a0, void *a1, void *a2);

extern cardgame_exe_vec_t D_80055c48;
extern cardgame_get_t DAT_8004df9c;
extern int32_t DAT_8004B7D0[];
extern int32_t CARDGAME_F0x80096828(int32_t kind, int32_t h0, int32_t h1);
/* NOTE: canonical CARDGAME_F0x80096180 recovery declares void; PAL stores
   the jal-result v0 to ctx[2] at this call site, modeled here as int. */
extern int32_t CARDGAME_F0x80096180(int32_t h0, int32_t h1);
extern void CARDGAME_F0x8009d9e4(void *st, void *ctx);
extern void CARDGAME_F0x8009edcc(void *st, void *ctx);
extern int16_t DAT_800A5D48[];
extern uint16_t DAT_800A5C74[];
extern int16_t DAT_80048D34[];

int32_t CARDGAME_F0x8009f110(uint8_t *st, int32_t *ctx)
{
    int32_t ret = 0;
    uint8_t phase = *(st + 0x2f9);
    int32_t acc;
    int32_t elapsed;
    uint8_t *receiver;
    int16_t idx;

    switch (phase) {
    case 0:
    default: {
        int32_t *p = (int32_t *)ctx[0];
        if (p == 0) {
            *(st + 0x2f9) = (uint8_t)(*(st + 0x2f9) + 1);
            goto function_end;
        }
        if (*(int32_t *)((uint8_t *)p + 0xc) != 1)
            goto function_end;
        if (*(int32_t *)((uint8_t *)p + 0x50) == 0)
            goto function_end;
        *(st + 0x2f9) = (uint8_t)(*(st + 0x2f9) + 1);
        goto function_end;
    }
    case 1: {
        ((cardgame_pair6_fn_t) * (cardgame_pair6_fn_t *)(ctx[6] + 0xeac))(
            (void *)ctx[6], 5, 5, 0x41, 0, 0x26);
        D_80055c48(0x40019);
        ctx[3] = CARDGAME_F0x80096828(0, DAT_800A5D48[0], DAT_800A5D48[1]);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    }
    case 2:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 0xb;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 3:
        D_80055c48(0x40019);
        ctx[4] = CARDGAME_F0x80096828(1, DAT_800A5D48[2], DAT_800A5D48[3]);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 4:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 0xb;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 5:
        D_80055c48(0x40019);
        ctx[5] = CARDGAME_F0x80096828(2, DAT_800A5D48[4], DAT_800A5D48[5]);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 6:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 0xb;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 7:
        ctx[2] = CARDGAME_F0x80096180(DAT_800A5D48[0], DAT_800A5D48[1]);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 8:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 0xb;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(int16_t *)(st + 0x2ea) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 9: {
        uint32_t a, b, c, d;
        a = (uint32_t)((cardgame_pad0_t)DAT_8004B7D0[0xfd])(0);
        b = (uint32_t)((cardgame_pad1_t)DAT_8004B7D0[0x102])(0, 4);
        c = (uint32_t)((cardgame_pad0_t)DAT_8004B7D0[0xff])(0);
        d = (uint32_t)((cardgame_pad1_t)DAT_8004B7D0[0x102])(0, 4);
        if (((a & (1u << b)) | (c & (1u << d))) != 0) {
            if (0 < *(int16_t *)(st + 0x2ea)) {
                *(int16_t *)(st + 0x2ea) =
                    (int16_t)(*(int16_t *)(st + 0x2ea) - 1);
                D_80055c48(0x4001b);
            }
        } else {
            uint32_t e, f, g, h;
            e = (uint32_t)((cardgame_pad0_t)DAT_8004B7D0[0xfd])(0);
            f = (uint32_t)((cardgame_pad1_t)DAT_8004B7D0[0x102])(0, 6);
            g = (uint32_t)((cardgame_pad0_t)DAT_8004B7D0[0xff])(0);
            h = (uint32_t)((cardgame_pad1_t)DAT_8004B7D0[0x102])(0, 6);
            if (((e & (1u << f)) | (g & (1u << h))) != 0) {
                if (*(int16_t *)(st + 0x2ea) < 2) {
                    *(int16_t *)(st + 0x2ea) =
                        (int16_t)(*(int16_t *)(st + 0x2ea) + 1);
                    D_80055c48(0x4001b);
                }
            } else {
                int32_t v = ((cardgame_pad0_t)DAT_8004B7D0[0xfd])(0);
                int32_t w = ((cardgame_pad1_t)DAT_8004B7D0[0x102])(0, 0xd);
                if (((v >> w) & 1) != 0) {
                    int32_t *slot;
                    int32_t *c2;
                    D_80055c48(0x4001c);
                    *(int32_t *)(st + 0x2fc) = 0;
                    *(st + 0x2f9) = (uint8_t)(*(st + 0x2f9) + 1);
                    idx = *(int16_t *)(st + 0x2ea);
                    slot = (int32_t *)((cardgame_context_slots_view *)ctx)->slots[idx];
                    (*(cardgame_obj_fn_t *)((uint8_t *)slot + 0x6c))(slot);
                    c2 = (int32_t *)ctx[2];
                    (*(cardgame_obj_fn_t *)((uint8_t *)c2 + 0x70))(c2);
                }
            }
        }
        {
            int16_t *base = DAT_800A5D48;
            int32_t h0, h1;
            void *c2;
            cardgame_st_fn_t fn;
            idx = *(int16_t *)(st + 0x2ea);
            h0 = base[idx * 2];
            h1 = base[idx * 2 + 1];
            c2 = (void *)ctx[2];
            fn = *(cardgame_st_fn_t *)((uint8_t *)c2 + 0x68);
            receiver = (uint8_t *)c2;
            fn(receiver, h0, h1);
            goto function_end;
        }
    }
    case 10:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 0xf;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 0xb: {
        int32_t *c2 = (int32_t *)ctx[2];
        (*(cardgame_obj_fn_t *)((uint8_t *)c2 + 0x6c))(c2);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    }
    case 0xd: {
        int32_t *c3;
        D_80055c48(0x4001a);
        c3 = (int32_t *)ctx[3];
        (*(cardgame_obj_fn_t *)((uint8_t *)c3 + 0x70))(c3);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    }
    case 0xe:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 4;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 0xf: {
        int32_t *c4;
        D_80055c48(0x4001a);
        c4 = (int32_t *)ctx[4];
        (*(cardgame_obj_fn_t *)((uint8_t *)c4 + 0x70))(c4);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    }
    case 0x10:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 4;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 0x11: {
        int32_t *c5;
        D_80055c48(0x4001a);
        c5 = (int32_t *)ctx[5];
        (*(cardgame_obj_fn_t *)((uint8_t *)c5 + 0x70))(c5);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    }
    case 0x12:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 4;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 0x13: {
        int32_t card = ctx[6];
        ((cardgame_obj_arg_fn_t) * (cardgame_obj_arg_fn_t *)(card + 0xeb0))(
            (void *)card, 5);
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    }
    case 0xc:
    case 0x14:
        elapsed = DAT_8004df9c();
        acc = *(int32_t *)(st + 0x2fc);
        acc += elapsed;
        *(int32_t *)(st + 0x2fc) = acc;
        acc = acc < 6;
        if (acc)
            goto function_end;
        phase = *(st + 0x2f9);
        *(int32_t *)(st + 0x2fc) = 0;
        *(st + 0x2f9) = (uint8_t)(phase + 1);
        goto function_end;
    case 0x15: {
        int32_t i;
        uint8_t *cursor;
        int16_t probe;
        i = 0;
        cursor = (uint8_t *)DAT_80048D34;
        idx = *(int16_t *)(st + 0x2ea);
        probe = ((cardgame_deck_table_view *)cursor)->cards[idx * 0x33];
        if (probe != 0) {
            uint8_t *table = cursor;
            cursor = st;
            for (; i < 0x28; i++) {
                int32_t t = *(int16_t *)(st + 0x2ea) * 0x33 + i;
                *(int16_t *)(cursor + 0x298) = (int16_t)(
                    *(int16_t *)(t * 2 + table + 0x63e) - 1);
                cursor += 2;
            }
        } else {
            uint8_t *dst;
            cursor = (uint8_t *)DAT_800A5C74;
            dst = st;
            for (; i < 0x28; i++) {
                *(uint16_t *)(dst + 0x298) = *(uint16_t *)cursor;
                cursor += 2;
                dst += 2;
            }
        }
        {
            int32_t card = ctx[6];
            uint8_t r = ((cardgame_deck_fn_t) * (cardgame_deck_fn_t *)
                (card + 0xf44))((void *)(st + 0x50), (void *)(st + 0x298),
                                (void *)(st + 0x248));
            *(st + 0x244) = r;
            for (i = 0; i < 0x28; i++) {
                ((cardgame_order_arrays_view *)st)->first[i] = (int16_t)i;
                ((cardgame_order_arrays_view *)st)->second[i] = (int16_t)(i + 0x28);
            }
        }
        *(st + 0x300) = 0;
        CARDGAME_F0x8009d9e4(st, ctx);
        *(int16_t *)(st + 0x66c) = 0x28;
        *(int16_t *)(st + 0x5a4) = 0x28;
        *(int16_t *)(st + 0x668) = 0;
        *(int16_t *)(st + 0x5a0) = 0;
        CARDGAME_F0x8009edcc(st, ctx);
        {
            cardgame_range_view *range;
            int32_t h0, h1;
            cardgame_st_fn_t fn;
            receiver = st;
            range = (cardgame_range_view *)(receiver + 0x59c);
            h0 = range->start;
            h1 = range->count;
            fn = (cardgame_st_fn_t) * (void **)(receiver + 0x810);
            ret = 1;
            fn(receiver, h0, h1);
            goto function_end;
        }
    }
    }

function_end:
    return ret;
}
