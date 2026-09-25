// CARDGAME:0x8008ce0c (size 464, 0x1D0)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xa15c (RAW, no header)
// Boundary: prologue 27bdff78 addiu sp,-0x88, saves s1/s3/s0/ra/s2 at
// 0x74/0x7c/0x70/0x80/0x78(sp); s1=a0, s3=a1, s0=a2; epilogue lw
// ra/s3/s2/s1/s0 + jr ra + addiu sp,+0x88 at 0x8008cfc0-0x8008cfd8.
// Next framed CARDGAME:0x8008cfdc at +0x1D0 (27bdffc8 prologue), size 0x1D0
// contiguous, no overlap.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 116
// words matches PAL; decompile CARDGAME_F0x8008ce0c(param_1,param_2,param_3)
// (hypothesis only): 5 indirect jalr via table in p2 (offsets 0xecc, 0xf14,
// 0xf3c, 0xf24, 0xec8), field stores at p1+0x440/0x422/0x438, halfword select
// via p1[0x575]*8+0x580, halfword clear at p2+0x594, 5-arg call (5th arg
// 0x1000 spilled at sp+0x10), 15-byte descending fill of 1 at p1+0x49e..0x4ac,
// zero at p1+0x4ad, 15-byte descending clear at p1+0x46f..0x47d, guarded card
// tail (direct jal to EXE 0x8001ebf8 with a0=sp+0x18, call through the word at
// sp+0x44, byte test against 6, *76 stride flag set at p2+0x150 with ori 1).
// cardgame.s is GUIDE only; never copied as source. No Ghidra state change.
// Callers: 2 direct jal from CARDGAME_F0x80084320 (0x80084d8c with a2=0,
// 0x80084da4 with a2=1, both jal word 0c023383); 1 direct callee EXE
// F0x8001ebf8 (0x8001ebf8, DISASSEMBLED; fills the 0x54 buffer including the
// callback word at +0x2c, read-only evidence only) plus 5 indirect table
// slots loaded from s3=p2.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match, no alternates needed.
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
// Reg note: the two static `flag = 1` sets (p3==0 inline + p3==1 gated) tail-merge
// into one `li s2,1`; the extra static set raises flag above p2 in the global
// pick order, yielding p1->s1, p2->s3, p3->s0, flag->s2 per PAL.
typedef void (*cardgame_8ce0c_cb1_t)(int32_t);
typedef void (*cardgame_8ce0c_cb3_t)(int32_t, int32_t, int32_t);
typedef void (*cardgame_8ce0c_cb4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_8ce0c_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_8ce0c_fn_t)(int32_t);

void F0x8001ebf8(void *buf);

void CARDGAME_F0x8008ce0c(int32_t p1, int32_t p2, int32_t p3)
{
    int i;
    int p;
    int flag;
    int h;
    int ha;
    int one;
    int k;
    uint8_t *u;
    uint8_t buf[0x54];

    ((cardgame_8ce0c_cb1_t)*(uint32_t *)(p2 + 0xecc))(p2);
    *(uint32_t *)(p1 + 0x440) = 0;
    *(uint8_t *)(p1 + 0x422) = 1;
    *(uint32_t *)(p1 + 0x438) = 0;
    ((cardgame_8ce0c_cb4_t)*(uint32_t *)(p2 + 0xf14))(p2, 0xf, 0xe500, 0x6100);
    ((cardgame_8ce0c_cb3_t)*(uint32_t *)(p2 + 0xf3c))(p2, 0xf, *(int16_t *)(p1 + (((int8_t *)p1)[0x575] << 3) + 0x580));
    *(uint16_t *)(p2 + 0x594) = 0;
    ((cardgame_8ce0c_cb5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 8, 0x1000, 0x1000);
    ((cardgame_8ce0c_cb1_t)*(uint32_t *)(p2 + 0xec8))(p2);
    one = 1;
    i = 0xe;
    p = p1 + i;
    do {
        *(uint8_t *)(p + 0x49e) = one;
        p--;
        i--;
    } while (i >= 0);
    *(uint8_t *)(p1 + 0x4ad) = 0;
    i = 0xe;
    p = p1 + i;
    do {
        *(uint8_t *)(p + 0x46f) = 0;
        p--;
        i--;
    } while (i >= 0);
    if (*(int8_t *)(p1 + 0x575) == 0)
        goto done;
    flag = 0;
    if (p3 == 0) {
        flag = 1;
        goto check;
    }
    if (p3 != 1)
        goto check;
    h = *(int16_t *)(p1 + ((*(int8_t *)(p1 + 0x575) - 1) << 3) + 0x580);
    F0x8001ebf8(buf);
    ha = p1 + h * 2;
    ((cardgame_8ce0c_fn_t)*(uint32_t *)(buf + 0x2c))(*(int16_t *)(ha + 0x50) + 1);
    if (**(uint8_t **)buf != 6)
        goto check;
    flag = 1;
check:
    if (flag == 0)
        goto done;
    k = *(int8_t *)(p1 + 0x575) + 11;
    u = (uint8_t *)(p2 + k * 76);
    u[0x150] |= 1;
    *(uint8_t *)(p1 + k + 0x49e) = 0;
    *(uint32_t *)(p1 + 0x438) = 1;
done:
    *(uint8_t *)(p1 + 0x499) = 1;
}



