// STCRDDEK:0x80084440 (896B, 224 words, file-off 0x1790 = vaddr - base 0x80082cb0).
// PAL: reference/extracted/pro/stcrddek.bin (35748 B, sha256
// 07a853a5774abf4b9f4e3cf57d93bb3f144dd1bde509b8472e4ff569d81c3d73).
// Boundary: prologue addiu sp,sp,-0x80 + sw s0/s1/s2/s3,ra at 0x80084440-0x80084460
// (beq a2,zero -> 0x800846e4 in-between); epilogue lw ra/s3/s2/s1/s0 + jr ra +
// addiu sp,sp,0x80 at 0x800847a4-0x800847bc. 224 words to next prologue.
// Ghidra project ddw3-pal-sles-03936, program STCRDDEK, read-only: disasm 224
// insns verified word-equal vs PAL (first word 27bdff80, last 27bd0080);
// decompile + x-ref to (3 direct jal callers: 0x80085aa0, 0x80085f44, 0x80086130;
// upstream stcrddek.s jal words 0x0c021110 at file-off 0x2df0/0x3294/0x3480 agree)
// + x-ref from (1 cross-module jal EXE 0x8001ebf8, internal cond jumps).
// Callee EXE:0x8001ebf8 (symbols/functions.csv DISASSEMBLED, 184B) fills the
// 88-byte stack buffer at sp+0x10: +0x0 data pointer, +0x2c callback(int),
// +0x50 callback(void)->int -- same protocol as CARDGAME:0x80085fd0
// (cardgame_f0x80085fd0.c: buf[22], buf[0]/buf[11], F0x8001ebf8).
// D_8005CCA8 (int at 0x8005cca8, s3 base via lui 0x8006/-0x3358) and
// D_80044B38.fn414 (slot at 0x80044f4c = base+0x414, s2 base) mirror the
// STCRDDEK_func_8008a100 TU idiom (D_80044B38/D_8005CCA8 externs).
// p2 is 7 object pointers at +0x44..0x60; each object dispatches via slots
// +0x114 (set3), +0x118 (set2i), +0x144 (set1i), +0x148 (set1i) -- indices
// 68/70/81/82. p1 carries two counts at +0x74/+0x78 selecting one int16 from
// the table at +0x88. Deck/album role unconfirmed; names conservative.
// stcrddek.s is GUIDE only; never copied. No Ghidra state change.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0; exact_byte_match 896/896.
// Matching notes: s is an int loaded from the int16 table (lh, no re-extension);
// the ret != 0 DRAW(0x60) calls +0x114 through an int return type so jump2 does
// not cross-jump it with the id-list DRAW(0x60) (same shape as STCRDSHP:0x80083780).
extern void F0x8001ebf8(void *);
extern int D_8005CCA8;
extern struct {
    char pad[0x414];
    int (*fn414)(int);
} D_80044B38;

void STCRDDEK_func_80084440(void *p1, void *p2, int flag)
{
    unsigned int buf[22];
    int s;
    int u;
    int ret;
    void *o;

    if (flag != 0) {
        int n;

        n = *(int *)((char *)p1 + 0x78) + *(int *)((char *)p1 + 0x74);
        s = *(short *)((char *)p1 + (n << 1) + 0x88);
        u = D_80044B38.fn414(D_8005CCA8 + 0x16);
        o = *(void **)((char *)p2 + 0x44);
        ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, s);
        F0x8001ebf8(buf);
        ((void (*)(int))buf[11])(s);
        ret = ((int (*)(void))buf[20])();
        if (ret != 0) {
            o = *(void **)((char *)p2 + 0x48);
            ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
            o = *(void **)((char *)p2 + 0x4c);
            ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
            u = D_80044B38.fn414(D_8005CCA8 + 0x1d);
            o = *(void **)((char *)p2 + 0x60);
            ((int (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, s);
        } else {
            u = D_80044B38.fn414(D_8005CCA8 + 0x32);
            o = *(void **)((char *)p2 + 0x48);
            ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, 8);
            o = *(void **)((char *)p2 + 0x4c);
            ((void (*)(void *, int, int))*(void **)((char *)o + 0x118))(o, 0, ((unsigned char *)buf[0])[5]);
            o = *(void **)((char *)p2 + 0x4c);
            ((void (*)(void *, int))*(void **)((char *)o + 0x148))(o, 1);
            if (s == 0x45 || s == 0x70 || s == 0x9b || s == 0xc6 || s == 0xf1) {
                u = D_80044B38.fn414(D_8005CCA8 + 0x1d);
                o = *(void **)((char *)p2 + 0x60);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, s);
            } else {
                o = *(void **)((char *)p2 + 0x60);
                ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
                u = D_80044B38.fn414(D_8005CCA8 + 0x32);
                o = *(void **)((char *)p2 + 0x50);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, 0x11);
                o = *(void **)((char *)p2 + 0x54);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x118))(o, 0, ((unsigned char *)buf[0])[1]);
                o = *(void **)((char *)p2 + 0x54);
                ((void (*)(void *, int))*(void **)((char *)o + 0x148))(o, 1);
                u = D_80044B38.fn414(D_8005CCA8 + 0x32);
                o = *(void **)((char *)p2 + 0x58);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x114))(o, u, 0x12);
                o = *(void **)((char *)p2 + 0x5c);
                ((void (*)(void *, int, int))*(void **)((char *)o + 0x118))(o, 0, ((unsigned char *)buf[0])[2]);
                o = *(void **)((char *)p2 + 0x5c);
                ((void (*)(void *, int))*(void **)((char *)o + 0x148))(o, 1);
                return;
            }
        }
    } else {
        o = *(void **)((char *)p2 + 0x44);
        ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
        o = *(void **)((char *)p2 + 0x48);
        ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
        o = *(void **)((char *)p2 + 0x4c);
        ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
        o = *(void **)((char *)p2 + 0x60);
        ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
    }
    o = *(void **)((char *)p2 + 0x50);
    ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
    o = *(void **)((char *)p2 + 0x54);
    ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
    o = *(void **)((char *)p2 + 0x58);
    ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
    o = *(void **)((char *)p2 + 0x5c);
    ((void (*)(void *, int))*(void **)((char *)o + 0x144))(o, 0);
}
