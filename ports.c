#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif

// ID portu
#define cCOM1 0
#define cCOM2 1
#define cCOM3 2
#define cCOM4 3


uchar COM_RecBuf[64];  // Buffer pro prichozi znaky
uchar COM_RBW_ix=0;    // index pro zapis do RecBuf
uchar COM_RBR_ix=0;    // index pro cteni z RecBuf


//void COM_Intr()
//{ if(COM_RBW_ix!=COM_RBR_ix)
//  { if(COM_CharReady(Config.COM_AdresaPortu))
//    { if(COM_RBW_ix < sizeof(COM_RecBuf))
//      { COM_RecBuf[COM_RBW_ix]=inp(Config.COM_AdresaPortu);
//        COM_RBW_ix++;
//        COM_RBW_ix=COM_RBW_ix % sizeof(COM_RecBuf);
//      }
//    }
//  }
//}


//------------------------------------------------------------------------
// Zjisti zakladni adresu COM portu
//------------------------------------------------------------------------
uint COM_GetBase(uchar Port, uint UserPortAdr)
{ switch(Port)
  { case 0: return(peek(0,0x0400));   // default 0x3F8;
    case 1: return(peek(0,0x0402));   // default 0x2F8;
    case 2: return(peek(0,0x0404));   // default 0x3E8;
    case 3: return(peek(0,0x0406));   // default 0x2E8;
    case 4: return(UserPortAdr);
    default: return(0);
  }
}


//------------------------------------------------------------------------
// Nastavi prenosovou rychlost aktualniho portu
//------------------------------------------------------------------------
void COM_SetSpeed(uint PortBase, ulong Baud)
{ uchar D0,D1;

  D0=(uchar) ((115200 / Baud) & 0xFF);        // Dolni byte
  D1=(uchar) (((115200 / Baud) >> 8) & 0xFF); // Horni byte

  outport(PortBase+3,128);   // Nastavi Zapis do reg. Adr+1
  outport(PortBase  ,D0);    // Zapis dolniho byte delitele ryhlosti
  outport(PortBase+1,D1);    // Zapis horniho byte delitele ryhlosti
  outport(PortBase+3,3);
};


//------------------------------------------------------------------------
// Nastaveni parametru prenosu
// ----------------------------
// bit 0..1   Delka slova 00=5, 01=6, 10=7, 11=8 bitu
// bit 2      Stop bity 0=1, 1=2
// bit 3..4   Parita x0=None, 01=Licha, 11=Suda
// bit 5      Parita znaku (BIOS nepouziva)
// bit 6      možné øízení BREAK; 1=zaèátek vysílání 0s
// bit 7      DLAB (pøístupový bit k registru dìlièky)
//            - urèuje mód portù 03F8H a 03F9H pro nast.
//            (pokud je DLAB=1, je pøístup k dìlièce)
//
//------------------------------------------------------------------------
void COM_SetProperties(uint PortBase, uchar PortSet)
{ outport(PortBase+3,PortSet);
}

// Nastaveni preruseni portu
// ----------------------------
//03F9H  - zápis: registr povolení pøerušení (DLAB=1)
//                ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿
//                ³7 6 5 4 3 2 1 0³
//                ÀÂÁÂÁÂÁÂÁÂÁÂÁÂÁÂÙ bit
//                 ³ ³ ³ ³ ³ ³ ³ ÀÄ> 0: 1=možné pøerušení pøi pøíjmu dat
//                 ³ ³ ³ ³ ³ ³ ÀÄÄÄ> 1: 1=možné pøer. pøi prázdném vys. buff.
//                 ³ ³ ³ ³ ³ ÀÄÄÄÄÄ> 2: 1=možné pøer. stavem pøijímací linky
//                 ³ ³ ³ ³ ÀÄÄÄÄÄÄÄ> 3: 1=možné pøer. stavem modemu
//                 ³ ³ ³ ÀÄÄÄÄÄÄÄÄÄ> 4: vždy = 0
//                 ³ ³ ÀÄÄÄÄÄÄÄÄÄÄÄ> 5: vždy = 0
//                 ³ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄ> 6: vždy = 0
//                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ> 7: vždy = 0
//       - zápis: registr   dìlièky  -  vyšší  bajt  (pokud  DLAB=1,  tj.  po
//                instrukci  OUT  03FBH,80H).  Po operaci OUT 03FBH,80H tento
//                port  obsahuje  vyšší bajt dìlièky taktu. Spoleènì s nižším
//                bajtem   (port   03F8H)  tvoøí  16-bitovou  hodnotu,  která
//                nastavuje rychlost pøenosu dat.
void COM_SetInterrupt(uint PortBase, uchar PortSet)
{ uchar backup;
  backup=inp(PortBase+3);                  // Ulozeni nastaveni portu
  outport(PortBase+3,inp(PortBase+3) & (0xFF-0x80)); // Nastaveni DLAB=0;
  outport(PortBase+1,PortSet);                // Nastaveni preruseni portu
  outport(PortBase+3,backup);                 // Obnoveni nastaveni portu
}



