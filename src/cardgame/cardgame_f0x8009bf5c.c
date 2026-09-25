// CARDGAME:0x8009bf5c (size 112, 0x70)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x192ac
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #155
// Prologue 27bdffe0 addiu sp,-0x20 ; sw s0,0x10(sp) ; move s0,a0 ; sw s1,0x14(sp) ; move s1,a1
// Setup lui a0,0x4 ; lui v0,0x8005 ; lw v0,0x5c48(v0) ; sw ra,0x18(sp) ; jalr v0 ; ori a0,0x1a (delay, a0=0x4001a)
// Body sll v0,s1,1 ; addu v0,v0,s1 ; sll v0,v0,3 (v0 = s1*24) ; addu s0,s0,v0
// sh 0x1000 at +0xe10/+0xe12 ; sb 3 at +0xe23 ; sh 6 at +0xe16/+0xe14 ; restore ; jr ra ; addiu sp,+0x20
// Prev CARDGAME:0x8009beac (176B) ends exactly at 0x8009bf5c ; next framed CARDGAME:0x8009c054 at +0xf8 (0x88 leaf gap)
// Ghidra ddw3-pal-sles-03936/CARDGAME disasm matches PAL words byte-for-word (28 insns) ; decompile:
//   (*_DAT_80055c48)(0x4001a); param_1 += param_2 * 0x18;
//   *(short *)(p+0xe10)=0x1000; *(short *)(p+0xe12)=0x1000; *(char *)(p+0xe23)=3;
//   *(short *)(p+0xe16)=6; *(short *)(p+0xe14)=6;
// Xref: DATA ref from CARDGAME_F0x8009d310+0x5c (sw v1,0xeb0(v0) with v1 = 0x800a0000-0x40a4 = 0x8009bf5c):
//   0x8009bf5c is registered as the callback at table slot +0xeb0 (no direct jal callers).
// Callee *0x80055c48 is EXE-resident shared vector (same idiom across 0x8009c0dc/0x8009c124/0x8009c1cc/0x8009c214 family).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base hypothesis).

typedef void (*cardgame_vec_t)(int);

#define CARDGAME_VEC (*(cardgame_vec_t *)0x80055c48)

void CARDGAME_F0x8009bf5c(char *base, int index)
{
    char *entry;

    CARDGAME_VEC(0x4001a);
    entry = base + index * 24;
    *(short *)(entry + 0xE10) = 0x1000;
    *(short *)(entry + 0xE12) = 0x1000;
    entry[0xE23] = 3;
    *(short *)(entry + 0xE16) = 6;
    *(short *)(entry + 0xE14) = 6;
}
