#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <exec/lists.h>
#include <proto/graphics.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <intuition/gadgetclass.h>
#include "VE_Prototypes.h"
#include "VE_Window.h"
#include "FW_DirWindow.dummy"
#include <JEO:raw.h>
#include "PatchWindow.h"

ULONG add_to_watch;

char VE_fw[300];
VOID fw_ScanDir (char *Dir, ULONG count, BOOL mode);
BOOL s_DeleteOne1 (char *Name);
VOID p_All (BOOL mode);
VOID SavePrefs (VOID);

ULONG last_del_val = 0;
ULONG last_top = 0;
typedef struct
{
  struct List list;
  ULONG howmany;
  ULONG checkSum;
} s_Sjef;

s_Sjef s_sjef;

ULONG s_len;
ULONG s_count_len;
BOOL s_save_flag;

MD5Data Calculate_MD5_from_file (char *FileName)
{
	BPTR fh;
	char *Buffer;
	int size;
	MD5Data md5;

	size = FileSize (FileName);
	if (size > 0)
	{
		if (Buffer = AllocMem (size, MEMF_CLEAR))
		{
			if (fh = Open (FileName, MODE_OLDFILE))
			{
				Read (fh, Buffer, size);
				Close (fh);
				md5 = Calculate_MD5 (Buffer, size);
			}
			FreeMem (Buffer, size);
		}
	}
	return (md5);
}

// *****************************************************************************
BOOL HandleFWIDCMP (VOID)
{
	struct IntuiMessage	*m;
	UWORD	code;
 	ULONG class;
	struct IntuiMessage	tmpmsg;
  BOOL ret = TRUE;

	while (m = GT_GetIMsg (PatchWnd->UserPort))
	{
		CopyMem ((char *)m, (char *)&tmpmsg, (long)sizeof(struct IntuiMessage));
		GT_ReplyIMsg (m);
		code = tmpmsg.Code;
		class = tmpmsg.Class;
		switch (tmpmsg.Class)
		{
			case IDCMP_DISKINSERTED:
				DiskInserted ();
				Status (GLS (&L_PLEASE_WAIT));
				break;
			case IDCMP_DISKREMOVED:
				DiskRemoved ();
				Status (GLS (&L_PLEASE_WAIT));
				break;
			case	IDCMP_RAWKEY:
			{
				switch (code)
				{
					case ESC:
						ret = FALSE;
						break;
					case RETURN:
						ret = FALSE;
						break;
					case ENTER:
						ret = FALSE;
						break;
				}
				break;
			}
			case	IDCMP_CLOSEWINDOW:
			{
				ret = FALSE;
				break;
			}
			case	IDCMP_GADGETUP:
			{
			  struct Gadget *gadget;
				UWORD nr;

			  gadget = (struct Gadget *)tmpmsg.IAddress;
				nr = gadget->GadgetID;

			  switch (nr)
			  {
					case GD_PATCHLIST:
					{
						struct Node *nd;
					  s_Base *bp;
						UWORD n;

						nd = s_sjef.list.lh_Head;
						for (n = 0; n < tmpmsg.Code; n++)
							nd = nd->ln_Succ;

					  bp = (s_Base *)nd;
					  last_del_val = n;
						SPrintF (Dummy, GLS (&L_REMOVE_FROM_LIST), bp->FileName);
						p_All (OFF);
						if (JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_REMOVE_CANCEL), S_MESSAGE))
						{
							if (s_DeleteOne1 (bp->FileName))
							{
								GT_SetGadgetAttrs (PatchGadgets[GD_PATCHLIST], PatchWnd, NULL , GTLV_Labels,
									&s_sjef.list, TAG_DONE, NULL);
								s_save_flag = TRUE;
							}
						}
						p_All (ON);
						ClearIntuition (PatchWnd);

					  break;
					}				
					case GD_EXIT:
					{
						ret = FALSE;
						break;
					}
				}
				break;
			}
		}
	}
	return (ret);
}

