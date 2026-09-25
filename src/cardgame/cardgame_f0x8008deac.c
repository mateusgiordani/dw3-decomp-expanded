/* CARDGAME:0x8008deac, 76 PAL bytes. Direct PAL callers target the
 * global callback load eight bytes before the former framed entry.
 * The callback is loaded from 0x8004df9c; no incoming-v0 ABI.
 * Evidence: reports/handoffs/cardgame-8008deb4-c-recovery.md. */
#include "common/types.h"

typedef struct {
    uint8_t pad_0x000[0x424];
    int32_t field_0x424;
} CardGameCtx_8008deac;

typedef int (*CallbackFn)(void);
extern CallbackFn D_8004df9c;

int CARDGAME_F0x8008deac(CardGameCtx_8008deac *ctx, int param_1_unused, int limit) {
    int v0;
    int v1;
    v0 = D_8004df9c();
    v1 = ctx->field_0x424 + v0;
    ctx->field_0x424 = v1;
    return (limit < v1) ? 1 : 0;
}
