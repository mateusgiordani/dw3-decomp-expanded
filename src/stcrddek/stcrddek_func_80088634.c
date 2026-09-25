/*
 * STCRDDEK:0x80088634 STCRDDEK_func_80088634
 * 148 bytes at STCRDDEK.PRO offset 0x5984 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float
 *  Variant     base
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80088634
 *  Symbols     stcrddek_exe_8001FCC0=0x8001fcc0
 *              stcrddek_exe_8002525C=0x8002525c
 *  Compare     148 bytes from 0x80088634 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80088634
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * No Ghidra state change.
 *
 * Behavior (conservative, disasm-verified): EXE helper fills a 16-byte stack
 * buffer; word +8 of the buffer is an indirect target called as fn(arg0+0x78,
 * arg1, 0); EXE strlen-like helper on arg0+0x78 gives a byte length whose half
 * (sra 1) is the start index; halfwords at arg0+0x78 are filled with 0x4081 up
 * to the int count at arg0+0x94.
 *
 * The count at +0x94 is reloaded every loop iteration (lw inside the loop, not
 * hoisted).
 *
 * sha256 9103041d1ebf2020b0b6fbadf282884393a50d47e086f5f7ed9779a77e1dd0c4
 * (148B).
 *
 * Matching idioms (load-bearing, do not "clean up"): arg0 is cast to int at
 * each use (no base local) -- this orders the prologue s2-pair before the
 * s1-pair and emits addu v1,v0,s2 for the pointer; the do-while body stores
 * first (sh), then p += 2, then idx += 1 with the count reloaded in the
 * condition, which places addiu a0,a0,1 between lw and slt (no load-delay nop)
 * and p += 2 in the bne delay slot.
 *
 * EXE-resident helpers (resolved via pipeline --symbol, not committed):
 */

extern void stcrddek_exe_8001FCC0(void *buf);
extern int stcrddek_exe_8002525C(const char *s);

void STCRDDEK_func_80088634(void *arg0, int arg1)
{
    unsigned int buf[4];
    int (*fn)(void *, int, int);
    char *s0;
    int tmp;
    int count;
    int idx;

    stcrddek_exe_8001FCC0(buf);
    s0 = (char *)((int)arg0 + 0x78);
    fn = (int (*)(void *, int, int))buf[2];
    fn(s0, arg1, 0);
    tmp = stcrddek_exe_8002525C(s0);
    count = *(int *)((int)arg0 + 0x94);
    idx = tmp >> 1;
    if (idx < count) {
        short fill = 0x4081;
        char *p = (char *)((idx << 1) + (int)arg0);
        do {
            *(short *)(p + 0x78) = fill;
            p += 2;
            idx += 1;
        } while (idx < *(int *)((int)arg0 + 0x94));
    }
}