VOID RemoveFW (VOID)
{
	ULONG	waitsigs, gotsigs;
	BOOL quit = 1;
	ULONG diff;

	if (s_len)	// Noen her?
	{
		diff = s_len;
		if (!OpenPatchWindow ())
		{
			SetWindowTitles (PatchWnd, GLS (&L_SELECT_FILE_TO_REMOVE), Ver);
			GT_SetGadgetAttrs (PatchGadgets[GD_PATCHLIST], PatchWnd, NULL , GTLV_Labels,
				&s_sjef.list, GTLV_Selected, last_del_val, GTLV_Top, last_top, TAG_DONE, NULL);
			waitsigs = (1L << PatchWnd->UserPort->mp_SigBit);

			while (quit)
			{
				gotsigs = Wait (waitsigs);
				quit = HandleFWIDCMP ();
			}
			if (PatchWnd)
				ClosePatchWindow ();
		}

		if (s_save_flag AND (diff != s_len))
		{
			if  (JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_FILE_WATCH_CHANGED), GLS (&L_SAVE_CANCEL), S_MESSAGE))
				s_SaveFiles ();
		}		
	}
	else
		JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_PLEASE_ADD_FIRST), GLS (&L_OK), S_MESSAGE);
}
// *****************************************************************************

VOID s_SlettBase (VOID)
{
  ULONG s;
  struct Node *nd;

  nd = s_sjef.list.lh_Head;
  for (s = 0; s < s_len; s++)
  {
    Remove (nd);
 	  FreeMem ((UBYTE *)nd, sizeof (s_Base));
    nd = 0;
    nd = s_sjef.list.lh_Head;
  }
  s_len = 0;
}

BOOL s_fib_open = OFF;

VOID s_fib_Open (VOID)
{
  s_len = 0;
  s_count_len = 0;
	s_save_flag = FALSE;
  NewList (&s_sjef.list);
  s_fib_open = ON;
}

VOID s_fib_Close (VOID)
{
  s_Base *bp;

	if (s_fib_open)
	{
	  s_SlettBase ();
 		bp = (s_Base *)s_sjef.list.lh_Head;
	  while (bp = (s_Base *)RemHead (&s_sjef.list))
 		{
	    FreeMem (bp, sizeof (s_Base));
 		  bp = 0;
		}
	}
}

struct Node *s_FindTheOne (UBYTE *Name)
{
  struct Node *nd;

