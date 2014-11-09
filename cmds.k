#include "cmd_def.k"      // Seznam ID prikazu pro pohyb v tabulce

// Definice navratovych hodnot pro jednotlive prikazy
//----------------------------------------------------
// Prikazy GSM modem
#define cCMD_CLIP       1
#define cCMD_CMGR       2
#define cCMD_CMTI       3
#define cCMD_CPIN       4
#define cCMD_CUSD       5
#define cCMD_COPS       6
#define cCMD_CSQ        7
#define cCMD_RING       8
#define cCMD_OK         9
#define cCMD_ERROR     10
#define cCMD_TC35      11
#define cCMD_M20       12
#define cCMD_SMS_ED    13

// SMS prikazy (Pristupne pouze pokud ma uzivatel prislusna prava)
#define cCMD_MENU      50
#define cCMD_KREDIT    51
#define cCMD_KUPON     52
#define cCMD_CAS       53
#define cCMD_MOD       54
#define cCMD_MOBIL     55
#define cCMD_TEPVEN    56
#define cCMD_TEPVODY   57
#define cCMD_TEPEKV    58
#define cCMD_TEPLOTY   59
#define cCMD_TEPZPA    60
#define cCMD_VSTUP230  61
#define cCMD_GETERR    62
#define cCMD_GETTIME   63
#define cCMD_SKVYKON   64
#define cCMD_VETMOD    65
#define cCMD_VETCERP   66
#define cCMD_VETTVODY  67
#define cCMD_VETPTVODY 68
#define cCMD_VETETVODY 69
#define cCMD_VETTMIST  70
#define cCMD_VETPTMIST 71
#define cCMD_VETETMIST 72
#define cCMD_VETUEQ    73
#define cCMD_VETDEQ    74
#define cCMD_TUVZPUSOB 75
#define cCMD_TUVSTAV   76
#define cCMD_TUVREZIM  77
#define cCMD_FormatEEP 78
#define cCMD_SetSIMSet 79
#define cCMD_ZAVOLEJ   80
#define cCMD_ERRDEF    81
#define cCMD_EEPREADH  82
#define cCMD_EEPREADA  83
#define cCMD_MACRO     84
#define cCMD_SMSMODE   85
#define cCMD_VETREZIM  86

//----------------------------------------------------

