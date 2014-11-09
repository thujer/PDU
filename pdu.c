#include "stdio.h"
#include "conio.h"
#include "shorttyp.c"      // Zkraceni definice typu
#include "keyb.c"          // Driver klavesnice
#include "ports.c"         // Driver portu
#include "cmds.k"          // Tabulka prikazu
#include "cmd_get.c"       // Sekvencni detekce ID prikazu
#include "times.c"         // Driver timeoutu a systemovy cas
#include "errors.k"        // Definice chybovych statusu


uchar ixOctet;
uchar Octet[2];
uchar bOctetFull;

struct
{ uchar SMSC[15];                // Cislo servisniho strediska
  uchar SMSC_NumberSize;         // Pocet oktetu format cisla + cislo servisniho strediska
  uchar SMSC_NumberFormat;       // Format cisla servisniho strediska
  uchar DeliverPDU;              // Stavove informace SMS
  uchar SenderPNumSize;          // Delka cisla odesilatele
  uchar SenderPNum[15];          // Cislo odesilatele
  uchar SenderPNumFormat;        // Format cisla odesilatele
  uchar PID;                     // Typ zpravy SMS
  uchar DCS;                     // Kodovaci schema zpravy
  uchar Date[8];                 // Datum doruceni zpravy do centra
  uchar TextSize;                // Delka textu SMS pred zakodovanim
} GSM;

#define ixSMSC GSM.SMSC[0]
#define ixSenderPNum GSM.SenderPNum[0]
#define ixDate GSM.Date[0]

uchar CMD;

#define cSMSPDU 0

uchar proc[10];      // Pole ridicich promennych stavovych automatu
uchar (*proc_routine) [10];

uchar RIBUF;
uchar RByte;

ulong PortBase;



// Spusteni sekvencniho procesu
void Proc_Start(uchar ID)
{
  if(ID < sizeof(proc))
  {
    proc[ID]=1;
  }
}


// Nenasilne ukonceni sekvencniho procesu
void Proc_Terminate(uchar ID)
{
  if(ID < sizeof(proc))
  {
    proc[ID]=0xFF;
  }
}



void COM_SendString(uchar *Str)
{
  uchar i;

  i=0;
  while(Str[i])
  {
    COM_SendChar(PortBase, Str[i]);
    i++;
  }
}



// Nacteni jednoho oktetu (2 byte) do bufferu Octet
uchar Octet_GetOne(uchar Ch)
{
  if(IsHex(Ch))                        // a pokud je to hex cislo
  {
    if(ixOctet < 2)                    // a pokud neni oktet naplnen
    {
      Octet[ixOctet]=Ch;               // zapis znak do oktetu
      ixOctet++;
      if(ixOctet >= 2)
        return(0);                     // vrat 0 -> buffer plny
      else
        return(1);                     // vrat 1 -> byte byl nacten, pokracuj
    }
    else
      return(0);                       // vrat 0 -> buffer plny
  }

  if((Ch==0x0D) || (Ch==0x0A))         // Pokud CR/LF, vrat 1 -> pokracuj
  {
    return(1);
  }

  return(0);                           // Vrat 0 -> chybny znak, nebo plny buffer
}


// Prohozeni byte v oktetu
void Octet_Swap()
{
  uchar Temp;

  Temp = Octet[0];
  Octet[0] = Octet[1];
  Octet[1] = Temp;
}


// Init oktetu
void Octet_Reset()
{
  ixOctet=0;
  Octet[0]=0;
  Octet[1]=0;
}



uchar Octet_to_byte()
{
  uchar Out;

  //if(Octet[1]=='F') Octet[1]=0;   // Pokud je F, byl je v oktetu pouze jeden znak
  if(IsNum(Octet[0]))
  {
    Out=((Octet[0]-'0') * 0x10);
  }
  else
  {
    Out=(((Octet[0]-'A')+10) * 0x10);
  }

  if(IsNum(Octet[1]))
  {
    Out+=(Octet[1]-'0');
  }
  else
  {
    Out+=((Octet[1]-'A')+10);
  }


  return(Out);
}


void pSMSPDU_GetOctet()
{
  bOctetFull=0;                          // Nuluj flag naplneneho oktetu
  if(RIBUF)                              // Pokud prijat znak
  {
    if(!Octet_GetOne(RByte))             // Nacti jeden oktet, pokud nebyl nacten je vracena 0
    {
      bOctetFull=1;                      // Nastav flag naplneni oktetu
    }
  }

  if(Timeout(cSMSPDU))                   // Pokud vyprsel timeout
  {
    bOctetFull=1;                        // nastav flag naplneni oktetu
  }
}


