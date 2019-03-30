#include <JEO:JEO.h>
#include <proto/exec.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <proto/socket.h>
#include <proto/usergroup.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "VE_Prototypes.h"
#include <errno.h>
#include <JEO:sys/stat.h>
#include <devices/timer.h>
#include <time.h>
#include <proto/xfdmaster.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/xadmaster.h>

#define UPDATE_Y	(68 + OFFY)
#define U_HEIGHT 126

extern UBYTE last_boot;
extern char Cat_Language[];
extern char Cat_Version[];

VOID SetUp (BOOL mode);
MD5Data Calculate_MD5_from_file (char *FileName);
VOID Do_unpack (char *FileName, UBYTE mode);
VOID SavePrefs (VOID);

LONG mics = 0, secs = 0;
struct Library *SocketBase = 0;
UBYTE update_count;
ULONG xad_cpu;
UBYTE install_count;

#define MAX_LIST 6
char ReadmeDir[] = "RAM:ReadMeFiles";

char LocalDir[] = "/VE/";

BOOL MakeDir (char *DirName)
{
	BPTR lock;
	BOOL ret = FALSE;

	if (Exists (DirName))	// Allready created?
		ret = TRUE;
	else
	{
 		if (lock = CreateDir (DirName))
 		{
  		UnLock (lock);
  		ret = TRUE;
  	}
	}
	return (ret);
}

struct
{
	char Name[30],
			 FileName[30];

} UpdateList[MAX_LIST] =
  {
    "VirusExecutor", "VirusExecutor.lha",
    "Lokale", "Catalog_norsk",
    "PatchBrain", "PatchBrain.lha",
    "xvs.library", "xvslibrary.lha",
    "xfdmaster.library", "xfdmaster.lha",
    "xadmaster.library", "xadmaster000.lha"
};

BOOL update_status[MAX_LIST];

enum
{
	U_VIRUSEXECUTOR = 0,
  U_CATALOG,
  U_PATCHBRAIN,
  U_XVS,
  U_XFD,
  U_XAD
};

//#include <amitcp/socketbasetags.h>

VOID URL_error (char *String)
{
	JEOSoundRequest (GLS (&L_MESSAGE), String, GLS (&L_OK), S_ERROR);
	All (OFF);
}

ULONG GetTime (VOID)
{
  struct timerequest tr;
  struct MsgPort *tport = 0;

  mics = secs = 0;
  if ((OpenDevice (TIMERNAME, UNIT_MICROHZ, (struct IORequest *)&tr, 0)))
    return (FALSE);
  if (!(tport = (struct MsgPort *)CreatePort (0, 0)))
  {
    CloseDevice ((struct IORequest *)&tr);
    return (FALSE);
  }
  tr.tr_node.io_Message.mn_Node.ln_Type	= NT_MESSAGE;
  tr.tr_node.io_Message.mn_Node.ln_Pri	= 20;
  tr.tr_node.io_Message.mn_Node.ln_Name	= NULL;
  tr.tr_node.io_Message.mn_ReplyPort	= tport;
  tr.tr_node.io_Command			= TR_GETSYSTIME;
  DoIO ((struct IORequest *)&tr);
  mics = tr.tr_time.tv_micro;
  secs = tr.tr_time.tv_secs;

  DeletePort (tport);
  CloseDevice ((struct IORequest *)&tr);
}

char * strchr1(char * string, char c)
{
  while (*string)
    if (*string == c)
      return string;
    else
      string++;

  return NULL;
}

char * strstr1(char * string, char * s)
{
  while (*string)
    if (strchr(s, *string) != NULL)
      return string;
    else
      string++;
  
  return NULL;
}

int getSockAddr (const char *host, int port, struct sockaddr_in * addr)
{
  struct hostent *remote;

  if ((remote = gethostbyname(host)) != NULL)
    memcpy(&addr->sin_addr, remote->h_addr, sizeof addr->sin_addr);
  else if ((addr->sin_addr.s_addr = inet_addr(host)) == (unsigned long)-1)
    return FALSE;

  addr->sin_port = htons(port);
  addr->sin_family = AF_INET;

  return TRUE;
}

const int ONE = 1;

