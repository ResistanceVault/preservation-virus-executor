#include <JEO:JEO.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/dos.h>
#include <exec/memory.h>
#include <exec/lists.h>
#include <devices/trackdisk.h>
#include <JEO:raw.h>

#include <libraries/reqtools.h>
#include <proto/reqtools.h>

#include "VE_Prototypes.h"
#include "VE_Record.w"
#include "VE_Window.h"

#define NAMESIZE	  36
#define MAXLINES	  25
#define GREY	0
#define BLACK	1
#define WHITE	2
#define BLUE	3

#define X1	 30
#define Y1	 27

#define OTHER	2
#define VIRUS	3

char CheckHead[14];

typedef struct
{
  struct List list;
  ULONG howmany;
  ULONG checkSum;
} Sjef;

Sjef sjef;

typedef struct
{
  struct Node nd;
  UBYTE Name[NAMESIZE],
        Buf[BUFSIZE];
} Base;

/*
typedef struct
{
  struct Node nd;
  UBYTE Name[NAMESIZE],	// Navnet på fila
  ULONG	offset,					// Offsetten på hovedfila
        lenght,					// Lengden på data (skal være 1024)
        nr,							// Nummeret på fila...
        flag;						// Diverse flags.
} Base;
*/

ULONG recCount = 0;
ULONG pos = 0;
UBYTE DummyName[NAMESIZE];
UBYTE DummyBuf[BUFSIZE];
UBYTE DataBaseEmpty[50];

extern struct Window *VirusWnd;
extern UWORD mainX;
extern UBYTE *Dummy;

extern UBYTE *Drive[];
extern UBYTE *Buffer[];
extern UWORD viewCol[];

extern UWORD knownBoot[];
extern WORD otherFlag[];

extern struct NewWindow NewWindowStructure2;
extern struct IOExtTD *diskreq;

extern struct Gadget RecordUpGD;
extern struct Gadget RecordDownGD;
extern struct Gadget PropGadget;

extern struct PropInfo Slide;

/* extern struct Image PropImage;*/

struct Window *w2 = 0;
struct RastPort *rp2;

BOOL saveFlag;
BOOL gadgFlag;

VOID KillBase (VOID)
{
  ULONG s;
  struct Node *nd;

  nd = sjef.list.lh_Head;
  for (s = 0; s < recCount; s++)
  {
    Remove (nd);
    FreeMem ((UBYTE *)nd, sizeof (Base));
		nd = 0;
    nd = sjef.list.lh_Head;
  }
  recCount = 0;
}

VOID CloseBase (VOID)
{
  Filbase *bp;

  KillBase ();
 	bp = (Filbase *)sjef.list.lh_Head;
  while (bp = (Filbase *)RemHead (&sjef.list))
 	{
    FreeMem (bp, sizeof (Base));
 	  bp = 0;
	}
}

VOID BaseCleanUp ()
{
  if (saveFlag == ON)
  {
    if (rtEZRequestTags (GLS (&L_CHANGES_DATABASE), GLS (&L_SAVE_CANCEL), NULL, NULL,
    RT_ReqPos, REQPOS_CENTERSCR, RT_TextAttr, &topaz8, 
    RTEZ_Flags, EZREQF_NORETURNKEY,
    RTEZ_ReqTitle, GLS (&L_MESSAGE), TAG_END))
      SaveBase (ON);
  }

  if (recCount != 0)
    CloseBase ();
}

struct Node *FindTheOne (UBYTE *Name)
{
  struct Node *nd;

  nd = sjef.list.lh_Head;
  while (nd->ln_Succ)
  {
    if (stricmp (nd->ln_Name, Name) > 0)
    {
      nd = nd->ln_Pred;
      return (nd);
    }
    nd = nd->ln_Succ;
  }
  return (FALSE);
}

BOOL InsertBase (UBYTE *Name, UBYTE *Buf)
{
  Base *bp=NULL;
  struct Node *nd;

  if (!(bp = AllocMem (sizeof (Base), MEMF_PUBLIC)))
    return (FALSE);

  if (AvailMem (MEMF_CHIP) < (60 * 1024))
    return (FALSE);

  JEOCopy (bp->Name, Name, NAMESIZE-1, NULL);
  CopyMem (Buf, bp->Buf, BUFSIZE);
  bp->nd.ln_Name=bp->Name;

  if (!(nd = FindTheOne (Name)))
    AddTail(&sjef.list, &bp->nd);
  else
    Insert (&sjef.list, &bp->nd, nd);

  recCount++;
  return (TRUE);
}

