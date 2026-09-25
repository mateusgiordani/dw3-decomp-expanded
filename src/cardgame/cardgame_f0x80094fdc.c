// CARDGAME:0x80094fdc (size 84, 0x54)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1232c
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #97
// Disasm (Ghidra CARDGAME, read-only): prologue 27bdffd8 addiu sp,-0x28 ; sw s1,0x1c(sp) ;
//   addu s1,a0 (ctx) ; addu a0,a1 (cbctx) ; sw s0,0x18(sp) ; addiu s0,zero,1 ;
//   addu a1,a2 (param) ; addu a2,zero,zero ; sw ra,0x20(sp) ; sw zero,0x424(s1) ;
//   sw s0,0x10(sp) (5th outgoing arg = 1) ; lw v0,0xee4(a0) ; nop (load delay) ;
//   jalr ra,v0 ; addu a3,a2,zero (delay, a3 = 0) ; sb s0,0x422(s1) ;
//   lw ra/s1/s0 ; jr ra ; addiu sp,+0x28.
// Decompile (Ghidra, hypothesis only): *(param_1 + 0x424) = 0;
//   (**(code **)(param_2 + 0xee4))(param_2, param_3, 0, 0, 1);
//   *(param_1 + 0x422) = 1.
// Xrefs (Ghidra x-ref to 0x80094fdc, read-only): 3 UNCONDITIONAL_CALL sites,
//   all inside CARDGAME_F0x80084320 (0x80084500, 0x80084518, 0x80084534).
// Words: 21/21 PAL words match reference (next word 27bdffd8 = CARDGAME:0x80095030
//   prologue, contiguous, size 0x54 confirmed self-contained).
// Caller/callee: 3 callers, 0 direct callees (indirect jalr via v0 = *(cbctx+0xee4)).
// Prior provenance: rev1 (w159) delivered noreorder asm wrapper with exact_byte_match
//   (candidate == reference sha256, 84 B); campaign rev2 requires reviewed portable C,
//   and asm-backed wrappers do not count as C, so this revision recovers portable C.
// Exact 84-byte native GCC 2.8.1 / ASPSX 2.79 result; replay recipe in
// docs/c-matching-guide/submissions/cardgame-80094fdc/manifest-r5.json.
// Keep the callback load in the call expression: a separate fn local moves
// it before the outgoing stack argument and changes the native delay slot.

typedef void (*cardgame_indirect_80094fdc_t)(void *arg0, int arg1, int arg2, int arg3, int arg4);

void CARDGAME_F0x80094fdc(void *ctx, void *cbctx, int param) {
    ((int *)ctx)[0x424 / 4] = 0;
    (*(cardgame_indirect_80094fdc_t *)((char *)cbctx + 0xEE4))(cbctx, param, 0, 0, 1);
    ((unsigned char *)ctx)[0x422] = 1;
}
