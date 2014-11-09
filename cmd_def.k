
// Definice maker v NodeTab
#define cGoNod     0x81  // If Ne, pricti k indexu nasl. hodnotu
#define cExitCode  0x82  // If Ano, ukonci test a vrat nasledujici navratovou hodnotu
#define cWaitPar   0x83  // Ocekavej parametry (muze byt text i cislo)
#define cResetIx   0x84  // Zacni znovu - nuluj index
#define cExitRet   0x85  // Skonci a zapis nasledujici akci do bufferu
#define cResetRun  0x86  // Restartuj detekci, a zapis nasledujici akci do buf
#define cGoNodFar  0x87  // If Ne, pricti k indexu nasl. hodnotu + 100h
#define cGoNodFar2 0x88  // If Ne, pricti k indexu nasl. hodnotu + 200h
#define cGoNodFar3 0x89  // If Ne, pricti k indexu nasl. hodnotu + 300h
#define cGoNodFar4 0x8A  // If Ne, pricti k indexu nasl. hodnotu + 400h
#define cRptRstIx  0x8B  // Zacni znovu, testuj znovu cyklus prvni urovne
