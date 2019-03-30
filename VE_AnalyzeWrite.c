#define DEBUG 0

#include <JEO:JEO.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include "VE_Prototypes.h"

#if DEBUG
VOID ShowMe (UWORD off, UBYTE *String);
#endif

UWORD writeLen;
UWORD writeOffset;
UWORD writeDoIOV12;
UWORD writeDoIO;
UWORD CMDWrite;
UWORD bootblockWrite;
BOOL  format;
extern UBYTE *Dummy;

#define VALUE	50
#define OFFSET	20

UBYTE ATab[] = "13579BDF89ABCDEF";
UBYTE RTab[] = "01234567";

BOOL Check_CLR (UWORD code)
{
  UWORD i;
  UBYTE Code[5];
  BOOL flag;

  SPrintF (Code, "%04lX", code);
  if (Code[0] == '4' AND Code[1] == '2' AND (Code[2] == '2' OR Code[2] == '6' OR Code[2] == 'A')) // clr.# $value(adr)
  {
    flag = FALSE;
    for (i = 8; i < 16; i++)
    {
      if (Code[3] == ATab[i])
      {
        flag = TRUE;
        break;
      }
    }
    if (flag)		// Found an A# adress
      return (TRUE);
  }
  return (FALSE);
}

WORD Check_value_WORD (UWORD code, WORD len)
{
  UWORD i;
  UBYTE Code[5];
  BOOL flag;

  SPrintF (Code, "%04lX", code);
  if ((Code[0] == '1' OR Code[0] == '3') AND Code[2] == '7' AND Code[3] == 'C')	// move.l #00000000,$value(adr)
  {
    flag = FALSE;
    for (i = 0; i < 8; i++)
    {
      if (Code[1] == ATab[i])
      {
        flag = TRUE;
        break;
      }
    }
    if (flag)		// Found an A# adress
      return (len);
  }
  return (ERROR);
}

LONG Check_value_LONG (UWORD code, UWORD len1, UWORD len2)
{
  UWORD i;
  UBYTE Code[5];
  BOOL flag;
  LONG offsetVal;

  SPrintF (Code, "%04lX", code);
  offsetVal = UWordToULong (len1, len2);
  if (Code[0] == '2' AND Code[2] == '7' AND Code[3] == 'C')	// move.l #00000000,$value(adr)
  {
    flag = FALSE;
    for (i = 0; i < 8; i++)
    {
      if (Code[1] == ATab[i])
      {
        flag = TRUE;
        break;
      }
    }
    if (flag)		// Found an A# adress
      return (offsetVal);
  }
  return (ERROR);
}

/*
BOOL Check_len (UWORD buf, UWORD off)
{
  UWORD i;
  UBYTE Code[5];
  BOOL flag;

  SPrintF (Code, "%04lX", buf);

  if ((Code[0] == '1' OR Code[0] == '3' OR Code[0] == '2') AND Code[2] == '4')	// move.# reg,value(adr)
  {
    flag = FALSE;
    for (i = 0; i < 8; i++)
    {
      if (Code[1] == ATab[i])
      {
        flag = TRUE;
        break;
      }
    }
    if (flag)		// Found an A# adress
    {
      flag = FALSE;
      for (i = 0; i < 8; i++)
      {
        if (Code[3] == RTab[i])
        {
          flag = TRUE;
          break;
        }
      }
      if (flag)		// Found a D# register
      {
#if DEBUG 
        SPrintF (Dummy, "move.#  d%lc,offset/len(Address)", Code[3]);
        ShowMe (off, Dummy);
        return (TRUE);
#endif
      }
    }
  }
  return (FALSE);
}
*/

