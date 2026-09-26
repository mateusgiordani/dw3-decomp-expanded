/*
 * CARDGAME:0x800a3934 CARDGAME_F0x800a3934
 * 1116 bytes at CARDGAME.PRO offset 0x20c84 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a3934
 *  Symbols     EXE_F0x8001ebf8=0x8001ebf8
 *  Compare     1116 bytes from 0x800a3934 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a3934
 */

#include <stdint.h>

typedef struct {
    int16_t index;
    int16_t value;
} CardgamePair3934;

typedef struct {
    uint8_t *data;
    uint8_t unknown04[40];
    void (*select)(int32_t value);
    uint8_t unknown30[36];
} CardgameWork3934;

extern void EXE_F0x8001ebf8(void *work);

int32_t CARDGAME_F0x800a3934(uint8_t *context, int32_t side, int32_t mask)
{
    CardgamePair3934 entries[6];
    CardgamePair3934 temporary;
    CardgameWork3934 work;
    int32_t index;
    int32_t count;
    int32_t groupCount;
    int32_t skip;
    int32_t groupFirst;
    int32_t groupSecond;
    int32_t totalFirst = 0;
    int32_t totalSecond = 0;
    int32_t bonusFirst = 0;
    int32_t bonusSecond = 0;

    {
        int32_t countOffset = side * 114;
        count = *(context + countOffset + 0x72c);
    }
    EXE_F0x8001ebf8(&work);

    for (index = 0; index < count; index++) {
        int32_t recordOffset = side * 114 + index * 14;
        int32_t valueOffset = *(int16_t *)(context + recordOffset + 0x72e) * 2;
        int16_t value = *(int16_t *)(context + valueOffset + 0x50);
        entries[index].index = index;
        entries[index].value = value;
    }

    for (index = 0; index < count - 1; index++) {
        for (groupCount = index + 1; groupCount < count; groupCount++) {
            if (entries[index].value > entries[groupCount].value) {
                temporary = entries[index];
                entries[index] = entries[groupCount];
                entries[groupCount] = temporary;
            }
        }
    }

    groupCount = 0;
    {
        int32_t rowOffset = entries[0].index;
        uint8_t *row;
        index = (mask >> rowOffset) & 1;
        rowOffset *= 14;
        row = context + (rowOffset + side * 114);
        skip = 0;
        groupFirst = *(int16_t *)(row + 0x734);
        groupSecond = *(int16_t *)(row + 0x736);
    }
    for (; index < count - 1;) {
        int32_t compareStep;
        CardgamePair3934 *currentEntry = &entries[index];
        work.select(currentEntry->value + 1);
        if (((mask >> entries[index + 1].index) & 1) != 0) {
            if (index == count - 2) {
                break;
            }
            skip = 1;
        }

        if (*(int16_t *)(work.data + 10) != 0 &&
            currentEntry->value == entries[(compareStep = skip + 1, index + compareStep)].value) {
            int32_t rowOffset;
            uint8_t *row;
            groupCount++;
            rowOffset = entries[index + skip].index * 14;
            row = context + (rowOffset + side * 114);
            groupFirst += *(int16_t *)(row + 0x734);
            groupSecond += *(int16_t *)(row + 0x736);
        } else {
            if (groupCount < 2) {
                int32_t nextStep = skip + 1;
                int32_t restartIndex;
                groupCount = 0;
                restartIndex = index + nextStep;
                if (restartIndex < count) {
                    int32_t rowOffset = entries[restartIndex].index * 14;
                    uint8_t *row = context + (rowOffset + side * 114);
                    groupFirst = *(int16_t *)(row + 0x734);
                    groupSecond = *(int16_t *)(row + 0x736);
                } else {
                    groupFirst = 0;
                    groupSecond = 0;
                }
            } else {
                int32_t nextStep = skip + 1;
                int32_t restartIndex;
                bonusFirst += groupFirst;
                bonusSecond += groupSecond;
                restartIndex = index + nextStep;
                if (restartIndex < count) {
                    int32_t rowOffset = entries[restartIndex].index * 14;
                    uint8_t *row = context + (rowOffset + side * 114);
                    groupFirst = *(int16_t *)(row + 0x734);
                    groupSecond = *(int16_t *)(row + 0x736);
                } else {
                    groupFirst = 0;
                    groupSecond = 0;
                }
                if (groupCount >= 3) {
                    groupCount = 0;
                    bonusFirst += 20;
                    bonusSecond += 20;
                    break;
                }
                groupCount = 0;
            }
        }

        if (skip != 0) {
            index++;
        }
        index++;
        skip = 0;
    }

    if (groupCount >= 2) {
        bonusFirst += groupFirst;
        bonusSecond += groupSecond;
        if (groupCount >= 3) {
            bonusFirst += 20;
            bonusSecond += 20;
        }
    }

    for (index = 0; index < count; index++) {
        if (((mask >> index) & 1) == 0) {
            totalFirst += *(int16_t *)(context + (side * 114 + index * 14) + 0x734);
            totalSecond += *(int16_t *)(context + (side * 114 + index * 14) + 0x736);
        }
    }

    return totalFirst + totalSecond + bonusFirst + bonusSecond;
}
