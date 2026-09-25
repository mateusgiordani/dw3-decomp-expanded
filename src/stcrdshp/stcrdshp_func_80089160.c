#include "common/types.h"

/* STCRDSHP:0x80089160 (size 220, 0xDC).
 * PAL: reference/extracted/pro/stcrdshp.bin base 0x80082cb0 file-off 0x64b0.
 * Ghidra ddw3-pal-sles-03936/STCRDSHP read-only: disasm 55 insns word-equal
 * to PAL; leaf function (no jal/jalr). Boundary: jr ra + delay addu at
 * 0x80089234/0x80089238, next function STCRDSHP:0x8008923c. The 108 bytes
 * before this entry (0x800890f4..0x80089160) are a separate leaf function
 * ending in its own jr ra, not a prefix of this one.
 * Logic: linear search of the 12-byte table at 0x8008cdd4 for the entry whose
 * word0 == id, stopping at the -1 terminator; on a miss the index falls back
 * to 0. The entry's word1 (count) is zeroed and recounted as the length of the
 * zero-terminated int16 list at word2. Returns the entry pointer.
 * Revision 5 (GCC 2.8.1 SN32 / ASPSX 2.79 O2, exact): the search is a for
 * loop over the index. GCC copies its test to the entry (the tbl[0].id == -1
 * branch with i = 0 in the delay slot) and the loop optimizer puts a derived
 * table pointer and the -1 terminator in pre-header registers, which is the
 * PAL shape; the earlier pointer do-while compiled to a rotated loop one word
 * shorter. */
typedef struct {
    int32_t id;
    int32_t count;
    int16_t *items;
} STCRDSHP_80089160_Entry;

extern STCRDSHP_80089160_Entry STCRDSHP_D_8008CDD4[];

STCRDSHP_80089160_Entry *STCRDSHP_func_80089160(int32_t id) {
    int32_t i;
    int32_t found;
    int32_t n;

    found = -1;
    for (i = 0; STCRDSHP_D_8008CDD4[i].id != -1; i++) {
        if (STCRDSHP_D_8008CDD4[i].id == id) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        found = 0;
    }
    STCRDSHP_D_8008CDD4[found].count = 0;
    n = 0;
    while (STCRDSHP_D_8008CDD4[found].items[n] != 0) {
        n++;
        STCRDSHP_D_8008CDD4[found].count++;
    }
    return &STCRDSHP_D_8008CDD4[found];
}
