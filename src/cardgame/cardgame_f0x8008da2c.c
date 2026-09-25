// CARDGAME:0x8008da2c, 200-byte PAL body at verified base 0x80082cb0.
// Flag-selected halfword copy with counter bump, then indirect callback.
// Boundary: HIGH (prologue addiu sp,-0x18, jr ra at 0x8008daec, size 200).
// Ghidra: ddw3-pal-sles-03936 CARDGAME, disasm/decompile/xref read-only.
// Best shape (H11/H15: 195/200 bytes match, 5-byte diff): factored address,
// switch cascade, named dst (8d938/9c9a0/8bc8c idiom), increment-assign call
// argument, indirect calls in branches (lesson 006 tail-merge).
// Blocker: prologue sum accumulates into v1 not v0 (see handoff for the
// rejected hypothesis log). O2/G0 base variant,
// psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79.

#include <stdint.h>

/* Indirect target recovered from PAL: lw v0,0xea0(a0) then jalr v0 with
   (obj, flag 0/1, 7, sign-extended short counter). Conservative signature. */
typedef void (*cardgame_da2c_cb_t)(void *obj, int32_t flag, int32_t unk7, int32_t value);

void CARDGAME_F0x8008da2c(uint8_t *base, void *obj, int32_t ia, int32_t ib) {
    uint8_t *elem;
    uint8_t *dst;
    int16_t cval;
    int16_t vval;
    int32_t tia;
    int32_t tib;

    tib = ib * 7;
    tia = ia * 57;
    elem = base + (tia + tib) * 2;
    switch (elem[0x738]) {
    case 0:
        cval = *(int16_t *)(base + 0x5a2);
        vval = *(int16_t *)(elem + 0x72e);
        dst = base + cval * 2;
        *(int16_t *)(dst + 0x614) = vval;
        ((cardgame_da2c_cb_t)*(void **)((uint8_t *)obj + 0xea0))(obj, 0, 7, (*(int16_t *)(base + 0x5a2) += 1));
        break;
    case 1:
        cval = *(int16_t *)(base + 0x66a);
        vval = *(int16_t *)(elem + 0x72e);
        dst = base + cval * 2;
        *(int16_t *)(dst + 0x6dc) = vval;
        ((cardgame_da2c_cb_t)*(void **)((uint8_t *)obj + 0xea0))(obj, 1, 7, (*(int16_t *)(base + 0x66a) += 1));
        break;
    default:
        break;
    }
}
