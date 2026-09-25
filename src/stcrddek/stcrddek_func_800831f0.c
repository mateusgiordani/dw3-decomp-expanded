// STCRDDEK:0x800831f0 (size 248, 0xf8)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x540
// Boundary: reports/investigations/pro-310-triage/modules/stcrddek-inventory.json
//   body-800831f0: address 2148020720, instruction_bytes 248, range [2148020720, 2148020968);
//   next body-800832e8 starts exactly at 2148020968 (addiu sp,-0x18); prev 16 bytes
//   (800893a8 800893a8 80089e2c 80089e5c) are not code of this function.
// Ghidra STCRDDEK (ddw3-pal-sles-03936, read-only): 62-insn disasm matches PAL
//   words exactly; decompile STCRDDEK_func_800831f0 shows gate on *(arg0+0xc)
//   (body unless 1 <= state <= 3), then EXE table calls, stack halfword table,
//   method at +0x12c of the returned object, *arg1 = STCRDDEK_func_8008a0b8(),
//   method at +0x38 of arg0 (hypothesis only, confirmed against disasm).
// Xref: Ghidra x-ref to 0x800831f0 empty (no direct jal); inventory call_seeds
//   proposes caller STCRDDEK 0x800832e8 via task_callback_argument at site
//   0x800832fc (F0x80014504(STCRDDEK_func_800831f0, 0x50, 4)); decompile of
//   0x800832e8 confirms this registration (evidence for callback role).
//   Upstream stcrddek.s holds no 0x800831f0 label (guide only, never authority).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base hypothesis).
// Attempt history on pinned base: (1) single if with || got merged by GCC into
//   (state-1)<u 3 (60-word object, 8 B short); (2) this three-if/goto chain keeps
//   beq/bltz/slti+bne separate and matches. No compiler alternates used.
// Status: C_MATCHING (portable C, no asm; pipeline exact_byte_match, difference_count 0).
#include <stdint.h>

extern int32_t D_8004DE10[];
extern int32_t STCRDDEK_func_8008a0b8(void);

void STCRDDEK_func_800831f0(int32_t arg0, int32_t *arg1)
{
    int32_t *tbl;
    int16_t h[4];
    int32_t obj;
    int32_t state;

    state = *(int32_t *)(arg0 + 0xc);
    if (state == 0)
        goto body;
    if (state < 0)
        goto body;
    if (state < 4)
        goto out;
body:
        tbl = D_8004DE10;
        ((void (*)(void))tbl[84])();
        ((void (*)(int32_t))tbl[85])(0xf000);
        ((void (*)(int32_t, int32_t, int32_t, int32_t))tbl[93])(0x140, 0xf0, 0, 0);
        h[0] = 0;
        h[1] = 0;
        h[2] = 0x140;
        h[3] = 0xf0;
        obj = ((int32_t (*)(int16_t *, int32_t, int32_t))tbl[91])(h, 3, 0x1000);
        ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(obj + 0x12c))(obj, 0, 0, 0);
        *arg1 = STCRDDEK_func_8008a0b8();
        ((void (*)(int32_t))*(void **)(arg0 + 0x38))(arg0);
out:
    return;
}