int inetconn (const struct sockaddr_in * addr)
{
  int sd;

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
  {
		URL_error (GLS (&L_SOCKET_ERROR));
    return -1;
  }
  
  setsockopt (sd, SOL_SOCKET, SO_KEEPALIVE, &ONE, sizeof ONE);
    
  if (connect(sd, (struct sockaddr *)addr, sizeof (struct sockaddr_in)) < 0)
	{
    CloseSocket (sd);
		URL_error (GLS (&L_CONNECTION_REFUSED));
    return -1;
  }

  return sd;
}

static int do_connect (char *host, int port)
{
  struct sockaddr_in addr = { 0 };
  
  if (getSockAddr (host, port, &addr) == FALSE)
	{
		URL_error (GLS (&L_UNKNOWN_HOST));
    return -1;
  }

  return inetconn (&addr);
}

char ValidMD5[33];

BOOL CheckMD5 (char *MD5, char *FileName)
{
	MD5Data md5;

	md5 = Calculate_MD5_from_file (FileName);

//	printf ("%s:\n$%s - $%s\n\n", FileName, ValidMD5, md5.md5);

	if (!(stricmp (md5.md5, MD5)))
		return (TRUE);

	return (FALSE);
}

/*
WORD CheckVersionString (char *CurVersion, char *NewVersion)
{
	WORD check, i, j;
	UWORD old_ver, new_ver, old_rev, new_rev;
	BOOL flag;

	old_ver = atoi (CurVersion);
	new_ver = atoi (NewVersion);

	flag = FALSE;
	for (i = 0, j = 0; CurVersion[i] != 0; i++)
	{
		if (flag)
		{
			Dummy[j] = CurVersion[i];
			j++;
		}
		if (CurVersion[i] == '.')
			flag = TRUE;
	}
	Dummy[j] = 0;
	old_rev = atoi (Dummy);

	flag = FALSE;
	for (i = 0, j = 0; NewVersion[i] != 0; i++)
	{
		if (flag)
		{
			Dummy[j] = NewVersion[i];
			j++;
		}
		if (NewVersion[i] == '.')
			flag = TRUE;
	}
	Dummy[j] = 0;
	new_rev = atoi (Dummy);

	if (old_ver == new_ver)	// like versjoner
	{
		if (old_rev < new_rev)
			check = -1;
		else if (old_rev == new_rev)
			check = 0;
		else
			check = 1;
	}
	else
	{
		if (old_ver < new_ver)
			check  = -1;
		else if (old_ver > new_ver)
			check  = 1;
	}
//	printf ("%ld OLD: %ld.%ld NEW: %ld.%ld\n", check, old_ver, old_rev, new_ver, new_rev);
	return (check);
}

*/

VOID PrintStatus (UBYTE nr, char *String)
{
	UWORD y = UPDATE_Y + (nr * 9);
	char S[70];

	JEOCopy (S, String, 34, ' ');
	JEOWrite (rp, mainX + 350, y, S, BLACK);
}

ULONG new_dl_size;
char AminetVersion[30];

BYTE FindVersion (char *Name, char *Buf, char *CurVersion, UBYTE nr, char *url)
{
	int i, j = 0;
	char Version[20];
	char Size[20];
	UBYTE flag = 0;
	UWORD y = UPDATE_Y + (nr * 9);

	new_dl_size = 0;
	AminetVersion[0] = 0;

	for (i = 0; Buf[i] != 0; i++)
	{
		if (flag == 0)	// Size
		{
			Size[i] = Buf[i];
			if (Buf[i] == '\n')
			{
				Size[i] = 0;
				flag = 1;
			}
		}
		else if (flag == 1) // Version
		{
			Version[j] = Buf[i];

			if (Buf[i] == '\n')
			{
				Version[j] = 0;
				flag = 2;
				j = 0;
			}
			else
				j++;
		}
		else if (flag == 2) // MD5
		{
			ValidMD5[j] = Buf[i];
			j++;
			if (j > 32)
			{
				ValidMD5[--j] = 0;
				break;
			}
		}
	}

	mystrcpy (AminetVersion, Version);
	new_dl_size = atoi (Size);

	SPrintF (Dummy, GLS (&L_CHECKING_FOR_UPDATES_1), Name);
	Status (Dummy);
	SPrintF (Dummy, "%5s", AminetVersion);
	JEOWrite (rp, mainX + 245, y, Dummy, BLACK);

	if (stricmp (CurVersion, AminetVersion) < 0)	// CheckVersion
	{
		BOOL go_flag = TRUE;

		switch (nr)	
		{
			case U_XFD: // xfd
			{
				if (!(strcmp (ValidMD5, prefs->xfdUpdateMD5)))	// Same as last update??
					go_flag = FALSE;
				else
					strcpy (prefs->xfdUpdateMD5, ValidMD5);

				break;
			}
			case U_XAD: // xad
			{
				if (!(strcmp (ValidMD5, prefs->xadUpdateMD5)))	// Same as last update??
					go_flag = FALSE;
				else
					strcpy (prefs->xadUpdateMD5, ValidMD5);

				break;
			}
		}

		if (go_flag)
		{
			Status (GLS (&L_CONNECTING_TO_SERVER));
			return (TRUE);
		}
		else
			return (ERROR); // MD5 check..
	}
	return (FALSE);
}


