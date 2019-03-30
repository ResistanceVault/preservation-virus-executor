char CopyRight[] = "Copyright © 1992-2004 Jan Erik Olausen";

#define _DEBUG1 0

#define _DEBUG 0
#define _JEO_DEBUG 0

#define MINSTACKSIZE            20000

#define Last_XVS "33.42"
#define Last_XFD "39.15"
#define Last_XAD "12.1"

#define PREFS_VERSION	13	// This is all we need to update! :)

/*
#ifdef _M68060
#define CPU " [060]"
#else
#ifdef _M68040
#define CPU " [040]"
#else
#ifdef _M68020
#define CPU " [020]"
#else
#define CPU ""
#endif
#endif
#endif
*/

#include <JEO.h>
#include <ctype.h>
#include <pragmas/mmu_pragmas.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <devices/trackdisk.h>
#include <devices/timer.h>
#include <proto/locale.h>
#include <proto/icon.h>
#include <proto/diskfont.h>
#include <proto/wb.h>
#include <libraries/locale.h>
#include <libraries/reqtools.h>
#include <libraries/asl.h>
#include <proto/reqtools.h>
#include "VE_Prototypes.h"
#include "VE_PatchOffsets.h"
#include <JEO:raw.h>
#include "Status.h"
#include "VE_Window.h"
#include <proto/xfdmaster.h>
#include <proto/xadmaster.h>
#include <proto/xvs.h>
#include <exec/resident.h>
#include <devices/audio.h>
#define LOCALE_TEXT
#include "VE_locale.h"

extern ULONG mem_start, mem_end;
extern ULONG exec_check;
extern BOOL filecheck_progress;
BOOL ValidMem (ULONG *ptr);

VOID FindMemoryAera (ULONG address, char *MemName);

char VE[14] = "VirusExecutor";
char VEAssign[300];

char AminetFileName[256];
BOOL aminet_flag;
BOOL aminet_mode;
BOOL restart_flag;
UWORD diskY[4];

struct TextFont *textfont = 0;
struct MsgPort *ve_port = 0;

VOID SetUp (BOOL mode);
VOID RemoveFW (VOID);
VOID InitInstallWindow (VOID);
VOID InitRecordWindow (VOID);
BOOL OpenIcon (VOID);
VOID CloseIcon (VOID);
VOID MainCheck (char *FileName, char *Buffer, ULONG size);

char VERealName[106];

BOOL wb_flag;
BOOL morphOS_flag;

ULONG asl_version, reqtools_version;

char DataName[300];
char Tmp1[20], Tmp2[20], Tmp3[20], Tmp4[20];

UWORD diskShowY;

char Cat_Language[51];
char Cat_Version[10];
char xvs_Version[10];
char patch_Version[10];
char xfd_Version[10] = "N/A";
char xad_Version[10] = "N/A";

BOOL xfdmaster_flag;
BOOL xadmaster_flag;

VOID SelectScreenMode (BOOL mode);

ULONG winsig;
ULONG iconsig;

long __stack = 20000;

/*
long _BackGroundIO;
long __priority = 0;
long __stack = 20000;
char *__procname = VE;
*/

char xvs_name[] = XVS_NAME;
char xfdmaster_name[] = XFDM_NAME;
char xadmaster_name[] = XADNAME;

struct LocaleBase *localeBase = NULL;
struct Catalog *mycatalog = NULL;

struct DiskObject *dobj;
struct MsgPort *iconport;
struct AppIcon *dcicon;
BOOL icon;			// iconified

UBYTE last_boot;

char *GLS (struct LocText *loctext)
{
  if (mycatalog)
  {
    if (!(GetCatalogStr (mycatalog, loctext->id, NULL)))
			return (loctext->text);
  }
	else
		return (loctext->text);
}

Vectors *vec = 0, *tmp_vec = 0;

char TDErrorMsg[50];

#define STATUSX		    5
#define STATUSY		   26
#define DISKX		 		 10
#define DISKY		 		210

#define OUT		0
#define IN		1

#define NEW		1
#define OLD		0

#define BUFSIZE		1024
#define NOVIRUS			 0
#define NODOS				 2

#define OTHER 		2
#define VIRUS			3

UBYTE memcheck_delay = 3;
BOOL counter = OFF;

Prefs *prefs = 0;

extern BOOL installFlag[];
extern UBYTE DummyName[];
extern UBYTE DummyBuf[];

BYTE *SoundBuffer = 0;
#define SOUND_LEN 32908
BOOL soundplay;

struct xvsVirusList *viruslist = 0;

// struct Task *tc = 0;

struct timerequest *TimerIO = 0;
struct MsgPort *TimerMP = 0;
struct Message *TimerMSG = 0;
LONG tdevice = 1;

LONG old_checksum;
WORD view_unit;

UWORD ROM_version, ROM_revision;
UWORD hw_ROM_version, hw_ROM_revision;
char HW_OSVer[10], OSVer[10];
ULONG ROM_start, ROM_end, ROM_size;
ULONG hw_ROM_start, hw_ROM_end, hw_ROM_size;
ULONG exec_start, exec_end, exec_size;
MD5Data exec_md5;

BOOL soft_ROM_flag;

char ROMVersion[20];
char HW_ROMVersion[20];

UWORD total_unpackers;

struct Library *xvsBase = 0;
extern struct ExecBase *SysBase;
struct xfdMasterBase *xfdMasterBase	= 0;
struct xadMasterBase *xadMasterBase	= 0;

UBYTE *Dummy = 0;

struct xvsBootInfo *bootinfo = 0;
struct xvsSectorInfo *sectorinfo = 0;

BOOL crunchFlag = OFF;
BOOL menuFlag;
UBYTE Ver[100];
UWORD screenWidth, screenHeight;
UWORD mainX;
UWORD processorFlag;

extern UBYTE *BootVirus[];
extern ULONG recCount;

struct Library *GadToolsBase = 0;
struct Library *UtilityBase = 0;
struct RastPort *rp;
struct ReqToolsBase *ReqToolsBase = 0;
struct AslBase *AslBase = 0;
struct MsgPort *diskport = 0;
struct IOExtTD *diskreq = 0;

LONG drive[4];
BOOL flagleft = FALSE;
BOOL flagright = FALSE;
UBYTE *Buffer[4];
UWORD viewCol[4];
UBYTE *Drive[] = { "DF0:", "DF1:", "DF2:", "DF3:" };
char DiskText[4][72];
UWORD knownBoot[4];

struct Process *pr;
APTR oldwinptr;

VOID SavePrefs (VOID);
VOID LoadPrefs (VOID);

 UBYTE TDError[][44] =
{
	"General catchall!",								// TDERR_NotSpecified
	"Couldn't even find a sector!",			// TDERR_NoSecHdr
	"Sector looked wrong!",							// TDERR_BadSecPreamble
	"Sector looked wrong!",							// TDERR_BadSecID		
	"Header had incorrect checksum!",		// TDERR_BadHdrSum		
	"Data had incorrect checksum!",			// TDERR_BadSecSum		
	"Couldn't find enough sectors!",		// TDERR_TooFewSecs	
	"Another sector looked wrong!",			// TDERR_BadSecHdr		
	"Can't write to a protected disk!",	// TDERR_WriteProt		
	"No disk in the drive!",						// TDERR_DiskChanged	
	"Couldn't find track 0!",						// TDERR_SeekError		
	"Ran out of memory!",								// TDERR_NoMem
	"Asked for a unit > NUMUNITS!",			// TDERR_BadUnitNum	
	"Not a drive that trackdisk groks!",	// TDERR_BadDriveType
	"Someone else allocated the drive!",	// TDERR_DriveInUse	
	"User hit reset; awaiting doom!"			// TDERR_PostReset		
};

#define TDStart 20
#define TDEnd		35

struct IOAudio *AIOptr;
struct MsgPort *port;
ULONG  systemClock;

static VOID PlayBuffer (BYTE *Buffer, ULONG len, UWORD hz)
{
	struct Message *msg;

	if (GetMsg (port) == 0)
	{
		AbortIO ((struct IORequest *)AIOptr);
		msg = GetMsg (port);
	}

	AIOptr->ioa_Length = len;
	AIOptr->ioa_Data   = Buffer;
	AIOptr->ioa_Period = (UWORD)(PAL_CLOCK_CONSTANT / hz);	

	BeginIO((struct IORequest *)AIOptr);	// start playing sample
}

VOID PlaySound (BYTE sound)
{
	if (sound == S_ERROR)
		ScreenToFront (Scr);

	if (soundplay)
	{
		switch (sound)
		{
			case S_ERROR: PlayBuffer (SoundBuffer, 5698, 12559); break;
			case S_MESSAGE: PlayBuffer (&SoundBuffer[5700], 6638, 10405); break;
			case S_VIRUS: PlayBuffer (&SoundBuffer[12340], 20568, 22095); break;
		}
	}
}

ULONG JEOSoundRequest (char *Title, char *TextFormat, char *GadgetFormat, BYTE sound)
{
	UWORD ret = 0;

	if (aminet_flag)
		return (0);
	else
	{
		PlaySound (sound);
	  ret = rtEZRequestTags (TextFormat, GadgetFormat, NULL, NULL, RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, Title, RT_WaitPointer, TRUE, RT_TextAttr, &topaz8, TAG_END);
	  if (filecheck_progress)
	  	All (OFF);
	  return (ret);
	}
}

/*
VOID Test (VOID)
{
	static ULONG i = 0;
	UBYTE Yo[50];
	
	FOREVER
	{
		i++;
		ObtainSemaphore (&MainSemaphore);
		SPrintF (Yo, "Dette er %ld", i);
		JEOWrite (rp, 20, 140 + OFFY, Yo, 1);
		ReleaseSemaphore (&MainSemaphore);
	}
	Wait (0);
}
*/

/*
VOID JEOOut (UBYTE *String)
{
	Write (Output (), String, strlen (String));
}
*/

BOOL AllocAudio (BOOL mode)
{
	int c;
	static ULONG device = 1L;
	static UBYTE chan1[]  = { 1 };		/* Audio channel allocation arrays */
	static UBYTE chan2[]  = { 2 };
	static UBYTE chan3[]  = { 4 };
	static UBYTE chan4[]  = { 8 };
	static UBYTE *chans[] = {chan1, chan2, chan3, chan4};
	
	if (mode == TRUE)
	{
		/* ALLOCATE AUDIO HARDWARE */

		if (!(AIOptr = (struct IOAudio *) AllocMem(sizeof(struct IOAudio), MEMF_PUBLIC | MEMF_CLEAR)))
			 return(FALSE);

		/* Create our audio ports */
		if (!(port = CreatePort(0,0)))
			 return(FALSE);

		/* Grab the first free audio channel--OpenDevice returns 0 on success */
		c=0;
		while ((device) AND (c < 4))
		{
			AIOptr->ioa_Request.io_Message.mn_ReplyPort   = port;
			AIOptr->ioa_Request.io_Message.mn_Node.ln_Pri = 127;	/* Don't let anyone take it from us! */
			AIOptr->ioa_AllocKey			   							    = 0;
			AIOptr->ioa_Data			     									  = chans[c];
			AIOptr->ioa_Length			   								    = 1;		/* Size of the allocation array--1 channel => 1 */

			device = OpenDevice(AUDIONAME, 0L, (struct IORequest *)AIOptr, 0L);
			c++;
		}
	
		if (device)
			return(FALSE);

		AIOptr->ioa_Request.io_Command	= CMD_WRITE;
		AIOptr->ioa_Request.io_Flags	= ADIOF_PERVOL;
		AIOptr->ioa_Volume = 60;
		AIOptr->ioa_Cycles = 1;
		AIOptr->ioa_Request.io_Message.mn_ReplyPort = port;
		AIOptr->ioa_Data = NULL;
		AIOptr->ioa_Length = 0L;
	}
	else
	{
		/* FREE AUDIO HARDWARE */
		if (device == 0) {CloseDevice((struct IORequest *)AIOptr); device = 1;} 
		if (port)	 {DeletePort(port); port = NULL;}
		if (AIOptr)	 {FreeMem(AIOptr, sizeof(struct IOAudio)); AIOptr = NULL;}
	}
	return(TRUE);
}

