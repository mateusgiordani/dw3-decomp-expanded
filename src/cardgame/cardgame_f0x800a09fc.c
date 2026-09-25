// CARDGAME:0x800a09fc (208 bytes, 0xd0)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1dd4c
// Ghidra CARDGAME ddw3-pal-sles-03936 read-only: 52 words match the PAL slice
//   (sha256 1ccc0d92d4b47ff321acdbda87ec2aafee01c5d5c1f5acc6202038f2407c4faa);
//   three nested loops (outer 5, middle 2, inner *pb) calling
//   CARDGAME_F0x800a0844(p, record address, middle, outer); returns whether any
//   record was visited. x-ref to: 3 callers (0x800a21d8, 0x800a2580, 0x800a0c54).
// Revision 6 source shape (GCC 2.8.1 SN32 / ASPSX 2.79 O2, exact):
//   - the record loop is a for loop: GCC copies its test to the entry
//     (beqz on *pb) and the loop optimizer places result = 1 and the stepped
//     record offset (li s1,2) in the pre-header. In the measured dumps inner
//     then has 18 loop-weighted references and the offset is a loop-created
//     register with 15, so inner is allocated first and takes s0;
//   - the record address is written &pb[inner * 14 + 2] so the stepped
//     register holds the offset (addu a1,s2,s1), not a pointer.
#include <stdint.h>

extern void CARDGAME_F0x800a0844(void *p, void *q, int a2, int a3);

int CARDGAME_F0x800a09fc(void *a0)
{
    unsigned char *p = (unsigned char *)a0;
    int result = 0;
    int outer = 0;
    int middle;
    int off;
    unsigned char *pb;
    int inner;

    result = 0;
    outer = 0;
    do {
        middle = 0;
        off = 0x72c;
        do {
            pb = p + off;
            for (inner = 0; inner < *pb; inner++) {
                result = 1;
                CARDGAME_F0x800a0844(p, &pb[inner * 14 + 2], middle, outer);
            }
            middle++;
            off += 0x72;
        } while (middle < 2);
        outer++;
    } while (outer < 5);
    return result;
}
