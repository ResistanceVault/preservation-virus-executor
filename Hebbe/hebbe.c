#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/execbase.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <proto/layers.h>
#include <proto/icon.h>
#include <proto/mathieeesingbas.h>
#include <proto/mathffp.h>
#include <proto/keymap.h>
#include <proto/expansion.h>
#include <proto/wb.h>
#include "prototypes.h"

VOID FindCode (UWORD *address);
VOID InitPatches (VOID);

struct TextAttr topaz8 = {
	( STRPTR )"topaz.font", 8, 0x00, 0x01 };

char far PatchNames[VE_MAX_PATCHES][35] =
{
	"*** UNKNOWN VECTOR ***",
	"SnoopDos by Eddy Carroll",
	"AMOS Library",
	"68040 Library",
	"Alert Patch 1.x",
	"A2232/QuickFix by Chr. Buchner",
	"AddPower 37.6 by Ian J. Einman",
	"AddPower 95.4 by Ian J. Einman",
	"Alert Patch by David Swasbrook",
	"Arq by Martin J. Laubach",
	"AssignX 1.2 by Steve Tibbett",
	"Audio.Device 34.0 by JG",
	"AutoReboot 1.1 by Chris Johnsen",
	"BlitzFonts by Hayes Haugen",
	"BorderBlank",
	"BusyPointer 1.0 by Dan Blender",
	"CacheIt 1.0 by HoukY-Soft",
	"CheckDebug by Thomas Kessler",
	"CopyMemQuicker by Arthur Hagen",
	"CPUBlit Eddy Carroll",
	"CPUClr 2.0 by Unicorn",
 	"CPUClr by Carnivore/BeerMacht",
	"CPUClr by Peter Simons",
	"CrM Library by Thomas Schwarz",
	"CrM RTDD by Thomas Schwarz",
	"CycleToMenu by Frederico Giannici",
	"DiskCache 2.00 by Oliver Wagner",
	"DosPatch by Markus M. Wild",
	"DosPrefs by Norbert Püschel",
	"EasyReqPatch by M. Origgi",
	"Enforcer by Michael Sinz",
	"Enhanced Alert.Hook by MJSoft",
	"ExecPatch by Arthur Hagen",
	"Explode Library by A.J. Brower",
	"Facction by ASDG Inc.",
	"FasterBlit by Arthur Hagen",
	"FastFonts 1.1 by Charlie Heath",
	"FastMemCopy 1.04 by M. Berndt",
	"FastWaitBlit by M. Berndt",
	"FastWaitBlit by Dave Jones",
	"FixReq",
	"ForceIcon by Kai Iske",
	"FrontPubScreen by Nicola Salmoria",
	"GOMF by Christian Johnsen",
	"HD Floppy Driver by Adam Hill",
	"Immune 1.2 by Xavier Leclercq",
	"IPrefs",
	"KCommodity by Kai Iske",
	"LibReq 1.4 by Nick Salmoria",
	"LoadWB",
	"Locale Library",
	"LowFrag by Eric Sauvageau",
	"LVD by Peter Stuer",
	"Magic Menu by Martin Korndörfer",
	"MakeVPort Patch by S. Vigna",
	"MemMan2",
	"ModePro by Michael Rivers",
	"MultiCX by Martin Berndt",
	"MultiUser 1.8 by G. Uytterhoeven",
	"MungWall",
	"MWA 1.0 by Stefan Stuntz",
	"New Alert 36.3",
	"NewIcons",
	"NewMenu by Brian Gontowski",
	"NewMode by Andreas Linnemann",
	"NickPrefs 37.9",
	"NoCare by Raymond Hoving",
	"NOG2 2.1 by Daniel Weber",
	"NoReq by Steve Tibbett",
	"PatchAlert by C. Beskow",
	"PatchAllocMem by Simon Dick",
	"PatchControl by Stefan Sommerfeld",
	"PatchLoadSeg 1.20 by P.v.L.",
	"PatchOpenWorkBench 39.1",
	"PatchPointer by Steve Lemieux",
	"PointerX by Dave Jones",
	"PointerX by Steve Tibbett",
	"PopUpMenu by Martin Adrian",
	"PowerCache by Michael Berg",
	"PowerData by Michael Berg",
	"PowerSnap by Nico François",
	"PP Library by Nico François",
	"PP Patcher 1.4 by Michael Berg",
	"PPDRP 2.03 by CodX",
	"PPLoadSeg by Nico François",
	"Print Manager by Nicola Salmoria",
	"Prop3D by Massimo Tantignone",
	"ProWizard by Nicolas Franck",
	"QMouse 2.10 by Dan Babcock",
	"QuickMenus 1.0 by Brian Moats",
	"QuietTD",
	"Replex 1.0 by Ekke Verheul",
	"ReqChange by Magnus Holmgren",
	"ReqPatch 1.2 by Raul Sobon",
	"Reset1200 by Carsten Sørensen",
	"ROM Crack 0.70 by SCA",
	"RTPatch by Nico François",
	"Safer Patches by Martin Adrian",
	"SegTracker by Michael Sinz",
	"SetMan by Nicola Salmoria",
	"SetPatch",
	"SetPatchMrgCop by Dig. Creations",
	"SFAPatch",
	"SmartWB by Grzegorz Calkowski",
	"SwazInfo by David Swasbrook",
	"SwitchNTSC by Mario Kemper",
	"SysException",
	"SysIHack by Mr. BOOPSI",
	"TagScreens 1.6 by M. Berndt",
	"TempoTopaz 1.0 by Preben Nielsen",
	"TimedReqs by Darrell Grainger",
	"screennotify.library",
	"ToolsDaemon by Nico François",
	"Tron's Window Activator",
	"TurboTopaz 1.0 by Preben Nielsen",
	"UrouHack by Tinic Urou",
	"Virus Interceptor by J. Eliasson",
	"WBExtender by Martin Berndt",
	"WBGauge by Jean-Michel Forgeas",
	"Wreq by Tuomo Mickelsson",
	"WTFPatch by David Jenkins",
	"xfdPatch by Georg Hörmann",
	"xLoadSeg by Chr. Schneider",
	"Xoper by Werner Günther",
	"ZoomWindows 1.0 by Dr.Maybe",
	"cybergraphic.library",
	"cyberintuition.library",
	"*** Neurotic Death VIRUS ***",
	"serial.device",
	"audio.device",
	"68060 Library",
	"CyberPatcher",
	"Miami by Holger Kruse",
	"SmartCrash by PitPlane Productions",
	"lowlevel.library",
	"FastExec by Torbjörn Andersson",
	"SlowReset by Marco Krause",
	"BetterOpenLibs by Thomas Richter",
	"ShowMem by Thomas Richter",
	"PoolMem by Thomas Richter",
	"NoDelete by Denis GOUNELLE",
	"CMQ by Dirk Busse",
	"Snap by Mikael Karlsson",
	"IDE-fix by Oliver Kast",
	"HDSleep by Thom/Union",
	"Setpatch (NSDPatch)",
	"FBlit by Stephen Brookes",
	"QLayers",
	"FText by Rick Pratt",
	"cyberlayers.Library",
	"1230 SCSI Device",
	"PrintManager by Stephan Rupprecht",
	"DOSTrace by Peter Stuer",
	"Exp. Layers by Andreas Schildbach",
	"AssignZ by Tobias Abt",
	"MagicCX by Kai Iske",
	"PatchHidden by E.Hambuch",
	"CEDFix by ?",
	"CyReq by Georg Steger",
	"NaeGrayPatch by ?",
	"NCommPatch by Marius Mortensen",
	"PatchBoot by ?",
	"PatchInterleaved by ?",
	"PatchReplyMsg by ?",
	"PatchWB by ?",
	"PatchWork by Richard Körber",
	"SetPatch / NSD / ROM Update",
	"NoFastMem",
	"ReKickReset by Mark Knibbs",
	"mmu.library by Thomas Richter",
	"VisualPrefs by Massimo Tantignone",
	"dopus5.library",
	"DiskLED by Per Johansson",
	"MagicASL by Flavio Stanchina",
	"ReqAttack by Georg Steger",
	"IconTrace by Peter Stuer",
	"DynamicCache by Christopher Wolf",
	"CEDPatch by Preben Nielsen",
	"MungMem by Marcin Kielesinski",
	"ExtraInfo by Nick Christie",
	"VMM by Martin Apel",
	"Scala Multimedia by Scala AS",
	"MCP by ALiENDESiGN",
	"HSMathLibs by Matthias Henze",
	"RAWBInfo by Stephan Rupprecht",
	"PowerWindows by Georg Steger",
	"WindowDaemon by David Swasbrook",
	"NewWPA8 by Michael van Elst",
	"PatchWPA8 by Pawel Hernik",
	"Executive by Petri Nordlund",
	"BlazeWCP by Rick Pratt",
	"StackAttack by Georg Steger",
	"Birdie 2000 by Trond Werner Hansen",
	"ObtainBetterPens by Petter Nilsen",
	"ParallelPatch by Peter Simons",
	"SerialPatch by Peter Simons",
	"SystemPatch by Sante Nocciolino",
	"FastRAD by Torbjörn Andersson",
	"QuickLens by Mike Berro",
	"TitleShadow by Peter Gordon",
	"FastIPrefs by Hans-W. Schober",
	"TopazPatch by Cliff Earl",
	"BootControl by Oliver Kastl",
	"PatchMult64 by Dirk Busse",
	"fastlayers.library",
	"rtg.library",
	"WB2Fast by Jaroslav Pokorny",
	"PowerWB by Georg Steger",
	"CleverWin by Stephan Rupprecht",
	"CyberGuard by Ralph Schmidt",
	"MathLibsPatch by Jess Sosnoski",
	"FPPrefs by Massimo Tantignone",
	"HDOff by Gideon Zenz",
	"AppChange by Stefan Sommerfeld",
	"WinSpeed by Olly Koenders",
	"MagicFrames by Luke Elliott",
	"FakeTopaz by Andrija Antonijevic",
	"PatchRAM by Thomas Richter",
	"MuForce by Thomas Richter",
	"MuGuardianAngel by Thomas Richter",
	"DefIcons by Stephan Rupprecht",
	"SysTracker by Andrew Bell",
	"SnoopSetFunction by Michael Boese",
	"AslToRT by Thore Boeckelmann",
	"OutLineFont by Rolf Max Rotvel",
	"MPLS Patch by Lejardinier Olivier",
	"KeyReq by Georg Steger",
	"OldIcons by Kyzer/CSG"
};

