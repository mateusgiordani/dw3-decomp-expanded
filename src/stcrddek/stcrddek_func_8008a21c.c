/* STCRDDEK:0x8008a21c (size 188, 0xBC) */
/* PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x756c */
/* Boundary: prologue addiu sp,-0x20 at 0x8008a21c, epilogue jr ra / addiu sp,+0x20 */
/* at 0x8008a2d0; next function prologue at 0x8008a2d8. 47/47 words match PAL. */
/* Ghidra ddw3-pal-sles-03936 program STCRDDEK: disasm 48 insns + decompile + */
/* x-ref to (no xrefs: reached only via overlay dispatch/table). No mutation. */
/* Body: probe EXE helper through table word EXE 0x80044F3C (kept as base */
/* 0x80044B38 + field 0x404 in s1, reloaded before every indirect jalr v0) */
/* with base value from EXE global 0x8005CCA8 (hi 0x8006 kept in s0, value */
/* reloaded before every call). Returns 1 on first nonzero probe; else result */
/* of probe(0x771) != 0. Delay-slot li v0,1 after each bne is dead on the */
/* fall-through path (overwritten by next call result). */
/* Toolchain hypothesis: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0. */

typedef struct {
    unsigned char pad[0x404];
    int (*fn)(int);
} stcrddek_tbl_80044B38_t;

/* EXE-resident objects (resolved via pipeline --symbol, not committed): */
extern stcrddek_tbl_80044B38_t stcrddek_tbl_80044B38;
extern int stcrddek_word_8005CCA8;

int STCRDDEK_func_8008a21c(void)
{
    if (stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x16) != 0)
        return 1;
    if (stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x1D) != 0)
        return 1;
    if (stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x32) != 0)
        return 1;
    if (stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x86) != 0)
        return 1;
    return stcrddek_tbl_80044B38.fn(0x771) != 0;
}
