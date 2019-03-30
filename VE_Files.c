#define FILEDEBUG 0

#include <JEO:JEO.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <exec/memory.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <exec/lists.h>
#include <JEO:raw.h>
#include <JEO:virus.h>
#include "VE_Prototypes.h"
#include "VE_Window.h"
#include <ctype.h>
#include <dos/datetime.h>
#include <proto/xvs.h>
#include <proto/xfdmaster.h>
#include <proto/xadmaster.h>
#include <dos/doshunks.h>

#include "VE_CheckAllWindow.c"

#define STATUSX		    5
#define STATUSY		   26

extern BOOL s_fib_open;
extern BOOL s_save_flag;
extern ULONG add_to_watch;
BOOL s_CheckFileNameExist (char *FileName);

char FilePW[9] = "";

BOOL filecheck_progress;

char FullName[500];
char InArchiveName[2048];
char N_A_Msg[] = "N/A";

struct DateStamp stamp_now;

VOID Do_disk_image (char *FileName, UBYTE *Buffer, ULONG size);
VOID Do_xad (char *FileName, char *Buffer, ULONG size);
BOOL CheckTeamMOS (UWORD *Buffer, LONG size);
BOOL CheckBobek (UWORD *Buf, ULONG size);
VOID ScanPoly (UWORD *Buffer, LONG size);
VOID fib_Close (VOID);
BOOL CheckExeFromBuffer (ULONG *Buf, ULONG size);
VOID Do_xfd_xad_error (char *FileName, LONG err, BOOL mode);
VOID Add_file (BOOL mode, char *Filename, ULONG size);

struct voldata *GetVolData (char *name);

BOOL link_virus_found;
BOOL main_stop;
BOOL checkArcs;
char CrunchName[50];
BOOL overwrite_status;
BOOL stripped;

BOOL multi_check;

extern struct ExecBase *SysBase;

#define DONE	-5
#define MAX_DEPTH	20000

BPTR far lock[MAX_DEPTH];

ULONG dir_depth;
ULONG cur;
ULONG max_depth;

char ArcName[512];
char ArcFile[512];
char Archive[50];

UWORD arc_depth = 0;
UWORD do_arc = 0;

ULONG dirCount = 0, fileCount = 0, archiveCount = 0, cFound = 0, cFiles = 0, virusCount = 0;
ULONG len = 0, exeFiles = 0, dataFiles = 0, skippedFiles = 0, archived = 0;

extern struct RastPort *rp;
extern UBYTE *Dummy;
extern UWORD mainX;

UWORD pack_checking;
BOOL pack_done;

ULONG new_size;
BOOL linkflag;

LONG c = 0;
ULONG timeStart, msecs;
char FileTime[20];

fib_Sjef fib_sjef;
ULONG fib_len;

BOOL autoremovelink;
BOOL autodeletefile;
ULONG *hunkresult = 0;

VOID ClearBar (VOID)
{
	SetAPen (VirusWnd->RPort, 0);
  RectFill (VirusWnd->RPort, mainX, 35 + OFFY, mainX + 628, 37 + OFFY);
}

BOOL CheckASCIIFile (UBYTE *Buf, ULONG len)
{
	ULONG i;

	for (i = 0; i < len; i++)
	{
		if ((Buf[i] > 0 AND Buf[i] < 7) OR (Buf[i] > 13 AND (Buf[i] < 31) OR Buf[i] == 127) OR (Buf[i] == 11 AND Buf[i] != 27))
			return (FALSE);
	}
	return (TRUE);
}

ULONG StripHunkFile (APTR Buffer, ULONG size)
{
  ULONG reslength = size;

	xfdStripHunks (Buffer, size, &reslength, XFDSHF_NAME|XFDSHF_SYMBOL|XFDSHF_DEBUG);
	if (size > reslength)
		return (reslength);
}

// ************************ MAIN CHECK *********************

#define STOP				-1

VOID PF (char *String)	// For aminet use only
{
	strcat (String, "\n\n");
	Write (Output (), String, strlen (String));
}

BOOL main_check_stop;

VOID Delete_action (char *VirusType, char *VirusName, char *FileName)
{
	UWORD action;
	char Hold[40];

	if (prefs->fast_file_checking)
	{
		WriteToLog ("No action taken", LOG_TEXT_LINE);
		return;
	}

	if (autodeletefile)
		action = 1;
	else
	{
		PlaySound (S_VIRUS);
		if (aminet_flag)
		{
			SPrintF (Dummy, "VIRUS: '%s'\n FILE: '%s'", VirusName, FileName);
			PF (Dummy);
			action = 0;
			main_stop = TRUE;
		}
		else
		{
			SPrintF (Dummy, GLS (&L_INFECTED_DELETE), VirusName, FileName);
			SPrintF (Hold, GLS (&L_VIRUS_WARNING), VirusType);
			action = rtEZRequestTags (Dummy, GLS (&L_DELETE_FILE_AUTO) , NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
  		                 RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, Hold, RT_TextAttr, &topaz8, TAG_END);
  		All (OFF);
  	}
 	}

	if (action == 2)	// Auto delete
	{
		autodeletefile = TRUE;
		action = 1;
	}
	switch (action)
	{
		case 1:	// Take action
		{
      SetProtection (FileName, 0);	// Unprotect
			if (DeleteFile (FileName))
			{
				WriteToLog ("File is deleted", LOG_TEXT_LINE);
				main_check_stop = TRUE;
			}
			else
			{
				WriteToLog ("Error deleting file", LOG_TEXT_LINE);

				SPrintF (Dummy, GLS (&L_FILE_NOT_DELETED), FileName);
				JEOSoundRequest (GLS (&L_MESSAGE),  Dummy, GLS (&L_OK), S_ERROR);
				main_check_stop = TRUE;
			}
			break;
		}
		case 3:	// Stop checking
		{
      WriteToLog (GLS (&L_USER_BREAK), LOG_TEXT_LINE);
			main_stop = TRUE;
			break;
		}
		case 0:	// Continue
		{
			WriteToLog ("No action taken", LOG_TEXT_LINE);
			main_check_stop = TRUE;
			break;
		}
  }
}

/*
VOID DeepCheck (char *FileName, UBYTE *Address, LONG size)
{
	int i;
	BOOL flag;
	UBYTE len, len1;
	char DotLibrary[] = ".library";
	char DotDevice[] = ".device";

	flag = FALSE;
	len = strlen (DotLibrary);
	len1 = strlen (DotDevice);
	for (i = 0; i < size; i++)
	{
		if (i < (size - len))
		{
			if (!(strnicmp (&Address[i], DotLibrary, len)))
			{
				flag = TRUE;
				break;
			}
		}
	}
	if (!flag)
	{
		SPrintF (Dummy, "Possible encrypted file:\n%s\nSize: %ld", FileName, size);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
	}
}
*/

/*
BOOL Check_EOR (UWORD *Buffer, LONG size)
{
	int i;
	UWORD j;

	if (size > 500)	// Zakapior
	{
		for (i = 0; i < (size - 500) / 2; i++)
		{
			if (i > 10000)	// Skal vi gidde mere?
				break;

			for (j = 0xB158; j <= 0xB15E; j++)
			{
				if (Buffer[i] == j)
					return (TRUE);
			}
			for (j = 0xB358; j <= 0xB35E; j++)
			{
				if (Buffer[i] == j)
					return (TRUE);
			}
		}
	}
	return (FALSE);
}
*/

