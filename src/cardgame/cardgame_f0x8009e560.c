// CARDGAME:0x8009e560 (size 204, 0xCC)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1b8b0
// sha256 f16027a2a15c1afbedc699e4cf375a561aafdd2851d94ea9ce6907e5c2abca98 (51 LE words)
// Framed: prologue addiu sp,-0x30, saves s0-s3/ra; epilogue jr ra / addiu sp,+0x30.
// Next function CARDGAME:0x8009e62c (addiu a1,a0,0x498 ...) starts at +0xCC: size self-contained.
// Prologue: lbu v0,0x306(a0); beqz v0 -> return (delay move s2,a1); flag-1 in s0;
//   lui v0,0x800a + addiu 0x5d14; sll s0,2; addu s0,v0 -> entry = DAT_800a5d14 + (flag-1)*4.
// Body: jal FUN_800a367c(entry[3]) (delay li s1,10); *(a1+4) = result;
//   method = *(result+0x68); method(result, entry[0..2]);
//   if (entry[3] == 2) s1 = 15; obj = *(a1+4) reload; method2 = *(obj+0x6c);
//   method2(obj, 0, 0, 0, s1, 1) with s1 -> 0x10(sp), 1 -> 0x14(sp); sb zero,0x306(a0).
// Ghidra ddw3-pal-sles-03936 program CARDGAME (read-only): disasm 51 words identical to PAL
//   (modulo BE/LE display); decompile matches this C line-for-line; FUN_800a367c decompile
//   confirms the object protocol (EXE func_0x80014504 allocator, methods installed at +0x68/+0x6c).
// Caller: 1 direct jal from CARDGAME FUN_800a2df8+0x3c (0x800a2e34, delay move a1,s1).
// Callee: FUN_800a367c direct (internal CARDGAME); 2 indirect jalr via +0x68/+0x6c.
// Table DAT_800a5d14 at file-off 0x23064 (inside cardgame.bin, 4 lbu bytes per entry).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base hypothesis).
#include "common/types.h"

extern uint8_t DAT_800a5d14[];
extern void *FUN_800a367c(uint8_t arg);

void CARDGAME_F0x8009e560(uint8_t *a0, void *a1)
{
    uint8_t *entry;
    int32_t level;
    uint32_t idx;
    uint32_t base;

    if (a0[0x306] == 0)
        return;
    idx = (uint32_t)(a0[0x306] - 1);
    base = (uint32_t)DAT_800a5d14;
    entry = (uint8_t *)(base + (idx << 2));
    level = 10;
    *(void **)((char *)a1 + 4) = FUN_800a367c(entry[3]);
    (*(void (**)(void *, uint8_t, uint8_t, uint8_t))((char *)(*(void **)((char *)a1 + 4)) + 0x68))(*(void **)((char *)a1 + 4), entry[0], entry[1], entry[2]);
    if (entry[3] == 2)
        level = 15;
    (*(void (**)(void *, int32_t, int32_t, int32_t, int32_t, int32_t))((char *)(*(void **)((char *)a1 + 4)) + 0x6c))(*(void **)((char *)a1 + 4), 0, 0, 0, level, 1);
    a0[0x306] = 0;
}
