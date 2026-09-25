// CARDGAME:0x80099ed4, 140 bytes, PAL-SLES-03936.
// Exact with PsyQ GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79, O2 base.
// flagUnits counts nine-byte units: two steps advance one 18-byte slot.
// Keep the counter between those steps: loop discovers the IVs in PAL
// initialization order, then combine folds the two pointer advances.
// Evidence: cardgame-80099ed4/strategy-r11-astra.md in guide submissions.
#include "common/types.h"

extern void CARDGAME_F0x80099d18(void *a0);
extern void CARDGAME_F0x80099de8(void *a0);

void CARDGAME_F0x80099ed4(uint8_t *a0)
{
    unsigned int i;
    unsigned int flagUnits = 0;
    uint8_t v;

    for (i = 0; i < 12; flagUnits++, i++, flagUnits++) {
        v = (a0 + flagUnits * 9)[0xcf8];
        if (v != 0) {
            if (v == 2)
                CARDGAME_F0x80099d18(a0 + (i * 0x12 + 0xce8));
            CARDGAME_F0x80099de8(a0 + (i * 0x12 + 0xce8));
        }
    }
}
