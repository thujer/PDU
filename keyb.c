//#define KeyboardTest
#define CompileObj

#ifdef CompileObj
#include <dos.h>
#endif

#ifdef KeyboardTest
#include <dos.h>
#include <conio.h>
#endif

#define uchar unsigned char
#define uint unsigned int

uchar Ascii=0;
uchar Scan=0;

#define kbESC        (Scan==1)
#define kbENTER      (Scan==28)
#define kbSPACE      (Scan==57)
#define kbTAB        (Scan==15)
#define kbBCKSPACE   (Scan==14)
#define kbF1         (Scan==59)
#define kbF2         (Scan==60)
#define kbF3         (Scan==61)
#define kbF4         (Scan==62)
#define kbF5         (Scan==63)
#define kbF6         (Scan==64)
#define kbF7         (Scan==65)
#define kbF8         (Scan==66)
#define kbF9         (Scan==67)
#define kbF10        (Scan==68)
#define kbUP         (Scan==0x48)
#define kbDOWN       (Scan==0x50)
#define kbCTRL_LEFT  (Scan==0x73)
#define kbCTRL_RIGHT (Scan==0x74)
#define kbALT_LEFT   (Scan==0x9B)
#define kbALT_RIGHT  (Scan==0x9D)
#define kbLEFT       (Scan==0x4B)
#define kbRIGHT      (Scan==0x4D)
#define kbPGUP       (Scan==0x49)
#define kbPGDN       (Scan==0x51)
#define kbHOME       (Scan==0x47)
#define kbEND        (Scan==0x4F)
#define kbDEL        (Scan==0x53)
#define kbINSERT     (Scan==0x52)
#define kbCTRL_R     ((Ascii==0x12) && (Scan==0x13))
#define kbCTRL_D     ((Ascii==0x04) && (Scan==0x20))
#define kbCTRL_C     ((Ascii==0x03) && (Scan==46))
#define kbCTRL_I     ((Ascii==0x09) && (Scan==23))
#define kbCTRL_N     ((Ascii==0x0E) && (Scan==0x31))
#define kbCTRL_L     ((Ascii==12) &&  (Scan==38))
#define kbCTRL_NUM5  ((Ascii==0x00) && (Scan==143))
#define kbALT_X      ((Ascii==0x00) && (Scan==0x2D))
#define kbALT_P      ((Scan==25) && (Ascii==0))
#define kbALT_S      ((Ascii==0x00) && (Scan==31))
#define kbALT_T      ((Ascii==0) && (Scan==20))
#define kbQ          (Scan==0x10)
#define kbW          (Scan==0x11)
#define kbE          (Scan==0x12)
#define kbR          (Scan==0x13)
#define kbT          (Scan==0x14)
#define kbY          (Scan==0x15)
#define kbU          (Scan==0x16)
#define kbI          (Scan==0x17)
#define kbO          (Scan==0x18)
#define kbP          (Scan==0x19)
#define kbA          (Scan==0x1E)
#define kbS          (Scan==0x1F)
#define kbD          (Scan==0x20)
#define kbF          (Scan==0x21)
#define kbG          (Scan==0x22)
#define kbH          (Scan==0x23)
#define kbJ          (Scan==0x24)
#define kbK          (Scan==0x25)
#define kbL          (Scan==0x26)
#define kbZ          (Scan==0x2C)
#define kbX          (Scan==0x2D)
#define kbC          (Scan==0x2E)
#define kbV          (Scan==0x2F)
#define kbB          (Scan==0x30)
#define kbN          (Scan==0x31)
#define kbM          (Scan==0x32)

//------------------------------------------------------------------------
// Zjisti stisknutou klavesu, pokud neni klavesa stisknuta, ceka.
//------------------------------------------------------------------------
void GetKey()
{ uchar S,A;
  asm{ MOV AH,0x10
       INT 0x16
       MOV A,AL
       MOV S,AH
     }
  Ascii=A;
  Scan=S;
}


//------------------------------------------------------------------------
// Vraci 128, pokud je zapnuty prepisovaci mod  ( stisknuto Insert )
//------------------------------------------------------------------------
uchar Insert()
{ return(peekb(0,0x0417) & 128);
}


//------------------------------------------------------------------------
// Vraci stav klaves SHIFT, 0=zadny
//------------------------------------------------------------------------
uchar Shift()
{ return(peekb(0,0x0417) & 3);
}



#ifdef KeyboardTest
void main()
{ while(1)
  { if(kbhit())
    { GetKey();
      cprintf("Ascii: %Xh",Ascii);
      cprintf("        Scan: %Xh",Scan);
      cprintf("        Char: %c \r\n",Ascii);
      if(kbESC) break;
    }
  }
}
#endif