int httpget (char *Name, char *u, char *CurVersion, UBYTE nr, BOOL mode)
{
  char buf[1024];
  int i, port, sd;
	fd_set rd;
	BYTE ret = FALSE;
	char Url[] = "superhebbe.com";

	if (mode == 0)
	{
		SPrintF (Dummy, GLS (&L_CHECKING_FOR_UPDATES_1), Name);
		Status (Dummy);
	}

  port = 80;	/* standard http -service port number */

  if ((sd = do_connect(Url, port)) < 0)
  {
		PrintStatus (nr, GLS (&L_ERROR_CONNECTING_TO_SERVER));
    return ERROR;
  }


	FD_ZERO (&rd);
	FD_SET(sd, &rd);

	i = strlen (u);

  if ((send(sd, "GET ", 4, 0) != 4) || (send(sd, u, i, 0) != i))
	{
    URL_error (GLS (&L_ERROR_SENDING));
		PrintStatus (nr, GLS (&L_ERROR_TALKING_TO_SERVER));
    return ERROR;
  }

	if (mode == 0)	// Just checking :)
	{
		BYTE ret;
		UWORD y = UPDATE_Y + (nr * 9);

		JEOWrite (rp, mainX + 10, y, UpdateList[nr].Name, BLACK);
		PrintStatus (nr, GLS (&L_CHECKING_VERSION));

		i = recv (sd, buf, sizeof buf, 0);
		buf[i] = 0;


//	  Write(Output(), buf, i);


		if (!atoi (buf))	// Should be a number... Filesize
		{
			PrintStatus (nr, GLS (&L_ERROR_READING_INFO_DATA));

			if (nr == U_CATALOG)
			{
				if (!(stricmp (Cat_Language, "english")))
					PrintStatus (nr, "Using internal catalog...");
			}
			return (FALSE);
		}
	  if (i < 5)
	  {
			PrintStatus (nr, GLS (&L_INFO_DATA_IS_CURRUPT));
	    return (FALSE);
	  }
		ret = FindVersion (Name, buf, CurVersion, nr, Url);

		switch (ret)
		{
			case FALSE:	PrintStatus (nr, GLS (&L_NO_NEW_VERSION)); break;
			case TRUE: PrintStatus (nr, GLS (&L_NEW_VERSION_FOUND)); break;
			case ERROR: PrintStatus (nr, GLS (&L_NO_NEW_MD5)); ret = FALSE; break;
		}

		return (ret);
	}  
	else	// DL :)
	{
		char FileName[51];
		ULONG cps;
		BPTR fh;
		int len = 0, sec, mic, sec1, mic1;
		DOUBLE time = 0;
		ULONG value, pst;

		ret = TRUE;
		sprintf (Dummy, GLS (&L_DOWNLOADING_PST) , 0);
		PrintStatus (nr, Dummy);

		SPrintF (FileName, "T:%s", UpdateList[nr].FileName);
		if (fh = Open (FileName, MODE_NEWFILE))
		{
			ULONG k;

			BOOL checkflag = TRUE;
	  	GetTime ();
		  sec = sec1 = secs;
  		mic = mic1 = mics;
			FOREVER
			{
				for (k = 0; k < 500000; k++);
				i = recv (sd, buf, 1024, 0);
				if (i > 0)
				{
					if (checkflag)
					{
						if (!(strncmp (buf, "<!", 2)))
						{
							PrintStatus (nr, GLS (&L_COULDNT_FIND_FILE_ON_SERVER));
							ret = ERROR;
							break;
						}
						checkflag = FALSE;
					}
			  	Write (fh, buf, i);
				  GetTime ();
				  sec = secs - sec1;
  				mic = mics - mic1;
					if (mic < 0)
					{
						mic += 1000000;
						sec--;
					}

					len += i;
		
				  SPrintF (Dummy, "%ld.%06ld\n", sec, mic);
					time = atof (Dummy);
					cps = (ULONG)(len / time);

					value = 10000000 / new_dl_size;
					pst = value * len / 100000;
					SPrintF (Dummy, GLS (&L_DOWNLOADING_CPS), UpdateList[nr].FileName, pst, cps);
		  		Status (Dummy);
					sprintf (Dummy, GLS (&L_DOWNLOADING_PST), pst);
					PrintStatus (nr, Dummy);

					JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, pst + 1, 100, BLUE);
			  }
			  else
			   break;
			}
			Close (fh);
			if (ret != ERROR)
			{
				SPrintF (Dummy, GLS (&L_DOWNLOADING_CPS) , UpdateList[nr].FileName, 100, cps);
  			Status (Dummy);
				JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, 100, 100, BLUE);
				PrintStatus (nr, GLS (&L_DOWNLOAD_COMPLETE));
				Delay (10);
				if (CheckMD5 (ValidMD5, FileName))
					update_count++;
				else
				{
					update_status[i] = FALSE;
					PrintStatus (nr, GLS (&L_CHECKSUM_ERROR));
					ret = FALSE;
				}
			}
		}
		if (pst)
			ClearBar ();
	}
	return (ret);
}

