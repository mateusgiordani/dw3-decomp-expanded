/*
 * STCRDDEK:0x80087788 STCRDDEK_func_80087788
 * 3424 bytes at STCRDDEK.PRO offset 0x4ad8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087788, jump table (.rodata) at 0x80082e38
 *  Symbols     DAT_8004B7D0=0x8004b7d0 D_80044F4C=0x80044f4c
 *              D_80055C48=0x80055c48 D_8005CCA8=0x8005cca8
 *              D_8008B848=0x8008b848 STCRDDEK_func_800867e8=0x800867e8
 *              STCRDDEK_func_8008687c=0x8008687c
 *              STCRDDEK_func_80086ad8=0x80086ad8
 *  Compare     3424 bytes from 0x80087788 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80087788
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 0x800884e8, PAL-SLES-03936.
 *
 * State values, context offsets and grid accesses follow the PAL instructions.
 *
 * Keep separate cancel/refresh paths: GCC merges them after register
 * allocation.
 */

#include <stdint.h>

extern int32_t STCRDDEK_func_800867e8(int32_t a0, int32_t a1);
extern int32_t STCRDDEK_func_8008687c(int32_t a0);
extern void STCRDDEK_func_80086ad8(int32_t a0, int32_t a1, int32_t a2);


typedef int32_t (*dek_pad0_t)(int32_t);
typedef int32_t (*dek_pad1_t)(int32_t, int32_t);
extern int32_t DAT_8004B7D0[];
extern int32_t D_8008B848[];
extern int32_t D_8005CCA8;
#define DEK_PAD_A() (((dek_pad0_t)DAT_8004B7D0[0xfd])(0))
#define DEK_PAD_B() (((dek_pad0_t)DAT_8004B7D0[0xfe])(0))
#define DEK_PAD_C() (((dek_pad0_t)DAT_8004B7D0[0xff])(0))
#define DEK_PAD_IDX(slot, bit) (((dek_pad1_t)DAT_8004B7D0[0x102])((slot), (bit)))
extern int32_t (*D_80055C48)(int32_t);
extern int32_t (*D_80044F4C)(int32_t);
#define DEK_SOUND(id) (D_80055C48((id)))
#define DEK_TEXT(x) (D_80044F4C((x)))

struct SlotArrayView { int32_t words[30]; uint16_t slots[20]; };