  nd = s_sjef.list.lh_Head;
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

BOOL s_DeleteOne (char *Name)
{
  struct Node *nd;
  s_Base *bp;

  if (nd = FindName (&s_sjef.list, Name)) 
  {
    bp = (s_Base *)nd;
		bp->FileName[0] = 0;
		s_save_flag = TRUE;

    return (TRUE);
  }
  else
    return (FALSE);
}

BOOL s_DeleteOne1 (char *Name)
{
  struct Node *nd;

  if (nd = FindName (&s_sjef.list, Name)) 
  {
    Remove (nd);
    FreeMem ((UBYTE *)nd, sizeof (s_Base));
    nd = 0;
		s_count_len--;
		s_len--;
		s_save_flag = TRUE;
    return (TRUE);
  }
  else
    return (FALSE);
}

BOOL s_CheckFileNameExist (char *FileName)
{
  struct Node *nd;
  s_Base *bp;

  nd = s_sjef.list.lh_Head;
  bp = (s_Base *)nd;
  while (nd->ln_Succ)
  {
    if (!(stricmp (bp->FileName, FileName)))
      return (TRUE);
    nd = nd->ln_Succ;
    bp = (s_Base *)nd;
  }
  return (FALSE);
}

VOID s_Insert (BOOL mode, char *FileName, LONG size, MD5Data md5)
{
  s_Base *bp = NULL;
  struct Node *nd;

	if (!s_CheckFileNameExist (FileName))
	{
		if (mode)
		{
			SPrintF (Dummy, GLS (&L_ADDING_TO_FILE_WATCH), FileName);
			Status (Dummy);
		}
	  if (bp = AllocMem (sizeof (s_Base), MEMF_CLEAR))
  	{
	    strcpy (bp->FileName, FileName);
  	  bp->size = size;
			strcpy (bp->md5.md5, md5.md5);
    	bp->nd.ln_Name = bp->FileName;
	    if (!(nd = s_FindTheOne (bp->FileName)))
  	    AddTail (&s_sjef.list, &bp->nd);
    	else
	      Insert (&s_sjef.list, &bp->nd, nd);

			add_to_watch++;
  	  s_len++;
  	  s_count_len++;
	  }
	}
}

char PathList[][23] = 
{
	"C:",
	"RAM:"
	"SYS:Utilities/",
	"SYS:Rexxc/",
	"SYS:System/",
	"SYS:S/",
	"SYS:Prefs/",
	"SYS:WBStartup/",
	"SYS:Tools/",
	"SYS:Tools/Commodities/"
};

#define MAX_PATH (sizeof (PathList) / 23)

char ExcludeList[][9] =
{
	"Alias",
	"Ask",
	"CD",
	"Echo",
	"Else",
	"EndCLI",
	"EndIf",
	"EndShell",
	"EndSkip",
	"Failat",
	"Fault",
	"Get",
	"Getenv",
	"If",
	"Lab",
	"NewCLI",
	"NewShell",
	"Path",
	"Prompt",
	"Quit",
	"Resident",
	"Run",
	"Set",
	"Setenv",
	"Skip",
	"Stack",
	"Unalias",
	"Unset",
	"Unsetenv",
	"Why",
	".ket",
	".bra",
	".key"
};

#define MAX_EXCLUDE (sizeof (ExcludeList) / 9)

/*
VOID GetPath (VOID)
{
	BPTR fh;

	Execute ("Path >T:VE_PathList", NULL, NULL);
	pathsize = FileSize ("T:VE_PathList");

	if (pathsize > 0)
	{
		if (PathList = AllocMem (pathsize, MEMF_CLEAR))
		{
			if (fh = Open ("T:VE_PathList", MODE_OLDFILE))
			{
				Read (fh, PathList, pathsize);
				Close (fh);
//				printf ("%s\n", PathList);
			}
		}
	}
}
*/

VOID Add_file (BOOL mode, char *Filename, ULONG size)
{
	UWORD i;

	if (!size)
		size = FileSize (Filename);
	if (size < 0)	// Not found / error
	{
		for (i = 0; i < MAX_PATH; i++)
		{
			strcpy (Dummy, PathList[i]);
			strcat (Dummy, Filename);
			size = FileSize (Dummy);
			if (size > 0)
			{
				mystrcpy (Filename, Dummy);
				break;
			}
		}
	}
	if (size > 0)
	{
		MD5Data md5;
		md5 = Calculate_MD5_from_file (Filename);
		s_Insert (mode, Filename, size, md5);
	}
}

VOID Check_changed (VOID)
{
  ULONG fc;
  struct Node *nd;
  s_Base *bp;
  LONG size, diff;
  MD5Data md5;
  BOOL s_delete_flag = FALSE;

	Status (GLS (&L_PLEASE_WAIT));
	if (!s_count_len)
	{
		JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NO_FILES_TO_CHECK), GLS (&L_OK), S_MESSAGE);
		Status (GLS (&L_READY));
		return;
	}

  fc = 0;
  nd = s_sjef.list.lh_Head;
  bp = (s_Base *)nd;
	s_save_flag = FALSE;

  while (nd->ln_Succ)
	{
    JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, fc + 1, s_count_len, BLUE);

		if (CheckESC (VirusWnd))
		{
			if (JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_WATCH_CHECK_PAUSED), GLS (&L_CONTINUE_STOP), S_NO_SOUND))
				All (OFF);
			else
			{
	      Status (GLS (&L_USER_BREAK));
	 	    break;
			}
		}

		if (*bp->FileName)
		{
			SPrintF (Dummy, GLS (&L_FW_CHECKING), bp->FileName);
			Status (Dummy);
			size = FileSize (bp->FileName);
			diff = size - bp->size;

			if (size == -1)	// Not Found
			{
				SPrintF (Dummy, GLS (&L_FILE_DOESNT_EXIST), bp->FileName);
				if (JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_REMOVE_FROM_LIST_CONT), S_ERROR))
				{
					All (OFF);
					if (s_DeleteOne (bp->FileName))
					{
						s_save_flag = TRUE;
						s_delete_flag = TRUE;
					}
				}
			}
			else
			{
				md5 = Calculate_MD5_from_file (bp->FileName);
		
				if (diff)
				{
					SPrintF (Dummy, GLS (&L_FILE_OLD_NEW_DIFF), bp->FileName, bp->size, size, diff);
					if (JEOSoundRequest (GLS (&L_FILE_SIZE_CHANGED), Dummy, GLS (&L_ACCEPT_CHANGE_CONTINUE), S_ERROR))
					{
						s_save_flag = TRUE;
						bp->size = size;
						md5 = bp->md5 = Calculate_MD5_from_file (bp->FileName);
					}
					All (OFF);
				}
				else if (stricmp (bp->md5.md5, md5.md5))	// HEY!
				{
					SPrintF (Dummy, GLS (&L_FILE_MD5_OLD_NEW), bp->FileName, bp->md5.md5, md5.md5);	// Fool
					if (JEOSoundRequest (GLS (&L_MD5_CHANGED), Dummy, GLS (&L_ACCEPT_CHANGE_CONTINUE), S_ERROR))
					{
						s_save_flag = TRUE;
						bp->size = size;	// YES!
						bp->md5 = Calculate_MD5_from_file (bp->FileName);
					}
					All (OFF);
				}
			}
		}
    nd = nd->ln_Succ;
 	  bp = (s_Base *)nd;
    fc++;
  }
	if (s_delete_flag)
	{
		 struct Node *nd1;

	  nd = s_sjef.list.lh_Head;
	  bp = (s_Base *)nd;

	  while (nd->ln_Succ)
	  {
			if (bp->FileName[0] == 0)
			{
		    nd1 = nd->ln_Succ;
				Remove (nd);
				FreeMem ((UBYTE *)nd, sizeof (s_Base));
				nd = 0;
				s_count_len--;
				s_len--;
				nd = nd1;
			}
			else
		    nd = nd->ln_Succ;

 		  bp = (s_Base *)nd;
		}
	}

	if (s_save_flag)
	{
		if  (JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_FILE_WATCH_CHANGED), GLS (&L_SAVE_CANCEL), S_MESSAGE))
			s_SaveFiles ();
	}

  SetAPen (VirusWnd->RPort, 0);
	ClearBar ();
	Status (GLS (&L_READY));
}

