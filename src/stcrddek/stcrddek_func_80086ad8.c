/*
 * STCRDDEK:0x80086ad8 STCRDDEK_func_80086ad8
 * 812 bytes at STCRDDEK.PRO offset 0x3e28 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80086ad8
 *  Symbols     STCRDDEK_func_80086ad8=0x80086ad8
 *              stcrddek_datum_8008B848=0x8008b848
 *              stcrddek_tbl_80044B38=0x80044b38
 *              stcrddek_word_8005CCA8=0x8005cca8
 *  Compare     812 bytes from 0x80086ad8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80086ad8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * In-program direct callers: 0x8008780c, 0x80087a0c, 0x8008838c
 * (UNCONDITIONAL_CALL).
 *
 * Semantics unconfirmed (deck-menu candidate): flag != 0 drives the
 * +0x114/+0x110/ +0x138 vectors with EXE probe results; flag == 0 drives the
 * +0x144 vector with arg 0. Object layouts beyond the used slots are unknown
 * (conservative pads).
 *
 * Layout note: the nonzero path is the fall-through after the prologue (PAL beq
 * to the zero path at the end), so it comes first here.
 *
 * Build: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0, pinned variant
 * o2-g0-no-strength-reduce. The loop keeps q and reads q[2]: with strength
 * reduction cc1 shifts the induction variable to objs+8 and loads at offset 0,
 * while PAL keeps s0 = objs and loads at offset 8.
 *
 * The datum base is an int because the C front end rewrites int + pointer into
 * pointer + int, which ties the sum to the base register; with an integer base
 * the scaled index stays first and the sum keeps the chain register, as PAL
 * does.
 */

typedef struct stcrddek_obj {
    unsigned char pad[0x110];
    void (*fn110)(struct stcrddek_obj *, void *);
    void (*fn114)(struct stcrddek_obj *, int, int);
    unsigned char pad2[0x20];
    void (*fn138)(struct stcrddek_obj *, int);
    unsigned char pad3[8];
    void (*fn144)(struct stcrddek_obj *, int);
} stcrddek_obj_t;

typedef struct {
    unsigned char pad[0x78];
    /* Address passed to fn110 (addiu, not a load); target type unknown. */
    unsigned char unk78;
    unsigned char pad2[0x33];
    int idx_ac;
} stcrddek_arg0_t;

typedef struct {
    unsigned char pad[0x414];
    int (*fn)(int);
} stcrddek_tbl_80044B38_t;

typedef struct {
    int count;
    /* Base of 3-word groups held as an address value; byte arithmetic below. */
    int items;
} stcrddek_datum_8008B848_t;

/* EXE-resident objects (resolved via pipeline --symbol, not committed): */
extern stcrddek_tbl_80044B38_t stcrddek_tbl_80044B38;
extern int stcrddek_word_8005CCA8;
/* STCRDDEK-resident word pair (resolved via pipeline --symbol, not committed): */
extern stcrddek_datum_8008B848_t stcrddek_datum_8008B848;

void STCRDDEK_func_80086ad8(stcrddek_arg0_t *arg0, stcrddek_obj_t **objs, int flag)
{
    int i;
    int v;
    int *cell;
    stcrddek_datum_8008B848_t *datum;
    stcrddek_obj_t **q;

    if (flag != 0) {
        v = stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x86);
        objs[0]->fn114(objs[0], v, 1);
        objs[1]->fn110(objs[1], &arg0->unk78);
        i = 0;
        objs[1]->fn138(objs[1], 1);
        datum = &stcrddek_datum_8008B848;
        q = objs;
        do {
            v = stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x86);
            cell = (int *)(arg0->idx_ac * 12 + datum->items + i * 4);
            q[2]->fn114(q[2], v, *cell);
            i++;
            q[2]->fn138(q[2], 1);
            q++;
        } while (i < 3);
        v = stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x86);
        objs[8]->fn114(objs[8], v, 0xd);
        objs[8]->fn138(objs[8], 1);
        v = stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x86);
        objs[9]->fn114(objs[9], v, 0xe);
        objs[9]->fn138(objs[9], 1);
        if (stcrddek_datum_8008B848.count < 2)
            return;
        v = stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x86);
        objs[10]->fn114(objs[10], v, 0x10);
        objs[10]->fn138(objs[10], 1);
        v = stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x86);
        objs[11]->fn114(objs[11], v, 0x11);
        objs[11]->fn138(objs[11], 1);
        return;
    } else {
        objs[0]->fn144(objs[0], 0);
        i = 0;
        objs[1]->fn144(objs[1], 0);
        q = objs;
        do {
            q[2]->fn144(q[2], 0);
            q++;
            i++;
        } while (i < 3);
        objs[8]->fn144(objs[8], 0);
        objs[9]->fn144(objs[9], 0);
        objs[10]->fn144(objs[10], 0);
        objs[11]->fn144(objs[11], 0);
    }
}