void STCRDDEK_func_80087788(int32_t *w, int32_t *ctx)
{
    int8_t flag;
    int32_t tmp;
    int32_t tab;
    int32_t key;
    int32_t delta;
    int32_t *menu;

    switch ((uint32_t)w[4]) {

case 0:
case 3:
case 4:
case 5:
case 6:
case 7:
case 8:
case 9:
case 12:
case 13:
case 14:
case 15:
case 16:
case 17:
case 18:
case 19:
case 24:
case 25:
case 26:
case 27:
case 28:
case 29:
case 30:
case 31:
case 32:
case 33:
case 34:
case 35:
case 36:
case 37:
case 38:
case 39:
case 40:
case 41:
case 42:
case 43:
case 44:
case 45:
case 46:
case 47:
case 48:
case 49:
case 50:
case 51:
case 52:
case 53:
case 54:
case 55:
case 56:
case 57:
case 58:
case 59:
case 60:
case 61:
case 62:
case 63:
case 64:
case 65:
case 66:
case 67:
case 68:
case 69:
case 70:
case 71:
case 72:
case 73:
case 74:
case 75:
case 76:
case 77:
case 78:
case 79:
case 80:
case 81:
case 82:
case 83:
case 84:
case 85:
case 86:
case 87:
case 88:
case 89:
case 90:
case 91:
case 92:
case 93:
case 94:
case 95:
case 96:
case 97:
case 98:
case 99:
default:
        STCRDDEK_func_800867e8((int32_t)((int8_t *)w + 0xcc), 1);
        w[4] = w[4] + 1;
        return;
case 1:
        if (STCRDDEK_func_8008687c((int32_t)((int8_t *)w + 0xcc)) == 0)
            return;
        STCRDDEK_func_80086ad8((int32_t)w, (int32_t)ctx, 1);
        w[0x2a] = 1;
        w[4] = w[4] + 1;
        return;
case 2:
        if (((DEK_PAD_A() >> DEK_PAD_IDX(0, 3)) & 1) != 0) {
            DEK_SOUND(0x4001b);
            w[0x26] = 0xd;
            w[0x27] = 6;
            return;
        }
        if (D_8008B848[0] > 1) {
            tab = w[0x2b];
            if (((DEK_PAD_B() >> DEK_PAD_IDX(0, 11)) & 1) == 0) {
                if ((DEK_PAD_A() >> DEK_PAD_IDX(0, 10)) & 1) {
                    w[0x2b]--;
                    if (w[0x2b] < 0) w[0x2b] = D_8008B848[0] - 1;
                    goto page_done;
                }
            }
            if (((DEK_PAD_B() >> DEK_PAD_IDX(0, 10)) & 1) == 0) {
                if ((DEK_PAD_A() >> DEK_PAD_IDX(0, 11)) & 1) {
                    w[0x2b]++;
                    if (D_8008B848[0] - 1 < w[0x2b]) w[0x2b] = 0;
                }
            }
page_done:
            if (tab != w[0x2b]) {
                DEK_SOUND(0x4001b);
                STCRDDEK_func_80086ad8((int32_t)w, (int32_t)ctx, 1);
                if (D_8005CCA8 == 0) {
                    int32_t page = w[0x2b];
                    if ((uint32_t)page < 2) {
                        int32_t *grid_base = D_8008B848;
                        int32_t layer = page * 210;
                        int32_t col = w[0x26];
                        int32_t row = w[0x27] * 30;
                        if (*(int8_t *)(layer + grid_base[2] + (col * 2 + row)) != 1) {
                        page_left:
                            w[0x26] = col - 1;
                            if (col - 1 < 0) w[0x26] = 14;
                            col = w[0x26];
                            if (*(int8_t *)(layer + grid_base[2] + (col * 2 + row)) != 1) goto page_left;
                        }
                    } else {
                        int32_t *grid_base = D_8008B848;
                        int32_t layer = page * 210;
                        int32_t wrap = 6;
                        int32_t row = w[0x27];
                        int32_t col = w[0x26] * 2;
                        int32_t cell_address, row_offset;
                        while ((cell_address = layer + grid_base[2], row_offset = row * 30,
                                *(int8_t *)(cell_address + (col + row_offset))) == 0) {
                            w[0x27] = row - 1;
                            if (row - 1 < 0) w[0x27] = wrap;
                            row = w[0x27];
                        }
                    }
                }
            }
        }
        if (((DEK_PAD_A() >> DEK_PAD_IDX(0, 7)) & 1) || ((DEK_PAD_C() >> DEK_PAD_IDX(0, 7)) & 1)) {
        {
            int32_t *grid_base = D_8008B848;
            int32_t layer = w[0x2b];
            int32_t row = w[0x27];
            int32_t col = w[0x26];
            int32_t addr;
            layer *= 0xd2;
            addr = layer + grid_base[2];
            addr += (row * 15 + col) * 2;
            flag = *(int8_t *)addr;
            if (flag < 0) w[0x26] = col + flag;
        }
        do {
            w[0x26]--;
            if (w[0x26] < 0) w[0x26] = 14;
        {
            int32_t layer = w[0x2b];
            int32_t row = w[0x27];
            int32_t col = w[0x26];
            int32_t addr;
            layer *= 0xd2;
            addr = layer + D_8008B848[2];
            addr += (row * 15 + col) * 2;
            flag = *(int8_t *)addr;
        }
        } while (flag != 1);
        DEK_SOUND(0x4001b);
        goto move_y;
        }
        else if (((DEK_PAD_A() >> DEK_PAD_IDX(0, 5)) & 1) || ((DEK_PAD_C() >> DEK_PAD_IDX(0, 5)) & 1)) {
        {
            int32_t *grid_base = D_8008B848;
            int32_t layer = w[0x2b];
            int32_t row = w[0x27];
            int32_t col = w[0x26];
            int32_t addr;
            layer *= 0xd2;
            addr = layer + grid_base[2];
            addr += (row * 15 + col) * 2;
            flag = *(int8_t *)addr;
            if (flag < 0) w[0x26] = col + flag;
        }
        do {
            w[0x26]++;
            if (w[0x26] > 14) w[0x26] = 0;
        {
            int32_t layer = w[0x2b];
            int32_t row = w[0x27];
            int32_t col = w[0x26];
            int32_t addr;
            layer *= 0xd2;
            addr = layer + D_8008B848[2];
            addr += (row * 15 + col) * 2;
            flag = *(int8_t *)addr;
        }
        } while (flag != 1);
        }
        else goto move_y;
        DEK_SOUND(0x4001b);
move_y:
        if (((DEK_PAD_A() >> DEK_PAD_IDX(0, 4)) & 1) || ((DEK_PAD_C() >> DEK_PAD_IDX(0, 4)) & 1)) {
        do {
            w[0x27]--;
            if (w[0x27] < 0) w[0x27] = 6;
        {
            int32_t layer = w[0x2b];
            int32_t row = w[0x27];
            int32_t col = w[0x26];
            int32_t addr;
            layer *= 0xd2;
            addr = layer + D_8008B848[2];
            addr += (row * 15 + col) * 2;
            flag = *(int8_t *)addr;
        }
        } while (flag == 0);
        DEK_SOUND(0x4001b);
        goto confirm;
        }
        else if (((DEK_PAD_A() >> DEK_PAD_IDX(0, 6)) & 1) || ((DEK_PAD_C() >> DEK_PAD_IDX(0, 6)) & 1)) {
        do {
            w[0x27]++;
            if (w[0x27] > 6) w[0x27] = 0;
        {
            int32_t layer = w[0x2b];
            int32_t row = w[0x27];
            int32_t col = w[0x26];
            int32_t addr;
            layer *= 0xd2;
            addr = layer + D_8008B848[2];
            addr += (row * 15 + col) * 2;
            flag = *(int8_t *)addr;
        }
        } while (flag == 0);
        }
        else goto confirm;
        DEK_SOUND(0x4001b);
confirm:
        if (((DEK_PAD_A() >> DEK_PAD_IDX(0, 0xd)) & 1) != 0) {
            delta = 0;
            while (*(int8_t *)(D_8008B848[2] + w[0x2b] * 210 + (w[0x27] * 30 + (w[0x26] + delta) * 2)) != 1) {
                delta--;
            }
            key = w[0x27] * 15 + w[0x26] + delta;
            DEK_SOUND(0x4001c);
            {
                switch (key) {
                case 0x46:
    {
        int32_t pos = w[0x24];
        if (pos <= w[0x25] - 1 && ((struct SlotArrayView *)w)->slots[pos] == 0x4081) {
            w[0x24] = pos - 1;
            if (pos - 1 < 0) w[0x24] = 0;
        }
        {
            int32_t *drv = (int32_t *)ctx[1];
            int32_t *rec = (int32_t *)drv[0x14];
            int32_t *tbl = (int32_t *)rec[4];
            uint32_t half = *(uint16_t *)((int8_t *)tbl + 4);
            ((struct SlotArrayView *)w)->slots[w[0x24]] = (half >> 8) | ((half & 255) << 8);
        }
    }
    {
        int32_t *drv = (int32_t *)ctx[1];
        ((void (*)(int32_t *, int8_t *))*(void **)((int8_t *)drv + 0x110))(drv, (int8_t *)w + 0x78);
        return;
    }

                case 100:
                    w[0x24]--;
                    if (w[0x24] >= 0) return;
                    w[0x24] = 0;
                    return;
                case 0x65:
                    w[0x24]++;
                    if (w[0x24] <= w[0x25] - 1) return;
                    w[0x24] = w[0x25] - 1;
                    return;
                case 0x55:
                {
                    int32_t *drv = (int32_t *)ctx[1];
                    int32_t *rec = (int32_t *)drv[0x14];
                    int32_t *tbl = (int32_t *)rec[4];
                    uint32_t half = *(uint16_t *)((int8_t *)tbl + 4);
                    ((struct SlotArrayView *)w)->slots[w[0x24]] = (half >> 8) | ((half & 255) << 8);
                    w[0x24]++;
                    if (w[0x24] > w[0x25] - 1) w[0x24] = w[0x25] - 1;
    {
        int32_t *drv = (int32_t *)ctx[1];
        ((void (*)(int32_t *, int8_t *))*(void **)((int8_t *)drv + 0x110))(drv, (int8_t *)w + 0x78);
        return;
    }

                }
                case 0x67:
                {
                    struct SlotView { int32_t words[30]; uint16_t slot; };
                    int32_t n = 0;
                    if (w[0x25] > 0) {
                        do {
                            uint32_t value = ((struct SlotArrayView *)w)->slots[n];
                            if (value != 0x4081 && value != 0) {
                                struct SlotView *cursor;
                                n = 19;
                                cursor = (struct SlotView *)((int8_t *)w + 0x26);
                            clear_backedge:
                                    value = cursor->slot;
                                    n--;
                                    if (value != 0x4081) {
                                        w[4] = 100;
                                        return;
                                    }
                                    cursor->slot = 0;
                                    cursor = (struct SlotView *)((int8_t *)cursor - 2);
                                if (n >= 0) goto clear_backedge;
                            }
                            n++;
                        } while (n < w[0x25]);
                    }
                    w[4] = 20;
                    return;
                }
                default:
                {
                    int32_t *grid_slot = D_8008B848 + 2;
                    int32_t layer = w[0x2b];
                    int32_t row = w[0x27];
                    int32_t col;
                    int32_t addr;
                    uint32_t card, half;
                    int32_t *drv, *rec, *tbl;
                    layer *= 0xd2;
                    addr = layer + *grid_slot;
                    col = w[0x26];
                    addr += (row * 15 + col) * 2;
                    card = *(uint8_t *)(addr + 1);
                    drv = (int32_t *)ctx[1];
                    rec = (int32_t *)drv[0x14];
                    tbl = (int32_t *)rec[3];
                    half = *(uint16_t *)((int8_t *)tbl + card * 4);
                    ((struct SlotArrayView *)w)->slots[w[0x24]] = (half >> 8) | ((half & 255) << 8);
                    {
                        int32_t *callback_drv = (int32_t *)ctx[1];
                        ((void (*)(int32_t *, int8_t *))*(void **)((int8_t *)callback_drv + 0x110))(callback_drv, (int8_t *)w + 0x78);
                    }
                    {
                        int32_t pos = w[0x24] + 1;
                        int32_t last = w[0x25] - 1;
                        w[0x24] = pos;
                        if (pos <= last) return;
                        w[0x26] = 0xd;
                        w[0x24] = last;
                        w[0x27] = 6;
                    }
                    return;
                }
                }
            }
        }
        if (((DEK_PAD_A() >> DEK_PAD_IDX(0, 0xe)) & 1) == 0)
            return;
        DEK_SOUND(0x800450bd);
    {
        int32_t pos = w[0x24];
        if (pos <= w[0x25] - 1 && ((struct SlotArrayView *)w)->slots[pos] == 0x4081) {
            w[0x24] = pos - 1;
            if (pos - 1 < 0) w[0x24] = 0;
        }
        {
            int32_t *drv = (int32_t *)ctx[1];
            int32_t *rec = (int32_t *)drv[0x14];
            int32_t *tbl = (int32_t *)rec[4];
            uint32_t half = *(uint16_t *)((int8_t *)tbl + 4);
            ((struct SlotArrayView *)w)->slots[w[0x24]] = (half >> 8) | ((half & 255) << 8);
        }
    }
    {
        int32_t *drv = (int32_t *)ctx[1];
        ((void (*)(int32_t *, int8_t *))*(void **)((int8_t *)drv + 0x110))(drv, (int8_t *)w + 0x78);
        return;
    }
case 10:
        STCRDDEK_func_80086ad8((int32_t)w, (int32_t)ctx, 0);
        STCRDDEK_func_800867e8((int32_t)((int8_t *)w + 0xcc), 0);
        w[0x2a] = 0;
        w[4] = w[4] + 1;
        return;
case 11:
        if (STCRDDEK_func_8008687c((int32_t)((int8_t *)w + 0xcc)) == 0)
            return;
        w[3] = 2;
        return;
case 20:
        w[0x2a] = 0;
        STCRDDEK_func_800867e8((int32_t)((int8_t *)w + 0xdc), 1);
        w[4] = w[4] + 1;
        return;
case 21:
        if (STCRDDEK_func_8008687c((int32_t)((int8_t *)w + 0xdc)) == 0)
            return;
        tmp = DEK_TEXT(D_8005CCA8 + 0x86);
        menu = (int32_t *)ctx[12];
        ((void (*)(int32_t, int32_t, int32_t))*(void **)((int8_t *)menu + 0x114))((int32_t)menu, tmp, 0x12);
        w[4] = w[4] + 1;
        return;
case 22:
        if (((DEK_PAD_A() >> DEK_PAD_IDX(0, 0xd)) & 1) == 0)
            return;
        menu = (int32_t *)ctx[12];
        ((void (*)(int32_t, int32_t))*(void **)((int8_t *)menu + 0x144))((int32_t)menu, 0);
        STCRDDEK_func_800867e8((int32_t)((int8_t *)w + 0xdc), 0);
        w[4] = w[4] + 1;
        return;
case 23:
        if (STCRDDEK_func_8008687c((int32_t)((int8_t *)w + 0xdc)) == 0)
            return;
        w[0x2a] = 1;
        w[4] = 2;
case 100:
        return;
    }
}