VOID CleanUp (VOID)
{
	UWORD unit;

//	VE_RestoreFunction ();

	if (s_save_flag)
	{
		if  (JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_FILE_WATCH_CHANGED), GLS (&L_SAVE_CANCEL), S_MESSAGE))
			s_SaveFiles ();
	}		

	if (prefs->logging)
		CloseLog ();
	if (!aminet_flag)
	{
	  s_fib_Close ();
		AllocAudio (0);
		if (SoundBuffer)
			FreeMem (SoundBuffer, SOUND_LEN);
	}

	if (vec)
		FreeMem (vec, sizeof (Vectors));
	if (tmp_vec)
		FreeMem (tmp_vec, sizeof (Vectors));

	if (viruslist)
		xvsFreeVirusList (viruslist);
	if (bootinfo)
		xvsFreeObject (bootinfo);
	if (sectorinfo)
		xvsFreeObject (sectorinfo);
	if (xvsBase)
		CloseLibrary (xvsBase);

	if (textfont)
		CloseFont (textfont);

	if (!aminet_flag)
	{
	  CloseOtherBoots ();
		BaseCleanUp ();

		for (unit = 0; unit < 4; unit++)
		{
			if (Buffer[unit])
				FreeMem (Buffer[unit], BUFSIZE);
		}

		if (TimerIO)
		{
			if (!(CheckIO ((struct IORequest *)TimerIO)))
	  	  AbortIO ((struct IORequest *)TimerIO);
			WaitIO ((struct IORequest *)TimerIO);
	  }

		if (!tdevice)
			CloseDevice ((struct IORequest *)TimerIO);
		if (TimerMP)
			DeletePort (TimerMP);
		if (TimerMSG)
			DeleteExtIO ((struct IORequest *)TimerIO);

		if (diskreq)
			DeleteExtIO ((struct IORequest *)diskreq);
		if (diskport)
			DeletePort (diskport);
		if (VirusWnd)
		{
			pr->pr_WindowPtr = oldwinptr;
			CloseVirusWindow ();
		}
		if (mycatalog)
			CloseCatalog (mycatalog);
		if (localeBase)
			CloseLibrary ((struct Library *)localeBase);
		CloseDownScreen ();
		SavePrefs ();
	}

	if (prefs)
		FreeMem (prefs, sizeof (Prefs));

	if (ReqToolsBase)
		CloseLibrary ((struct Library *)ReqToolsBase);
	if (AslBase)
		CloseLibrary ((struct Library *)AslBase);
	if (UtilityBase)
		CloseLibrary (UtilityBase);
	if (GadToolsBase)
		CloseLibrary (GadToolsBase);

	if (Dummy)
		FreeMem (Dummy, 10000);

	if (xfdMasterBase)
		CloseLibrary((struct Library *)xfdMasterBase);
	if (xadMasterBase)
		CloseLibrary((struct Library *)xadMasterBase);

	if (!aminet_flag)
	{
		RemPort (ve_port);
  	DeleteMsgPort (ve_port);
  }

//  MWReport("At end of main()", MWR_FULL);  /* Generate a memory usage report */
	if (restart_flag)
	{
		char *Buf;

		Buf = AllocMem (0, NULL);
	}
	exit (TRUE);
}

UWORD CenterWindowLeft (UWORD width)
{
	return (UWORD)((screenWidth / 2) - (width / 2));
}

UWORD CenterWindowTop (UWORD height)
{
	return (UWORD)((screenHeight / 2) - (height / 2));
}

struct Window *IconWnd = NULL;
extern UBYTE *PubScreenName;

struct Screen *IconScr;

int OpenIconWindow (VOID)
{
	if (!(IconScr = LockPubScreen (PubScreenName)))
		return (2L);

	if (!(IconWnd = OpenWindowTags (NULL,
				WA_Left,	0,
				WA_Top,		0,
				WA_Width,	20,
				WA_Height,	11,
				WA_IDCMP,	IDCMP_DISKINSERTED|IDCMP_DISKREMOVED|IDCMP_REFRESHWINDOW,
				WA_Flags,	WFLG_SMART_REFRESH|WFLG_BACKDROP|WFLG_BORDERLESS,
				WA_ScreenTitle,	"",
				WA_PubScreen,	IconScr,
				TAG_DONE )))
	return( 4L );

	GT_RefreshWindow (IconWnd, NULL);

	return( 0L );
}

void CloseIconWindow (VOID)
{
	if (IconWnd)
	{
		CloseWindow (IconWnd);
		IconWnd = NULL;
	}
	UnlockPubScreen (NULL, IconScr);
}

VOID Iconify (VOID)
{
	if (!icon)
	{
		All (ON);
		if (!OpenIconWindow ())
		{
			if (OpenIcon ())
			{	
				if (textfont)
					CloseFont (textfont);
				if (VirusWnd)
				{
					ClearIntuition (VirusWnd);
					pr->pr_WindowPtr = oldwinptr;
					CloseVirusWindow ();
					VirusWnd = 0;
				}
				CloseDownScreen ();

				winsig = 1L<<IconWnd->UserPort->mp_SigBit;
			}
			else
				strcpy (Dummy, GLS (&L_COULDNT_CREATE_ICON));
		}
		else
			strcpy (Dummy, GLS (&L_NO_WB));
		if (!icon)
			JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
	}		
}

VOID UnIconify (VOID)
{
	ClearIntuition (IconWnd);
	CloseIconWindow ();
	CloseIcon ();

	if (SetupScreen ())
		CleanUp ();

	if (OpenVirusWindow ())
		CleanUp ();

	winsig = 1L<<VirusWnd->UserPort->mp_SigBit;

	if (prefs->wb_flag)
		SetWindowTitles (VirusWnd, Ver, NULL);
	else
		SetWindowTitles (VirusWnd, NULL, Ver);

	rp = VirusWnd->RPort;
	pr->pr_WindowPtr = VirusWnd;

	textfont = OpenFont (&topaz8);
	if (textfont)
		SetFont (rp, textfont);
		
//	ScreenToFront (Scr);
	SetUp (0);

	Status (GLS (&L_READY));
}

VOID Status (char *Streng)
{
	char S[70];
	UWORD len;

	if (icon OR aminet_flag)
		return;

	len = strlen (Streng);
	if (len >= 69)
	{
		strncpy (S, &Streng[len - 69], 69);
		S[0] = '<';
		S[1] = '<';
		S[69] = 0;
	}
	else
		JEOCopy (S, Streng, 69, ' ');
	JEOWrite (rp, mainX + STATUSX + 68, STATUSY+OFFY, S, BLACK);
}

VOID DiskStatus (UBYTE *Streng, WORD unit)
{
	UBYTE S[72];

	if (aminet_flag)
		return;

	len = strlen (Streng);
	if (len > 71)
	{
		strncpy (S, &Streng[len - 71], 71);
		S[0] = '<';
		S[1] = '<';
		S[71] = 0;
	}
	else
		JEOCopy (S, Streng, 71, ' ');

	mystrcpy (DiskText[unit], S);

	if (!icon)
	{
		if (!filecheck_progress)
			JEOWrite (rp, mainX + DISKX + 44, diskY[unit] + OFFY, S, BLACK);
	}
}

BOOL GetSysTime (UBYTE *TimeString)
{
	struct timerequest tr;
	struct MsgPort *tport;
	ULONG days, hrs, mins, secs, mics;

  if ((OpenDevice (TIMERNAME, UNIT_MICROHZ, (struct IORequest *)&tr, 0)))
    return (FALSE);
  if (!(tport = CreatePort (0, 0)))
  {
    CloseDevice ((struct IORequest *) &tr);
    return (FALSE);
  }
  tr.tr_node.io_Message.mn_Node.ln_Type	= NT_MESSAGE;
  tr.tr_node.io_Message.mn_Node.ln_Pri	= 0;
  tr.tr_node.io_Message.mn_Node.ln_Name	= NULL;
  tr.tr_node.io_Message.mn_ReplyPort	= tport;
  tr.tr_node.io_Command			= TR_GETSYSTIME;
  DoIO ((struct IORequest *)&tr);
  mics = tr.tr_time.tv_micro;
  secs = tr.tr_time.tv_secs;
  DeletePort (tport);
  CloseDevice ((struct IORequest *) &tr);

  mins = secs / 60;
  hrs = mins / 60;
  days = hrs / 24;
  secs = secs % 60;
  mins = mins % 60;
  hrs = hrs % 24;

  SPrintF (TimeString, "%2ld:%02ld:%02ld", hrs, mins, secs);
  return (TRUE);
}

ULONG mmu;
BOOL enforcer_flag;

VOID PrintTime (VOID)
{
	UBYTE col;
	UWORD x1 = 8, y1 = 49;
	char Dummy[20];

	if (icon OR aminet_flag)
		return;

	GetSysTime (Dummy);
	Dummy[5] = 0;
	JEOWrite (rp, mainX + 29, 123+OFFY, Dummy, BLACK);

	if (mmu)
		SPrintF (Dummy, "%ld", mmu);
	else
		SPrintF (Dummy, "-----");

	if (enforcer_flag)
		col = BLUE;
	else
		col = BLACK;
	y1 += 16;
	JEOWrite (rp, mainX + x1 + (5 * 8) - 3, y1 + OFFY, Dummy, col);
}

VOID tderror (WORD error, WORD unit)
{
	if (error >= TDStart AND error <= TDEnd)
		SPrintF (TDErrorMsg, GLS (&L_UNKNOWN_TRACKDISK0), error, TDError[(WORD)(error - TDStart)]);
	else
	{
		SPrintF (Dummy, GLS (&L_UNKNOWN_TRACKDISK1), error, unit);
		 JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
		SPrintF (TDErrorMsg, GLS (&L_UNKNOWN_TRACKDISK2), error);
	}
}

/*
BOOL NewBoot (VOID)	// Returnerer TRUE dersom det er ny boot
{
	LONG readerror = 0;
	ULONG *Tmp_buf;
	BOOL flag = FALSE;
	UBYTE u;

	for (u = 0; u < 4; u++)
	{
		if (viewCol[u] == WHITE)	// Er det en disk i?
		{
			if (Tmp_buf = AllocMem (512, MEMF_CHIP | MEMF_PUBLIC))
			{
				if (!OpenDevice (TD_NAME, u, (struct IORequest *)diskreq, NULL))
				{
				  diskreq->iotd_Req.io_Length = 512;
				  diskreq->iotd_Req.io_Data = (APTR)Tmp_buf;
			  	diskreq->iotd_Req.io_Command = CMD_READ;
				  diskreq->iotd_Req.io_Offset = 0;
		 			DoIO ((struct IORequest *)diskreq);
			  	if (diskreq->iotd_Req.io_Error)
				    readerror = diskreq->iotd_Req.io_Error;
				  DiskMotor (diskreq, AV);
					CloseDevice ((struct IORequest *)diskreq);
					if (readerror == 0)	// Vi sjekker kun dersom det er ok...
					{
						if (Tmp_buf[1] != old_checksum)	// Har noen lagret en ny boot?
						{
							if (VE_ReadBoot (u))
								VE_CheckBoot (u);
						}
					}
				}
				FreeMem (Tmp_buf, 512);
			}
		}
	}

	return (flag);
}
*/

BOOL VE_ReadBoot (WORD unit)
{
	LONG readerror = 0;

	decrypt_flag = FALSE;

	if (unit < 0 OR unit > 4)
		return (FALSE);

	if (!OpenDevice (TD_NAME, unit, (struct IORequest *)diskreq, NULL))
	{
	  diskreq->iotd_Req.io_Length = 1024;
	  diskreq->iotd_Req.io_Data = (APTR)Buffer[unit];
	  diskreq->iotd_Req.io_Command = CMD_READ;
	  diskreq->iotd_Req.io_Offset = 0;
 		DoIO ((struct IORequest *)diskreq);
	  if (diskreq->iotd_Req.io_Error)
	    readerror = diskreq->iotd_Req.io_Error;
	  DiskMotor (diskreq, AV);

		CloseDevice ((struct IORequest *)diskreq);
		if (readerror != 0)
		{
			tderror (readerror, unit);
			DiskStatus (TDErrorMsg, unit);
			return (FALSE);
		}
	}
	else
		return (FALSE);
	viewCol[unit] = WHITE;
	return (TRUE);
}

LONG WriteBoot (WORD unit)
{
	LONG writeerror = 0;

	if (unit < 0 OR unit > 4)
		return (ERROR);

	if (!OpenDevice (TD_NAME, unit, (struct IORequest *)diskreq, NULL))
	{
	  diskreq->iotd_Req.io_Length = 1024;
	  diskreq->iotd_Req.io_Data = (APTR)Buffer[unit];
	  diskreq->iotd_Req.io_Command = CMD_WRITE;
	  diskreq->iotd_Req.io_Offset = 0;
 		DoIO ((struct IORequest *)diskreq);
		writeerror = diskreq->iotd_Req.io_Error;
	  if (writeerror == 0)		// No error
	  {
  	  diskreq->iotd_Req.io_Command = CMD_UPDATE;	// Flush buffer to disk
    	DoIO ((struct IORequest *)diskreq);
			writeerror = diskreq->iotd_Req.io_Error;
	  }
	  DiskMotor (diskreq, AV);
		CloseDevice ((struct IORequest *)diskreq);
		if (writeerror)	// Error?
		{
			tderror (writeerror, unit);
			DiskStatus (TDErrorMsg, unit);
		}
	}
	viewCol[unit] = WHITE;
	return (writeerror);
}