VOID DeleteAll (VOID)
{
  if (recCount == 0)
  {
    Status (DataBaseEmpty);
    return;
  }
  Status (GLS (&L_DELETING_DATABASE));
 
  SPrintF (Dummy, GLS (&L_DELETE_DATABASE_ALL), recCount);

  if (rtEZRequestTags (Dummy, GLS (&L_DELETE_CANCEL), NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
                       RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_DELETE_DATABASE), RT_TextAttr, &topaz8, TAG_END))
  {
    if (rtEZRequestTags (GLS (&L_ARE_YOU_SURE), GLS (&L_DELETE_CANCEL), NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
                       RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_DELETE_DATABASE), RT_TextAttr, &topaz8, TAG_END))
    {
      KillBase ();
      saveFlag = OFF;
      if (DeleteFile (DataName))
        Status (GLS (&L_DATABASE_DELETED));
      else
        Status (GLS (&L_ERROR_DELETE_DATABASE));
    }
    else
      Status (GLS (&L_READY));
  }
  else
    Status (GLS (&L_READY));
}

BOOL DeleteOne (UBYTE *Name)
{
  struct Node *nd;

  if (nd = FindName (&sjef.list, Name)) 
  {
    Remove (nd);
    FreeMem ((UBYTE *)nd, sizeof (Base));
    nd = 0;
    recCount--;
    return (TRUE);
  }
  else
    return (FALSE);
}

BOOL SaveBase (BOOL end)
{
  BPTR fh = 0;
  Base *bp;
  struct Node *nd;
  ULONG i;

  if (saveFlag == OFF)
  {
    if (!end)
    {
      if (recCount > 0)
        Status (GLS (&L_NO_CHANGES_DATABASE));
      else
        Status (DataBaseEmpty);
    }
    return (FALSE);
  }
  if (recCount == 0)	/* No entries here */
  {
    if (!end)
      Status (DataBaseEmpty);
    return (FALSE);
  }
  if (fh = Open (DataName, MODE_NEWFILE))
  {
    Status (GLS (&L_SAVING_DATABASE));
    All (OFF);

    Write (fh, VE, strlen (VE) + 1);

    nd = sjef.list.lh_Head;
    bp = (Base *)nd;
    i = 0;
	  while (nd->ln_Succ)
    {
      Write (fh, &bp->Name, NAMESIZE);
      Write (fh, &bp->Buf, BUFSIZE);
      nd = nd->ln_Succ;
      bp = (Base *)nd;
      i++;
      JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, i, recCount, BLUE);
      SPrintF (Dummy, GLS (&L_SAVING_DATABASE_1), i);
      Status (Dummy);
    }
    Close (fh);
		ClearBar ();
    Status (GLS (&L_DATABASE_SAVED));
    saveFlag = OFF;
    All (ON);
    return (TRUE);
  }
  else
  {
    Status (GLS (&L_ERROR_SAVING_DATABASE));
    return (FALSE);
  }
}

typedef struct
{
  UBYTE Name[NAMESIZE],
        Buf[BUFSIZE];
} TempLoad;

TempLoad tempLoad;

VOID LoadBase ()
{
  BPTR fh = 0;
  WORD i = 0;
  LONG size;

  Status (GLS (&L_LOADING_DATABASE));
  saveFlag = OFF;

  NewList (&sjef.list);
  size = FileSize (DataName);
  if (size <= 0)	/* No file */
		return;

  size -= 4;
  size /= (NAMESIZE + BUFSIZE);
  if (fh = Open (DataName, MODE_OLDFILE))
  {
    Read (fh, CheckHead, strlen (VE) + 1);
    if (!(strcmp (CheckHead, VE)))
    {
      while (Read (fh, &tempLoad, sizeof (TempLoad)))
      {
        i++;
        JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, i, size, BLUE);
        SPrintF (Dummy, GLS (&L_LOADING_DATABASE), i);
        Status (Dummy);
        if (!InsertBase (tempLoad.Name, tempLoad.Buf))
        {
          Status (GLS (&L_NO_MEM_DATABASE));
          break;
        }
      }
    }
    else
    {
      SPrintF (Dummy, GLS (&L_NOT_A_DATABASE), DataName);
      Status (Dummy);
			JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
    }
    Close (fh);
  }
	ClearBar ();
}

