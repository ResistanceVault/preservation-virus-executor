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

#define STATUSX		    5
#define STATUSY		   26

char FullName[500];
char InArchiveName[2048];

struct DateStamp stamp_now;

BYTE Do_xad (char *FileName, char *Buffer, ULONG size);
BOOL CheckTeamMOS (UWORD *Buffer, LONG size);
BOOL CheckBobek (UWORD *Buf, ULONG size);
BOOL CheckBastard (UWORD *Buf, ULONG size);
VOID ScanPoly (UWORD *Buffer, LONG size);
UWORD Check_pack (UBYTE *Buffer);
BOOL CheckExclude (char *Comment);
VOID fib_Close (VOID);

BOOL link_virus_found;
BOOL main_stop;
BOOL checkArcs;
char CrunchName[50];
BOOL overwrite_status;
BOOL stripped;

extern struct ExecBase *SysBase;

#define ARC_PATH_SIZE	12
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

ULONG StripHunkFile (APTR Buffer, ULONG size)
{
	UWORD error;

	error = xfdStripHunks (Buffer, size, hunkresult, XFDSHF_NAME);
	if (error == XFDERR_OK)
		size = *hunkresult;

	return (size);
}

// ************************ MAIN CHECK *********************

#define STOP				-1
#define XVS_LIB			0
#define INTERNAL		1

VOID PF (char *String)	// For aminet use only
{
	Write (Output (), String, strlen (String));
}