//03FAH  - ètení: identifikaèní  registr  pøerušení. Pokud nastane pøerušení,
//                ètìte  tento  registr,  pokud  chcete zjistit, co zpùsobilo
//                pøerušení.
//                ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿
//                ³7 6 5 4 3 2 1 0³
//                ÀÂÁÂÁÂÁÂÁÂÁÂÁÂÁÂÙ  bit
//                 ³ ³ ³ ³ ³ ³ ³ ÀÄ>  0:  1=nenastalo žádné pøerušení
//                 ³ ³ ³ ³ ³ ÀÄÁÄÄÄ> 1-2: 00=pøerušení stavem pøijímací linky
//                 ³ ³ ³ ³ ³                 vzniká  pøi pøeteèení, paritì,
//                 ³ ³ ³ ³ ³                 chybì rámu nebo pøi pøerušení.
//                 ³ ³ ³ ³ ³                 Resetování ètením stavu linky
//                 ³ ³ ³ ³ ³                 (port 03FDH)
//                 ³ ³ ³ ³ ³              01=pøijatá data platná (resetování
//                 ³ ³ ³ ³ ³                 ètením pøijím. bufferu 03F8H)
//                 ³ ³ ³ ³ ³              10=vysílací buffer prázdný (reset
//                 ³ ³ ³ ³ ³                 zápisem do vysíl. bufferu 03F8H)
//                 ³ ³ ³ ³ ³              11=stav modemu. Vzniká pøi zmìnì
//                 ³ ³ ³ ³ ³                 signálù CTS, DSR, RI nebo RLSD.
//                 ³ ³ ³ ³ ³                 Reset ètením stavu modemu 03FEH.
//                 ³ ³ ³ ³ ÀÄÄÄÄÄÄÄ>  3:  vždy = 0
//                 ³ ³ ³ ÀÄÄÄÄÄÄÄÄÄ>  4:  vždy = 0
//                 ³ ³ ÀÄÄÄÄÄÄÄÄÄÄÄ>  5:  vždy = 0
//                 ³ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄ>  6:  vždy = 0
//                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ>  7:  vždy = 0
//
uchar COM_GetInterrupt(uint PortBase)
{ return(inport(PortBase+2));
}


//------------------------------------------------------------------------
// Aktivace (Status=1) / Deaktivace (Status=0) signalu DTR
//------------------------------------------------------------------------
void COM_SetDTR(uint PortBase, uchar Status)
{ if(Status) outport(PortBase+4,inp(PortBase+4) | 1);
  else outport(PortBase+4,inp(PortBase+4) & (0xFF-1));   // Aktivace DTR
}


//03FDH  - ètení: stavový registr linky
//         -----------------
//          7 6 5 4 3 2 1 0
//         ----------------- bit
//          | | | | | | | +-> 0: 1=data pøipravena (DR) (reset ètením dat)
//          | | | | | | +---> 1: 1=chyba pøeteèení (OE): pøedch. znak ztracen
//          | | | | | +-----> 2: 1=chyba parity (PE): reset ètením st.linky
//          | | | | +-------> 3: 1=chyba rámu (FE): špatný stop-bit ve znaku
//          | | | +---------> 4: 1=indikace break (BI): pøijata dlouhá mezera
//          | | +-----------> 5: 1=vysílací registr prázdný (OK k vysílání)
//          | +-------------> 6: 1=vysílaè prázdný. Nevysílá žádná data.
//          +---------------> 7: vždy = 0
//         Pozn.: Pøi povoleném pøerušení (03F9H) zpùsobí bity 1-4 pøerušení.
//
uchar COM_GetError(uint PortBase)
{ return(PortBase+5);
}