BOOL CheckNameBufExist (UBYTE *Name, UBYTE *Buf)
{
  struct Node *nd;
  Base *bp;

  nd = sjef.list.lh_Head;
  bp = (Base *)nd;
  while (nd->ln_Succ)
  {
    if (!(strcmp (bp->Name, Name)))
      return (TRUE);
    if (!(memcmp (bp->Buf, Buf, BUFSIZE)))
      return (TRUE);
    nd = nd->ln_Succ;
    bp = (Base *)nd;
  }
  return (FALSE);
}

VOID MergeBase ()
{
	BPTR fh = 0;
  WORD flag;
  UBYTE DiskName[108] = "RAM:";
  ULONG i, c;
  LONG size;

  Status (GLS (&L_MERGE_DATABASE));

  if (!(GetFileName (DiskName, GLS (&L_MERGE_DATABASE), GLS (&L_MERGE), "VirusExecutor.Data")))
  {
    Status (GLS (&L_READY));
    return;
  }

  size = FileSize (DiskName);
  if (size <= 0)	/* No file */
  {
    SPrintF (Dummy, GLS (&L_ERROR_MERGING), DiskName);
    Status (Dummy);
    return;
  }
  size -= 4;
  size /= (NAMESIZE + BUFSIZE);

  if (fh = Open (DiskName, MODE_OLDFILE))
  {
    Read (fh, CheckHead, strlen (VE) + 1);
    if (!(strcmp (CheckHead, VE)))
    {
      Status (GLS (&L_MERGING_DATABASE));
      i = c = 0;
      while (Read (fh, &tempLoad, sizeof (TempLoad)))
      {
        c++;
        JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, c, size, BLUE);
        if (CheckNameBufExist (tempLoad.Name, tempLoad.Buf))
          continue;
        if (!InsertBase (tempLoad.Name, tempLoad.Buf))
        {
          Status (GLS (&L_NO_MEM_MERGE_DATABASE));
          flag = TRUE;
          break;
        }
        else
        {
          SPrintF (Dummy, GLS (&L_MERGING), i + 1);
          Status (Dummy);
        }
        i++;
      }
      if (i > 0)
        saveFlag = ON;
			SPrintF (Dummy, GLS (&L_MERGE_END), i, 1 == 1 ? GLS (&L_BOOTBLOCK) : GLS (&L_BOOTBLOCKS), c);
      Status (Dummy);
    }
    else
    {
      SPrintF (Dummy, GLS (&L_NOT_A_DATABASE), DataName);
      Status (Dummy);
			JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
      flag = TRUE;
    }
    flag = TRUE;
    Close (fh);
	}
  else
    flag = FALSE;

	ClearBar ();
  if (!flag)
  {
    Status (GLS (&L_ERROR_MERGING_DATABASE));
		JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR_MERGING_DATABASE), GLS (&L_OK), S_ERROR);
  }
}

BOOL FindPosition (VOID)
{
  ULONG p;

  if (recCount <= MAXLINES)
  {
    if (pos != 0)
    {
      pos = 0;
      return (TRUE);
    }
    else      
      return (FALSE);
  }
  p = pos;
  pos = (LONG)Slide.VertPot * 100 / 0xFFFF * (recCount - MAXLINES);
  pos /= 100;

  if (p == pos)
    return (FALSE);	/* No need to refresh */
  else
    return (TRUE);	/* Refresh */
}

WORD holdX;
WORD holdY;