BOOL hide_known_patches = TRUE;

ULONG max_entries;

ULONG ROMstart, ROMend;
ULONG ROMVersion;
char ROMVer[10];
char ROMVersions[20];
char Dummy[5000];
char Ok[] = "Ok";

ULONG check_sum;

extern struct ExecBase *SysBase;
struct ReqToolsBase *ReqToolsBase = 0;

typedef struct
{
	UWORD type,
	      name_off;
	WORD	function,
				offset;
	ULONG	data;
	
} VEPatch;

VEPatch far patch[5000];

// ***********************************************************************

VOID Do_mega_stuff (ULONG *p, struct Library *lib, WORD off)
{
	BPTR fh;
	char FileDir[500] = "SYS:UNKNOWN";
	char FileName[500];
	BPTR lock = 0;

	lock = CreateDir (FileDir);
	if (lock)
		UnLock (lock);
	
	sprintf (FileName, "%s/%s", FileDir, lib->lib_Node.ln_Name);
	lock = CreateDir (FileName);
	if (lock)
		UnLock (lock);

	sprintf (FileDir, "%s/Function %4ld", FileName, off);
	if (fh = Open (FileDir, MODE_NEWFILE))
	{
		ULONG i;

		for (i = 0; i < 256; i++)
		{
			Write (fh, &*p, 4);
			p++;
		}

		Close (fh);
	}
}