VOID ViewBoot (UWORD unit)
{
	UWORD i, y;
	char *View = 0;

	last_boot = unit;
	if (icon)
		return;


	if (View = AllocMem (BUFSIZE, MEMF_CLEAR))
	{
		if (!filecheck_progress)
		{
			for (i = 0; i < 4; i++)
			{
				if (drive[i] != ERROR)
					JEOWrite (rp, mainX + DISKX + 33, diskY[i] + OFFY, " ", GREY);
			}
			if (viewCol[unit] != BLUE)
			{
				JEOWrite (rp, mainX + DISKX + 33, diskY[unit] + OFFY, "*", BLUE);
				view_unit = unit;
			}
			else
				view_unit = ERROR;
		}

		for (i = 0; i < BUFSIZE; i ++)
		{
	    if ((Buffer[unit][i] >= 32 AND Buffer[unit][i] <= 127) OR (Buffer[unit][i] >= 161 AND Buffer[unit][i] <= 255))
				View[i] = Buffer[unit][i];
			else
				View[i] = '.';
		}
		y = 0;
		for (i = 0; i < BUFSIZE; i += 64)
		{
			SetAPen (rp, viewCol[unit]);
			Move (rp, mainX + 106, 52 + (y * 9) + OFFY);
			Text (rp, &View[i], 64);
			y++;
		}
		FreeMem (View, BUFSIZE);
	}
}

BOOL CheckForFormat (WORD unit)
{
	if (!(strncmp (Buffer[unit], "BAD0", 4)))
		return (TRUE);
	else if (!(strncmp (Buffer[unit], "COPY", 4)))
		return (TRUE);
	else
		return (FALSE);
}

BOOL CheckForPFS (WORD unit)
{
	if (!(strncmp (Buffer[unit], "PFS", 3)))
	{
		DiskStatus (GLS (&L_PROF_FILE), unit);
		return (TRUE);
	}
	else
		return (FALSE);
}

BOOL CheckForFlashback (WORD unit)
{
	UWORD i;

	if (Buffer[unit][0] == 'F')
	{
		for (i = 1; i < 4; i++)
		{
			if (!isdigit (Buffer[unit][i]))
				return (FALSE);
		}
	}
	else
		return (FALSE);

	strcpy (Dummy, "Flashback #");
	Dummy[11] = Buffer[unit][1];
	Dummy[12] = Buffer[unit][2];
	Dummy[13] = Buffer[unit][3];
	Dummy[14] = '.';
	Dummy[15] = 0;
	DiskStatus (Dummy, unit);
	return (TRUE);
}

BOOL CheckForABackup (WORD unit)
{
	UWORD i;

	if (Buffer[unit][0] == '#')
	{
		for (i = 1; i < 4; i++)
		{
			if (!isdigit (Buffer[unit][i]))
				return (FALSE);
		}
	}
	else
		return (FALSE);

	strcpy (Dummy, "ABackup #");
	Dummy[9] = Buffer[unit][1];
	Dummy[10] = Buffer[unit][2];
	Dummy[11] = Buffer[unit][3];
	Dummy[12] = '.';
	Dummy[13] = 0;
	DiskStatus (Dummy, unit);
	return (TRUE);
}

BOOL CheckForQuarterback (WORD unit)
{
	UWORD i;

	if (!(strnicmp (Buffer[unit], "QB", 2)))
	{
		for (i = 2; i < 4; i++)
		{
			if (!isdigit (Buffer[unit][i]))
				return (FALSE);
		}
	}
	else
		return (FALSE);
	strcpy (Dummy, "Quarterback #");
	Dummy[13] = Buffer[unit][2];
	Dummy[14] = Buffer[unit][3];
	Dummy[15] = '.';
	Dummy[16] = '\0';
	DiskStatus (Dummy, unit);
	return (TRUE);
}

/*
BOOL CheckForChecksum (UWORD unit)
{
	LONG check;

	check = CalculateBBChecksum ((ULONG *)Buffer[unit]);
	if (check != old_checksum)
	{
		DiskStatus (GLS (&L_ERROR_IN_CHECKSUM), unit);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
		return (FALSE);
	}
	else
		return (TRUE);
}
*/

BOOL CheckForNormal (WORD unit)
{
	LONG check[2];

	if (!(strncmp (Buffer[unit], "DOS", 3)))
	{
		CopyMem (Buffer[unit], check, 8);
		old_checksum = check[1];

		strcpy (Dummy, GLS (&L_INSTALLED_DOS));
	  switch (check[1])
		{
			case 0x00000000:
			{
				if (Buffer[unit][8] == 0)
				if (Buffer[unit][16] == 0)
				if (Buffer[unit][32] == 0)
				if (Buffer[unit][64] == 0)
				{
					DiskStatus (GLS (&L_UNINST), unit);
					return (TRUE);
				}
			}
			case 0xc0200f19:
			{
				UWORD i;
				BOOL flag =TRUE;

				for (i = 50; i < 400; i++)
				{
					if (Buffer[unit][i] != 0)
					{
						flag = FALSE;
						break;
					}
				}
				if (flag)
				{
					strcat (Dummy, " 1.x (OFS).");
					DiskStatus (Dummy, unit);
					return (TRUE);
				}
				break;
			}
			case 0xe33d0e73: strcat (Dummy, " 2.0 (OFS)."); DiskStatus (Dummy, unit); return (TRUE);
			case 0xe33d0e72: strcat (Dummy, " 2.0 (FFS)."); DiskStatus (Dummy, unit); return (TRUE);
			case 0xe33d0e71: strcat (Dummy, " 2.0 (OFS Int)."); DiskStatus (Dummy, unit); return (TRUE);
			case 0xe33d0e70: strcat (Dummy, " 2.0 (FFS Int)."); DiskStatus (Dummy, unit); return (TRUE);
			case 0xe33d0e6f: strcat (Dummy, " 3.0 (OFS DC)."); DiskStatus (Dummy, unit); return (TRUE);
			case 0xe33d0e6e: strcat (Dummy, " 3.0 (FFS DC)."); DiskStatus (Dummy, unit); return (TRUE);

			case 0xaac8246d: SPrintF (Dummy, GLS (&L_FMS), "2.2"); DiskStatus (Dummy, unit); return (TRUE);
			case 0x225a4b65: SPrintF (Dummy, GLS (&L_FMS), "3.0"); DiskStatus (Dummy, unit); return (TRUE);
			case 0xde5c20a4: SPrintF (Dummy, GLS (&L_FMS), "3.0 Int"); DiskStatus (Dummy, unit); return (TRUE);
		}
		if (!(strncmp (&Buffer[unit][4], "DOS", 3)))
		{
			DiskStatus (GLS (&L_UNINST), unit);
			return (TRUE);
		}
	}
	return (FALSE);
}

VOID VE_CheckBoot (WORD unit)
{
	LONG retcode;
	UBYTE Message[51];

	DiskStatus (GLS (&L_CHECKING_BOOTBLOCK), unit);
	ViewBoot (unit);
	prefs->checked++;

	knownBoot[unit] = TRUE;	// TRUE = trenger ikke å recorde
	if (CheckForNormal (unit))
		return;
	if (CheckForFormat (unit))
	{
		DiskStatus (GLS (&L_BAD_FORMAT_DISK), unit);
		return;
	}

	bootinfo->xvsbi_Bootblock = Buffer[unit];
	retcode = xvsCheckBootblock (bootinfo);
	switch (retcode)
	{
		case XVSBT_VIRUS:
		{
			if (icon)
				UnIconify ();
			PlaySound (S_VIRUS);
			All (OFF);
      ScreenToFront (Scr);
			knownBoot[unit] = VIRUS;
			prefs->bootVir++;
			UpDateVirusCount ();

			WriteToLog ("", LOG_DATE);
			SPrintF (Dummy, GLS (&L_VIRUS_FOUND_IN), bootinfo->xvsbi_Name, Drive[unit]);
			WriteToLog (Dummy, LOG_TEXT);
			WriteToLog ("", LOG_END);

			SPrintF (Dummy, GLS (&L_VIRUS_1), bootinfo->xvsbi_Name);
			DiskStatus (Dummy, unit);
			SPrintF (Dummy, GLS (&L_VIRUS_FOUND_IN), bootinfo->xvsbi_Name, Drive[unit]);
			if (rtEZRequestTags (Dummy, GLS (&L_KILL_VIRUS_CANCEL),
				NULL, NULL, RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_WARNING), RT_TextAttr, &topaz8, TAG_END))
			{	
				if (!Install (unit, 0))
				{
					if (VE_ReadBoot (unit))	// Read newly hopfully installed boot
					{
						VE_CheckBoot (unit);	// Check it!!!
						prefs->checked--;		// No need to be updated here?
					}
				}
			}
			All (ON);
			break;
		}
		case XVSBT_UNKNOWN:
		{
			knownBoot[unit] = TRUE;
			if (CheckRecorded (unit))	// Har vi den i databasen?
				return;

			knownBoot[unit] = OTHER;
			switch (CheckOtherBoots ((ULONG *)Buffer[unit]))
			{
			  case UTILITY:
					DiskStatus (GLS (&L_UTILITY_BOOT), unit);
					return;
			  case VIRUSKILLER:
					DiskStatus (GLS (&L_VIRUS_KILLER), unit);
					return;
			  case INTRO:
					DiskStatus (GLS (&L_INTRO_BOOT), unit);
					return;
			  case LOADER:
					DiskStatus (GLS (&L_LOADER), unit);
					return;
			}

			if (icon)
				UnIconify ();
			DiskStatus (GLS (&L_ANALYZING), unit);
			knownBoot[unit] = FALSE;
			Analyze ((UWORD *)Buffer[unit], unit);
      ScreenToFront (Scr);
			PlaySound (S_MESSAGE);
			break;
		}
		case XVSBT_NOTDOS:
		{
			knownBoot[unit] = FALSE;
			if (CheckForFlashback (unit))
				return;
			if (CheckForABackup (unit))
				return;
			if (CheckForQuarterback (unit))
				return;
			if (CheckForPFS (unit))
				return;

			knownBoot[unit] = TRUE;
			if (CheckNotBoot (unit, Message))	// Sjekker om det er en kickstart...
				DiskStatus (Message, unit);
			else
				DiskStatus (GLS (&L_NOT_A_BOOT_DISK), unit);
			break;
		}
		case XVSBT_STANDARD13: // plus!
		{
			knownBoot[unit] = TRUE;	// TRUE = trenger ikke å recorde
			strcpy (Dummy, GLS (&L_INSTALLED_DOS));
			strcat (Dummy, "1.x (OFS+).");
			DiskStatus (Dummy, unit);
			break;
		}
		default:
			SPrintF (Dummy, GLS (&L_RETCODE_NAME), retcode, bootinfo->xvsbi_Name);
			JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
			DiskStatus ("???.", unit);
			break;
	}
}

LONG CheckProtected (WORD unit)
{
	LONG error = 0;

	if (unit < 0 OR unit > 4)
		return (ERROR);

	if (!OpenDevice (TD_NAME, unit, (struct IORequest *)diskreq, NULL))
	{
	  diskreq->iotd_Req.io_Command = TD_PROTSTATUS;
 		DoIO ((struct IORequest *)diskreq);
		error = diskreq->iotd_Req.io_Error;	// blir vel aldri no error her?
//	  DiskMotor (diskreq, AV);
		CloseDevice ((struct IORequest *)diskreq);
	}

	return (error);
}

LONG Install (WORD unit, WORD mode)
{
	LONG ret;

	ret = CheckProtected  (unit);

	if (mode == DATABASE)
	{	
		SPrintF (Dummy, GLS (&L_INSTALLING_TO), DummyName, Drive[unit]);
		Status (Dummy);
		if (ret == 0)	// Ok?
		{
			CopyMem (DummyBuf, Buffer[unit], BUFSIZE);
			ret = WriteBoot (unit);
		}
	}
	else
	{
		SPrintF (Dummy, GLS (&L_INSTALLING), Drive[unit]);
		Status (Dummy);
		if (ret == 0)	// Ok?
		{
			xvsInstallBootblock (Buffer[unit], XVSBT_STANDARD20, mode);
			ret = WriteBoot (unit);
		}
	}

	return (ret);
}

VOID Menus (BOOL status)
{
	if (aminet_flag)
		return;

	if (status == ON)
	{
		if (menuFlag == OFF)
		{
			Forbid ();
			VirusWnd->Flags ^= RMBTRAP;
			Permit ();
			menuFlag = ON;
		}
	}
	else
	{
		if (menuFlag == ON)
		{
			Forbid ();
			VirusWnd->Flags ^= RMBTRAP;
			Permit ();
			menuFlag = OFF;
		}
	}
}

VOID All (BOOL mode)
{
	if (aminet_flag OR icon)
		return;

	if (!mode)
	{
		rtSetWaitPointer (VirusWnd);
		Menus (OFF);
	}
	else
	{
		Menus (ON);
		ClearPointer (VirusWnd);
	}
}