VOID ReversName (WORD mouseX, UWORD mouseY)
{
  ULONG i, maxLines;
  UWORD y;
  UWORD x;

  y = Y1 - 8;
  x = X1;
  if (recCount < MAXLINES)
    maxLines = recCount;
  else
    maxLines = MAXLINES;

  for (i = 0; i < maxLines; i++)
  {
    if (mouseX >= x AND mouseY >= y AND mouseX <= (x + (8 * (NAMESIZE + 5 - 1))) AND mouseY <= (y + 8))
    {
      if (holdX != ERROR)
      {
        if (holdX == x AND holdY == y)	/* Same as last */
          return;

        SetDrMd (rp2, COMPLEMENT);
        RectFill (rp2, holdX - 5, holdY + 1 + OFFY, holdX + ((NAMESIZE + 5) * 8) - 4, holdY + 9 + OFFY);
        SetDrMd (rp2, JAM2);

        SetDrMd (rp2, COMPLEMENT);
        RectFill (rp2, x - 5, y + 1 + OFFY, x + ((NAMESIZE + 5) * 8) - 4, y + 9 + OFFY);
        SetDrMd (rp2, JAM2);

        holdX = x;
        holdY = y;
        return;
      }
      else
      {
        SetDrMd (rp2, COMPLEMENT);
        RectFill (rp2, x - 5, y + 1 + OFFY, x + ((NAMESIZE + 5)* 8) - 4, y + 9 + OFFY);
        SetDrMd (rp2, JAM2);
        holdX = x;
        holdY = y;
        return;
      }
    }
    y += 9;
  }
  if (holdX != ERROR)
  {
    SetDrMd (rp2, COMPLEMENT);
    RectFill (rp2, holdX - 5, holdY + 1 + OFFY, holdX + ((NAMESIZE + 5) * 8) - 4, holdY + 9 + OFFY);
    SetDrMd (rp2, JAM2);
    holdX = ERROR;
    holdY = ERROR;
  }
}

WORD FindXY (WORD mouseX, UWORD mouseY)
{
  LONG i, maxLines;
  UWORD y;
  UWORD x;
  WORD ret = ERROR;

  if (recCount < MAXLINES)
    maxLines = recCount;
  else
    maxLines = MAXLINES;

  y = Y1 - 8;
  x = X1;
  for (i = 0; i < maxLines; i++)
  {
    if (mouseX >= x AND mouseY >= y AND mouseX <= (x + (8 * (NAMESIZE + 5 - 1))) AND mouseY <= (y + 8))
    {
      ret = i + pos;
      break;
    }
    y += 9;
  }
  return (ret);
}

VOID UpdateNameAndBuf (WORD nr)
{
  struct Node *nd;
  Base *bp;
  WORD i = 0;

  nd = sjef.list.lh_Head;
  bp = (Base *)nd;
  while (nd->ln_Succ)
  {
    if (i == nr)
    {
      mystrcpy (DummyName, bp->Name);
      CopyMem (bp->Buf, DummyBuf, BUFSIZE);
      break;
    }
    nd = nd->ln_Succ;
    bp = (Base *)nd;
    i++;
  }
}

VOID PrintOutNames (ULONG start)
{
  struct Node *nd;
  ULONG i, j, len;
  UWORD lines;
  WORD x, y;

  nd = sjef.list.lh_Head;	/* Start Tail = end */

  if (start != 0)
  {
    for (i = 0; i < start; i++)	/* Find startpoint */
       nd = nd->ln_Succ;
  }

  x = X1;
  y = Y1;
  lines = 0;
  SetAPen (rp2, BLACK);
  for (i = 0; i < recCount; i++)
  {
    SPrintF (Dummy, "%3ld. %s", start + i + 1, nd->ln_Name);
    len = strlen (Dummy);
    for (j = len; j < 40; j++)
      Dummy[j] = ' ';
    Dummy[j] = 0;

    Move (rp2, x, y + OFFY);
    Text (rp2, Dummy, 40);	/* Don't use JEOWrite due to speed */
    nd = nd->ln_Succ;
    lines++;
    y += 9;
    if (lines >= MAXLINES)
      break;
  }
}

VOID InitRecordWindow (VOID)
{
	PropGadget.TopEdge += OFFY;
}

