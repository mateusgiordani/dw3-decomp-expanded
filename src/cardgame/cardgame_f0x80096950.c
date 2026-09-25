// CARDGAME:0x80096950 (size 324, 0x144)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x13CA0
// Framed function: prologue 27bdff40 addiu sp,-0xc0; epilogue jr ra / 27bd00c0 addiu sp,+0xc0.
// Next CARDGAME:0x80096a94 at +0x144; boundary from symbols/function_labels.csv (size 324,
// reports/handoffs/cardgame-boundary-sweep.md). Prev ends at 0x80096950 (gap 0).
// Ghidra program CARDGAME read-only (project ddw3-pal-sles-03936, base 0x80082cb0):
// disasm 81 words matches PAL word-for-word; decompile CARDGAME_F0x80096950(param_1);
// x-ref to: 1 caller CARDGAME_F0x8009bba8 at 0x8009be18 (UNCONDITIONAL_CALL, jal);
// x-refs from: direct jal EXE 0x8001f648, 4 computed jalr (stack slots sp+0x8c/0x84/0x98/0x94
// = buf+0x7c/+0x74/+0x88/+0x84) + EXE data vector *0x8004df9c, DATA vector *0x80044f5c.
// Body order: F0x8001f648(buf at sp+0x10, 0xa0-byte callback struct, see
// decomp/src/exe/exe_f0x8001f648.c); (buf+0x7c)(0x100,2); (buf+0x74)(0x340,0);
// mode = byte at p1+0xE9E selects via switch: 0 -> sel stays 0; 1 -> stepped path
// (if byte at p1+0xE9D >= 4: that byte -= 4, byte at p1+0xE9C += 1 with 0xB clamp
// that also forces the mode byte to 2; sel = byte at p1+0xE9C;
// byte at p1+0xE9D += DAT_8004df9c()); 2 -> sel = 0xB; default -> sel stays 0.
// (buf+0x88)(sel); idx = (*(p1+0x58) >> 1) & 0x3F (srl, logical);
// svc = DAT_80044f5c(0x25D0003); (buf+0x84)(svc,0,idx,idx). Caller passes only a0.
// Dispatch shape note: the three-case switch (0/1/2 + default) is what makes
// gcc-2.8.1 -O2 emit PAL's exact dispatch head: beq-to-A with the signed slti
// range check in its delay slot, bne-to-merge, then beq-forward to the out-of-line
// 'li s1,0xB' select with j-to-merge. Two-case switch/if-else-if spellings collapse
// to a fused bne select and lose the range check (316 B). Upstream cardgame.s GUIDE
// only, never copied. No Ghidra mutation.
// No domain pack: the EXE helper slots are generic service callbacks and neither DATA
// vector has a proven domain role; the target touches no battle/camera, skill, record,
// dialogue, map/tile, sprite, disc-I/O or loader contract beyond overlay mechanics,
// so a speculative pack would record guesses, not evidence.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base hypothesis,
// same framed-callback family as CARDGAME_F0x80096c20/0x8009848c/0x80097958).
// Status: exact_byte_match (worker-measured via fn_exact_pipeline.py, full-range
// candidate_sha256 == reference_sha256; coordinator independently recompiles/promotes).
#include <stdint.h>

extern void F0x8001f648(void *buf);
extern int32_t (*DAT_8004df9c)(void);
extern uint32_t (*DAT_80044f5c)(uint32_t);

void CARDGAME_F0x80096950(int32_t p1)
{
    uint8_t buf[0xA0];
    int mode;
    int32_t elapsed;
    int32_t sel;
    uint32_t idx;
    uint32_t svc;

    F0x8001f648(buf);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x7C))(0x100, 2);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x74))(0x340, 0);
    mode = *(uint8_t *)(p1 + 0xE9E);
    sel = 0;
    switch (mode) {
    case 0:
        break;
    case 1:
        if (*(uint8_t *)(p1 + 0xE9D) >= 4) {
            uint8_t b;

            *(uint8_t *)(p1 + 0xE9D) -= 4;
            b = (uint8_t)(*(uint8_t *)(p1 + 0xE9C) + 1);
            *(uint8_t *)(p1 + 0xE9C) = b;
            if (b >= 0xB) {
                *(uint8_t *)(p1 + 0xE9C) = 0xB;
                *(uint8_t *)(p1 + 0xE9E) = 2;
            }
        }
        sel = *(uint8_t *)(p1 + 0xE9C);
        elapsed = DAT_8004df9c();
        *(uint8_t *)(p1 + 0xE9D) += (uint8_t)elapsed;
        break;
    case 2:
        sel = 0xB;
        break;
    default:
        break;
    }
    ((void (*)(int32_t))*(void **)(buf + 0x88))(sel);
    idx = (*(uint32_t *)(p1 + 0x58) >> 1) & 0x3F;
    svc = DAT_80044f5c(0x25D0003u);
    ((void (*)(uint32_t, int32_t, uint32_t, uint32_t))*(void **)(buf + 0x84))(svc, 0, idx, idx);
}
