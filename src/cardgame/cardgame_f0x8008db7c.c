// CARDGAME:0x8008db7c (size 244, 0xf4)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xaecc
// Boundary: prev CARDGAME:0x8008daf4 (0x88) ends exactly at 0x8008db7c; next
//   prologue 27bdffe8 (addiu sp,-0x18) at 0x8008db7c+0xf4 = 0x8008dc70.
//   Contiguous, no gap.
// Guard: two separate if-gotos, NOT (v1 == 0 || v1 != 1). The || form
//   collapses to a single beq v1,1 under O2, while PAL keeps the redundant
//   branch-to-next (beq v1,zero->store, ret=0 in its delay slot), an explicit
//   load-delay nop, and the late li v0,1 + beq v1,v0->check (first sll of the
//   p3*76 chain in its delay slot). ret = 0 is initialized before the load.
// Path-A address: fresh dst (addrA = p2 + off, base first) emits
//   addu v0,s1,v0. Reusing the addend (t = p2 + t) or the inline
//   (p2 + p3 * 76) form emits addu v0,v0,s1 instead (cf. lesson 008).
// Path B reuses the loaded guard value as the constant: bne v0,v1, since
//   v1 == 1 on that edge; p1 stays parked in a3 (move a3,a0 at entry).
// Xref: 1 caller CARDGAME_F0x8008dd38 via 0x8008dd78 UNCONDITIONAL_CALL
//   (arity 3 confirmed by that caller's extern decl). 0 direct callees;
//   2 indirect jalr via table slots p2+0xf08 (5 args, a3=-0x5000, 0x6100 at
//   0x10(sp)) and p2+0xf20 (4 args, a3=a2=0x1200). Raw lbu at +0x14a;
//   sh stores at +0x12e/+0x120. Stride 0x4c=76. Offsets stay raw: no
//   CARDGAME slot mapping in the advisory rendering pack.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 base.
// Status: C_MATCHING (exact_byte_match, sha 4f84a12a...).
#include <stdint.h>

typedef void (*cardgame_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_cb4_t)(int32_t, int32_t, int32_t, int32_t);

int32_t CARDGAME_F0x8008db7c(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t ret = 0;
    int32_t v1 = *(int32_t *)(p1 + 0x424);

    if (v1 == 0)
        goto store;
    if (v1 == 1)
        goto check;
store:
    {
        int32_t off = p3 * 76;
        int32_t addrA = p2 + off;

        *(int32_t *)(p1 + 0x424) = 1;
        *(int16_t *)(addrA + 0x12e) = 1;
        ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xf08))(p2, p3, 15, -0x5000, 0x6100);
        ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xf20))(p2, p3, 0x1200, 0x1200);
    }
    goto done;
check:
    {
        int32_t addr = p2 + p3 * 76;

        if (*(uint8_t *)(addr + 0x14a) == 1) {
            *(int16_t *)(addr + 0x12e) = 0;
            *(int16_t *)(addr + 0x120) = 0;
            ret = 1;
        }
    }
done:
    return ret;
}