VOID CheckShellScript (UWORD *Buf, ULONG size, char *FileName)
{
	BOOL flag = FALSE;
	char Decoded[1025];
	UBYTE *ptr;
	ULONG i;

	if (aminet_flag)
		return;

	ptr = (UBYTE *)Buf;
	if (size == 2408)	// Startup-CoderII
	{
		i = 86/2;				// start of recog

		if (Buf[i++] == 0x303c)
		if (Buf[i++] == 0x03ff)
		if (Buf[i++] == 0x1831)
		if (Buf[i++] == 0x0800)
		if (Buf[i++] == 0x0404)
		if (Buf[i++] == 0x0080)
		if (Buf[i++] == 0x1584)
		if (Buf[i++] == 0x0800)
		if (Buf[i++] == 0x51c8)
		if (Buf[i++] == 0xfff2)
		if (Buf[i++] == 0x2c79)
		if (Buf[i++] == 0x0000)
		if (Buf[i++] == 0x0004)
		if (Buf[i++] == 0x43f9)
		{
			ptr += 178;	// Start of decrunched text

			flag = TRUE;
			for (i = 0; i < 1024; i++)
			{
				Decoded[i] = *ptr++ - 128;
				if (*ptr == 0)
				{
					Decoded[i] = 0;
					break;
				}
			}
		}	  
	}
	if (!flag)
	{
		if (size < 5000 AND size > 70)	// Fyoot-Myoot
		{
			i = 50/2;

			if (Buf[i++] == 0x4eae)
			if (Buf[i++] == 0xffc4)
			if (Buf[i++] == 0x2e00)
			if (Buf[i++] == 0x6720)
			if (Buf[i++] == 0x2200)
			if (Buf[i++] == 0x41fa)
			if (Buf[i++] == 0x0035)
			if (Buf[i++] == 0x2408)
			if (Buf[i++] == 0x7601)
			if (Buf[i++] == 0x4eae)
			if (Buf[i++] == 0xffd0)
			if (Buf[i++] == 0x2207)
			if (Buf[i++] == 0x41fa)
			if (Buf[i++] == 0x0028)
			if (Buf[i++] == 0x2408)
			if (Buf[i++] == 0x263c)
			{
				ptr += 116;

				flag = TRUE;
				for (i = 0; i < 1024; i++)
				{
					Decoded[i] = *ptr++;
					if (*ptr == 0)
					{
						Decoded[i] = 0;
						break;
					}
				}
			}
		}	  
	}
	if (!flag)
	{
		if (size < 5000 AND size > 70)	// New shell script
		{
			i = 32/2;				// Code start / 2

			if (Buf[i++] == 0x2c78)
			if (Buf[i++] == 0x0004)
			if (Buf[i++] == 0x43fa)
			if (Buf[i++] == 0x0028)
			if (Buf[i++] == 0x7021)
			if (Buf[i++] == 0x4eae)
			if (Buf[i++] == 0xfdd8)
			if (Buf[i++] == 0x4a80)
			if (Buf[i++] == 0x671a)
			if (Buf[i++] == 0x2c40)
			if (Buf[i++] == 0x41fa)
			if (Buf[i++] == 0x0024)
			if (Buf[i++] == 0x2208)
			if (Buf[i++] == 0x7400)
			if (Buf[i++] == 0x7600)
			if (Buf[i++] == 0x4eae)
			if (Buf[i++] == 0xff22)
			{
				ptr += 90;

				flag = TRUE;
				for (i = 0; i < 1024; i++)
				{
					Decoded[i] = *ptr++;
					if (*ptr == 0)
					{
						Decoded[i] = 0;
						break;
					}
				}
			}
		}	  
	}

	if (!flag)
	{
		if (size > 20000)	// Rainboot
		{
			i = 0x20/2;
			if (Buf[i++] == 0x4E55)
			if (Buf[i++] == 0xFFFC)
			if (Buf[i++] == 0x42AD)
			if (Buf[i++] == 0xFFFC)
			if (Buf[i++] == 0x7A00)
			if (Buf[i++] == 0x2C78)
			if (Buf[i++] == 0x0004)
			if (Buf[i++] == 0x93C9)
			if (Buf[i++] == 0x4EAE)
			if (Buf[i++] == 0xFEDA)
			if (Buf[i++] == 0x2640)
			if (Buf[i++] == 0x4AAB)
			if (Buf[i++] == 0x00AC)
			if (Buf[i++] == 0x6614)
			if (Buf[i++] == 0x41EB)
			if (Buf[i++] == 0x005C)
			if (Buf[i++] == 0x4EAE)
			if (Buf[i++] == 0xFE80)
			if (Buf[i++] == 0x41EB)
			if (Buf[i++] == 0x005C)
			if (Buf[i++] == 0x4EAE)
			if (Buf[i++] == 0xFE8C)
			if (Buf[i++] == 0x2B40)
			if (Buf[i++] == 0xFFFC)
			if (Buf[i++] == 0x43FA)
			if (Buf[i++] == 0x00FC)
			if (Buf[i++] == 0x7025)
			if (Buf[i++] == 0x4EAE)
			if (Buf[i++] == 0xFDD8)
			if (Buf[i++] == 0x4A80)
			if (Buf[i++] == 0x6700)
			if (Buf[i++] == 0x00D4)
			if (Buf[i++] == 0x2C40)
			if (Buf[i++] == 0x43FA)
			if (Buf[i++] == 0x00FC)
			if (Buf[i++] == 0x2209)
			if (Buf[i++] == 0x243C)
			if (Buf[i++] == 0x0000)
			{
				UBYTE *ptr2;

				ptr += (i * 2);
				for (; i < size; i++)
				{
					if (i >= size - 16)
						break;
					ptr2 = ptr;
					if (*ptr2++ == '%')
					if (*ptr2++ == 'R')
					if (*ptr2++ == 'U')
					if (*ptr2++ == 'N')
					if (*ptr2++ == '=')
					if (*ptr2++ == '"')
					{
						flag = TRUE;
						for (i = 0; i < 1024; i++)
						{
							Decoded[i] = *ptr2++;
							if (*ptr2 == '"')
							{
								Decoded[++i] = 0;
								break;
							}
						}
						break;
					}
					ptr++;
				}
			}
		}
	}

	if (!flag)
	{
		UWORD *check;
		char TempCheck[12];
		int j;
		UBYTE count = 0;

		strcpy (TempCheck, "run c:"); 
		strcat (TempCheck, "de");
		strcat (TempCheck, "lete");
		check = (UWORD *)TempCheck;

		if (size > 3000)	// UAE-Hater
		{
			for (i = 0; i < (size - (8)) / 2; i++)
			{
				check = (UWORD *)TempCheck;
				count = 0;

				for (j = 0; j < 6; j++)
				{
					if (Buf[i+j] == *check)
					{
						count++;
						check++;
					}
					else
						break;
				}
				if (count == 6)	// Find end...
				{
					ULONG start = i;
					char *p;

					p = (char *)&Buf[start];
					count = 0;
					for (i = start, j = 0; i < size; i++, j++)
					{
						Decoded[j] = *p;
						if ((*p == 0) OR (*p == 13))
							break;
						if (count > 60)
							break;
						p++;
					}
					Decoded[j] = 0;
					flag = TRUE;
					break;
				}
			}
		}
	}

	if (flag)
	{
		char StripName[500];

		Decoded[i] = 0;
		StripFile (FileName, StripName);	// Leaves the file name only
		SPrintF (Dummy, GLS (&L_FOUND_A_SHELL_SCRIPT), FileName);

		WriteToLog (Dummy, LOG_LINE_TEXT);
		WriteToLog ("", LOG_TEXT);
		WriteToLog (Decoded, LOG_TEXT_LINE);
		if (!prefs->fast_file_checking)
		{
			if (JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_DECODE_CONTINUE), S_MESSAGE))
				JEOSoundRequest (StripName, Decoded, GLS (&L_OK), S_NO_SOUND);
		}
	}
}

VOID CheckInstallScript (UBYTE *Buf, ULONG size, char *FileName)
{
	BOOL flag = FALSE;
	ULONG i;

	if (size < 200)
		return;

	for (i = 0; i < 15; i++)
	{
		if (!isascii(Buf[i]))
			return;
	}

	for (i = 0; i < size; i++)
	{
		if (i < (size - 25))
		{
			if (Buf[i] == '(')
			if (Buf[i+1] == 'a')
			if (Buf[i+2] == 'p')
			if (Buf[i+3] == 'p')
			if (Buf[i+4] == 'e')
			if (Buf[i+5] == 'n')
			if (Buf[i+6] == 'd')
			if (Buf[i+7] == ' ')
			if (Buf[i+8] == '"')
			{
				i += 9;
				if (Buf[i] == '\\')
				if (Buf[i+1] == '0')
				if (Buf[i+2] == '\\')
				if (Buf[i+3] == '0')
				if (Buf[i+4] == '\\')
				if (Buf[i+5] == 3)
				if (Buf[i+6] == '\\')
				if (Buf[i+7] == 243)
				{
					flag = TRUE;
					break;
				}
			}
		}
	}
	if (flag)
	{
		char StripName[500];

		StripFile (FileName, StripName);	// Leaves the file name only
		SPrintF (Dummy, GLS (&L_HIDDEN_EXE_FOUND), StripName);
		JEOSoundRequest (GLS (&L_WARNING), Dummy, GLS (&L_OK), S_ERROR);
	}
}

BOOL Check_crunch (UBYTE *Buffer, ULONG size)
{
	struct xfdBufferInfo *bi = 0;
	BOOL flag = FALSE;

	if (bi = xfdAllocObject (XFDOBJ_BUFFERINFO))
	{
		bi->xfdbi_SourceBuffer = Buffer;
		bi->xfdbi_SourceBufLen = size;
 	  bi->xfdbi_Flags = XFDFF_RECOGEXTERN;
		if (xfdRecogBuffer (bi))	 // Crunched?
		{
			flag = TRUE;
//			printf ("CRUNCH: '%s', %ld\n", bi->xfdbi_PackerName, size);
		}
		xfdFreeObject (bi);
	}
	return (flag);
}

BOOL Check_pack (UBYTE *Buffer, ULONG size)
{
	ULONG *Buf;

	Buf = (ULONG *)Buffer;
	if (xadmaster_flag)
	{
		struct xadClient *cl;

		if (cl = xadRecogFile (size, Buf, XAD_NOEXTERN, 0, TAG_DONE))
		{
			mystrcpy (Archive, cl->xc_ArchiverName);
//			printf ("PACK: '%s', %ld\n", cl->xc_ArchiverName, size);
			return (TRUE);
		}
	}
	else
	{
		if (!(strncmp (&Buffer[2], "-lh", 3)))	// LHA, LZH archive
  	  return (TRUE);
		else if (!(strncmp (Buffer, "PK", 2)))	// ZIP archive
  	  return (TRUE);
/*
	else if (Buf[0] >= 0x1f8b0800 AND Buf[0] <= 0x1f8b0808)					// GZIP archive
    return (TRUE);
*/
		else if (!(strncmp (Buffer, "LZX", 3)))	// LZX archive
  	  return (TRUE);
		else if (!(strncmp (Buffer, "DMS!", 4)))	// LZX archive
  	  return (TRUE);
	  else if (Buf[0] == 0x000003F3)	// Exe
  	{
			UWORD hunks, start;

			if (size > 50)
			{
				hunks = Buf[2];
			  start = ((6 + hunks) + 1);	// start of 1. hunk code

				if (Buf[start++] == 0x60000006)
				if (Buf[start++] == 0x000024f2)
				if (Buf[start++] == 0x48e77efe)
				if (Buf[start++] == 0x24482400)
					return (TRUE);
			}
  	}
  }
  return (FALSE);
}