VOID MenuPick (USHORT code)
{
	switch (MENUNUM (code))
	{
		case 0:		// Project
		{
			switch (ITEMNUM (code))
			{
				case 0:		// About
				{
					Status (GLS (&L_PLEASE_WAIT));
					All (OFF);
					VE_About ();
					All (ON);
					Status (GLS (&L_READY));
					break;
				}
				case 2:		// Iconify
				{
					Iconify ();
					break;
				}
				case 4:		// Prefs
				{
					All (OFF);
					PrefsWindow ();
					All (ON);
					break;
				}
				case 6:		// Memory check
				{
					Status (GLS (&L_PLEASE_WAIT));
					All (OFF);
					CheckMemory (ON);
					All (ON);
					Status (GLS (&L_READY));
					break;
				}
				case 7:		// Memory check
				{
					Status (GLS (&L_PLEASE_WAIT));
					All (OFF);
					if (JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_REBOOT_THE_MACHINE), GLS (&L_CANCEL_REBOOT_CANCEL), S_NO_SOUND) == 2)
					{
						All (OFF);
						SavePrefs ();
						Delay (200);
						Disable ();
					  SysBase->WarmCapture  = 0;
					  SysBase->KickCheckSum = 0;
					  SysBase->KickMemPtr   = 0;
					  SysBase->KickTagPtr   = 0;
					  SysBase->CoolCapture  = 0;
						SysBase->ColdCapture  = 0;
						ColdReboot ();
					}
					All (ON);
					Status (GLS (&L_READY));
					break;
				}
				case 9:		// File check
				{
					Status (GLS (&L_PLEASE_WAIT));
					All (OFF);
					CheckFiles ("", 0);
					All (ON);
					break;
				}
				case 10:		// Check all valid volumes
				{
					Status (GLS (&L_PLEASE_WAIT));
					All (OFF);
					CheckFiles ("", 1);
					All (ON);
					break;
				}
				case 11:		// Sector check
				{
					Status (GLS (&L_PLEASE_WAIT));
					All (OFF);
					CheckSector (0, 79);
					Status (GLS (&L_READY));
					All (ON);
					break;
				}
				case 13:		// Select Screen Mode
				{
					SelectScreenMode (1);
					break;
				}
				case 15:		// Avslutt
				{
					CleanUp ();
					break;
				}
			}
			break;
		}
		case 1:		// File watch
		{
			switch (ITEMNUM (code))
			{
				case 0:	// Check now!
				{
					All (OFF);
					Check_changed ();
					All (ON);
					break;
				}
				case 2:	// Add a directory
				{
					All (OFF);
					s_AddDir ();
					All (ON);
					break;
				}
				case 3: // Add/update files used by 's:startup-sequence'
				{
					All (OFF);
					Add_files ();
					All (ON);
					break;
				}
				case 5:	// Remove files...
				{
					All (OFF);
					RemoveFW ();
					All (ON);
					Status (GLS (&L_READY));
					break;
				}
				case 7:	// Save
				{
					All (OFF);
					s_SaveFiles ();
					All (ON);
					Status (GLS (&L_READY));
					break;
				}
			}
			break;
		}
		case 2:		// Show
		{
			switch (ITEMNUM (code))
			{
				case 0: ShowStuff (XVSLIST_BOOTVIRUSES); break;
				case 1: ShowStuff (XVSLIST_LINKVIRUSES); break;
				case 2: ShowStuff (XVSLIST_FILEVIRUSES); break;
				case 3: ShowStuff (XVSLIST_DATAVIRUSES); break;
				case 5: ShowStuff (TASKS); break;
				case 6: ShowStuff (LIBS); break;
				case 7: ShowStuff (DEVICES); break;
				case 9: ShowStuff (PATCHES); break;
			}
			break;
		}
		case 3:		// Installere
		{
			switch (ITEMNUM (code))
			{
				case 0: FInstall (0); All (ON); break;
				case 1: FInstall (1); All (ON); break;
				case 2: FInstall (2); All (ON); break;
				case 3: FInstall (3); All (ON); break;
			}
			break;
		}
		case 4:		// Database
		{
			switch (ITEMNUM (code))
			{
				case 0: All (OFF); Record (); All (ON); break;		   // Lagre bootblokk
				case 2: All (OFF); ViewBase (); All (ON); break;	   // Vis
				case 3: All (OFF); EditName (); All (ON); break;	   // Forandre navn
				case 4: All (OFF); MergeBase (); All (ON); break;	   // Flette
				case 6: All (OFF); DeleteOneRecord (); All (ON); break;	   // Slette en
				case 7: All (OFF); DeleteAll (); All (ON); break;	   // Slette alt
				case 9: All (OFF); SaveBase (OFF); All (ON); break;	   // Lagre
			}
			break;
		}
		case 5:		// Utilities
		{
			switch (ITEMNUM (code))
			{
				case 0: Status (GLS (&L_PLEASE_WAIT)); Rename_pics (); All (ON); Status (GLS (&L_READY)); break;
				case 2: Status (GLS (&L_PLEASE_WAIT)); Convert_PC_text (); All (ON); Status (GLS (&L_READY)); break;
				case 4: Status (GLS (&L_PLEASE_WAIT)); SaveROM (); Status (GLS (&L_READY)); break;
				case 6: Status (GLS (&L_PLEASE_WAIT)); Update_files (); Status (GLS (&L_READY)); break;
			}
			break;
		}
	}
}

VOID WriteAllBootToRAM (VOID)
{
	BPTR fh = 0;
	UBYTE unit;

  Status ("Writing all boots to RAM:");
	for (unit = 0; unit < 4; unit++)
	{
		if (!OpenDevice (TD_NAME, unit, (struct IORequest *)diskreq, NULL))
		{
			if (DiskInDrive (diskreq))
			{
			  SPrintF (Dummy, "RAM:BB%ld", unit);
				if (fh = Open (Dummy, MODE_NEWFILE))
				{
					Write (fh, Buffer[unit], BUFSIZE);
					Close (fh);
				}
			}
			CloseDevice ((struct IORequest *)diskreq);
		}
	}
}

VOID RawKey (USHORT code)
{
//  BYTE unit;

	switch (code)
	{
		case NUMERIC0:
		{
			if (viewCol[0] == WHITE)
				ViewBoot (0);
			break;
		}
		case NUMERIC1:
		{
			if (viewCol[1] == WHITE)
				ViewBoot (1);
			break;
		}
		case NUMERIC2:
		{
			if (viewCol[2] == WHITE)
				ViewBoot (2);
			break;
		}
		case NUMERIC3:
		{
			if (viewCol[3] == WHITE)
				ViewBoot (3);
			break;
		}
		case N1: memcheck_delay = 1; counter = ON; break;
		case N2: memcheck_delay = 2; counter = ON; break;
		case N3: memcheck_delay = 3; counter = ON; break;
		case N4: memcheck_delay = 4; counter = ON; break;
		case N5: memcheck_delay = 5; counter = ON; break;
		case N6: memcheck_delay = 6; counter = ON; break;
		case N7: memcheck_delay = 7; counter = ON; break;
		case N8: memcheck_delay = 8; counter = ON; break;
		case N9: memcheck_delay = 9; counter = ON; break;
		case N0: memcheck_delay = 10; counter = ON; break;

		case HELP:
		{
			SPrintF (Dummy, "SYS:Utilities/MultiView %s%s.guide", VEAssign, VE);
			Execute (Dummy, NULL, NULL);
		  break;
    }

		case F5:
		{
			wb_flag ^= 1;
			Iconify ();
			UnIconify ();
			break;
		}
#if _DEBUG
		case F1:
		{
		  WriteAllBootToRAM ();
		  break;
    }

		case F6:
		{
		  SPrintF (Dummy, "Last checksum = $%08lX", old_checksum);
		  Status (Dummy);
		  break;
		}
		case F9:
		{
			UBYTE unit;

      for (unit = 0; unit < 4; unit++)
 	    {
				if (drive[unit] != ERROR)
				{
					if (VE_ReadBoot (unit))
						VE_CheckBoot (unit);
				}
			}
			break;
		}
		case F10:
		{
			UBYTE unit;

      for (unit = 0; unit < 4; unit++)
 	    {
				if (drive[unit] != ERROR)
					Analyze ((UWORD *)Buffer[unit], unit);
 			}
			break;
		}
#endif
	}
}

BYTE WhoChanged (VOID)
{
	BYTE unit;
	LONG count;

	for (unit = 0; unit < 4; unit++)
	{
		if (drive[unit] == ERROR)
			continue;

		if (!OpenDevice (TD_NAME, unit, (struct IORequest *)diskreq, NULL))
		{	
			diskreq->iotd_Req.io_Command = TD_CHANGENUM;
			diskreq->iotd_Req.io_Flags = IOF_QUICK;
			DoIO ((struct IORequest *)diskreq);

			count = diskreq->iotd_Req.io_Actual;
			CloseDevice ((struct IORequest *)diskreq);

			if (drive[unit] != count)
			{
				drive[unit] = count;
				return (unit);
			}
		}
	}
	return (ERROR);
}


VOID DiskInserted (VOID)
{
	WORD unit;

	unit = WhoChanged ();
	if (unit == ERROR)	// No changes
		return;
	DiskStatus (GLS (&L_PLEASE_WAIT), unit);
	if (VE_ReadBoot (unit))
		VE_CheckBoot (unit);
}

VOID ClearDiskBuffer (WORD unit)
{
	memset (Buffer[unit], '.', BUFSIZE);
	viewCol[unit] = BLUE;
}

VOID DiskRemoved ()
{
	WORD unit, i;

	unit = WhoChanged ();
	if (unit == ERROR)
		return;

	ClearDiskBuffer (unit);
	DiskStatus (GLS (&L_NO_DISK), unit);
	for (i = 0; i < 4; i++)	// Is there any other disks I can view
	{
		if (viewCol[i] == WHITE)
		{
			ViewBoot (i);
			return;
		}
	}
	ViewBoot (unit);
	Status (GLS (&L_READY));
}

#if _JEO_DEBUG

VOID VE_Debug (WORD mouseX, UWORD mouseY)
{
	LONG i, linjer, j = 0, k;
	UWORD y = 46;
	UWORD x;
	BOOL flag = FALSE;
	UBYTE Hold[5];
	UBYTE Mode[20];
	BPTR fh = 0;

	for (linjer = 0; linjer < 16; linjer++)
	{
		x = mainX + 115;
		for (i = 0; i < 64; i++)
		{
			if (mouseX >= x AND mouseY >= y AND mouseX <= (x + 8) AND mouseY <= (y + 8))
			{
				flag = TRUE;
				if (view_unit != ERROR)
				{
					if (j < 1024 - 4)
					{
						for (k = 0; k < 4; k++)
						{
							if (Buffer[view_unit][j + k] > 31 AND Buffer[view_unit][j + k] < 127)
								Hold[k] = Buffer[view_unit][j + k];
							else
							{
								k = 0;
								break;
							}
						}
						if (k)
						{
							Hold[k] = 0;
							if (strlen (Hold) == 4)
							{
								SPrintF (Dummy, "%08lX, %ld, \"%4s\"", old_checksum, j, Hold);
								Status (Dummy);
								Mode[0] = 0;
								switch (JEOSoundRequest (GLS (&L_MESSAGE), "Select:", "UTILITY|VIRUSKILLER|INTRO|LOADER|Exit", S_NO_SOUND))
								{
									case 1: mystrcpy (Mode, "UTILITY"); break;
									case 2: mystrcpy (Mode, "VIRUSKILLER"); break;
									case 3: mystrcpy (Mode, "INTRO"); break;
									case 4: mystrcpy (Mode, "LOADER"); break;
								}
								if (Mode[0])
								{
									SPrintF (Dummy, "  Insert_boot (i++, %s, 0x%08lX, %ld, \"%4s\");\n", Mode, old_checksum, j, Hold);
									if (fh = Open ("-V:INSERTBOOTS", MODE_OLDFILE))
									{
							      Seek (fh, 0, OFFSET_END);
										Write (fh, Dummy, strlen (Dummy));
										Close (fh);
									}
								}
							}
							else
								Status ("Error: Text too short...");
						}
						else
							Status ("Error: Non ASCII in text...");
					}
					else
						Status ("Error: to far away...");
				}
				else
					Status ("No disk in any drive");
				break;
			}
			j++;
			x += 8;
		}
		if (flag)
			break;
		y += 9;
	}
}
#endif

VOID FindLibraryVersion (char *String, APTR Lib)
{
	ULONG version;

	version = FindLibVersion (Lib);
	SPrintF (String, "%ld.%ld", version>>16, (UWORD)version);
}

