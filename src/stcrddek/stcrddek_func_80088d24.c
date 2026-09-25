/* STCRDDEK:0x80088d24, PAL-SLES-03936, full body 336 bytes.
 * exact_byte_match 336/336 with psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
 * The receiver/method expression lives inside each first call, so expand_call emits the
 * row arithmetic after the constant argument (a2 = -1 / a1 = 0); the child-loop state
 * (fieldOffset/count/cursor, count/hideCursor) is assigned after the call and hoisted by sched1.
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
