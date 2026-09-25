/* STCRDDEK:0x80085210. PAL recovery of all menu states and the shared clock tail.
 * Ghidra structure was checked against every PAL instruction and the 64-slot table;
 * callback arguments absent from the decompiler were restored from a0/a1 setup.
 * r6: every case stores its own next state and continues to the clock tail (case 0x3f falls
 * through with its store), bases are extern symbols, and branch layout/locals follow PAL.
 * r7: exact_byte_match (4964/4964). Quatro formas obrigatorias, medidas em strategy-r7.md:
 * lista de cartas como membro array de struct (ordem base+indice no addu), corpo do caso 0x20
 * com enderecos planos sem locais a/b, callback +0x2c com retorno void (sem sugestao de v0 no
 * local-alloc) e D_80055C48 com retorno (sugestao de v0 empurra x5c para a3). */
#include "common/types.h"
typedef int32_t code();
/* 0x80048D34 deck table view: per-card limits at +0x3A2, 51-slot int16 decks at +0x63E. */
typedef struct {
  uint8_t unk0[0x3A2];
  int8_t limit[0x29C];
  int16_t decks[16][51];
} DeckTable;

/* 0x8008B830: fade/window helper table. */
typedef struct {
    int32_t unk0;
    int32_t unk4;
    void (*start)(int32_t a0, int32_t a1);  /* +0x8 (0x8008B838) */
    int32_t (*done)(int32_t a0);            /* +0xC (0x8008B83C) */
} Stcrddek8008B830;

/* 0x8004B7D0: EXE input helper table. */
typedef struct {
    uint8_t unk0[0x3F4];
    int32_t (*held)(int32_t a0);                /* +0x3F4 (0x8004BBC4) */
    int32_t (*repeat)(int32_t a0);              /* +0x3F8 (0x8004BBC8) */
    int32_t (*pressed)(int32_t a0);             /* +0x3FC (0x8004BBCC) */
    int32_t unk400;
    int32_t unk404;
    uint32_t (*button)(int32_t a0, int32_t a1); /* +0x408 (0x8004BBD8) */
} Exe8004B7D0;

/* 0x80044B38: EXE text/string helper table. */
typedef struct {
    uint8_t unk0[0x414];
    int32_t (*text)(int32_t a0); /* +0x414 (0x80044F4C) */
} Exe80044B38;

extern Stcrddek8008B830 D_8008B830;
extern Exe8004B7D0 D_8004B7D0;
extern Exe80044B38 D_80044B38;
/* Retorno nao-void e obrigatorio: call_value sugere v0 ao alvo no local-alloc (gcc 2.8.1
 * local-alloc.c combine_regs), o que desloca a cadeia x5c*51 do caso 0x18 para v1 e x5c para a3. */
extern int32_t (*D_80055C48)(int32_t a0);
extern int32_t D_8005CCA8;
extern int32_t (*D_8004DF9C)(void); /* EXE frame clock */
extern uint8_t D_80048D34[]; /* deck table */
/* param_1 + 0x88: lista de cartas int16 do registro do menu. Precisa ser membro array real:
 * ARRAY_REF passa por get_inner_reference e gera (plus base indice) -> `addu v0,s2,v0`;
 * a forma ponteiro/inteira gera (plus (mult) base) -> `addu v0,v0,s2`. */
