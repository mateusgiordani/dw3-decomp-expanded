// CARDGAME:0x800a0754 (size 240, 0xF0) -- C_MATCHING (portable C, rev3)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1daa4
// Verified base 0x80082cb0, REFERENCE_VERSION PAL-SLES-03936.
// Next function CARDGAME:0x800a0844 at +0xF0 (d0ffbd27), confirms size.
// Ghidra ddw3-pal-sles-03936/CARDGAME read-only: disasm 60 words match PAL
// little-endian words 1:1 (verified); decompile shows p1/p2 base/index,
// pb = p1 + p2*0x72 +0x72c, p3 = p1 + p2*200 +0x59c, *pb=0, loop over
// *(int16*)(p3+10) with cur=p3 and index i. Condition: *pb < *(uint8_t*)(p1+0x444)
// (unsigned, sltu) && *(int8_t*)(p1+i+0x46f)!=0 (signed lb). Body jal
// 0x800a0708(p1,p2,*(int16*)(cur+100)), then cur+=2, i++. Tail jal
// 0x800a0590(p1,p3). Xref DATA at 0x800a328c in FUN_800a3240 installs this
// as function pointer at slot 0x818.
// Provenance: boundary sweep row #54, 240B, -0x30/+0x30 frame.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
// exact_byte_match, no alternates. Order matters: off1= p2*0x72+0x72c before
// p1 add (addiu v0,0x72c; addu v0,a0,v0) to keep constant before base;
// similarly off2; pb store as sb 0(pb) not folded offset; and loop increment
// order cur+=2 before i++ reproduces PAL lh/addiu/slt/bne scheduling with
// bne offset -17 and no extra nop. Signed int8_t on 0x46f path was required.
// No Ghidra state changed. No global symbols added.

#include <stdint.h>

extern void CARDGAME_F0x800a0708(int32_t a0, int32_t a1, int32_t a2);
extern void CARDGAME_F0x800a0590(int32_t a0, int32_t a1);

void CARDGAME_F0x800a0754(int32_t p1, int32_t p2)
{
    int32_t off1;
    int32_t off2;
    uint8_t *pb;
    int32_t p3;

    off1 = p2 * 0x72 + 0x72c;
    pb = (uint8_t *)(p1 + off1);
    off2 = p2 * 200 + 0x59c;
    p3 = p1 + off2;
    *pb = 0;
    if (*(int16_t *)(p3 + 10) > 0) {
        int32_t i = 0;
        int32_t cur = p3;
        do {
            if (*pb < *(uint8_t *)(p1 + 0x444) && *(int8_t *)(p1 + i + 0x46f) != 0) {
                CARDGAME_F0x800a0708(p1, p2, (int32_t)*(int16_t *)(cur + 100));
            }
            cur += 2;
            i++;
        } while (i < *(int16_t *)(p3 + 10));
    }
    CARDGAME_F0x800a0590(p1, p3);
}
