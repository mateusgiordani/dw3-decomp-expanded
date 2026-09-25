/* STCRDDEK:0x80084278 (size 456, 0x1C8; file off 0x15C8 = vaddr - 0x80082cb0).
 * PAL bytes (authority): prologue addiu sp,sp,-0x30 at 0x80084278; epilogue
 * jr ra / addiu sp,sp,+0x30 at 0x80084438; next function prologue
 * addiu sp,sp,-0x80 at 0x80084440. All 114 words verified against
 * reference/extracted/pro/stcrddek.bin at coordinator root (first word
 * d0ffbd27, last word 3000bd27; full 456-byte hex dump in handoff).
 * Ghidra ddw3-pal-sles-03936 program STCRDDEK (read-only, no mutation):
 * disasm 0x80084278 (64 insns) + tail 0x80084374/0x80084410, decompile as
 * STCRDDEK_func_80084278, x-ref to: callers at 0x80085984 (a2=1),
 * 0x80085f34 (a2=1), 0x8008610c (a2=0) via jal.
 * Body: 8-slot row refresh. rows holds 8 groups of 3 object-pointer words
 * starting at word 0x19 (bytes 0x64/0x68/0x6C); the rows[i*3+k] index form lets
 * the -O2 strength reducer keep rows itself as the loop base. When mode
 * is 0 every object gets its +0x144 method called with 0. Otherwise each slot
 * reads a signed-short id from data+0x88 indexed by (*(data+0x78)+i): id 0
 * repeats the three +0x144 calls with 0; nonzero id fetches two resources via
 * the EXE helper behind 0x80044B38+0x414 with base *(0x8005CCA8) (+0x16 for
 * the id, +0x32 with count 8), calls +0x114 on the first two objects, +0x118
 * on the third with the signed byte at data+id+0x2FE, then the tail calls the
 * third object via +0x144 (flag 0) or +0x148 (flag 1).
 * Keep each final indirect call in its own branch: GCC merges their tail
 * and preserves the PAL callback-load/argument scheduling. Exact with
 * PsyQ GCC 2.8.1 / ASPSX 2.79, O2/G0 base. Widget layout remains unknown.
 */
#include <stdint.h>

typedef void (*STCRDDEK_m114_t)(void *obj, void *res, int32_t arg);
typedef void (*STCRDDEK_m118_t)(void *obj, int32_t zero, int32_t val);
typedef void (*STCRDDEK_m144_t)(void *obj, int32_t flag);

typedef struct {
    uint8_t pad[0x414];
    void *(*fn)(int32_t arg);
} stcrddek_exe_tbl_t;

/* EXE-resident objects (resolved via pipeline --symbol, not committed): */
extern stcrddek_exe_tbl_t stcrddek_tbl_80044B38;
extern int32_t stcrddek_word_8005CCA8;

void STCRDDEK_func_80084278(uint8_t *data, uint32_t *rows, int32_t mode)
{
    int32_t i;

    /* PAL block order: the filled-slot loop is first (entered by fall-through),
     * the mode==0 loop follows it; the entry test is beq-to-empty-block. The
     * first loop therefore exits forward over the second (beq + j-back) while
     * the second loop exits by fall-through (bne-back only). */
    if (mode != 0) {
        i = 0;
        do {
            int32_t idx = *(int32_t *)(data + 0x78) + i;
            int16_t id = *(int16_t *)(data + (idx << 1) + 0x88);
            void *o;

            /* beq-to-empty layout: the taken branch is the empty slot, the
             * fall-through is the filled slot. GCC merges the final call
             * tails while preserving the two callback-slot choices. */
            if (id != 0) {
                void *res;

                res = stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x16);
                o = (void *)rows[i * 3 + 0x19];
                (*(STCRDDEK_m114_t *)((uint8_t *)o + 0x114))(o, res, id);
                res = stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x32);
                o = (void *)rows[i * 3 + 0x1A];
                (*(STCRDDEK_m114_t *)((uint8_t *)o + 0x114))(o, res, 8);
                o = (void *)rows[i * 3 + 0x1B];
                (*(STCRDDEK_m118_t *)((uint8_t *)o + 0x118))(o, 0, *(int8_t *)(data + id + 0x2FE));
                /* Reload: a0 is clobbered by the call above (PAL re-reads
                 * the third word here), so the tail object is not kept live. */
                o = (void *)rows[i * 3 + 0x1B];
                (*(STCRDDEK_m144_t *)((uint8_t *)o + 0x148))(o, 1);
            } else {
                o = (void *)rows[i * 3 + 0x19];
                (*(STCRDDEK_m144_t *)((uint8_t *)o + 0x144))(o, 0);
                o = (void *)rows[i * 3 + 0x1A];
                (*(STCRDDEK_m144_t *)((uint8_t *)o + 0x144))(o, 0);
                o = (void *)rows[i * 3 + 0x1B];
                (*(STCRDDEK_m144_t *)((uint8_t *)o + 0x144))(o, 0);
            }
            i++;
        } while (i < 8);
    } else {
        i = 0;
        do {
            void *o0 = (void *)rows[i * 3 + 0x19];

            (*(STCRDDEK_m144_t *)((uint8_t *)o0 + 0x144))(o0, 0);
            o0 = (void *)rows[i * 3 + 0x1A];
            (*(STCRDDEK_m144_t *)((uint8_t *)o0 + 0x144))(o0, 0);
            o0 = (void *)rows[i * 3 + 0x1B];
            (*(STCRDDEK_m144_t *)((uint8_t *)o0 + 0x144))(o0, 0);
            i++;
        } while (i < 8);
    }
}