typedef struct {
  uint8_t unk0[0x88];
  int16_t cards[0x100];
} CardList;
extern int32_t STCRDDEK_func_80083934();
extern int32_t STCRDDEK_func_80083d28();
extern int32_t STCRDDEK_func_80084278();
extern int32_t STCRDDEK_func_80084440();
extern int32_t STCRDDEK_func_800847c0();
extern int32_t STCRDDEK_func_80088ac8();
void STCRDDEK_F0x80085210(int param_1,int param_2)
{
  int16_t sVar1;
  int16_t sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int32_t uVar6;
  int iVar8;
  int iVar9;
  int old;
  switch(*(int32_t *)(param_1 + 0x10)) {
  case 0:
  default:
    D_8008B830.start(param_1 + 0x444,1);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 1:
    iVar3 = D_8008B830.done(param_1 + 0x444);
    if (iVar3 == 0) goto LAB_80086520;
    *(int32_t *)(param_1 + 0x440) = 0;
    STCRDDEK_func_80083d28(param_1,param_2,1);
    STCRDDEK_func_800847c0(param_1);
    uVar6 = STCRDDEK_func_80083934(param_1);
    *(int32_t *)(param_2 + 0xcc) = uVar6;
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 2:
    if (*(int *)(*(int *)(param_2 + 0xcc) + 0xc) != 2) goto LAB_80086520;
    *(int32_t *)(param_1 + 0x70) = 1;
    *(int *)(param_1 + 0x10) = 5;
    break;
  case 5: {
    int v;
    old = *(int *)(param_1 + 0x60) + *(int *)(param_1 + 100) * 9;
    if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,7)) & 1U) != 0 || ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0,7)) & 1U) != 0) {
      *(int *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + -1;
      if (*(int *)(param_1 + 0x60) < 0) {
        *(int32_t *)(param_1 + 0x60) = 0;
      }
    }
    else if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,5)) & 1U) != 0 || ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0,5)) & 1U) != 0) {
      if (*(int *)(param_1 + 100) == 4) {
        *(int *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + 1;
        if (3 < *(int *)(param_1 + 0x60)) {
          *(int32_t *)(param_1 + 0x60) = 3;
        }
      }
      else {
        *(int *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + 1;
        if (8 < *(int *)(param_1 + 0x60)) {
          *(int32_t *)(param_1 + 0x60) = 8;
        }
      }
    }
    if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,4)) & 1U) != 0 || ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0,4)) & 1U) != 0) {
      *(int *)(param_1 + 100) = *(int *)(param_1 + 100) + -1;
      if (*(int *)(param_1 + 100) < 0) {
        *(int32_t *)(param_1 + 100) = 0;
      }
    }
    else if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,6)) & 1U) != 0 || ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0,6)) & 1U) != 0) {
      v = *(int *)(param_1 + 100) + 1;
      *(int *)(param_1 + 100) = v;
      if (3 < *(int *)(param_1 + 0x60)) {
        if (3 < v) {
          *(int32_t *)(param_1 + 100) = 3;
        }
      }
      else if (4 < v) {
        *(int32_t *)(param_1 + 100) = 4;
      }
    }
    STCRDDEK_func_80083d28(param_1,param_2,1);
    if (old != *(int *)(param_1 + 0x60) + *(int *)(param_1 + 100) * 9) {
      D_80055C48(0x4001b);
      goto LAB_80086520;
    }
    if (((D_8004B7D0.repeat(0) >> D_8004B7D0.button(0,10)) & 1U) == 0 && ((D_8004B7D0.held(0) >> D_8004B7D0.button(0,0xb)) & 1U) != 0) {
      D_80055C48(0x4001b);
      *(int *)(param_1 + 0x10) = 10;
      break;
    }
    if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,0xd)) & 1U) != 0) {
      if (*(int *)(param_1 + 0x7c) == 0) goto LAB_80086520;
      D_80055C48(0x4001c);
      *(int *)(param_1 + 0x10) = 0x14;
      break;
    }
    if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,0xe)) & 1U) != 0) {
      D_80055C48(0x800450bd);
      *(int *)(param_1 + 0x10) = 0x32;
      break;
    }
    if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,0xc)) & 1U) == 0) goto LAB_80086520;
    D_80055C48(0x800450bd);
    *(int *)(param_1 + 0x10) = 0x1e;
    break;
  }
  case 10:
    *(int32_t *)(param_1 + 0x70) = 0;
    *(int *)(param_1 + 0x43c) = 1 - *(int *)(param_1 + 0x43c);
    if (*(int *)(param_1 + 0x43c) != 0) {
      D_8008B830.start(param_1 + 0x454,1);
      *(int32_t *)(param_1 + 0x10) = 0xb;
      *(int32_t *)(param_1 + 0x14) = 0;
    }
    else {
      STCRDDEK_func_80083d28(param_1,param_2,1);
      D_8008B830.start(param_1 + 0x454,0);
      *(int32_t *)(param_1 + 0x10) = 0xb;
      *(int32_t *)(param_1 + 0x14) = 1;
    }
    goto LAB_80086520;
  case 0xb:
    iVar3 = D_8008B830.done(param_1 + 0x454);
    if (iVar3 != 0) {
      if (*(int *)(param_1 + 0x14) == 0) {
        STCRDDEK_func_80083d28(param_1,param_2,1);
      }
      *(int32_t *)(param_1 + 0x70) = 1;
      /* Retorno void obrigatorio: sem sugestao de v0 para o alvo, o `li 1` (vida mais curta)
       * recebe v0 e o alvo v1, como em PAL 0x80085858. */
      (**(void (**)(int, int))(param_1 + 0x2c))(param_1,5);
    }
    goto LAB_80086520;
  case 0x14:
    *(int32_t *)(*(int *)(param_2 + 0xcc) + 0xc) = 3;
    STCRDDEK_func_800847c0(param_1);
    D_8008B830.start(param_1 + 0x464,1);
    while (((CardList *)param_1)->cards[*(int *)(param_1 + 0x78) + *(int *)(param_1 + 0x74)] == 0) {
      *(int *)(param_1 + 0x78) = *(int *)(param_1 + 0x78) + -1;
      if (*(int *)(param_1 + 0x78) < 0) {
        *(int32_t *)(param_1 + 0x78) = 0;
        *(int *)(param_1 + 0x74) = *(int *)(param_1 + 0x74) + -1;
        if (*(int *)(param_1 + 0x74) < 0) {
          *(int32_t *)(param_1 + 0x74) = 0;
        }
      }
    }
    *(int32_t *)(param_1 + 0x70) = 0;
    D_8008B830.start(param_1 + 0x444,0);
    if (*(int *)(param_1 + 0x43c) != 0) {
      *(int32_t *)(param_1 + 0x45c) = 0;
      *(int32_t *)(param_1 + 0x43c) = 0;
    }
    STCRDDEK_func_80083d28(param_1,param_2,0);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x15:
    iVar3 = D_8008B830.done(param_1 + 0x444);
    if (iVar3 == 0) goto LAB_80086520;
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x16:
    iVar3 = D_8008B830.done(param_1 + 0x464);
    if (iVar3 == 0) goto LAB_80086520;
    STCRDDEK_func_80084278(param_1,param_2,1);
    D_8008B830.start(param_1 + 0x474,1);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x17:
    iVar3 = D_8008B830.done(param_1 + 0x474);
    if (iVar3 == 0) goto LAB_80086520;
    if ((8 < *(int *)(param_1 + 0x7c)) && (*(int *)(param_2 + 0xd0) == 0)) {
      int obj;
      obj = STCRDDEK_func_80088ac8();
      *(int *)(param_2 + 0xd0) = obj;
      (**(code **)(obj + 0x8c))(obj,0x125,0xc);
      (**(code **)(*(int *)(param_2 + 0xd0) + 0x90))(*(int *)(param_2 + 0xd0),0x2a,0x8f);
      (**(code **)(*(int *)(param_2 + 0xd0) + 0x94))
                (*(int *)(param_2 + 0xd0),8,*(int32_t *)(param_1 + 0x7c));
      (**(code **)(*(int *)(param_2 + 0xd0) + 0x98))
                (*(int *)(param_2 + 0xd0),*(int32_t *)(param_1 + 0x74));
    }
    (**(code **)(*(int *)(param_2 + 0xc4) + 0x84))
              (*(int *)(param_2 + 0xc4),0x89,*(int *)(param_1 + 0x74) * 0xe + 0x27);
    (**(code **)(*(int *)(param_2 + 0xc4) + 0x80))(*(int *)(param_2 + 0xc4),1);
    STCRDDEK_func_80084440(param_1,param_2,1);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x18: {
    int v;
    int lim;
    int i;
    int old78;
    int oldsum;
    uint8_t *deck;
    int card;
    int other;
    oldsum = *(int *)(param_1 + 0x78) + *(int *)(param_1 + 0x74);
    old78 = *(int *)(param_1 + 0x78);
    if (8 < *(int *)(param_1 + 0x7c)) {
      if ((((D_8004B7D0.repeat(0) >> D_8004B7D0.button(0,0xb)) & 1U) == 0 && ((D_8004B7D0.held(0) >> D_8004B7D0.button(0,10)) & 1U) != 0) ||
          (((D_8004B7D0.repeat(0) >> D_8004B7D0.button(0,0xb)) & 1U) == 0 && ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0,10)) & 1U) != 0)) {
        v = *(int *)(param_1 + 0x78) + -7;
        *(int *)(param_1 + 0x78) = v;
        if (v < 0) {
          *(int32_t *)(param_1 + 0x78) = 0;
        }
      }
      else if ((((D_8004B7D0.repeat(0) >> D_8004B7D0.button(0,10)) & 1U) == 0 && ((D_8004B7D0.held(0) >> D_8004B7D0.button(0,0xb)) & 1U) != 0) ||
               (((D_8004B7D0.repeat(0) >> D_8004B7D0.button(0,10)) & 1U) == 0 && ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0,0xb)) & 1U) != 0)) {
        for (i = 0; i < 7; i = i + 1) {
          v = *(int *)(param_1 + 0x78) + 1;
          *(int *)(param_1 + 0x78) = v;
          if (*(int *)(param_1 + 0x7c) + -8 < v) {
            *(int *)(param_1 + 0x78) = *(int *)(param_1 + 0x7c) + -8;
            break;
          }
        }
      }
    }
    if (oldsum == *(int *)(param_1 + 0x78) + *(int *)(param_1 + 0x74)) {
      if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,4)) & 1U) != 0 || ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0,4)) & 1U) != 0) {
        v = *(int *)(param_1 + 0x74) + -1;
        *(int *)(param_1 + 0x74) = v;
        if (v < 0) {
          *(int32_t *)(param_1 + 0x74) = 0;
          v = *(int *)(param_1 + 0x78) + -1;
          *(int *)(param_1 + 0x78) = v;
          if (v < 0) {
            *(int32_t *)(param_1 + 0x78) = 0;
          }
        }
      }
      else if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,6)) & 1U) != 0 || ((D_8004B7D0.pressed(0) >> D_8004B7D0.button(0,6)) & 1U) != 0) {
        if (8 < *(int *)(param_1 + 0x7c)) {
          v = *(int *)(param_1 + 0x74) + 1;
          *(int *)(param_1 + 0x74) = v;
          if (7 < v) {
            *(int32_t *)(param_1 + 0x74) = 7;
            v = *(int *)(param_1 + 0x78) + 1;
            *(int *)(param_1 + 0x78) = v;
            lim = *(int *)(param_1 + 0x7c) + -8;
            if (lim < v) {
              *(int *)(param_1 + 0x78) = lim;
            }
          }
        }
        else {
          v = *(int *)(param_1 + 0x74) + 1;
          *(int *)(param_1 + 0x74) = v;
          lim = *(int *)(param_1 + 0x7c) + -1;
          if (lim < v) {
            *(int *)(param_1 + 0x74) = lim;
          }
        }
      }
    }
    if ((*(int *)(param_2 + 0xd0) != 0) && (old78 != *(int *)(param_1 + 0x78))) {
      (**(code **)(*(int *)(param_2 + 0xd0) + 0x98))(*(int *)(param_2 + 0xd0), *(int *)(param_1 + 0x78));
    }
    if (oldsum != *(int *)(param_1 + 0x78) + *(int *)(param_1 + 0x74)) {
      D_80055C48(0x8004513e);
      (**(code **)(*(int *)(param_2 + 0xc4) + 0x84))
                (*(int *)(param_2 + 0xc4),0x89,*(int *)(param_1 + 0x74) * 0xe + 0x27);
      STCRDDEK_func_80084278(param_1,param_2,1);
      STCRDDEK_func_80084440(param_1,param_2,1);
      goto LAB_80086520;
    }
    if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,0xd)) & 1U) != 0) {
      card = ((CardList *)param_1)->cards[*(int *)(param_1 + 0x78) + *(int *)(param_1 + 0x74)];
      deck = D_80048D34;
      other = ((DeckTable *)deck)->decks[*(int *)(param_1 + 0x5c)][*(int *)(param_1 + 0x60) + *(int *)(param_1 + 100) * 9];
      D_80055C48(0x8004503c);
      if ((card != other) &&
         (3 < (int)*(int8_t *)(card + deck + 930) - (int)*(int8_t *)(param_1 + card + 0x2fe))) {
        *(int *)(param_1 + 0x10) = 0x3c;
      }
      else {
        uint8_t *tbl;
        tbl = D_80048D34;
        ((DeckTable *)tbl)->decks[*(int *)(param_1 + 0x5c)][*(int *)(param_1 + 0x60) + *(int *)(param_1 + 100) * 9] =
             card;
        (**(code **)(*(int *)(param_1 + 0x50) + 0x11c))(*(int *)(param_1 + 0x50));
        *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
      }
    }
    else {
      if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,0xe)) & 1U) == 0) goto LAB_80086520;
      D_80055C48(0x800450bd);
      *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    }
    break;
  }
  case 0x19:
    D_8008B830.start(param_1 + 0x464,0);
    STCRDDEK_func_80084278(param_1,param_2,0);
    D_8008B830.start(param_1 + 0x474,0);
    STCRDDEK_func_80084440(param_1,param_2,0);
    (**(code **)(*(int *)(param_2 + 0xc4) + 0x80))(*(int *)(param_2 + 0xc4),0);
    if (*(int *)(param_2 + 0xd0) != 0) {
      *(int32_t *)(*(int *)(param_2 + 0xd0) + 0xc) = 3;
    }
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x1a:
    D_8008B830.done(param_1 + 0x464);
    iVar3 = D_8008B830.done(param_1 + 0x474);
    if (iVar3 != 0) {
      *(int32_t *)(param_1 + 0x74) = 0;
      *(int32_t *)(param_1 + 0x78) = 0;
      *(int32_t *)(param_1 + 0x10) = 0;
    }
    goto LAB_80086520;
  case 0x1e:
    *(int32_t *)(*(int *)(param_2 + 0xcc) + 0xc) = 3;
    STCRDDEK_func_800847c0(param_1);
    *(int32_t *)(param_1 + 0x18) = 0;
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    D_8008B830.start(param_1 + 0x444,0);
    *(int32_t *)(param_1 + 0x70) = 0;
    D_8008B830.start(param_1 + 0x444,0);
    *(int32_t *)(param_1 + 0x45c) = 0;
    *(int32_t *)(param_1 + 0x43c) = 0;
    STCRDDEK_func_80083d28(param_1,param_2,0);
    goto LAB_80086520;
  case 0x1f:
    iVar3 = D_8008B830.done(param_1 + 0x474);
    if (iVar3 == 0) goto LAB_80086520;
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x20: {
    int deck;
    int card;
    int i;
    int j;
    i = 0;
    deck = (int)D_80048D34;
    /* Enderecos planos `x5c*0x66 + n*2 + deck + 1598` sem locais a/b: em EXPAND_SUM o segundo
     * MULT vai para a frente (`j*2 + b`), e sem alvo igual ao operando nao ha troca; CSE
     * compartilha x5c*0x66 antes do primeiro `sh` e recarrega x5c depois, como em PAL. */
    for (; i < 0x27; i = i + 1) {
      for (j = i + 1; j < 0x28; j = j + 1) {
        card = *(int16_t *)(*(int *)(param_1 + 0x5c) * 0x66 + i * 2 + deck + 1598);
        if (*(int16_t *)(*(int *)(param_1 + 0x5c) * 0x66 + j * 2 + deck + 1598) < card) {
          *(uint16_t *)(*(int *)(param_1 + 0x5c) * 0x66 + i * 2 + deck + 1598) =
               *(uint16_t *)(*(int *)(param_1 + 0x5c) * 0x66 + j * 2 + deck + 1598);
          *(int16_t *)(*(int *)(param_1 + 0x5c) * 0x66 + j * 2 + deck + 1598) = card;
        }
      }
    }
    *(int32_t *)(param_1 + 0x10) = 0;
    goto LAB_80086520;
  }
  case 0x32:
    *(int32_t *)(*(int *)(param_2 + 0xcc) + 0xc) = 3;
    *(int32_t *)(param_1 + 0x70) = 0;
    D_8008B830.start(param_1 + 0x444,0);
    if (*(int *)(param_1 + 0x43c) != 0) {
      *(int32_t *)(param_1 + 0x45c) = 0;
      *(int32_t *)(param_1 + 0x43c) = 0;
    }
    STCRDDEK_func_80083d28(param_1,param_2,0);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x33:
    iVar3 = D_8008B830.done(param_1 + 0x444);
    if (iVar3 != 0) {
      *(int32_t *)(param_1 + 0xc) = 3;
    }
    goto LAB_80086520;
  case 0x3c:
    (**(code **)(*(int *)(param_2 + 0xc4) + 0x88))(*(int *)(param_2 + 0xc4),7);
    (**(code **)(*(int *)(param_2 + 0xc4) + 0x94))(*(int *)(param_2 + 0xc4),1);
    D_8008B830.start(param_1 + 0x484,1);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x3d:
    iVar3 = D_8008B830.done(param_1 + 0x484);
    if (iVar3 == 0) goto LAB_80086520;
    uVar6 = D_80044B38.text(D_8005CCA8 + 0x32);
    (**(code **)(*(int *)(param_2 + 200) + 0x114))(*(int *)(param_2 + 200),uVar6,0x1e);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x3e:
        if (((D_8004B7D0.held(0) >> D_8004B7D0.button(0,0xd)) & 1U) == 0) goto LAB_80086520;
    D_80055C48(0x4001c);
    (**(code **)(*(int *)(param_2 + 200) + 0x144))(*(int *)(param_2 + 200),0);
    D_8008B830.start(param_1 + 0x484,0);
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + 1;
    break;
  case 0x3f:
    iVar3 = D_8008B830.done(param_1 + 0x484);
    if (iVar3 == 0) goto LAB_80086520;
    (**(code **)(*(int *)(param_2 + 0xc4) + 0x88))(*(int *)(param_2 + 0xc4),0);
    (**(code **)(*(int *)(param_2 + 0xc4) + 0x94))(*(int *)(param_2 + 0xc4),0);
    *(int *)(param_1 + 0x10) = 0x17;
  }
LAB_80086520:
  *(int *)(param_1 + 0x440) += D_8004DF9C();
  if (0x78 < *(int *)(param_1 + 0x440)) {
    *(int *)(param_1 + 0x440) = *(int *)(param_1 + 0x440) + -0x78;
  }
  return;
}