VOID Delete_action (BOOL mode, char *VirusType, char *VirusName, char *FileName)
{
	UWORD action;
	char Hold[40];

	if (autodeletefile)
		action = 1;
	else
	{
		PlaySound (S_VIRUS);
		if (aminet_flag)
		{
			sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n", VirusName, FileName);
			PF (Dummy);
			action = 0;
			main_stop = TRUE;
		}
		else
		{
			sprintf (Dummy, GLS (&L_INFECTED_DELETE), VirusName, FileName);
			sprintf (Hold, GLS (&L_VIRUS_WARNING), VirusType);
			if (mode)
				strcat (Dummy, "\n\nThis is really a link-virus, but I need som more infected files\nin order to fix them...\n\nWhen I run this virus on my test machine it crashes.\nSo if you have an infected file, please mail it to me at:\nvirusexecutor.c2i.net\n\nThanks :)");
			action = rtEZRequestTags (Dummy, GLS (&L_DELETE_FILE_AUTO) , NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
  		                 RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, Hold, RT_TextAttr, &topaz8, TAG_END);
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
				WriteToLog ("File is deleted", LOG_REPORT_TEXT);
			else
			{
				WriteToLog ("Error deleting file", LOG_REPORT_TEXT);

				sprintf (Dummy, GLS (&L_FILE_NOT_DELETED), FileName);
				JEOSoundRequest (GLS (&L_MESSAGE),  Dummy, GLS (&L_OK), S_ERROR);
				All (OFF);
			}
			break;
		}
		case 3:	// Stop checking
		{
      WriteToLog (GLS (&L_USER_BREAK), LOG_REPORT_TEXT);
			main_stop = TRUE;
			break;
		}
		case 0:	// Continue
		{
			WriteToLog ("No action taken", LOG_REPORT_TEXT);
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
		sprintf (Dummy, "Possible encrypted file:\n%s\nSize: %ld", FileName, size);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
		All (OFF);
	}
}
*/

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

BYTE CheckVirus (char *FileName, UBYTE *Address, LONG size)
{
  BYTE ret;
	UWORD action;
	BPTR fh = 0;
	struct xvsFileInfo *fileinfo;

	if (Check_pack (Address))
		return (FALSE);

	if (!(fileinfo = xvsAllocObject (XVSOBJ_FILEINFO)))
	{
		JEOSoundRequest (XVS_NAME, GLS (&L_NO_MEM), GLS (&L_OK), S_ERROR);
		main_stop = TRUE;
		return (FALSE);
	}

	fileinfo->xvsfi_File = Address;
	fileinfo->xvsfi_FileLen = size;
	xvsCheckFile (fileinfo);

	ret = FALSE;	// No found?
	switch (fileinfo->xvsfi_FileType)
	{
		case XVSFT_EMPTYFILE: break;
		case XVSFT_DATAFILE:
		{
			dataFiles++;
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
					sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n  ARC: '%s'\n", fileinfo->xvsfi_Name, ArcFile, ArcName);
					PF (Dummy);
				}
				else
				{
					sprintf (Dummy, "DATA VIRUS FOUND: %s\n            FILE: %s\n         ARCHIVE: %s", fileinfo->xvsfi_Name, ArcFile, ArcName);
					WriteToLog (Dummy, LOG_STANDARD_TEXT);
					sprintf (Dummy, GLS (&L_VIRUS_INFECTED_ARCHIVE), fileinfo->xvsfi_Name, ArcFile, ArcName);
					JEOSoundRequest (GLS (&L_DATA_VIRUS_WARNING), Dummy, GLS (&L_OK), S_VIRUS);
					All (OFF);
				}
			}
			else
			{
				sprintf (Dummy, "DATA VIRUS FOUND: %s\n            FILE: %s", fileinfo->xvsfi_Name, FileName);
				WriteToLog (Dummy, LOG_STANDARD_TEXT);

				Delete_action (XVS_LIB, GLS (&L_DATA), fileinfo->xvsfi_Name, FileName);
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
					sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n  ARC: '%s'\n", fileinfo->xvsfi_Name, ArcFile, ArcName);
					PF (Dummy);
				}
				else
				{
					sprintf (Dummy, "FILE VIRUS FOUND: %s\n            FILE: %s\n         ARCHIVE: %s", fileinfo->xvsfi_Name, ArcFile, ArcName);
					WriteToLog (Dummy, LOG_STANDARD_TEXT);
					sprintf (Dummy, GLS (&L_VIRUS_INFECTED_ARCHIVE), fileinfo->xvsfi_Name, ArcFile, ArcName);
					JEOSoundRequest (GLS (&L_FILE_VIRUS_WARNING), Dummy, GLS (&L_OK), S_VIRUS);
					All (OFF);
				}
			}
			else
			{
				sprintf (Dummy, "FILE VIRUS FOUND: %s\n            FILE: %s", fileinfo->xvsfi_Name, FileName);
				WriteToLog (Dummy, LOG_STANDARD_TEXT);

				Delete_action (XVS_LIB, GLS (&L_FILE1), fileinfo->xvsfi_Name, FileName);
			}
			break;
		}
		case XVSFT_LINKVIRUS:
		{
			virusCount++;
			prefs->fileVir++;
			UpDateVirusCount ();
			ret = TRUE;

			if (pack_checking)	// Er vi i et arkiv?
			{
				if (aminet_flag)
				{
					sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n  ARC: '%s'\n", fileinfo->xvsfi_Name, ArcFile, ArcName);
					PF (Dummy);
				}
				else
				{
					sprintf (Dummy, "LINK VIRUS FOUND: %s\n            FILE: %s\n         ARCHIVE: %s", fileinfo->xvsfi_Name, ArcFile, ArcName);
					WriteToLog (Dummy, LOG_STANDARD_TEXT);
					sprintf (Dummy, GLS (&L_VIRUS_INFECTED_ARCHIVE), fileinfo->xvsfi_Name, ArcFile, ArcName);
					JEOSoundRequest (GLS (&L_LINK_VIRUS_WARNING), Dummy, GLS (&L_OK), S_VIRUS);
					All (OFF);
				}
			}
			else
			{
				sprintf (Dummy, "LINK VIRUS FOUND: %s\n            FILE: %s", fileinfo->xvsfi_Name, FileName);
				WriteToLog (Dummy, LOG_STANDARD_TEXT);

				if (autoremovelink)
					action = 1;	// Don't ask..
				else
				{
					PlaySound (S_VIRUS);

					if (aminet_flag)
					{
						action = 0;
						sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n", fileinfo->xvsfi_Name, FileName);
						PF (Dummy);
					}
					else
					{
						sprintf (Dummy, GLS (&L_VIRUS_INFECTED_FILE), fileinfo->xvsfi_Name, FileName);
						action = rtEZRequestTags (Dummy, GLS (&L_REMOVE_VIRUS_AUTO), NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
    		  		RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_LINK_VIRUS_WARNING), RT_TextAttr, &topaz8, TAG_END);
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
								sprintf (Dummy, GLS (&L_VIRUS_REMOVED), size, new_size);
								action = JEOSoundRequest (GLS (&L_MESSAGE),  Dummy, GLS (&L_SAVE_NEW_FILE), S_NO_SOUND);
								All (OFF);
							}
							if (action == 1)
							{
								if (fh = Open (FileName, MODE_NEWFILE))
								{
									if (Write (fh, fileinfo->xvsfi_Fixed, new_size) != new_size)
									{
										WriteToLog ("Error saving file", LOG_REPORT_TEXT);

										sprintf (Dummy, GLS (&L_ERROR_WRITING_FILE), FileName);
										JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
										All (OFF);
									}
									else
									{
										WriteToLog ("Virus is now removed from file", LOG_REPORT_TEXT);
										stripped = TRUE;	// Flytte denne ned :)
									}
									Close (fh);
								}
								else
								{
									WriteToLog ("Error opening file", LOG_REPORT_TEXT);

									sprintf (Dummy, GLS (&L_ERROR_OPENING_FILE), FileName);
									JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
									All (OFF);
								}
							}
							else
					      WriteToLog ("No action taken", LOG_REPORT_TEXT);
						}
						else	// Error repairing file
						{
							WriteToLog ("Error removing virus from file", LOG_REPORT_TEXT);

							sprintf (Dummy, GLS (&L_ERROR_VIRUS_NOT_REMOVED), fileinfo->xvsfi_Name);
							action = JEOSoundRequest (GLS (&L_ERROR_REMOVING_VIRUS),  Dummy, GLS (&L_DELETE_FILE_STOP), S_ERROR);
							All (OFF);
							switch (action)
							{
								case 1:	// Take action
								{
//									flag = TRUE;	// Don't get an error when returning
						      SetProtection (FileName, 0);	// Unprotect
									if (!(action = DeleteFile (FileName)))
									{
										sprintf (Dummy, GLS (&L_FILE_NOT_DELETED), FileName, action);
										JEOSoundRequest (GLS (&L_MESSAGE),  Dummy, GLS (&L_OK), S_ERROR);
										All (OFF);
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
			      WriteToLog (GLS (&L_USER_BREAK), LOG_REPORT_TEXT);
						main_stop = TRUE;
						break;
					}
					case 0:	// Continue
					{
			      WriteToLog ("No action taken", LOG_REPORT_TEXT);
  	    		break;
	  	    }
				}
			}
			break;
		}
		case XVSFT_EXEFILE:
		{
			if (CheckTeamMOS ((UWORD *)Address, size))
			{
				ret = TRUE;
				virusCount++;
				prefs->fileVir++;
				UpDateVirusCount ();
				fileinfo->xvsfi_Name = "Zakahackandpatch TCP Trojan";
				if (pack_checking)	// Er vi i et arkiv?
				{
					if (aminet_flag)
					{
						sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n  ARC: '%s'\n", fileinfo->xvsfi_Name, ArcFile, ArcName);
						PF (Dummy);
					}
					else
					{
						sprintf (Dummy, "        VIRUS: %s\n\nInfected file: %s\n      Archive: %s", fileinfo->xvsfi_Name, ArcFile, ArcName);
						JEOSoundRequest ("FILE VIRUS WARNING!", Dummy, GLS (&L_OK), S_VIRUS);
						All (OFF);
					}
				}
				else
					Delete_action (XVS_LIB, "FILE", fileinfo->xvsfi_Name, FileName);
			}
			else if (CheckBastard ((UWORD *)Address, size))
			{
				ret = TRUE;
				virusCount++;
				prefs->fileVir++;
				UpDateVirusCount ();
				fileinfo->xvsfi_Name = "Bastard";
				if (pack_checking)	// Er vi i et arkiv?
				{
					if (aminet_flag)
					{
						sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n  ARC: '%s'\n", fileinfo->xvsfi_Name, ArcFile, ArcName);
						PF (Dummy);
					}
					else
					{
						sprintf (Dummy, "        VIRUS: %s\n\nInfected file: %s\n      Archive: %s", fileinfo->xvsfi_Name, ArcFile, ArcName);
						JEOSoundRequest ("FILE VIRUS WARNING!", Dummy, GLS (&L_OK), S_VIRUS);
						All (OFF);
					}
				}
				else
					Delete_action (INTERNAL, "FILE", fileinfo->xvsfi_Name, FileName);
			}
			else if (CheckBobek ((UWORD *)Address, size))
			{
				ret = TRUE;
				new_size = size - 460;
				fileinfo->xvsfi_Name = "Bobek";
				virusCount++;
				prefs->fileVir++;
				UpDateVirusCount ();

				if (pack_checking)	// Er vi i et arkiv?
				{
					if (aminet_flag)
					{
						sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n  ARC: '%s'\n", fileinfo->xvsfi_Name, ArcFile, ArcName);
						PF (Dummy);
					}
					else
					{
						sprintf (Dummy, "LINK VIRUS FOUND: %s\n            FILE: %s\n         ARCHIVE: %s", fileinfo->xvsfi_Name, ArcFile, ArcName);
						WriteToLog (Dummy, LOG_STANDARD_TEXT);
						sprintf (Dummy, GLS (&L_VIRUS_INFECTED_ARCHIVE), fileinfo->xvsfi_Name, ArcFile, ArcName);
						JEOSoundRequest (GLS (&L_LINK_VIRUS_WARNING), Dummy, GLS (&L_OK), S_VIRUS);
						All (OFF);
					}
				}
				else
				{
					sprintf (Dummy, "LINK VIRUS FOUND: %s\n            FILE: %s", fileinfo->xvsfi_Name, FileName);
					WriteToLog (Dummy, LOG_STANDARD_TEXT);

					if (autoremovelink)
						action = 1;	// Don't ask..
					else
					{
						PlaySound (S_VIRUS);

						if (aminet_flag)
						{
							action = 0;
							sprintf (Dummy, "VIRUS: '%s'\n FILE: '%s'\n", fileinfo->xvsfi_Name, FileName);
							PF (Dummy);
						}
						else
						{
							sprintf (Dummy, GLS (&L_VIRUS_INFECTED_FILE), fileinfo->xvsfi_Name, FileName);
							action = rtEZRequestTags (Dummy, GLS (&L_REMOVE_VIRUS_AUTO), NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
    		  			RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_LINK_VIRUS_WARNING), RT_TextAttr, &topaz8, TAG_END);
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

							if (autoremovelink)
								action = 1;
							else
							{							
								sprintf (Dummy, GLS (&L_VIRUS_REMOVED), size, new_size);
								action = JEOSoundRequest (GLS (&L_MESSAGE),  Dummy, GLS (&L_SAVE_NEW_FILE), S_NO_SOUND);
								All (OFF);
							}
							if (action == 1)
							{
								if (fh = Open (FileName, MODE_NEWFILE))
								{
									if (Write (fh, Address, new_size) != new_size)
									{
										WriteToLog ("Error saving file", LOG_REPORT_TEXT);

										sprintf (Dummy, GLS (&L_ERROR_WRITING_FILE), FileName);
										JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
										All (OFF);
									}
									else
									{
										stripped = TRUE;	// Flytte denne ned :)
										WriteToLog ("Virus is now removed from file", LOG_REPORT_TEXT);
									}
									Close (fh);
								}
								else
								{
									WriteToLog ("Error opening file", LOG_REPORT_TEXT);

									sprintf (Dummy, GLS (&L_ERROR_OPENING_FILE), FileName);
									JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
									All (OFF);
								}
							}
							else
					      WriteToLog ("No action taken", LOG_REPORT_TEXT);
					    break;
						}
						case 3:	// Stop checking
						{
				      WriteToLog (GLS (&L_USER_BREAK), LOG_REPORT_TEXT);
							main_stop = TRUE;
							break;
						}
						case 0:	// Continue
						{
				      WriteToLog ("No action taken", LOG_REPORT_TEXT);
  	    			break;
		  	    }
					}
				}
			}
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

ULONG FileBuffer[2];

BOOL CheckExeFromBuffer (ULONG *Buf, ULONG size)
{
	if (size < 8)
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
    if (Read (fh, FileBuffer, 8) == ERROR)		// protected?
    {
      SetProtection (FileName, 0);						// Unprotect
      if (Read (fh, FileBuffer, 8) == ERROR)	// Error
        ret = FALSE;
    }
    Close (fh);
		if (!ret)
		{
			sprintf (Dummy, GLS (&L_ERROR_READING_FILE), FileName);
			JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
			All (OFF);
		}
  }
  else
  {
  	ret = FALSE;
		sprintf (Dummy, GLS (&L_ERROR_OPENING_FILE), FileName);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
		All (OFF);
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
				All (OFF);
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
						if (JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_FILE_CHECK_PAUSED), GLS (&L_CONTINUE_STOP), S_NO_SOUND))
							All (OFF);
						else
						{
				      Status (GLS (&L_USER_BREAK));
				      WriteToLog (GLS (&L_USER_BREAK), LOG_REPORT_TEXT);
			  	    ret = FALSE;
						}
						ClearIntuition (VirusWnd);
						break;
					}
			  }
	  	}
	  }
	}
	return (ret);
}

