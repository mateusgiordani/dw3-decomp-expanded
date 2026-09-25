/* STCRDDEK:0x800888a8 -- 8 B word setter (file off 0x5bf8 = vaddr - 0x80082cb0).
 * PAL bytes (authority): 0x03E00008 jr ra; 0xAC850074 sw a1,0x74(a0) [store in jr delay slot].
 * Ghidra STCRDDEK read-only: STCRDDEK_func_800888a8 size 8, contiguous
 * (prev 0x80088894+20 ends at entry, next 0x800888b0); decompiler agrees:
 * *(param_1 + 0x74) = param_2. No intra-overlay jal/abs-ptr callers (cross-overlay
 * or table-driven callers); xrefs none. Portable C only, no register variables.
 * Struct type intentionally left unrecovered: unknown layout beyond word at +0x74. */
#include <stdint.h>

void STCRDDEK_func_800888a8(uint32_t *arg0, uint32_t arg1) {
    arg0[0x74 / 4] = arg1;
}