// ***********************************************************************



VOID CleanUp (VOID)
{
	if (ReqToolsBase)
		CloseLibrary ((struct Library *)ReqToolsBase);

	exit (0);
}

BOOL CheckValidAddress (ULONG address)
{
	if ((address < ROMstart OR address > ROMend) AND ((address != 0) AND (address != (ULONG)-1)))
		return (FALSE);
	else
		return (TRUE);
}

VOID FindLibraryVersion (char *String, APTR Lib)
{
	ULONG version;

	version = FindLibVersion (Lib);
	sprintf (String, "%ld.%ld", version>>16, (UWORD)version);
}

ULONG FindDeviceAddress (char *DeviceName)
{
  struct Node *node;
  ULONG dev_address = 0;

	Forbid ();
	node = SysBase->DeviceList.lh_Head;
	while (node->ln_Succ)
	{
		if (!(stricmp (DeviceName, node->ln_Name)))
		{
			dev_address = (ULONG)node;
			break;
		}
		node = node->ln_Succ;
	}
	Permit ();

	return (dev_address);
}

char PatchName[50];

ULONG CheckAddress (UWORD type, struct Library *lib, WORD function)
{
	ULONG *p;
	char *p_tmp;
	UWORD i;
	BOOL p_flag;
	ULONG val;

	p = (ULONG *)GetFunction (lib, function);

	check_sum += (ULONG)p;

	if ((ULONG)p < ROMstart OR (ULONG)p > ROMend)
	{
		if ((ULONG)p>>16 == 0x4E75)
			return (0);	// Ok

		p_flag = FALSE;

		for (i = 0; i < max_entries; i++)
		{
			if (patch[i].type == type)	// Er vi på riktig sted?
			{
				if (function == patch[i].function)	// Funnet et patch!
				{
					p_tmp = (char *)p;
					if (patch[i].offset != 0)
					{
						p_tmp += patch[i].offset;
						CopyMem ((APTR)p_tmp, &val, 4);
					}
					else
						CopyMem ((APTR)p_tmp, &val, 4);

					if (val == patch[i].data)
					{
						strcpy (PatchName, PatchNames[patch[i].name_off]);
						p_flag = TRUE;
						break;
					}
				}
			}
		}

		if (!p_flag)
			strcpy (PatchName, PatchNames[VE_UnknVecText]);

		return ((ULONG)p);
	}
	return (0);	// OK
}