//------------------------------------------------------------------------
// Vraci 1 pokud je port pripraven vyslat dalsi znak
//------------------------------------------------------------------------
uchar COM_SendReady(uint PortBase)  // 0 = Vysilac je prazdny a pripraven vysilat
{ if((inp(PortBase+5)&(32+64))==(32+64)) return(1);
  else return(0);
}



//------------------------------------------------------------------------
// Vraci 1 pokud port prijal dalsi znak
//------------------------------------------------------------------------
uchar COM_CharReady(uint PortBase)     // 1 = Znak byl prijat
{ if((inp(PortBase+5) & 1)==1) return(1);
  else return(0);
}


//------------------------------------------------------------------------
// Vraci prijaty znak
//------------------------------------------------------------------------
uchar COM_GetChar(uint PortBase)
{ return(inp(PortBase));
}

//------------------------------------------------------------------------
// Odesle znak
//------------------------------------------------------------------------
uchar COM_SendChar(uint PortBase, uchar Byte)
{ return(outp(PortBase, Byte));
}

//------------------------------------------------------------------------
//  Nacte status portu
//------------------------------------------------------------------------
uchar COM_GetPortStatus(uint PortBase)
{ return(inp(PortBase+5));
}


//------------------------------------------------------------------------
// Pri ztrate predchoziho znaku vraci 2
//------------------------------------------------------------------------
uchar COM_OverrunError(uint PortBase)
{ return(COM_GetPortStatus(PortBase) & 2);
}


//------------------------------------------------------------------------
// Pri chybne parite vraci 4
//------------------------------------------------------------------------
uchar COM_ParityError(uint PortBase)
{ return(COM_GetPortStatus(PortBase) & 4);
}


//------------------------------------------------------------------------
// Pri chybnem stopbitu vraci 8
//------------------------------------------------------------------------
uchar COM_StopbitError(uint PortBase)
{ return(COM_GetPortStatus(PortBase) & 8);
}


//------------------------------------------------------------------------
// Pokud je prijmana trvala nula, vraci 16
//------------------------------------------------------------------------
uchar COM_AlwaysZero(uint PortBase)
{ return(COM_GetPortStatus(PortBase) & 16);
}


//3faH  Zápis:  *** pouze 16550 *** Rídící registr fronty FIFO
//      (FIFO = First in, first out, tedy první dovnitr, první ven)
//      +7-6-5-4-3-2-1-0+
//      ¦   ¦0 0 0¦ ¦ ¦ ¦
//      +-+-------------+ bit
//        ¦        ¦ ¦ +-? 0: 1=povolení FIFO prijímace a vysílace
//        ¦        ¦ ¦        0=výmaz prijímací a vysílací fronty, znakový režim
//        ¦        ¦ +---? 1: 1=reset prijímacích FIFO registru
//        ¦        +-----? 2: 1=reset vysílacích FIFO registru
//        +--------------? 7-6: Nastavení FIFO registru 00 = 1 byte
//                                                      01 = 4 byty
//                                                      10 = 8 bytu
//                                                      11 = 14 bytu
void COM_SetFIFO(uint PortBase, uchar Status)
{ outport(PortBase+2,Status);
}

//**************************************************************************


//------------------------------------------------------------------------
// Zjisti zakladni adresu LPT portu
//------------------------------------------------------------------------
uint LPT_GetBase(uchar Port, uint UserPortAdr)
{ switch(Port)
  { case 0: return(peek(0,0x0408));   // default 0x378;
    case 1: return(peek(0,0x040A));   // default 0x278;
    case 2: return(peek(0,0x040C));   // default 0x3BC;
    case 3: return(peek(0,0x040E));   // default 0x2BC;
    case 4: return(UserPortAdr);
    default: return(0);
  }
}


//------------------------------------------------------------------------
// Precteni byte z portu
//------------------------------------------------------------------------
uchar LPT_ReadByte(uint PortBase)
{ return(inport(PortBase));
}

