// CARDGAME:0x8009cda4 (size 88, 0x58)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1a0f4
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #171
// PAL words (MIPS LE, 22 words): 27BDFFE0 AFB10014 00808821 26240090 AFB00010
//   3C108005 2610B5DC AFBF0018 8E020024 00000000 0040F809 2405000A 262400E4
//   8E020024 00000000 0040F809 2405000A 8FBF0018 8FB10014 8FB00010 03E00008 27BD0020
// Frame -0x20/+0x20; saves s1/s0/ra; s1=a0; s0=0x8004B5DC (EXE dispatch base);
// two indirect jalr via lw v0,36(s0) (slot 0x8004B600), a1=10, a0=s1+0x90 then s1+0xE4.
// Ghidra CARDGAME (seeded): CARDGAME_F0x8009cda4 size 88 confirmed; 0 direct jal
// callers (jal 0x0C027369 absent in overlay); prev 0x8009ca3c / next 0x8009ce28.
// Prior provenance: noreorder asm wrapper exact_byte_match (rev 1, ASM evidence);
// this revision recovers portable C (loop rule: inline asm does not count as C).
// Toolchain hypothesis: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).

typedef void (*cardgame_dispatch_fn)(void *a0, int a1);

extern unsigned int D_8004B5DC[];

void CARDGAME_F0x8009cda4(void *arg) {
    ((cardgame_dispatch_fn)D_8004B5DC[9])((char *)arg + 0x90, 10);
    ((cardgame_dispatch_fn)D_8004B5DC[9])((char *)arg + 0xE4, 10);
}