VOID CheckVectors (UWORD type, struct Library *lib)
{
	WORD i;
	WORD max_func;
	ULONG p;
	char Type[50] = "???";

	max_func = GetMaxFunction (lib) *-1;
	printf ("\n$%08lX: %s %ld.%ld\n", lib, lib->lib_Node.ln_Name, lib->lib_Version, lib->lib_Revision);

	for (i = -6; i >= max_func; i -= 6)
	{
		p = CheckAddress (type, lib, i);
		if (p > 0)	// Ikke i ROM?
		{
		  printf ("  %5d (-$%03lX)  :  $%08X %s\n", i, i * -1, p, PatchName);
			if (!(strcmp (PatchName, PatchNames[VE_UnknVecText])))
			{
				switch (type)
				{
					case EXEC_LIB: strcpy (Type, "EXEC_LIB"); break;
					case DOS_LIB: strcpy (Type, "DOS_LIB"); break;
					case UTIL_LIB: strcpy (Type, "UTIL_LIB"); break;
					case GFX_LIB: strcpy (Type, "GFX_LIB"); break;
					case INT_LIB: strcpy (Type, "INT_LIB"); break;
					case GAD_LIB: strcpy (Type, "GAD_LIB"); break;
					case LAY_LIB: strcpy (Type, "LAY_LIB"); break;
					case ICON_LIB: strcpy (Type, "ICON_LIB"); break;
					case AUDIO_DEV: strcpy (Type, "AUDIO_DEV"); break;
					case CONSOLE_DEV: strcpy (Type, "CONSOLE_DEV"); break;
					case INPUT_DEV: strcpy (Type, "INPUT_DEV"); break;
					case SCSI_DEV: strcpy (Type, "SCSI_DEV"); break;
					case TIMER_DEV: strcpy (Type, "TIMER_DEV"); break;
					case TRACKDISK_DEV: strcpy (Type, "TRACKDISK_DEV"); break;
					case MATHIEEE_LIB: strcpy (Type, "MATHIEEE_LIB"); break;
					case GAMEPORT_DEV: strcpy (Type, "GAMEPORT_DEV"); break;
					case KEYBOARD_DEV: strcpy (Type, "KEYBOARD_DEV"); break;
					case RAMDRIVE_DEV: strcpy (Type, "RAMDRIVE_DEV"); break;
					case CARDDISK_DEV: strcpy (Type, "CARDDISK_DEV"); break;
					case ROMBOOT_DEV: strcpy (Type, "RAMBOOT_DEV"); break;
					case MATHFFP_LIB: strcpy (Type, "MATHFFP_LIB"); break;
					case KEYMAP_LIB: strcpy (Type, "KAYMAP_LIB"); break;
					case WB_LIB: strcpy (Type, "WB_LIB"); break;
					case EXPANSION_LIB: strcpy (Type, "EXPANSION_LIB"); break;
				}
				printf ("  InsertPatch (%s, %ld, ", Type, i);
				FindCode ((UWORD *)p);
				printf (", VE_Msg);\n");
			}

/*
			if (!(strcmp (PatchName, PatchNames[VE_UnknVecText])))
			{
			  printf ("  %5d (-$%03lX)  :  $%08X %s\n", i, i * -1, p, PatchName);
				Do_mega_stuff ((ULONG *)p, lib, i);	// Address, library, offsett
			}
*/
		}
	}
	printf ("\n");
}

