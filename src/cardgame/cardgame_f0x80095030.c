#include <stdint.h>

/* CARDGAME:0x80095030, 348 PAL bytes at verified base 0x80082cb0
   (file off 0x12380, prologue addiu sp,-0x28, epilogue jr ra + addiu sp,+0x28).
   State machine on byte p1+0x422 (states 1..4, returns 0 except state 4 -> 1);
   state 2 polls two EXE sys bits (slots 0x3f4/0x408) then advances to 3 via
   card-method slot p2+0xee8. Shared EXE base is an extern array so cc1 emits
   lui %hi + addiu %lo; bit tests are nested (sys0(0) >> sys1(0,imm)) & 1 with
   no explicit shift mask (srav masks implicitly), as in sibling
   cardgame_f0x800878b4.c. State 1 stores the loaded dfa byte itself (PAL sb
   uses the loaded reg, not the constant reg).
   Hypothesis 1, toolchain psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0.
   H2: PAL uses slti (signed <3) with lbu loads, so locals are int, not uint8_t.
   H3: PAL dispatches beq-forward to the state-2 body (chain inline), i.e.
   the source tests st != 2 with the chain as the then-block.
   H4: every sub-test is an inverted (!=) guard with early return: PAL shows
   beq-forward + j-epilogue for ==1, ==3 and ==4 alike, which is exactly
   `if (x != c) return ret;` (false falls out-of-line). This also accounts
   for all 7 missing instructions (4 in the ==1 guard block, 1 in the ==3
   test shape, 2 in the advance j-epilogue).
   H5: H4 folded ret away (adjacent `ret = 1; return` constant-folds, then every
   other `return ret` reads constant 0). PAL keeps ret in s4 with state4
   FALLING through (li s4,1 then move, no jump), so the arm must assign ret
   and fall out of the chain to a single trailing `return ret`; the constant
   cannot cross the out-of-line state-2 join, keeping ret a variable.
   H6: no-else guards lay the body out inline (fallthrough), but PAL puts the
   ==1 and ==4 bodies out-of-line behind a forward branch. Tested explicit
   `else { body }`: byte-identical output (jump threading collapses it), so
   the outlining must come from another lever; else reverted.
   H7: register density (refs/range, s0 picked first) explains everything:
   nine `return ret` statements give ret 11 refs (3rd pick, s2), while PAL
   keeps ret in s4 (last pick, lowest density). The original must funnel all
   exits through goto to ONE shared `return ret` (3 refs: init, set, read),
   which generates identical jumps but starves ret's density. The scheduler
   later sinks that single move into predecessor delay slots, which is why
   PAL shows `move v0,s4` in several places.
   H7b: the ==1 guard still lays out bne/inline-body, but PAL is beq/j with
   the body out-of-line. Forward-goto-to-body collapses byte-identical.
   H7c: branch-prediction lever (`if (==) {body} goto`): also byte-identical;
   jump threading normalizes every nested/guard shape to the same RTL.
   H8: the layout is a FLAT goto dispatch, not nested if/else. PAL shows
   every test as beq-forward (==2, ==1, ==3, ==4) with slti+beq for >=3
   (inversion is free in the branch opcode), fallthrough `goto L_ret`
   where PAL has j-epilogue, and bodies as trailing labels in source order
   B1, S2, B3, B4, tail -- exactly PAL block order with no reorder needed.
   Nested forms can never place all bodies after all tests; flat gotos do. */

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
typedef void (*cardgame_m1_t)(int32_t);

extern int32_t DAT_8004B7D0[];

int32_t CARDGAME_F0x80095030(int32_t p1, int32_t p2)
{
    int32_t ret = 0;
    int32_t st;
    int32_t v;

    st = *(uint8_t *)(p1 + 0x422);
    if (st == 2)
        goto L_s2;
    if (st >= 3)
        goto L_ge3;
    if (st == 1)
        goto L_b1;
    goto L_ret;
L_ge3:
    if (st == 3)
        goto L_b3;
    if (st == 4)
        goto L_b4;
    goto L_ret;
L_b1:
    v = *(uint8_t *)(p2 + 0xdfa);
    if (v != 2)
        goto L_ret;
    *(uint8_t *)(p1 + 0x422) = v;
    goto L_ret;
L_s2:
    if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >>
          ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) != 0)
        goto L_adv;
    if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >>
          ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 14)) & 1) == 0)
        goto L_ret;
L_adv:
    *(uint8_t *)(p1 + 0x422) = 3;
    ((cardgame_m1_t)*(uint32_t *)(p2 + 0xee8))(p2);
    goto L_ret;
L_b3:
    if (*(uint8_t *)(p2 + 0xdfa) != 0)
        goto L_ret;
    *(uint8_t *)(p1 + 0x422) = 4;
    goto L_ret;
L_b4:
    ret = 1;
L_ret:
    return ret;
}