VOID Check_libs (UBYTE *Last, UBYTE *Version, UBYTE *Name)
{
	WORD check, i, j;
	UWORD old_ver, new_ver, old_rev, new_rev;
	BOOL flag;

	old_ver = atoi (Last);
	new_ver = atoi (Version);

	flag = FALSE;
	for (i = 0, j = 0; Last[i] != 0; i++)
	{
		if (flag)
		{
			Dummy[j] = Last[i];
			j++;
		}
		if (Last[i] == '.')
			flag = TRUE;
	}
	Dummy[j] = 0;
	old_rev = atoi (Dummy);

	flag = FALSE;
	for (i = 0, j = 0; Version[i] != 0; i++)
	{
		if (flag)
		{
			Dummy[j] = Version[i];
			j++;
		}
		if (Version[i] == '.')
			flag = TRUE;
	}
	Dummy[j] = 0;
	new_rev = atoi (Dummy);

	if (old_ver == new_ver)	// like versjoner
	{
		if (old_rev < new_rev)
			check = 1;
		else if (old_rev == new_rev)
			check = 0;
		else
			check = -1;
	}
	else
	{
		if (old_ver < new_ver)
			check  = 1;
		else if (old_ver > new_ver)
			check  = -1;
	}

	if (check < 0)
	{
		SPrintF (Dummy, GLS (&L_YOU_HAVE_THE_OLD), Name, Version, Name, Last);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
	}
}

VOID VE_About (VOID)
{
	USHORT boot_virus, link_virus, file_virus, data_virus, i;
	struct xvsVirusList *viruslist;
	struct xfdSlave *xs;
	char Hold[200], xvs_ID[40];

	total_unpackers = 0;

	if (xfdmaster_flag)
	{
		for (xs = xfdMasterBase->xfdm_FirstSlave; xs; xs = xs->xfds_Next)
		 	++total_unpackers;
	}

	boot_virus = link_virus = file_virus = data_virus = 0;

	if (viruslist = xvsCreateVirusList (XVSLIST_BOOTVIRUSES))
	{
		boot_virus = viruslist->xvsvl_Count;
		xvsFreeVirusList (viruslist);
	}
	if (viruslist = xvsCreateVirusList (XVSLIST_LINKVIRUSES))
	{
		link_virus = viruslist->xvsvl_Count;
		xvsFreeVirusList (viruslist);
	}
	if (viruslist = xvsCreateVirusList (XVSLIST_FILEVIRUSES))
	{
		file_virus = viruslist->xvsvl_Count;
		xvsFreeVirusList (viruslist);
	}
	if (viruslist = xvsCreateVirusList (XVSLIST_DATAVIRUSES))
	{
		data_virus = viruslist->xvsvl_Count;
		xvsFreeVirusList (viruslist);
	}

	SPrintF (Hold, "%s", xvsBase->lib_IdString);
	ReverseString (Hold);
	for (i = 0; Hold[i] != 0; i++)
	{
		xvs_ID[i] = Hold[i];
		if (xvs_ID[i] == ' ')
		{
			xvs_ID[i] = 0;
			ReverseString (xvs_ID);
			break;
		}
	}

	SPrintF (Dummy, "%s v%s\n", VE, ProgramVersion);
  for (i = 0; Dummy[i] != 0; i++)
  	Hold[i] = '~';
	Hold[i++] = '\n';
	Hold[i] = 0;
	strcat (Dummy, Hold);

	mystrcpy (Hold, GLS (&L_TRANSLATED_BY));
	if (strcmp (Hold, "Translated to LANGUAGE by PUT YOUR NAME HERE"))
	{
		SPrintF (Hold, "%s\n\n", GLS (&L_TRANSLATED_BY));
		strcat (Dummy, Hold);
	}

	strcat (Dummy, CopyRight);
	strcat (Dummy, "\nEMail: virusexecutor@c2i.net\n\n");
	strcat (Dummy, GLS (&L_USING));

	SPrintF (Hold, "\n%s v%s %s\n", xvs_name, xvs_Version, xvs_ID);
	strcat (Dummy, Hold);

	if (boot_virus)
	{
		SPrintF (Hold, GLS (&L_KNOWS_VIRUS), boot_virus + data_virus + link_virus + file_virus - 1);
		strcat (Dummy, Hold);
	}

	if (xfdmaster_flag)
	{
		SPrintF (Hold, "%s v%s\n", xfdmaster_name, xfd_Version);
		strcat (Dummy, Hold);
		SPrintF (Hold, GLS (&L_KNOWS_DECRUNCHERS), total_unpackers);
		strcat (Dummy, Hold);
	}

	if (xadmaster_flag)
	{
		SPrintF (Hold, "%s v%s\n\n", xadmaster_name, xad_Version);
		strcat (Dummy, Hold);
	}

	SPrintF (Hold, "%s PatchBrain v%s\n", VE, patch_Version);
	strcat (Dummy, Hold);
	SPrintF (Hold, GLS (&L_KNOWS_PATCHES), 	max_entries, ve_max_patches - 1);
	strcat (Dummy, Hold);

	ConvertNumber (Tmp1, prefs->checked);
	ConvertNumber (Tmp2, prefs->fileChecked);

	SPrintF (Hold, GLS (&L_I_HAVE_CHECKED_S), Tmp1, prefs->checked == 1 ? GLS (&L_BOOTBLOCK) : GLS (&L_BOOTBLOCKS), Tmp2, prefs->fileChecked == 1 ? GLS (&L_FILE) : GLS (&L_FILES));
	strcat (Dummy, Hold);

	rtEZRequestTags (Dummy, GLS (&L_OK), NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT, RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_ABOUT), RT_TextAttr, &topaz8, TAG_END);
}

#define FINDY	167

VOID UpDateVirusCount (VOID)
{
	char Dummy[20];

	if (aminet_flag)
		return;

	ConvertNumber (Tmp1, prefs->bootVir);
	SPrintF (Dummy, "%10s", Tmp1);
	JEOWrite (rp, mainX + 8, FINDY +  8 + OFFY, Dummy, prefs->bootVir == 0 ? BLUE : BLACK);
	ConvertNumber (Tmp1, prefs->fileVir);
	SPrintF (Dummy, "%10s", Tmp1);
	JEOWrite (rp, mainX + 8, FINDY + 24 + OFFY, Dummy, prefs->fileVir == 0 ? BLUE : BLACK);
}

VOID StartTimer (ULONG secs, ULONG micro)
{
	TimerIO->tr_node.io_Command = TR_ADDREQUEST;
	TimerIO->tr_time.tv_secs = secs;
	TimerIO->tr_time.tv_micro = micro;
	SendIO ((struct IORequest *)TimerIO);
}

ULONG cpu, fpu;

VOID UpDateDriveInfo (VOID)
{
	UBYTE unit;

	StyleIt2 (rp, mainX+8-5 , 200 + OFFY, mainX+628, 246 + OFFY - diskShowY, BLACK, WHITE, GADGET_STYLE);	// df0:
	for (unit = 0; unit < 4; unit++)
	{
		if (drive[unit] != ERROR)
		{
			JEOWrite (rp, mainX + DISKX, diskY[unit] + OFFY, Drive[unit], WHITE);

			DiskStatus (DiskText[unit], unit);
		}
	}
	ViewBoot (last_boot);
}

char ChipSetName[][11] = 
{
	"Std Agnus ",
	"ECS Agnus ",
	"AGA Alice ",
	"Std Denise",
	"ECS Denise",
	"AGA Lisa  ",
};

VOID SetUp (BOOL mode)
{
	char Dummy[50];
	WORD unit, i;
	UBYTE gfx, display;
	ULONG gfxTypes;
	UWORD x1 = 8, y1 = 49;
//	struct Library *MMUBase;

	Status (GLS (&L_PLEASE_WAIT));
	filecheck_progress = FALSE;

	if (mode)	// UnIconify?
	{
		decrypt_flag = FALSE;

		InitInstallWindow ();
		InitRecordWindow ();

		vec = AllocMem (sizeof (Vectors), MEMF_CLEAR|MEMF_PUBLIC);
		tmp_vec = AllocMem (sizeof (Vectors), MEMF_CLEAR|MEMF_PUBLIC);

		view_unit = ERROR;
		for (i = 0; i < 5; i++)
			installFlag[i] = 0;
	}

	DrawImage (rp, &Statusimg, mainX, STATUSY - 9 + OFFY);
//	DrawImage (rp, &VE_image, mainX, 256);
	StyleIt2 (rp, mainX+STATUSX+60, 15 + OFFY, mainX+628,  32 + OFFY, WHITE, BLACK, STRING_STYLE);	// Status 
	StyleIt2 (rp, mainX+95,  40 + OFFY, mainX+628, 194 + OFFY, BLACK, WHITE, STRING_STYLE);	// Bootblock
	StyleIt2 (rp, mainX+x1-5 , 200 + OFFY, mainX+628, 246 + OFFY - diskShowY, BLACK, WHITE, GADGET_STYLE);	// df0:

	StyleIt2 (rp, mainX+x1-5,  y1 - 9 + OFFY, mainX+x1+82, y1 + 19 + OFFY, BLACK, WHITE, GADGET_STYLE);	// CPU, FPU, MMU
	StyleIt2 (rp, mainX+x1-5,  y1 + 22 + OFFY, mainX+x1+82, y1 + 58 + OFFY, BLACK, WHITE, GADGET_STYLE);	// GFX
	StyleIt2 (rp, mainX+x1-5,  113 + OFFY, mainX+x1+82, 128 + OFFY , BLACK, WHITE, GADGET_STYLE);	// Clock
	StyleIt2 (rp, mainX+x1-5, FINDY - 9 + OFFY, mainX+x1+82, FINDY + 27 + OFFY, BLACK, WHITE, GADGET_STYLE);	// Viruscount

	if (mode)
	  StartTimer (memcheck_delay, 0);

	SetAPen (rp, BLACK);

/***************************** CPU *******************************/
	JEOWrite (rp, mainX + x1, y1 + OFFY, "CPU:", WHITE);
	SPrintF (Dummy, "%ld", cpu);
	JEOWrite (rp, mainX + x1 + (5 * 8) - 3, y1 + OFFY, Dummy, BLACK);

	y1 += 8;
/***************************** FPU *******************************/

	JEOWrite (rp, mainX + x1, y1 + OFFY, "FPU:", WHITE);

	fpu = 0;
	if (processorFlag & AFF_FPU40)	// Må sjekke om processor  er 060 eller 040!
	{
		if (processorFlag & AFF_68060)
			fpu = 68060;
		else if (processorFlag & AFF_68040)
			fpu = 68040;
		else if (processorFlag & AFF_68882)
			fpu = 68882;
		else if (processorFlag & AFF_68881)
			fpu = 68881;
	}
	else if (processorFlag & AFF_68882)
		fpu = 68882;
	else if (processorFlag & AFF_68881)
		fpu = 68881;

	if (fpu)
		SPrintF (Dummy, "%ld", fpu);
	else
		SPrintF (Dummy, "-----");

	JEOWrite (rp, mainX + x1 + (5 * 8) - 3, y1 + OFFY, Dummy, BLACK);

/***************************** MMU *******************************/

	y1 += 8;
	JEOWrite (rp, mainX + x1, y1 + OFFY, "MMU:", WHITE);

/*
	if (MMUBase = OpenLibrary ("mmu.library", NULL))
	{
		ULONG type;

		type = GetMMUType ();
		SPrintF (Dummy, "%ld\n", type);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_NO_SOUND);
		CloseLibrary (MMUBase);
	}
*/
	mmu = CheckMMU ();

	if (mmu == 68040)	// Har vi 060?
	{
		if (processorFlag & AFF_68060)
			mmu = 68060;
	}

	if (mmu)
		SPrintF (Dummy, "%ld", mmu);
	else
		SPrintF (Dummy, "-----");

	JEOWrite (rp, mainX + x1 + (5 * 8) - 3, y1 + OFFY, Dummy, BLACK);

	PrintTime ();
/************************** GRAFIKK ******************************

	$00 Std Agnus		// PAL
	$10 Std Agnus		// NTSC

	$20 ECS Agnus		// PAL
	$30 ECS Agnus		// NTSC

	$21 ECS Agnus		// PAL
	$31 ECS Agnus		// NTSC

	$22 AGA Alice		// PAL
	$32 AGA Alice		// NTSC

	$23 AGA Alice		// PAL
	$33 AGA Alice		// NTSC

Fat Agnus 8370 NTSC			A500, A2000 
Fat Agnus 8371 PAL			A500, A2000 
Super Agnus 8372 1 MEG 	A2000 
Fat Agnus 8372A 1 MEG 	CDTV 
Fat Agnus 8372 1 MEG 		???? 
Fat Agnus 8372 2 MEG 		A3000 
Fat Agnus 8375 2 MEG 		A600, CDTV-II 
Fat Agnus 8375 ? MEG 		A2000 

Paula 8364 							A2000, A3000 
Paula 8364 R7 					A500, CDTV, A3000 
*/

	gfxTypes = GetGFXTypes ();
	gfx = gfxTypes>>16;
	display = (UWORD)gfxTypes;

	y1 += 15;
	JEOWrite (rp, mainX + x1, y1 + OFFY, "GFX:", WHITE);
	SPrintF (Dummy, "$%02lx", gfx);
	JEOWrite (rp, mainX + x1 + 50, y1 + OFFY, Dummy, BLUE);
	switch (gfx)
	{
		case 0x00: strcpy (Dummy, ChipSetName[0]); break;	// PAL NORMAL AGNUS (8371) 
		case 0x10: strcpy (Dummy, ChipSetName[0]); break;	// NTSC NORMAL AGNUS 512kb

		case 0x20: strcpy (Dummy, ChipSetName[1]); break;	// ESC PAL AGNUS 2mb
		case 0x30: strcpy (Dummy, ChipSetName[1]); break;	// ESC NTSC AGNUS 2mb
		case 0x21: strcpy (Dummy, ChipSetName[1]); break;	// ESC PAL AGNUS 2mb
		case 0x31: strcpy (Dummy, ChipSetName[1]); break;	// ESC NTSC AGNUS 2mb

		case 0x22: strcpy (Dummy, ChipSetName[2]); break;	// AA PAL ALICE
		case 0x32: strcpy (Dummy, ChipSetName[2]); break;	// AA NTSC ALICE
		case 0x23: strcpy (Dummy, ChipSetName[2]); break;	// AA PAL ALICE
		case 0x33: strcpy (Dummy, ChipSetName[2]); break;	// AA NTSC ALICE


//		case 0x01: strcpy (Dummy, "AGNUS 1MB "); break;	// (8372A)
//		case 0x02: strcpy (Dummy, "AGNUS 2MB "); break;	// (8372B)

		default:   SPrintF (Dummy, "$%lX ???  ", gfx); break;
	}
	y1 += 8;
	JEOWrite (rp, mainX + x1, y1 + OFFY, Dummy, BLACK);
/**************************  VIDEO  ******************************

	$FF Std Denise
	$FC ECS Denise
	$EC ECS Denise
	$F8 AGA Lisa

Denise 8362 R6 					A500, A2000 
Denise 8362 R8 					A500, CDTV, A3000 
ECS Denise 8373 				A3000 
ECS Denise 							????
*/

	y1 += 8;
	JEOWrite (rp, mainX + x1, y1 + OFFY, "VIDEO:", WHITE);
	SPrintF (Dummy, "$%02lx", display);
	JEOWrite (rp, mainX + x1 + 50, y1 + OFFY, Dummy, BLUE);
	switch (display)
	{
		case 0x00: strcpy (Dummy, ChipSetName[3]); break; // Normal NTSC Denise
		case 0xFF: strcpy (Dummy, ChipSetName[3]); break; // PAL/NTSC (8362)
		case 0xFC: strcpy (Dummy, ChipSetName[4]); break;	// PAL/NTSC (8373)
		case 0xEC: strcpy (Dummy, ChipSetName[4]); break;
		case 0xF8: strcpy (Dummy, ChipSetName[5]); break;	// PAL/NTSC (?)
		default:   SPrintF (Dummy, "$%lX ???  ", display); break;
	}
	y1 += 8;
	JEOWrite (rp, mainX + x1, y1 + OFFY, Dummy, BLACK);

/*************************  VIRUSCOUNT ***************************/
	JEOWrite (rp, mainX + x1, FINDY + OFFY, GLS (&L_BB_VIRUS), WHITE);
	JEOWrite (rp, mainX + x1, FINDY + 16 + OFFY, GLS (&L_FILE_VIRUS), WHITE);
	UpDateVirusCount ();
/***************************  BOOT  ******************************/

	if (mode)
	{
		for (unit = 0; unit < 4; unit++)
		{
			if (drive[unit] != ERROR)
			{
				ClearDiskBuffer (unit);
				JEOWrite (rp, mainX + DISKX, diskY[unit] + OFFY, Drive[unit], WHITE);
				DiskStatus (GLS (&L_PLEASE_WAIT), unit);
			}
		}
	}
	else
	{
		for (unit = 0; unit < 4; unit++)
		{
			if (drive[unit] != ERROR)
			{
				JEOWrite (rp, mainX + DISKX, diskY[unit] + OFFY, Drive[unit], WHITE);
				DiskStatus (DiskText[unit], unit);
			}
		}
	}

	ViewBoot (last_boot); //	Print out unit

	if (mode)
	{
		if (!LoadPatchData ())	// Error from sub
			CleanUp ();
//		FindMemList ();

#if _DEBUG1
		JEOSoundRequest ("DEBUG", "CheckMemory ()", GLS (&L_OK), S_MESSAGE);
#endif
		CheckMemory (OFF);
		All (OFF);

#if _DEBUG1
		JEOSoundRequest ("DEBUG", "s_fib_Open ()", GLS (&L_OK), S_MESSAGE);
#endif
	  s_fib_Open ();
#if _DEBUG1
		JEOSoundRequest ("DEBUG", "s_LoadFiles ()", GLS (&L_OK), S_MESSAGE);
#endif
		s_LoadFiles ();

		SPrintF (patch_Version, "%ld.%02ld", patchBrainVersion, patchBrainRevision);
	}
}