/*
07
91
246020099990 420602909909
24
0C
91
246060169215 420606612951
00
00
40505191657008 04051519560708 15.5.2004 19:56:07
6E
41F45B0D6A86E5F4B4BB0C7AEBF3F6701B342F83E06F10FA4D76974164F6BB8E2E80C86F711914669741E33788FE06ADC3E6F22F88
7EAB41CB307D1D4639CBF4FA3CDD06A9CB733A3B0D529741F437885E26E74173B83C6C77E741E3F49CFD4E01
*/




uchar pSMSPDU()
{
  uchar i;

  //printf("\r\npSMSPDU:%i\r\n",proc[cSMSPDU]);

  switch(proc[cSMSPDU])
  {
    case  1: // Init promennych a pokracovat dalsim stavem
             Timeout_Set(cSMSPDU, 3);
             Octet_Reset();

             proc[cSMSPDU]++;
             break;


    case  2: // Zde cekat na prichozi znak, pokud je to 0x0A nebo 0x0D, nevsimej si, pokud to neni cislo, ukonci proces
             // pokud to je cislo, uloz cislo do bufferu a cekej na druhe cislo.
             // Nastav promennou pro odpocet nepotrebnych byte, pokracuj dalsim stavem

             pSMSPDU_GetOctet();                    // Nacti oktet, nastav priznaky, zkontroluj timeout

             if(bOctetFull)
             {
               GSM.SMSC_NumberSize = Octet_to_byte();   // Zapis pocet byte nasledujici informace
               Octet_Reset();                           // Nuluj oktet
               proc[cSMSPDU]++;                         // Pokud oktet naplnen pokracuj dalsim stavem
             }

             if(Timeout(cSMSPDU))                   // Pokud vyprsel timeout
             {
               proc[cSMSPDU]=0xFF;                  // Ukonci proces
             }
             break;

    case  3: // Zde cekej dokud neni naplnen oktet urcujici typ cisla
             pSMSPDU_GetOctet();                            // Nacti oktet, nastav priznaky, zkontroluj timeout
             if(bOctetFull)
             {
               GSM.SMSC_NumberFormat = Octet_to_byte(); // Zapis format cisla centra 81=mezinarodni, 91=narodni
               Octet_Reset();
               ixSMSC=1;                                    // Nuluj index
               proc[cSMSPDU]++;                             // Pokud oktet naplnen pokracuj dalsim stavem
             }
             break;


    case  4: // Zde cekej dokud neni odpocitano cislo centra, pokracuj dalsim stavem
             pSMSPDU_GetOctet();                    // Nacti oktet, nastav priznaky, zkontroluj timeout
             if(bOctetFull)
             {
               if((ixSMSC < (GSM.SMSC_NumberSize*2-1)) && (ixSMSC < (sizeof(GSM.SMSC)-1)))
               {
                 Octet_Swap();                   // Prohozeni byte v oktetu
                 GSM.SMSC[ixSMSC  ]=Octet[0];    // Zapis znak do cisla centra
                 GSM.SMSC[ixSMSC+1]=Octet[1];
                 ixSMSC+=2;                      // Posun index o dve pozice
                 Octet_Reset();                  // Nuluj oktet

                 if(ixSMSC >= (GSM.SMSC_NumberSize*2-1))
                 {
                   proc[cSMSPDU]++;        // Pokud oktet naplnen pokracuj dalsim stavem
                 }
               }
               else
               {
                 Octet_Reset();          // Nuluj oktet
                 proc[cSMSPDU]++;        // Pokud oktet naplnen pokracuj dalsim stavem
               }
             }
             break;

    case  5: // Cekej na dalsi oktet, uloz ho jako status SMS (stavove informace zpravy), pokracuj dalsim stavem
             pSMSPDU_GetOctet();                    // Nacti oktet, nastav priznaky, zkontroluj timeout

             if(bOctetFull)
             {
               GSM.DeliverPDU = Octet_to_byte();    // Zapis stavove informace SMS
               Octet_Reset();                       // Nuluj oktet
               proc[cSMSPDU]++;                     // Pokud oktet naplnen pokracuj dalsim stavem
             }
             break;

    case  6: // Cekej na dalsi oktet, udava pocet znaku nasledujiciho telefonniho cisla, zkopiruj jej do promenne
             pSMSPDU_GetOctet();                     // Nacti oktet, nastav priznaky, zkontroluj timeout

             if(bOctetFull)
             {
               GSM.SenderPNumSize = Octet_to_byte();  // Zapis stavove informace SMS
               Octet_Reset();                         // Nuluj oktet
               proc[cSMSPDU]++;                       // Pokud oktet naplnen pokracuj dalsim stavem
             }
             break;

    case  7: // Cekej na dalsi oktet, udava format nasledujiciho telefonniho cisla, zkopiruj jej do promenne
             pSMSPDU_GetOctet();                       // Nacti oktet, nastav priznaky, zkontroluj timeout

             if(bOctetFull)
             {
               GSM.SenderPNumFormat = Octet_to_byte(); // Zapis format cisla odesilatele
               ixSenderPNum=1;                         // Nastav index cisla odesilatele na zacatek bufferu
               Octet_Reset();                          // Nuluj oktet
               proc[cSMSPDU]++;                        // Pokud oktet naplnen pokracuj dalsim stavem
             }
             break;

    case  8: // Ukladej a odpocitavej jednotlive znaky (ne oktety) cisla odesilatele,
             // odfiltruj pripadne posledni F, po odpocteni pokracuj dalsim stavem

             pSMSPDU_GetOctet();                    // Nacti oktet, nastav priznaky, zkontroluj timeout
             if(bOctetFull)
             {
               if((ixSenderPNum < GSM.SenderPNumSize) && (ixSenderPNum < (sizeof(GSM.SenderPNum)-1)))
               {
                 Octet_Swap();                         // Prohozeni byte v oktetu
                 GSM.SenderPNum[ixSenderPNum  ]=Octet[0];    // Zapis znak do cisla centra
                 GSM.SenderPNum[ixSenderPNum+1]=Octet[1];
                 ixSenderPNum+=2;                      // Posun index o dve pozice
                 Octet_Reset();

                 if(ixSenderPNum >= GSM.SenderPNumSize)
                 {
                   proc[cSMSPDU]++;        // Pokud oktet naplnen pokracuj dalsim stavem
                 }
               }
               else
               {
                 Octet_Reset();
                 proc[cSMSPDU]++;        // Pokud oktet naplnen pokracuj dalsim stavem
               }
             }
             break;

    case  9: // Cekej na dalsi oktet, je to Byte PID, urcuje typ zpravy
             //  00h - obyèejná SMS zpráva (implicitní hodnota)
             //  01h - telex
             //  02h - fax (skupina3)
             //  03h - fax (skupina 4)
             //  04h - normální telefon (tj. konverze do hlasu)
             // pokracuj dalsim stavem

             pSMSPDU_GetOctet();                       // Nacti oktet, nastav priznaky, zkontroluj timeout

             if(bOctetFull)
             {
               GSM.PID = Octet_to_byte();              // Typ zpravy SMS
               Octet_Reset();                          // Nuluj oktet
               proc[cSMSPDU]++;                        // Pokud oktet naplnen pokracuj dalsim stavem
             }
             break;

    case 10: // Cekej na dalsi oktet, urcuje kodovani textu SMS
             //  DCS (Data Coding Scheme) urèuje kódovací schéma dat. Napø.:
             //  00h - 7 bitová výchozí abeceda
             //  F6h - 8 bitové datové kódování dle Class 2
             // pokracuj dalsim stavem

             pSMSPDU_GetOctet();                       // Nacti oktet, nastav priznaky, zkontroluj timeout

             if(bOctetFull)
             {
               GSM.DCS = Octet_to_byte();              // Kodovaci schema textu SMS
               Octet_Reset();                          // Nuluj oktet
               ixDate=1;
               proc[cSMSPDU]++;                        // Pokud oktet naplnen pokracuj dalsim stavem
             }
             break;

    case 11: // Dalsi oktety jsou
             //  10210390606504
             //  SCTS - definuje datum doruèení SMS do SMSCentra. Jednotlivé dvojce èísel, zleva doprava urèují:
             //  rok, mìsíc, den, hodinu, minutu, sekundu, èasovou zónu.
             //  Èísla ve dvojicích jsou opìt prohozena (swap). Pro ukázku platí: 30.12.2001, 09:06:56. Údaj v poslední dvojici èísel urèuje rozdíl ve ètvrt hodinách mezi místním èasem a GTM (Greenwich Main Time).
             // pokracuj dalsim stavem

             pSMSPDU_GetOctet();                    // Nacti oktet, nastav priznaky, zkontroluj timeout
             if(bOctetFull)
             {
               if(ixDate < sizeof(GSM.Date))
               {
                 Octet_Swap();                   // Prohozeni byte v oktetu
                 GSM.Date[ixDate  ]=Octet[0];    // Zapis znak do cisla centra
                 GSM.Date[ixDate+1]=Octet[1];
                 ixDate+=2;                      // Posun index o dve pozice
                 Octet_Reset();

                 if(ixDate >= sizeof(GSM.Date))
                 {
                   proc[cSMSPDU]++;        // Pokud oktet naplnen pokracuj dalsim stavem
                 }
               }
               else
               {
                 Octet_Reset();
                 proc[cSMSPDU]++;        // Pokud oktet naplnen pokracuj dalsim stavem
               }
             }
             break;

    case 12: // Dalsi oktet je
             //  UDL - udává poèet znakù v následující zprávì pøed jejím zakódováním.
             //  Tzn. že pokud je použito výchozí 7 - bitové kódování tak mùže být poèet
             //  bytù menší než poèet znakù, které udává UDL. Pøi 8 - bitovém kódování
             //  je poèet bytù a znakù ve zprávì totožný

             proc[cSMSPDU]++;
             break;

    case 13: // Dalsi oktety jsou jiz samotny text zpravy
                 printf("\r\n\nSMS info:\r\n");
                 printf("  SizeofSMSC:       %d\r\n",GSM.SMSC_NumberSize);
                 printf("  SMSNumFormat:     %Xh\r\n",GSM.SMSC_NumberFormat);
                 printf("  SMSC:             ");
                   for(i=1; i<(GSM.SMSC_NumberSize*2); i++)
                   {
                     printf("%c",GSM.SMSC[i]);
                   }
                 printf("\r\n");
                 printf("  DeliverPDU:       %Xh\r\n",GSM.DeliverPDU);
                 printf("  SenderPNumSize:   %d\r\n",GSM.SenderPNumSize);
                 printf("  SenderPNumFormat: %Xh\r\n",GSM.SenderPNumFormat);
                 printf("  SenderPNum:       ");
                   for(i=1; i<(GSM.SenderPNumSize+1); i++)
                   {
                     printf("%c",GSM.SenderPNum[i]);
                   }
                 printf("\r\n");
                 printf("  MessageType:      %Xh\r\n",GSM.PID);
                 printf("  CodingScheme:     %Xh\r\n",GSM.DCS);
                 printf("  Date:             ");
                   for(i=1; i<ixDate; i++)
                   {
                     printf("%c ",GSM.Date[i]);
                   }
                 printf("\r\n\n");
             proc[cSMSPDU]=0xFF;
             break;

    case 0xFF: proc[cSMSPDU]=0;
               printf("\r\nPDUSMS terminated.\r\n");
               break;

    default: proc[cSMSPDU]=0; break;
  }
  return(0);
}



