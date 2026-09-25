/*
 * CARDGAME:0x80084320 CARDGAME_F0x80084320
 * 6856 bytes at CARDGAME.PRO offset 0x1670 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80084320, jump table (.rodata) at 0x80082cf8
 *  Symbols     CARDGAME_F0x80084188=0x80084188 CARDGAME_F0x8008423c=0x8008423c
 *              CARDGAME_F0x80084320=0x80084320 CARDGAME_F0x80086330=0x80086330
 *              CARDGAME_F0x8008642c=0x8008642c CARDGAME_F0x80086d78=0x80086d78
 *              CARDGAME_F0x80086eb0=0x80086eb0 CARDGAME_F0x80087edc=0x80087edc
 *              CARDGAME_F0x80088f0c=0x80088f0c CARDGAME_F0x80088f78=0x80088f78
 *              CARDGAME_F0x800894e4=0x800894e4 CARDGAME_F0x80089580=0x80089580
 *              CARDGAME_F0x80089974=0x80089974 CARDGAME_F0x8008a068=0x8008a068
 *              CARDGAME_F0x8008ad98=0x8008ad98 CARDGAME_F0x8008baa4=0x8008baa4
 *              CARDGAME_F0x8008bc6c=0x8008bc6c CARDGAME_F0x8008c044=0x8008c044
 *              CARDGAME_F0x8008c5f0=0x8008c5f0 CARDGAME_F0x8008c6e0=0x8008c6e0
 *              CARDGAME_F0x8008c988=0x8008c988 CARDGAME_F0x8008ca4c=0x8008ca4c
 *              CARDGAME_F0x8008cb5c=0x8008cb5c CARDGAME_F0x8008ce0c=0x8008ce0c
 *              CARDGAME_F0x8008cfdc=0x8008cfdc CARDGAME_F0x8008d274=0x8008d274
 *              CARDGAME_F0x8008d594=0x8008d594 CARDGAME_F0x8008ddc0=0x8008ddc0
 *              CARDGAME_F0x8008def8=0x8008def8 CARDGAME_F0x8008e398=0x8008e398
 *              CARDGAME_F0x8008e5bc=0x8008e5bc CARDGAME_F0x8008ec44=0x8008ec44
 *              CARDGAME_F0x8008eec8=0x8008eec8 CARDGAME_F0x8008f0f4=0x8008f0f4
 *              CARDGAME_F0x8008f530=0x8008f530 CARDGAME_F0x8008f560=0x8008f560
 *              CARDGAME_F0x8008f750=0x8008f750 CARDGAME_F0x8008f790=0x8008f790
 *              CARDGAME_F0x8008f978=0x8008f978 CARDGAME_F0x8008fa9c=0x8008fa9c
 *              CARDGAME_F0x8008ff08=0x8008ff08 CARDGAME_F0x80090058=0x80090058
 *              CARDGAME_F0x800907ac=0x800907ac CARDGAME_F0x80090ba0=0x80090ba0
 *              CARDGAME_F0x80090cd8=0x80090cd8 CARDGAME_F0x800915d4=0x800915d4
 *              CARDGAME_F0x80091858=0x80091858 CARDGAME_F0x800919fc=0x800919fc
 *              CARDGAME_F0x80091c6c=0x80091c6c CARDGAME_F0x80091eb0=0x80091eb0
 *              CARDGAME_F0x80092198=0x80092198 CARDGAME_F0x80092438=0x80092438
 *              CARDGAME_F0x80092638=0x80092638 CARDGAME_F0x80092ba4=0x80092ba4
 *              CARDGAME_F0x80092e60=0x80092e60 CARDGAME_F0x80093710=0x80093710
 *              CARDGAME_F0x80093b10=0x80093b10 CARDGAME_F0x80093c18=0x80093c18
 *              CARDGAME_F0x80093cac=0x80093cac CARDGAME_F0x8009403c=0x8009403c
 *              CARDGAME_F0x800942dc=0x800942dc CARDGAME_F0x80094848=0x80094848
 *              CARDGAME_F0x80094b9c=0x80094b9c CARDGAME_F0x80094fdc=0x80094fdc
 *              CARDGAME_F0x80095030=0x80095030 CARDGAME_F0x8009518c=0x8009518c
 *              CARDGAME_F0x80095274=0x80095274 CARDGAME_F0x8009535c=0x8009535c
 *              FUN_80083f48=0x80083f48 FUN_80083f84=0x80083f84
 *              FUN_80083fc8=0x80083fc8 FUN_8008400c=0x8008400c
 *              FUN_8008404c=0x8008404c FUN_80084090=0x80084090
 *              FUN_800840d0=0x800840d0 FUN_800840f0=0x800840f0
 *              FUN_8008985c=0x8008985c FUN_80089ec8=0x80089ec8
 *              FUN_8008ad48=0x8008ad48 FUN_8008c8d8=0x8008c8d8
 *              FUN_8008deac=0x8008deac FUN_8008e08c=0x8008e08c
 *              FUN_8008e5b0=0x8008e5b0 FUN_8008f0b4=0x8008f0b4
 *              FUN_8008f2d8=0x8008f2d8 FUN_8008f948=0x8008f948
 *              FUN_8009076c=0x8009076c FUN_80090a6c=0x80090a6c
 *              FUN_80090a90=0x80090a90 FUN_800915c4=0x800915c4
 *              FUN_80093624=0x80093624 FUN_80093adc=0x80093adc
 *              FUN_80094828=0x80094828 FUN_80094b78=0x80094b78
 *              FUN_80095208=0x80095208 FUN_800952f4=0x800952f4
 *              func_0x8001ebf8=0x8001ebf8
 *  Compare     6856 bytes from 0x80084320 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80084320
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Source lifetimes and the count access view preserve native PsyQ allocation.
 */