BYTE CheckVirus (char *FileName, UBYTE *Address, LONG size)
{
  BYTE ret;
	UWORD action;
	BPTR fh = 0;
	struct xvsFileInfo *fileinfo;

	if (Check_pack (Address, size))
		return (FALSE);

	if (!(fileinfo = xvsAllocObject (XVSOBJ_FILEINFO)))
	{
		JEOSoundRequest (xvs_name, GLS (&L_NO_MEM), GLS (&L_OK), S_ERROR);
		main_stop = TRUE;
		return (FALSE);
	}

	fileinfo->xvsfi_File = Address;
	fileinfo->xvsfi_FileLen = size;
	xvsCheckFile (fileinfo);

	ret = FALSE;	// No found?
	switch (fileinfo->xvsfi_FileType)
	{
		case XVSFT_EMPTYFILE:
			break;
		case XVSFT_DATAFILE:
		{
			if (prefs->checkData)
			{
				dataFiles++;
				CheckInstallScript (Address, size, FileName);
			}
			break;
		}
		case XVSFT_DATAVIRUS:
		{
			dataFiles++;
			virusCount++;
			prefs->fileVir++;
			UpDateVirusCount ();

			ret = TRUE;
			if (pack_checking)	// Er vi i et arkiv?
			{
				if (aminet_flag)
				{
					SPrintF (Dummy, "VIRUS: '%s'\n FILE: '%s'\n  ARC: '%s'", fileinfo->xvsfi_Name, ArcFile, ArcName);
					PF (Dummy);
				}
				else
				{
					SPrintF (Dummy, "DATA VIRUS FOUND: %s\n            FILE: \"%s\"\n         ARCHIVE: \"%s\"", fileinfo->xvsfi_Name, ArcFile, ArcName);
					WriteToLog (Dummy, LOG_LINE_TEXT_LINE);

					if (!prefs->fast_file_checking)
					{
						SPrintF (Dummy, GLS (&L_VIRUS_INFECTED_ARCHIVE), fileinfo->xvsfi_Name, ArcFile, ArcName);
						JEOSoundRequest (GLS (&L_DATA_VIRUS_WARNING), Dummy, GLS (&L_OK), S_VIRUS);
					}
				}
			}
			else
			{
				SPrintF (Dummy, "DATA VIRUS FOUND: %s\n            FILE: \"%s\"", fileinfo->xvsfi_Name, FileName);
				WriteToLog (Dummy, LOG_LINE_TEXT);

				Delete_action (GLS (&L_DATA), fileinfo->xvsfi_Name, FileName);
			}
			break;
		}
		case XVSFT_FILEVIRUS:
		{
			virusCount++;
			prefs->fileVir++;
			UpDateVirusCount ();
			ret = TRUE;

			if (pack_checking)	// Er vi i et arkiv?
			{
				if (aminet_flag)
				{
					SPrintF (Dummy, "VIRUS: '%s'\n FILE: '%s'\n  ARC: '%s'", fileinfo->xvsfi_Name, ArcFile, ArcName);
					PF (Dummy);
				}
				else
				{
					SPrintF (Dummy, "FILE VIRUS FOUND: %s\n            FILE: \"%s\"\n         ARCHIVE: \"%s\"", fileinfo->xvsfi_Name, ArcFile, ArcName);
					WriteToLog (Dummy, LOG_LINE_TEXT_LINE);

					if (!prefs->fast_file_checking)
					{
						SPrintF (Dummy, GLS (&L_VIRUS_INFECTED_ARCHIVE), fileinfo->xvsfi_Name, ArcFile, ArcName);
						JEOSoundRequest (GLS (&L_FILE_VIRUS_WARNING), Dummy, GLS (&L_OK), S_VIRUS);
					}
				}
			}
			else
			{
				SPrintF (Dummy, "FILE VIRUS FOUND: %s\n            FILE: \"%s\"", fileinfo->xvsfi_Name, FileName);
				WriteToLog (Dummy, LOG_LINE_TEXT);

				Delete_action (GLS (&L_FILE1), fileinfo->xvsfi_Name, FileName);
			}
			break;
		}
		case XVSFT_LINKVIRUS:
		{
			virusCount++;
			prefs->fileVir++;
			UpDateVirusCount ();
			ret = TRUE;

//			printf ("%ld\n", fileinfo->xvsfi_InternalType);
			if (pack_checking)	// Er vi i et arkiv?
			{
				if (aminet_flag)
				{
					SPrintF (Dummy, "VIRUS: %s\n FILE: '%s'\n  ARC: '%s'", fileinfo->xvsfi_Name, ArcFile, ArcName);
					PF (Dummy);
				}
				else
				{
					SPrintF (Dummy, "LINK VIRUS FOUND: %s\n            FILE: \"%s\"\n         ARCHIVE: \"%s\"", fileinfo->xvsfi_Name, ArcFile, ArcName);
					WriteToLog (Dummy, LOG_LINE_TEXT_LINE);

					if (!prefs->fast_file_checking)
					{
						SPrintF (Dummy, GLS (&L_VIRUS_INFECTED_ARCHIVE), fileinfo->xvsfi_Name, ArcFile, ArcName);
						JEOSoundRequest (GLS (&L_LINK_VIRUS_WARNING), Dummy, GLS (&L_OK), S_VIRUS);
					}
				}
			}
			else
			{
				SPrintF (Dummy, "LINK VIRUS FOUND: %s\n            FILE: \"%s\"", fileinfo->xvsfi_Name, FileName);
				WriteToLog (Dummy, LOG_LINE_TEXT);

				if (prefs->fast_file_checking)
					break;
				

				if (autoremovelink)
					action = 1;	// Don't ask..
				else
				{
					PlaySound (S_VIRUS);

					if (aminet_flag)
					{
						action = 0;
						SPrintF (Dummy, "VIRUS: '%s'\n FILE: '%s'", fileinfo->xvsfi_Name, FileName);
						PF (Dummy);
					}
					else
					{
						SPrintF (Dummy, GLS (&L_VIRUS_INFECTED_FILE), fileinfo->xvsfi_Name, FileName);
						action = rtEZRequestTags (Dummy, GLS (&L_REMOVE_VIRUS_AUTO), NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
    		  		RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_LINK_VIRUS_WARNING), RT_TextAttr, &topaz8, TAG_END);
    		  	All (OFF);
    		  }
    	  }

				if (action == 2)	// Auto remove?
				{
					autoremovelink = TRUE;
					action = 1;
				}
				switch (action)
				{
					case 1:	// Take action
					{
						WORD action;

			      SetProtection (FileName, 0);	// Unprotect
						if (xvsRepairFile (fileinfo))
						{
							new_size = fileinfo->xvsfi_FixedLen;	// Oppdaterer størrelsen
							CopyMem (fileinfo->xvsfi_Fixed, Address, fileinfo->xvsfi_FixedLen);
							if (autoremovelink)
								action = 1;
							else
							{							
								SPrintF (Dummy, GLS (&L_VIRUS_REMOVED_DIFF), size, new_size, size - new_size);
								action = JEOSoundRequest (GLS (&L_MESSAGE),  Dummy, GLS (&L_SAVE_NEW_FILE), S_NO_SOUND);
							}
							if (action == 1)
							{
								if (fh = Open (FileName, MODE_NEWFILE))
								{
									if (Write (fh, fileinfo->xvsfi_Fixed, new_size) != new_size)
									{
										WriteToLog ("Error saving file", LOG_LINE_TEXT);

										SPrintF (Dummy, GLS (&L_ERROR_WRITING_FILE), FileName);
										JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
									}
									else
									{
										WriteToLog ("Virus removed", LOG_TEXT_LINE);
										stripped = TRUE;	// Flytte denne ned :)
									}
									Close (fh);
								}
								else
								{
									WriteToLog ("Error opening file", LOG_TEXT_LINE);

									SPrintF (Dummy, GLS (&L_ERROR_OPENING_FILE), FileName);
									JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
								}
							}
							else
					      WriteToLog ("No action taken", LOG_TEXT_LINE);
						}
						else	// Error repairing file
						{
							WriteToLog ("Error removing virus from file!", LOG_TEXT_LINE);

							SPrintF (Dummy, GLS (&L_ERROR_VIRUS_NOT_REMOVED), fileinfo->xvsfi_Name);
							action = JEOSoundRequest (GLS (&L_ERROR_REMOVING_VIRUS),  Dummy, GLS (&L_DELETE_FILE_STOP), S_ERROR);
							switch (action)
							{
								case 1:	// Take action
								{
//									flag = TRUE;	// Don't get an error when returning
						      SetProtection (FileName, 0);	// Unprotect
									if (!(action = DeleteFile (FileName)))
									{
										SPrintF (Dummy, GLS (&L_FILE_NOT_DELETED), FileName, action);
										JEOSoundRequest (GLS (&L_MESSAGE),  Dummy, GLS (&L_OK), S_ERROR);
									}
									break;
								}
								case 2:	// Stop checking
								{
									main_stop = TRUE;
									break;
								}
							}
						}
						break;
					}
					case 3:	// Stop checking
					{
			      WriteToLog (GLS (&L_USER_BREAK), LOG_TEXT_LINE);
						main_stop = TRUE;
						break;
					}
					case 0:	// Continue
					{
			      WriteToLog ("No action taken", LOG_TEXT_LINE);
  	    		break;
	  	    }
				}
			}
			break;
		}
		case XVSFT_EXEFILE:
		{
			CheckShellScript ((UWORD *)Address, size, FileName);
			break;
		}
	}

	if (fileinfo)
	{
		xvsFreeObject (fileinfo);
		fileinfo = 0;
	}

	return (ret);
}

LONG FindAvailDepth (VOID)
{
	int i;

	for (i = 0; i < MAX_DEPTH; i++)
	{
		if (lock[i] == DONE)
			return i;			// Vi bruker neste som er ledig...
	}
	return (ERROR);
}

#define SCAN_SIZE	100

ULONG far FileBuffer[SCAN_SIZE / 4];

BOOL CheckExeFromBuffer (ULONG *Buf, ULONG size)
{
	if (size < 4)
		return (FALSE);

	if (Buf[0] == 0x000003F3)
		return (TRUE);

	return (FALSE);
}

BOOL CheckExe (char *FileName, ULONG size)
{
	BPTR fh;
  BOOL ret = TRUE;

	if (size < 8)
		return (FALSE);

  if (fh = Open (FileName, MODE_OLDFILE))
  {
    if (Read (fh, FileBuffer, SCAN_SIZE) == ERROR)		// protected?
    {
      SetProtection (FileName, 0);						// Unprotect
      if (Read (fh, FileBuffer, SCAN_SIZE) == ERROR)	// Error
        ret = FALSE;
    }
    Close (fh);
		if (!ret)
		{
			SPrintF (Dummy, GLS (&L_ERROR_READING_FILE), FileName);
			JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
		}
  }
  else
  {
  	ret = FALSE;
		SPrintF (Dummy, GLS (&L_ERROR_OPENING_FILE), FileName);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
	}

	if (ret)
	{
		if (FileBuffer[0] != 0x000003F3)
			ret = FALSE;
	}
	return (ret);
}