/*
VOID s_ListFiles (VOID)
{
  ULONG fc;
  struct Node *nd;
  s_Base *bp;

	if (!s_count_len)
	{
		JEOSoundRequest (GLS (&L_MESSAGE), "No files to list!", GLS (&L_OK), S_MESSAGE);
		return;
	}

	printf ("\n");
	printf ("Cur size MD5            File name\n");
	printf ("-------- -------        ---------\n");

  fc = 0;
  nd = s_sjef.list.lh_Head;
  bp = (s_Base *)nd;
  while (nd->ln_Succ)
  {
 		printf ("%8ld MD5: %s '%s'\n", bp->size, bp->md5.md5, bp->FileName);

    nd = nd->ln_Succ;
    bp = (s_Base *)nd;
    fc++;
  }
	printf ("\n");
}
*/

VOID Add_files (VOID)
{
	int i, j;
	BPTR fh;
	char Hold[500];
	int size;
	UBYTE a;
	BOOL inside, start, skip;
	ULONG old_nr;
	char Startup[][19] = {"s:startup-sequence", "s:user-startup"};

	Status (GLS (&L_PLEASE_WAIT));

	old_nr = s_count_len;
	for (a = 0; a < 2; a++)
	{
		size = FileSize (Startup[a]);
		if (size > 0)
		{
			char *Buf;

			if (Buf = AllocMem (size, MEMF_CLEAR))
			{
				if (fh = Open (Startup[a], MODE_OLDFILE))
				{
					Read (fh, Buf, size);
					Close (fh);

					inside = skip = start = FALSE;
					for (i = 0, j = 0; i < size; i++)
					{
						if (Buf[i] == '\"')
						{
							if (inside)
								inside = FALSE;
							else
								inside = TRUE;
							continue;
						}
						if (Buf[i] == '\n')	// Ny linje
						{
							inside = skip = start = FALSE;
							Hold[j] = 0;						
							j = 0;
							if (*Hold)
							{
								BOOL go_flag = TRUE;
								int k;

								for (k = 0; k < MAX_EXCLUDE; k++)
								{
									if (!(stricmp (Hold, ExcludeList[k])))
									{
										go_flag = FALSE;
										break;
									}
								}
								if (go_flag)							
								{
									char Tmp[200];

									for (k = 0; k < MAX_EXCLUDE; k++)
									{
										mystrcpy (Tmp, "C:");
										strcat (Tmp, ExcludeList[k]);
										if (!(stricmp (Hold, Tmp)))
										{
											go_flag = FALSE;
											break;
										}
									}
								}
							
								if (go_flag)							
								{
									BPTR lock;
	
									if (lock = Lock (Hold, ACCESS_READ))
									{
										NameFromLock (lock, Hold, 500);
										UnLock (lock);
										Add_file (1, Hold, 0);
									}
								}
							}
							continue;
						}
						if (skip)	// Skal vi skippe???
							continue;
						if ((Buf[i] == ';') AND (inside == FALSE))
						{
							skip = TRUE;
							continue;
						}
						if (Buf[i] == ' ')
						{
							if (!inside)
							{
								if (start)
								{
									start = FALSE;
									skip = TRUE;
								}
							}
						}
						else
							start = TRUE;
						if (start)
						{
							Hold[j] = Buf[i];
							j++;
						}
					}
				}
				FreeMem (Buf, size);
			}
		}
	}
	SPrintF (Hold, "SYS:WBStartup");
	fw_ScanDir (Hold, s_count_len - old_nr, 0);
}

