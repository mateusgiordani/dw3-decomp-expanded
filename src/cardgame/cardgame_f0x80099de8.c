/* CARDGAME:0x80099de8 (size 236, 0xec) -- portable C recovery, status C_MATCHING. */
/* Pipeline (fn_exact_pipeline): exact_byte_match, base variant -O2 -G0 with */
/* psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79; candidate SHA-256 equals PAL */
/* reference SHA-256 bb9d6c887432db17f15c052fe43eda0aaf8dee1ee644b9753844e46d50f5b742, */
/* difference_count 0 over 236 body bytes (no padding). Symbols: */
/* EXE_F0x8001f648=0x8001f648 EXE_D_80044f5c=0x80044f5c. */
/* PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x17138. */
/* Boundary (sweep #132, HIGH): prologue 27bdff48 addiu sp,-0xb8, epilogue */
/* 03e00008 jr ra + 27bd00b8 addiu sp,+0xb8; prev CARDGAME:0x80099d20 ends */
/* exactly at 0x80099de8 (0x80099d20+0xc8), next CARDGAME:0x80099ed4 starts */
/* exactly at 0x80099de8+0xec. Frame -0xb8/+0xb8 verified. */
/* Ghidra (project ddw3-pal-sles-03936, program CARDGAME, read-only, no import */
/* or mutation): disasm 59 words byte-exact vs PAL, halfword counter at */
/* param[6] (+0xc), scaled quotient q = (next<<16)>>18, slti+beq+j branch */
/* funnel with delay-slot sh/clear, then indirect calls. x-ref to: 1 caller */
/* (jal at 0x80099f24 in CARDGAME_F0x80099ed4). x-refs from: 1 direct jal to */
/* EXE 0x8001f648, 5 indirect jalr (stack slots sp+0x84/0x8c/0x94/0x98 plus */
/* EXE word at 0x80044f5c). */
/* Recovery notes: the 0xa0-byte stack block (blk at sp+0x10) reproduces the */
/* PAL 184-byte frame with callback slots at sp+0x84/0x8c/0x94/0x98. The */
/* select logic is spelled with gotos (set3/mid/set1/funnel) to reproduce */
/* PAL's block layout: q==1 -> sel 3; q==2 -> sel 1; else sel 0 with */
/* param[6] = 0 unless q == 0. Semantics identical to the if-chain form; the */
/* goto spelling exists only to match the original's branch lowering. */
/* Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0, base variant. */

#include <stdint.h>

extern void EXE_F0x8001f648(void *work);
extern void *EXE_D_80044f5c;

typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb1_t)(int32_t a0);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*exe_svc_t)(int32_t a0);

/* Stack block hypothesis: 0x74-byte work area filled by EXE_F0x8001f648, */
/* followed by callback slots. Exact inter-slot gaps (0x84/0x8c/0x94/0x98) */
/* are PAL-observed; this struct is a readable proxy, not a layout claim. */
typedef struct {
    uint8_t work[0x74];
    cardgame_cb2_t cb340;
    uint32_t slot_78;
    cardgame_cb2_t cb100;
    uint8_t pad[4];
    cardgame_cb4_t cb_last;
    cardgame_cb1_t cb_mid;
    uint8_t tail[0x14];
} cardgame_99de8_block_t;

void CARDGAME_F0x80099de8(int16_t *param) {
    cardgame_99de8_block_t blk;
    int32_t sel;
    int32_t q;
    int16_t prev;
    int16_t next;

    EXE_F0x8001f648(blk.work);
    blk.cb100(0x100, 1);
    blk.cb340(0x340, 0);

    prev = param[6];
    next = (int16_t)(prev + 1);
    param[6] = next;
    q = ((int32_t)(next << 16)) >> 18;

    if (q == 1)
        goto set3;
    if (q < 2) {
        if (q == 0)
            goto mid;
        param[6] = 0;
        goto mid;
    }
    if (q == 2)
        goto set1;
    param[6] = 0;
mid:
    sel = 0;
    goto funnel;
set3:
    sel = 3;
    goto funnel;
set1:
    sel = 1;
funnel:
    blk.cb_mid(sel);
    sel = ((exe_svc_t)EXE_D_80044f5c)(0x025D0003);
    blk.cb_last(sel, 8, (int32_t)param[0], (int32_t)param[1]);
}