BOOL Check_file_intuition (VOID)
{
	struct IntuiMessage *message;
	ULONG class;
	USHORT code;
	BOOL ret;

	if (aminet_flag)
		return (TRUE);

	ret = TRUE;
	while (message = (struct IntuiMessage *)GetMsg (VirusWnd->UserPort))
	{
		class = message->Class;
		code = message->Code;
		ReplyMsg ((struct Message *)message);

		switch (class)
		{
			case IDCMP_DISKINSERTED:
				DiskInserted ();
				Status (GLS (&L_PLEASE_WAIT));
				break;
			case IDCMP_DISKREMOVED:
				DiskRemoved ();
				Status (GLS (&L_PLEASE_WAIT));
				break;
			case IDCMP_RAWKEY:
			{
				switch (code)
				{
					case ESC:
					{
						if (!multi_check)
						{
							if (!JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_FILE_CHECK_PAUSED), GLS (&L_CONTINUE_STOP), S_NO_SOUND))
							{
					      Status (GLS (&L_USER_BREAK));
								SPrintF (Dummy, "\n%s\n", GLS (&L_USER_BREAK));
				    	  WriteToLog (Dummy, LOG_TEXT);
								main_stop = TRUE;
				  	    ret = FALSE;
							}
							ClearIntuition (VirusWnd);
						}
						else
						{
							UBYTE ret;

							ret = JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_FILE_CHECK_PAUSED), GLS (&L_CONTINUE_SKIP_STOP), S_NO_SOUND);
							if (ret != 1) // Continue?
							{
					      Status (GLS (&L_USER_BREAK));
								SPrintF (Dummy, "\n%s\n", GLS (&L_USER_BREAK));
				    	  WriteToLog (Dummy, LOG_TEXT);

								main_stop = TRUE;
								if (ret == 2)	// Skip
									main_check_stop = TRUE;
				  	    ret = FALSE;
							}
							ClearIntuition (VirusWnd);
						}
						break;
					}
			  }
	  	}
	  }
	}
	return (ret);
}

ULONG i = 0;

//extern ULONG stacksize;
extern struct Process *myproc;

VOID MainCheck (char *FileName, char *Buffer, ULONG size)
{
	BOOL exe;

	if (main_stop)
		return;
	if (main_check_stop)
		return;

	if (Check_pack (Buffer, size))	// Is it an archive?
	{
		archived++;						// Main sfx pack
		FileStat (FileName);
	}

/*
	stacksize = (ULONG)myproc->pr_Task.tc_SPUpper-(ULONG)myproc->pr_Task.tc_SPLower;
	printf ("%ld %ld\n", (ULONG)myproc->pr_Task.tc_SPUpper-(ULONG)myproc->pr_Task.tc_SPReg,
			(ULONG)myproc->pr_Task.tc_SPReg-(ULONG)myproc->pr_Task.tc_SPLower);
*/
	exe = CheckExeFromBuffer ((ULONG *)Buffer, size);
	CrunchName[0] = 0;

#if FILEDEBUG 
		printf ("\nMAIN: %ld exe %ld (%s)\n", i++, exe, FileName);
#endif

// VIRUS
	if (!main_check_stop)
	{
		if (CheckVirus (FileName, Buffer, size))
		{
			if (aminet_flag)
			{
				main_stop = TRUE;
				return;
			}
			else
			{
				if (stripped)
				{
					if (!linkflag)	// Ikke i en linka fil???
					{
						size = new_size;
						MainCheck (FileName, Buffer, size);	// Vi sjekker igjen :)
					}
					else
						main_check_stop = TRUE;
				}
				else
					main_check_stop = TRUE;
				if (pack_checking)
				{
					if (!main_stop)
					{
						main_check_stop = FALSE;
							return;
					}
				}
			}
		}
	}

// UNARCHIVE
	if (!main_check_stop)
	{
		if (xadmaster_flag)
		{
			if (pack_checking AND (Check_pack (Buffer, size)))
			{
				SPrintF (InArchiveName, "%s>%s", FileName, ArcFile);
				Do_xad (InArchiveName, Buffer, size);
			}
			else
			{
				mystrcpy (InArchiveName, FileName);
				Do_xad (FileName, Buffer, size);
			}
		}
	}

// UNLINK
	if (!main_check_stop)
	{
		if (xfdmaster_flag AND exe)
		{
			struct xfdLinkerInfo *li = 0;
	
			if (li = xfdAllocObject (XFDOBJ_LINKERINFO))
			{
				li->xfdli_Buffer = Buffer;
				li->xfdli_BufLen = size;
				if (xfdRecogLinker (li))
				{
					linkflag = TRUE;

					if (!pack_checking)
					{
						SPrintF (Dummy, GLS (&L_UNLINKING_FILE), FileName, li->xfdli_LinkerName);
						Status (Dummy);
						#if FILEDEBUG 
							printf ("%s\n", Dummy);
						#endif
					}
					if (xfdUnlink (li))	// Fila er linka!
					{
						#if FILEDEBUG 
							printf ("\nUnlink 1: %ld\n", li->xfdli_SaveLen1);
						#endif
						MainCheck (FileName, li->xfdli_Save1, li->xfdli_SaveLen1);
						if (!main_check_stop)
						{
							#if FILEDEBUG 
								printf ("\nUnlink 2: %ld\n", li->xfdli_SaveLen2);
							#endif
							MainCheck (FileName, li->xfdli_Save2, li->xfdli_SaveLen2);
						}
					}
				}
				if (li)
			    xfdFreeObject(li);
			}
			else if (!prefs->fast_file_checking)
				Do_xfd_xad_error ("", XFDERR_NOMEMORY, 1);
		}
	}

// DECRUNCH
	if (!main_check_stop)
	{
		if (xfdmaster_flag)
		{
			UWORD len;

			if (size > 500)
				len = 500;
			else
				len = size;

			if (!CheckASCIIFile (Buffer, len))
			{
				struct xfdBufferInfo *bufferinfo;

				if (bufferinfo = xfdAllocObject (XFDOBJ_BUFFERINFO))
				{
					UWORD buflen;

					CrunchName[0] = 0;

					bufferinfo->xfdbi_SourceBuffer = Buffer;
					bufferinfo->xfdbi_SourceBufLen = size;
		  	  bufferinfo->xfdbi_Flags = XFDFF_RECOGEXTERN;

					if (xfdRecogBuffer (bufferinfo))	 // Crunched?
					{
						BOOL error_flag = FALSE;
						BOOL crunch_flag = FALSE;

						if (!(strcmp ("StoneCracker 4.04", bufferinfo->xfdbi_PackerName)))
						{
							UWORD *Buf;

							Buf = (UWORD *)Buffer;
							if (Buf[29] != 0x4a80)	// Not a StoneCracker... Fixes AmiGameBoy.key
								error_flag = TRUE;
						}
						if (!error_flag)
						{
							cFound++;
							if (prefs->check_crunched)	// Really?
							{
								crunch_flag = TRUE;
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_PASSWORD)
								{
									if (prefs->skip_encrypted)
										crunch_flag = FALSE;
								}
							}
/*	// Why did I do this?
							if (bufferinfo->xfdbi_PackerFlags & XFDPFF_ADDR)	// Vil ikke decrunche
								crunch_flag = FALSE;
*/
						}
						if (crunch_flag)
						{
							mystrcpy (CrunchName, bufferinfo->xfdbi_PackerName);
							FileStat (FileName);

							if (!pack_checking)
							{
					 			SPrintF (Dummy, "%s (%s)...", FileName, bufferinfo->xfdbi_PackerName);
						  	Status (Dummy);
						  }
							if (bufferinfo->xfdbi_PackerFlags & XFDPFF_PASSWORD)
							{
								char *pwbuf = 0;

				        buflen = (bufferinfo->xfdbi_MaxSpecialLen == 0xFFFF) ? 256 : bufferinfo->xfdbi_MaxSpecialLen;
    				    if (pwbuf = AllocMem (buflen, MEMF_ANY | MEMF_CLEAR))
							  {
								  if (rtGetString (pwbuf, buflen, GLS (&L_ENTER_PW), NULL, RT_ReqPos, REQPOS_CENTERSCR, RTGS_Invisible, TRUE, RT_TextAttr, &topaz8, TAG_END))
						    	  bufferinfo->xfdbi_Special = pwbuf;
							  
									FreeMem (pwbuf, buflen);
								}
  	  			  }
//							bufferinfo->xfdbi_TargetBufMemType = MEMF_ANY;

							#if FILEDEBUG 
							  printf ("Name: %s\nType is '%s'\n", FileName, bufferinfo->xfdbi_PackerName);
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_RELOC)
									printf ("RELOC\n");
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_ADDR)
									printf ("ADDR\n");
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_DATA)
									printf ("DATA\n");
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_PASSWORD)
									printf ("PASSWORD\n");
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_RELMODE)
									printf ("RELMODE\n");
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_KEY16)
									printf ("KEY16\n");
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_KEY32)
									printf ("KEY32\n");
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_RECOGLEN)
									printf ("XFDPFF_RECOGLEN\n");
								if (bufferinfo->xfdbi_PackerFlags & XFDPFF_USERTARGET)
									printf ("XFDPFF_USERTARGET\n");

								printf ("$%08lX: %ld\n", Buffer, size);
							#endif
							if (xfdDecrunchBuffer (bufferinfo))		// DECRUNCHER!!!
							{
								#if FILEDEBUG 
									printf ("Decruncha %ld (%ld)\n", bufferinfo->xfdbi_TargetBufSaveLen, bufferinfo->xfdbi_TargetBufLen);
								#endif
								if (!pack_checking)
								{
									if (prefs->overwrite_crunched_files)	// Really?
									{
										BOOL do_flag = FALSE;

										if (!overwrite_status)	// Asked before?
										{
											if (JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_FOUND_A_CRUNCHED_FILE), GLS (&L_DO_IT_TURN_OFF), S_MESSAGE))
											{
												do_flag = TRUE;
												overwrite_status = ON;
											}
											else
												prefs->overwrite_crunched_files = OFF;
										}
										else
											do_flag = TRUE;
										if (do_flag)
										{
											BPTR fh;

											SPrintF (Dummy, GLS (&L_SAVING_FILE), FileName);
											Status (Dummy);
											SetProtection (FileName, 0);	// Unprotect
											if (fh = Open (FileName, MODE_NEWFILE))
											{
												if (Write (fh, bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufSaveLen) != bufferinfo->xfdbi_TargetBufSaveLen)
													JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR_SAVING_FILE), GLS (&L_OK), S_ERROR);
												Close (fh);
											}
										}
									}
								}
								cFiles++;

								MainCheck (FileName, bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufSaveLen);
								#if FILEDEBUG 
									printf ("FreeMem %ld\n", bufferinfo->xfdbi_TargetBufLen);
								#endif
								FreeMem (bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufLen);
							}
						  else	// Error decrunching
								Do_xfd_xad_error (FileName, bufferinfo->xfdbi_Error, 1);
						}
					}
					xfdFreeObject (bufferinfo);
				}
				else
				{
					Do_xfd_xad_error ("", XFDERR_NOMEMORY, 1);
					CleanUp ();
				}
			}
		}
	}

