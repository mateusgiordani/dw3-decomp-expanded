/* STCRDDEK:0x80086574 -- state-dispatch tick (152 B, file off 0x38c4 = vaddr - 0x80082cb0).
 * PAL bytes (authority): prologue addiu sp,-0x20, save s0/s1/ra; v1 = *(a0 + 0xc);
 * beq v1,1 -> 0x800865e4 path; slti/bne pair skips states 2..3; else path does
 * jalr *(a0 + 0x38), jal 0x8008397c, then stores 10 at +0x444/+0x454/+0x464/
 * +0x474/+0x484 (last store in j delay slot); state==1 path is jal 0x80085210
 * then jal 0x80084890. Epilogue restores ra/s1/s0, jr ra, addiu sp,0x20.
 * Ghidra STCRDDEK read-only: disasm matches PAL words exactly (38 words);
 * decompiler agrees on the if (state == 1) / else-if (state < 2 || state > 3)
 * shape; x-ref to 0x80086574: none (callers outside STCRDDEK or table-driven).
 * Struct layout intentionally left unrecovered beyond word offsets used here.
 * Status C_MATCHING (2026-09-08 retry wave): goto-funnel shape keeps the
 * unfolded slti/bne pair (slti 2 / bne->setup with slti 4 in the delay slot,
 * bne->done with nop in the delay slot), the explicit load-delay nop before
 * jalr, and the reloaded move a0,s0 after jalr; exact pipeline under base
 * (psyq-gcc-2.8.1-sn32-4.0.0010/aspsx-2.79 -O2 -G0) reports exact_byte_match,
 * 152/152 bytes, difference_count 0. Supersedes the nested-if C_NONMATCHING
 * note: the `||` fold (addu+sltu, 148 bytes) was a source-shape artifact,
 * not a toolchain variant gap.
 */
#include <stdint.h>

extern void STCRDDEK_func_8008397c(uint32_t *arg0, uint32_t arg1);
extern void STCRDDEK_func_80084890(uint32_t *arg0);
extern void STCRDDEK_func_80085210(uint32_t *arg0, uint32_t arg1);

void STCRDDEK_func_80086574(uint32_t *arg0, uint32_t arg1) {
    int32_t state = (int32_t)arg0[0xc / 4];

    if (state == 1)
        goto state1;
    if (state < 2)
        goto setup;
    if (state < 4)
        goto done;
setup:
    ((void (*)(uint32_t *))arg0[0x38 / 4])(arg0);
    STCRDDEK_func_8008397c(arg0, arg1);
    arg0[0x444 / 4] = 10;
    arg0[0x454 / 4] = 10;
    arg0[0x464 / 4] = 10;
    arg0[0x474 / 4] = 10;
    arg0[0x484 / 4] = 10;
    goto done;
state1:
    STCRDDEK_func_80085210(arg0, arg1);
    STCRDDEK_func_80084890(arg0);
done:;
}