VOID AnalyzeWrite (UWORD *Buf, UWORD start)
{
  ULONG i, j, val;
  UBYTE B[2];
  WORD offset;
  LONG offsetVal;

  if (bootblockWrite < 2)
    bootblockWrite = 0;

  CMDWrite++;
#if DEBUG 
  SPrintF (Dummy, "#%ld CMD_WRITE", CMDWrite);
  ShowMe (start - 2, Dummy);
#endif
  offset = start - OFFSET;	// start litt bakifra :)
  if (offset < 0)
    offset = 0;
  for (i = offset, j = 0; j < 40; i++, j++)
  {
    if (bootblockWrite > 2 AND format)
      return;
    if (i >= 511)
    {
#if DEBUG
      ShowMe (i, "i >= 511");
#endif
      return;
    }

    if (i > start) 	// Er vi etter start?
    {
      B[0] = (Buf[i] >> 8);	// Kun den første byten
      {
        if (B[0] == 0x60 OR B[0] == 0x61)	// bsr.s
        {
          val = (Buf[i] & 0xFF);	// Sjekk den andre byten, JUMP
          if (val == 0)
          {
#if DEBUG
            ShowMe (i, "bsr");
#endif
          }
#if DEBUG 
          SPrintF (Dummy, "bsr.s -> %02lx (%04lx)", val, (((i + 1) + (val / 2)) * 2));
          ShowMe (i, Dummy);
#endif
          i += (val / 2) + 1;
          continue;
        }
      }
    }
    switch (Buf[i])
    {
      case 0x4E75:		// rts 
      {
        if (i > start)
          return;
      }
      case 0x001C:		// New command between 
      {
        if (i > start AND i >= 2)
        {
          if (Buf[i-2] == 0x337C OR Buf[i-1] == 0x3340 OR Buf[i-1] == 0x3341)
          {
#if DEBUG
            ShowMe (i, "New command between");
#endif
            return;
          }
        }
        break;
      }
      case 0x001D:		// New command between 
      {
        if (i > start AND i >= 2)
        {
          if (Buf[i-2] == 0x337C OR Buf[i-1] == 0x3340 OR Buf[i-1] == 0x3341)
          {
#if DEBUG
            ShowMe (i, "New command between");
#endif
            return;
          }
        }
        break;
      }
      case 0x009C:		// New command between 
      {
        if (i > start AND i >= 2)
        {
          if (Buf[i-2] == 0x337C OR Buf[i-1] == 0x3340 OR Buf[i-1] == 0x3341)
          {
#if DEBUG
            ShowMe (i, "New command between");
#endif
            return;
          }
        }
        break;
      }
// ******************************* $24 (length) ******************************
      case 0x0024:		// len 
      {
        if (i > 12)
        {
          if ((offsetVal = Check_value_LONG (Buf[i-3], Buf[i-2], Buf[i-1])) != ERROR)
          {
#if DEBUG 
            SPrintF (Dummy, "move.l  #$%08lx,$24(Adress)", offsetVal);
            ShowMe (i - 3, Dummy);
#endif
            if (offsetVal == 0x0400)	// 1024
            {
              bootblockWrite++;
              writeLen++;
            }
            else if (offsetVal > 0x400)
              format = TRUE;
          }
          else if ((offsetVal = Check_value_WORD (Buf[i-2], Buf[i-1])) != ERROR)
          {
#if DEBUG 
            SPrintF (Dummy, "move.w  #$%04lx,$24(Adress)", offsetVal);
            ShowMe (i - 2, Dummy);
#endif
            if (offsetVal == 0x0400)	// 1024
            {
              writeLen++;
              bootblockWrite++;
            }
            else if (offsetVal > 0x400)
              format = TRUE;
          }
        }
//        else if (Check_len (Buf[i-1], i - 1))
//          writeLen++;
        break;
      }
// ******************************* $2C (offset) ******************************
      case 0x002C:
      {
        if (i > 12)
        {
          if (Check_CLR (Buf[i-1]))
          {
#if DEBUG 
            ShowMe (i, "clr.l   $2C(Adress)");
#endif
            writeOffset++;
            bootblockWrite++;
          }
          else if ((offsetVal = Check_value_LONG (Buf[i-3], Buf[i-2], Buf[i-1])) != ERROR)
          {
#if DEBUG 
            SPrintF (Dummy, "move.l  #$%08lx,$2C(Adress)", offsetVal);
            ShowMe (i - 3, Dummy);
#endif
            if (offsetVal == 0)
            {
              writeOffset++;
              bootblockWrite++;
            }
            else
              format = TRUE;
          }
          else if ((offsetVal = Check_value_WORD (Buf[i-2], Buf[i-1])) != ERROR)
          {
#if DEBUG 
            SPrintF (Dummy, "move.l  #$%04lx,$2C(Adress)", offsetVal);
            ShowMe (i - 2, Dummy);
#endif
            if (offsetVal == 0)
            {
              writeOffset++;
              bootblockWrite++;
            }
            else
              format = TRUE;
          }
        }
//        else if (Check_len (Buf[i-1], i - 1))
//          writeOffset++;
        break;
      }
      case 0x4EAE:		// jsr   _LVODoIO 
      {
        if (i >= 511 OR i < start)
          break;
        if (Buf[i+1] == 0xFE38)
        {
#if DEBUG 
          ShowMe (i, "jsr _LVODoIO");
#endif
          writeDoIO++;
          return;
        }
        break;
      }
      case 0x4EB9:		// jsr   $FC06DC 
      {
        if (i >= 510 OR i < start)
          break;
        if (Buf[i+1] == 0x00FC AND Buf[i+2] == 0x06DC)
        {
#if DEBUG 
          ShowMe (i, "jsr FC06DC");
#endif
          writeDoIOV12++;
          return;
        }
        break;
      }
      case 0x4EF9:		// jmp   $FC06DC
      {
        if (i >= 510 OR i < start)
          break;
        if (Buf[i+1] == 0x00FC AND Buf[i+2] == 0x06DC)
        {
#if DEBUG 
          ShowMe (i, "jmp FC06DC");
#endif
          writeDoIOV12++;
          return;
        }
#if DEBUG
        ShowMe (i, "jmp");
#endif 
       return;
      }
    }
  }
}