char PrefsName[300];

VOID LoadPrefs (VOID)
{
	BPTR fh = 0;
	LONG size;
	BOOL flag;

	if (!(prefs = AllocMem (sizeof (Prefs), MEMF_CLEAR)))
	{
		JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NO_MEM), GLS (&L_OK), S_ERROR);
		CleanUp ();
	}

	flag = FALSE;
	SPrintF (PrefsName, "%s%s.prefs", VEAssign, VE);
	size = FileSize (PrefsName);
	if (size == sizeof (Prefs))
	{
		if (fh = Open (PrefsName, MODE_OLDFILE))
		{
			Read (fh, prefs, sizeof (Prefs));
			Close (fh);
			flag = TRUE;
		}
	}
	else	// Sjekker i S: skuffen oxo...
	{
		flag = FALSE;
		SPrintF (PrefsName, "S:%s.prefs", VE);
		size = FileSize (PrefsName);
		if (size == sizeof (Prefs))
		{
			if (fh = Open (PrefsName, MODE_OLDFILE))
			{
				Read (fh, prefs, sizeof (Prefs));
				Close (fh);
				flag = TRUE;
			}
		}
		if (flag)
		{
			SavePrefs ();
			DeleteFile (PrefsName);
		}
	}

	if (!flag)	// Kunne ikke hente prefs
	{
		SPrintF (PrefsName, "%s%s.prefs", VEAssign, VE);
		if (fh = Open (PrefsName, MODE_NEWFILE))
		{
			prefs->check_crunched = ON;
			prefs->overwrite_crunched_files = OFF;
			prefs->skip_encrypted = ON;
			prefs->checkData = ON;
			prefs->extract_archives = ON;
			prefs->n_a1 = OFF;
			prefs->n_a2 = OFF;
			prefs->skip_days = OFF;
			prefs->days_count = 0;
			prefs->fast_file_checking = FALSE;

			prefs->hide_known_patches = ON;
			prefs->fast_startup = ON;

			prefs->wb_flag = OFF;

			prefs->fw_include_data = OFF;
			prefs->fw_exclude_icons = ON;
			prefs->fw_confirm = ON;
			prefs->fw_count = 0;

			prefs->logging = OFF;
			prefs->iconify = OFF;

			prefs->dump = 0;	// HEX mode
			prefs->address_mode_flag = 1;	// Offsett
			prefs->xfdUpdateMD5[0] = 0;
			prefs->xadUpdateMD5[0] = 0;

			strcpy (prefs->Log_path, VEAssign);

			if (*prefs->Log_path)
			{
				if (prefs->Log_path[strlen (prefs->Log_path)-1] == '/')
					prefs->Log_path[strlen (prefs->Log_path)-1] = 0;
			}

			Write (fh, prefs, sizeof (Prefs));
			Close (fh);
		}
	}
	wb_flag = prefs->wb_flag;
	if (!*prefs->Log_path)
		strcpy (prefs->Log_path, VEAssign);
}

VOID SavePrefs (VOID)
{
	BPTR fh = 0;

	if (prefs)	// Could be called by CleanUp from LoadPrefs AllocMem Err
	{
		if (CheckDiskState (VEAssign))	// Writeable?
		{
			SPrintF (PrefsName, "%s%s.prefs", VEAssign, VE);
//		Status (GLS (&L_SAVING_PREFERENCES));
			if (fh = Open (PrefsName, MODE_NEWFILE))
			{
				Write (fh, prefs, sizeof (Prefs));
				Close (fh);
			}
		}
	}
}

#if _JEO_DEBUG
	if (1)
	{
		BPTR fh = 0;
		int i;
		struct Node *nd;

		if (fh = Open ("RAM:List", MODE_NEWFILE))
		{
		  nd = viruslist->xvsvl_List.lh_Head;
			for (i = 0; i < viruslist->xvsvl_Count; i++)
			{
				SPrintF (Dummy, "%s\n", nd->ln_Name);
				Write (fh, Dummy, strlen (Dummy));
				nd = nd->ln_Succ;
			}
			Close (fh);
		}
	}
#endif

VOID SelectScreenMode (BOOL mode)	// 0 = oppstart 1 = under kjøring
{
	struct rtScreenModeRequester *scrmodereq;

	if (scrmodereq = rtAllocRequestA (RT_SCREENMODEREQ, NULL))
	{
		if (rtScreenModeRequest (scrmodereq, GLS (&L_SELECT_SCREEN_MODE),
											 	RTSC_Flags, SCREQF_SIZEGADS|
												SCREQF_AUTOSCROLLGAD|SCREQF_OVERSCANGAD,
												RT_ReqPos, REQPOS_CENTERSCR, TAG_END))
		{
			if ((scrmodereq->DisplayWidth < 640) OR (scrmodereq->DisplayHeight < 256))
				JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_SMALL_SCREEN_SIZE), GLS (&L_OK), S_ERROR);
			else
			{
				CopyMem (scrmodereq, &prefs->screenmode, sizeof (struct rtScreenModeRequester));
				SavePrefs ();
				if (mode == 1)	// Er vi inne i progget???
				{
					Iconify ();
					UnIconify ();
				}
				else
					mode = 1;	// Vi er ikke inne i progget men fortsetter allikevel...
			}
		}
		else
		{
			if (mode == 0)	// Oppstart??
			{
				JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NO_PICK_SCREEN), GLS (&L_SHIT), S_ERROR);
				prefs->screenmode.DisplayID = 0x29000;
				prefs->screenmode.DisplayWidth = 640;
				prefs->screenmode.DisplayHeight = 256;
				prefs->screenmode.AutoScroll = 0x4000;
				prefs->screenmode.OverscanType = 0;
			}
		}
	}
	rtFreeRequest (scrmodereq);
}

VOID TimerCheck (VOID)
{
	static BOOL flag = FALSE;
	WORD col;

	if (aminet_flag)
		return;

	if (!icon)
	{
		PrintTime ();

		if (!flag)
			col = BLUE;
		else
			col = BLACK;

		JEOWrite (rp, mainX + 29 + 16, 123+OFFY, ":", col);

		flag ^= 1;
	}

	CheckMemory (OFF);
}

BOOL OpenIcon (VOID)
{
	if (iconport = CreateMsgPort())
	{
		iconsig = 1L << iconport->mp_SigBit;

		SPrintF (Dummy, "%s%s", VEAssign, VERealName);
		dobj = GetDiskObjectNew (Dummy);
		if (dobj)
		{
			char Hold[17];

			SPrintF (Hold, "-%s-", VE);
			if (dcicon = AddAppIconA (0, 0, Hold, iconport, 0, dobj, 0))
			{
				icon = TRUE;
				return (icon);
			}

			FreeDiskObject (dobj);
		}
		DeleteMsgPort (iconport);
	}
	iconport = 0;
	return (icon);
}

VOID CloseIcon (VOID)
{
	if (iconport AND icon)
	{
		RemoveAppIcon (dcicon);
		FreeDiskObject (dobj);
		DeleteMsgPort (iconport);
		icon = FALSE;
	}
}

struct MyMessage
{
	struct Node mn_Node;
  struct MsgPort *mn_ReplyPort;
  UWORD  mn_Length;
  char   *Command, *Args;
};

//ULONG stacksize;
struct Process *myproc;


/*
VOID __asm far FuncOutput (register __a3 ULONG *ptr)
{
	char *Process;
	char *LibName;
	WORD function;
	char Text[100];
	char Hold[100];
	struct Library *IntBase;

	Process = (char *)*ptr;
	SPrintF (Text, " Process: %s\n", Process);

	ptr++;
	function = *ptr * -1;
	SPrintF (Hold, "Function: %ld\n", function);
	strcat (Text, Hold);

	ptr++;
	LibName = (char *)*ptr;
	SPrintF (Hold, " Library: %s\n\n", LibName);
	strcat (Text, Hold);

	if (IntBase = OpenLibrary ("intuition.library", 0))
	{
		DisplayBeep (NULL);
		CloseLibrary (IntBase);
	}

//	JEOSoundRequest (GLS (&L_MESSAGE), Text, GLS (&L_OK), S_MESSAGE);
}
*/

