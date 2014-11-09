#include "shorttyp.c"


/*========================================================================*/
/*                         Prevod znaku na velky                          */
/*------------------------------------------------------------------------*/
/* Standarni fuknce                                                       */
/*------------------------------------------------------------------------*/
uchar Str_UpCase(uchar Ch)
{ if((Ch>='a') && (Ch<='z')) Ch&=(255-32);
  return(Ch);
}


/*========================================================================*/
/*                         Porovnani retezce                              */
/*------------------------------------------------------------------------*/
/* Standarni fuknce                                                       */
/*------------------------------------------------------------------------*/
/* Pokud si Bytes znaku v retezcich Ptr1 a Ptr2 odpovida vrati 1          */
/*------------------------------------------------------------------------*/
uchar Str_Cmp(uchar *Ptr1, uchar *Ptr2, uchar Bytes)
{ uchar i;
  uchar Out;

  Out=1;
  for(i=0; i<Bytes; i++) if(Str_UpCase(Ptr1[i])!=Str_UpCase(Ptr2[i])) Out=0;
  return(Out);
}

/*========================================================================*/
/*                        Kopirovani retezce                              */
/*------------------------------------------------------------------------*/
/* Standarni fuknce                                                       */
/*------------------------------------------------------------------------*/
/* Kopirovani retezce Source ukonceneho #0 do Dest, vrati pocet           */
/* zkopirovanych byte                                                     */
/*------------------------------------------------------------------------*/
uchar Str_Copy(uchar *Source, uchar *Dest)
{ uchar i;

  i=0;
  while(Source[i])
  { Dest[i]=Source[i];
    i++; if(i>160) break;
  }
  return(i);
}


/*========================================================================*/
/*                        Kopirovani pameti                               */
/*------------------------------------------------------------------------*/
/* Standarni fuknce                                                       */
/*------------------------------------------------------------------------*/
/* Kopirovani Bytes byte z Source do Dest                                 */
/*------------------------------------------------------------------------*/
void Str_MemCopy(uchar *Source, uchar *Dest, uchar Bytes)
{ uchar i;
  for(i=0; i<Bytes; i++) Dest[i]=Source[i];
}


/*========================================================================*/
/*                       Prevod cisla do textu                            */
/*------------------------------------------------------------------------*/
/* Standarni fuknce                                                       */
/*------------------------------------------------------------------------*/
/* Prevede cislo N do textu - na pointer *Buf, cislo muze byt typu long   */
/*------------------------------------------------------------------------*/
void Str_NumToStr(uchar *Buf, ulong N)
{ uchar Ix,i;
  ulong Nasobek;

  Ix=0; Nasobek=10000000;
  for(i=0; i<7; i++)
  { if(N>=Nasobek) { Buf[Ix]=((N / Nasobek) % 10) + '0'; Ix++; }
    Nasobek/=10;
  }
  Buf[Ix]=((N / 1) % 10) + '0'; Ix++;
}


// Je-li znak cislo, pismeno, nebo znamenko vrati 1
uchar Str_IsPrint(uchar Char)
{ if(((Char >= 'A') && (Char <= 'Z')) ||
     ((Char >= 'a') && (Char <= 'z')) ||
     ((Char >= '0') && (Char <= '9')) ||
     (Char=='+') || (Char=='-') || (Char=='#') ||
     (Char==' ') || (Char=='(') || (Char==')') ||
     (Char=='.'))
     return(1);
  else return(0);
}


// Je-li znak cislice vrati 1
uchar Str_IsNum(uchar Char)
{ if(((Char >= '0') && (Char <= '9')) || (Char=='+')) return(1); else return(0);
}


// Prevede cislo z BCD kodu na dekadicky
uchar Str_BCD2DEC(uchar BCD)
{ uchar Out;

  Out=BCD & 15;
  Out+=((BCD >> 4) & 15)*10;
  return(Out);
}

// Vrati velikost retezce
uchar Str_StrSize(uchar *Str)
{ uchar i;
  i=0;
  while((Str_IsPrint(Str[i])) && (i<255)) i++;
  return(i);
}

// Vrati velikost cisla v retezci
uchar Str_StrNumSize(uchar *StrNum)
{ uchar i;
  i=0;
  while((Str_IsNum(StrNum[i])) && (i<255)) i++;
  return(i);
}

