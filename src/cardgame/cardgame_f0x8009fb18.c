/*
 * CARDGAME:0x8009fb18 CARDGAME_F0x8009fb18
 * 704 bytes at CARDGAME.PRO offset 0x1ce68 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009fb18, jump table (.rodata) at 0x800838e8
 *  Symbols     CARDGAME_F0x8009fb18=0x8009fb18
 *  Compare     704 bytes from 0x8009fb18 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009fb18
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936, 704-byte body + 40-byte switch table.
 *
 * Reference base 0x80082cb0; table at 0x800838e8; next entry 0x8009fdd8.
 *
 * Exact with pinned PsyQ GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79, -O2 -G0 base.
 *
 * Each callback branch has a single-assignment offset: GCC can reduce the full
 * 14-byte stride and emits the PAL cursor order. Sharing one offset assigned in
 * all five branches instead reduces the 7-byte intermediate (724 bytes).
 *
 * The selector header has a separate address local. Case 0 computes its record
 * address within each half branch, ending the offset lifetime before the kind
 * load; this preserves PAL v0/v1 allocation.
 *
 * No semantic field names are asserted; widths and offsets follow PAL accesses.
 */

#include <stdint.h>

typedef int (*cardgame_9fb18_cb_t)(void *env, int arg);

void CARDGAME_F0x8009fb18(uint8_t *p, uint8_t *q)
{
    int i;
    uint8_t *t;
    uint8_t *e;

    i = 0;
    do {
        int kind;
        unsigned int mode;

        p[i + 0x46f] = 0;
        kind = (int8_t)p[0x575];
        {
            uint8_t *header;
            header = p + kind * 8;
            mode = header[0x585];
        }
        if (mode < 10) {
            switch (mode) {
            case 0: {
                int o;
                if (i < 6) {
                    if (i >= p[0x72c])
                        break;
                    o = i * 14 + 0x72e;
                    e = p + o;
                } else {
                    if (i - 6 >= p[0x79e])
                        break;
                    o = i * 14 + 0x74c;
                    e = p + o;
                }
                t = p + (int8_t)p[0x575] * 8;
                if (e[0xc] == t[0x586])
                    p[i + 0x46f] = 1;
                break;
            }
            case 2:
                if (i >= 6)
                    break;
                if (i >= p[0x72c])
                    break;
                p[i + 0x46f] = 1;
                break;
            case 1:
                if (i < 6)
                    break;
                if (i - 6 >= p[0x79e])
                    break;
                p[i + 0x46f] = 1;
                break;
            case 3:
                p[i + 0x46f] = 1;
                break;
            case 4: {
                int a;
                int r;
                if (i < 6)
                    a = *(int16_t *)(p + i * 14 + 0x72e);
                else {
                    int offset;
                    offset = (i - 6) * 14;
                    a = *(int16_t *)(p + offset + 0x7a0);
                }
                r = ((cardgame_9fb18_cb_t)*(void **)(q + 0xf40))(q, a);
                if (r != 1)
                    p[i + 0x46f] = 1;
                break;
            }
            case 5: {
                int a;
                int r;
                if (i < 6)
                    a = *(int16_t *)(p + i * 14 + 0x72e);
                else {
                    int offset;
                    offset = (i - 6) * 14;
                    a = *(int16_t *)(p + offset + 0x7a0);
                }
                r = ((cardgame_9fb18_cb_t)*(void **)(q + 0xf40))(q, a);
                if (r != 2)
                    p[i + 0x46f] = 1;
                break;
            }
            case 6: {
                int a;
                int r;
                if (i < 6)
                    a = *(int16_t *)(p + i * 14 + 0x72e);
                else {
                    int offset;
                    offset = (i - 6) * 14;
                    a = *(int16_t *)(p + offset + 0x7a0);
                }
                r = ((cardgame_9fb18_cb_t)*(void **)(q + 0xf40))(q, a);
                if (r == 3)
                    p[i + 0x46f] = 1;
                break;
            }
            case 7: {
                int a;
                int r;
                if (i < 6)
                    a = *(int16_t *)(p + i * 14 + 0x72e);
                else {
                    int offset;
                    offset = (i - 6) * 14;
                    a = *(int16_t *)(p + offset + 0x7a0);
                }
                r = ((cardgame_9fb18_cb_t)*(void **)(q + 0xf40))(q, a);
                if (r != 4)
                    p[i + 0x46f] = 1;
                break;
            }
            case 8: {
                int a;
                int r;
                if (i < 6)
                    a = *(int16_t *)(p + i * 14 + 0x72e);
                else {
                    int offset;
                    offset = (i - 6) * 14;
                    a = *(int16_t *)(p + offset + 0x7a0);
                }
                r = ((cardgame_9fb18_cb_t)*(void **)(q + 0xf40))(q, a);
                if (r == 6)
                    p[i + 0x46f] = 1;
                break;
            }
            case 9:
                break;
            }
        }
        i++;
    } while (i < 12);
}