VOID LoadSounds (VOID)
{
	BPTR fh;
	char SoundsName[208];

	SPrintF (SoundsName, "%s%s.sounds", VEAssign, VE);
	if (FileSize (SoundsName) == SOUND_LEN)
	{
		SoundBuffer = AllocMem (SOUND_LEN, MEMF_CHIP);
		if (fh = Open (SoundsName, MODE_OLDFILE))
		{
			if (Read (fh, SoundBuffer, SOUND_LEN) == SOUND_LEN)
			{
				if (AllocAudio (1))
					soundplay = ON;
			}
			Close (fh);
		}
	}
} 

VOID main_END (VOID);

VOID GetOSVersionNumber (ULONG ROMver, ULONG start, char *OSVerNumber)
{
  UBYTE *ptr;
  ULONG i, j;

	if (ROMver == ROM_37_175)
		strcpy (OSVerNumber, "2.04");
	else if (ROMver >= ROM_37_299 AND ROMver <= ROM_37_350)
		strcpy (OSVerNumber, "2.05");
	else // 3.00++
	{
		for (i = start + 100, j = 0; i < start + 200; i++)
	  {
  	  ptr = (UBYTE *)i;
    	if (isdigit(*ptr) OR *ptr == '.')
	    {
  	    if (*ptr == '.' AND j == 0)
    	    continue;
      	OSVerNumber[j] = *ptr;
	      j++;
  	  }
    	else if (j > 0)
      	break;
	  }
  	OSVerNumber[j] = 0;
  }
}

ULONG GetKickStartVersion (ULONG *ROM_start)
{
	ULONG *p;

	p = ROM_start;
	p += 3;
	return (*p);
}

