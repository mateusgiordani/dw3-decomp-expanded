#include "common/types.h"

/* CARDGAME:0x80091eb0 (size 744, 0x2E8)
 * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xF200
 * Scope is exactly one framed function: prologue 27bdffc8 addiu sp,-0x38 at
 * 0x80091eb0; epilogue 03e00008 jr ra + 27bd0038 addiu sp,+0x38 at
 * 0x80092190/0x80092194; next prologue c8ffbd27 addiu sp,-0x38 at 0x80092198
 * = +0x2E8. Size 744 contiguous (186 words), no overlap.
 * PAL word check (primary checkout, read-only): first8 LE
 * 27bdffc8 afb20020 00808821 afb5002c 00c0a821 001510c0 00551023 000210c0
 * equal Ghidra disasm bytes word-for-word; last4 8fb1001c 8fb00018 03e00008
 * 27bd0038; body sha256 401c0b1c9524e3db (744 B at 0xF200).
 * Ghidra program CARDGAME (project ddw3-pal-sles-03936), read-only queries,
 * no cache mutation:
 *   disasm 0x80091eb0 (200-instr query) + disasm 0x80092074 (120-instr query)
 *     -> full 186-instr body recovered (prologue through jr/delay + next
 *     prologue confirmed).
 *   decompile 0x80091eb0 -> CARDGAME_F0x80091eb0(int,int,int): 12-slot scan,
 *     14-byte entry shift, 2 indirect calls, 64+12-byte struct copy, direct
 *     call FUN_8009170c, flag-propagation loop, final indirect call + stores.
 *   x-ref to 0x80091eb0: 1 caller CARDGAME_F0x80084320 at 0x8008481c
 *     (jal 0x80091eb0; a0=s1,a1=s0,a2=s2 pass-through handles + selector).
 *   x-ref from 0x80091eb0: 1 direct callee FUN_8009170c at 0x800920c0
 *     (jal 0x8009170c, args a0=p1,a1=p2,a2=sel,a3=slot); 3 indirect jalr via
 *     p2+0xF14 (p2,slot,0xE500,0x6100), p2+0xF3C (p2,slot,half@s0+2),
 *     p2+0xF24 (p2,found,5,0,stack 0x1000); rest are intra-function branches
 *     and stack spills.
 * Signature is conservative: p1/p2 are byte bases with raw offsets (no
 * invented struct); sel/slot/found are word-sized ints; halfword at dst+2 is
 * signed (lh). Callers pass (s1,s0,s2) so all three params are word handles.
 * Domain pack: none generated. Function touches card-entry bytes and p2 slot
 * arrays only; no battle/camera, skill, digimon-record, dialogue,
 * field/map/tile, sprite/rendering, disc-I/O, or overlay-load semantics were
 * identified (indirect callbacks at p2+0xF14/0xF3C/0xF24 are unidentified, so
 * no knowledge domain can be meaningfully attached; recomp C is never copied
 * per campaign rule).
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
 */

extern void CARDGAME_F0x8009170c(uint8_t *p1, uint8_t *p2, int32_t sel, int32_t slot);

typedef void (*cardgame_cb4_t)(uint8_t *p2, int32_t slot, int32_t kind, int32_t arg);
typedef void (*cardgame_cb3_t)(uint8_t *p2, int32_t slot, int32_t arg);
typedef void (*cardgame_cb5_t)(uint8_t *p2, int32_t slot, int32_t a2, int32_t a3, int32_t a4);

typedef struct { uint32_t w0, w1, w2; int16_t h; } __attribute__((packed, aligned(2))) CardRec14;

typedef struct { uint32_t w0, w1, w2, w3; } CardQW16;

typedef struct { uint32_t w0, w1, w2; } CardW12;

typedef struct { uint8_t prefix[0x146]; uint8_t flags[0]; } CardFlagView;

void CARDGAME_F0x80091eb0(uint8_t *p1, uint8_t *p2, int32_t sel) {
    uint8_t *t1;
    uint32_t orig;
    int32_t slot;
    int32_t found;
    int32_t i;
    int32_t off1;
    int32_t off2;
    uint8_t *dst;
    uint8_t *src;
    int16_t card;
    cardgame_cb4_t cb14;
    cardgame_cb3_t cb3c;
    uint16_t *slotbase;
    uint8_t *q;

    t1 = p1 + (sel * 114 + 0x72C);
    orig = t1[0];
    slot = (int32_t)orig;
    if (sel != 0) {
        slot = (int32_t)(orig + 6);
    }

    found = 0;
    src = t1 + (orig * 14 + 2);
    for (i = 0, off2 = 0x74c, off1 = 0x72e; i < 12; off2 += 14, i++, off1 += 14) {
        if (i < 6) {
            if (i >= (int32_t)p1[0x72C]) continue;
            src = p1 + off1;
        } else {
            if ((i - 6) >= (int32_t)p1[0x79E]) continue;
            src = p1 + off2;
        }
        if (((int8_t *)(p1 + i))[0x46F] != 0) {
            found = i;
            break;
        }
    }

    dst = t1 + orig * 14;
    *(CardRec14 *)(dst + 2) = *(const CardRec14 *)src;

    cb14 = *(cardgame_cb4_t *)(p2 + 0xF14);
    cb14(p2, slot, 0xE500, 0x6100);
    card = *(int16_t *)(dst + 2);
    cb3c = *(cardgame_cb3_t *)(p2 + 0xF3C);
    cb3c(p2, slot, (int32_t)card);

    {
        uint32_t *d = (uint32_t *)(p2 + slot * 76 + 0x108);
        uint32_t *s = (uint32_t *)(p2 + found * 76 + 0x108);
        uint32_t *e = s + 16;
        do {
            *(CardQW16 *)d = *(const CardQW16 *)s;
            d += 4;
            s += 4;
        } while (s != e);
        *(CardW12 *)d = *(const CardW12 *)s;
    }
    slotbase = (uint16_t *)(p2 + slot * 76 + 0x120);
    *slotbase = 0;

    CARDGAME_F0x8009170c(p1, p2, sel, slot);

    i = 0;
    if ((int32_t)((int8_t)p1[0x575]) - 1 > 0) {
        do {
            q = p1 + i * 8;
            if (q[0x585] == 0) {
                if (((CardFlagView *)p2)->flags[i + found * 76] != 0) {
                    ((CardFlagView *)p2)->flags[i + slot * 76] = 1;
                }
            }
            i++;
        } while (i < (int32_t)((int8_t)p1[0x575]) - 1);
    }

    p1[0x422] = 1;
    (*(cardgame_cb5_t *)(p2 + 0xF24))(p2, found, 5, 0, 0x1000);
    *(uint32_t *)(p1 + 0x424) = 7;
}
