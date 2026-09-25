/* CARDGAME:0x8008c5f0, full 240-byte PAL body at base 0x80082cb0.
 * Read-only Ghidra CARDGAME: all 60 body words and predecessor verified.
 * Caller: CARDGAME:0x80085b00. Direct callees: 0x80085fd0, 0x8008735c.
 * The for header preserves the initial and repeated signed count tests and
 * reproduces i=s1 / strength-reduced p+2*i=s2 with native O2/base.
 * GCC 2.8.1 SN32 4.0.0010 + ASPSX 2.79: exact 240 bytes, no padding.
 * Evidence: reports/handoffs/cardgame-8008c5f0-c-recovery.md.
 */
#include "common/types.h"

extern int32_t CARDGAME_F0x80085fd0(void *a0, void *a1, int32_t a2);
extern void CARDGAME_F0x8008735c(void *a0, void *a1, void *a2, int32_t a3, int32_t a4);

int32_t CARDGAME_F0x8008c5f0(unsigned char *p, void *a1)
{
    int32_t i;

    for (i = 0; i < *(int16_t *)(p + 0x66e); i++) {
        int16_t v;
        unsigned char *t;

        *(p + i + 0x46f) = 0;
        if ((uint32_t)p[0x444] < 6 &&
            CARDGAME_F0x80085fd0(p, p + 0x670, *(int16_t *)(p + i * 2 + 0x6c8)) != 0) {
            v = *(int16_t *)(p + i * 2 + 0x6c8);
            t = p + (v - 0x28) * 4;
            if (t[0x35c] != 5) {
                CARDGAME_F0x8008735c(p, a1, p + 0x664, 0, v);
                *(p + i + 0x46f) = 1;
                p[0x444] = p[0x444] + 1;
            }
        }
    }
    return 1;
}
