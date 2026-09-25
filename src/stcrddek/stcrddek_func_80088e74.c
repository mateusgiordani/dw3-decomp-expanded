// STCRDDEK:0x80088e74 (size 984, 0x3d8)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x61c4
// Prologue 27bdff30 addiu sp,-0xd0, saves s0-s5/ra, move s4,a0, addiu a0,sp,0x10, jal 0x8001f648
// Epilogue 03e00008 jr ra + 27bd00d0 addiu sp,+0xd0 at 0x80089244/0x80089248
// Next STCRDDEK:0x8008924c (addiu sp,-0x90) starts exactly at +984; prev ends at 0x80088e74
// Ghidra program STCRDDEK (project ddw3-pal-sles-03936) read-only, no state changed:
//   disasm 246 insns, decompile, x-ref to (caller STCRDDEK_func_80089e98 @ 0x8008a058),
//   x-ref from (jal EXE 0x8001f648 + intra-function branches + stack-callback jalr).
// All 246 words verified equal to stcrddek.bin @ 0x61c4 (sha256 5bb84767...0708).
// EXE helper F0x8001f648 fills the sp+0x10 buffer (buf[40]); callback slots at
// buf+0x74/0x7c/0x84/0x88/0x8c/0x94 (same dispatch shape as CARDGAME/STFGTREP).
// Tables D_80044B38 (slot 0x424/4=265, arg 0x63e0000) and D_8004DE10 (slot 0x188/4=98).
// Upstream stcrddek.s used as guide only. No new headers or symbols.
// Status: exact_byte_match 984/984 with psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0,
// variant o2-g0-no-strength-reduce (base folds the cursor offset 0xf4 into s0).
// Matching notes: the +0x68 counter is updated in memory before the compare; the
// last draw call is written in both arms so jump2 cannot cross-jump the a0 copy;
// the loop base tb is loaded after s3 = 0; the later +0x58 read uses its own local.

extern void F0x8001f648(void *);
extern int D_80044B38[];
extern int D_8004DE10[];

void STCRDDEK_F0x80088e74(void *p) {
    unsigned int buf[40];
    int *tbl;
    int *tb;
    char *s0;
    int s1;
    int s2;
    int s3;
    int a0v;
    int v1;
    int a1v;
    void *disp;
    int poll;

    F0x8001f648(buf);
    ((void (*)(int, int))buf[31])(*(int *)((char *)p + 0x50), *(int *)((char *)p + 0x54));
    ((void (*)(int, int))buf[29])(0x280, 0);
    if (*(int *)((char *)p + 0x5c) != 0) {
        v1 = *(int *)((char *)p + 0x58) + 1;
        *(int *)((char *)p + 0x58) = v1;
        a0v = 0;
        if (v1 < 0x60)
            a0v = v1;
        *(int *)((char *)p + 0x58) = a0v;
        *(int *)((char *)p + 0x5c) = 0;
    } else {
        *(int *)((char *)p + 0x5c) = 1;
    }
    tbl = D_80044B38;
    disp = ((void *(*)(int))tbl[265])(0x63e0000);
    a1v = *(int *)((char *)p + 0x58);
    ((void (*)(void *, int, int, int))buf[33])(disp, 8, a1v, a1v);
    ((void (*)(int, int))buf[31])(*(int *)((char *)p + 0x50), *(int *)((char *)p + 0x54) - 1);
    if (*(int *)((char *)p + 0xc4) != 0) {
        if (*(int *)((char *)p + 0xc4) != 0x1000) {
            ((void (*)(int, int, int))buf[35])(*(int *)((char *)p + 0xc4), 0x1000, 0x1000);
            ((void (*)(int, int))buf[37])(0x140, 0x22);
        }
        disp = ((void *(*)(int))tbl[265])(0x63e0000);
        ((void (*)(void *, int, int, int))buf[33])(disp, 0x30, 0x7b, 0x1c);
    }
    s3 = 0;
    tb = D_80044B38;
    s2 = 0x50;
    s1 = 0x63;
    s0 = (char *)p;
    do {
        if (*(int *)(s0 + 0xf4) != 0) {
            ((void (*)(int, int, int))buf[35])(*(int *)(s0 + 0xf4), 0x1000, 0x1000);
            if (*(int *)(s0 + 0xf4) != 0x1000)
                ((void (*)(int, int))buf[37])(0x17, s1);
            disp = ((void *(*)(int))tb[265])(0x63e0000);
            ((void (*)(void *, int, int, int))buf[33])(disp, 0x41, 0x17, s2);
        }
        s2 += 0x2d;
        s1 += 0x2d;
        s3 += 1;
        s0 += 0x10;
    } while (s3 < 3);
    if (*(int *)((char *)p + 0xd4) != 0) {
        ((void (*)(int, int, int))buf[35])(*(int *)((char *)p + 0xd4), 0x1000, 0x1000);
        if (*(int *)((char *)p + 0xd4) != 0x1000)
            ((void (*)(int, int))buf[37])(0x140, 0x37);
        disp = ((void *(*)(int))D_80044B38[265])(0x63e0000);
        ((void (*)(void *, int, int, int))buf[33])(disp, 0x2d, 0x92, 0x1c);
    }
    if (*(int *)((char *)p + 0xe4) != 0) {
        ((void (*)(int, int))buf[31])(*(int *)((char *)p + 0x50), *(int *)((char *)p + 0x54) - 2);
        ((void (*)(int, int, int))buf[35])(0x1000, *(int *)((char *)p + 0xe4), 0x1000);
        if (*(int *)((char *)p + 0xe4) != 0x1000)
            ((void (*)(int, int))buf[37])(0x17, *(int *)((char *)p + 0x60) * 0x2d + 99);
        if (*(int *)((char *)p + 0x64) == 0) {
            poll = ((int (*)(void))D_8004DE10[98])();
            if (poll - *(int *)((char *)p + 0x6c) > 4) {
                poll = ((int (*)(void))D_8004DE10[98])();
                *(int *)((char *)p + 0x6c) = poll;
                *(int *)((char *)p + 0x68) += 1;
                if (*(int *)((char *)p + 0x68) > 15)
                    *(int *)((char *)p + 0x68) = 0;
            }
            ((void (*)(int))buf[34])(*(int *)((char *)p + 0x68));
            disp = ((void *(*)(int))D_80044B38[265])(0x63e0000);
            ((void (*)(void *, int, int, int))buf[33])(disp, 0x42, 0x17, *(int *)((char *)p + 0x60) * 0x2d + 0x50);
        } else {
            disp = ((void *(*)(int))D_80044B38[265])(0x63e0000);
            ((void (*)(void *, int, int, int))buf[33])(disp, 0x44, 0x17, *(int *)((char *)p + 0x60) * 0x2d + 0x50);
        }
    }
}