#include <stdint.h>

typedef uint8_t undefined1;
typedef uint16_t undefined2;
typedef uint32_t undefined4;
typedef uint8_t byte;
typedef uint16_t ushort;
typedef uint32_t uint;
typedef void code();

/* PAL count at +0x5a6, signed halfword, with a 200-byte row stride.
 * The zero-length tail specifies an access view, not an inferred capacity. */
typedef struct {
  int16_t count;
  uint8_t unknown_after_count[198];
} CardCountRow;
typedef struct {
  uint8_t unknown_before_count[0x5a6];
  CardCountRow rows[0];
} CardCountView;


void CARDGAME_F0x80084320(int param_1,undefined4 param_2)

{
  uint cVar1;
  undefined1 dispatched;
  uint uVar2;
  uint cardAddress;
  undefined2 uVar3;
  ushort uVar4;
  int iVar5;
  undefined2 uVar6;
  int iVar7;
  int iVar8;
  int selectedOffset;
  int iLoop;
  undefined4 uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint8_t bDispatch;
  uintptr_t local_70[21];

  /* Preserve the original selector while forming the preceding row first. */
  int selected = *(int8_t *)(param_1 + 0x575);
  int sSel = selected;
  uint uAddr1;
  uint uAddr2;
  sSel -= 1;
  uAddr1 = param_1 + sSel * 8;
  sSel = selected;
  uAddr2 = param_1 + sSel * 8;
  uVar11 = (uint)*(byte *)(uAddr1 + 0x584);
  bDispatch = *(uint8_t *)(param_1 + 0x421);
  cVar1 = *(uint8_t *)(uAddr2 + 0x584);
  uVar12 = uVar11 ^ 1;
  if (bDispatch == 0) goto LAB_80084ec8;
  switch (*(uint8_t *)(param_1 + 0x421)) {
  case 1:
    *(undefined4 *)(param_1 + 0x424) = 0x2d;
    break;
  case 18:
    CARDGAME_F0x80092438(param_1,param_2,0);
    break;
  case 19:
    CARDGAME_F0x80092438(param_1,param_2,1);
    break;
  case 20:
    CARDGAME_F0x80092ba4(param_1,param_2);
    break;
  case 27:
    CARDGAME_F0x80093c18(param_1,param_2);
    break;
  case 21:
    FUN_80093624(param_1,param_2,0);
    break;
  case 22:
    FUN_80093624(param_1,param_2,1);
    break;
  case 23:
    FUN_80093adc(param_1,param_2,0);
    break;
  case 24:
    FUN_80093adc(param_1,param_2,1);
    break;
  case 25:
    CARDGAME_F0x8009403c(param_1,param_2,0);
    break;
  case 26:
    CARDGAME_F0x8009403c(param_1,param_2,1);
    break;
  case 28:
  case 29:
    FUN_80094b78(param_1,param_2);
    break;
  case 30:
  case 31:
  case 32:
  case 34:
  case 35:
    FUN_8008f2d8(param_1,param_2,1,0x1e);
    break;
  case 33:
    FUN_8008f2d8(param_1,param_2,2,0x1e);
    break;
  case 36:
  case 37:
  case 38:
  case 39:
  case 40:
    FUN_8008f2d8(param_1,param_2,0xfffffffe,0x1e);
    break;
  case 41:
    CARDGAME_F0x8008f530(param_1,param_2);
    break;
  case 47:
    CARDGAME_F0x80094fdc(param_1,param_2,0x39);
    break;
  case 48:
    CARDGAME_F0x80094fdc(param_1,param_2,0x35);
    break;
  case 49:
    if (uVar11 == 0) {
      CARDGAME_F0x80094fdc(param_1,param_2,0x3b);
    }
    break;
  case 43:
    CARDGAME_F0x8009518c(param_1,param_2,0);
    break;
  case 45:
    CARDGAME_F0x8009518c(param_1,param_2,1);
    break;
  case 44:
    CARDGAME_F0x80095274(param_1,param_2,0);
    break;
  case 46:
    CARDGAME_F0x80095274(param_1,param_2,1);
    break;
  case 62:
    CARDGAME_F0x8008f978(param_1,param_2,uVar12,0);
    break;
  case 63:
    CARDGAME_F0x8008f978(param_1,param_2,uVar11,0);
    break;
  case 64:
    CARDGAME_F0x8008f978(param_1,param_2,uVar12,1);
    break;
  case 65:
    CARDGAME_F0x8008f750(param_1,param_2);
    break;
  case 66:
    CARDGAME_F0x8008ff08(param_1,param_2,uVar11,3);
    break;
  case 67:
    CARDGAME_F0x8008ff08(param_1,param_2,uVar11,4);
    break;
  case 68:
    FUN_8009076c(param_1,param_2,uVar11);
    break;
  case 69:
    FUN_80090a6c(param_1,param_2,2);
    break;
  case 70:
    {
      int iVar8;
      int32_t k = uVar11 * 200;
      iVar8 = 3;
      iVar8 -= *(short *)(param_1 + k + 0x5a6);
      if (iVar8 < 1) {
        iVar8 = 0;
      }
      FUN_80090a6c(param_1,param_2,iVar8);
      break;
    }
  case 71:
    FUN_80090a6c(param_1,param_2,6);
    break;
  case 72:
    CARDGAME_F0x80090ba0(param_1,param_2,uVar11);
    break;
  case 73:
    FUN_8008f0b4(param_1,param_2,uVar11);
    break;
  case 76:
    FUN_8008e5b0(param_1,param_2);
    break;
  case 77:
  case 78:
    FUN_800915c4(param_1,param_2);
    break;
  case 93:
    *(undefined1 *)(param_1 + 0x422) = 1;
    break;
  case 90:
    FUN_80094828(param_1,param_2);
    break;
  case 79:
    CARDGAME_F0x80091858(param_1,param_2,uVar11,0x50);
    break;
  case 80:
    CARDGAME_F0x80091858(param_1,param_2,uVar11,0x51);
    break;
  case 81:
    CARDGAME_F0x80091858(param_1,param_2,uVar11,0x52);
    break;
  case 82:
    CARDGAME_F0x80091858(param_1,param_2,uVar11,0x53);
    break;
  case 83:
    CARDGAME_F0x80091858(param_1,param_2,uVar11,0x54);
    break;
  case 84:
    CARDGAME_F0x80091858(param_1,param_2,uVar11,0x55);
    break;
  case 85:
    CARDGAME_F0x80091858(param_1,param_2,uVar11,0x56);
    break;
  case 86:
    CARDGAME_F0x800919fc(param_1,param_2,uVar11);
    break;
  case 87:
    CARDGAME_F0x80091eb0(param_1,param_2,uVar11);
    break;
  case 88:
    CARDGAME_F0x8008ddc0(param_1,param_2,0);
    break;
  case 89:
    CARDGAME_F0x8008ddc0(param_1,param_2,1);
    break;
  case 94:
    CARDGAME_F0x8008def8(param_1,param_2,0xa000a,1);
    break;
  case 95:
    CARDGAME_F0x8008def8(param_1,param_2,0x1e,1);
    break;
  case 96:
    CARDGAME_F0x8008def8(param_1,param_2,0x320032,1);
    break;
  case 97:
    CARDGAME_F0x8008def8(param_1,param_2,0x140014,1);
    break;
  case 98:
    CARDGAME_F0x8008def8(param_1,param_2,0x1e001e,1);
    break;
  case 99:
    CARDGAME_F0x8008def8(param_1,param_2,10,1);
    break;
  case 100:
    CARDGAME_F0x8008def8(param_1,param_2,0xa0000,1);
    break;
  case 101:
    CARDGAME_F0x8008def8(param_1,param_2,0xffc4,0);
    break;
  case 102:
    CARDGAME_F0x8008def8(param_1,param_2,0xfff1,0);
    break;
  case 103:
    CARDGAME_F0x8008def8(param_1,param_2,0xffe2,0);
    break;
  case 104:
    CARDGAME_F0x8008def8(param_1,param_2,0xafff6,0);
    break;
  case 105:
    CARDGAME_F0x8008def8(param_1,param_2,0xff9d0000,2);
    break;
  {
    undefined1 dispatched;
  case 106:
    dispatched = 1;
    goto LAB_8008498c;
  case 107:
    dispatched = 2;
    goto LAB_8008498c;
  case 108:
    dispatched = 3;
    goto LAB_8008498c;
  case 109:
    dispatched = 4;
    goto LAB_8008498c;
  case 110:
    *(undefined1 *)(param_1 + 0x306) = 5;
    *(undefined4 *)(param_1 + 0x424) = 0xf;
    break;
  case 111:
    dispatched = 6;
LAB_8008498c:
    *(undefined1 *)(param_1 + 0x306) = dispatched;
    *(undefined4 *)(param_1 + 0x424) = 10;
    break;
  }
  case 152:
    CARDGAME_F0x80086330(param_1,param_2,0xe);
    break;
  case 155:
    CARDGAME_F0x80086eb0(param_1,param_2,(int)*(short *)(param_1 + 0x5a6),5);
    break;
  case 156:
    CARDGAME_F0x80086eb0(param_1,param_2,(int)*(short *)(param_1 + 0x5a2),9);
    break;
  case 168:
    FUN_80089ec8(param_1,param_2);
    break;
  case 154:
    CARDGAME_F0x80086d78(param_1,param_2,param_1 + 0x59c);
    break;
  case 157:
    *(undefined1 *)(param_1 + 0x444) = 0;
    break;
  case 158:
    CARDGAME_F0x80088f0c(param_1,param_2);
    break;
  case 153:
    {
      int32_t k = (uint)*(byte *)(param_1 + 0x579) * 200 + 0x59c;
      CARDGAME_F0x80086d78(param_1,param_2,param_1 + k);
      break;
    }
  case 159:
  case 160:
    CARDGAME_F0x800894e4(param_1,param_2,0);
    break;
  case 161:
    CARDGAME_F0x800894e4(param_1,param_2,1);
    break;
  case 162:
    CARDGAME_F0x800894e4(param_1,param_2,2);
    break;
  case 163:
    CARDGAME_F0x800894e4(param_1,param_2,3);
    break;
  case 164:
    CARDGAME_F0x800894e4(param_1,param_2,4);
    break;
  case 165:
    CARDGAME_F0x800894e4(param_1,param_2,5);
    break;
  case 166:
    CARDGAME_F0x800894e4(param_1,param_2,6);
    break;
  case 167:
    FUN_8008985c(param_1,param_2);
    break;
  case 172:
    CARDGAME_F0x8008baa4(param_1,param_2,uVar11,2);
    break;
  case 173:
    CARDGAME_F0x8008baa4(param_1,param_2,uVar12,1);
    break;
  case 169:
    CARDGAME_F0x8008baa4(param_1,param_2,uVar12,0);
    break;
  case 170:
    CARDGAME_F0x8008baa4(param_1,param_2,uVar11,0);
    break;
  case 171:
    CARDGAME_F0x8008baa4(param_1,param_2,uVar11,3);
    break;
  case 174:
    FUN_8008ad48(param_1,param_2,0);
    dispatched = *(undefined1 *)(param_1 + 0x421);
    *(undefined1 *)(param_1 + 0x421) = 0;
    *(undefined1 *)(param_1 + 0x420) = dispatched;
    goto LAB_80084ec8;
  case 112:
    CARDGAME_F0x8008ca4c(param_1,param_2,0);
    break;
  case 113:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x4000);
    break;
  case 114:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x2000);
    break;
  case 115:
    CARDGAME_F0x8008ca4c(param_1,param_2,1);
    break;
  case 116:
    CARDGAME_F0x8008ca4c(param_1,param_2,4);
    break;
  case 117:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x10);
    break;
  case 118:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x40);
    break;
  case 119:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x100);
    break;
  case 120:
    CARDGAME_F0x8008ca4c(param_1,param_2,2);
    break;
  case 121:
    CARDGAME_F0x8008ca4c(param_1,param_2,8);
    break;
  case 122:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x20);
    break;
  case 123:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x80);
    break;
  case 124:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x200);
    break;
  case 125:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x3ff);
    break;
  case 126:
    CARDGAME_F0x8008ca4c(param_1,param_2,0xf0000);
    break;
  case 127:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x8000);
    break;
  case 129:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x400);
    break;
  case 128:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x1400);
    break;
  case 130:
    CARDGAME_F0x8008ca4c(param_1,param_2,0x1000);
    break;
  case 131:
    CARDGAME_F0x8008ce0c(param_1,param_2,0);
    break;
  case 132:
    CARDGAME_F0x8008ce0c(param_1,param_2,1);
    break;
  case 141:
    FUN_8008ad48(param_1,param_2,1);
    dispatched = *(undefined1 *)(param_1 + 0x421);
    *(undefined1 *)(param_1 + 0x421) = 0;
    *(undefined1 *)(param_1 + 0x420) = dispatched;
    goto LAB_80084ec8;
  case 142:
    CARDGAME_F0x8008d274(param_1,param_2,cVar1,1);
    break;
  case 143:
    CARDGAME_F0x8008d274(param_1,param_2,cVar1,2);
    break;
  case 144:
    CARDGAME_F0x8008d274(param_1,param_2,cVar1,3);
    break;
  case 145:
    CARDGAME_F0x8008d274(param_1,param_2,cVar1,4);
    break;
  case 146:
    CARDGAME_F0x8008d274(param_1,param_2,cVar1,5);
    break;
  case 147:
    CARDGAME_F0x8008d274(param_1,param_2,cVar1,6);
    break;
  case 148:
    CARDGAME_F0x8008d274(param_1,param_2,cVar1,7);
    break;
  case 149:
    CARDGAME_F0x8008d274(param_1,param_2,cVar1,8);
    break;
  case 151:
    CARDGAME_F0x8008bc6c(param_1,param_2,cVar1);
    break;
  default:
    break;
  }
  dispatched = *(undefined1 *)(param_1 + 0x421);
  *(undefined1 *)(param_1 + 0x421) = 0;
  *(undefined1 *)(param_1 + 0x420) = dispatched;
  LAB_80084ec8:
  switch(*(undefined1 *)(param_1 + 0x420)) {
  case 0:
    return;
  case 1:
    {
      int iVar8;
      iVar8 = FUN_80084090(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 2:
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    *(short *)(param_1 + 0x4e0) = *(short *)(param_1 + 0x4e0) + 2;
    return;
  case 3:
    {
      undefined2 prior = *(undefined2 *)(param_1 + 0x4e0);
      *(undefined2 *)(param_1 + 0x4e2) = 2;
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      *(undefined2 *)(param_1 + 0x4e4) = prior;
      return;
    }
  case 4:
    {
      undefined2 prior = *(undefined2 *)(param_1 + 0x4e0);
      *(undefined2 *)(param_1 + 0x4e2) = 3;
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      *(undefined2 *)(param_1 + 0x4e4) = prior;
      return;
    }
  case 5:
    {
      undefined2 prior = *(undefined2 *)(param_1 + 0x4e0);
      *(undefined2 *)(param_1 + 0x4e2) = 5;
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      *(undefined2 *)(param_1 + 0x4e4) = prior;
      return;
    }
  case 6:
    uVar4 = *(short *)(param_1 + 0x4e2) - 1;
    *(ushort *)(param_1 + 0x4e2) = uVar4;
    if (0 < (int)((uint)uVar4 << 0x10)) {
      *(undefined2 *)(param_1 + 0x4e0) = *(undefined2 *)(param_1 + 0x4e4);
    }
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 7:
    FUN_80083f48(param_1,param_2,1,1);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 8:
    FUN_80083f48(param_1,param_2,1,2);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0xf:
    func_0x8001ebf8(local_70);
    {
    /* The incoming argument is dead on this returning path; reuse its life
     * for the index, keeping the offset live before the row calculation. */
    param_2 = 0;
    while ((int)param_2 < ((CardCountView *)param_1)->rows[uVar12].count) {
        int32_t off = param_2 * 2;
        int32_t k = uVar12 * 200;
        int32_t k2;
        off += k;
        k2 = *(short *)(param_1 + off + 0x600) * 2;
        ((code *)local_70[11])(*(short *)(param_1 + k2 + 0x50) + 1);
        if (*(uint8_t *)local_70[0] != 5) {
          *(short *)(param_1 + 0x4e0) = *(short *)(param_1 + 0x4e0) + -4;
          break;
        }
      param_2 = param_2 + 1;
    }
    }
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 9:
    FUN_80083f84(param_1,uVar11,10,9);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 10:
    FUN_80083fc8(param_1,uVar11,10,0xfffffff7);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0xb:
    FUN_80083f84(param_1,uVar11,3,9);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0xc:
    FUN_80083fc8(param_1,uVar11,3,0xfffffff7);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x10:
    FUN_8008404c(param_1,uVar11,5);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0xd:
    FUN_80083fc8(param_1,uVar11,2,4);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0xe:
    FUN_8008400c(param_1,uVar12,2);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x11:
    {
      int iVar8;
      iVar8 = param_1 + (*(int8_t *)(param_1 + 0x575) + -1) * 8;
      *(byte *)(iVar8 + 0x584) = *(byte *)(iVar8 + 0x584) ^ 1;
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0x12:
  case 0x13:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80092638(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x14:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80092e60(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x1b:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80093cac(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x15:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80093710(param_1,param_2,0);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x16:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80093710(param_1,param_2,1);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x17:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80093b10(param_1,param_2,0);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x18:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80093b10(param_1,param_2,1);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x19:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x800942dc(param_1,param_2,0);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x1a:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x800942dc(param_1,param_2,1);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x1c:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80094b9c(param_1,param_2,0);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x1d:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80094b9c(param_1,param_2,1);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x1e:
  case 0x1f:
  case 0x20:
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
  case 0x25:
  case 0x26:
  case 0x27:
  case 0x28:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008423c(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x29:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008f560(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x2a:
    {
      int iVar8;
      iVar8 = FUN_8008f948(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x2b:
  case 0x2d:
    {
      int iVar8;
      iVar8 = FUN_80095208(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x2c:
  case 0x2e:
    {
      int iVar8;
      iVar8 = FUN_800952f4(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x2f:
    {
      int iVar8;
      if (uVar11 == 0) {
        iVar8 = CARDGAME_F0x80095030(param_1,param_2);
        if (iVar8 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x2f4) = 2;
        return;
      }
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0x30:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80095030(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x31:
    {
      int iVar8;
      if (uVar11 == 0) {
        iVar8 = CARDGAME_F0x80095030(param_1,param_2);
        if (iVar8 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x2f4) = 2;
        return;
      }
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0x34:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar11,3,0xfd);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x35:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar11,3,0xfe);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x32:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar11,3,0xff);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x33:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar12,3,0xff);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x37:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar11,2,0xff);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x36:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar12,2,0xff);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x38:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar12,2,0x81);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x39:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar12,2,0xbf);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x3a:
    CARDGAME_F0x8008ec44(param_1,param_2,uVar11,4,0xff);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x3b:
    CARDGAME_F0x8008eec8(param_1,param_2,uVar11,0x3fc);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x3c:
    CARDGAME_F0x8008eec8(param_1,param_2,uVar11,0x2fc);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x3d:
    CARDGAME_F0x8008eec8(param_1,param_2,uVar11,0x1fc);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x3e:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008fa9c(param_1,param_2,uVar12,0);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x3f:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008fa9c(param_1,param_2,uVar11,0);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x40:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008fa9c(param_1,param_2,uVar12,1);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x41:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008f790(param_1,param_2);
      if (iVar8 == 0) {
        return;
      }
      *(undefined1 *)(param_1 + 0x4dd) = 1;
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0x42:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80090058(param_1,param_2,uVar11,3);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x43:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80090058(param_1,param_2,uVar11,4);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x44:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x800907ac(param_1,param_2,uVar11);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x45:
  case 0x46:
  case 0x47:
    {
      int iVar8;
      iVar8 = FUN_80090a90(param_1,param_2,uVar11);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x48:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80090cd8(param_1,param_2,uVar11);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x49:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008f0f4(param_1,param_2,uVar11);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x4a:
    {
      int count, index;
      int32_t k;
      goto search_start;
    found_card:
      *(int *)(param_1 + 0x440) = index;
      goto done_search;
    search_start:
      k = uVar12 * 200;
      count = *(short *)(param_1 + k + 0x5a6);
      index = 0;
      if (count > 0) {
        int limit = *(short *)(param_1 + k + 0x5a6);
      search_next:
        if (*(int8_t *)(param_1 + index + 0x446) != 0) goto found_card;
        index++;
        if (index < limit) goto search_next;
      }
    done_search:
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0x4b:
    FUN_800840d0(param_1,param_2,uVar12);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x4c:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008e5bc(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x4d:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x800915d4(param_1,param_2,0);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x4e:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x800915d4(param_1,param_2,1);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x4f:
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80091c6c(param_1,param_2,uVar11);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x57:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80092198(param_1,param_2,uVar11);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x58:
    {
      int iVar8;
      iVar8 = FUN_8008deac(param_1,param_2,0x24);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x59:
    {
      int iVar8;
      iVar8 = FUN_8008deac(param_1,param_2,0x1c);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x5a:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80094848(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x5b:
    FUN_800840f0(param_1,param_2);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x5c:
    CARDGAME_F0x8008e398(param_1,param_2);
    *(undefined1 *)(param_1 + 0x2f4) = 2;
    return;
  case 0x5d:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8009535c(param_1,param_2);
      if (iVar8 != 1) {
        if (iVar8 != 2) {
          return;
        }
        *(undefined1 *)(param_1 + 0x421) = 0x4d;
        return;
      }
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0x5e:
  case 0x5f:
  case 0x60:
  case 0x61:
  case 0x62:
  case 99:
  case 100:
  case 0x65:
  case 0x66:
  case 0x67:
  case 0x68:
  case 0x69:
    {
      int iVar8;
      iVar8 = FUN_8008e08c(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x6a:
  case 0x6b:
  case 0x6c:
  case 0x6d:
  case 0x6e:
  case 0x6f:
    {
      int iVar8;
      iVar8 = FUN_80084090(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 2;
      }
      return;
    }
  case 0x70:
  case 0x71:
  case 0x72:
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
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008cb5c(param_1,param_2);
      if (iVar8 == -1) {
        return;
      }
      *(int8_t *)(param_1 + 0x57a) = (int8_t)iVar8;
      *(undefined1 *)(param_1 + 0x2f4) = 0;
      return;
    }
  case 0x7f:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008cb5c(param_1,param_2);
      if (iVar8 == -1) {
        return;
      }
      *(int8_t *)(param_1 + 0x57a) = (int8_t)iVar8;
      *(undefined1 *)(param_1 + 0x2f4) = 0;
      return;
    }
  case 0x80:
  case 0x81:
  case 0x82:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008cb5c(param_1,param_2);
      if (iVar8 == -1) {
        return;
      }
      *(int8_t *)(param_1 + 0x57a) = (int8_t)iVar8;
      *(undefined1 *)(param_1 + 0x2f4) = 0;
      return;
    }
  case 0x83:
  case 0x84:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008cfdc(param_1,param_2);
      if (iVar8 == -1) {
        return;
      }
      *(int8_t *)(param_1 + 0x57a) = (int8_t)iVar8;
      *(undefined1 *)(param_1 + 0x2f4) = 0;
      return;
    }
  case 0x87:
    CARDGAME_F0x8008eec8(param_1,param_2,cVar1,0x180);
    *(undefined1 *)(param_1 + 0x421) = 0x8d;
    return;
  case 0x86:
    CARDGAME_F0x8008eec8(param_1,param_2,cVar1,0x1bc);
    *(undefined1 *)(param_1 + 0x421) = 0x8d;
    return;
  case 0x85:
    CARDGAME_F0x8008eec8(param_1,param_2,cVar1,0x1fc);
    *(undefined1 *)(param_1 + 0x421) = 0x8d;
    return;
  case 0x8a:
    CARDGAME_F0x8008eec8(param_1,param_2,cVar1,0x280);
    *(undefined1 *)(param_1 + 0x421) = 0x8d;
    return;
  case 0x89:
    CARDGAME_F0x8008eec8(param_1,param_2,cVar1,700);
    *(undefined1 *)(param_1 + 0x421) = 0x8d;
    return;
  case 0x88:
    CARDGAME_F0x8008eec8(param_1,param_2,cVar1,0x2fc);
    *(undefined1 *)(param_1 + 0x421) = 0x8d;
    return;
  case 0x8b:
    CARDGAME_F0x8008eec8(param_1,param_2,cVar1,0x3fc);
    *(undefined1 *)(param_1 + 0x421) = 0x8d;
    return;
  case 0x8c:
    CARDGAME_F0x8008eec8(param_1,param_2,cVar1,0x380);
    *(undefined1 *)(param_1 + 0x421) = 0x8d;
    return;
  case 0x8d:
    {
    int result = CARDGAME_F0x8008ad98(param_1,param_2);
    switch (result) {
    case 1:
      *(undefined1 *)(param_1 + 0x57a) = 0;
      break;
    case 2:
      {
      int index = *(int *)(param_1 + 0x440);
      if (index < 6) {
        uVar2 = index << 3;
        uVar2 -= index;
        uVar2 <<= 1;
        cardAddress = param_1 + uVar2;
        uVar2 = *(undefined1 *)(cardAddress + 0x73a);
        selectedOffset = (int)*(int8_t *)(param_1 + 0x575) << 3;
      }
      else {
        index -= 6;
        uVar2 = index << 3;
        uVar2 -= index;
        uVar2 <<= 1;
        cardAddress = param_1 + uVar2;
        uVar2 = *(undefined1 *)(cardAddress + 0x7ac);
        selectedOffset = (int)*(int8_t *)(param_1 + 0x575) << 3;
      }
      goto LAB_80085db4;
      }
    default:
      return;
    }
    *(undefined1 *)(param_1 + 0x2f4) = 0;
    return;
    }
  case 0x98:
    {
      int result = CARDGAME_F0x8008642c(param_1,param_2);
      switch (result) {
      case 0:
        return;
      case 1:
        *(int8_t *)(param_1 + 0x57a) = result;
        *(undefined1 *)(param_1 + 0x2f4) = 0;
        return;
      case 2:
        *(undefined1 *)(param_1 + 0x57a) = 0;
        *(undefined1 *)(param_1 + 0x2f4) = 0;
        return;
      default:
        return;
      }
    }
  case 0x99:
    {
      int32_t k = (uint)*(byte *)(param_1 + 0x579) * 200 + 0x59c;
      iVar8 = CARDGAME_F0x80087edc(param_1,param_2,param_1 + k);
    }
    if (iVar8 == -1) {
      return;
    }
    *(int8_t *)(param_1 + 0x57a) = (int8_t)iVar8;
LAB_80085dc4:
    *(undefined1 *)(param_1 + 0x2f4) = 0;
    return;
  case 0x9a:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80087edc(param_1,param_2,param_1 + 0x59c);
      if (iVar8 != -1) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x9d:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008c5f0(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x9b:
  case 0x9c:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80087edc(param_1,param_2,param_1 + 0x59c);
      if (iVar8 != -1) {
        *(undefined1 *)(param_1 + 0x2f4) = 3;
      }
      return;
    }
  case 0xa8:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008a068(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 3;
      }
      return;
    }
  case 0x9e:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80088f78(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0x9f:
  case 0xa0:
  case 0xa1:
  case 0xa2:
  case 0xa3:
  case 0xa4:
  case 0xa5:
  case 0xa6:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80089580(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0xa7:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x80089974(param_1,param_2);
      if (iVar8 != 0) {
        *(undefined1 *)(param_1 + 0x2f4) = 0;
      }
      return;
    }
  case 0xac:
    {
      int iVar8;
      if (uVar11 == 0) {
        iVar8 = CARDGAME_F0x8008c044(param_1,param_2,2);
        if (iVar8 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x2f4) = 2;
        return;
      }
      FUN_8008c8d8(param_1,param_2);
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0xad:
    {
      int iVar8;
      if (uVar11 == 0) {
        iVar8 = CARDGAME_F0x8008c044(param_1,param_2,0);
        if (iVar8 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x2f4) = 2;
        return;
      }
      CARDGAME_F0x8008c988(param_1,param_2);
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0xa9:
    {
      int iVar8;
      if (uVar11 == 0) {
        iVar8 = CARDGAME_F0x8008c044(param_1,param_2,0);
        if (iVar8 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x2f4) = 2;
        return;
      }
      CARDGAME_F0x8008c6e0(param_1,param_2,1);
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0xaa:
    {
      int iVar8;
      if (uVar11 == 0) {
        iVar8 = CARDGAME_F0x8008c044(param_1,param_2,0);
        if (iVar8 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x2f4) = 2;
        return;
      }
      CARDGAME_F0x8008c6e0(param_1,param_2,0);
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0xab:
    {
      int iVar8;
      if (uVar11 == 0) {
        iVar8 = CARDGAME_F0x8008c044(param_1,param_2,0);
        if (iVar8 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x2f4) = 2;
        return;
      }
      CARDGAME_F0x8008c6e0(param_1,param_2,1);
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0xae:
    {
      int iVar8;
      if (uVar11 == 0) {
        iVar8 = CARDGAME_F0x8008ad98(param_1,param_2);
        if (iVar8 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x2f4) = 2;
        return;
      }
      CARDGAME_F0x80084188(param_1);
  LAB_80085ce8:
      *(undefined1 *)(param_1 + 0x2f4) = 2;
      return;
    }
  case 0x8e:
  case 0x8f:
  case 0x90:
  case 0x91:
  case 0x92:
  case 0x93:
  case 0x94:
  case 0x95:
    {
      int iVar8;
      iVar8 = CARDGAME_F0x8008d594(param_1,param_2);
      if (iVar8 == -1) {
        return;
      }
      *(int8_t *)(param_1 + 0x57a) = (int8_t)iVar8;
      *(undefined1 *)(param_1 + 0x2f4) = 0;
      return;
    }
  case 0x96:
    CARDGAME_F0x8008ec44(param_1,param_2,cVar1,2,0xfd);
    *(undefined1 *)(param_1 + 0x421) = 0x97;
    return;
  case 0x97:
    {
    int result = CARDGAME_F0x8008c044(param_1,param_2,1);
    switch (result) {
    case 1:
      *(undefined1 *)(param_1 + 0x57a) = 0;
      *(undefined1 *)(param_1 + 0x2f4) = 0;
      return;
    case 2:
      if (cVar1 == '\0') {
        selectedOffset = (int)*(int8_t *)(param_1 + 0x575) << 3;
        uVar2 = *(int *)(param_1 + 0x440) * 2;
        cardAddress = param_1 + uVar2;
        uVar2 = *(undefined1 *)(cardAddress + 0x600);
      }
      else {
        selectedOffset = (int)*(int8_t *)(param_1 + 0x575) << 3;
        uVar2 = *(int *)(param_1 + 0x440) * 2;
        cardAddress = param_1 + uVar2;
        uVar2 = *(undefined1 *)(cardAddress + 0x6c8);
      }
LAB_80085db4:
      *(undefined1 *)(param_1 + selectedOffset + 0x586) = uVar2;
      *(undefined1 *)(param_1 + 0x57a) = 1;
      *(undefined1 *)(param_1 + 0x2f4) = 0;
      return;
    }
    }
  }
  return;
}