VOID Do_Kicks (VOID)
{
	ULONG hw_ROM_data, ROM_data;
	ULONG i, rom;
	ULONG *p;
	UWORD *p1;

	p = (ULONG *)0x00FFFFEC;
	hw_ROM_size = *p;
	hw_ROM_start = 0x01000000 - hw_ROM_size;
	hw_ROM_end = hw_ROM_start + hw_ROM_size;

	hw_ROM_data = GetKickStartVersion ((ULONG *)hw_ROM_start);
	hw_ROM_version = hw_ROM_data >> 16;
	hw_ROM_revision = (UWORD)hw_ROM_data;
 	GetOSVersionNumber (hw_ROM_data, hw_ROM_start, HW_OSVer);

//	printf ("\n  Found HW-ROM at $%08lX - $%08lX (%ldKb)\n", hw_ROM_start, hw_ROM_end - 1, hw_ROM_size / 1024);
//	printf ("  HW ROM: %s (%ld.%ld)\n\n", HW_OSVer, hw_ROM_version, hw_ROM_revision);

// *********************************** SOFTROM ***************************************

	ROM_start = soft_ROM_flag = FALSE;
	p1 = (UWORD *)GetSoftROMAddress ();

	if (hw_ROM_start != (ULONG)p1)	// Do we really have a softrom?
	{
	  if (ValidMem((ULONG *)p1))
		{
			rom = (ULONG)p1;
			for (i = rom; i < rom + 0x10000; i++, p1++)
			{
				if (ValidMem ((ULONG *)p1))
				{
					if (*p1 == 0x1114)
					{
						UWORD *p;

						p = p1;
						p++;
						if (*p++ == 0x4ef9)
						{
							if (*p++ == i >> 16)
							{
								p++;
								if (*p++ == 0x0000)
								{
									if (*p++ == 0xffff)
									{
										ROM_start = (ULONG)p1;
										soft_ROM_flag = TRUE;
								
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (ROM_start)
	{
		ROM_data = GetKickStartVersion ((ULONG *)ROM_start);
		ROM_version = ROM_data >> 16;
		ROM_revision = (UWORD)ROM_data;
	 	GetOSVersionNumber (ROM_data, ROM_start, OSVer);

//		printf ("\n  Found kickstart at $%08lX\n", ROM_start);
//		printf ("  Kickstart: %s (%ld.%ld)\n\n", OSVer, ROM_version, ROM_revision);
	}
	else
	{
		ROM_version = hw_ROM_version;
		ROM_revision = hw_ROM_version;
		ROM_start = hw_ROM_start;
		ROM_end = hw_ROM_end;
		ROM_size = hw_ROM_size;
	}

	SPrintF (HW_ROMVersion, "%s (%ld.%ld)", HW_OSVer, hw_ROM_version, hw_ROM_revision);
	SPrintF (ROMVersion, "%s (%ld.%ld)", OSVer, ROM_version, ROM_revision);
}

//VOID __stdargs __main (char *Line)
VOID main (int argc, char **argv)
{
	BOOL aminet_file = FALSE;
	UBYTE *AminetBuf;
	LONG aminet_size;

	LONG unit;
	struct IntuiMessage *message;
	ULONG class;
	USHORT Code;
	APTR Address;
	ULONG signals;
  ULONG portsig, timersig;
	struct MyMessage *mess;
	UWORD y;
	char FileName[300];
	struct Resident *r;
#if _JEO_DEBUG
	UWORD mouseX, mouseY;
#endif

	restart_flag = FALSE;
//	printf ("%08lX %08lX %ld\n", main, main_END, (ULONG)main_END - (ULONG)main);

  SysBase = (*((struct ExecBase **) 4));

	processorFlag = SysBase->AttnFlags;
	if (processorFlag & AFF_68060)
		cpu = 68060;
	else if (processorFlag & AFF_68040)
		cpu = 68040;
	else if (processorFlag & AFF_68030)
		cpu = 68030;
	else if (processorFlag & AFF_68020)
		cpu = 68020;
	else if (processorFlag & AFF_68010)
		cpu = 68010;
	else
		cpu = 68000;

	myproc = (struct Process *)(SysBase->ThisTask);
	NameFromLock (myproc->pr_HomeDir, VEAssign, 300);	// Vi finner dirnavnet!
 	if ((VEAssign[strlen (VEAssign) - 1] != ':') AND (VEAssign[strlen (VEAssign) - 1] != '/'))
  	strcat (VEAssign, "/");

	if (myproc->pr_TaskNum)	// From shell?
	{
		UWORD i;

		strcpy (VERealName, argv[0]);

		ReverseString (VERealName);
		for (i = 0; VERealName[i] != 0; i++)
		{
			if ((VERealName[i] == '/') OR (VERealName[i] == ':'))
			{
				VERealName[i] = 0;
				break;
			}
		}
		ReverseString (VERealName);
	}
	else
		strcpy (VERealName, myproc->pr_Task.tc_Node.ln_Name);

	SPrintF (FileName, "%s%s", VEAssign, VERealName);

	Do_Kicks ();

	if (ROM_version < 37)
	{
		JEOEasyRequest (NULL, GLS (&L_MESSAGE), "VirusExecutor requires kickstart v37+!", GLS (&L_OK), NULL);
		exit (0);
	}

	exec_start = 0;
	exec_end = 0;

	if (r =  FindResident ("exec.library"))
	{
		if (r->rt_Version >= 45)	// BB2 exec
		{
			exec_start = (ULONG)r;
			exec_end = (ULONG)r->rt_EndSkip;
			exec_size = exec_end - exec_start;

//			printf ("%08lX - %08lX, %ld\n", exec_start, exec_end, exec_size);

			exec_md5 = Calculate_MD5 ((char *)exec_start, exec_size);
		}
	}

	morphOS_flag = (ULONG)FindResident ("MorphOS");

	if (!Exists (VEAssign))
		exit (0);

	recCount = 0;
	aminet_flag = FALSE;
	icon = FALSE;
	mystrcpy (AminetFileName, "*");
	if (argc >= 2)
	{
		ULONG success;
		UBYTE i;
		BPTR lock;
		BOOL error = TRUE;
		BOOL do_dir_flag = TRUE;
		char Dir[400];

		mystrcpy (AminetFileName, argv[1]);

		GetCurrentDirName (Dir, 400);
		for (i = 0; AminetFileName[i] != 0; i++)
		{
			if (AminetFileName[i] == ':' OR AminetFileName[i] == '/')
			{
				do_dir_flag = FALSE;
				break;
			}
		}

		if (do_dir_flag)
		{
			if (*Dir)
			{
				if (Dir[i] != ':' OR Dir[i] != '/')
					strcat (Dir, "/");
				strcat (Dir, AminetFileName);
				strcpy (AminetFileName, Dir);
			}
		}

		if (lock = Lock (AminetFileName, ACCESS_READ))
		{
			struct FileInfoBlock *fib = 0;

		  fib = AllocMem (sizeof (struct FileInfoBlock), MEMF_CLEAR);
			if (success = Examine (lock, fib))
			{
				if (fib->fib_DirEntryType < 0)	// A file?
					aminet_file = TRUE;
				else
				{
					if (AminetFileName[strlen (AminetFileName) - 1] != ':' AND AminetFileName[strlen (AminetFileName) - 1] != '/')
						strcat (AminetFileName, "/");
					aminet_flag = TRUE;
					error = FALSE;
				}
			}
			FreeMem (fib, sizeof (struct FileInfoBlock));
			UnLock (lock);
		}
		if (aminet_file)	// Check only one file!
		{
			BPTR fh;
			
			error = TRUE;
			aminet_size = FileSize (AminetFileName);
			if (aminet_size > 0)
			{
				if (fh = Open (AminetFileName, MODE_OLDFILE))
				{
					if (AminetBuf = AllocMem (aminet_size, MEMF_PUBLIC))
					{
						if (aminet_size == Read (fh, AminetBuf, aminet_size))
						{
							extern BOOL main_stop, main_check_stop, multi_check;

							error = FALSE;
							main_stop = main_check_stop = multi_check = FALSE;
							aminet_flag = TRUE;
						}
					}
					Close (fh);
				}
			}
		}
		if (error)
		{
			char Hold[256];

			SPrintF (Hold, "ERROR: Opening '%s'!", AminetFileName);
			PF (Hold);
			exit (0);
		}
	}

	if (!aminet_flag)
	{
		struct MsgPort *port, *rport;
    struct MyMessage *mess;
    char Hold[] = "SCREENTOFRONT";
    BOOL flag = FALSE;

		if (mess = (struct MyMessage *)AllocVec (sizeof(struct MyMessage), MEMF_PUBLIC))
    {
			mess->mn_Length = sizeof(struct MyMessage);

			if (mess->Command = (char *) AllocVec (strlen (Hold)+1, MEMF_PUBLIC))
				strcpy (mess->Command, Hold);
			if (mess->Args = (char *) AllocVec (strlen (AminetFileName)+1, MEMF_PUBLIC))
				strcpy (mess->Args, AminetFileName);

			rport = CreateMsgPort ();
			mess->mn_ReplyPort=rport;
		}
    Forbid();
    if (port = FindPort (VE))
    {
      PutMsg (port, (struct Message *)mess);
      WaitPort (rport);
      flag = TRUE;
		}
    Permit ();
    DeleteMsgPort (rport);
    FreeVec (mess->Command);
    FreeVec (mess->Args);
    FreeVec (mess);

		if (flag)
	    exit (0);
	}

	if (!(Dummy = AllocMem (10000, MEMF_CLEAR)))
		CleanUp ();

	enforcer_flag = mmu = FALSE;
	soundplay = OFF;
	counter = OFF;
	s_save_flag = OFF;

	if (!aminet_flag)
	{
		last_boot = FALSE;

	  if (!(ve_port = CreateMsgPort()))
		{
			JEOEasyRequest (NULL, GLS (&L_MESSAGE), "Couldn't allocate message ports!", GLS (&L_OK), NULL);
  	  CleanUp ();
	  }
  	ve_port->mp_Node.ln_Name = VE;
	  AddPort (ve_port);
  	portsig = 1L << ve_port->mp_SigBit;

		if (localeBase = (struct LocaleBase *)OpenLibrary ("locale.library", 37))
		{

//			printf ("%s %s\n", localeBase->loc_LocaleName, localeBase->loc_LanguageName);
			SPrintF (Dummy, "%s.catalog", VE);
			mycatalog = OpenCatalogA (NULL, Dummy, NULL);
			if (mycatalog)
			{
				mystrcpy (Cat_Language, mycatalog->cat_Language);
				sprintf (Cat_Version, "%ld.%ld", mycatalog->cat_Version, mycatalog->cat_Revision);
			}
			else
			{
				strcpy (Cat_Language, "english");
				strcpy (Cat_Version, ProgramVersion);
			}
		}
		else
			JEOEasyRequest (NULL, GLS (&L_MESSAGE), "Error: Couldn't open locale.library!", GLS (&L_OK), NULL);
		for (unit = 0; unit < 4; unit++)
		{
			Buffer[unit] = 0;
			if (!(Buffer[unit] = (UBYTE *)AllocMem (BUFSIZE, MEMF_CHIP | MEMF_PUBLIC)))
				CleanUp ();
		}

		if (!(pr = (struct Process *)FindTask (0)))
			CleanUp ();
		oldwinptr = pr->pr_WindowPtr;
		if (!(pr->pr_FileSystemTask = DeviceProc ("RAM")))
			CleanUp ();
	}

	reqtools_version = asl_version = 0;
	if (!(ReqToolsBase = (struct ReqToolsBase *)OpenLibrary (REQTOOLSNAME, REQTOOLSVERSION)))
	{
		SPrintF (Dummy, GLS (&L_VE_LIB_NEED), REQTOOLSNAME, REQTOOLSVERSION);
		JEOEasyRequest (NULL, GLS (&L_ERROR), Dummy, GLS (&L_OK), NULL);
		CleanUp ();
	}
	reqtools_version = FindLibVersion ((struct Library *)ReqToolsBase);

	if (AslBase = (struct AslBase *)OpenLibrary (AslName, 0))
		asl_version = FindLibVersion ((struct Library *)AslBase);

	if (!aminet_flag)
	{
		if (Check_checksum (FileName))
			JEOSoundRequest (GLS (&L_WARNING), 	GLS (&L_VE_MODIFIED), GLS (&L_OK), S_NO_SOUND);
	}
	if (!(GadToolsBase = OpenLibrary ("gadtools.library", 0)))
		CleanUp ();
	if (!(UtilityBase = OpenLibrary ("utility.library", 0)))
		CleanUp ();

	LoadPrefs ();
	prefs->hide_known_patches = ON;
	prefs->fast_startup = ON;

	xfdmaster_flag = TRUE;
	if (!(xfdMasterBase = (struct xfdMasterBase *) OpenLibrary (xfdmaster_name, NULL)))
	{
		SPrintF (Dummy, GLS (&L_VE_NEEDS_XFDMASTER), VE, xfdmaster_name);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
		xfdmaster_flag = FALSE;
		prefs->check_crunched = FALSE;
	}
	xadmaster_flag = TRUE;
	if (!(xadMasterBase = (struct xadMasterBase *) OpenLibrary (xadmaster_name, 9)))
	{
		SPrintF (Dummy, GLS (&L_VE_NEEDS_XADMASTER), VE, xadmaster_name, 9, xadmaster_name);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
		xadmaster_flag = FALSE;
	}

	if (!(xvsBase = OpenLibrary (xvs_name, XVS_VERSION)))
	{
		SPrintF (Dummy, GLS (&L_VE_LIB_NEED), xvs_name, XVS_VERSION);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
		CleanUp ();
	}
	if (!xvsSelfTest ())
	{
		JEOSoundRequest (GLS (&L_WARNING), GLS (&L_XVS_MODIFIED), GLS (&L_OK), S_ERROR);
		CleanUp ();
	}
	if (!(bootinfo = xvsAllocObject (XVSOBJ_BOOTINFO)))
	{
		JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NO_MEM), GLS (&L_OK), S_ERROR);
		CleanUp ();
	}
	if (!(sectorinfo = xvsAllocObject (XVSOBJ_SECTORINFO)))
	{
		JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NO_MEM), GLS (&L_OK), S_ERROR);
		CleanUp ();
	}

	total_unpackers = 0;

	if (!aminet_flag)
	{
		LoadSounds ();
		if (!(TimerMP = CreatePort (0, 0)))
			CleanUp ();
		if (!(TimerIO = (struct timerequest *)CreateExtIO (TimerMP, sizeof (struct timerequest))))
			CleanUp ();
		if ((tdevice = OpenDevice (TIMERNAME, UNIT_VBLANK, (struct IORequest *)TimerIO, 0)))
			CleanUp ();

		if (!(diskport = (struct MsgPort *)CreatePort (NULL, NULL)))
			CleanUp ();
		if (!(diskreq = (struct IOExtTD*)CreateExtIO (diskport, sizeof (struct IOExtTD))))
			CleanUp ();
	}

	FindLibraryVersion (xvs_Version, xvsBase);
	if (xfdmaster_flag)
		FindLibraryVersion (xfd_Version, (struct Library *)xfdMasterBase);
	if (xadmaster_flag)
		FindLibraryVersion (xad_Version, (struct Library *)xadMasterBase);


	Check_libs (Last_XVS, xvs_Version, xvs_name);
	if (xfdmaster_flag)
		Check_libs (Last_XFD, xfd_Version, xfdmaster_name);


/*
	if (xadmaster_flag)
		Check_libs (Last_XAD, xad_Version, xadmaster_name);
*/
	if (aminet_flag)
	{
		prefs->check_crunched = ON;
		prefs->overwrite_crunched_files = OFF;
		prefs->skip_encrypted = OFF;
		prefs->checkData = ON;
		prefs->extract_archives = ON;
		prefs->n_a1 = OFF;
		prefs->n_a2 = OFF;
		prefs->skip_days = OFF;
		filestat = OFF;

		if (aminet_file)
		{
			MainCheck (AminetFileName, AminetBuf, aminet_size);
			if (AminetBuf)
				FreeMem (AminetBuf, aminet_size);
		}
		else
			CheckFiles (AminetFileName, 0);
		CleanUp ();
 	}

	if (prefs->screenmode.DisplayID == 0)	// Ingen Screenmode ennå...
		SelectScreenMode (0);	

// Init Drives!!!!!!!!!!!!!!!!!!!!!
	diskShowY = 0;
	y = DISKY;
	for (unit = 0; unit < 4; unit++)
	{
		if (!OpenDevice (TD_NAME, unit, (struct IORequest *)diskreq, NULL))
		{
			CloseDevice ((struct IORequest *)diskreq);
			diskY[unit] = y;
			y += 10;
		}
		else
		{
			drive[unit] = ERROR;
			diskShowY += 10;
			switch (unit)
			{
				case 0: VirusNewMenu[38].nm_Flags ^= NM_ITEMDISABLED; break;
				case 1: VirusNewMenu[39].nm_Flags ^= NM_ITEMDISABLED; break;
				case 2: VirusNewMenu[40].nm_Flags ^= NM_ITEMDISABLED; break;
				case 3:	VirusNewMenu[41].nm_Flags ^= NM_ITEMDISABLED; break;
			}
		}
	}

	if (SetupScreen ())
	{
		SelectScreenMode (0);	

		screenWidth = prefs->screenmode.DisplayWidth;
		mainX = (screenWidth - 640) / 2;
		screenHeight = prefs->screenmode.DisplayHeight;

		if (SetupScreen ())
			CleanUp ();
	}

	filestat = OFF;
	if (screenHeight >= (276 + OFFY))
	{
		filestat = ON;

		if (prefs->wb_flag)
			screenHeight = 276 + OFFY;
	}

	if (OpenVirusWindow ())
		CleanUp ();

//	strcat (ProgramVersion, CPU);
	SPrintF (Ver, "%s v%s - %s", VE, ProgramVersion, CopyRight);
	if (prefs->wb_flag)
		SetWindowTitles (VirusWnd, Ver, NULL);
	else
		SetWindowTitles (VirusWnd, NULL, Ver);

	rp = VirusWnd->RPort;

	textfont = OpenFont (&topaz8);
	if (textfont)
		SetFont (rp, textfont);

	pr->pr_WindowPtr = VirusWnd;
	menuFlag = ON;

	All (OFF);
	check_sum = 0;
	SetUp (1);

	if (prefs->logging)
		OpenLog ();

	rtSetWaitPointer (VirusWnd);
	if (!prefs->fast_startup)
	{
		Status (GLS (&L_PLEASE_WAIT));
		VE_About ();
	}
	SPrintF (DataName, "%s%s.data", VEAssign, VE);

#if _DEBUG1
		JEOSoundRequest ("DEBUG", "OpenOtherBoots ()", GLS (&L_OK), S_MESSAGE);
#endif
  OpenOtherBoots ();
#if _DEBUG1
		JEOSoundRequest ("DEBUG", "LoadBase ()", GLS (&L_OK), S_MESSAGE);
#endif
	LoadBase ();
#if _DEBUG1
		JEOSoundRequest ("DEBUG", "CheckingDrives", GLS (&L_OK), S_MESSAGE);
#endif
	Status (GLS (&L_CHECKING_DRIVES));
	for (unit = 0; unit < 4; unit++)
	{
		if (drive[unit] != ERROR)
		{
			if (!OpenDevice (TD_NAME, unit, (struct IORequest *)diskreq, NULL))
			{
				diskreq->iotd_Req.io_Command = TD_CHANGENUM;
				diskreq->iotd_Req.io_Flags = IOF_QUICK;
				DoIO ((struct IORequest *)diskreq);
				drive[unit] = diskreq->iotd_Req.io_Actual;

				if (DiskInDrive (diskreq))
				{
					CloseDevice ((struct IORequest *)diskreq);
					if (VE_ReadBoot (unit))
						VE_CheckBoot (unit);
				}
				else
				{
					CloseDevice ((struct IORequest *)diskreq);
					DiskStatus (GLS (&L_NO_DISK), unit);
				}
			}
		}
	}
	ClearIntuition (VirusWnd);

	if (prefs->version != PREFS_VERSION)
	{
		prefs->version = PREFS_VERSION;
		PrefsWindow ();
	}

	winsig = 1L<<VirusWnd->UserPort->mp_SigBit;
	timersig = 1L << TimerMP->mp_SigBit;
	
	Status (GLS (&L_READY));
	All (ON);

	if (prefs->iconify)
		Iconify ();

  FOREVER
	{
		if (icon) // ICONIFIED
			signals = Wait (iconsig | timersig | portsig | winsig);
		else
			signals = Wait (timersig | winsig | portsig);

		TimerMSG = GetMsg (TimerMP);
		if (TimerMSG == (struct Message *)TimerIO)
			counter = ON;
 
		if (signals & winsig)
		{
			if (!icon)
			{
				while (message = (struct IntuiMessage *)GetMsg (VirusWnd->UserPort))
				{
					class = message->Class;
					Code = message->Code;
					Address = message->IAddress;

#if _JEO_DEBUG
					mouseX = message->MouseX;
					mouseY = message->MouseY;
#endif					
					ReplyMsg ((struct Message *)message);

					switch (class)
					{
						case IDCMP_DISKINSERTED:
							All (OFF);
							DiskInserted ();
							All (ON);
							break;
						case IDCMP_DISKREMOVED:
							DiskRemoved ();
							break;
						case IDCMP_RAWKEY:
						{
							RawKey (Code);
							break;
						}
#if _JEO_DEBUG
						case IDCMP_MOUSEBUTTONS: 
						{
							if (Code == MIDDLEDOWN)
								Iconify ();
							VE_Debug (mouseX, mouseY);
							break;
						}
#endif
						case IDCMP_MENUPICK:
							MenuPick (Code);
							break;
						case IDCMP_CLOSEWINDOW:
							CleanUp ();
							break;
					}
					if (icon)
						break;
				}
			}
			else	// ICONIFIED!
			{
				while (message = (struct IntuiMessage *)GetMsg (IconWnd->UserPort))
				{
					class = message->Class;
					ReplyMsg ((struct Message *)message);

					switch (class)
					{
						case IDCMP_DISKINSERTED:
							DiskInserted ();
							break;
						case IDCMP_DISKREMOVED:
							DiskRemoved ();
							break;
					}
					if (!icon)
						break;
				}
			}
		}

		if (icon)
		{
			if (signals & iconsig)
			{
				struct AppMessage *appmsg;
				BOOL flag = FALSE;
				LONG numArgs;

				while (appmsg = (struct AppMessage *)GetMsg (iconport))
				{
					numArgs = appmsg->am_NumArgs;
					ReplyMsg ((struct Message *)appmsg);
					if (!numArgs)
						flag = TRUE;
				}
				if (flag)
					UnIconify ();
			}
		}
		if (signals & portsig)
		{
			char Command[50], Args[50];

			while (mess = (struct MyMessage *)GetMsg (ve_port))
			{
				strcpy (Command, mess->Command);
				strcpy (Args, mess->Args);
		    ReplyMsg ((struct Message *)mess);
		    if (!(strcmp (Command, "SCREENTOFRONT")))
		    {
		    	if (icon)
		    		UnIconify ();
		    	else
		    	{
		    		ScreenToFront (Scr);
		    		ActivateWindow (VirusWnd);
		    	}
	    	}
		  }
		}
		if (counter)
		{
			TimerCheck ();
		  StartTimer (memcheck_delay, 0);
			counter = OFF;
		}
	}
}

VOID main_END (VOID)
{}