void LPT_SendByte(uint PortBase,uchar B)
{ outport(PortBase, B);
}

uchar LPT_ReadInputs1(uint PortBase)
{ return(inport(PortBase+1));
}

uchar LPT_ReadInputs2(uint PortBase)
{ return(inport(PortBase+2));
}

uchar LPT_Read_ECR(uint PortBase)
{ return(inport(PortBase+0x402));
}

uchar LPT_Read_ecpDFifo(uint PortBase)
{ return(inport(PortBase+0x400));
}

uchar LPT_Read_dcr(uint PortBase)
{ return(inport(PortBase+2));
}

uchar LPT_Read_CnfgA(uint PortBase)
{ return(inport(PortBase+0x400));
}

uchar LPT_Read_CnfgB(uint PortBase)
{ return(inport(PortBase+0x401));
}

uchar LPT_ReadDMASet(uint PortBase)
{ return(LPT_Read_ECR(PortBase) & 8);
}

uchar LPT_ReadDMAIntr(uint PortBase)
{ return(LPT_Read_ECR(PortBase) & 4);
}

uchar LPT_ReadDirection(uint PortBase)
{ return(LPT_Read_dcr(PortBase) & 32);
}

void LPT_Set_ECR(uint PortBase,uchar B)
{ outport(PortBase+0x402,B);
}

void LPT_Set_dcr(uint PortBase,uchar B)
{ outport(PortBase+2,B);
}

void LPT_Set_CnfgA(uint PortBase,uchar B)
{ outport(PortBase+0x400, B);
}

void LPT_Set_CnfgB(uint PortBase,uchar B)
{ outport(PortBase+0x401, B);
}


// Zjisteni aktualniho rezimu LPT
// ---------------------------------------------------
// 0: Standard LPT - single direction
// 1: PS/2 bi-direction (set by dcr.direction
// 2: LPT with FIFO (dcr.direction must be LOW
// 3: ECP mode - bi-direction
// 4: Reserved ?
// 5: Reserved ?
// 6: Testing mode
// 7: Config mode - cnfgA and cnfgB is enabled
// ---------------------------------------------------
uchar LPT_ReadMode(uint PortBase)
{ return((LPT_Read_ECR(PortBase) & (32+64+128)) >> 5);
}

void LPT_SetMode(uint PortBase, uchar Mode)
{ LPT_Set_ECR(PortBase,((LPT_Read_ECR(PortBase) & 31) | (Mode << 5)));
}



//**************************************************************************


// Hodnota obdržená instrukcí IN z portu 0201H má strukturu:
//
//        -----------------
//         7 6 5 4 3 2 1 0
//        ----------------- bit
//         | | | | | | | +-> 0: Ax -+
//         | | | | | | +---> 1: Ay  +-> souøadnice (odporové, èasovì
//         | | | | | +-----> 2: Bx  |               závislé vstupy)
//         | | | | +-------> 3: By -+
//         | | | +---------> 4: A1 -+
//         | | +-----------> 5: A2  +-> tlaèítka/spouštìní (èíslicové vstupy)
//         | +-------------> 6: B1  |
//         +---------------> 7: B2 -+
//
//    Oznaèení:  A=joystick  A;  B=joystick  B; x,y = souøadnice X a Y; 1,2 =
//               tlaèítka joysticku 1 a 2.
//
uchar GamePort_ReadButtons()
{ asm{ mov       dx,201h         // adresa portu
       out       dx,al           // inicializace øízení (AL=cokoliv)
       in        al,dx           // ètení bitù 4-7 tlaèítek (0=stisk)
     }
  return(_AL);
}


uchar GamePort_ReadDA()
{
//  asm{     mov       dx,0201h            ; adresa portu
//           out       dx,al               ; inicializace øízení (AL=cokoliv)
//           mov       cx,-1               ; nastavení èítaèe odporu
//      opet:in        al,dx               ; ètení nastavení
//           inc       cx                  ; èítaè
//           test      al,1                ; pøešel již bit souøadnice X na 0 ?
//           jnz       opet                ; èekání dokud je bit souø. X = 1
//     }
//
  return(_AL);
}












