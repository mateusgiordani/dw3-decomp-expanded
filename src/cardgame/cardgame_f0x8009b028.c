// CARDGAME:0x8009b028, 320-byte PAL body at verified base 0x80082cb0.
// The spacing temporary with o2-g0-no-strength-reduce keeps k*8+3 grouped
// before the coordinate load. Exact evidence and callback layouts are in the handoff.

typedef struct {
    void (*fn)();
    int unk;
} CardWorkSlot;

typedef struct {
    unsigned char data[116];
    CardWorkSlot slot[5];
    unsigned char tail[4];
} CardWork;

typedef struct {
    int w0;         /* +0x00 */
    int w1;         /* +0x04 */
    char pad08[16]; /* +0x08..0x17 */
    int w6;         /* +0x18 */
    char pad1c[34]; /* +0x1c..0x3d */
    char b3e[3];    /* +0x3e..0x40 */
    char pad41[4];  /* +0x41..0x44 */
    char b45;       /* +0x45 */
} CardCtx;

typedef struct {
    char pad[0x424];    /* +0x000..0x423 (contents unknown) */
    int (*cb)(int arg); /* +0x424 */
} ExeCbBlock;

extern void F0x8001f648(void *work);
extern ExeCbBlock EXE_80044b38;

void CARDGAME_F0x8009b028(int a0, CardCtx *s)
{
    CardWork w;
    int i;
    int k;
    int resource;
    int spacing;

    if (s->b45 != 0) {
        i = 0;
        k = 0;
        for (; i < 3; i++) {
            if (s->b3e[i] != 0) {
                F0x8001f648(&w);
                if (s->w6 != 0x10001000) {
                    w.slot[4].fn((s->w0 >> 8) + 0x14, (s->w1 >> 8) + 0x17);
                    w.slot[3].fn((short)s->w6, *(short *)((char *)s + 0x1a), 0x1000);
                }
                w.slot[1].fn(0x100, 1);
                w.slot[0].fn(0x280, 0);
                resource = EXE_80044b38.cb(0x25d0002);
                spacing = k * 8 + 3;
                w.slot[2].fn(resource, i + 0x37,
                             (s->w0 >> 8) + spacing, (s->w1 >> 8) + 0x15);
                k++;
            }
        }
    }
}
