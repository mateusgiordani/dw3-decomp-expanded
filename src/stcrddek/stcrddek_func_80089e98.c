/* STCRDDEK:0x80089e98 (544B; file-off 29160 = vaddr - 0x80082cb0).
 *
 * PAL-first recovery: disassembly-first via
 *   python tools/ghidra.py --project ddw3-pal-sles-03936 --program STCRDDEK disasm 0x80089e98 --instructions 160
 * (read-only; no Ghidra state changed), decompiler as hypothesis only, PAL
 * bytes at file-off 29160 cross-checked against inventory evidence
 * body-80089e98 (sha256 89cf36637a294ba1ac740c7dea86c3c47410f1efa66bfcbb95cfbeefc60b9b89).
 *
 * Shape: dispatcher with frame 0x20 (s0, s1, ra), (menu,ppItem) kept in
 * s0/s1. Outer dispatch is a goto chain reproducing the PAL branch order
 * exactly (beq mode,1 far / slti+bne to def / beq mode,2 far /
 * beq mode,3 far, fallthrough into def): an if/else chain was tried first
 * but GCC 2.8.1 inlines the mode==1 arm (+2w), and a switch lowers as a
 * binary search rooted at 2 (+12B). Modes 0, <0 and >3 share the default
 * path at 0x80089ed8. The flag70 test is inverted (flag70 != 0 first) so
 * the flag==0 arm sits immediately before the shared menu+0x28 call and
 * falls into it, matching PAL (forward order costs an extra j).
 * The step sub-tests keep the observed double-branch shape (beq step,0 to
 * the default arm, beq step,1 to the step==1 arm), hence the explicit
 * case 0 alongside default in the two inner switches -- GCC reproduces the
 * shape including reuse of the live mode register for the tag==2 compare.
 *
 *   case 1: STCRDDEK_func_80089354(menu, ppItem), then tail.
 *   case 2: sub-dispatch on menu+0x70, then menu+0x10 step tag:
 *     flag==0: run menu+0x28(menu,1) only when *ppItem == NULL, then tail.
 *     flag!=0, step!=1: when (*ppItem)+0x10 == 100, run
 *       (*ppItem)+0xEC(item, D_8004935C + idx*102) then (*ppItem)+0xF0(),
 *       step += 1, then tail (tail taken even when kind != 100).
 *     flag!=0, step==1: when (*ppItem)+0x0C == 2 (compared against the
 *       still-live mode value in v1), set it to 3 and run menu+0x28(menu,1),
 *       then tail (tail taken even when tag != 2).
 *   case 3: r1 = *(0x80048D34+0x2714)(); r2 = *(0x80048D34+0x2708)();
 *     *(0x80048D34+0x270C)(r1, r2); returns directly (no tail call). The
 *     shared 0x80048D34 base (one lui+addiu, positive offsets) replaces
 *     three per-pointer lui+lw sequences (-1w).
 *   default: step==1 selects the 0x8008B834 gate; otherwise *0x8008B830()
 *     then step += 1. When the gate returns 0: STCRDDEK_func_80088b34(),
 *     six slots (0xBC/0xEC/0xFC/0x10C/0xCC/0xDC) set to 10, then the
 *     menu+0x38 callback. Default path always returns directly.
 *   tail (cases 1/2 only): STCRDDEK_func_80088e74(menu).
 *
 * The record-table address is referenced symbolically (D_8004935C, same
 * pattern as sibling STCRDDEK_func_80088d24): the assembler then emits the
 * rounded lui 0x8005 + addiu -0x6ca4 late, with the branch delay kept as
 * nop. An inline 0x8004935C literal makes every pinned cc1 synthesize
 * truncated lui 0x8004 + ori hoisted into the branch delay (verified for
 * all six pinned cc1 builds), which cannot match PAL.
 *
 * Caller evidence (inventory call_seeds): STCRDDEK:0x8008A0B8 passes
 * 0x80089E98 in a0 (a1=0x120, a2=0x6C) to 0x80014504, i.e. this function
 * serves as a task/callback argument -- deck-menu candidate; name and both
 * struct shapes stay conservative (semantics unconfirmed).
 *
 * Match (GCC 2.8.1-sn32-4.0.0010 / ASPSX 2.79, -O2 -G0 -mips1
 * -msoft-float, variant base, 8 --symbol resolutions):
 *   python tools/fn_exact_pipeline.py --name STCRDDEK_func_80089e98
 *     --ref upstream/ddw3/asm/dw2003/pro/stcrddek.s
 *     --c decomp/src/overlays/stcrddek/stcrddek_func_80089e98.c
 *     --reference-bin reference/extracted/pro/stcrddek.bin --base 0x80082cb0
 *     --address 0x80089e98 --size 544 --opt-level O2
 *     --cc1-id psyq-gcc-2.8.1-sn32-4.0.0010 --aspsx-id aspsx-2.79
 *     --variant base --symbol STCRDDEK_func_80089354=0x80089354
 *     --symbol STCRDDEK_func_80088b34=0x80088b34
 *     --symbol STCRDDEK_func_80088e74=0x80088e74 --symbol D0x8008B830=0x8008B830
 *     --symbol D0x8008B834=0x8008B834 --symbol D0x80048D34=0x80048D34
 *     --symbol D_8004935C=0x8004935C
 * -> exact_byte_match, candidate sha256 == reference sha256
 *    89cf36637a294ba1ac740c7dea86c3c47410f1efa66bfcbb95cfbeefc60b9b89. */