VOID FWActivateGadget (UWORD nr)
{
  ActivateGadget (FWDIRGadgets[nr], FWDIRWnd, NULL);
}

VOID FWInitGadget (UWORD num, LONG tagtype, LONG tagvalue)
{
  GT_SetGadgetAttrs (FWDIRGadgets[num], FWDIRWnd, NULL, tagtype, tagvalue, TAG_DONE);
}

VOID s_All (BOOL mode)
{
	if (!mode)
		rtSetWaitPointer (FWDIRWnd);
	else
		ClearPointer (FWDIRWnd);
}

VOID p_All (BOOL mode)
{
	if (!mode)
		rtSetWaitPointer (PatchWnd);
	else
		ClearPointer (PatchWnd);
}

VOID Add_files_from_dir (ULONG count, BOOL mode)
{
  ULONG fc;
  struct Node *nd;
  Filbase *bp;
  BOOL go_flag = TRUE;
	ULONG added, old_nr;
	char Hold[200];

  fc = 0;
  nd = fib_sjef.list.lh_Head;
  bp = (Filbase *)nd;
	old_nr = s_count_len;
  while (nd->ln_Succ)
  {
		if (!Check_file_intuition ())
			go_flag = FALSE;
		if (!go_flag)
			break;

		if (mode)
	    JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, fc + 1, fib_len, BLUE);
		if (!s_CheckFileNameExist (bp->FileName))	// Allready added?
		{
			if (prefs->fw_confirm)	// Confirm every file?
			{
				UWORD nr;
				UWORD len;

				len = strlen (bp->FileName);
				switch (bp->flag)
				{
					case 1:	mystrcpy (Hold, GLS (&L_EXE));	break;
					case 2:	mystrcpy (Hold, GLS (&L_DATA));	break;
					case 3:	mystrcpy (Hold, GLS (&L_ICON));	break;
					default: 	mystrcpy (Hold, GLS (&L_FW_ERROR));	break;
				}

				SPrintF (Dummy, GLS (&L_FILE_NAME_SIZE_TYPE), bp->FileName, bp->size, Hold);
				nr = JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_ADD_FILE_CANCEL_SKIP), S_NO_SOUND);
				if (mode)
					s_All (OFF);
				switch (nr)
				{
					case 1:	// Add file
						Add_file (1, bp->FileName, 0);
						break;
					case 2:	// Cancel
						go_flag = FALSE;
						break;
				}
			}
			else
				Add_file (1, bp->FileName, 0);
		}

    nd = nd->ln_Succ;
    bp = (Filbase *)nd;
    fc++;
  }
  fib_SlettBase ();	// Sletter filene i dirbuffer!

	added = s_count_len - old_nr;
	added += count;	// fra s:startup's
	if (added)
	{
		s_save_flag = TRUE;
		mystrcpy (Hold, GLS (&L_SAVE_CONTINUE));
	}
	else
		mystrcpy (Hold, GLS (&L_OK));

	Status (GLS (&L_PLEASE_WAIT));

	SPrintF (Dummy, GLS (&L_ADDED_FILES_TO_WATCH), added, added == 1 ? GLS (&L_FILE) : GLS (&L_FILES));
	if (JEOSoundRequest (GLS (&L_MESSAGE), Dummy, Hold, S_MESSAGE))
		s_SaveFiles ();

	if (mode)
		ClearBar ();

	Status (GLS (&L_READY));
}

