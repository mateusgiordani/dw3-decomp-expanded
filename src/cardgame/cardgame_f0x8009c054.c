#include "common/types.h"

/* CARDGAME:0x8009c054 (size 136, 0x88) - C_NONMATCHING (portable C, rev2)
 * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x193a4
 * Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #156.
 * Provenance: rev1 (w180) established boundary + exact_byte_match via noreorder
 * __asm__ wrapper (candidate SHA e7c8b944aa33cd4074f198a8a739b53cb0cfcacec2c2f872ef9c843e9c759c0f).
 * Per cardgame-human-v1 loop rule (reports/handoffs/dsh-muse-cardgame-loop.md step 5),
 * inline ASM/words do not count as C; this rev2 recovers genuine portable C.
 * Ghidra program CARDGAME (project ddw3-pal-sles-03936), read-only queries, no mutation:
 *   disasm 0x8009c054 (34 instrs) matches PAL words byte-for-byte;
 *   decompile 0x8009c054 -> CARDGAME_F0x8009c054(int, u4, u1, u2, u4):
 *     (*_DAT_80055c48)(0x40019); sh 0xc @ +0xdf2/+0xdf0; sh param_4 @ +0xdf4;
 *     sw param_2 @ +0xde8; sb param_3 @ +0xdfb; sw param_5 @ +0xde4; sb 1 @ +0xdfa.
 *   x-ref to 0x8009c054: DATA ref from 0x8009d390 in CARDGAME_F0x8009d310
 *     (function-pointer table entry; graph callers/callees empty - indirect only).
 * Next function CARDGAME:0x8009c0dc at +0x88 confirms size 0x88.
 * Signature is conservative: scalar widths taken from store widths (sw/sh/sb),
 * not from guessed struct; offsets left raw (no invented struct).
 * Indirect callee at *0x80055c48 is an EXE global function pointer shared with
 * siblings 0x8009c0dc/0x8009c124/0x8009c174/0x8009c1cc/0x8009c214.
 */

extern void (*D_80055c48)(int32_t);

void CARDGAME_F0x8009c054(uint8_t *base, int32_t a1, int32_t a2, int32_t a3, int32_t a4) {
    D_80055c48(0x40019);
    *(int16_t *)(base + 0xDF2) = 12;
    *(int16_t *)(base + 0xDF0) = 12;
    *(int16_t *)(base + 0xDF4) = (int16_t)a3;
    *(int32_t *)(base + 0xDE8) = a1;
    *(base + 0xDFB) = (uint8_t)a2;
    *(int32_t *)(base + 0xDE4) = a4;
    *(base + 0xDFA) = 1;
}
