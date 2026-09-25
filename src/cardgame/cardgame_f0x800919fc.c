// CARDGAME:0x800919fc (size 624, 0x270)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xed4c
// Prologue 27bdffc0 addiu sp,-0x40 ; frame -0x40/+0x40
// Body: for-loop search/remove i16 entry (found-path callback takes the matched
// byte as 3rd arg, pre-colored to $a2) + 2 direct calls + 5 indirect calls.
// Epilogue lw ra/s8/s7/s6/s5/s4/s3/s2/s1/s0 ; jr ra ; addiu sp,+0x40
// Next function CARDGAME:0x80091c6c at +0x270 (boundary sweep #80, HIGH).
// 1 caller: CARDGAME:0x80084804 (in CARDGAME_F0x80084320; a0=ptr, a1=ptr, a2=idx).
// 2 direct callees: CARDGAME:0x800a0844 (5x loop), CARDGAME:0x8009170c.
// 5 indirect: lw p1+0x81c (p1,idx,byte) ; lw p2+0xf14 (p2,u7,0xe500,0x6100) ;
//   lw p2+0xf3c (p2,u7,*(short*)(pb+u6*14+2)) ; lw p2+0xea0 (p2,idx,6,cnt) ;
//   lw p2+0xf24 (p2,u7,0x14,0x1000,0x1000 via sp+0x10).
// Strength-reduction fingerprints (plain C multiplies): idx*114, idx*200,
// u6*14, u7*76. Loop1 pointer-bump v+=2 with fixed +0x64 load offset.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only, no state
// changed: disasm 156 insns in 40/40/40/36 chunks, word-equal vs PAL bin @
// 0xed4c (first 27bdffc0, last 03e00008+27bd0040); decompile; x-ref to (1
// caller above); x-ref from (jal 0x800a0844 + jal 0x8009170c).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).

extern void CARDGAME_F0x800a0844(void *p, void *e, int idx, int k);
extern void CARDGAME_F0x8009170c(void *p1, void *p2, int idx, unsigned int u);

typedef void (*cardgame_cb1_t)(void *, int, unsigned int);
typedef void (*cardgame_f14_t)(void *, unsigned int, unsigned int, unsigned int);
typedef void (*cardgame_f3c_t)(void *, unsigned int, int);
typedef void (*cardgame_fea0_t)(void *, int, int, int);
typedef void (*cardgame_ff24_t)(void *, unsigned int, int, unsigned int,
    unsigned int);

typedef struct {
    unsigned char pad0[0xa];
    short cnt;
    unsigned char pad1[0x64 - 0xa - 2];
    short arr[1];
} cardgame_q_t;

void CARDGAME_F0x800919fc(void *p1, void *p2, int idx) {
    unsigned char *pb = (unsigned char *)p1 + (idx * 114 + 0x72c);
    cardgame_q_t *q = (cardgame_q_t *)((char *)p1 + (idx * 200 + 0x59c));
    unsigned int u6 = *pb;
    unsigned int u7 = u6;
    int i;

    if (idx != 0)
        u7 = u6 + 6;
    for (i = 0; i < q->cnt; i++) {
        if (*(unsigned char *)((char *)p1 +
                ((((signed char *)p1)[0x575] - 1) << 3) + 0x586) ==
            q->arr[i]) {
            ((cardgame_cb1_t)*(void **)((char *)p1 + 0x81c))(p1, idx,
                *(unsigned char *)((char *)p1 +
                    ((((signed char *)p1)[0x575] - 1) << 3) + 0x586));
            break;
        }
    }
    {
        int j;
        for (j = i; j < q->cnt - 1; j++)
            q->arr[j] = q->arr[j + 1];
    }
    q->cnt--;
    (*pb)--;
    ((cardgame_f14_t)*(void **)((char *)p2 + 0xf14))(p2, u7, 0xe500, 0x6100);
    {
        int off = u6 * 14;
        ((cardgame_f3c_t)*(void **)((char *)p2 + 0xf3c))(p2, u7,
            *(short *)(pb + off + 2));
        for (i = 0; i < 5; i++)
            CARDGAME_F0x800a0844(p1, pb + (off + 2), idx, i);
    }
    {
        char *r = (char *)p2 + u7 * 76;
        unsigned char b;
        int x = u6 * 14;
        *(unsigned char *)(r + 0x14b) = *(unsigned char *)(pb + x + 8);
        b = *(unsigned char *)(pb + x + 10);
        *(unsigned short *)(r + 0x120) = 0;
        *(unsigned char *)(r + 0x14c) = b;
    }
    CARDGAME_F0x8009170c(p1, p2, idx, u7);
    *(unsigned char *)((char *)p1 + 0x422) = 1;
    ((cardgame_fea0_t)*(void **)((char *)p2 + 0xea0))(p2, idx, 6, q->cnt);
    ((cardgame_ff24_t)*(void **)((char *)p2 + 0xf24))(p2, u7, 0x14, 0x1000,
        0x1000);
}