VOID InsertPatch (UWORD type, WORD function, WORD offset, ULONG data, UWORD name_off)
{
	patch[max_entries].type = type;
	patch[max_entries].function = function;
	patch[max_entries].offset = offset;
	patch[max_entries].data = data;
	patch[max_entries].name_off = name_off;
	max_entries++;
}

ULONG MakeULONG (char *Id)
{
	ULONG id = 0;

	id += (ULONG)Id[0]<<24;
	id += (ULONG)Id[1]<<16;
	id += (ULONG)Id[2]<<8;
	id += (ULONG)Id[3]<<0;

	return (id);
}

ULONG MakeUWORD (char *Id)
{
	UWORD id = 0;

	id += (UWORD)Id[0]<<24;
	id += (UWORD)Id[1]<<16;

	return (id);
}

char CPUIntTxt[][20] =
{
	"Bus Error",
	"Address Error",
	"Illegal Instruction",
	"Division By Zero",
	"CHK / CHK2",
	"TRAPV",
	"Privilege Violation",
	"Trace Trap",
	"Line A Emulator",
	"Line F Emulator",
	"Coproc. Protocol",
	"Format Error",
	"Uninit. Interrupt",
	"Spurious Interrupt",
	"Level 1 Autovector",
	"Level 2 Autovector",
	"Level 3 Autovector",
	"Level 4 Autovector",
	"Level 5 Autovector",
	"Level 6 Autovector",
	"Level 7 Autovector"
};

