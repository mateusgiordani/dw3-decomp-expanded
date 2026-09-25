/*
 * CARDGAME:0x80090058 CARDGAME_F0x80090058
 * 1812 bytes at CARDGAME.PRO offset 0xd3a8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80090058, jump table (.rodata) at 0x8008353c
 *  Symbols     CARDGAME_F0x80090058=0x80090058 DAT_8004DF9C=0x8004df9c
 *              EXE_FUNC_8001EBF8=0x8001ebf8
 *  Compare     1812 bytes from 0x80090058 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80090058
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Xref: 2 callers in CARDGAME_F0x80084320 via 0x800855d0 (a3=3) and 0x800855f4
 * (a3=4) UNCONDITIONAL_CALL (jal 0x80090058); jump-table DATA ref to 0x8008353c
 * indexed by (*(u8*)(p1+0x422))-1 with 6 computed targets
 * (0x800900bc/0x80090190/0x8009059c/0x800906a4/0x800906f4/0x80090740), default
 * returns 0.
 *
 * Codegen notes: switch on state byte p1+0x422 (values 1..6, s5 return flag,
 * case6 sets 1); per-side stride 200 (0xc8) at p1+p3*200 and per-slot stride 76
 * (0x4c) at p2+idx*76; indirect table calls via
 * p2+0xed8/0xf08/0xea0/0xebc/0xf1c plus EXE jal 0x8001ebf8 filling stack buffer
 * (fn at sp+0x44, base at sp+0x18) and EXE indirect via word at 0x8004df9c (lui
 * 0x8005 + lw -0x2064 + jalr).
 */

#include <stdint.h>

typedef struct {
    uint8_t prefix[0x5b0];
    int16_t cards[0x28];
} cardgame_deck_order_view;

typedef struct { int8_t first, second; } cardgame_pair_t;

/* Observed member at +0x14a with a 76-byte stride; minimum view, not capacity. */
typedef struct {
    uint8_t prefix[0x14a];
    uint8_t slot_flags[1][76];
} cardgame_slot_flags_view;