// Tabulka prikazu
uchar CMDs[]={/*


 Char | Action    Par  | Action    Par  |  Offset
      | <-----(=)----> | <-----(!=)---> |
                                           */
   0,     cGoNod,               5,     cGoNod,   5,    // 0

 // #MENU
 '#',     cGoNod,               5,     cGoNod,  25,    // 5                    <-+
 'M',     cGoNod,               5,   cResetIx,   0,    // 10                     |
 'E',     cGoNod,               5,   cResetIx,   0,    // 15                     |
 'N',     cGoNod,               5,   cResetIx,   0,    // 20                     |
 'U',  cExitCode,       cCMD_MENU,   cResetIx,   0,    // 25                     |
                                                       //                        |
 // +CLIP: XX                                                                    |
 '+',     cGoNod,               5,     cGoNod, 140,    // 30                   <-+
 'C',     cGoNod,               5,   cResetIx,   0,    // 35                     |
 'L',     cGoNod,               5,     cGoNod,  20,    // 40               <-+   |
 'I',     cGoNod,               5,   cResetIx,   0,    // 45                 |   |
 'P',     cGoNod,               5,   cResetIx,   0,    // 50                 |   |
 ':',   cWaitPar,       cCMD_CLIP,   cResetIx,   0,    // 55                 |   |
                                                       //                    |   |
 // +CMGR: XX                                                                |   |
 'M',     cGoNod,               5,     cGoNod,  35,    // 60               <-+   |
 'G',     cGoNod,               5,     cGoNod,  15,    // 65           <-+   |   |
 'R',     cGoNod,               5,   cResetIx,   0,    // 70             |   |   |
 ':',   cWaitPar,       cCMD_CMGR,   cResetIx,   0,    // 75             |   |   |
                                                       //                |   |   |
 // +CMTI: XX                                                            |   |   |
 'T',     cGoNod,               5,  cRptRstIx,   0,    // 80           <-+   |   |
 'I',     cGoNod,               5,   cResetIx,   0,    // 85                 |   |
 ':',   cWaitPar,       cCMD_CMTI,   cResetIx,   0,    // 90                 |   |
                                                       //                    |   |
 // +COPS: XX                                                                |   |
 'O',     cGoNod,               5,     cGoNod,  20,    // 95               <-+   |
 'P',     cGoNod,               5,   cResetIx,   0,    // 100                |   |
 'S',     cGoNod,               5,   cResetIx,   0,    // 105                |   |
 ':',   cWaitPar,       cCMD_COPS,   cResetIx,   0,    // 110                |   |
                                                       //                    |   |
 // +CPIN: XX                                                                |   |
 'P',     cGoNod,               5,     cGoNod,  20,    // 115              <-+   |
 'I',     cGoNod,               5,   cResetIx,   0,    // 120                |   |
 'N',     cGoNod,               5,   cResetIx,   0,    // 125                |   |
 ':',   cWaitPar,       cCMD_CPIN,   cResetIx,   0,    // 130                |   |
                                                       //                    |   |
 // +CSQ: XX                                                                 |   |
 'S',     cGoNod,               5,     cGoNod,  15,    // 135              <-+   |
 'Q',     cGoNod,               5,   cResetIx,   0,    // 140                |   |
 ':',   cWaitPar,        cCMD_CSQ,   cResetIx,   0,    // 145                |   |
                                                       //                    |   |
 // +CUSD: XX                                                                |   |
 'U',     cGoNod,               5,  cRptRstIx,   0,    // 150              <-+   |
 'S',     cGoNod,               5,   cResetIx,   0,    // 155                    |
 'D',     cGoNod,               5,   cResetIx,   0,    // 160                    |
 ':',   cWaitPar,       cCMD_CUSD,   cResetIx,   0,    // 165                    |
                                                       //                        |
 // >_                                                                           |
 '>',     cGoNod,               5,     cGoNod,  10,    // 170                  <-+
 ' ',  cExitCode,     cCMD_SMS_ED,   cResetIx,   0,    // 175                    |
                                                       //                        |
 // CAS                                                                          |
 'C',     cGoNod,               5,     cGoNod,  15,    // 180                  <-+
 'A',     cGoNod,               5,   cResetIx,   0,    // 185                    |
 'S',  cExitCode,        cCMD_CAS,   cResetIx,   0,    // 190                    |
                                                       //                        |
 // EEPFORMAT                                                                    |
 'E',     cGoNod,               5,     cGoNod,  70,    // 195                  <-+
 'E',     cGoNod,               5,     cGoNod,  40,    // 200              <-+   |
 'P',     cGoNod,               5,   cResetIx,   0,    // 205               5|   |
 'F',     cGoNod,               5, cGoNodFar3,  37,    // 210+768+37  ->- -10|->-| ---------+
 'O',     cGoNod,               5,   cResetIx,   0,    // 215              15|   |          |
 'R',     cGoNod,               5,   cResetIx,   0,    // 220              20|   |          |
 'M',     cGoNod,               5,   cResetIx,   0,    // 225              25|   |          |
 'A',     cGoNod,               5,   cResetIx,   0,    // 230              30|   |          |
 'T',  cExitCode,  cCMD_FormatEEP,   cResetIx,   0,    // 235              35|   |          |
                                                       //                    |   |          |
 // ERROR<CR>                                                              40|   |          |
 'R',     cGoNod,               5,  cRptRstIx,   0,    // 240              <-+   |          |
 'R',     cGoNod,               5,   cResetIx,   0,    // 245                    |          |
 'O',     cGoNod,               5, cGoNodFar2, 218,    // 250                    | <-+      |
 'R',     cGoNod,               5,   cResetIx,   0,    // 255                    |   |      |
  13,  cExitCode,      cCMD_ERROR,   cResetIx,   0,    // 260                    |   |      |
                                                       //                        |   |      |
 // HAVSTAV                                                                      |   |      |
 'H',     cGoNod,               5,     cGoNod,  35,    // 265                  <-+   |      |
 'A',     cGoNod,               5,   cResetIx,   0,    // 270                    |   |      |
 'V',     cGoNod,               5,   cResetIx,   0,    // 275                    |   |      |
 'S',     cGoNod,               5,   cResetIx,   0,    // 280                    |   |      |
 'T',     cGoNod,               5,   cResetIx,   0,    // 285                    |   |      |
 'A',     cGoNod,               5,   cResetIx,   0,    // 290                    |   |      |
 'V',  cExitCode,     cCMD_GETERR,   cResetIx,   0,    // 295                    |   |      |
                                                       //                        |   |      |
 // KREDIT                                                                       |   |      |
 'K',     cGoNod,               5,     cGoNod,  50,    // 300                  <-+   |      |
 'R',     cGoNod,               5,     cGoNod,  25,    // 305              <-+   |   |      |
 'E',     cGoNod,               5,   cResetIx,   0,    // 310                |   |   |      |
 'D',     cGoNod,               5,   cResetIx,   0,    // 315                |   |   |      |
 'I',     cGoNod,               5,   cResetIx,   0,    // 320                |   |   |      |
 'T',  cExitCode,     cCMD_KREDIT,   cResetIx,   0,    // 325                |   |   |      |
                                                       //                    |   |   |      |
 // KUPON XXXXX                                                              |   |   |      |
 'U',     cGoNod,               5,  cRptRstIx,   0,    // 330              <-+   |   |      |
 'P',     cGoNod,               5,   cResetIx,   0,    // 335                    |   |      |
 'O',     cGoNod,               5,   cResetIx,   0,    // 340                    |   |      |
 'N',   cWaitPar,      cCMD_KUPON,   cResetIx,   0,    // 345                    |   |      |
                                                       //                        |   |      |
 // M20<CR>                                                                      |   |      |
 'M',     cGoNod,               5,     cGoNod,  45,    // 350                  <-+   |      |
 '2',     cGoNod,               5,     cGoNod,  15,    // 355              <-+   |   |      |
 '0',     cGoNod,               5,   cResetIx,   0,    // 360                |   |   |      |
  13,  cExitCode,        cCMD_M20,   cResetIx,   0,    // 365                |   |   |      |
                                                       //                    |   |   |      |
 // MOBIL XX                                                                 |   |   |      |
 'O',     cGoNod,               5, cGoNodFar2, 113,    // 370              <-+->-|->-|---+  |
 'B',     cGoNod,               5,     cGoNod,  15,    // 375              <-+   |   |   |  |
 'I',     cGoNod,               5,   cResetIx,   0,    // 380                |   |   |   v  |
 'L',   cWaitPar,      cCMD_MOBIL,   cResetIx,   0,    // 385                |   |   |   |  |
                                                       //                    |   |   |   |  |
 // MOD XX                                                                   |   |   |   |  |
 'D',   cWaitPar,        cCMD_MOD,  cRptRstIx,   0,    // 390              <-+   |   |   |  |
                                                       //                        |   |   |  |
 // OK                                                                           |   |   |  |
 'O',     cGoNod,               5,     cGoNod,  15,    // 395                  <-+   |   |  |
 'K',     cGoNod,               5,   cResetIx,   0,    // 400                    |   |   |  |
  13,  cExitCode,         cCMD_OK,   cResetIx,   0,    // 405                    |   |   |  |
                                                       //                        |   |   |  |
 // RGCAS                                                                        |   |   |  |
 'R',     cGoNod,               5,     cGoNod,  45,    // 410                  <-+   |   |  |
 'G',     cGoNod,               5,     cGoNod,  20,    // 415              <-+   |   |   |  |
 'C',     cGoNod,               5,   cResetIx,   0,    // 420                |   |   |   |  |
 'A',     cGoNod,               5,   cResetIx,   0,    // 425                |   |   |   |  |
 'S',  cExitCode,    cCMD_GETTIME,   cResetIx,   0,    // 430                |   |   |   |  |
                                                       //                    |   |   |   |  |
 // RING<CR>                                                                 |   |   |   |  |
 'I',     cGoNod,               5,  cRptRstIx,   0,    // 435              <-+   |   |   |  |
 'N',     cGoNod,               5,   cResetIx,   0,    // 440                    |   |   |  |
 'G',     cGoNod,               5,   cResetIx,   0,    // 445                    |   |   |  |
  13,  cExitCode,       cCMD_RING,   cResetIx,   0,    // 450                    |   |   |  |
                                                       //                        |   |   |  |
 // SIMSET XX                                                                    |   |   |  |
 'S',     cGoNod,               5,     cGoNod,  30,    // 455                  <-+   |   |  |
 'I',     cGoNod,               5, cGoNodFar2,  73,    // 460+512+73  ---->----  |   |   |  | --+
 'M',     cGoNod,               5,   cResetIx,   0,    // 465                    |   |   |  |   |
 'S',     cGoNod,               5,   cResetIx,   0,    // 470                    |   |   |  |   |
 'E',     cGoNod,               5,   cResetIx,   0,    // 475                    |   |   |  |   |
 'T',   cWaitPar,  cCMD_SetSIMSet,   cResetIx,   0,    // 480                    |   |   |  |   |
                                                       //                        |   |   |  |   |
 // TC35<CR>                                                                     |   |   |  |   |
 'T',     cGoNod,               5,     cGoNod, 180,    // 485                  <-+   |   |  |   |
 'C',     cGoNod,               5,     cGoNod,  20,    // 490              <-+   |   |   |  |   |
 '3',     cGoNod,               5,   cResetIx,   0,    // 495                |   |   |   |  |   |
 '5',     cGoNod,               5,   cResetIx,   0,    // 500                |   |   |   |  |   |
  13,  cExitCode,       cCMD_TC35,   cResetIx,   0,    // 505                |   |   |   |  |   |
                                                       //                    |   |   |   |  |   |
 // TEPEKV                                                                   |   |   |   |  |   |
 'E',     cGoNod,               5,     cGoNod,  90,    // 510              <-+   |   |   |  |   |
 'P',     cGoNod,               5,   cResetIx,   0,    // 515                |   |   |   |  |   |
 'E',     cGoNod,               5,     cGoNod,  15,    // 520          <-+   |   |   |   |  |   |
 'K',     cGoNod,               5,   cResetIx,   0,    // 525            |   |   |   |   |  |   |
 'V',  cExitCode,     cCMD_TEPEKV,   cResetIx,   0,    // 530            |   |   |   |   |  |   |
                                                       //                |   |   |   |   |  |   |
 // TEPLOTY                                                              |   |   |   |   |  |   |
 'L',     cGoNod,               5,     cGoNod,  20,    // 535          <-+   |   |   |   |  |   |
 'O',     cGoNod,               5,   cResetIx,   0,    // 540            |   |   |   |   |  |   |
 'T',     cGoNod,               5,   cResetIx,   0,    // 545            |   |   |   |   |  |   |
 'Y',  cExitCode,    cCMD_TEPLOTY,   cResetIx,   0,    // 550            |   |   |   |   |  |   |
                                                       //                |   |   |   |   |  |   |
 // TEPVEN                                                               |   |   |   |   |  |   |
 'V',     cGoNod,               5,     cGoNod,  30,    // 555           -+   |   |   |   |  |   |
 'E',     cGoNod,               5,     cGoNod,  10,    // 560      <-+   |   |   |   |   |  |   |
 'N',  cExitCode,     cCMD_TEPVEN,   cResetIx,   0,    // 565        |   |   |   |   |   |  |   |
                                                       //            |   |   |   |   |   |  |   |
 // TEPVODY                                                          |   |   |   |   |   |  |   |
 'O',     cGoNod,               5,     cGoNod,  15,    // 570      <-+   |   |   |   |   |  |   |
 'D',     cGoNod,               5,   cResetIx,   0,    // 575            |   |   |   |   |  |   |
 'Y',  cExitCode,    cCMD_TEPVODY,   cResetIx,   0,    // 580            |   |   |   |   |  |   |
                                                       //                |   |   |   |   |  |   |
 // TEPZPA                                                               |   |   |   |   |  |   |
 'Z',     cGoNod,               5,  cRptRstIx,   0,    // 585          <-+   |   |   |   |  |   |
 'P',     cGoNod,               5,   cResetIx,   0,    // 590                |   |   |   |  |   |
 'A',  cExitCode,     cCMD_TEPZPA,   cResetIx,   0,    // 595                |   |   |   |  |   |
                                                       //                    |   |   |   |  |   |
 // TUVMOD                                                                   |   |   |   |  |   |
 'U',     cGoNod,               5,  cRptRstIx,   0,    // 600              <-+   |   |   |  |   |
 'V',     cGoNod,               5,   cResetIx,   0,    // 605                    |   |   |  |   |
 'M',     cGoNod,               5,     cGoNod,  15,    // 610          <-+       |   |   |  |   |
 'O',     cGoNod,               5,   cResetIx,   0,    // 615            |       |   |   |  |   |
 'D',  cExitCode,   cCMD_TUVREZIM,   cResetIx,   0,    // 620            |       |   |   |  |   |
                                                       //                |       |   |   |  |   |
 // TUVSTAV                                                              |       |   |   |  |   |
 'S',     cGoNod,               5,     cGoNod,  20,    // 625          <-+       |   |   |  |   |
 'T',     cGoNod,               5,   cResetIx,   0,    // 630            |       |   |   |  |   |
 'A',     cGoNod,               5,   cResetIx,   0,    // 635            |       |   |   |  |   |
 'V',  cExitCode,    cCMD_TUVSTAV,   cResetIx,   0,    // 640            |       |   |   |  |   |
                                                       //                |       |   |   |  |   |
 // TUVZPUS                                                              |       |   |   |  |   |
 'Z',     cGoNod,               5,  cRptRstIx,   0,    // 645          <-+       |   |   |  |   |
 'P',     cGoNod,               5,   cResetIx,   0,    // 650                    |   |   |  |   |
 'U',     cGoNod,               5,   cResetIx,   0,    // 655                    |   |   |  |   |
 'S',  cExitCode,  cCMD_TUVZPUSOB,   cResetIx,   0,    // 660                    |   |   |  |   |
                                                       //                        |   |   |  |   |
 // VETDEKV XX                                                                   |   |   |  |   |
 'V',     cGoNod,               5,  cGoNodFar,  24,    // 665                  <-+   |   |  |   |
 'E',     cGoNod,               5,     cGoNod, 220,    // 670              <-+   |   |   |  |   |
 'T',     cGoNod,               5,   cResetIx,   0,    // 675                |   |   |   |  |   |
 'D',     cGoNod,               5,     cGoNod,  20,    // 680          <-+   |   |   |   |  |   |
 'E',     cGoNod,               5,   cResetIx,   0,    // 685            |   |   |   |   |  |   |
 'K',     cGoNod,               5,   cResetIx,   0,    // 690            |   |   |   |   |  |   |
 'V',   cWaitPar,     cCMD_VETDEQ,   cResetIx,   0,    // 695            |   |   |   |   |  |   |
                                                       //                |   |   |   |   |  |   |
 // VETMOD XX                                                            |   |   |   |   |  |   |
 'M',     cGoNod,               5,     cGoNod,  15,    // 700          <-+   |   |   |   |  |   |
 'O',     cGoNod,               5,   cResetIx,   0,    // 705            |   |   |   |   |  |   |
 'D',   cWaitPar,     cCMD_VETMOD,   cResetIx,   0,    // 710            |   |   |   |   |  |   |
                                                       //                |   |   |   |   |  |   |
 // VETNEKV XX                                                           |   |   |   |   |  |   |
 'N',     cGoNod,               5,     cGoNod,  20,    // 715          <-+   |   |   |   |  |   |
 'E',     cGoNod,               5,   cResetIx,   0,    // 720            |   |   |   |   |  |   |
 'K',     cGoNod,               5,   cResetIx,   0,    // 725            |   |   |   |   |  |   |
 'V',   cWaitPar,     cCMD_VETUEQ,   cResetIx,   0,    // 730            |   |   |   |   |  |   |
                                                       //                |   |   |   |   |  |   |
 // VETTCERP XX                                                          |   |   |   |   |  |   |
 'T',     cGoNod,               5,  cGoNodFar,  79,    // 735          <-+   |   |   |   |  |   |
 'C',     cGoNod,               5,     cGoNod,  20,    // 740      <-+   |   |   |   |   |  |   |
 'E',     cGoNod,               5,   cResetIx,   0,    // 745        |   |   |   |   |   |  |   |
 'R',     cGoNod,               5,   cResetIx,   0,    // 750        |   |   |   |   |   |  |   |
 'P',   cWaitPar,    cCMD_VETCERP,   cResetIx,   0,    // 755        |   |   |   |   |   |  |   |
                                                       //            |   |   |   |   |   |  |   |
 // VETTMIST XX                                                      |   |   |   |   |   |  |   |
 'M',     cGoNod,               5,     cGoNod,  20,    // 760      <-+   |   |   |   |   |  |   |
 'I',     cGoNod,               5,   cResetIx,   0,    // 765        |   |   |   |   |   |  |   |
 'S',     cGoNod,               5,   cResetIx,   0,    // 770        |   |   |   |   |   |  |   |
 'T',   cWaitPar,   cCMD_VETTMIST,   cResetIx,   0,    // 775        |   |   |   |   |   |  |   |
                                                       //            |   |   |   |   |   |  |   |
 // VETTOMIST XX                                                     |   |   |   |   |   |  |   |
 'O',     cGoNod,               5,     cGoNod,  45,    // 780      <-+   |   |   |   |   |  |   |
 'M',     cGoNod,               5,     cGoNod,  20,    // 785  <-+   |   |   |   |   |   |  |   |
 'I',     cGoNod,               5,   cResetIx,   0,    // 790    |   |   |   |   |   |   |  |   |
 'S',     cGoNod,               5,   cResetIx,   0,    // 795    |   |   |   |   |   |   |  |   |
 'T',   cWaitPar,  cCMD_VETETMIST,   cResetIx,   0,    // 800    |   |   |   |   |   |   |  |   |
                                                       //        |   |   |   |   |   |   |  |   |
 // VETTOVODY XX                                                 |   |   |   |   |   |   |  |   |
 'V',     cGoNod,               5,     cGoNod,  20,    // 805  <-+   |   |   |   |   |   |  |   |
 'O',     cGoNod,               5,   cResetIx,   0,    // 810        |   |   |   |   |   |  |   |
 'D',     cGoNod,               5,   cResetIx,   0,    // 815        |   |   |   |   |   |  |   |
 'Y',   cWaitPar,  cCMD_VETETVODY,   cResetIx,   0,    // 820        |   |   |   |   |   |  |   |
                                                       //            |   |   |   |   |   |  |   |
 // VETTPMIST XX                                                     |   |   |   |   |   |  |   |
 'P',     cGoNod,               5,     cGoNod,  45,    // 825      <-+   |   |   |   |   |  |   |
 'M',     cGoNod,               5,     cGoNod,  20,    // 830  <-+   |   |   |   |   |   |  |   |
 'I',     cGoNod,               5,   cResetIx,   0,    // 835    |   |   |   |   |   |   |  |   |
 'S',     cGoNod,               5,   cResetIx,   0,    // 840    |   |   |   |   |   |   |  |   |
 'T',   cWaitPar,  cCMD_VETPTMIST,   cResetIx,   0,    // 845    |   |   |   |   |   |   |  |   |
                                                       //        |   |   |   |   |   |   |  |   |
 // VETTPVODY XX                                                 |   |   |   |   |   |   |  |   |
 'V',     cGoNod,               5,  cRptRstIx,   0,    // 850  <-+   |   |   |   |   |   |  |   |
 'O',     cGoNod,               5,   cResetIx,   0,    // 855        |   |   |   |   |   |  |   |
 'D',     cGoNod,               5,   cResetIx,   0,    // 860        |   |   |   |   |   |  |   |
 'Y',   cWaitPar,  cCMD_VETPTVODY,   cResetIx,   0,    // 865        |   |   |   |   |   |  |   |
                                                       //            |   |   |   |   |   |  |   |
 // VETTVODY                                                         |   |   |   |   |   |  |   |
 'V',     cGoNod,               5,  cRptRstIx,   0,    // 870      <-+   |   |   |   |   |  |   |
 'O',     cGoNod,               5,   cResetIx,   0,    // 875            |   |   |   |   |  |   |
 'D',     cGoNod,               5,   cResetIx,   0,    // 880            |   |   |   |   |  |   |
 'Y',   cWaitPar,   cCMD_VETTVODY,   cResetIx,   0,    // 885            |   |   |   |   |  |   |
                                                       //                |   |   |   |   |  |   |
 // VSTUP230                                                             |   |   |   |   |  |   |
 'S',     cGoNod,               5,     cGoNod,  35,    // 890            | <-+   |   |   |  |   |
 'T',     cGoNod,               5,   cResetIx,   0,    // 895            |   |   |   |   |  |   |
 'U',     cGoNod,               5,   cResetIx,   0,    // 900            |   |   |   |   |  |   |
 'P',     cGoNod,               5,   cResetIx,   0,    // 905            |   |   |   |   |  |   |
 '2',     cGoNod,               5,   cResetIx,   0,    // 910            |   |   |   |   |  |   |
 '3',     cGoNod,               5,   cResetIx,   0,    // 915            |   |   |   |   |  |   |
 '0',  cExitCode,   cCMD_VSTUP230,   cResetIx,   0,    // 920            |   |   |   |   |  |   |
                                                       //                |   |   |   |   |  |   |
 // VYKON                                                                |   |   |   |   |  |   |
 'Y',     cGoNod,               5,  cRptRstIx,  20,    // 925            | <-+   |   |   |  |   |
 'K',     cGoNod,               5,   cResetIx,   0,    // 930            |       |   |   |  |   |
 'O',     cGoNod,               5,   cResetIx,   0,    // 935            |       |   |   |  |   |
 'N',  cExitCode,    cCMD_SKVYKON,   cResetIx,   0,    // 940            |       |   |   |  |   |
                                                       //                |       |   |   |  |   |
 // ZAVOLEJ                                                              |       |   |   |  |   |
 'Z',     cGoNod,               5,   cResetIx,   0,    // 945            |     <-+   |   |  |   |
 'A',     cGoNod,               5,   cResetIx,   0,    // 950            |           |   |  |   |
 'V',     cGoNod,               5,   cResetIx,   0,    // 955            |           |   |  |   |
 'O',     cGoNod,               5,   cResetIx,   0,    // 960            |           |   |  |   |
 'L',     cGoNod,               5,   cResetIx,   0,    // 965            |           |   |  |   |
 'E',     cGoNod,               5,   cResetIx,   0,    // 970            |           |   |  |   |
 'J',   cWaitPar,    cCMD_ZAVOLEJ,   cResetIx,   0,    // 975            |           |   |  |   |
                                                       //                |           |   |  |   |
 // ERRDEF xx                                                            |           |   |  |   |
 'D',     cGoNod,               5,   cResetIx,   0,    // 980            |         <-+   |  |   |
 'E',     cGoNod,               5,   cResetIx,   0,    // 985            |               |  |   |
 'F',   cWaitPar,     cCMD_ERRDEF,   cResetIx,   0,    // 990            |               |  |   |
                                                       //                |               |  |   |
 // MACRO X X X ...                                                      |               |  |   |
 'A',     cGoNod,               5,  cRptRstIx,   0,    // 995            |             <-+  |   |
 'C',     cGoNod,               5,   cResetIx,   0,    // 1000           |                  |   |
 'R',     cGoNod,               5,   cResetIx,   0,    // 1005           |                  |   |
 'O',   cWaitPar,      cCMD_MACRO,   cResetIx,   0,    // 1010           |                  |   |
                                                       //                |                  |   |
 // EEPREADH XXXX                                                        |                  |   |
 'R',     cGoNod,               5,  cRptRstIx,   0,    // 1015           |                <-+   |
 'E',     cGoNod,               5,   cResetIx,   0,    // 1020           |                      |
 'A',     cGoNod,               5,   cResetIx,   0,    // 1025           |                      |
 'D',     cGoNod,               5,   cResetIx,   0,    // 1030           |                      |
 'H',   cWaitPar,   cCMD_EEPREADH,     cGoNod,   5,    // 1035           |                <-+   |
                                                       //                |                  |   |
 // EEPREADA XXXX                                                        |                  |   |
 'A',   cWaitPar,   cCMD_EEPREADA,  cRptRstIx,   0,    // 1040           |                <-+   |
                                                       //                |                      |
 // SMSMOD  X                                                            |                      |
 'M',     cGoNod,               5,   cResetIx,   0,    // 1045           |                    <-+
 'S',     cGoNod,               5,   cResetIx,   0,    // 1050           |
 'M',     cGoNod,               5,   cResetIx,   0,    // 1055           |
 'O',     cGoNod,               5,   cResetIx,   0,    // 1060           |
 'D',   cWaitPar,    cCMD_SMSMODE,  cRptRstIx,   0,    // 1065           |
                                                       //                |
 // VETREZIM                                                             |
 'R',     cGoNod,               5,  cRptRstIx,   0,    // 1070         <-+
 'E',     cGoNod,               5,   cResetIx,   0,    // 1075
 'Z',     cGoNod,               5,   cResetIx,   0,    // 1080
 'I',     cGoNod,               5,   cResetIx,   0,    // 1085
 'M',   cWaitPar,   cCMD_VETREZIM,  cRptRstIx,   0,    // 1090
};