// STRIP HUNK
	if (!main_check_stop)
	{
		if (xfdmaster_flag AND exe)
		{
			ULONG tmp_size;

			if (exe)
				tmp_size = StripHunkFile (Buffer, size);
			if (size > tmp_size)
			{
				#if FILEDEBUG 
					printf ("STRIPPED HUNK!\n");
				#endif
				MainCheck (FileName, Buffer, tmp_size);
			}
		}
	}
}

BOOL CheckDirectory (VOID)
{
  ULONG fc;
  struct Node *nd;
  Filbase *bp;
  BPTR fh;
	BOOL exe;
	BOOL do_flag;
	BOOL pack_flag;
	char *Buffer = 0;
	BOOL ret = TRUE;
	UWORD pack;

	dirCount++;

  fc = 0;
  nd = fib_sjef.list.lh_Head;
  bp = (Filbase *)nd;

  while (nd->ln_Succ)
	{
  	if (main_stop)
  		break;
		if (!Check_file_intuition ())
		{
			ret = FALSE;
			break;
		}

		if (!aminet_flag)
			JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, fc + 1, fib_len, BLUE);

		fileCount++;
		Status (bp->FileName);
		FileStat (bp->FileName);

		do_flag = FALSE;
		pack_flag = FALSE;
		InArchiveName[0] = 0;
		if (exe = CheckExe (bp->FileName, bp->size))	// Loads 8 bytes to FileBuffer
		{
			BOOL add_to_watch_flag = TRUE;

			if (s_fib_open AND add_to_watch_flag)
			{
				if (!s_CheckFileNameExist (bp->FileName))
					Add_file (0, bp->FileName, bp->size);
			}

			do_flag = TRUE;
			exeFiles++;
		}

		pack = Check_pack ((UBYTE *)FileBuffer, SCAN_SIZE);	// Is it an archive
		if (pack AND prefs->extract_archives)
			do_flag = TRUE;
		else
		{
			CrunchName[0] = 0;
			FileStat (bp->FileName);
			if (prefs->checkData)	// Check for data???
			{
				if (!pack)	// Is it an non archive
					do_flag = TRUE;
			}
		}
		pack_checking = OFF;

		if (do_flag AND bp->size)	// Vi skal sjekke ;)
		{
			ULONG check_size;
			ULONG size;
			BOOL crunch = 0;
			BOOL small_size = FALSE;

			size = bp->size;
			do_flag = FALSE;

			if (prefs->check_crunched)
				crunch = Check_crunch ((UBYTE *)FileBuffer, SCAN_SIZE);	// Is it an archive
			if (!exe AND !pack AND !crunch)	// Check data file max size
			{
				if (size > (64 * 1024))
				{
					size = 1024 * 64;	// Only read 64 kb
					small_size = TRUE;
				}
			}
			if (Buffer = AllocMem (size, MEMF_PUBLIC))
			{
			  if (fh = Open (bp->FileName, MODE_OLDFILE))
				{
	  	   	if (Read (fh, Buffer, size) == size);
	  	   	{
	  	   		do_flag = TRUE;	//// ELELELELELELELSE

						if (small_size)	// Didn't load the whole file
						{
							if (!pack)	// New check with new buf
								pack = Check_pack (Buffer, size);	// Is it an archive
							if (!crunch)
								crunch = Check_crunch (Buffer, size);	// Is it an cruncher

							if (pack OR crunch)	// Load whole file!
							{
								FreeMem (Buffer, size);
								Buffer = 0;

								do_flag = FALSE;
								size = bp->size;
								if (Buffer = AllocMem (size, MEMF_PUBLIC))
								{
								  if (fh = Open (bp->FileName, MODE_OLDFILE))
									{
						  	   	if (Read (fh, Buffer, size) == size);
						  	   		do_flag = TRUE;	//// ELELELELELELELSE
						  	  }
						  	}
							}
						}
	  	   	}
	  			Close (fh);
  				fh = 0;
		  	}
	  	}

			if (do_flag)
			{
				check_size = size;
				linkflag = FALSE;
				if (!main_stop)
					main_check_stop = FALSE;
				stripped = FALSE;

/*
				if (0)	// pw!
				{
					MD5Data md5;
					UWORD len;

					strcpy (FilePW, "Hebbe55");
					SPrintF (Dummy, "%s%08lX%s", bp->FileName, size, FilePW);
					len = strlen (Dummy);
					printf ("'%-50s'  ", Dummy);
					md5 = Calculate_MD5 (Dummy, len);
					SPrintF (Dummy, "%lc%lc%lc%lc", md5.md5[0], md5.md5[8], md5.md5[16], md5.md5[24]);
					VE_Crypt (CRYPT, (char *)&Dummy[0], 4);
					printf ("%s\n", Dummy);
					SetComment (bp->FileName, Dummy);
				}
*/
				MainCheck (bp->FileName, Buffer, check_size);
			}
			if (Buffer)
			{
				FreeMem (Buffer, size);
				Buffer = 0;
			}
		}
		else if (pack)
			archived++;

	  nd = nd->ln_Succ;
    bp = (Filbase *)nd;
   	fc++;
  }

  fib_SlettBase ();	// Sletter filene i dirbuffer!
	if (!aminet_flag)
		ClearBar ();
  return (ret);
}

struct DateStamp Convert_DateTime_to_stamp (char *Date, char *Time)
{
  struct DateTime datetime = { NULL };
  struct DateStamp stamp;

	datetime.dat_Format = FORMAT_CDN;	// dd-mm-yy
	datetime.dat_Flags = 0;
	datetime.dat_StrDate = Date;
	datetime.dat_StrTime = Time;
	StrToDate (&datetime);
	stamp = datetime.dat_Stamp;
	return (stamp);
}

struct FileInfoBlock *fib = 0;

VOID ScanDirectory (char *Dir)
{
	char Name[300];
  ULONG success;
  int i;
  BOOL go_flag;
  BOOL ret;

  Status (GLS (&L_PLEASE_WAIT));

	max_depth = 0;
	dir_depth = 0;

	for (i = 0; i < MAX_DEPTH; i++)
		lock[i] = DONE;

	go_flag = TRUE;

	if (lock[dir_depth] = Lock (Dir, ACCESS_READ))
	{
		while (go_flag)
		{
			go_flag = FALSE;
			for (i = 0; i < MAX_DEPTH; i++)
			{
				if (lock[i] != DONE)	// Ikke ferdig?
				{
					cur = i;
					go_flag = TRUE;
					break;
				}
			}
			if (go_flag)	// Har vi flere dirs og sjekke?
			{
				Status (GLS (&L_PLEASE_WAIT));
	  		if (success = Examine (lock[cur], fib))
 				{
		  		NameFromLock (lock[cur], Name, 300);	// Vi finner dirnavnet!
					FOREVER
					{
						if (main_stop)
							goto end;
						ret = Check_file_intuition ();
						if (ret == FALSE)
							goto end;
						success = ExNext (lock[cur], fib);	// FindNext File/Dir

  				  if (!success)				// End of directory?
	  		  	{
  			  		UnLock (lock[cur]);
  			  		lock[cur] = DONE;
							if (!CheckDirectory ())
								goto end;
			  			break;
		   			}

		 			  if (fib->fib_DirEntryType < 0)	// A file?
		 			  {
		 			  	LONG days_diff;
		 			  	BOOL go_flag = TRUE;

						 	if ((Name[strlen (Name) - 1] != ':') AND (Name[strlen (Name) - 1] != '/'))
						  	strcat (Name, "/");
						  SPrintF (FullName, "%s%s", Name, fib->fib_FileName);

							if (prefs->skip_days)
							{
								days_diff = stamp_now.ds_Days - fib->fib_Date.ds_Days;
								if (days_diff > prefs->days_count)	// Skal vi skippe?
								{
									go_flag = FALSE;	// JA!
									fileCount++;
									FileStat ("Skipping files...");
								}
							}

							if (go_flag)
					      fib_Insett (FullName, fib->fib_Size, 0);
				    }
		 			  else if (fib->fib_DirEntryType == 2)	// 3 = SOFT 4 = HARD
  				  {
  				  	char New_dir[300];

  			  		NameFromLock (lock[cur], New_dir, 300);	// Vi får aldri med '/'
	 				  	if (New_dir[strlen (New_dir) - 1] != ':')
	  				  	strcat (New_dir, "/");
	 			  		strcat (New_dir, fib->fib_FileName);
	
							dir_depth = FindAvailDepth ();
					    if ((dir_depth == MAX_DEPTH - 1) OR dir_depth == ERROR)
					    {
								JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR65), GLS (&L_OK), S_ERROR);
								ret = FALSE;
								goto end;
							}	

  				  	if (dir_depth > max_depth)
  				  		max_depth = dir_depth;

	 		  			if (!(lock[dir_depth] = Lock (New_dir, ACCESS_READ)))
 			  			{
								SPrintF (Dummy, GLS (&L_ERROR_LOCKING_DIR), New_dir);
								JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
								lock[dir_depth] = DONE;
							}
						}
					}
  		  }
  		}
  	}
	}

end:
	for (i = 0; i < MAX_DEPTH; i++)	// Just in case
	{
		if (lock[i] > 0)
			UnLock (lock[i]);
	}
}

struct voldata
{
  UBYTE *vd_name;
  UBYTE vd_buf[1];
};

VOID ClearFileStatus (BOOL mode);