VOID fw_ScanDir (char *Dir, ULONG count, BOOL mode)
{
  struct FileInfoBlock *fib = 0;
  ULONG success;
  BOOL ret;
  BPTR lock;

  Status (GLS (&L_PLEASE_WAIT));
  fib = AllocMem (sizeof (struct FileInfoBlock), MEMF_CLEAR);

  fib_Open ();
	if (lock = Lock (Dir, ACCESS_READ))
	{
 		if (success = Examine (lock, fib))
		{
	  	BOOL go_flag;
	  	BPTR fh;
	  	ULONG Buffer[1];

			for (;;)
			{
				ret = Check_file_intuition ();
				if (ret == FALSE)
					break;

				success = ExNext (lock, fib);	// FindNext File/Dir

			  if (!success)				// End of directory?
	  			break;

 			  if (fib->fib_DirEntryType < 0)	// A file?
 			  {
					ULONG flag;

					go_flag = FALSE;

					NameFromLock (lock, Dir, 500);
				 	if ((Dir[strlen (Dir) - 1] != ':') AND (Dir[strlen (Dir) - 1] != '/'))
						strcat (Dir, "/");

				  SPrintF (FullName, "%s%s", Dir, fib->fib_FileName);
//					printf ("'%s'\n", FullName);

					flag = 0;

					Buffer[0] = 0;
					if (fh = Open (FullName, MODE_OLDFILE))	// Opening the file...
					{
						Read (fh, &Buffer, 4);
						Close (fh);
					}
					if (Buffer[0] == 0x000003F3)	// Always
					{
						flag = 1;
						go_flag = TRUE;
					}
					if (!go_flag)	// Ikke en exe
					{
						if (mode)	// Vi skal ikke ta med data filer i startup
						{
							if (prefs->fw_include_data)	// Include data?
							{
								go_flag = TRUE;

								flag = 2;
								if (prefs->fw_exclude_icons)	// Exclude icons
								{
									if (Buffer[0] == 0xE3100001)	// Icon found!
									{
										flag = 3;
										go_flag = FALSE;
									}
								}
								else if (Buffer[0] == 0xE3100001)	// Just for report back
									flag = 3;
							}
						}
					}
					if (go_flag)
				  	fib_Insett (FullName, fib->fib_Size, flag);
			  }
	    }
  	}
 		UnLock (lock);
	}
	FreeMem (fib, sizeof (struct FileInfoBlock));
	if (fib_len)
		Add_files_from_dir (count, mode);
	else
	{
		SPrintF (Dummy, GLS (&L_ADDED_FILES_TO_WATCH), count, count == 1 ? GLS (&L_FILE) : GLS (&L_FILES));
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
		Status (GLS (&L_READY));
	}

  fib_Close ();
}

