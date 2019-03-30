#include <JEO:JEO.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include "VE_Prototypes.h"
#include "VE_Install.w"
#include <JEO:raw.h>
#include "VE_Window.h"

#define X	162
#define Y	 24

BOOL installFlag[5];
struct Window *w3 = 0;
struct RastPort *rp3;

extern UBYTE *Dummy;
extern UBYTE *Drive[];
extern ULONG recCount;

VOID InstallGadget (UWORD nr, BOOL flag)
{
  switch (flag)
  {
    case ON:
    {
      switch (nr)
      {
        case 2: if (!(InstallIntGD.Flags & SELECTED))  { InstallIntGD.Flags  ^= SELECTED; RefreshGList (&InstallIntGD,  w3, NULL, 1); } break;
        case 3:	/* Directory Cache */
        {
          if (InstallIntGD.Flags & SELECTED)		/* Is it on? */
          {
            InstallIntGD.Flags ^= SELECTED;		/* Turn off */
            RefreshGList (&InstallIntGD,  w3, NULL, 1);
            installFlag[2] ^= 1;
          }
          OffGadget (&InstallIntGD, w3, NULL);
          if (!(InstallDCGD.Flags & SELECTED))
          {
            InstallDCGD.Flags ^= SELECTED;
            RefreshGList (&InstallDCGD,  w3, NULL, 1);
            installFlag[3] ^= 1;
          }
          break;
        }
        case 4:			/* Fra Database ON */
        {
          if (InstallIntGD.Flags & SELECTED)
          {
            InstallIntGD.Flags ^= SELECTED;
            RefreshGList (&InstallIntGD,  w3, NULL, 1);
            installFlag[2] ^= 1;
          }
          if (InstallDCGD.Flags & SELECTED)
          {
            InstallDCGD.Flags ^= SELECTED;
            RefreshGList (&InstallDCGD,  w3, NULL, 1);
            installFlag[3] ^= 1;
          }
          OffGadget (&InstallFFSGD, w3, NULL);
          OffGadget (&InstallIntGD, w3, NULL);
          OffGadget (&InstallDCGD, w3, NULL);
          if (!(InstallFraDatabaseGD.Flags & SELECTED))
          {
            InstallFraDatabaseGD.Flags ^= SELECTED;
            RefreshGList (&InstallFraDatabaseGD,  w3, NULL, 1);
            installFlag[1] ^= 1;
          }
          break;
        }
      }
      break;
    }
    case OFF:
    {
      switch (nr)
      {
        case 2: if (InstallIntGD.Flags & SELECTED)  { InstallIntGD.Flags  ^= SELECTED; RefreshGList (&InstallIntGD,  w3, NULL, 1); } break;
        case 3:
        {
          OnGadget (&InstallIntGD, w3, NULL);
          if (InstallDCGD.Flags & SELECTED)
          {
            InstallDCGD.Flags ^= SELECTED;
            RefreshGList (&InstallDCGD,  w3, NULL, 1);
          }
          break;
        }
        case 4:			/* Fra Database OFF */
        {
          OnGadget (&InstallFFSGD, w3, NULL);
          if (installFlag[1])
            JEOWrite (rp3, X, Y + OFFY, "FFS", BLACK);
          else
            JEOWrite (rp3, X, Y + OFFY, "OFS", BLACK);
          if (!installFlag[3])
            OnGadget (&InstallIntGD, w3, NULL);
          if (!installFlag[2])
            OnGadget (&InstallDCGD, w3, NULL);
          if (InstallFraDatabaseGD.Flags & SELECTED)
          {
            InstallFraDatabaseGD.Flags ^= SELECTED;
            RefreshGList (&InstallFraDatabaseGD,  w3, NULL, 1);
          }
          break;
        }
      }
      break;
    }
  }
}

WORD InstallGadgetUp (APTR Address)
{
  UWORD nr;

  nr = ((struct Gadget *)Address)->GadgetID;
  switch (nr)
  {
    case 1:			/* Fil system */
    {
      if (installFlag[1])
      {
        JEOWrite (rp3, X, Y + OFFY, "OFS", BLACK);
        installFlag[nr] ^= 1;
      }
      else
      {
        JEOWrite (rp3, X, Y + OFFY, "FFS", BLACK);
        installFlag[nr] ^= 1;
      }
      return (FALSE);
    }
    case 0:			/* Avbryt */
      return (ERROR);
    case 5:			/* Install */
      return (TRUE);
  }
  return (FALSE);
}

VOID InstallGadgetDown (APTR Address)
{
  UWORD nr;

  nr = ((struct Gadget *)Address)->GadgetID;
  installFlag[nr] ^= 1;
  InstallGadget (nr, installFlag[nr]);
}

