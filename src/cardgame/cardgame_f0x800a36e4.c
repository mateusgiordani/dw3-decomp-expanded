/*
 * CARDGAME:0x800a36e4 CARDGAME_F0x800a36e4
 * 592 bytes at CARDGAME.PRO offset 0x20a34 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a36e4, jump table (.rodata) at 0x80083b04
 *  Symbols     CARDGAME_F0x8008ec44=0x8008ec44 CARDGAME_F0x8008eec8=0x8008eec8
 *              EXE_F0x8001ebf8=0x8001ebf8
 *  Compare     592 bytes from 0x800a36e4 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a36e4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x80 (s0-s2, ra).
 *
 * Checks whether the effect `kind` (0x70..0x96, 39-entry jump table at
 * 0x80083b04) can be used now. Most kinds ask CARDGAME_F0x8008ec44 or
 * CARDGAME_F0x8008eec8 with fixed arguments; 0x83 needs an active slot and 0x84
 * needs the active slot's card record kind to be 6. Returns 1 if usable.
 *
 * Matching note: every case sets ret itself and breaks to the one return; jump2
 * then cross-jumps the identical `ret = 1` tails into one check, which keeps
 * the CARDGAME_F0x8008ec44 calls apart as in PAL.
 */

#include <stdint.h>

struct Lookup {
    uint8_t *rec;
    int32_t pad0[10];
    void (*fn)(int32_t);
    int32_t pad1[10];
};

int32_t CARDGAME_F0x8008ec44(int32_t p1, int32_t p2, int32_t a, int32_t b, int32_t c);
int32_t CARDGAME_F0x8008eec8(int32_t p1, int32_t p2, int32_t a, int32_t b);
void EXE_F0x8001ebf8(struct Lookup *out);

#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x800a36e4(int32_t p1, int32_t p2, int32_t kind)
{
    struct Lookup info;
    int32_t ret;

    ret = 0;
    switch (kind) {
    default:
        return ret;
    case 0x70:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7a:
    case 0x7b:
    case 0x7c:
    case 0x7d:
    case 0x7e:
        ret = 1;
        break;
    case 0x80:
    case 0x81:
        if (CARDGAME_F0x8008ec44(p1, p2, 1, 4, 0xff) != 0)
            ret = 1;
        break;
    case 0x83:
        if (S8(p1, 0x575) != 0)
            ret = 1;
        break;
    case 0x84:
        if (S8(p1, 0x575) != 0) {
            int32_t k = S8(p1, 0x575) - 1;
            int32_t o = k * 8;
            int32_t card = S16(p1 + o, 0x580);
            int32_t c;

            EXE_F0x8001ebf8(&info);
            c = card * 2;
            info.fn(S16(p1 + c, 0x50) + 1);
            if (info.rec[0] != 6)
                return ret;
            ret = 1;
        }
        return ret;
    case 0x72:
        if (CARDGAME_F0x8008ec44(p1, p2, 0, 3, 0xff) != 0)
            ret = 1;
        break;
    case 0x82:
        if (CARDGAME_F0x8008ec44(p1, p2, 1, 3, 0xff) != 0)
            ret = 1;
        break;
    case 0x71:
        if (CARDGAME_F0x8008ec44(p1, p2, 1, 2, 0xff) != 0)
            ret = 1;
        break;
    case 0x7f:
        if (CARDGAME_F0x8008ec44(p1, p2, 0, 2, 0xff) != 0)
            ret = 1;
        break;
    case 0x96:
        if (CARDGAME_F0x8008ec44(p1, p2, 1, 2, 0xfd) != 0)
            ret = 1;
        break;
    case 0x85:
    case 0x8e:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x1fc) != 0)
            ret = 1;
        break;
    case 0x87:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x180) != 0)
            ret = 1;
        break;
    case 0x88:
    case 0x8f:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x2fc) != 0)
            ret = 1;
        break;
    case 0x89:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x2bc) != 0)
            ret = 1;
        break;
    case 0x8a:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x280) != 0)
            ret = 1;
        break;
    case 0x8b:
    case 0x90:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x3fc) != 0)
            ret = 1;
        break;
    case 0x8c:
    case 0x95:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x380) != 0)
            ret = 1;
        break;
    case 0x91:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x3f8) != 0)
            ret = 1;
        break;
    case 0x92:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x3f4) != 0)
            ret = 1;
        break;
    case 0x93:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x310) != 0)
            ret = 1;
        break;
    case 0x94:
        if (CARDGAME_F0x8008eec8(p1, p2, 1, 0x3dc) != 0)
            ret = 1;
        break;
    }
    return ret;
}