VOID CheckFiles (UBYTE *Dir, BOOL mode)
{
  UBYTE Hold[108] = "RAM:";
  BOOL overwrite_tmp;
  char Temp[108];
  char Date[20], Time[20];
  ULONG timeEnd;
  BOOL write_status = TRUE;

  add_to_watch = 0;

	main_stop = main_check_stop = multi_check = FALSE;
	GetDateTime (Date, Time, 0);	// Henter tiden nå...
	stamp_now = Convert_DateTime_to_stamp (Date, Time);

	arc_depth = 0;
	autoremovelink = autodeletefile = FALSE;

	overwrite_status = FALSE;
	overwrite_tmp = prefs->overwrite_crunched_files;

	if (prefs->fast_file_checking)
		JEOWrite (rp, mainX + 30, 123+OFFY, "     ", BLACK);	// Disable clock

	filecheck_progress = TRUE;
	if (mode == 0)
	{
		if (Dir[0])	// Sjekk 1 dir av gangen
			mystrcpy (Hold, Dir);
		else
		{
			if (prefs->FileCheckPath[0])	// Er den ok?
			{
				if (Exists (prefs->FileCheckPath))	// F eks df0: som er tatt ut!
					mystrcpy (Hold, prefs->FileCheckPath);
			}
			if (!(GetDirName (Hold, GLS (&L_CHECK_FILES_VIRUS), NULL)))
	  	{
	  	  Status (GLS (&L_READY));
  	  	return;
	  	}
		  else
				mystrcpy (prefs->FileCheckPath, Hold);
		}
	  len = strlen (Hold);

		SPrintF (Temp, "Started file checking from: \"%s\"", Hold);
		WriteToLog (Temp, LOG_DATE);
	}

  dirCount = archived = archiveCount = fileCount = cFound = cFiles = virusCount = c = 0;
	exeFiles = dataFiles = 0;
	pack_checking = OFF;
	CrunchName[0] = 0;

	fib_Open ();
	hunkresult = AllocMem (4, MEMF_CLEAR);
	timeStart = timeEnd = 0;
	CurrentTime (&timeStart, &msecs);

  fib = AllocMem (sizeof (struct FileInfoBlock), MEMF_CLEAR);

	if (mode == 0)	// Single dirs..
	{
		ClearFileStatus (1);
		if (!CheckDiskState (Hold))	// Ikke writeable?
			prefs->overwrite_crunched_files = FALSE;	// Slår av dette flagget...
		ScanDirectory (Hold);
		SPrintF (Temp, "Ended file checking from: \"%s\"", Hold);
		WriteToLog (Temp, LOG_DATE);
		WriteToLog ("", LOG_END);
	}
	else
	{
		BPTR lock;
		struct RootNode *rn;
		struct DosInfo *di;
		struct DeviceList *dl;
		struct InfoData *id;
		char VolumeName[50][81];
		UBYTE max_volumes, count = 0;
		char Hold[81];

		rn = DOSBase->dl_Root;
		di = BADDR (rn->rn_Info);
		dl = BADDR (di->di_DevInfo);

		if (id = AllocMem (sizeof (struct InfoData), MEMF_PUBLIC))
		{
			Forbid ();
			while (dl->dl_Next)
			{
				if (dl->dl_Type == DLT_VOLUME)
				{
					SPrintF (Hold, "%b:", dl->dl_Name);
					if (lock = Lock (Hold, ACCESS_READ))
					{
						BOOL go_flag = TRUE;

						if (Info (lock, id))
						{
							if (id->id_DiskState != ID_VALIDATING)
							{
								switch (id->id_DiskType)
								{
									case ID_NO_DISK_PRESENT: go_flag = FALSE;
									case ID_UNREADABLE_DISK: go_flag = FALSE;
									case ID_NOT_REALLY_DOS: go_flag = FALSE;
									case ID_KICKSTART_DISK: go_flag = FALSE;
								}
							}
						}
						else
							go_flag = FALSE;
						UnLock (lock);

						if (go_flag)
						{
							if (go_flag)
								strcpy (VolumeName[count++], Hold);
						}
					}
				}
				dl = BADDR (dl->dl_Next);
			}
			Permit ();
			max_volumes = count;
			FreeMem (id, sizeof (struct InfoData));
		}
		if (max_volumes)
		{
			BYTE i, j;
			struct voldata *vd;

			strcpy (Dummy, GLS (&L_DO_FILE_CHECK_VOL));
			strcat (Dummy, "\n\n");
			for (i = max_volumes - 1, j = 0; i >= 0; i--, j++)
			{
				volumFlag[j] = ON;	// All on...

				mystrcpy (Hold, VolumeName[i]);

				VolumNames[j][0] = 0; 
				if (vd = GetVolData (Hold))
				{
					Hold[strlen (Hold) - 1] = 0;
					SPrintF (VolumNames[j], "%s: '%s'", vd->vd_name, Hold);
					FreeVec (vd);
			  }
			}

			CheckAllHeight = (70 + (max_volumes * 11));
			CheckAllNGad[0].ng_TopEdge = CheckAllNGad[1].ng_TopEdge = (11 * max_volumes) + 34;

			OpenCheckAllWindow (max_volumes);
			StyleIt2 (CheckAllWnd->RPort, 18 , 26, 350-18, 26 + ((11 * max_volumes)) + 8, BLACK, WHITE, GADGET_STYLE);
			count = HandleCheckAll (max_volumes);	// returns nr;
			CloseCheckAllWindow ();

			if (count)
			{
				ClearFileStatus (1);

				multi_check = TRUE;
				for (i = max_volumes - 1, j = 0; i >= 0; i--, j++)
				{
					if (volumFlag[j])	// Flagged?
					{
						if (count == 1)	// last volume_
							multi_check = FALSE;
						main_stop = main_check_stop = FALSE;
						SPrintF (Temp, "Started file checking from: \"%s\"", VolumeName[i]);
						WriteToLog (Temp, LOG_DATE);
						if (!CheckDiskState (VolumeName[i]))	// Ikke writeable?
							prefs->overwrite_crunched_files = FALSE;	// Slår av dette flagget...
						ScanDirectory (VolumeName[i]);
						SPrintF (Temp, "Ended file checking from: \"%s\"", VolumeName[i]);
						WriteToLog (Temp, LOG_DATE);
						WriteToLog ("", LOG_END);
						prefs->overwrite_crunched_files = overwrite_tmp;

						if (!main_check_stop)
						{
							if (main_stop)
								break;
						}
						count--;
					}
				}
			}
			else
				write_status = FALSE;
		}
	}
	FreeMem (fib, sizeof (struct FileInfoBlock));
	FreeMem (hunkresult, 4);
  fib_Close ();

	filecheck_progress = FALSE;
	if (write_status)
	{
		if (!aminet_flag)
		{
			ULONG timeStart2;

			FileStat ("");	// Get time...

			CurrentTime (&timeEnd, &msecs);

			timeStart2 = timeStart;
			timeEnd -= timeStart2;
			timeStart2 = timeEnd / 60;
			timeEnd %= 60;
			msecs = timeStart2 / 60;
			timeStart2 %= 60;

			SPrintF (FileTime, "%ld:%02ld:%02ld", msecs, timeStart2, timeEnd);

		  Status (GLS (&L_PLEASE_WAIT));
	  	SPrintF (Dummy, GLS (&L_FILE_SCAN_END1),
                   dirCount, dirCount == 1 ? GLS (&L_DIR) : GLS (&L_DIRS), fileCount, fileCount == 1 ? GLS (&L_FILE) : GLS (&L_FILES),
                   exeFiles, exeFiles == 1 ? GLS (&L_FILE) : GLS (&L_FILES), cFiles,
                   dataFiles, dataFiles == 1 ? GLS (&L_FILE) : GLS (&L_FILES),
                   archiveCount, archiveCount == 1 ? GLS (&L_ARCHIVE) : GLS (&L_ARCHIVES),
                   FileTime);

		  SPrintF (Hold, GLS (&L_FOUND), virusCount);
  		strcat (Dummy, Hold);
			PrintTime ();
			PlaySound (S_MESSAGE);
  		rtEZRequestTags (Dummy, GLS (&L_OK) , NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
	                   RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_FILE_CHECK_STATUS), RT_TextAttr, &topaz8, TAG_END);
	  }
		if (!prefs->fast_file_checking AND filestat)
		{
			ClearFileStatus (0);
  		UpDateDriveInfo ();

//		SizeWindow (VirusWnd, 0, -100);
		}
	}

	prefs->overwrite_crunched_files = overwrite_tmp;
	prefs->fileChecked += (exeFiles + dataFiles);

	if (add_to_watch)
	{
		SPrintF (Dummy, GLS (&L_ADDED_FILES_TO_WATCH), add_to_watch, add_to_watch == 1 ? GLS (&L_FILE) : GLS (&L_FILES));
		if (JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_SAVE_CONTINUE), S_MESSAGE))
		{
			s_save_flag = TRUE;
			s_SaveFiles ();
		}
	}

  Status (GLS (&L_READY));
	All (ON);
}

//*************************************************************************************

VOID fib_SlettBase (VOID)
{
  ULONG s;
  struct Node *nd;

  nd = fib_sjef.list.lh_Head;
  for (s = 0; s < fib_len; s++)
  {
    Remove (nd);
    FreeMem ((UBYTE *)nd, sizeof (Filbase));
    nd = 0;
    nd = fib_sjef.list.lh_Head;
  }
  fib_len = 0;
}

VOID fib_Close (VOID)
{
  Filbase *bp;

  fib_SlettBase ();
  bp = (Filbase *)fib_sjef.list.lh_Head;
  while (bp = (Filbase *)RemHead (&fib_sjef.list))
  {
    FreeMem (bp, sizeof (Filbase));
    bp = 0;
  }
}

struct Node *fib_FindTheOne (UBYTE *Name)
{
  struct Node *nd;

