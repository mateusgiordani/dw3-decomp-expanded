extern void CARDGAME_F0x80095bfc(void);
extern int EXE_F0x80014504(void *a0, int a1, int a2);

void CARDGAME_F0x80095e2c(void) {
    EXE_F0x80014504(CARDGAME_F0x80095bfc, 0x50, 4);
}