BOOL main_check_stop;

VOID MainCheck (char *FileName, char *Buffer, ULONG size)
{
	if (main_stop)
		return;
	if (main_check_stop)
		return;

	CrunchName[0] = 0;

	printf ("VIRUS\n");
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
					main_check_stop = FALSE;
						return;
				}
			}
		}
	}

	printf ("UNARHICVE\n");
// UNARCHIVE
	if (!main_check_stop)
	{
		if (xadmaster_flag)
		{
			if (pack_checking)
			{
				if (Check_pack (Buffer))
				{
					sprintf (InArchiveName, "%s>%s", FileName, ArcFile);
					Do_xad (InArchiveName, Buffer, size);
				}
			}
			else
			{
				mystrcpy (InArchiveName, FileName);
				Do_xad (FileName, Buffer, size);
			}
		}
	}

	printf ("UNLINK\n");
// UNLINK
	if (!main_check_stop)
	{
		if (xfdmaster_flag)
		{
			struct xfdLinkerInfo *li = 0;

			if (li = AllocMem (sizeof (struct xfdLinkerInfo), MEMF_CLEAR))
			{
				li->xfdli_Buffer = Buffer;
				li->xfdli_BufLen = size;
				if (xfdRecogLinker (li))
				{
					linkflag = TRUE;

					if (!pack_checking)
					{
						sprintf (Dummy, GLS (&L_UNLINKING_FILE), FileName, li->xfdli_LinkerName);
						Status (Dummy);
					}
					if (xfdUnlink (li))	// Fila er linka!
					{
						MainCheck (FileName, li->xfdli_Save1, li->xfdli_SaveLen1);
						if (!main_check_stop)
							MainCheck (FileName, li->xfdli_Save2, li->xfdli_SaveLen2);
					}
				}
				if (li)
					FreeMem (li, sizeof (struct xfdLinkerInfo));
			}
			else
				JEOSoundRequest (XFDM_NAME, GLS (&L_NO_MEM), GLS (&L_OK), S_ERROR);
		}
	}

	printf ("DECRUNCH\n");