VOID CheckIntCPU (VOID)
{
	ULONG *Int;
	ULONG i, j;
	ULONG vbr;
	UBYTE valid;

	vbr = GetVBR ();

	printf ("\nCPU Interrupt Vectors in RAM (VBR: $%08lx)\n\n", vbr);
  for (i = vbr + 0x08, j = 0; i <= vbr + 0x2C; i += 4, j++)
  {
    Int = (ULONG *)i;
		check_sum += (ULONG)*Int;
    printf ("  %-19s: $%08lX ", CPUIntTxt[j], *Int);
		valid = CheckValidAddress (*Int);

		if (!valid)	// Ikke i ROM?
		{
	    if (i == vbr + 0x08)	// Bus Error
  	  {
    		ULONG *p;
	
	  		p = (ULONG *)*Int;  	

				valid = FALSE;
				if (*p == 0x082f0002)
				{
	  			printf ("%s\n", PatchNames[VE_68060LibMsg]);
	  			valid = 2;
	  		}
				else
				{
		  		p += 2;

 					if (*p == 0x3e2f0048)
					{
		  			printf ("%s\n", PatchNames[VE_EnforcerMsg]);
		  			valid = 2;
	  			}
	  		}
	    }
	    else if (i == vbr + 0x2c)	// Line F Emulator
  	  {
    		ULONG *p;
	
	  		p = (ULONG *)*Int;  	
				if (*p == 0x0c6f202c)
				{
	  			printf ("%s\n", PatchNames[VE_68040LibMsg]);
	  			valid = 2;
	  		}
	  		else
	  			valid = FALSE;
	    }
	  }
		switch (valid)
		{
			case TRUE: printf ("%s\n", Ok); break;
  		case FALSE:	printf ("%s\n", PatchNames[VE_UnknVecText]); break;
  	}
	}
  for (i = vbr + 0x34; i <= vbr + 0x3c; i += 4, j++)
  {
    Int = (ULONG *)i;
		check_sum += (ULONG)*Int;
    printf ("  %-19s: $%08lX ", CPUIntTxt[j], *Int);

		valid = CheckValidAddress (*Int);

		switch (valid)
		{
			case TRUE: printf ("%s\n", Ok); break;
  		case FALSE:	printf ("%s\n", PatchNames[VE_UnknVecText]); break;
  	}
	}
  for (i = vbr + 0x60; i <= vbr + 0x7C; i += 4, j++)
  {
    Int = (ULONG *)i;
		check_sum += (ULONG)*Int;
    printf ("  %-19s: $%08lX ", CPUIntTxt[j], *Int);
		valid = CheckValidAddress (*Int);

		if (!valid)	// Ikke i ROM?
		{
	    if (i == vbr + 0x64)
  	  {
    		char *p;
    		ULONG val;
	
	  		p = (char *)*Int;  	
	  		p += 0x22;
				CopyMem ((APTR)p, &val, 4);
				if (val == 0x4eaefebc)
				{
	  			printf ("%s\n", PatchNames[VE_EnforcerMsg]);
	  			valid = 2;
	  		}
	  		else
	  			valid = FALSE;
	    }
	  }
		switch (valid)
		{
			case TRUE: printf ("%s\n", Ok); break;
  		case FALSE:	printf ("%s\n", PatchNames[VE_UnknVecText]); break;
  	}
	}
}

char ExecIntTxt[][12] =
{
	"Serial Out",
	"Disk Block",
	"Soft Int",
	"CIA A", 
	"Copper",    
	"Vert. Blank",
	"Blit Ready",
	"Audio Ch. 0",
	"Audio Ch. 1",
	"Audio Ch. 2",
	"Audio Ch. 3",
	"Serial In",
	"Disk Sync",
	"CIA B",
	"Int. Enable",
	"NMI"
};

VOID CheckIntRam (VOID)
{
	ULONG p, *exec, i;

	p = (ULONG)SysBase;
	exec = (ULONG *)p;

	exec += 22;

	printf ("\nExec interrupt vectors in RAM\n\n");
	for (i = 0; i < 16; i++)
	{
		check_sum += (ULONG)*exec;
		printf ("  %-11s: $%08lX ", ExecIntTxt[i], *exec);
		if (((ULONG)*exec < ROMstart OR (ULONG)*exec > ROMend) AND ((*exec != 0) AND (*exec != (ULONG)-1)))
			printf ("*** NON STANDARD VECTOR ***\n");
		else
			printf ("%s\n", Ok);
		exec += 3;
	}
}

