#include "common/types.h"

/* STCRDSHP:0x80087d00 (396B, off 0x5050, base 0x80082cb0, range 0x80087d00-0x80087e8c).
 * PAL bytes verified against reference/extracted/pro/stcrdshp.bin slice (sha256
 * 188cf415cdc5599332108f58efcdb827fa400ce1b337e440198590a5e52e962a).
 * Ghidra ddw3-pal-sles-03936/STCRDSHP (read-only). Caller STCRDSHP_func_80088b50
 * @ 0x80088be8 passa (object, out-buffer).
 * Status: candidato exact_byte_match no pipeline (H16). Catalogo permanece
 * C_NONMATCHING ate o pai promover. backedge + 0xe0000; do-while(0) recoloca
 * NOTE_INSN_LOOP_END antes do setup de F0x8001b364.
 */

extern void *F0x8001ae38(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
extern void *F0x8001b364(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

void STCRDSHP_func_80087d00(uint8_t *a0, void **a1)
{
    void **s0;
    void *elem;
    void *obj;
    int32_t s3;
    int32_t base;
    int32_t cur;

    a1[0] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 1, 0x1d, 0x16);
    s3 = 0;
    base = 0x350000;
    a1[1] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 1, 0xd3, 0xcc);
    a1[4] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 3, 0x117, 0x1c);
    a1[3] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 3, 0x11a, 0x1c);
    s0 = a1;
loop1:
    cur = base >> 16;
    obj = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 1, 0xa7, cur);
    s0[5] = obj;
    s0 += 1;
    s3 += 1;
    base += 0xe0000;
    if (s3 < 3) {
        goto loop1;
    }
    do {
    } while (0);
    base = 0;
    obj = F0x8001b364((int32_t)*(int16_t *)(a0 + 0x50),
        *(int32_t *)(a0 + 0x54) - 2, 0xa7,
        ((*(int32_t *)(a0 + 0x6c) * 14 + 0x35) << 16) >> 16);
    a1[8] = obj;
    ((void (*)(void *, int32_t)) *(void **)((char *)obj + 0x80))(obj, base);
    a1[2] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 1, 0x9a, 0x71);
    s0 = *(void ***)(a0 + 0x24);
    if (base < *(int32_t *)(a0 + 0x20) - 3) {
        do {
            base += 1;
            elem = *s0;
            ((void (*)(void *, int32_t)) *(void **)((char *)elem + 0x15c))
                (elem, *(int32_t *)(a0 + 0x54) - 2);
            s0 += 1;
        } while (base < *(int32_t *)(a0 + 0x20) - 3);
    }
}
