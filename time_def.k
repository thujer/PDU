
// Definice pro textovy report
#define  cTime_TimeoutsReport  1
#define  cTime_SysTimeReport   2

// Prirazeni timeoutu rutinam
#define cMain_TimeGuard          0  // Main:     Spousteni casovych akci
#define cMain_Delay              1  // Main:     Akce "Zpozdeni"
#define cMain_RG_TestErr         2  // Main:     Test havarijniho statusu
#define cMain_RecognizeSMS       3  // Main:     Rozpoznavani SMS
#define cMain_EndRecogn          4  // Main:     Konec recognizace SMS
#define cMain_SIMSet             5  // Main:     Nastaveni parametru SIM
#define cMain_SendSMSs           6  // Main:     Odesilani SMS
#define cGSM_Init                7  // GSM:      Inicializace GSM modemu
#define cGSM_EraseSMS            8  // GSM:      Vymaz SMS a reset priznaku
#define cGSM_CallBack            9  // GSM:      Volani zpet
#define cGSM_Report             10  // GSM:      Vytvoreni reportu
#define cGSM_SendSMS            11  // GSM:      Odeslani SMS
#define cAction_Timeout         12  // Main:     Process driver
#define cAction_Timeout2        13  // Main:     Process driver 2
#define cRG320_SendMessage      14  // RG320:    Odeslani zpravy
#define cRG320_ReadData         15  // RG320:    Cteni vice dat
#define cRG320_Report           16  // RG320:    Vytvoreni reportu
#define cEEP_WriteData          17  // EEPROM:   Sekvencni zapis dat
#define cEEP_Clear              18  // EEPROM:   Sekvencni vymazani
#define cHCOM_IntUp             19  // HCOM:     Konec vysilani bez preruseni
#define cLoopTest               20  // LoopTest: Test hlavni smycky
#define cLED_Timer              21  // LED:      Rizeni doby svitu signalizacnich LED
// ----------------------------
#define No_Timeouts             22  // Pocet timeoutu celkem