WORD GetBaseBoot (UBYTE *WTitle)
{
  struct IntuiMessage *message;
  ULONG Signals;
  ULONG class;
  USHORT Code;
  APTR Address;
  UWORD mouseX, mouseY;
  WORD nr = ERROR;
  ULONG ram = 0;

  if (recCount == 0)
  {
    Status (DataBaseEmpty);
    return (ERROR);
  }

  if (recCount > MAXLINES)
    Slide.VertBody = (ULONG)0xFFFF*MAXLINES/recCount;
  else
    Slide.VertBody = (ULONG)0xFFFF;

  NewWindowStructure2.Screen = Scr;
  NewWindowStructure2.Title = WTitle;
  NewWindowStructure2.Width = screenWidth;
  NewWindowStructure2.Height = screenHeight;
  NewWindowStructure2.Height = screenHeight;

  if (!(w2 = OpenWindow (&NewWindowStructure2)))
  {
    Status (GLS (&L_ERROR_OPENING_WINDOW));
    return (ERROR);
  }
  rp2 = w2->RPort;

	if (textfont)
		SetFont (rp2, textfont);

  StyleIt2 (rp2, 393, 49 + OFFY, 422, 248 + OFFY, WHITE, BLACK, GADGET_STYLE); /* Scroller */
  StyleIt2 (rp2, 393, 16 + OFFY, 568,  38 + OFFY, WHITE, BLACK, GADGET_STYLE);
  SPrintF (Dummy, GLS (&L_RECORDED_VIEW), recCount);
  JEOWrite (rp2, 400, 25 + OFFY, Dummy, BLACK);
  ram = AvailMem (MEMF_CHIP);
  ram += AvailMem (MEMF_FAST);
  SPrintF (Dummy, GLS (&L_FREE_VIEW), (ram / 1024) - 60);
  JEOWrite (rp2, 400, 34 + OFFY, Dummy, BLACK);

  StyleIt2 (rp2, 14, 16 + OFFY, 365, 248 + OFFY, BLACK, WHITE, GADGET_STYLE); // main

  holdX = ERROR;
  gadgFlag = FALSE;
  FindPosition ();
  PrintOutNames (pos);

  FOREVER 
  {
    Signals = Wait (1<<w2->UserPort->mp_SigBit);
    if (Signals & (1<<w2->UserPort->mp_SigBit)) 
    {
      while (message = (struct IntuiMessage *)GetMsg (w2->UserPort)) 
      {
        class = message->Class;
        Code = message->Code;
        Address = message->IAddress;
        mouseX = message->MouseX;
        mouseY = message->MouseY - OFFY;
        ReplyMsg ((struct Message *)message);

        switch (class)
        {
          case RAWKEY:
          {
            switch (Code)
            {
              case ESC:
                CloseWindow (w2);
                Status (GLS (&L_READY));
                return (ERROR);
            }
            break;
          }
          case GADGETDOWN:
          {
            if (Address == (APTR)&PropGadget)
            {
              gadgFlag = TRUE;
              if (recCount > MAXLINES)
              {
                if (FindPosition ())
                  PrintOutNames (pos);
              }
            }
            break;
          }
          case GADGETUP:
          {
            if (Address == (APTR)&PropGadget)
            {
              gadgFlag = FALSE;
              if (recCount > MAXLINES)
              {
                if (FindPosition ())
                  PrintOutNames (pos);
                ReversName (mouseX, mouseY);
              }
            }
            break;
          }
          case MOUSEBUTTONS:
          {
            switch (Code)
            {
              case SELECTDOWN: 
                nr = FindXY (mouseX, mouseY);
            }
            break;
          }
          case MOUSEMOVE:
          {
            if (recCount > MAXLINES)	/* More than one side */
            {
              if (FindPosition ())
                PrintOutNames (pos);
              else
              {
                if (!gadgFlag)
                  ReversName (mouseX, mouseY);
              }
            }
            else if (!gadgFlag)
              ReversName (mouseX, mouseY);
            break;
          }
          case CLOSEWINDOW:
          {
            CloseWindow (w2);
            Status (GLS (&L_READY));
            return (ERROR);
          }
        }
      }
    }
    if (nr != ERROR)
      break;
    ClearIntuition (w2);
  }
  CloseWindow (w2);
  UpdateNameAndBuf (nr);
  return (nr);
}

BOOL LoadBaseBoot ()
{
  WORD nr;

  Status (GLS (&L_PLEASE_WAIT));
  if ((nr = GetBaseBoot (GLS (&L_SELECT_BB_INSTALL))) != ERROR)
    return (TRUE);
  else
    return (FALSE);
}