VOID s_AddDir (VOID)
{
	ULONG Signals;
	ULONG class;
	USHORT Code;
	APTR Address;
	struct IntuiMessage *message;
	BOOL go_flag;
	char Hold[500] = "";

  Status (GLS (&L_PLEASE_WAIT));
	if (OpenFWDIRWindow ())
		return;

	if (prefs->fw_include_data)
		FWInitGadget (GD_INCLUDE_DATA, (GTCB_Checked), ON);
	else
 	  FWInitGadget (GD_EXCLUDE_ICONS, GA_Disabled, TRUE);
	if (prefs->fw_exclude_icons)
		FWInitGadget (GD_EXCLUDE_ICONS, (GTCB_Checked), ON);
	if (prefs->fw_confirm)
		FWInitGadget (GD_CONFIRM, (GTCB_Checked), ON);
	
	go_flag = TRUE;
	while (go_flag)
	{
		Signals = Wait (1 << FWDIRWnd->UserPort->mp_SigBit);

		if (Signals & (1<<FWDIRWnd->UserPort->mp_SigBit))
		{
			while (message = (struct IntuiMessage *)GetMsg (FWDIRWnd->UserPort))
			{
        class = message->Class;
        Code = message->Code;
        Address = message->IAddress;
        ReplyMsg ((struct Message *)message);
		
				switch (class)
				{
					case IDCMP_RAWKEY:
					{
						switch (Code)
						{
							case ESC:
								go_flag = FALSE;
								break;
						}
						break;
					}
					case IDCMP_CLOSEWINDOW:
					{
						go_flag = FALSE;
						break;
					}
					case	IDCMP_GADGETUP:
					{
					  struct Gadget *gadget;
						UWORD nr;

					  gadget = (struct Gadget *)Address;
						nr = gadget->GadgetID;

						switch (nr)
						{
							case GD_INCLUDE_DATA:
							{
								prefs->fw_include_data ^= 1;
								if (prefs->fw_include_data)
							    FWInitGadget (GD_EXCLUDE_ICONS, GA_Disabled, FALSE);
							  else
						  	  FWInitGadget (GD_EXCLUDE_ICONS, GA_Disabled, TRUE);
								break;
							}
							case GD_EXCLUDE_ICONS:
								prefs->fw_exclude_icons ^= 1;
								break;
							case GD_CONFIRM:
								prefs->fw_confirm ^= 1;
								break;
							case GD_DONE:
								go_flag = FALSE;
								break;
							case GD_ADD:
							{
								s_All (OFF);
								if (*prefs->FWDirPath)	// Er den ok?
								{
									if (Exists (prefs->FWDirPath))	// F eks df0: som er tatt ut!
										mystrcpy (Hold, prefs->FWDirPath);
								}
								else
									strcpy (Hold, "C:");

								if (GetDirName (Hold, GLS (&L_SELECT_A_DIRECTORY), NULL))
								{
									mystrcpy (prefs->FWDirPath, Hold);
								  fw_ScanDir (Hold, 0, 1);
								}
								ClearIntuition (FWDIRWnd);
								s_All (ON);
								break;
							}
						}
						break;
					}
					case	IDCMP_GADGETDOWN:
					{
					  struct Gadget *gadget;
						UWORD nr;

					  gadget = (struct Gadget *)Address;
						nr = gadget->GadgetID;
//				  	SPrintF (Dummy, "GADGETDOWN %ld\n", nr);
//				  	JEOReqRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK));

						break;
					}	
				}
			}
		}
	}
	
	CloseFWDIRWindow ();
  Status (GLS (&L_READY));
}

#define MEGA (i * crypt_size - ((b + (c * 5)) ^ 4) / (3 + (crypt_size + 3) * 4) + (d * 2))
#define CRYPT		0
#define DECRYPT	1

VOID s_Crypt (BOOL mode, UBYTE *Buffer, LONG crypt_size)
{
  ULONG i, a, b, c, d , e;

  a = b = c = d = e = 1;
  for (i = 0; i < crypt_size; i++)
  {
		if (mode == DECRYPT)
	    Buffer[i] = Buffer[i] - MEGA;
	  else
	    Buffer[i] = Buffer[i] + MEGA;
  
    a = b = c = d = e += 1;
    if (a == 9)
      a = 0;
    if (b == 20)
      b = 0;
    if (c == 4)
      c = 0;
    if (d == 3)
      d = 0;
    if (e == 8)
      e = 0;
  }
}

typedef struct
{
	UWORD head_size;
  char FileName[256];
  LONG size;
  MD5Data md5;
} s_tmp_Base;

s_tmp_Base s_tmp_base;

char Watch_ID[] = "VEFW#1";

