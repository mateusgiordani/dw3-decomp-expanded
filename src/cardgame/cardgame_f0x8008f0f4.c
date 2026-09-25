// CARDGAME:0x8008f0f4, 484-byte PAL body at verified base 0x80082cb0.
// Boundary: prologue 27bdffd8 (addiu sp,sp,-0x28) at file-off 0x0c444;
// epilogue jr ra + 27bd0028 at 0x8008f2d0/0x8008f2d4; next CARDGAME:0x8008f2ec.
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only): 121-insn disasm matches PAL
// word-for-word (head 27bdffd8 afb10014 00808821 afb40020 00a0a021;
// tail 8fbf0024 8fb40020 8fb3001c 8fb20018 8fb10014 8fb00010 03e00008 27bd0028).
// Xref: 1 caller CARDGAME_F0x80084320 via 0x80085674 UNCONDITIONAL_CALL.
// Decompile CARDGAME_F0x8008f0f4(int,int,int): mode byte at p1+0x422 dispatches
// 1 (twin EXE-vector accumulations plus two p2+0xea0 method calls) and 2
// (per-index halfword table copy with stride 200); returns 0/1 via s3.
// Codegen notes (hypothesis 1): EXE vector is extern word-array base
// DAT_8004DE10 with slot 0x63 (lui 0x8005 + addiu -0x21f0 into s0, held across
// both jalr calls; lw 0x18c(s0)), matching the sibling DAT_8004B7D0 idiom.
// idx*200 stays inline at each site (PAL recomputes the ((2i+i)*8+i)*8 chain
// per site, hoisting only idx*2); entry gate compares ret<limit so the PAL
// slt-against-s3 (not blez/$zero) is expressible; cursor is re-read for ++
// (lh for address scaling, lhu for increment+store).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Full-range exact C: r9 H6, SHA-256 17e1c58f5c0162f4eab6e1a7bc11f382
// 52b188bf7fdabff1fc803aaea580111b. Explicit backedge prevents invariant
// motion; distinct loaded-value lifetimes let local allocation choose PAL
// homes. See docs/c-matching-guide/submissions/cardgame-8008f0f4/strategy-r9.md.
#include <stdint.h>

typedef int32_t (*cardgame_tick_t)(void);
typedef int32_t (*cardgame_f0f4_sub_t)(int32_t self, int32_t idx, int32_t kind, int32_t val);

extern int32_t DAT_8004DE10[];

int32_t CARDGAME_F0x8008f0f4(int32_t p1, int32_t p2, int32_t idx)
{
    int32_t ret = 0;
    int32_t w;
    int32_t total;
    int32_t adv;
    int32_t k;
    int32_t idx2;
    int32_t t;
    int32_t kk;
    /* E4 (chain-life split): entry stride chain in ekk, loop chain in kk.
       Copy-free (no data flows between them); drops the entry refs from kk
       and re-ranks the loop temps. Measured: entry region now exact. */
    int32_t ekk;
    /* r8 H4/H5: sh e o destino de k<<1/merge; t so e srcbase (p1+sh).
       Destinos distintos fazem o sched2 emitir recbase antes do merge.
       off forca rec=p1+off base-first. */
    int32_t sh;
    int32_t off;

    /* PAL outlines both bodies after the dispatch: single-arm if-goto tests
       jump forward to each body label on true (beq, never inverted since there
       is no else), and a bare goto jumps over both bodies for the
       neither-mode path. Sibling 8642c uses goto-joined tails the same way. */
    if (*(uint8_t *)(p1 + 0x422) == 1) {
        goto body1;
    }
    if (*(uint8_t *)(p1 + 0x422) == 2) {
        goto body2;
    }
    goto epi;
body1:
    w = ((cardgame_tick_t)DAT_8004DE10[0x63])();
    *(int32_t *)(p1 + 0x424) += w;
    w = ((cardgame_tick_t)DAT_8004DE10[0x63])();
    total = *(int32_t *)(p1 + 0x430) + w;
    *(int32_t *)(p1 + 0x430) = total;
    if (total > 6) {
            /* PAL keeps a 0/1 flag in a0: adv=0 is sunk into the blez delay
               slot, adv=1 doubles as the increment register in addu, the flag
               is tested with bne, and that delay slot carries the first call
               arg (move a0,s4). */
            adv = 0;
            if (*(int32_t *)(p1 + 0x428) > 0) {
                adv = 1;
                *(int32_t *)(p1 + 0x428) -= 1;
                *(int32_t *)(p1 + 0x42c) += adv;
            }
            if (adv == 0) {
                *(uint8_t *)(p1 + 0x422) = 2;
            }
            ((cardgame_f0f4_sub_t)*(uint32_t *)(p2 + 0xea0))(p2, idx, 6, *(int32_t *)(p1 + 0x428));
            ((cardgame_f0f4_sub_t)*(uint32_t *)(p2 + 0xea0))(p2, idx, 7, *(int32_t *)(p1 + 0x42c));
            *(int32_t *)(p1 + 0x430) -= 7;
        }
    goto epi;
body2:;
    /* PAL keeps idx*2 in a3 (computed once, shared by entry check, loop and
       final store) and recomputes the ((2i+i)*8+i)*8 stride per site with
       shifts/adds, giving the compiler no mult to strength-reduce. */
    /* The entry check builds the stride chain into ekk first, which also
       fixes the entry addu operand order. */
    /* E2 (pre-entry reset, cf. matched 800a0754/800886c8): zero k before the
       entry check so dbr can sink the predecessor move into the entry-beq
       delay instead of stealing the post-loop final-chain head (insn 272).
       Harmless on the exit path (k dead there); loop shape unchanged. */
    idx2 = idx * 2;
    ekk = idx2 + idx;
    ekk <<= 3;
    ekk += idx;
    ekk <<= 3;
    k = 0;
    if (ret < *(int16_t *)(p1 + ekk + 0x5a6)) {
        int32_t rec;
        int32_t c;
        int32_t value;
        int32_t cursor;
        int32_t limit;
        /* A structured loop hoists the invariant stride after lifetime
           separation. This backedge preserves its PAL recomputation. */
        loop_body: {
            sh = k << 1;
            k++;
            kk = idx2 + idx;
            kk <<= 3;
            kk += idx;
            kk <<= 3;
            c = p1 + kk;
            sh = sh + kk;
            rec = *(int16_t *)(c + 0x5a2);
            t = p1 + sh;
            rec <<= 1;
            rec += kk;
            value = *(uint16_t *)(t + 0x600);
            off = rec;
            rec = p1 + off;
            *(uint16_t *)(rec + 0x614) = (uint16_t)value;
            cursor = *(uint16_t *)(c + 0x5a2);
            limit = *(int16_t *)(c + 0x5a6);
            cursor++;
            *(int16_t *)(c + 0x5a2) = (int16_t)cursor;
        }
        if (k < limit) goto loop_body;
    }
    /* The final store recomputes idx*200 into w (dead after mode 1); PAL
       re-emits the chain into v0 here, and naming the intermediate matches
       the entry addu operand order. */
    w = idx * 200;
    *(int16_t *)(p1 + w + 0x5a6) = 0;
    ret = 1;
epi:
    return ret;
}