#define MEGA (i * crypt_size - ((b + (c * 4)) ^ 6) / (2 + (crypt_size + 2) * 3) + (d * 3))
#define CRYPT		0
#define DECRYPT	1

VOID VE_Crypt (BOOL mode, UBYTE *Buffer, LONG crypt_size)
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

VOID SaveData (char *FileName)
{
	BPTR fh;
	ULONG ve_head;
	UWORD max_patches;

	ve_head = 0x56455041;	// "VEPA"
	if (fh = Open (FileName, MODE_NEWFILE))
	{
		UWORD i;

		Write (fh, &ve_head, 4);	// Header

		Write (fh, PatchBrain, PATCHBRAINSIZE);
		Write (fh, &patchBrainVersion, 2);
		Write (fh, &patchBrainRevision, 2);

		Write (fh, &max_entries, 4);	// # of known patches
		max_patches = VE_MAX_PATCHES;	// # of known patch programs
		Write (fh, &max_patches, 2);

		VE_Crypt (CRYPT, (char *)&PatchNames[0], VE_MAX_PATCHES * 35);
		Write (fh, &PatchNames[0], VE_MAX_PATCHES * 35);

		VE_Crypt (CRYPT, (UBYTE *)&patch[0], sizeof (VEPatch) * max_entries);
		for (i = 0; i < max_entries; i++)
			Write (fh, &patch[i], sizeof (VEPatch));

		Close (fh);
	}
}

