// CARDGAME:0x80096828 (size 296, 0x128)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x13b78
// Framed function: prologue addiu sp,-0x88 (16 bytes outgoing args at
// sp+0x00..0x0f, 0x58-byte locals at sp+0x10..sp+0x67, saves s0-s6,ra at
// 0x68..0x84), body, epilogue jr ra / addiu sp,+0x88. Prev CARDGAME bytes
// end 0x80096824 (jr ra); next CARDGAME:0x80096950 starts at +0x128.
// Ghidra CARDGAME (project ddw3-pal-sles-03936, read-only disasm/decompile/
// xrefs, no import/mutation): all 74 words equal the PAL bytes.
// Callers (x-ref): 3 jal sites in CARDGAME_F0x8009f110 (a0 = 0, 1, 2), each
// storing the v0 return value; a1/a2 come from the halfwords at
// 0x800a5d48/0x800a5d4a and are stored back as halfwords.
// Callees: EXE F0x8001ebf8 fills the 0x58-byte locals; EXE EXE_F0x80014504
// allocates the 0x74-byte obj with tpl CARDGAME_F0x80096418, flag 0x1c; the
// per-iteration indirect callback comes from the locals slot at +0x2c.
// Data: EXE halfword table base 0x80048d34, read at +0x63e, indexed by
// a0*102 + 2*i over 40 iterations; 6 byte counters at obj+0x60..0x65.
// Stored handlers: CARDGAME_F0x800967f8 -> obj+0x6c,
// CARDGAME_F0x80096808 -> obj+0x70.
// Revision 7 (GCC 2.8.1 SN32 / ASPSX 2.79 O2, exact):
// - the table base is held as an integer and added as off + base, so the
//   address add keeps PAL's offset-first operand order (pointer arithmetic
//   puts the pointer first);
// - a1/a2 are int32_t parameters narrowed at their halfword stores, which lets
//   all three parameter moves precede the first call's argument setup as in PAL.
// Status: exact candidate; parent reproduction pending.
#include <stdint.h>

extern void F0x8001ebf8(void *buf);
extern void *EXE_F0x80014504(void *tpl, int32_t size, int32_t flag);
extern void CARDGAME_F0x80096418(void);
extern void CARDGAME_F0x800967f8(void);
extern void CARDGAME_F0x80096808(void);
extern uint8_t D0x80048d34[];

typedef void (*CardCb96828)(int32_t arg);

typedef struct {
    uint8_t *cur;        /* +0x00: index-byte pointer (lw + lbu each pass) */
    uint8_t unk04[0x28]; /* +0x04 */
    CardCb96828 cb;      /* +0x2c: per-iteration indirect callback */
    uint8_t unk30[0x28]; /* +0x30 */
} Ctx96828;              /* 0x58 bytes at sp+0x10, saves above */

uint8_t *CARDGAME_F0x80096828(int32_t a0, int32_t a1, int32_t a2)
{
    Ctx96828 ctx;
    uint8_t *newobj;
    uint8_t *obj;
    uint8_t *tab;
    int32_t base;
    int32_t off;
    int32_t i;
    uint8_t *p;
    uint8_t *slot;
    uint8_t *o;

    F0x8001ebf8(&ctx);
    newobj = (uint8_t *)EXE_F0x80014504((void *)CARDGAME_F0x80096418, 0x74, 0x1c);
    i = 5;
    obj = newobj;
    p = obj + i;
    do {
        p[0x60] = 0;
        p--;
        i--;
    } while (i >= 0);
    i = 0;
    base = (int32_t)D0x80048d34;
    off = a0 * 102;
    do {
        tab = (uint8_t *)(off + base);
        ctx.cb(*(int16_t *)(tab + 0x63e));
        off += 2;
        slot = obj + (*ctx.cur - 1);
        slot[0x60]++;
        i++;
    } while (i < 0x28);
    o = obj;
    *(void **)(o + 0x6c) = (void *)CARDGAME_F0x800967f8;
    *(int32_t *)(o + 0x50) = a0;
    *(int16_t *)(o + 0x58) = (int16_t)a1;
    *(int16_t *)(o + 0x5a) = (int16_t)a2;
    o[0x67] = 0;
    *(void **)(o + 0x70) = (void *)CARDGAME_F0x80096808;
    return o;
}
