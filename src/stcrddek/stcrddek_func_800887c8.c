// STCRDDEK:0x800887c8 (size 12, 0xc)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5b18
// Boundary: reviewed inventory 12 B (off 23320-23332); prev STCRDDEK:0x800886c8 (256 B) ends at 0x800887c8; next STCRDDEK:0x800887d4 (addiu sp,sp,-0x20).
// Ghidra STCRDDEK (ddw3-pal-sles-03936, read-only): li v0,0xa; jr ra; _sw v0,0x10(a0) (store in jr delay slot).
// Decompile: *(undefined4 *)(param_1 + 0x10) = 10; return;
// Xref: no direct callers in STCRDDEK (empty x-ref to 0x800887c8; table/EXE callers not excluded).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Status: C_MATCHING candidate (portable C, no asm) -- exact_byte_match pending pipeline.
#include <stdint.h>

void STCRDDEK_func_800887c8(int32_t p1)
{
    *(uint32_t *)(p1 + 0x10) = 10;
}