VOID EditName ()
{
  ULONG nr;
  UBYTE NewName[NAMESIZE];

  if (recCount == 0)
  {
    Status (DataBaseEmpty);
    return;
  }

  Status (GLS (&L_PLEASE_WAIT));
	SPrintF (Dummy, "%s...", GLS (&L_EDIT_NAME));
  nr = GetBaseBoot (Dummy);
  if (nr == ERROR)
  {
    Status (GLS (&L_USER_BREAK));
    return;	/* Avbryt */
  }
  SPrintF (Dummy, GLS (&L_CHANGE_NAME), DummyName);
  Status (Dummy);
  mystrcpy (NewName, DummyName);
  if (rtGetString (NewName, NAMESIZE - 1, GLS (&L_CHANGE_TO), NULL, RT_ReqPos, REQPOS_CENTERSCR, RT_TextAttr, &topaz8, TAG_END))
  {
    if (!(strcmp (NewName, DummyName)))
    {
      Status (GLS (&L_CHANGE_MIND));
      return;
    }
    else if (CheckNameExist (NewName))	/* Allready there? */
    {
      SPrintF (Dummy, GLS (&L_ALLREADY_EXISTS), NewName);
      Status (Dummy);
      return;
    }
  }
  else
  {
    Status (GLS (&L_USER_BREAK));
    return;	/* Avbryt */
  }
  DeleteOne (DummyName);
  if (InsertBase (NewName, DummyBuf))
  {
    SPrintF (Dummy, GLS (&L_CHANGED_OK), DummyName, NewName);
    Status (Dummy);
    saveFlag = ON;
    return;
  }
}

BOOL CheckBootExist (WORD unit)
{
  WORD i;
  WORD flag;
  struct Node *nd;
  Base *bp;

  nd = sjef.list.lh_Head;
  bp = (Base *)nd;
  while (nd->ln_Succ)
  {
    flag = TRUE;
    for (i = 0; i < BUFSIZE; i++)
    {
      if (Buffer[unit][i] != bp->Buf[i])
      {
        flag = ERROR;
        break;
      }
    }
    if (flag != ERROR)
    {
      mystrcpy (DummyName, nd->ln_Name);
      return (TRUE);
    }
    nd = nd->ln_Succ;
    bp = (Base *)nd;
  }
  return (FALSE);
}

BOOL CheckNameExist (UBYTE *Name)
{
  struct Node *nd;

  nd = sjef.list.lh_Head;
  while (nd->ln_Succ)
  {
    if (!(strcmp (nd->ln_Name, Name)))
      return (TRUE);
    nd = nd->ln_Succ;
  }
  return (FALSE);
}

BOOL CheckRecorded (WORD unit)
{
  if (recCount > 0)
  {
    if (CheckBootExist (unit))	/* A known recorded bootblock */
    {
      SPrintF (Dummy, "%s.", DummyName);
      DiskStatus (Dummy, unit);
      return (TRUE);
    }
    else
      return (FALSE);
  }
  else
    return (FALSE);
}

VOID ViewBase ()
{
  WORD nr, flag;
  ULONG i, j, k;

  if (recCount == 0)
  {
    Status (DataBaseEmpty);
    return;
  }
  Status (GLS (&L_PLEASE_WAIT));
  flag = TRUE;
  while (flag)
  {
    if ((nr = GetBaseBoot (GLS (&L_VIEW_BB))) != ERROR)
    {
      for (k = 0; k < 73; k++)
        Dummy[k] = ' ';
      Dummy[k++] = '\n';
      Dummy[k++] = '\n';
      Dummy[k++] = '\n';
      for (i = 0, j = k; i < BUFSIZE; i++, j++)
      {
        if (DummyBuf[i] > 31 AND DummyBuf[i] < 127)
        {
          if (DummyBuf[i] == 37)	/* % */
            Dummy[j] = '@';
          else
            Dummy[j] = DummyBuf[i];
        }
        else
          Dummy[j] = '.';
  
        if (!((i+1) % 64))
          Dummy[++j] = '\n';
      }
      Dummy[--j] = 0;
  
      strcat (Dummy, "\n\n\n\n");
      rtEZRequestTags (Dummy, GLS (&L_OK), NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
                         RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, DummyName, RT_TextAttr, &topaz8, TAG_END);
    }
    else
      flag = FALSE;
  }
  Status (GLS (&L_READY));
}

BOOL DeleteOneRecord ()
{
  WORD nr;

  Status (GLS (&L_PLEASE_WAIT));
  if ((nr = GetBaseBoot (GLS (&L_SELECT_BB_DELETE))) != ERROR)
  {
    SPrintF (Dummy, GLS (&L_DELETE_ONE_DATABASE), DummyName);
    if (rtEZRequestTags (Dummy, GLS (&L_DELETE_CANCEL), NULL, NULL,
                         RT_ReqPos, REQPOS_CENTERSCR, RT_TextAttr, &topaz8,
                         RTEZ_ReqTitle, GLS (&L_MESSAGE), TAG_END))
    {
      if (DeleteOne (DummyName))	/* Delete selected name and buffer */
      {
        saveFlag = ON;
        SPrintF (Dummy, GLS (&L_IS_DELETED), DummyName);
        Status (Dummy);
        return (TRUE);
      }
      else
      {
        SPrintF (Dummy, GLS (&L_ERROR_DELETING_BB), DummyName);
        Status (Dummy);
        return (FALSE);
      }      
    }
    else
    {
      Status (GLS (&L_READY));
      return (FALSE);
    }
  }
  else
    return (FALSE);
}

