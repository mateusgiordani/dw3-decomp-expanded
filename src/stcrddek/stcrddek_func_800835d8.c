/* STCRDDEK:0x800835d8 -- deck-menu candidate (476 B, file off 2344 = vaddr - 0x80082cb0).
 * PAL bytes (authority): reference/extracted/pro/stcrddek.bin base 0x80082cb0,
 * file off 2344, 476 B; word-for-word match vs inventory body-800835d8 verified.
 * Prologue addiu sp,sp,-0x130, saves ra/s0-s7; epilogue restores + jr ra /
 * addiu sp,+0x130. Prev STCRDDEK:0x80083594 (68 B) ends at entry; next
 * STCRDDEK:0x800837b4 starts at +476: contiguous, no overlap.
 * Ghidra ddw3-pal-sles-03936/STCRDDEK read-only (function list only, no cache
 * mutation): STCRDDEK_func_800835d8 size 476, agrees with inventory.
 * Disassembly (capstone on PAL bytes, MIPS32 LE, base 0x800835d8, 119 insns):
 * two direct EXE calls F0x8001f648(sp+0x10) and F0x8001ebf8(sp+0xb0) fill a
 * stack callback table; then indirect calls fn84(0x280,0), fn8c(arg+0x54,
 * arg+0x58), fnEC(0x140,0x100), fnF0(0x300,0x100), fnE8(arg+0x54,arg+0x58);
 * then per-index loop over count *(arg+0x5c): halfword lookup from a
 * D_80048D34-indexed address (lh +0x63e), fnDC(that halfword), divide index
 * by 9 (magic 0x38E38E39: q = i/9, r = i%9), v0 = D_80044B38.fn424(0x63E0000),
 * x = (r<<5)+0x10, y = (q<<5)+0x3a, fn94(v0, **(sp+0xb0)+0x52, x, y),
 * fnF4(r,q), fnE4(x,y); i++. Grid suggests deck/card layout; name is a
 * deck-menu CANDIDATE only -- semantics unconfirmed, types/names conservative.
 * Direct caller (binary jal search): STCRDDEK 0x8008391c (inside 0x8008397c).
 * Portable C only, no explicit register variables.
 * EXACT_BYTE_MATCH 2026-09-08 (retry rev 2): fn_exact_pipeline base O2/G0
 * psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, sha256
 * bbedd216df0e72b8c1e2a9439dbb5c64eace96f247f01cce21a77f074eff0927,
 * difference_count 0. Decisive idioms: (1) r = i % 9 before q = i / 9
 * (divmod combine emits move s2,s3 + delay-slot subu for r); (2) inline
 * cast-and-call for the two (arg+0x54,arg+0x58) sites (PAL load order
 * a0,a1,v0 + load-delay nop; hoisted fn local schedules v0 first);
 * (3) multi-statement addr formation for the lh base (addu v0,v0,s7
 * operand order; single expression canonicalizes to addu v0,s7,v0). */
#include "common/types.h"

extern void F0x8001f648(void *buf);
extern void F0x8001ebf8(void *buf);

extern uint8_t D_80048D34[];
extern struct {
    uint8_t pad[0x424];
    int32_t (*fn424)(int32_t a0);
} D_80044B38;

typedef struct {
    uint8_t pad[0x50];
    void *p50;
    int32_t v54;
    int32_t v58;
    int32_t count5c;
} STCRDDEK_835d8_Arg;

typedef void (*STCRDDEK_835d8_Fn1)(int32_t a0);
typedef void (*STCRDDEK_835d8_Fn2)(int32_t a0, int32_t a1);
typedef void (*STCRDDEK_835d8_Fn4)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

void STCRDDEK_func_800835d8(STCRDDEK_835d8_Arg *arg) {
    uint8_t work[0xF8];
    uint8_t *deckbase;
    void *b38;
    STCRDDEK_835d8_Fn2 fn;
    STCRDDEK_835d8_Fn4 fn4;
    int32_t i;
    int32_t r;
    int32_t q;
    int32_t x;
    int32_t y;
    int32_t head;
    int32_t addr;
    int32_t cell;
    int32_t hv;
    uint8_t ch;

    F0x8001f648(work);
    fn = *(STCRDDEK_835d8_Fn2 *)(work + 0x74);
    fn(0x280, 0);
    i = 0;
    (*(STCRDDEK_835d8_Fn2 *)(work + 0x7C))(arg->v54, arg->v58);
    F0x8001ebf8(work + 0xA0);
    fn = *(STCRDDEK_835d8_Fn2 *)(work + 0xDC);
    fn(0x140, 0x100);
    fn = *(STCRDDEK_835d8_Fn2 *)(work + 0xE0);
    fn(0x300, 0x100);
    (*(STCRDDEK_835d8_Fn2 *)(work + 0xD8))(arg->v54, arg->v58);
    if (arg->count5c <= 0) {
        return;
    }
    deckbase = D_80048D34;
    b38 = &D_80044B38;
    do {
        head = *(int32_t *)((uint8_t *)arg->p50 + 0x5C);
        addr = ((head * 51 + i) << 1) + (int32_t)deckbase;
        cell = *(int16_t *)(addr + 0x63E);
        (*(STCRDDEK_835d8_Fn1 *)(work + 0xCC))(cell);
        r = i % 9;
        q = i / 9;
        hv = (*(int32_t (**)(int32_t))((uint8_t *)b38 + 0x424))(0x63E0000);
        x = (r << 5) + 0x10;
        y = (q << 5) + 0x3A;
        ch = **(uint8_t **)(work + 0xA0);
        fn4 = *(STCRDDEK_835d8_Fn4 *)(work + 0x84);
        fn4(hv, (int32_t)(ch + 0x52), x, y);
        (*(STCRDDEK_835d8_Fn2 *)(work + 0xE4))(r, q);
        (*(STCRDDEK_835d8_Fn2 *)(work + 0xD4))(x, y);
        i++;
    } while (i < arg->count5c);
}