  nd = fib_sjef.list.lh_Head;
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

VOID fib_Rename (UBYTE *Name)
{
  struct Node *nd;
  Filbase *bp = NULL;

  nd = FindTheOne (Name);
  nd = nd->ln_Succ;
  bp = (Filbase *)nd;

  mystrcpy (bp->FileName, Name);
  bp->nd.ln_Name = bp->FileName;
}

VOID fib_Insett (char *FileName, LONG size, ULONG flag)
{
  Filbase *bp = NULL;
  struct Node *nd;
  UWORD len;

  if (bp = AllocMem (sizeof (Filbase), MEMF_CLEAR))
  {
    mystrcpy (bp->FileName, FileName);
    len = strlen (FileName);
    bp->flag = flag;	// Used by fw dir
    bp->size = size;

    bp->nd.ln_Name = bp->FileName;
    if (!(nd = fib_FindTheOne (bp->FileName)))
      AddTail (&fib_sjef.list, &bp->nd);
    else
      Insert (&fib_sjef.list, &bp->nd, nd);

    fib_len++;
  }
}


VOID fib_Open (VOID)
{
  fib_len = 0;
  NewList (&fib_sjef.list);
}

// **********************************************************************
/*
VOID ArcStatus (char *Streng)
{
	char S[71];
	UBYTE len, col;
	UBYTE i, j, start;
	char Hold[71];
	char Tmp[200];

	if (icon OR aminet_flag)
		return;

//	Status (Streng);

	len = strlen (Streng);
	if (len >= 70)
	{
		strncpy (S, &Streng[len - 70], 70);
		S[0] = '<';
		S[1] = '<';
		S[70] = 0;
	}
	else
		JEOCopy (S, Streng, 70, ' ');

	col = 0;
	start = 0;
	for (i = 0, j = 0; ; i++)
	{
		Hold[j] = S[i];
		if ((Hold[j] == '·') OR (S[i] == 0))
		{
			if (Hold[j] != 0)
			{
				Hold[j] = '/';
				Hold[j+1] = 0;
			}
			else
			col++;
//			SPrintF (Tmp, "%ld col: %ld start: %ld\n'%s'\n", pack_checking, col, start * 8, Hold);
			JEOWrite (rp, mainX + STATUSX + 68 + (start*8), STATUSY+OFFY, Hold, col);
//			JEOSoundRequest (GLS (&L_MESSAGE), Tmp, GLS (&L_OK), 0);
			start = i + 1;
			j = 0;
			#if FILEDEBUG 
				printf ("%ld col = %ld\n", pack_checking, col);
			#endif
			if (col == 3)
				col = 0;
		}
		else
			j++;
		if (S[i] == 0)
			break;
	}
}
*/

#define VE_FILEINFO 0
#define VE_DISKINFO 1

VOID Do_xfd_xad_error (char *FileName, LONG err, BOOL mode)
{
	char *ptr;

	if (mode)	// xfd
		ptr = xfdGetErrorText (err);
	else
		ptr = xadGetErrorText (err);
	ptr[0] = toupper (ptr[0]);

	if (!*FileName)
		SPrintF (Dummy, GLS (&L_ERROR_X), ptr);
	else if (pack_checking)	// In archive?
	{
/*
		char Hold[500];
		UWORD off = 0, i, j, k, len;

		mystrcpy (Hold, GLS (&L_ARCHIVE_ERROR_1));
		for (i = 0; i < (strlen (Hold) - 1); i++)
		{
			if (Hold[i] == '%' AND Hold[i+1] == 's')	// First %s
			{
				off = i;
				break;
			}
		}

		len = strlen (FileName);
		for (i = j = 0; i < len; i++, j++)	// strip multiple archives...
		{
			Hold[j] = FileName[i];
			if (i < (len - 3))
			{
				if (FileName[i] == '·' AND FileName[i+1] == '·' AND FileName[i+2] == '>') // Cleanup multiple archives?
				{
					Hold[++j] = FileName[i+1];
					Hold[++j] = FileName[i+2];
					Hold[++j] = '\n';
					for (k = 0; k < off; k++)
						Hold[++j] = ' ';
					i += 2;
				}
			}
		}
		Hold[j] = 0;
*/
		SPrintF (Dummy, GLS (&L_ARCHIVE_ERROR_1), FileName, Archive, ArcFile, ptr);
	}
	else
		SPrintF (Dummy, GLS (&L_FILE_NAME_ERROR), FileName, ptr);
	if (aminet_flag)
		PF (Dummy);
	else
	{
		WriteToLog (Dummy, LOG_LINE_TEXT_LINE);
		if (!prefs->fast_file_checking)
			JEOSoundRequest (mode == 1 ? xfdmaster_name : xadmaster_name, Dummy, GLS (&L_OK), S_ERROR);
	}
}

VOID Do_arc_file_check (char *FileName, struct xadArchiveInfo *ai, BOOL mode)
{
  BYTE ret = FALSE;	// Not done
	BOOL flag, extract_flag;
  struct xadFileInfo *xfi;
  BOOL go_flag = TRUE;
  ULONG size;

  xfi = ai->xai_FileInfo;
	if (!xfi)
	{
		go_flag = FALSE;
		ret = FALSE;
	}
	else
		ret = TRUE;

	flag = FALSE;
  while (xfi)
	{
	 	if (main_stop)
	 		ret = STOP;
		if (!Check_file_intuition ())
			ret = STOP;
		if (ret != TRUE)
			break;

		size = xfi->xfi_Size;
		mystrcpy (ArcFile, xfi->xfi_FileName);
		extract_flag = TRUE;
		if (xfi->xfi_Flags & XADFIF_DIRECTORY)
			extract_flag = FALSE;
		if (xfi->xfi_Flags & XADFIF_LINK)
			extract_flag = FALSE;
		if (!size)
			extract_flag = FALSE;
		if (extract_flag)
		{
			char *Buf = 0;

			if (Buf = AllocMem (size, MEMF_ANY))
			{
				LONG result;
				BOOL pack, exe;

				if (mode == VE_FILEINFO)
					result = xadFileUnArc (ai, XAD_ENTRYNUMBER, xfi->xfi_EntryNumber, XAD_OUTMEMORY, Buf, XAD_OUTSIZE, size, TAG_END);
				else
					result = xadDiskFileUnArc (ai, XAD_ENTRYNUMBER, xfi->xfi_EntryNumber, XAD_OUTMEMORY, Buf, XAD_OUTSIZE, size, TAG_END);
				if (!result)
				{
					BOOL do_flag = FALSE;
					flag = TRUE;

					fileCount++;
					CrunchName[0] = 0;
					FileStat (FileName);

					exe = CheckExeFromBuffer ((ULONG *)Buf, size);
					if (exe)
					{
						exeFiles++;
						do_flag = TRUE;
					}
					else if (pack = Check_pack (Buf, size))
					{
						if (prefs->extract_archives)
							do_flag = TRUE;
					}
					else if (prefs->checkData)	// Check for data???
						do_flag = TRUE;
					if (do_flag)
					{
						linkflag = FALSE;
						if (!main_stop)
							main_check_stop = FALSE;
						stripped = FALSE;
						MainCheck (FileName, Buf, size);
						SPrintF (Dummy, GLS (&L_CHECKING), FileName);
						Status (Dummy);	// Must be in the loop to get back right name...
					}
					if (main_stop)
						ret = STOP;
				}
				else
				{
					Do_xfd_xad_error (FileName, result, 0);
					ret = FALSE;
				}
				FreeMem (Buf, size);
			}
		}
  	xfi = xfi->xfi_Next;
  }
	if (flag)
		archiveCount++;
}

VOID Do_xad (char *FileName, char *Buffer, ULONG size)
{
	LONG result;
	ULONG flags;
	struct xadArchiveInfo *ai;
  BOOL go_flag;
  UWORD pack;

	go_flag = FALSE;
	pack = Check_pack (Buffer, size);
	if (pack)
	{
		if (prefs->extract_archives)
			go_flag = TRUE;
	}

	if (go_flag)	// Er det et arkiv?
	{
		SPrintF (Dummy, GLS (&L_CHECKING), FileName);
		Status (Dummy);	// Must be in the loop to get back right name...

		pack_checking = TRUE;
		if (ai = xadAllocObjectA (XADOBJ_ARCHIVEINFO, 0))
		{
			result = xadGetInfo (ai, XAD_INMEMORY, Buffer, XAD_INSIZE, size, TAG_DONE);
			if (!result)
			{
				flags = ai->xai_Flags;

				mystrcpy (ArcName, FileName);
				mystrcpy (Archive, ai->xai_Client->xc_ArchiverName);	// (lha)

				if (ai->xai_DiskInfo)	// DMS
					Do_disk_image (FileName, Buffer, size);
				else if (ai->xai_FileInfo)
				{
					UWORD buflen;

					go_flag = TRUE;
					if (flags & XADAIF_CRYPTED)
					{
						if (prefs->skip_encrypted)
							go_flag = FALSE;
						else
						{
							char *pwbuf = 0;

							SPrintF (Dummy, GLS (&L_ARCHIVE_IS_CRYPTED), FileName);
							JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);

		    	    buflen = 256;
 				  	  if (pwbuf = AllocMem (buflen, MEMF_ANY | MEMF_CLEAR))
					  	{
							  if (rtGetString (pwbuf, buflen, GLS (&L_ENTER_PW), NULL, RT_ReqPos, REQPOS_CENTERSCR, RTGS_Invisible, TRUE, RT_TextAttr, &topaz8, TAG_END))
							  {
				  	  	  ai->xai_Password = pwbuf;
				    		  go_flag = TRUE;
				  	  	}
				    		else
				    			go_flag = FALSE;

								FreeMem (pwbuf, buflen);
			  	  	}
				  	}
					}
					if (go_flag)
						Do_arc_file_check (FileName, ai, VE_FILEINFO);
		  	}
				if (!result)
					xadFreeInfo (ai);
	      xvsFreeObject(ai);
      }
      else
      {
				mystrcpy (ArcFile, N_A_Msg);
				Do_xfd_xad_error (FileName, result, 0);
			}
		}
	}
}

ASM(LONG) OutHookCheckX(REG(a0, struct Hook *hook), REG(a1, struct xadHookParam *hp))
{
  ULONG i;

  switch(hp->xhp_Command)
  {
  case XADHC_WRITE:
    if((i = 1024-hp->xhp_DataPos) > hp->xhp_BufferSize)
      i = hp->xhp_BufferSize;
    CopyMem(hp->xhp_BufferPtr, ((STRPTR)(hook->h_Data))+hp->xhp_DataPos, i);
    hp->xhp_DataPos += i;
    if(hp->xhp_DataPos == 1024)
      return 10000; /* CheckX own error code for error detection */
  case XADHC_INIT:
  case XADHC_FREE:
  case XADHC_ABORT:
    break;
  default: return XADERR_NOTSUPPORTED;
  }

  return 0;
}

static ASM(LONG) BreakHookCheckX(void);
static const struct Hook breakhook = {{0,0},(ULONG (*)()) BreakHookCheckX, 0};

static ASM(LONG) BreakHookCheckX(void)
{
  return (SetSignal(0L,0L) & SIGBREAKF_CTRL_C) ? 0 : XADPIF_OK;
}

