#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "shorttyp.c"    // Zkraceni definice uchar uint a ulong
#include "strutils.c"    // Utility pro praci s retezci
#include "time_def.k"    // Definice

//#define DEBUG_MODE

#define CyclesTo1s 100

struct
{ uchar Year;
  uchar Month;
  uchar Day;
  uchar Hour;
  uchar Min;
  uchar Sec;
} Time;

uint Timeouts[No_Timeouts];   // Definice pro obsluhu casoveho limitu

uchar Timeout_1Sec;

uchar T2CON=0;       // Timer control
uchar TR2=1;         // Start citace
uchar ET2=1;         // Timer 2 - povoleni preruseni
uchar TF2=0;         // Nulovani priznaku pro vznik preruseni pretecenim casovace
uchar EXF2=0;        // Nulovani priznaku pro vznik preruseni
uchar RCAP2L=0xFF;   // Timer 2 control low      (FFFFh - 7800h)
uchar RCAP2H=0x87;   // Timer 2 control high
uchar int_called=0;  // Flag volani preruseni



/*=========================================================================*/
/*                       Interrupt - Timer 2                               */
/*-------------------------------------------------------------------------*/
//#pragma NOAREGS
void int_ct2()// interrupt 5
{ uchar i;

  //if(TF2)
  { for(i=0; i<No_Timeouts; i++)           // Odpocitavej timeouty
    { if(Timeouts[i])
      { Timeouts[i]--;
      }
    }

    if(Timeout_1Sec) Timeout_1Sec--;      // Odpocitavej 1s
    else
    { Timeout_1Sec=CyclesTo1s;            // Napln citac pro 1s
      Time.Sec++;                         // Pocitej cas
      if(Time.Sec>59)
      { Time.Sec=0; Time.Min++;
        if(Time.Min>59)
        { Time.Min=0; Time.Hour++;
          if(Time.Hour>23)
          { Time.Hour=0; Time.Day++;
            if(Time.Day>30)
            { Time.Day=0; Time.Month++;
              if(Time.Month>12)
              { Time.Month=0; Time.Year++;
              }
            }
          }
        }
      }
    }
    TF2=0;
  }
  EXF2=0;
}
//#pragma AREGS


/*=========================================================================*/
/*                          Nastaveni timeoutu                             */
/*-------------------------------------------------------------------------*/
void Timeout_Set(uchar Counter, uint Time)
{ if(Counter<No_Timeouts) Timeouts[Counter]=Time;
}


uchar Timeout(uchar Counter)
{ if(Timeouts[Counter]) return(0); else return(1);
}


uint Timeout_Value(uchar Counter)
{ return((uint) Timeouts[Counter]);
}


// Nastaveni parametru a start casovace 2
void Time_Init()
{ uchar i;

  RCAP2L=0xFF;   // Timer 2 control low      (FFFFh - 7800h)
  RCAP2H=0x87;   // Timer 2 control high

  Timeout_1Sec=CyclesTo1s;   // Napln citac pro 1s
  for(i=0; i<No_Timeouts; i++) Timeouts[i]=0;

  // Pri 6-taktovem rezimu, f osc=18432Mhz a hodnote citace 7800h
  // probiha preruseni kazdych 10ms, pokud se tedy nastavi hodnota
  // timeoutu 100, je odpocitavana doba 1s

  T2CON=0;       // Timer control
  TR2=1;         // Start citace
  ET2=1;         // Timer 2 - povoleni preruseni
  TF2=0;         // Nulovani priznaku pro vznik preruseni pretecenim casovace
  EXF2=0;        // Nulovani priznaku pro vznik preruseni

  Time.Day=0;
  Time.Month=0;
  Time.Year=0;
  Time.Sec=0;
  Time.Min=0;
  Time.Hour=0;
}



// Nastaveni systemovych hodin
void Time_SetTime(uchar Hour,uchar Min,uchar Sec)
{ Time.Hour=Hour;
  Time.Min=Min;
  Time.Sec=Sec;
}

// Nastaveni systemoveho data
void Time_SetDate(uchar Year,uchar Month,uchar Day)
{ Time.Year=Year;
  Time.Month=Month;
  Time.Day=Day;
}

// Zjisteni aktualni hodiny
uchar Time_GetHour()
{ return(Time.Hour);
}

// Zjisteni aktualni minuty
uchar Time_GetMin()
{ return(Time.Min);
}

// Zjisteni aktualni sekundy
uchar Time_GetSec()
{ return(Time.Sec);
}

// Zjisteni aktualniho roku
uchar Time_GetYear()
{ return(Time.Year);
}

// Zjisteni aktualniho mesice
uchar Time_GetMonth()
{ return(Time.Month);
}

// Zjisteni aktualniho dne
uchar Time_GetDay()
{ return(Time.Day);
}

// Nastaveni aktualni hodiny
void Time_SetHour(uchar Hour)
{ Time.Hour=Hour;
}

// Nastaveni aktualni minuty
void Time_SetMin(uchar Min)
{ Time.Min=Min;
}

// Nastaveni aktualni sekundy
void Time_SetSec(uchar Sec)
{ Time.Sec=Sec;
}

// Nastaveni aktualniho roku
void Time_SetYear(uchar Year)
{ Time.Year=Year;
}

// Nastaveni aktualniho mesice
void Time_SetMonth(uchar Month)
{ Time.Month=Month;
}

// Nastaveni aktualniho dne
void Time_SetDay(uchar Day)
{ Time.Day=Day;
}



//void Time_CreateReport(uchar ID)
//{ uchar i;
//
//  switch(ID)
//  { case cTime_TimeoutsReport:
//      Report_ClearBuf();
//      Str_Copy("Timeouts: ", Report_Buf); Report_ix=10;
//      for(i=0; i<No_Timeouts; i++)
//      { Str_Copy("    ",Report_Buf+(i*4)+Report_ix);
//        Str_NumToStr(Report_Buf+(i*4)+Report_ix,Timeouts[i]);
//      }
//      break;
//
//    case cTime_SysTimeReport:
//      sprintf(Report_Buf+Report_ix,"Systemovy cas: %.2i:%.2i:%.2i ",(int)Time.Hour,(int)Time.Min,(int)Time.Sec);
//      Report_Add();
//      sprintf(Report_Buf+Report_ix,"%.2i/%.2i/%.2i",(int)Time.Year,(int)Time.Month,(int)Time.Day);
//      Report_Add();
//      break;
//  }
//}


void Time_Proc()
{
  if(peekb(0x0000,0x046C) & 16)   // Pokud je nastaven 4.bit systemoveho citace
  {
    if(!int_called)               // a pokud jeste nebylo volano preruseni
    { int_ct2();                  // Volani casoveho "preruseni"
      int_called=1;               // nastav flag volaneho preruseni
    }
  }
  else
  {
    int_called=0;                 // shod flag volaneho preruseni
  }
}



// void main()
// {
//   while(1)
//   {
//     Time_Proc();
//
//   }
// }