typedef int32_t (*cardgame_cb2_t)(int32_t, int32_t);
typedef int32_t (*cardgame_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef int32_t (*cardgame_cb4_t)(int32_t, int32_t, int32_t, int32_t);
typedef int32_t (*cardgame_cb1_t)(int32_t);
typedef void (*cardgame_void2_t)(int32_t, int32_t);
typedef void (*cardgame_void1_t)(int32_t);

typedef struct {
    uint8_t *items;
    uint8_t _pad[40];
    int32_t (*fn)(int32_t);
    uint8_t tail[0x24];
} cardgame_lut_t;

extern void EXE_FUNC_8001EBF8(cardgame_lut_t *out);
extern int32_t DAT_8004DF9C[];

int32_t CARDGAME_F0x80090058(int32_t p1, int32_t p2, int32_t p3, int32_t p4)
{
    int32_t ret = 0;
    int32_t idx200;
    int32_t slot;
    int32_t count;
    int32_t tmp;
    int32_t u;
    int32_t sid;
    cardgame_lut_t lut;
    int32_t state = *(uint8_t *)(p1 + 0x422);

    switch (state) {
    case 1: {
        int32_t slot;
        int32_t ok = 0;
        int32_t base;
        int32_t dst;
        int32_t r;
        if (p3 == 0)
            ok = (*(int16_t *)(p2 + 0x64) == 2);
        else if (*(int16_t *)(p2 + 0xb8) == 2)
            ok = 1;
        base = p1 + p3 * 200;
        slot = *(int16_t *)(base + 0x5a6) - 1;
        dst = p2 + slot * 76;
        *(int32_t *)(dst + 0x108) = 0x14a00;
        if (ok && (*(uint8_t *)(p1 + 0x498) == 0)) {
            r = ((cardgame_cb2_t)*(void **)(p2 + 0xed8))(*(int16_t *)(base + 0x5a6), slot);
            *(int16_t *)(dst + 0x120) = 0x1000;
            ((cardgame_cb5_t)*(void **)(p2 + 0xf08))(p2, slot, 15, r + 0x1800, 0x6100);
            *(uint8_t *)(p1 + 0x422) = 2;
        }
        break;
    }
    case 2: {
        int32_t twice_side = p3 * 2;
        int32_t base = p1 + ((twice_side + p3) * 8 + p3) * 8;
        slot = *(int16_t *)(base + 0x5a6) - 1;
        if (((cardgame_slot_flags_view *)p2)->slot_flags[slot][0] != 1)
            break;
        if (p4 == 4) {
            int32_t count;
            int32_t value;
            count = *(int32_t *)(p1 + 0x440);
            value = *(int16_t *)(base + 0x5a2) - 1;
            goto forward_test;
        forward_body:
            value = count + 1;
            count = value;
            idx200 = twice_side + p3;
            idx200 *= 8;
            idx200 += p3;
            idx200 *= 8;
            base += idx200;
            {
                int32_t row_word = count * 2;
                value = row_word + idx200;
            }
            {
                int32_t address_offset = value;
                value = p1 + address_offset;
            }
            {
                int32_t address_offset = base;
                base = p1 + address_offset;
            }
            *(int16_t *)(base + 0x614) = *(int16_t *)(value + 0x614);
            {
                int32_t row_offset = idx200;
                idx200 = p1 + row_offset;
            }
            value = *(int16_t *)(idx200 + 0x5a2) - 1;
        forward_test:
            base = count * 2;
            idx200 = count;
            value = idx200 < value;
            if (value)
                goto forward_body;
            {
                int32_t final_base = p1 + p3 * 200;
                *(int16_t *)(final_base + 0x5a2) = *(int16_t *)(final_base + 0x5a2) - 1;
            }
            goto set_timer;
        } else {
            if (p3 == 0) {
                int32_t n;
                for (n = *(int32_t *)(p1 + 0x440);
                     *(int16_t *)(p1 + 0x5a0) + 1 <= n; n--) {
                    ((cardgame_deck_order_view *)p1)->cards[n] =
                        ((cardgame_deck_order_view *)p1)->cards[n - 1];
                }
            } else {
                tmp = *(uint8_t *)(p1 + 0x41b);
                count = *(int32_t *)(p1 + 0x440);
                if (count < tmp) {
                    if (count < *(int16_t *)(p1 + 0x668) + 1) {
                        /* fall through to tail fill */
                    } else {
                        int32_t dsti, srci;
                    shift_visible:
                        dsti = count * 2;
                        count--;
                        srci = p1 + count * 2;
                        {
                            int32_t address_offset = dsti;
                            dsti = p1 + address_offset;
                        }
                        *(int16_t *)(dsti + 0x678) = *(int16_t *)(srci + 0x678);
                        *(cardgame_pair_t *)(dsti + 0x30a) =
                            *(cardgame_pair_t *)(srci + 0x30a);
                        if (*(int16_t *)(p1 + 0x668) + 1 <= count)
                            goto shift_visible;
                        goto fill_side1;
                    }
                    goto fill_side1;
                } else {
                    int32_t cur = p1 + count * 2;
                    if (*(uint8_t *)(cur + 0x30b) == 7) {
                        {
                            int32_t dst_offset, dsti, srci;
                            goto shift_seven_test;
                        shift_seven_body:
                            count--;
                            srci = p1 + count * 2;
                            dsti = p1 + dst_offset;
                            *(int16_t *)(dsti + 0x678) = *(int16_t *)(srci + 0x678);
                            *(cardgame_pair_t *)(dsti + 0x30a) =
                                *(cardgame_pair_t *)(srci + 0x30a);
                        shift_seven_test:
                            if (count < *(int16_t *)(p1 + 0x668) + 1)
                                goto shift_seven_done;
                            dst_offset = count * 2;
                            goto shift_seven_body;
                        shift_seven_done:;
                        }
                    } else {
                        int32_t limit;
                        uint16_t tail = *(uint16_t *)(p1 + 0x6c6);
                        int32_t save = *(int16_t *)(cur + 0x678);
                        *(uint16_t *)(cur + 0x678) = tail;
                        limit = *(int16_t *)(p1 + 0x668);
                        *(uint16_t *)(p1 + 0x6c6) = save;
                        count = 0x27;
                        {
                            int32_t dsti, srci;
                            goto shift_swapped_test;
                        shift_swapped_body:
                            dsti = count * 2;
                            count--;
                            srci = p1 + count * 2;
                            {
                                int32_t address_offset = dsti;
                                dsti = p1 + address_offset;
                            }
                            *(int16_t *)(dsti + 0x678) = *(int16_t *)(srci + 0x678);
                            *(cardgame_pair_t *)(dsti + 0x30a) =
                                *(cardgame_pair_t *)(srci + 0x30a);
                            limit = *(int16_t *)(p1 + 0x668);
                        shift_swapped_test:
                            if (limit + 1 <= count)
                                goto shift_swapped_body;
                        }
                    }
                    tmp = *(uint8_t *)(p1 + 0x41b) + 1;
                    u = *(uint8_t *)(p1 + 0x41c) + 1;
                    *(uint8_t *)(p1 + 0x41b) = tmp;
                    *(uint8_t *)(p1 + 0x41c) = u;
                    fill_side1:
                    {
                        int32_t v = 0x27;
                        int32_t ptr = p1 + 0x4e;
                        do {
                            *(int8_t *)(ptr + 0x30a) = (int8_t)v;
                            v--;
                            ptr -= 2;
                        } while (v >= 0);
                    }
                    goto case2_tail;
                }
            }
        case2_tail:
            {
                int32_t stride = p3 * 200;
                int32_t stats_base = p1 + stride;
                *(int16_t *)(stats_base + 0x5a0) = *(int16_t *)(stats_base + 0x5a0) + 1;
                *(int16_t *)(stats_base + 0x5a4) = *(int16_t *)(stats_base + 0x5a4) - 1;
                sid = *(int16_t *)(p1 + (slot * 2 + stride) + 0x600);
            }
            EXE_FUNC_8001EBF8(&lut);
            sid *= 2;
            {
                int32_t address_offset = sid;
                sid = p1 + address_offset;
            }
            lut.fn(*(int16_t *)(sid + 0x50) + 1);
            if (*lut.items > 5)
                goto set_timer;
            *(uint8_t *)(p1 + 0x422) = 3;
            ((cardgame_void2_t)*(void **)(p2 + 0xf1c))(p2, slot);
            goto update_stats;
        }
    set_timer:
        *(uint8_t *)(p1 + 0x422) = 4;
        *(int32_t *)(p1 + 0x424) = 45;
    update_stats:
        ((cardgame_cb4_t)*(void **)(p2 + 0xea0))(p2, 0, 5, *(int16_t *)(p1 + 0x5a4));
        ((cardgame_cb4_t)*(void **)(p2 + 0xea0))(p2, 0, 6, *(int16_t *)(p1 + 0x5a6));
        ((cardgame_cb4_t)*(void **)(p2 + 0xea0))(p2, 0, 7, *(int16_t *)(p1 + 0x5a2));
        ((cardgame_cb4_t)*(void **)(p2 + 0xea0))(p2, 1, 5, *(int16_t *)(p1 + 0x66c));
        ((cardgame_cb4_t)*(void **)(p2 + 0xea0))(p2, 1, 6, *(int16_t *)(p1 + 0x66e));
        ((cardgame_cb4_t)*(void **)(p2 + 0xea0))(p2, 1, 7, *(int16_t *)(p1 + 0x66a));
        break;
    }
    case 3: {
        int32_t base = p3 * 200;
        int32_t s = *(int16_t *)(p1 + base + 0x5a6) - 1;
        if (((cardgame_slot_flags_view *)p2)->slot_flags[s][0] == 1) {
            int32_t k;
            *(uint8_t *)(p1 + 0x422) = 4;
            *(int32_t *)(p1 + 0x424) = 45;
            sid = *(int16_t *)(p1 + (s * 2 + base) + 0x600);
            EXE_FUNC_8001EBF8(&lut);
            sid *= 2;
            {
                int32_t address_offset = sid;
                sid = p1 + address_offset;
            }
            lut.fn(*(int16_t *)(sid + 0x50) + 1);
            if (*lut.items < 6) {
                uint8_t cur;
                k = base - 1;
                cur = *(uint8_t *)(p1 + (*lut.items + k) + 0x5a8);
                if (cur < 99)
                    *(uint8_t *)(p1 + (*lut.items + k) + 0x5a8) = cur + 1;
                ((cardgame_cb4_t)*(void **)(p2 + 0xea0))(p2, p3, *lut.items - 1, *(uint8_t *)(p1 + (*lut.items + k) + 0x5a8));
            }
        }
        break;
    }
    case 4: {
        int32_t now = ((int32_t (*)(void))DAT_8004DF9C[0])();
        u = *(int32_t *)(p1 + 0x424) - now;
        *(int32_t *)(p1 + 0x424) = u;
        if (u < 1) {
            *(uint8_t *)(p1 + 0x422) = 5;
            ((cardgame_void2_t)*(void **)(p2 + 0xebc))(p2, p3);
            *(uint8_t *)(p1 + 0x499) = *(uint8_t *)(p1 + 0x49b);
        }
        break;
    }
    case 5: {
        int32_t ok = 0;
        if (p3 == 0)
            ok = (*(int16_t *)(p2 + 0x64) == 0);
        else if (*(int16_t *)(p2 + 0xb8) == 0)
            ok = 1;
        if (ok && (*(uint8_t *)(p1 + 0x498) == 0))
            *(uint8_t *)(p1 + 0x422) = 6;
        break;
    }
    case 6:
        ret = 1;
        break;
    default:
        break;
    }
    return ret;
}