// ***************************************************************

int VE_Update (char *Name, char *u, char *CurVersion, UBYTE nr, BOOL mode)
{
	BYTE rv;

	rv = httpget (Name, u, CurVersion, nr, mode);
	return (rv);
}

BOOL CheckXadFree (char *Name)
{
	LONG size;
	BOOL ret = FALSE;
	char *Buf;
	BPTR fh;

	size = FileSize (Name);
	if (size > 0)
	{
		if (Buf = AllocMem (size, MEMF_CLEAR))
		{
			if (fh = Open (Name, MODE_OLDFILE))
			{
				ULONG i;

				Read (fh, Buf, size);
				Buf[size-1] = 0;
				Close (fh);

				for (i = 0; i < size - 2; i++)
				{
					if (Buf[i] == '\n')
					{
						if (Buf[i+1] == 'A')	// AmigaOS 3.9 special version
							ret = TRUE;
						break;
					}
				}
			}
			FreeMem (Buf, size);
		}
	}
	return ret;
}


VOID Update_files (VOID)
{
	UWORD y;
	UBYTE i;
	static BOOL strcat_flag = FALSE;

	if (!xadmaster_flag)
	{
		SPrintF (Dummy, GLS (&L_PLEASE_INSTALL), UpdateList[5].Name);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);

		return;
	}

	install_count = 0;
	for (y = 0; y < MAX_LIST; y++)
		update_status[y] = 0;

	update_count = 0;
	if ((SocketBase = OpenLibrary ("bsdsocket.library", 4)) != NULL)
	{
		UBYTE val;
		WORD ret = FALSE;
		char u[100];

		All (OFF);
		val = 1;
		if (val)
		{
			char Version[20];

			SetAPen (VirusWnd->RPort, 0);
			RectFill (VirusWnd->RPort, mainX+3, UPDATE_Y - 28, mainX + 628, UPDATE_Y + U_HEIGHT);
			StyleIt2 (rp, mainX+3, UPDATE_Y - 28, mainX + 628, UPDATE_Y - 14, BLACK, WHITE, GADGET_STYLE);
			StyleIt2 (rp, mainX+3, UPDATE_Y - 10, mainX + 628, UPDATE_Y + U_HEIGHT, BLACK, WHITE, GADGET_STYLE);

			JEOWrite (rp, mainX +  10, UPDATE_Y - 18, GLS (&L_UP_NAME), WHITE);
			JEOWrite (rp, mainX + 165, UPDATE_Y - 18, GLS (&L_OLD), WHITE);
			JEOWrite (rp, mainX + 245, UPDATE_Y - 18, GLS (&L_NEW), WHITE);
			JEOWrite (rp, mainX + 350, UPDATE_Y - 18, GLS (&L_STATUS), WHITE);
			y = UPDATE_Y;
			for (i = 0; i < MAX_LIST; i++)
			{
				if (i == U_CATALOG)	// Catalog
				{
					SPrintF (UpdateList[i].Name, "Locale (%s)", Cat_Language);
					SPrintF (UpdateList[i].FileName, "Catalog_%s.lha", Cat_Language);
				}
				JEOWrite (rp, mainX + 10, y, UpdateList[i].Name, BLUE);
				switch (i)
				{
					case 0:	mystrcpy (Version, ProgramVersion); break;
					case 1:	mystrcpy (Version, Cat_Version); break;
					case 2:	mystrcpy (Version, patch_Version); break;
					case 3:	mystrcpy (Version, xvs_Version); break;
					case 4:	mystrcpy (Version, xfd_Version); break;
					case 5:	mystrcpy (Version, xad_Version); break;
				}
				SPrintF (Dummy, "%5s", Version);
				JEOWrite (rp, mainX + 165, y, Dummy, BLACK);
				JEOWrite (rp, mainX + 245, y, "-----", BLUE);

				y += 9;
			}

			for (i = 0; i < MAX_LIST; i++)
			{
				switch (i)
				{
					case 0:	mystrcpy (Version, ProgramVersion); break;
					case 1:	mystrcpy (Version, Cat_Version); break;
					case 2:	mystrcpy (Version, patch_Version); break;
					case 3:	mystrcpy (Version, xvs_Version); break;
					case 4:	mystrcpy (Version, xfd_Version); break;
					case 5:
					{
						mystrcpy (Version, xad_Version);
						if (!strcat_flag)
						{
							xad_cpu = 0;

							if (processorFlag & AFF_68060)
								xad_cpu = 60;
							else if (processorFlag & AFF_68040)
								xad_cpu = 20;
							else if (processorFlag & AFF_68030)
								xad_cpu = 20;
							else if (processorFlag & AFF_68020)
								xad_cpu = 20;
							SPrintF (UpdateList[i].FileName, "xadmaster%03ld.lha", xad_cpu);

							strcat_flag = TRUE;
						}
						break;
					}
				}
//	mystrcpy (Version, "1.00");
				
				if (i == U_CATALOG)	// Catalogs
					SPrintF (u, "%s-Catalog_%s\n", LocalDir, Cat_Language);
				else if (i == U_XAD)	// xadmaster.library
					SPrintF (u, "%s-%s%03ld.library\n", LocalDir, "xadmaster", xad_cpu);
				else
					SPrintF (u, "%s-%s\n", LocalDir, UpdateList[i].Name);
				ret = VE_Update (UpdateList[i].Name, u, Version, i, 0); // Get version!
				if (ret == TRUE)
				{
					SPrintF (u, "%s%s\n", LocalDir, UpdateList[i].FileName);
					ret = VE_Update (UpdateList[i].Name, u, "", i, 1);
					if (ret)
						update_status[i] = TRUE;
				}
				else if (ret == ERROR)
					break;
			}

			if (update_count)
			{
				SPrintF (Dummy, GLS (&L_FOUND_UPDATES) , update_count, update_count == 1 ? GLS (&L_UP_UPDATE) : GLS (&L_UP_UPDATES));
				Status (Dummy);
				strcat (Dummy, "\n\n");

				for (i = 0; i < MAX_LIST; i++)
				{
					if (update_status[i])	// Updated?
					{
						strcat (Dummy, UpdateList[i].Name);
						if (i == U_XAD)	// xadmaster.library
						{
							char Hold[10];

							sprintf (Hold, " (%03ld)", xad_cpu);
							strcat (Dummy, Hold);
						}
						strcat (Dummy, "\n");
					}
				}
				Dummy[strlen (Dummy)-1] = 0;
				
				if (JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_INSTALL_CANCEL), S_MESSAGE))
				{
					All (OFF);
					MakeDir (ReadmeDir);
					for (i = 0; i < MAX_LIST; i++)
					{
						if (update_status[i])	// Updated?
						{
							BOOL go_flag = TRUE;

							if (i == U_XAD)	// xadmaster.library
							{
								BPTR fh;
								char OutPutTxt[] = "T:xadversion";

								if (fh = Open (OutPutTxt, MODE_NEWFILE))
								{
									sprintf (Dummy, "Version FILE FULL %s", "LIBS:xadmaster.library");
									Execute (Dummy, NULL, fh);
									Close (fh);
									if (CheckXadFree (OutPutTxt))
									{
										SPrintF (Dummy, GLS (&L_YOU_HAVE_A_SPECIAL), UpdateList[i].Name);
										if (!JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_INSTALL_CANCEL), S_MESSAGE))
											go_flag = FALSE;
									}
									DeleteFile (OutPutTxt);
								}
							}

							if (go_flag)
							{
								PrintStatus (i, GLS (&L_INSTALLING_1));
							
								SPrintF (Dummy, "T:%s", UpdateList[i].FileName);
								Do_unpack (Dummy, i);
								PrintStatus (i, GLS (&L_FINISHED) );
								if (i == U_XFD OR i == U_XAD)	// xfd/xad -master.library
									SavePrefs ();
							}
							else
								PrintStatus (i, GLS (&L_ABORTED));
						}
					}
					if (install_count)
					{
						restart_flag = TRUE;
						SPrintF (Dummy, GLS (&L_README_FILES_ARE_LOCATED), ReadmeDir); 
						JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
					}
				}
			}
			else
				JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_YOUR_FILES_ARE_UP_TO_DATE), GLS (&L_OK), S_MESSAGE);

			SetAPen (VirusWnd->RPort, 0);
			RectFill (VirusWnd->RPort, mainX+3, UPDATE_Y - 28, mainX + 628, UPDATE_Y + U_HEIGHT);
			SetUp (0);
		}
		CloseLibrary (SocketBase);
	}
	else
		URL_error (GLS (&L_NO_TCP));

	for (i = 0; i < MAX_LIST; i++)
	{
		if (update_status[i])	// Updated?
		{
			SPrintF (Dummy, "T:%s", UpdateList[i].FileName);
			DeleteFile (Dummy);
		}
	}

	All (ON);
	if (restart_flag)
		CleanUp ();
}