void PDUSMS_Init()
{
  uchar i;

  for(i=0; i<sizeof(GSM.SMSC); i++) GSM.SMSC[i]=0;
  GSM.SMSC_NumberSize=0;
  GSM.SMSC_NumberFormat=0;
  GSM.DeliverPDU=0;
  GSM.SenderPNumSize=0;
  for(i=0; i<sizeof(GSM.SenderPNum); i++) GSM.SenderPNum[i]=0;
  GSM.SenderPNumFormat=0;
}



void Proc_Runtime()
{
  if(proc[cSMSPDU]) pSMSPDU();
}


void main_init()
{
  ixOctet=0;
  RIBUF=0;
  CMD=0;
}




void main()
{
  clrscr();

  main_init();

  PortBase=COM_GetBase(cCOM1,0);  // Nacti adresu portu COM1
  COM_SetSpeed(PortBase, 19200);  // Nastav rychlost portu

  printf("Extract PDU data\r\nAltX for Exit\r\n\nF1...Start PDUSMS\r\nF2...Read SMS 2\r\nF10...Terminate PDUSMS process\n");

  while(!kbALT_X)
  {
    RIBUF=COM_CharReady(PortBase);        // Zkopiruj flag cekajiciho znaku na portu

    if(kbhit())
    { GetKey();

      if(kbF1) Proc_Start(cSMSPDU);       // Spusteni sekvencniho procesu
      if(kbF10) Proc_Terminate(cSMSPDU);  // Ukonceni procesu

      if(kbF2)
      { if(COM_SendReady(PortBase))
        {
          COM_SendString("at+cmgr=2\r");
        }
      }
    }

    if(RIBUF)
    {
      RByte=COM_GetChar(PortBase);         // Nacti znak z portu

      printf("%c",RByte);

      CMD = CMD_TestProc(CMDs, RByte);     // Detekuj sekvencni prikaz
      switch(CMD)
      {
        case cCMD_OK:   cprintf("ReturnedOK!!!\r\n"); break;
        case cCMD_CMGR: Proc_Start(cSMSPDU); break;
        case cCMD_MENU: cprintf("\r\n!MENU!\r\n"); break;
      }
    }
    Proc_Runtime();
    Time_Proc();
  }
}