typedef struct StcrddekMenu StcrddekMenu;
typedef struct StcrddekItem StcrddekItem;

struct StcrddekMenu {
    int unk_00;                      /* +0x00 */
    int unk_04;                      /* +0x04 */
    int unk_08;                      /* +0x08 */
    int mode;                        /* +0x0C */
    int step;                        /* +0x10 */
    int pad_14[5];                   /* +0x14 */
    void (*cb_step)(StcrddekMenu *, int); /* +0x28 */
    int pad_2c[3];                   /* +0x2C */
    void (*cb_done)(StcrddekMenu *);           /* +0x38 (arg registers on entry hold stale menu/ppItem; signature uncertain) */
    int pad_3c[9];                   /* +0x3C */
    int index;                       /* +0x60 */
    int pad_64[3];                   /* +0x64 */
    int flag70;                      /* +0x70 */
    int pad_74[18];                  /* +0x74 */
    int val_bc;                      /* +0xBC */
    int pad_c0[3];                   /* +0xC0 */
    int val_cc;                      /* +0xCC */
    int pad_d0[3];                   /* +0xD0 */
    int val_dc;                      /* +0xDC */
    int pad_e0[3];                   /* +0xE0 */
    int val_ec;                      /* +0xEC */
    int pad_f0[3];                   /* +0xF0 */
    int val_fc;                      /* +0xFC */
    int pad_100[3];                  /* +0x100 */
    int val_10c;                     /* +0x10C */
};

struct StcrddekItem {
    int pad_00[3];                   /* +0x00 */
    int tag;                         /* +0x0C */
    int kind;                        /* +0x10 */
    int pad_14[54];                  /* +0x14..0xEB */
    void (*cb_ec)(StcrddekItem *, void *); /* +0xEC */
    void (*cb_f0)(StcrddekItem *);             /* +0xF0 */
};

extern void STCRDDEK_func_80089354(void *a0, void *a1);
extern void STCRDDEK_func_80088b34(void *a0, void *a1);
extern void STCRDDEK_func_80088e74(void *a0);
extern void (*D0x8008B830)(void);
extern int (*D0x8008B834)(void);
extern unsigned char D0x80048D34[];             /* 0x80048D34 shared EXE-gate base */
extern char D_8004935C[];                        /* EXE-resident record table (sibling 80088d24 precedent) */

void STCRDDEK_func_80089e98(StcrddekMenu *menu, StcrddekItem **ppItem)
{
    int mode = menu->mode;
    int r1;
    int r2;

    /* PAL branch order: beq mode,1 far / slti+bne to def / beq mode,2 far /
     * beq mode,3 far with fallthrough into def; outlined m1/m2/tail/m3. */
    if (mode == 1)
        goto m1;
    if (mode < 2)
        goto defpath;
    if (mode == 2)
        goto m2;
    if (mode == 3)
        goto m3;
defpath:
    switch (menu->step) {
    case 0:
    default:
        D0x8008B830();
        menu->step += 1;
        break;
    case 1:
        if (D0x8008B834() != 0)
            break;
        STCRDDEK_func_80088b34(menu, ppItem);
        menu->val_bc = 10;
        menu->val_ec = 10;
        menu->val_fc = 10;
        menu->val_10c = 10;
        menu->val_cc = 10;
        menu->val_dc = 10;
        menu->cb_done(menu);
        break;
    }
    return;
m1:
    STCRDDEK_func_80089354(menu, ppItem);
    goto tail;
m2:
    if (menu->flag70 != 0) {
        switch (menu->step) {
        case 0:
        default:
            if ((*ppItem)->kind == 100) {
                (*ppItem)->cb_ec(*ppItem, (void *)(D_8004935C + menu->index * 102));
                (*ppItem)->cb_f0(*ppItem);
                menu->step += 1;
            }
            break;
        case 1:
            if ((*ppItem)->tag == 2) {
                (*ppItem)->tag = 3;
                menu->cb_step(menu, 1);
            }
            break;
        }
    } else {
        if (*ppItem == 0)
            menu->cb_step(menu, 1);
    }
    goto tail;
tail:
    STCRDDEK_func_80088e74(menu);
    return;
m3:
    {
        unsigned char *b = D0x80048D34;
        r1 = ((int (*)(void))(*(void **)(b + 0x2714)))();
        r2 = ((int (*)(void))(*(void **)(b + 0x2708)))();
        ((void (*)(int, int))(*(void **)(b + 0x270C)))(r1, r2);
    }
    return;
}