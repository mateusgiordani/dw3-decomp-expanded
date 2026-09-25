/*
 * STCRDDEK:0x80088d24 STCRDDEK_func_80088d24
 * 336 bytes at STCRDDEK.PRO offset 0x6074 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088d24
 *  Symbols     D_8004935C=0x8004935c STCRDDEK_func_80088d24=0x80088d24
 *  Compare     336 bytes from 0x80088d24 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80088d24
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936, full body 336 bytes.
 *
 * The receiver/method expression lives inside each first call, so expand_call
 * emits the row arithmetic after the constant argument (a2 = -1 / a1 = 0); the
 * child-loop state (fieldOffset/count/cursor, count/hideCursor) is assigned
 * after the call and hoisted by sched1.
 */

extern char D_8004935C[];

void STCRDDEK_func_80088d24(int context, int objects, int field, int mode)
{
    int receiver;
    int argument;
    int fieldOffset;
    int cursor;
    int count;
    int slot;
    int hideCursor;
    unsigned int value;
    int address;

    receiver = field;
    if (mode != 0) {
        address = receiver * 2;
        address += receiver;
        value = (unsigned int)address << 4;
        {
            unsigned int shifted = value;
            value = address + shifted;
        }
        value <<= 1;
        argument = (int)D_8004935C;
        argument = value + argument;
        ((unsigned char *(*)(int, char *, int))*(int *)(*(int *)(objects + ((receiver << 3) - receiver << 2) + 8) + 0x114))(*(int *)(objects + ((receiver << 3) - receiver << 2) + 8), (char *)argument, -1);
        fieldOffset = address * 8;
        count = 0;
        cursor = receiver * 28;
        for (; count < 6; count++) {
            slot = objects + cursor;
            receiver = *(int *)(slot + 12);
            ((void (*)(int, int, int))*(int *)(receiver + 0x118))(receiver, 0, *(int *)(context + fieldOffset + 0x74));
            fieldOffset += 4;
            cursor += 4;
            receiver = *(int *)(slot + 12);
            ((void (*)(int, int))*(int *)(receiver + 0x148))(receiver, 1);
        }
    } else {
        ((void (*)(int, int))*(int *)(*(int *)(objects + ((receiver << 3) - receiver << 2) + 8) + 0x144))(*(int *)(objects + ((receiver << 3) - receiver << 2) + 8), 0);
        count = 0;
        hideCursor = receiver * 28;
        for (; count < 6; count++) {
            receiver = *(int *)(objects + hideCursor + 12);
            ((void (*)(int, int))*(int *)(receiver + 0x144))(receiver, 0);
            hideCursor += 4;
        }
    }
}
