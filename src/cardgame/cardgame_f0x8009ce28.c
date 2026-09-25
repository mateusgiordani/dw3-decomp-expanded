// CARDGAME:0x8009ce28 (size 108, 0x6c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1a178
// Portable C recovery (rev 2, cardgame-human-v1). Prior rev 1 was ASM_MATCHING
// noreorder wrapper (exact_byte_match, sha 062d3d61...); this rev replaces it
// with reviewed portable C per campaign (asm wrappers are not C).
// Ghidra CARDGAME read-only: disasm 27 words matches PAL LE words 1:1;
// decompile: (*_DAT_80055c48)(0x4001c); param_1 + param_2 * 0x4c;
//   *(param_1 + 0x14a) = 0xb; *(param_1 + 0x134) = 0; *(param_1 + 0x130) = 0;
// x-ref to 0x8009ce28: DATA ref from 0x8009d48c (CARDGAME_F0x8009d310), no direct
//   code caller; x-ref from: stack saves + READ 0x80055c48 (indirect target).
// Upstream cardgame.s GUIDE only. Rendering domain pack read-only:
//   build/cardgame-8009ce28-domain.json (no C copied).
// Match key: helper returns int32_t (not void) so GCC 2.8.1 -O2 keeps v0 live
// and allocates the 76*index temp to v1 ($3), matching PAL sll/addu/subu chain.
#include "common/types.h"

extern int32_t (*D_80055c48)(uint32_t);

int32_t CARDGAME_F0x8009ce28(uint8_t *base, int32_t index) {
    uint8_t *rec;

    (void)D_80055c48(0x4001c);
    rec = base + index * 76;
    rec[0x14a] = 11;
    *(int32_t *)(rec + 0x134) = 0;
    *(int32_t *)(rec + 0x130) = 0;
    return 0;
}
