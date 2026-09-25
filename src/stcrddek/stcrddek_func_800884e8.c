// STCRDDEK:0x800884e8 (size 320, 0x140)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5838
// Boundary: symbols/function_labels.csv STCRDDEK,0x800884e8,function,
// STCRDDEK_func_800884e8,320. Prologue addiu sp,-0x58 ; sw s0,0x48(sp) ;
// move s0,a0 ; sw s2,0x50(sp) ; sw ra,0x54(sp) ; sw s1,0x4c(sp).
// Epilogue lw ra,0x54(sp) ; lw s2,0x50(sp) ; lw s1,0x4c(sp) ;
// lw s0,0x48(sp) ; jr ra ; addiu sp,+0x58. 80 words, next function is
// beyond +0x140 (no overlap; body ends with jr/addu pair).
// Dispatch on *(int32_t *)(arg0 + 0xc): state 1 runs the short path
// (STCRDDEK_func_80087788(arg0,arg1) then STCRDDEK_func_80086e04(arg0));
// states 2-3 do nothing (fall to epilogue); any other state runs the long
// path: indirect *(arg0+0x38)(arg0), EXE filler F0x8001ffa8(buf at sp+0x10,
// 56 bytes, fn slots at buf+0x24 -> sp+0x34 and buf+0x2c -> sp+0x3c),
// slot(buf+0x24)(*(arg0+0x5c),*(arg0+0x60)), r = D_80044f5c(0x7710000),
// slot(buf+0x2c)(r), overlay-data triple at D_8008B848/4C/50 selected by
// D_8005CCA8 (zero: 3/0x8008B368/0x8008B38C, else: 1/0x8008B604/0x8008B610),
// *(arg0+0xbc)=*(arg0+0xdc)=*(arg0+0xcc)=10, then
// STCRDDEK_func_800868e8(arg0,arg1). Source uses forward gotos so the
// generator keeps the PAL block order (dispatch inline, long block,
// short block, shared epilogue) including the slti <2 / slti <4 tests
// (PAL 28620002/14400003/28620004/1440003c, second slti in the first
// bne delay slot). Stack-table shape matches sibling C_MATCHING
// STCRDDEK_func_8008a100 (char buf[56], slots buf+0x24/0x2c).
// Ghidra program STCRDDEK read-only (never import): disasm 80 insns
// matches PAL word-for-word; decompile STCRDDEK_func_800884e8 shows this
// dispatch shape (hypothesis only, confirmed against disasm, not copied).
// Xrefs to 0x800884e8: none (reached via overlay dispatch/table, same as
// sibling inventory bodies). Callees: direct EXE 0x8001ffa8 + 3 direct
// STCRDDEK jal (0x800868e8/0x80087788/0x80086e04) + 3 computed jalr
// (stack slots sp+0x34/sp+0x3c, EXE vector *0x80044f5c). Upstream
// stcrddek.s .L0x00005838 GUIDE only. Toolchain base hypothesis:
// psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0.
// v2: triple dests as one array extern (D_8008B848[3]) so the generator shares
// one base (PAL addiu v1,v1,-0x47b8 + sw 4/8(v1)); pointer values as extern
// addresses (&D_8008B368/...) so they assemble as lui 0x8009 + addiu (PAL
// 0x8009/-0x4c98 form) with relocations instead of folded li+ori.
// Status: C_MATCHING (portable C, no asm; fn_exact_pipeline exact_byte_match,
// base O2, difference_count 0, sha 97efd2d2...89437; no alternates needed).
#include <stdint.h>

extern void F0x8001ffa8(void *buf);
extern int32_t (*D_80044f5c)(int32_t a0);
extern int32_t D_8005CCA8;
extern int32_t D_8008B848[3];
extern int32_t D_8008B368;
extern int32_t D_8008B38C;
extern int32_t D_8008B604;
extern int32_t D_8008B610;
extern void STCRDDEK_func_800868e8(int32_t a0, int32_t a1);
extern void STCRDDEK_func_80087788(int32_t a0, int32_t a1);
extern void STCRDDEK_func_80086e04(int32_t a0);

void STCRDDEK_func_800884e8(int32_t arg0, int32_t arg1)
{
    uint8_t buf[56];
    int32_t state;
    int32_t r;

    state = *(int32_t *)(arg0 + 0xc);
    if (state == 1) {
        goto st1;
    }
    if (state < 2) {
        goto other;
    }
    if (state < 4) {
        goto out;
    }
other:
    ((void (*)(int32_t))*(void **)(arg0 + 0x38))(arg0);
    F0x8001ffa8(buf);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x24))(*(int32_t *)(arg0 + 0x5c), *(int32_t *)(arg0 + 0x60));
    r = D_80044f5c(0x7710000);
    ((void (*)(int32_t))*(void **)(buf + 0x2c))(r);
    if (D_8005CCA8 == 0) {
        D_8008B848[0] = 3;
        D_8008B848[1] = (int32_t)&D_8008B368;
        D_8008B848[2] = (int32_t)&D_8008B38C;
    } else {
        D_8008B848[0] = 1;
        D_8008B848[1] = (int32_t)&D_8008B604;
        D_8008B848[2] = (int32_t)&D_8008B610;
    }
    *(int32_t *)(arg0 + 0xbc) = 10;
    *(int32_t *)(arg0 + 0xdc) = 10;
    *(int32_t *)(arg0 + 0xcc) = 10;
    STCRDDEK_func_800868e8(arg0, arg1);
    goto out;
st1:
    STCRDDEK_func_80087788(arg0, arg1);
    STCRDDEK_func_80086e04(arg0);
out:
    return;
}