VOID Do_disk_image (char *FileName, UBYTE *Buffer, ULONG size)
{
  LONG err = 0;
  struct TagItem tih[2];
	struct xadArchiveInfo *ai;

	tih[0].ti_Tag = XAD_INMEMORY;
	tih[0].ti_Data = (ULONG)Buffer;

  if ((ai = (struct xadArchiveInfo *) xadAllocObject (XADOBJ_ARCHIVEINFO, 0)))
  {
    if (!xadGetInfo (ai, XAD_INSIZE, size, XAD_PROGRESSHOOK, &breakhook, TAG_MORE, tih))
    {
      if (ai->xai_DiskInfo)
      {
			  STRPTR Buf;

		  	/* reduce stacksize, as we have a recursive program */
			  if ((Buf = (STRPTR)AllocMem (1024+512, MEMF_PUBLIC)))
			  {
	   			struct xadDiskInfo *di;
			    struct xadArchiveInfo *aid;
	   			di = ai->xai_DiskInfo;

			    while (di)
    			{
						if (di->xdi_TextInfo)
						{
							struct xadTextInfo *ti;
							ULONG i = 1;

							for (ti = di->xdi_TextInfo; ti; ti = ti->xti_Next)
							{
								if (ti->xti_Size AND ti->xti_Text)
								{
									strcpy (ArcFile, "DMS->TEXT");
									mystrcpy (ArcName, FileName);
									CheckVirus (FileName, ti->xti_Text, ti->xti_Size);
								}
				        ++i;
				      }
				    }
						// BOOT BLOCK VIRUS CHECK!
						if (di->xdi_LowCyl == 0) // boot check only, when first block!
						{
				      struct Hook hook = {{0,0},(ULONG (*)()) OutHookCheckX, 0};

			        hook.h_Data = Buf;

			        if (xadDiskUnArc (ai, XAD_ENTRYNUMBER, di->xdi_EntryNumber, XAD_OUTHOOK, &hook, XAD_PROGRESSHOOK, NULL, TAG_DONE) == 10000) /* special hook return code! */
							{
								strcpy (ArcFile, "DMS->BOOTBLOCK");
								mystrcpy (ArcName, FileName);
								CheckVirus (FileName, Buf, 1024);
								strcpy (ArcFile, N_A_Msg);
							}
						}
			      if ((aid = (struct xadArchiveInfo *)xadAllocObject (XADOBJ_ARCHIVEINFO, 0)))
			      {
			        struct TagItem ti[4];
			        struct xadClient *cl;

	  					ti[0].ti_Tag = XAD_INSIZE;
					  	ti[0].ti_Data = size;
					  	ti[1].ti_Tag = XAD_ENTRYNUMBER;
					  	ti[1].ti_Data = di->xdi_EntryNumber;
              ti[2].ti_Tag = XAD_PROGRESSHOOK;
              ti[2].ti_Data = (ULONG)&breakhook;
					  	ti[3].ti_Tag = TAG_MORE;
					  	ti[3].ti_Data = (ULONG)tih;
	
					  	if (!(err = xadGetDiskInfo(aid, XAD_NOEMPTYERROR, 1, XAD_PROGRESSHOOK, &breakhook, XAD_INDISKARCHIVE, ti, TAG_DONE)))
					  	{
								Do_arc_file_check (FileName, aid, VE_DISKINFO);
							  cl = aid->xai_Client->xc_Next;
							  xadFreeInfo (aid);
							  while (cl) // multiple filesystems
							  {
					  	    if (!xadGetDiskInfo (aid, XAD_NOEMPTYERROR, 1, XAD_INDISKARCHIVE, ti, XAD_PROGRESSHOOK, &breakhook, XAD_STARTCLIENT, cl, TAG_MORE, TAG_DONE))
					  	    {
										Do_arc_file_check (FileName, aid, VE_DISKINFO);
							      cl = aid->xai_Client->xc_Next;
							      xadFreeInfo (aid);
							    }
							    else
						      cl = 0;
							  }
					  	}
					  	else
								Do_xfd_xad_error (FileName, err, 0);
					  	xadFreeObjectA (aid, 0);
	  		    }
		        di = di->xdi_Next;
					} // while
					FreeMem (Buf, 1024+512);
        } // AllocMem buffer
				else
					Do_xfd_xad_error ("", XADERR_NOMEMORY, 0);
      }
      xadFreeInfo(ai);
    } // xadGetInfo
/*
    else if (!xadGetDiskInfo (ai, XAD_NOEMPTYERROR, 1, XAD_PROGRESSHOOK, &breakhook, XAD_INSIZE, size, TAG_MORE, tih))
    {
      struct xadClient *cl;
      ULONG isvalid = 1;

      if (fd->fd_Flags & CHECKXFLAG_NOUNTRACK)
      {
        if(buffer)
          err = DoFileUnLink(fd, buffer, size);
      }
      else if (!OpenNewDir(fd, FilePart(fd->fd_Name)))
        err = CHKXERR_OPENERR;
      else
      {
				Do_arc_file_check (FileName, aid, VE_DISKINFO);
        while (isvalid AND (cl = ai->xai_Client->xc_Next))
        {
          xadFreeInfo (ai);

          if (!xadGetDiskInfo(ai, XAD_NOEMPTYERROR, 1, XAD_PROGRESSHOOK, &breakhook, XAD_INSIZE, size, XAD_STARTCLIENT, cl, TAG_MORE, tih))
          {
            if(ai->xai_Flags & XADAIF_FILECORRUPT)
            {
              ++fd->fd_CorruptedArchives;
              PrintCHKXTxt(fd, "%s (FILESYSTEM, CORRUPTED)", ai->xai_Client->xc_ArchiverName);
            }
            else
              PrintCHKXTxt(fd, "%s (FILESYSTEM)", ai->xai_Client->xc_ArchiverName);
            CheckArcFiles(fd, ai, 1);
          }
          else
            isvalid = 0;
        } 
        if(!OpenParentDir(fd))
          err = CHKXERR_OPENDIR;
      }
      if(isvalid)
        xadFreeInfo(ai);
    }
    else if (buffer)
      err = DoFileUnLink(fd, buffer, size);
*/
    xadFreeObjectA (ai, 0);
  }
  else
		Do_xfd_xad_error ("", XADERR_NOMEMORY, 0);
}

// ******************************************************************************

struct voldata *GetVolData (char *name)
{
  struct voldata *vd = NULL;
  ULONG tmpnamelen;
  UBYTE *tmpname;
  struct DosList *doslist, *dosentry;
  struct MsgPort *handler;

  tmpnamelen = strlen (name);
  if (!(tmpname = AllocMem (tmpnamelen + 1, MEMF_ANY)))
    return NULL;

  strcpy (tmpname, name);
  if (tmpname[tmpnamelen - 1] == ':')
  {
    tmpname[tmpnamelen - 1] = '\0';
  }

  doslist = LockDosList(LDF_VOLUMES | LDF_DEVICES | LDF_READ);
  dosentry = FindDosEntry (doslist, tmpname, LDF_VOLUMES);
  FreeMem (tmpname, tmpnamelen + 1);

  if (dosentry)
  {
    /* Scan the devicelist for entry with a matching handler */
    handler = dosentry->dol_Task;

    dosentry = doslist;
    while ((dosentry = NextDosEntry(dosentry, LDF_DEVICES)))
    {
      if (dosentry->dol_Task == handler)
      {
        const UBYTE *volname;

        volname = BADDR(dosentry->dol_Name);
        if (volname)
        {
          vd = AllocVec(sizeof(struct voldata) + volname[0], MEMF_PUBLIC);
          if (vd)
          {
            vd->vd_name           = vd->vd_buf;
            CopyMem((APTR) &volname[1], vd->vd_name, volname[0]);
            vd->vd_name[volname[0]] = '\0';

            break;
          }
        }
      }
    }
  }

  UnLockDosList(LDF_VOLUMES | LDF_DEVICES | LDF_READ);

  return vd;
}

/*
#define XADAIF_CRYPTED		(1<<XADAIB_CRYPTED)
#define XADAIF_FILECORRUPT	(1<<XADAIB_FILECORRUPT)
#define XADAIF_FILEARCHIVE	(1<<XADAIB_FILEARCHIVE)
#define XADAIF_DISKARCHIVE	(1<<XADAIB_DISKARCHIVE)
#define XADAIF_OVERWRITE	(1<<XADAIB_OVERWRITE)
#define XADAIF_MAKEDIRECTORY	(1<<XADAIB_MAKEDIRECTORY)
#define XADAIF_IGNOREGEOMETRY	(1<<XADAIB_IGNOREGEOMETRY)
#define XADAIF_VERIFY		(1<<XADAIB_VERIFY)
#define XADAIF_NOKILLPARTIAL	(1<<XADAIB_NOKILLPARTIAL)
#define XADAIF_DISKIMAGE	(1<<XADAIB_DISKIMAGE)
#define XADAIF_FORMAT		(1<<XADAIB_FORMAT)
#define XADAIF_NOEMPTYERROR	(1<<XADAIB_NOEMPTYERROR)
#define XADAIF_ONLYIN		(1<<XADAIB_ONLYIN)
#define XADAIF_ONLYOUT		(1<<XADAIB_ONLYOUT)
#define XADAIF_USESECTORLABELS	(1<<XADAIB_USESECTORLABELS)

#define XADCF_FILEARCHIVER	(1<<XADCB_FILEARCHIVER)
#define XADCF_DISKARCHIVER	(1<<XADCB_DISKARCHIVER)
#define XADCF_EXTERN		(1<<XADCB_EXTERN)
#define XADCF_FILESYSTEM	(1<<XADCB_FILESYSTEM)
#define XADCF_NOCHECKSIZE	(1<<XADCB_NOCHECKSIZE)
#define XADCF_FREESKIPINFO	(1<<XADCB_FREESKIPINFO)
#define XADCF_FREETEXTINFO	(1<<XADCB_FREETEXTINFO)
#define XADCF_FREETEXTINFOTEXT	(1<<XADCB_FREETEXTINFOTEXT)
#define XADCF_FREEFILEINFO	(1<<XADCB_FREEFILEINFO)
#define XADCF_FREEDISKINFO	(1<<XADCB_FREEDISKINFO)
*/
