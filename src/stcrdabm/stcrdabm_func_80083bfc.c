#include "common/types.h"

extern void *F0x8001ae38(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

/* STCRDABM:0x80083bfc (536 bytes, 134 instructions)
 * PAL-SLES-03936; recovered from the STCRDABM program at base 0x80082cb0
 * (reference/extracted/pro/stcrdabm.bin @ file offset 0xF4C; first word
 * 0x27bdffe0 matches the addiu sp,sp,-0x20 prologue).
 * Builds 17 EXE object handles via F0x8001ae38 into out[0..16], keyed by the
 * sign-extended halfword at arg0+0x50, then re-arms (count-2) entries of the
 * table at *(arg0+0x24) through each entry's slot 0x15c with
 * *(arg0+0x54)-3. Sole PAL caller: STCRDABM_func_80085540 (jal at 0x800855dc).
 * NOTE: `i = 0` after the 17 calls (not before) is required for the
 * prologue scheduling match (li a1/a2/a3 before saves, s2 cleared in the
 * first jal delay slot).
 */
void STCRDABM_func_80083bfc(int32_t arg0, int32_t *arg1) {
    int32_t *ptr;
    int32_t limit;
    int32_t i;
    int32_t obj;

    arg1[0] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x10, 0x19);
    arg1[1] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xd0, 0x19);
    arg1[2] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x34, 0x9e);
    arg1[3] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x35, 0x9e);
    arg1[4] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x4a, 0x9e);
    arg1[5] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x12, 0x6c);
    arg1[6] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x121, 0x6c);
    arg1[7] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x88, 0xa1);
    arg1[8] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x115, 0xa1);
    arg1[9] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x126, 0xa1);
    arg1[10] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x115, 0xc7);
    arg1[11] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x12c, 0xc7);
    arg1[12] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x50, 0xb8);
    arg1[13] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xce, 0xb8);
    arg1[14] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xf0, 0xb8);
    arg1[15] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xce, 0xc5);
    arg1[16] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xf0, 0xc5);
    i = 0;
    ptr = *(int32_t **)(arg0 + 0x24);
    limit = *(int32_t *)(arg0 + 0x20) - 2;
    if (limit > 0) {
        do {
            i++;
            obj = *ptr;
            ((void (*)(int32_t, int32_t))*(int32_t *)(obj + 0x15c))(obj, *(int32_t *)(arg0 + 0x54) - 3);
            ptr++;
        } while (i < (*(int32_t *)(arg0 + 0x20) - 2));
    }
}