// DECRUNCH
	if (!main_check_stop)
	{
		if (xfdmaster_flag)
		{
			struct xfdBufferInfo *bufferinfo = 0;

			if (bufferinfo = xfdAllocObject (XFDOBJ_BUFFERINFO))
			{
				UWORD buflen;

				CrunchName[0] = 0;
				bufferinfo->xfdbi_SourceBuffer = Buffer;
				bufferinfo->xfdbi_SourceBufLen = size;
				bufferinfo->xfdbi_Flags = 0;
				if (xfdRecogBuffer (bufferinfo))	// Crunched?
				{
					BOOL crunch_flag = FALSE;

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

					if (bufferinfo->xfdbi_PackerFlags & XFDPFF_ADDR)	// Vil ikke decrunche
						crunch_flag = FALSE;

					if (crunch_flag)
					{
						char *pwbuf = 0;

						mystrcpy (CrunchName, bufferinfo->xfdbi_PackerName);
						FileStat (FileName);

						if (!pack_checking)
						{
				 			sprintf (Dummy, "%s (%s)...", FileName, bufferinfo->xfdbi_PackerName);
					  	Status (Dummy);
					  }
						if (bufferinfo->xfdbi_PackerFlags & XFDPFF_PASSWORD)
						{
			        buflen = (bufferinfo->xfdbi_MaxSpecialLen == 0xFFFF) ? 256 :
							bufferinfo->xfdbi_MaxSpecialLen;
    			    if (pwbuf = AllocMem (buflen, MEMF_ANY | MEMF_CLEAR))
						  {
							  if (rtGetString (pwbuf, buflen, GLS (&L_ENTER_PW), NULL, RT_ReqPos, REQPOS_CENTERSCR, RTGS_Invisible, TRUE, RT_TextAttr, &topaz8, TAG_END))
					    	  bufferinfo->xfdbi_Special = pwbuf;
					    	All (OFF);
						  }
  	  		  }
						bufferinfo->xfdbi_TargetBufMemType = MEMF_ANY;


	  printf ("Name: %s\nType is '%s'\n", Name, bufferinfo->xfdbi_PackerName);
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


						if (xfdDecrunchBuffer (bufferinfo))		// DECRUNCHER!!!
						{
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

										sprintf (Dummy, GLS (&L_SAVING_FILE), FileName);
										Status (Dummy);
										SetProtection (FileName, 0);	// Unprotect
										if (fh = Open (FileName, MODE_NEWFILE))
										{
											if (Write (fh, bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufLen) != bufferinfo->xfdbi_TargetBufLen)
												JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR_SAVING_FILE), GLS (&L_OK), S_ERROR);
											Close (fh);
										}
									}
								}
							}
							cFiles++;
							MainCheck (FileName, bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufLen);
						}
					  else
						{
		  	 			char *String = "";
							char StripName[500];

							StripFile (FileName, StripName);	// Leaves the file name only
							String = xfdGetErrorText ( bufferinfo->xfdbi_Error);
							String[0] = toupper (String[0]);
							sprintf (Dummy, GLS (&L_FILE_NAME_ERROR), StripName, String);
							JEOSoundRequest (XFDM_NAME, Dummy, GLS (&L_OK), S_ERROR);
							All (OFF);
						}
						if (pwbuf)
						{
							FreeMem (pwbuf, buflen);
							pwbuf = 0;
						}
					}
				}
				FreeMem (bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufLen);
				xfdFreeObject (bufferinfo);
			}
			else
			{
				JEOSoundRequest (XFDM_NAME, GLS (&L_NO_MEM), GLS (&L_OK), S_ERROR);
				CleanUp ();
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
			JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 640, 37 + OFFY, fc + 1, fib_len, BLUE);

		fileCount++;
		Status (bp->FileName);
		FileStat (bp->FileName);

		do_flag = FALSE;
		pack_flag = FALSE;
		InArchiveName[0] = 0;
		if (exe = CheckExe (bp->FileName, bp->size))
		{
			do_flag = TRUE;
			exeFiles++;
		}
		else	// Data
		{
			UWORD pack;

			pack = Check_pack ((UBYTE *)FileBuffer);	// Is it an archive
			if (pack)
				archived++;	// Found!
			switch (pack)
			{
				case PACK_LHA:
				{
					if (prefs->check_LHA)
						pack_flag = TRUE;
					break;
				}
				case PACK_LZX:
				{
					if (prefs->check_LZX)
						pack_flag = TRUE;
					break;
				}
				case PACK_ZIP:
				{
					if (prefs->check_ZIP)
						pack_flag = TRUE;
					break;
				}
			}
			if (pack_flag)
				do_flag = TRUE;
			else
			{
				CrunchName[0] = 0;
				FileStat (bp->FileName);

				if (prefs->checkData)	// Check for data???
					do_flag = TRUE;
			}
		}
		pack_checking = OFF;

		if (do_flag AND bp->size)	// Vi skal sjekke ;)
		{
			ULONG check_size;

			do_flag = FALSE;
			if (Buffer = AllocMem (bp->size, MEMF_PUBLIC))
			{
			  if (fh = Open (bp->FileName, MODE_OLDFILE))
				{
	  	   	if (Read (fh, Buffer, bp->size) == bp->size);
	  	   		do_flag = TRUE;	//// ELELELELELELELSE
	  			Close (fh);
  				fh = 0;
		  	}
	  	}

			if (do_flag)
			{
				if (exe)
					check_size = StripHunkFile (Buffer, bp->size);
				else
					check_size = bp->size;

				linkflag = FALSE;
				main_check_stop = FALSE;
				stripped = FALSE;
				MainCheck (bp->FileName, Buffer, check_size);
			}
			if (Buffer)
			{
				FreeMem (Buffer, bp->size);
				Buffer = 0;
			}
		}
	  nd = nd->ln_Succ;
    bp = (Filbase *)nd;
   	fc++;
  }

  fib_SlettBase ();	// Sletter filene i dirbuffer!
	if (!aminet_flag)
	{
	  SetAPen (VirusWnd->RPort, 0);
		RectFill (VirusWnd->RPort, mainX, 35 + OFFY, mainX + 640, 37 + OFFY);
	}
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

