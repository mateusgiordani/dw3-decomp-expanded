// STCRDDEK:0x8008a2d8 (size 148, 0x94)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x7628
// Prev STCRDDEK:0x8008a21c (188B) ends exactly at 0x8008a2d8; next STCRDDEK:0x8008a36c at +0x94.
// Prologue 27bdffe8 addiu sp,sp,-0x18 ; afb00010 sw s0,0x10(sp) ; 00808021 move s0,a0 ;
//   24020001 li v0,1 ; afbf0014 sw ra,0x14(sp) ;
//   10a0000d beq a1,zero,0x8008a324 with delay ae02000c sw v0,0xc(s0) (param_1[3] = 1 always).
// Then (a1 != 0): 3c040004 lui a0,4 ; 3c028005 lui v0,0x8005 ; 8c425c48 lw v0,0x5c48(v0) ;
//   00000000 nop ; 0040f809 jalr v0 ; 34840019 ori a0,a0,0x19 delay (a0 = 0x40019) ;
//   8e030000 lw v1,0(s0) ; 24021000 li v0,0x1000 ; 0043001a div v0,v1 ; 00001012 mflo v0 ;
//   080228d6 j 0x8008a358 with delay ae000008 sw zero,8(s0) ; sw v0,4(s0) at join.
// Else (a1 == 0): same vector call with 3484001a ori delay (a0 = 0x4001a) ;
//   8e020000 lw v0,0(s0) ; 24031000 li v1,0x1000 ; 0062001a div v1,v0 ; 00001012 mflo v0 ;
//   ae030008 sw v1,8(s0) ; 00021040 sll v0,v0,1 ; 00021023 subu v0,zero,v0 ; sw v0,4(s0) at join.
// Epilogue 8fbf0014 lw ra,0x14(sp) ; 8fb00010 lw s0,0x10(sp) ; 03e00008 jr ra ; 27bd0018 addiu sp,sp,0x18.
// Ghidra ddw3-pal-sles-03936/STCRDDEK disasm matches PAL words byte-for-word (37 insns);
//   decompile: param_1[3] = 1; if (param_2 == 0) { vec(0x4001a); param_1[2] = 0x1000;
//   iVar1 = (0x1000 / *param_1) * -2; } else { vec(0x40019); iVar1 = 0x1000 / *param_1;
//   param_1[2] = 0; } param_1[1] = iVar1;
// Xrefs: x-ref to 0x8008a2d8 empty; graph callers/callees empty in STCRDDEK program
//   (possible table-dispatched callback; caller unresolved, preserved as uncertainty).
// Callee *0x80055c48 is the EXE-resident shared vector (same lui v0,0x8005 / lw v0,0x5c48(v0)
//   idiom as the CARDGAME 0x8009c0dc family).
// Toolchain hypothesis: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base first).

void STCRDDEK_func_8008a2d8(int *param_1, int param_2)
{
    param_1[3] = 1;
    if (param_2 != 0) {
        int v;
        (*(void (**)(int))0x80055c48)(0x40019);
        v = 0x1000 / param_1[0];
        param_1[2] = 0;
        param_1[1] = v;
    } else {
        int v;
        int origin;
        (*(void (**)(int))0x80055c48)(0x4001a);
        origin = 0x1000;
        v = (origin / param_1[0]) * -2;
        param_1[2] = origin;
        param_1[1] = v;
    }
}