VOID RecordBoot (UWORD unit)
{
  UBYTE Name[NAMESIZE] = "";

  if (knownBoot[unit] == TRUE)
  {
    rtEZRequestTags (GLS (&L_NO_NEED_RECORD), GLS (&L_OK), NULL, NULL,
	    RT_ReqPos, REQPOS_CENTERSCR, RT_TextAttr, &topaz8,
  	  RTEZ_ReqTitle, GLS (&L_MESSAGE), TAG_END);
    Status (GLS (&L_READY));
    return;
  }
  else if (knownBoot[unit] == VIRUS)
  {
    rtEZRequestTags (GLS (&L_NO_RECORD_VIRUS), GLS (&L_OK), NULL, NULL,
	    RT_ReqPos, REQPOS_CENTERSCR, RT_TextAttr, &topaz8,
  	  RTEZ_ReqTitle, GLS (&L_MESSAGE), TAG_END);
    Status (GLS (&L_READY));
    return;
  }
/*
	if (1)
		SPrintF (Name, "! %s", bootinfo->xvsbi_Name);
*/
  SPrintF (Dummy, GLS (&L_GIVE_BB_NAME), Drive[unit]);
  if (rtGetString (Name, NAMESIZE - 1, Dummy, NULL, RT_ReqPos, REQPOS_CENTERSCR, RT_TextAttr, &topaz8, TAG_END))
  {
    if (CheckNameBufExist (Name, Buffer[unit]))	/* Allready there? */
    {
      PlaySound (S_ERROR);
      SPrintF (Dummy, GLS (&L_ALLREADY_EXISTS), Name);
      Status (Dummy);
      return;
    }
    if (!(InsertBase (Name, Buffer[unit])))
    {
      PlaySound (S_ERROR);
      Status (GLS (&L_NO_MEM_RECORD_BB));
      return;
    }
    SPrintF (Dummy, "%s.", Name);
    DiskStatus (Dummy, unit);
    knownBoot[unit] = TRUE;
    Status (GLS (&L_READY));
    saveFlag = ON;
  }
  else
  {
    Status (GLS (&L_READY));
    return;
  }
}

VOID Record ()
{
  UBYTE Units[60] = "";
  UWORD unit, flag, i;
  UWORD uFlag[4];

	mystrcpy (DataBaseEmpty, GLS (&L_DATABASE_EMPTY));

  Status (GLS (&L_DATABASE));
  for (unit = 0, i = 0; unit < 4; unit++)
  {
    if (viewCol[unit] != BLUE)	// Not empty
    {
      if (drive[unit] != ERROR)
      {
        strcat (Units, Drive[unit]);
        strcat (Units, "|");
        uFlag[i] = unit;
        i++;
      }
    }
  }   
  if (i == 0)
  {
		JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NO_DISK_ANY_DRIVE), GLS (&L_OK), S_MESSAGE);
    return;
  }
  if (i > 1)	/* More than one disk in the system */
  {
    strcat (Units, GLS (&L_WCANCEL));
    SPrintF (Dummy, GLS (&L_SELECT_BB_RECORD));
    if (flag = rtEZRequestTags (Dummy, Units, NULL, NULL,
      RT_ReqPos, REQPOS_CENTERSCR, RT_TextAttr, &topaz8,
      RTEZ_ReqTitle, GLS (&L_RECORD_A_BB), TAG_END))
    {
      switch (flag)
      {
        case 1: RecordBoot (uFlag[0]); break;
        case 2: RecordBoot (uFlag[1]); break;
        case 3: RecordBoot (uFlag[2]); break;
        case 4: RecordBoot (uFlag[3]); break;
      }
    }
    else
      Status (GLS (&L_READY));      
  }
  else	/* Just one disk in the system */
    RecordBoot (uFlag[0]);
}