VOID Do_unpack (char *FileName, UBYTE nr)
{
	LONG result;
	ULONG flags;
	struct xadArchiveInfo *ai;
  LONG size;
  char *Buffer;
  BPTR fh;
  BOOL first;

	size = FileSize (FileName);
	if (size > 0)
	{
		if (Buffer = AllocMem (size, MEMF_PUBLIC))
		{
			if (fh = Open (FileName, MODE_OLDFILE))
			{
				Read (fh, Buffer, size);
				Close (fh);

				if (!(strncmp (&Buffer[2], "-lh", 3)))	// LHA, LZH archive
				{
					if (ai = xadAllocObjectA (XADOBJ_ARCHIVEINFO, 0))
					{
						result = xadGetInfo (ai, XAD_INMEMORY, Buffer, XAD_INSIZE, size, TAG_DONE);
						if (!result)
						{
							flags = ai->xai_Flags;
	
							if (ai->xai_FileInfo)
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
								first = TRUE;
								install_count++;
							  while (xfi)
								{
									size = xfi->xfi_Size;
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
										char ToFileName[300];

										if (Buf = AllocMem (size, MEMF_ANY))
										{
											result = xadFileUnArc (ai, XAD_ENTRYNUMBER, xfi->xfi_EntryNumber, XAD_OUTMEMORY, Buf, XAD_OUTSIZE, size, TAG_END);
											if (!result)
											{
												BPTR fh;

												switch (nr)
												{
													case U_VIRUSEXECUTOR: // VirusExecutor
													{
														if (!(stricmp (xfi->xfi_FileName, "VirusExecutor.readme")))
															SPrintF (ToFileName, "%s/%s", ReadmeDir, xfi->xfi_FileName);
														else
															SPrintF (ToFileName, "%s%s", VEAssign, xfi->xfi_FileName);
														
														SPrintF (Dummy, "", xfi->xfi_FileName);
														Status (Dummy);
														if (fh = Open (ToFileName, MODE_NEWFILE))
														{
															Write (fh, Buf, size);
															Close (fh);
														}
														break;
													}
													case U_CATALOG: // Catalogs
													{
														SPrintF (ToFileName, "LOCALE:Catalogs/%s/%s", Cat_Language, xfi->xfi_FileName);

														SPrintF (Dummy, GLS (&L_INSTALLING_2), xfi->xfi_FileName);
														Status (Dummy);
														if (fh = Open (ToFileName, MODE_NEWFILE))
														{
															Write (fh, Buf, size);
															Close (fh);
														}
														break;
													}
													case U_PATCHBRAIN: // PatchBrain
													{
														if (!(stricmp (xfi->xfi_FileName, "VEPatchBrain.readme")))
															SPrintF (ToFileName, "%s/%s", ReadmeDir, xfi->xfi_FileName);
														else
															SPrintF (ToFileName, "%s%s", VEAssign, xfi->xfi_FileName);

														SPrintF (Dummy, GLS (&L_INSTALLING_2), xfi->xfi_FileName);
														Status (Dummy);
														if (fh = Open (ToFileName, MODE_NEWFILE))
														{
															Write (fh, Buf, size);
															Close (fh);
														}
														break;
													}
													case U_XVS: // xvs.library
													{
														if (!(stricmp (xfi->xfi_FileName, "xvslibrary.readme")))
															SPrintF (ToFileName, "%s/%s", ReadmeDir, xfi->xfi_FileName);
														else
															SPrintF (ToFileName, "LIBS:%s", xfi->xfi_FileName);

														SPrintF (Dummy, GLS (&L_INSTALLING_2) , xfi->xfi_FileName);
														Status (Dummy);
														if (fh = Open (ToFileName, MODE_NEWFILE))
														{
															Write (fh, Buf, size);
															Close (fh);
														}
														break;
													}
													case U_XFD: // xfdmaster.library
													{
														char DirName[] = "LIBS:xfd";

														if (first)	// Do LIBS:xfd/
														{
															if (Exists (DirName))
																Execute ("Delete LIBS:xfd/ ALL FORCE QUIET >NIL:", NULL, NULL);

														  MakeDir (DirName);
															first = FALSE;
														}
														if (!(stricmp (xfi->xfi_FileName, "xfdmaster.readme")))
															SPrintF (ToFileName, "%s/%s", ReadmeDir, xfi->xfi_FileName);
														else
															SPrintF (ToFileName, "LIBS:%s", xfi->xfi_FileName);
	
														SPrintF (Dummy, GLS (&L_INSTALLING_2), xfi->xfi_FileName);
														Status (Dummy);
														if (fh = Open (ToFileName, MODE_NEWFILE))
														{
															Write (fh, Buf, size);
															Close (fh);
														}
														break;
													}
													case U_XAD: // xadmaster.library
													{
														char DirName[] = "LIBS:xad";

														if (first)	// Do LIBS:xad/
														{
															if (Exists (DirName))
																Execute ("Delete LIBS:xad/ ALL FORCE QUIET >NIL:", NULL, NULL);

														  MakeDir (DirName);
															first = FALSE;
														}
														SPrintF (Dummy, "xadmaster%03ld.readme", xad_cpu);
														if (!(stricmp (xfi->xfi_FileName, Dummy)))
															SPrintF (ToFileName, "%s/%s", ReadmeDir, xfi->xfi_FileName);
														else
															SPrintF (ToFileName, "LIBS:%s", xfi->xfi_FileName);
	
														SPrintF (Dummy, GLS (&L_INSTALLING_2), xfi->xfi_FileName);
														Status (Dummy);
														if (fh = Open (ToFileName, MODE_NEWFILE))
														{
															Write (fh, Buf, size);
															Close (fh);
														}
														break;
													}
												}
											}
											FreeMem (Buf, size);
										}
									}
							  	xfi = xfi->xfi_Next;
							  }
							}
							xadFreeInfo (ai);
			      }
			      xvsFreeObject(ai);
					}
				}
			}
		}
	}
}