VOID ScanDirectory (char *Dir)
{
	char Name[300];
  struct FileInfoBlock *fileinfoblock = 0;
  ULONG success;
  int i;
  BOOL go_flag;
  BOOL ret;

  Status (GLS (&L_PLEASE_WAIT));

	if (filestat)
	{
		StyleIt2 (rp, mainX, 260 + OFFY - diskShowY, mainX + 639, 286 + OFFY - diskShowY, BLACK, WHITE, GADGET_STYLE);
		StyleIt2 (rp, mainX, 290 + OFFY - diskShowY, mainX + 639, 336 + OFFY - diskShowY, BLACK, WHITE, GADGET_STYLE);
	}

  fileinfoblock = (struct FileInfoBlock *) AllocMem (sizeof (struct FileInfoBlock), MEMF_CLEAR);
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
	  		if (success = Examine (lock[cur], fileinfoblock))
 				{
		  		NameFromLock (lock[cur], Name, 300);	// Vi finner dirnavnet!
					FOREVER
					{
						if (main_stop)
							goto end;
						ret = Check_file_intuition ();
						if (ret == FALSE)
							goto end;

						success = ExNext (lock[cur], fileinfoblock);	// FindNext File/Dir

  				  if (!success)				// End of directory?
	  		  	{
  			  		UnLock (lock[cur]);
  			  		lock[cur] = DONE;
							if (!CheckDirectory ())
								goto end;
			  			break;
		   			}

		 			  if (fileinfoblock->fib_DirEntryType < 0)	// A file?
		 			  {
		 			  	LONG days_diff;
		 			  	BOOL go_flag = TRUE;

						 	if ((Name[strlen (Name) - 1] != ':') AND (Name[strlen (Name) - 1] != '/'))
						  	strcat (Name, "/");
						  sprintf (FullName, "%s%s", Name, fileinfoblock->fib_FileName);

							if (prefs->skip_days)
							{
								days_diff = stamp_now.ds_Days - fileinfoblock->fib_Date.ds_Days;
								if (days_diff > prefs->days_count)	// Skal vi skippe?
									go_flag = FALSE;	// JA!
							}

//		  	  	if (CheckExclude (fileinfoblock->fib_Comment))
//							go_flag = FALSE;

							if (go_flag)
					      fib_Insett (FullName, fileinfoblock->fib_Size, 0);
				    }
		 			  else if (fileinfoblock->fib_DirEntryType == 2)	// 3 = SOFT 4 = HARD
  				  {
  				  	char New_dir[300];

  			  		NameFromLock (lock[cur], New_dir, 300);	// Vi får aldri med '/'
	 				  	if (New_dir[strlen (New_dir) - 1] != ':')
	  				  	strcat (New_dir, "/");
	 			  		strcat (New_dir, fileinfoblock->fib_FileName);
	
							dir_depth = FindAvailDepth ();
					    if ((dir_depth == MAX_DEPTH - 1) OR dir_depth == ERROR)
					    {
								JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR65), GLS (&L_OK), S_ERROR);
								All (OFF);
								ret = FALSE;
								goto end;
							}	

  				  	if (dir_depth > max_depth)
  				  		max_depth = dir_depth;

	 		  			if (!(lock[dir_depth] = Lock (New_dir, ACCESS_READ)))
 			  			{
								sprintf (Dummy, GLS (&L_ERROR_LOCKING_DIR), New_dir);
								JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
								All (OFF);
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
	FreeMem (fileinfoblock, sizeof (struct FileInfoBlock));
}

VOID CheckFiles (UBYTE *Dir)
{
  UBYTE Hold[108] = "RAM:";
  BOOL overwrite_tmp;
  char Temp[108];
  char Date[20], Time[20];
  ULONG timeEnd;

	main_stop = FALSE;

	GetDateTime (Date, Time, 0);	// Henter tiden nå...
	stamp_now = Convert_DateTime_to_stamp (Date, Time);

	arc_depth = 0;
	autoremovelink = autodeletefile = FALSE;

	overwrite_status = FALSE;
	overwrite_tmp = prefs->overwrite_crunched_files;

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

	if (!CheckDiskState (Hold))	// Ikke writeable?
		prefs->overwrite_crunched_files = FALSE;	// Slår av dette flagget...

  len = strlen (Hold);

  dirCount = archived = archiveCount = fileCount = cFound = cFiles = virusCount = c = 0;
	exeFiles = dataFiles = 0;
//	pack_checking = OFF;
	CrunchName[0] = 0;

	sprintf (Dummy, "Started file checking from: '%s'\n", Hold);
	WriteToLog (Dummy, LOG_DATE);

	fib_Open ();
	hunkresult = AllocMem (4, MEMF_CLEAR);
	timeStart = timeEnd = 0;
	CurrentTime (&timeStart, &msecs);
	ScanDirectory (Hold);

	FreeMem (hunkresult, 4);
  fib_Close ();

	sprintf (Temp, "Ended file checking from: '%s'", Hold);
	WriteToLog (Temp, LOG_DATE);

	if (!aminet_flag)
	{
		FileStat ("");	// Get time...
		ReverseString (FileTime);
		FileTime[strlen (FileTime) - 1] = 0;
		ReverseString (FileTime);

	  Status (GLS (&L_PLEASE_WAIT));
  	sprintf (Dummy, GLS (&L_FILE_SCAN_END1),
                   dirCount, dirCount == 1 ? GLS (&L_DIR) : GLS (&L_DIRS), fileCount, fileCount == 1 ? GLS (&L_FILE) : GLS (&L_FILES),
                   exeFiles, exeFiles == 1 ? GLS (&L_FILE) : GLS (&L_FILES), cFiles,
                   dataFiles, dataFiles == 1 ? GLS (&L_FILE) : GLS (&L_FILES),
                   archiveCount, archiveCount == 1 ? GLS (&L_ARCHIVE) : GLS (&L_ARCHIVES),
                   FileTime);

	  sprintf (Hold, GLS (&L_FOUND), virusCount);
  	strcat (Dummy, Hold);
		PlaySound (S_MESSAGE);
  	rtEZRequestTags (Dummy, GLS (&L_OK) , NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT,
                   RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_FILE_CHECK_STATUS), RT_TextAttr, &topaz8, TAG_END);
  }
  else if (!virusCount)
	{
		sprintf (Dummy, "No known virus found '%s'\n", Dir);
		PF (Dummy);
	}

	if (filestat)
	{
		SetAPen (VirusWnd->RPort, 0);
		RectFill (VirusWnd->RPort, mainX, 260 + OFFY - diskShowY, mainX + 640, 356 + OFFY - diskShowY);
//		SizeWindow (VirusWnd, 0, -100);
	}

	WriteToLog (Dummy, LOG_END);
	prefs->overwrite_crunched_files = overwrite_tmp;
  Status (GLS (&L_READY));
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

  if (bp = (Filbase *)AllocMem (sizeof (Filbase), MEMF_CLEAR))
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

UWORD Check_pack (UBYTE *Buffer)
{
	if (!(strncmp (&Buffer[2], "-lh", 3)))	// LHA, LZH archive
    return (PACK_LHA);
	if (!(strncmp (Buffer, "PK", 2)))	// ZIP archive
    return (PACK_ZIP);
	if (!(strncmp (Buffer, "LZX", 3)))	// LZX archive
    return (PACK_LZX);

  return (FALSE);
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
//			sprintf (Tmp, "%ld col: %ld start: %ld\n'%s'\n", pack_checking, col, start * 8, Hold);
			JEOWrite (rp, mainX + STATUSX + 70 + (start*8), STATUSY+OFFY, Hold, col);
//			JEOSoundRequest (GLS (&L_MESSAGE), Tmp, GLS (&L_OK), 0);
			start = i + 1;
			j = 0;
			printf ("%ld col = %ld\n", pack_checking, col);
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

BYTE Do_xad (char *FileName, char *Buffer, ULONG size)
{
	LONG result;
	ULONG flags;
	struct xadArchiveInfo *ai;
  struct xadFileInfo *xfi;
  BYTE ret = FALSE;	// Not done
  BOOL go_flag;
  UWORD pack;
	BOOL exe;

	go_flag = FALSE;
	pack = Check_pack (Buffer);
	if (pack_checking)
		archived++;
	switch (pack)
	{
		case PACK_LHA:
		{
			if (prefs->check_LHA)
				go_flag = TRUE;
			break;
		}
		case PACK_LZX:
		{
			if (prefs->check_LZX)
				go_flag = TRUE;
			break;
		}
		case PACK_ZIP:
		{
			if (prefs->check_ZIP)
				go_flag = TRUE;
			break;
		}
	}

	if (go_flag)	// Er det et arkiv?
	{
		sprintf (Dummy, GLS (&L_CHECKING), FileName);
		Status (Dummy);	// Must be in the loop to get back right name...

		pack_checking = TRUE;
		if (ai = xadAllocObjectA (XADOBJ_ARCHIVEINFO, 0))
		{
			result = xadGetInfo (ai, XAD_INMEMORY, Buffer, XAD_INSIZE, size, TAG_DONE);
			flags = ai->xai_Flags;

			if (!result)	// oki? :)
			{
				ULONG size;
				char *Buf;
				char *pwbuf = 0;
				UWORD buflen;

				mystrcpy (ArcName, FileName);
				mystrcpy (Archive, ai->xai_Client->xc_ArchiverName);	// (lha)

				go_flag = TRUE;
				if (flags & XADAIF_CRYPTED)
				{
					sprintf (Dummy, GLS (&L_ARCHIVE_IS_CRYPTED), FileName);
					JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
					All (OFF);

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
				  }
				}
				if (go_flag)
				{
				  xfi = ai->xai_FileInfo;
					if (!xfi)
					{
						go_flag = FALSE;
						ret = FALSE;
					}
					else
						ret = TRUE;
				}
				if (go_flag)
				{
					BOOL flag, extract_flag;
					ULONG check_size;

					flag = FALSE;

				  while (xfi)
					{
				  	if (main_stop)
				  		ret = STOP;
				  	if (main_check_stop)
				  		ret = STOP;
						if (!Check_file_intuition ())
						{
							ret = STOP;
							main_stop = TRUE;
						}
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
							if (Buf = AllocMem (size, MEMF_ANY))
							{
								result = xadFileUnArc (ai, XAD_ENTRYNUMBER, xfi->xfi_EntryNumber, XAD_OUTMEMORY, Buf, XAD_OUTSIZE, size, TAG_END);
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
										check_size = StripHunkFile (Buf, size);
										do_flag = TRUE;
									}
									else if (pack = Check_pack (Buf))
									{
										switch (pack)
										{
											case PACK_LHA:
											{
												if (prefs->check_LHA)
													do_flag = TRUE;
												break;
											}
											case PACK_LZX:
											{
												if (prefs->check_LZX)
													do_flag = TRUE;
												break;
											}
											case PACK_ZIP:
											{
												if (prefs->check_ZIP)
													do_flag = TRUE;
												break;
											}
										}
									}
									else if (prefs->checkData)	// Check for data???
									{
										do_flag = TRUE;
										check_size = size;
									}
									if (do_flag)
									{
										linkflag = FALSE;
										main_check_stop = FALSE;
										stripped = FALSE;
										MainCheck (FileName, Buf, size);
										sprintf (Dummy, GLS (&L_CHECKING), FileName);
										Status (Dummy);	// Must be in the loop to get back right name...
									}
									if (main_stop)
										ret = STOP;
								}
								else
								{
									char *ptr;

									ptr = xadGetErrorText (result);
									ptr[0] = toupper (ptr[0]);
									if (aminet_flag)
									{
										sprintf (Dummy, "ERROR: %s! '%s'\n", ptr, FileName);
										PF (Dummy);
									}
									else
									{
										sprintf (Dummy, GLS (&L_ARCHIVE_ERROR), FileName, ArcFile, ptr);
										JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
										All (OFF);
									}
									ret = FALSE;
								}
								FreeMem (Buf, size);
								Buf = 0;
							}
						}
		  	  	xfi = xfi->xfi_Next;
		  	  }
					if (flag)
						archiveCount++;
		  	}
 				if (pwbuf)
				{
					FreeMem (pwbuf, buflen);
					pwbuf = 0;
				}
				xadFreeInfo (ai);
		  }
      xvsFreeObject(ai);
		}
	}
	return (ret);
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
