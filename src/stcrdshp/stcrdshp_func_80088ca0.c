#include "common/types.h"

/* STCRDSHP:0x80088ca0 (388 bytes, 0x80088ca0-0x80088e24). PAL-first recovery
   (Ghidra disasm/decompile/xrefs on ddw3-pal-sles-03936 program STCRDSHP,
   base 0x80082cb0; pseudocode hypothesis only).
   Status: exact_byte_match candidate r5 (see submissions/stcrdshp-80088ca0).
   The sentinel-table loop indexes D_8008CB28 by counter so the optimizer
   derives the cursor (late base completion in the entry-branch delay slot);
   the service base is a local (lui/addiu pair, lui in the 0x400-branch
   delay slot). Signature of STCRDSHP_func_80088b50 is not yet recovered
   (address-only use below). Caller STCRDSHP_func_80087bdc stores the
   return value through its second parameter. */
extern void *EXE_F0x80014504(void *, int32_t, int32_t);
extern void STCRDSHP_func_80088afc(void *);
extern void STCRDSHP_func_80088b50(void);

extern uint8_t D_80048D34[];
extern uint8_t D_80044B38[];
extern int32_t (*D_8004B448)(void);
extern int32_t D_8005CCA8;
extern int32_t D_8008CB28[];

void *STCRDSHP_func_80088ca0(void) {
    uint8_t *ctx;
    uint8_t *ex;
    uint8_t *svc;
    int32_t sel;
    int32_t i;

    ctx = (uint8_t *)EXE_F0x80014504((void *)STCRDSHP_func_80088b50, 0x3cc, 0x2c);
    *(void **)(ctx + 0x3c8) = (void *)STCRDSHP_func_80088afc;
    *(int32_t *)(ctx + 0x50) = 0x1000;
    *(int32_t *)(ctx + 0x54) = 7;
    ex = D_80048D34;
    *(int32_t *)(ctx + 0x60) = (*(int32_t (**)(void))(ex + 0x2708))();
    sel = *(int32_t *)(ex + 0x34);
    i = 0;
    while (D_8008CB28[i] != 0) {
        int32_t *row = D_8008CB28 + i;
        if (row[0] == sel)
            *(int32_t *)(ctx + 0x64) = row[1];
        i += 2;
    }
    if (*(int32_t *)(ctx + 0x64) == 0)
        *(int32_t *)(ctx + 0x64) = 0x1f;
    if (D_8004B448() == 0x400)
        *(int32_t *)(ctx + 0x6c) = 2;
    svc = D_80044B38;
    (*(void (**)(int32_t))(svc + 0x40c))(0x7f6);
    (*(void (**)(int32_t))(svc + 0x40c))(0x7f7);
    (*(void (**)(int32_t))(svc + 0x40c))(0x7f8);
    (*(void (**)(int32_t))(svc + 0x40c))(0x7f9);
    (*(void (**)(int32_t))(svc + 0x40c))(0x7fa);
    (*(void (**)(int32_t))(D_80044B38 + 0x40c))(D_8005CCA8 + 0x16);
    (*(void (**)(int32_t))(D_80044B38 + 0x40c))(D_8005CCA8 + 0x1d);
    return ctx;
}