BOOL InstallDrive (WORD unit)
{
  LONG error;

  if (installFlag[4])	/* Fra Database */
  {
    if (LoadBaseBoot ())
      error = Install (unit, DATABASE);
    else
    {
      Status (GLS (&L_USER_BREAK));
      return (FALSE);
    }
  }
  else if ((installFlag[3]) AND (installFlag[1]))	/* DC + FFS */
    error = Install (unit, NORMAL_FFS_DC);
  else if (installFlag[3])				/* DC + OFS */
    error = Install (unit, NORMAL_OFS_DC);
  else if ((installFlag[2]) AND (installFlag[1]))	/* INT + FFS */
    error = Install (unit, NORMAL_FFS_INT);
  else if (installFlag[2])				/* INT + OFS */
    error = Install (unit, NORMAL_OFS_INT);
  else if (installFlag[1])				/* FFS */
    error = Install (unit, NORMAL_FFS);
  else							/* OFS */
    error = Install (unit, NORMAL_OFS);

  if (!error)
  {
    VE_ReadBoot (unit);
    VE_CheckBoot (unit);
    return (TRUE);
  }
  else
  {
    PlaySound (S_ERROR);
    tderror (error, unit);
		DiskStatus (TDErrorMsg, unit);
		Status (GLS (&L_READY));
    return (FALSE);
  }
}

VOID InitInstallWindow (VOID)
{
	InstallNewWindowStructure1.Height += OFFY;
	InstallFraDatabaseGD.TopEdge += OFFY;
	InstallDCGD.TopEdge += OFFY;
	InstallIntGD.TopEdge += OFFY;
	InstallFFSGD.TopEdge += OFFY;
	InstallInstallGD.TopEdge += OFFY;
	InstallAvbrytGD.TopEdge += OFFY;
}

BOOL FInstall (WORD unit)
{
   ULONG Signals;
   ULONG class;
   USHORT Code;
   APTR Address;
   struct IntuiMessage *message;
   UWORD y = 27, x = 15;
   WORD flag;
   BOOL ret;

  if (drive[unit] == ERROR)	/* Just in case */
    return (FALSE);

  All (OFF);
  Status (GLS (&L_PLEASE_WAIT));

  SPrintF (Dummy, GLS (&L_INSTALL_TO), Drive[unit]);
  InstallNewWindowStructure1.Screen = Scr;
  InstallNewWindowStructure1.Title = Dummy;

  InstallNewWindowStructure1.LeftEdge = (Scr->Width / 2) - (InstallNewWindowStructure1.Width / 2);
  InstallNewWindowStructure1.TopEdge = (Scr->Height / 2) - (InstallNewWindowStructure1.Height / 2);

  ret = TRUE;

	mystrcpy (i_Install, GLS (&L_WINSTALL));
	mystrcpy (i_Cancel, GLS (&L_WCANCEL));

  if (!(w3 = OpenWindow (&InstallNewWindowStructure1)))
  {
    Status (GLS (&L_ERROR_OPENING_WINDOW));
    ret = FALSE;
    goto Close;
  }
  rp3 = w3->RPort;
	if (textfont)
		SetFont (rp3, textfont);

  if (!recCount)	/* Any entries? */
    OffGadget (&InstallFraDatabaseGD, w3, NULL);
  else
    OnGadget (&InstallFraDatabaseGD, w3, NULL);

  if (!installFlag[4])	/* Er Fra Database på */
  {
    if (installFlag[1])
      JEOWrite (rp3, X, Y + OFFY, "FFS", BLACK);
    else
      JEOWrite (rp3, X, Y + OFFY, "OFS", BLACK);
  }

  JEOWrite (rp3, x, y - 3 + OFFY, GLS (&L_FILE_SYSTEM), BLACK);
  y += 12;
  JEOWrite (rp3, x, y + OFFY, GLS (&L_INT_MODUS), BLACK);
  y += 12;
  JEOWrite (rp3, x, y + OFFY, GLS (&L_DS_MODUS), BLACK);
  y += 17;
  JEOWrite (rp3, x, y + OFFY, GLS (&L_FROM_DATABASE), BLACK);

  FOREVER 
  {
    Signals = Wait (1<<w3->UserPort->mp_SigBit);
    if (Signals & (1<<w3->UserPort->mp_SigBit)) 
    {
      while (message = (struct IntuiMessage *)GetMsg (w3->UserPort)) 
      {
        class = message->Class;
        Code = message->Code;
        Address = message->IAddress;
        ReplyMsg ((struct Message *)message);

        switch (class)
        {
          case CLOSEWINDOW:
          {
            Status (GLS (&L_READY));
            ret = FALSE;
            goto Close;
          }
          case RAWKEY:
          {
            if (Code == ESC)
            {
	            Status (GLS (&L_READY));
              ret = FALSE;
              goto Close;
            }
            break;
          }
          case GADGETUP:
          {
            flag = InstallGadgetUp (Address);
            if (flag == TRUE)	/* Install */
            {
              CloseWindow (w3);
              w3 = 0;
              if (InstallDrive (unit))
                Status (GLS (&L_DISK_INSTALLED));
              else
                ret = FALSE;
              goto Close;
            }
            else if (flag == FALSE)	/* Fil system */
              break;
            else		/* Avbryt */
            {
	            Status (GLS (&L_READY));
              ret = FALSE;
              goto Close;
            }
          }
          case GADGETDOWN:
          {
            InstallGadgetDown (Address);	/* Select */
            break;
          }
        }
      }
    }
  }
Close:
  if (w3)
  {
    CloseWindow (w3);
    w3 = 0;
  }
  All (ON);
  return (ret);
}
