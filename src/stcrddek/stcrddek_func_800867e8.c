/* STCRDDEK:0x800867e8 (size 148, 0x94; file off 0x3b38 = vaddr - 0x80082cb0).
 * PAL bytes (authority): reference/extracted/pro/stcrddek.bin base 0x80082cb0,
 * 37 words 27bdffe8...27bd0018 (sha256 e2080944b1b57ff4a012d250a2323ebfc21a600043fedcdb2a159eebfc20114b).
 * Ghidra STCRDDEK (project ddw3-pal-sles-03936) read-only, no mutation:
 * disasm 37 words matches PAL word-for-word; decompile:
 * param_1[3] = 1; if (param_2 == 0) { (*0x80055c48)(0x4001a); p[2] = 0x1000;
 * i = (0x1000 / *p) * -2; } else { (*0x80055c48)(0x40019); i = 0x1000 / *p; p[2] = 0; }
 * p[1] = i. Boundary: framed (addiu sp,-0x18; sw s0/ra; jr ra + addiu sp,+0x18),
 * contiguous (next STCRDDEK:0x8008687c, symbols/functions.csv + labels reviewed).
 * Callers: 4 direct jal (0x800877e4 in STCRDDEK_func_80087788; 0x80088398/0x800883d0/
 * 0x8008848c) + upstream stcrddek.s 4x jal 0x800867e8; xrefs agree. Callees: 2 indirect
 * jalr via EXE global function pointer *0x80055c48 (lui 0x8005 + lw 0x5c48; args
 * 0x40019/0x4001a via lui 0x4 + ori in delay slot); same idiom as CARDGAME/STFGTREP
 * family (cf. STFGTREP 0x80085c90, same 148B shape with 0x40000019/1a constants).
 * Upstream stcrddek.s is GUIDE only. No domain pack: pure integer arithmetic on
 * caller struct + notification call; no battle/camera/skills/records/dialogue/tiles/
 * sprites/disc-IO semantics evident. Portable C only, no asm, no register vars.
 * Toolchain base: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 -> 220B vs 148B.
 * Alternates (budget exhausted): (1) cc1 2.7.2-cygnus-970404 + aspsx-2.79 -O2 -> 224B;
 * (2) cc1 2.8.1 + aspsx-2.81 -O2 -> 220B. Assembler probe: every pinned aspsx
 * (>=2.67; 2.56 predates %hi/%lo syntax) expands 3-operand div into an 11-word
 * trap-check sequence; every pinned cc1 (2.7.2/2.8.1/egcs/2.95.2) emits 3-operand
 * div for this C. Excising the two 9-word check bodies leaves 37 words identical
 * to PAL (incl. beq offset 0x0d, lui/ori consts, v0/v1 asymmetry, sll+subu
 * negation, epilogue). Sibling STFGTREP 0x80085c90 (same shape) re-run now also
 * yields 220 > 148, so its old exact_byte_match no longer reproduces.
 * Status: C_NONMATCHING (portable C, compiles, blocker below). */
#include <stdint.h>

extern void (*D_80055c48)(int32_t);

void STCRDDEK_func_800867e8(int32_t *a0, int32_t a1) {
    a0[3] = 1;
    if (a1 != 0) {
        D_80055c48(0x40019);
        a0[2] = 0;
        a0[1] = 0x1000 / a0[0];
    } else {
        D_80055c48(0x4001a);
        a0[2] = 0x1000;
        a0[1] = -((0x1000 / a0[0]) * 2);
    }
}
