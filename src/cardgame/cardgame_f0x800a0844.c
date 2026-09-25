// CARDGAME:0x800a0844 (size 440, 0x1b8)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1db94
// Boundary: symbols/function_labels.csv row CARDGAME,0x800a0844 (440 B,
// "PAL prologue + jr ra (sweep)"); prologue addiu sp,-8 at 0x800a0844,
// epilogue jr ra + addiu sp,+8 at 0x800a09f4/0x800a09f8, next prologue at
// 0x800a09fc. First8 LE words match Ghidra disasm bytes word-for-word.
// Ghidra CARDGAME (project ddw3-pal-sles-03936, read-only, no mutation):
// disasm 110 insns, decompile CARDGAME_F0x800a0844, x-ref to (2 callers),
// x-ref from (jump table at 0x80083960, threshold table at 0x800a5c20).
// Shape: guard *(short *)(p1 + p2[0]*2 + 0x50) == TBL_800a5c20[mode] else
// return with no writes; then 2-iteration loop over stack slots computing a
// mode-selected value (jump table 0x80083960, cases 0-4, default skips the
// store), clamping each slot to [0,99] via memory reloads, and copying the
// low halfwords out to p2[3]/p2[4].
// Callers: CARDGAME:0x800a0a60 (in 0x800a09fc; a3 loops 0..4) and
// CARDGAME:0x80091b90 (in 0x800919fc; a3 loops 0..4). Both pass 4 args.
// Strides: t2 = p1 + p3*200 + 0x59c (short fields +6/+10); rec base =
// p1 + p3*114 + 0x72c (byte field); case 3/4 use absolute-disp fields
// +0x72c/+0x79e and +0x5a2/+0x66a off p1 (same 0x72c family as neighbor
// CARDGAME_F0x800a0708: a1*0x72 + 0x72c).
// No knowledge-domain pack: pure integer/short arithmetic on private card
// structs plus two internal tables; no battle/camera/skill/dialogue,
// field/map, sprite/rendering, disc-I/O, or overlay-loader semantics.
#include <stdint.h>

extern int16_t CARDGAME_800a5c20[];

void CARDGAME_F0x800a0844(int32_t p1, int16_t *p2, int32_t p3, int32_t mode)
{
    int32_t vals[2];
    int32_t *dst;
    int32_t index;
    int16_t *w;
    uint8_t *rec;
    int32_t woff;
    int32_t roff;
    int32_t cur;
    int32_t goff;
    int16_t loaded;
    int32_t acc;
    int32_t tmp;

    /* Order matters for matching (cf. CARDGAME_F0x800a0708): the 0x59c/0x72c
       bias is added to p3*stride BEFORE p1 (addiu v0,bias; addu t,p1,v0). */
    woff = p3 * 200 + 0x59c;
    w = (int16_t *)(p1 + woff);
    roff = p3 * 114 + 0x72c;
    rec = (uint8_t *)(p1 + roff);
    goff = p2[0] * 2;
    if (*(int16_t *)(p1 + goff + 0x50) != CARDGAME_800a5c20[mode])
        return;
    index = 0;
    do {
        dst = vals + index;
        if (dst == vals)
            cur = p2[1];
        else
            cur = p2[2];
        switch (mode) {
        case 0:
            *dst = (int32_t)*rec * 20 + cur;
            break;
        case 1:
            loaded = w[5];
            acc = (int32_t)loaded * 4;
            acc += loaded;
            acc += acc;
            goto add10;
        case 2:
            loaded = w[3];
            acc = (int32_t)loaded * 4;
            acc += loaded;
            acc *= 4;
            /* Cauda compartilhada PAL 0x800a0940: tmp=cur+10; acc+=tmp; sw v1
               no delay de j clamp, pulando o sw v0 em 0x800a0998. */
add10:
            tmp = cur + 10;
            acc = acc + tmp;
            *dst = acc;
            break;
        case 3:
            *dst = ((int32_t)*(uint8_t *)(p1 + 0x72c) + (int32_t)*(uint8_t *)(p1 + 0x79e)) * 10 + cur;
            break;
        case 4:
            acc = (int32_t)*(int16_t *)(p1 + 0x5a2) + (int32_t)*(int16_t *)(p1 + 0x66a);
            tmp = acc * 10;
            acc = cur + 10;
            *dst = tmp + acc;
            break;
        default:
            break;
        }
        if (*dst >= 99)
            *dst = 99;
        if (*dst <= 0)
            *dst = 0;
        index++;
    } while ((int32_t)(vals + index) < (int32_t)(vals + 2));
    /* PAL loads the low halfword of each 32-bit slot (lhu 0(sp)/4(sp)). */
    p2[3] = ((uint16_t *)vals)[0];
    p2[4] = ((uint16_t *)vals)[2];
}