VOID s_SaveFiles (VOID)
{
  struct Node *nd;
  s_Base *bp;
  BPTR fh;

	if (s_save_flag)
	{
		All (OFF);
		ClearBar ();
		Status (GLS (&L_SAVING_WATCH));
		if (fh = Open (VE_fw, MODE_NEWFILE))
		{
			UWORD i = 0;

			strcpy (Dummy, Watch_ID);
			Write (fh, Dummy, 6);

		  nd = s_sjef.list.lh_Head;
  		bp = (s_Base *)nd;
	  	while (nd->ln_Succ)
	  	{
				if (*bp->FileName)
				{
					s_tmp_base.head_size = strlen (bp->FileName) + 1;

					s_Crypt (CRYPT, (UBYTE *)&s_tmp_base.head_size, sizeof (UWORD));
					Write (fh, &s_tmp_base.head_size, sizeof (UWORD));	

					mystrcpy (s_tmp_base.FileName, bp->FileName);
					s_Crypt (CRYPT, (UBYTE *)&s_tmp_base.FileName, strlen (bp->FileName) + 1);
	  			Write (fh, &s_tmp_base.FileName, strlen (bp->FileName) + 1);

					s_tmp_base.size = bp->size;
					s_Crypt (CRYPT, (UBYTE *)&s_tmp_base.size, sizeof (LONG));
	  			Write (fh, &s_tmp_base.size, sizeof (LONG));

					strcpy (s_tmp_base.md5.md5, bp->md5.md5);
					s_Crypt (CRYPT, (UBYTE *)&s_tmp_base.md5, sizeof (MD5Data));
	  			Write (fh, &s_tmp_base.md5, sizeof (MD5Data));
					if (s_len)
						JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, i, s_len, BLUE);
	  			i++;
				}
  		  nd = nd->ln_Succ;
    		bp = (s_Base *)nd;
		  }
			s_save_flag = FALSE;
		  Close (fh);
		  prefs->fw_count = i;
		  SavePrefs ();

			ClearBar ();
			Status (GLS (&L_READY));
		}
		All (ON);
	}
}

VOID s_LoadFiles (VOID)
{
  BPTR fh;
  LONG size;

	SPrintF (VE_fw, "%s%s.fw", VEAssign, VE);
	size = FileSize (VE_fw);

	if (size > 0)
	{
		if (fh = Open (VE_fw, MODE_OLDFILE))
		{
			BOOL delete_flag = FALSE;

			Status (GLS (&L_LOADING_WATCH));
			if (Read (fh, Dummy, 6))
			{
				if (!(strncmp (Dummy, Watch_ID, 6)))
				{
					UWORD i;

					i = 0;
		  	  while (Read (fh, &s_tmp_base.head_size, sizeof (UWORD)))
  			  {
						s_Crypt (DECRYPT, (UBYTE *)&s_tmp_base.head_size, sizeof (UWORD));
	
	 					Read (fh, &s_tmp_base.FileName, s_tmp_base.head_size);
						s_Crypt (DECRYPT, (UBYTE *)&s_tmp_base.FileName, s_tmp_base.head_size);
	
	 					Read (fh, &s_tmp_base.size, sizeof (LONG));
						s_Crypt (DECRYPT, (UBYTE *)&s_tmp_base.size, sizeof (LONG));

	 					Read (fh, &s_tmp_base.md5, sizeof (MD5Data));
						s_Crypt (DECRYPT, (UBYTE *)&s_tmp_base.md5, sizeof (MD5Data));
				
		  	  	if (*s_tmp_base.FileName)
							s_Insert (0, s_tmp_base.FileName, s_tmp_base.size, s_tmp_base.md5);

						JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, i++, prefs->fw_count, BLUE);
					}
	    	}
	    	else
	    	{
					if (JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_THIS_VERSION_REQUIRES), GLS (&L_DELETE_CANCEL), S_MESSAGE))
					{
						Close (fh);
						DeleteFile (VE_fw);
						delete_flag = TRUE;
					}
				}
	  	}
			if (!delete_flag)	// Allready closed?
	   		Close (fh);
	  }
  }
	ClearBar ();
}