VOID CheckDeviceVectors (VOID)
{
  ULONG dev_address = 0;

	dev_address = FindDeviceAddress ("audio.device");
	if (dev_address)
		CheckVectors (AUDIO_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("carddisk.device");
	if (dev_address)
		CheckVectors (CARDDISK_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("console.device");
	if (dev_address)
		CheckVectors (CONSOLE_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("gameport.device");
	if (dev_address)
		CheckVectors (GAMEPORT_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("input.device");
	if (dev_address)
		CheckVectors (INPUT_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("keyboard.device");
	if (dev_address)
		CheckVectors (KEYBOARD_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("ramdrive.device");
	if (dev_address)
		CheckVectors (RAMDRIVE_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("romboot.device");
	if (dev_address)
		CheckVectors (ROMBOOT_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("scsi.device");
	if (dev_address)
		CheckVectors (SCSI_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("timer.device");
	if (dev_address)
		CheckVectors (TIMER_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("trackdisk.device");
	if (dev_address)
		CheckVectors (TRACKDISK_DEV, (struct Library *)dev_address);
}

VOID main (VOID)
{
	ROMVersion = GetROMVersion ();
	ROMstart = FindROMStart ();
	ROMend = ROMstart + 0x80000;

	ROMVersion = GetROMVersion ();
  GetROMVersionNumber (ROMVer);
	printf ("\n  %s (%ld.%ld)\n\n", ROMVer, ROMVersion>>16, (UWORD)ROMVersion);

	max_entries = 0;

	if (!(ReqToolsBase = (struct ReqToolsBase *)OpenLibrary (REQTOOLSNAME, REQTOOLSVERSION)))
	{
		if (!(ReqToolsBase = (struct ReqToolsBase *)OpenLibrary ("VirusExecutor:libs/reqtools.library", REQTOOLSVERSION)))
		{
			sprintf (Dummy, "VirusExecutor needs '%s' v%ld +", REQTOOLSNAME, REQTOOLSVERSION);
			JEOEasyRequest (NULL, "Error!", Dummy, Ok, NULL);
			CleanUp ();
		}
	}

	check_sum = 0;

/*
	CheckIntRam ();	
	CheckIntCPU ();
*/
	InitPatches ();

	CheckDeviceVectors ();

	CheckVectors (EXEC_LIB, (struct Library *)SysBase);
	CheckVectors (UTIL_LIB, (struct Library *)ReqToolsBase->UtilityBase);
	CheckVectors (GFX_LIB, (struct Library *)ReqToolsBase->GfxBase);
	CheckVectors (INT_LIB, (struct Library *)ReqToolsBase->IntuitionBase);
	CheckVectors (DOS_LIB, (struct Library *)ReqToolsBase->DOSBase);
	CheckVectors (GAD_LIB, (struct Library *)ReqToolsBase->GadToolsBase);
	CheckVectors (LAY_LIB, (struct Library *)LayersBase);
	CheckVectors (ICON_LIB, (struct Library *)IconBase);
	CheckVectors (MATHIEEE_LIB, (struct Library *)MathIeeeSingBasBase);
	CheckVectors (MATHFFP_LIB, (struct Library *)MathBase);
	CheckVectors (KEYMAP_LIB, (struct Library *)KeymapBase);
	CheckVectors (EXPANSION_LIB, (struct Library *)ExpansionBase);
	CheckVectors (WB_LIB, (struct Library *)WorkbenchBase);

	SaveData ("VirusExecutor:VirusExecutor.patches");
 	printf (" max patches = %ld/5000\n", max_entries);
	printf (" VE_MAX_PATCHES = %ld/1000\n\n", VE_MAX_PATCHES);


//	printf ("\ncheck_sum = $%08lX\n\n", check_sum);
	CleanUp ();
}

VOID FindCode (UWORD *address)
{
	ULONG i, j;
	UWORD *p;

	p = address;

	for (i = 0; i < 300; i++)
	{
/*
		if (*p == MakeUWORD ("Mu"))
		{
			p++;
			if (*p == MakeUWORD ("lt"))
			{
				printf ("%ld, MakeULONG (\"Mult\")", i * 2 - 16);
				return;
			}
			else
				p--;
		}
*/
		j = 0x9eFC;
		if (*p == j)
		{
			printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
			return;
		}
		j = 0x254E;
		if (*p == j)
		{
			printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
			return;
		}

		for (j = 0x2F48; j <= 0x2F4E; j++)
		{
			if (*p == j)
			{
				printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
				return;
			}
		}
		for (j = 0x2068; j <= 0x206E; j++)
		{
			if (*p == j)
			{
				printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
				return;
			}
		}

		for (j = 0x2268; j <= 0x226E; j++)
		{
			if (*p == j)
			{
				printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
				return;
			}
		}

		for (j = 0x2468; j <= 0x246E; j++)
		{
			if (*p == j)
			{
				printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
				return;
			}
		}

		for (j = 0x2668; j <= 0x266E; j++)
		{
			if (*p == j)
			{
				printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
				return;
			}
		}

		for (j = 0x2868; j <= 0x286E; j++)
		{
			if (*p == j)
			{
				printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
				return;
			}
		}
		for (j = 0x2A68; j <= 0x2A6E; j++)
		{
			if (*p == j)
			{
				printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
				return;
			}
		}

		for (j = 0x2C68; j <= 0x2C6E; j++)
		{
			if (*p == j)
			{
				printf ("%ld, 0x%04lx%04lx", i * 2, j, *++p);
				return;
			}
		}

		if (*p == 0x6100)	// bsr.w
		{
			printf ("%ld, 0x%04lx%04lx", i * 2, 0x6100, *++p);
			return;
		}

		p++;
	}
}

VOID InitPatches (VOID)
{
	Insert_DEVICES ();

	Insert_EXEC_LIB ();
	Insert_UTIL_LIB ();
	Insert_GFX_LIB ();
	Insert_INT_LIB ();
	Insert_DOS_LIB ();
	Insert_GAD_LIB ();
	Insert_LAY_LIB ();
	Insert_ICON_LIB ();
	Insert_MATHIEEE_LIB ();
	Insert_MATHFFP_LIB ();
	Insert_EXPANSION_LIB ();
	Insert_WB_LIB ();
}
