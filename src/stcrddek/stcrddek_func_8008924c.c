// STCRDDEK:0x8008924c (size 264, 0x108)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x659C
// Inventory: reports/investigations/pro-310-triage/modules/stcrddek-inventory.json
//   body-8008924c (file_offset 26012, 264B); Ghidra range [0x8008924c,0x80089354)
// Prologue: addiu sp,-0x90 ; save s0-s7,ra ; a0=sp+0x10 -> jal 0x8001ebf8 (EXE work-buffer init)
// Body: 3 outer iters; per iter zero 6 words at state+off+0x74, then 40 iters:
//   h = *(short *)(0x80048D34 + 0x63E + roff) [eff. 0x80049372+roff]; callback(*(sp+0x3C))(h);
//   b = **(u8 **)(sp+0x10); state[off + (b-1)*4 + 0x74]++
// Caller: STCRDDEK_func_80089354 default case (jal at 0x800893a8).
// Callee: EXE F0x8001ebf8 (0x8001ebf8) fills 0x54-byte stack work buffer; per-iter
//   indirect callback is work+0x2C (EXE F0x8001e7ec: short-keyed table update, global side effects).
// Data: s7 anchor 0x80048D34 + lh 0x63E => eff. short table at 0x80049372, stride 0x66;
//   sibling site (case 0x36) indexes 0x66-stride structs at 0x8004935C, so our shorts sit at
//   struct+0x16 with 40 shorts filling the 0x66 struct (0x16 + 80 = 0x66). Exact struct TBD.
// Semantic (unconfirmed): deck-menu candidate histogram -- 3 groups x 6 buckets at state+0x74..0xB8,
//   40 card-key samples per group; bucket picked by callback-updated byte (*work). Menu execution
//   itself is NOT confirmed (name-based role hypothesis only).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 variant
//   o2-g0-no-strength-reduce (-fno-strength-reduce). Base -O2 reverses the trivial
//   6-trip zero loop to countdown (li 5/bgez, 260B); with SR off it stays count-up
//   (clear/slti 6/bne, 264B). Loop bodies containing calls (the 40-loop) are never
//   reversed, so they match under both. dbase materialized in the outer for-init
//   places lui/addiu between the counter inits; comma-inits (counter first) fix the
//   clear-s0 placement; `roff + (int)dbase` defeats pointer-first addu canonicaliza-
//   tion (addu roff,base) and keeps the lh 0x63E displacement; `a = idx + t` fixes
//   the bucket addu order.
// Status: C_MATCHING -- fn_exact_pipeline exact_byte_match, difference_count 0,
//   sha aaac35c8... (see reports/handoffs/stcrddek-8008924c-c-recovery.md).
// Types preserve uncertainty: state layout unknown (byte-addressed); D_80048D34 blob TBD.

typedef void (*StcrddekCb)(short v);

extern unsigned char D_80048D34[];
void F0x8001ebf8(void *buf);

void STCRDDEK_func_8008924c(char *state)
{
    unsigned char work[0x54];
    StcrddekCb fn;
    unsigned char *bp;
    unsigned char *dbase;
    short h;
    int i;
    int n;
    int offA;
    int offB;
    int q;
    int t;
    int roff;
    int idx;
    int a;
    int cnt;

    F0x8001ebf8(work);
    for (i = 0, dbase = D_80048D34, offA = 0, offB = 0; i < 3; i++) {
        for (n = 0, q = offA; n < 6; n++, q += 4) {
            *(int *)(state + q + 0x74) = 0;
        }
        for (n = 0, t = offA, roff = offB; n < 40; n++) {
            h = *(short *)(roff + (int)dbase + 0x63E);
            roff += 2;
            fn = *(StcrddekCb *)(work + 0x2C);
            fn(h);
            bp = *(unsigned char **)work;
            idx = (*bp - 1) * 4;
            a = idx + t;
            cnt = *(int *)(state + a + 0x74);
            *(int *)(state + a + 0x74) = cnt + 1;
        }
        offA += 0x18;
        offB += 0x66;
    }
}
