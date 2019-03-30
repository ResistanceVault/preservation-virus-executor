;/*
sc NOOPT NOLINK CODE=f Hebbe
slink LIB:c.o+"Hebbe.o" to Hebbe LIB LIB:sc.lib LIB:reqtools.lib LIB:JEO.lib
Copy Hebbe c:
quit
*/

#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/execbase.h>
#include <exec/resident.h>
#include <libraries/reqtools.h>
#include <libraries/asl.h>
#include <proto/reqtools.h>
#include <proto/layers.h>
#include <proto/icon.h>
#include <proto/mathieeesingbas.h>
#include <proto/mathffp.h>
#include <proto/keymap.h>
#include <proto/expansion.h>
#include <proto/wb.h>

UWORD patchBrainVersion = 1;
UWORD patchBrainRevision = 56; // <<<<< HUSK **********************************

ULONG vbr;

#define PATCHBRAINSIZE	40
char PatchBrain[PATCHBRAINSIZE] = "$VER: PatchBrain 1.56 (18.01.2004)";	// ALWAYS 40 chars

ULONG ROMstart, ROMend;
ULONG ROMstart1, ROMend1;
ULONG ROMsize;

VOID FindCode (UWORD *address);
VOID InitPatches (VOID);

BOOL all_flag;

struct TextAttr topaz8 = {
	( STRPTR )"topaz.font", 8, 0x00, 0x01 };

enum
{
  VE_UnknVecText = 0,
  VE_SnoopDosMsg, 				
  VE_AMOSLibMsg, 					
  VE_68040LibMsg,
  VE_A2232FixMsg, 				
  VE_AddPowerMsg, 				
  VE_AlertPatchMsg, 			
  VE_ArqMsg, 							
  VE_AssignXMsg, 					
  VE_NewAudioMsg, 				
  VE_AutoRebootMsg, 			
  VE_BlitzFontsMsg, 			
  VE_BorderBlankMsg, 			
  VE_BusyPointerMsg, 			
  VE_CacheItMsg, 					
  VE_CheckDebugMsg, 			
  VE_CopyMemQuickerMsg, 	
  VE_CPUBlitMsg, 					
  VE_CPUClrUnicornMsg, 		
  VE_CPUClrCarnivoreMsg, 	
  VE_CPUClrPeterMsg, 			
  VE_CrMLibMsg,	 					
  VE_CrmDataDecrMsg, 			
  VE_CycleToMenuMsg	, 		
  VE_DiskCacheMsg, 				
  VE_DosPatchMsg, 				
  VE_DosPrefsMsg, 				
  VE_EasyReqPatchMsg, 		
  VE_EnforcerMsg, 				
  VE_EnhancedAlertMsg, 		
  VE_ExecPatchMsg, 				
  VE_ExplodeLibMsg, 			
  VE_FacctionMsg, 				
  VE_FasterBlitMsg, 			
  VE_FastFontsMsg, 				
  VE_FastMemCopyMsg, 			
  VE_FastWaitBlitMsg, 		
  VE_FastWaitBlitDaveMsg, 
  VE_FixReqMsg,						
  VE_ForceIconMsg, 				
  VE_FrontPubScreenMsg, 	
  VE_GOMFMsg, 						
  VE_PowerHDFloppyMsg, 		
  VE_ImmuneMsg,	 					
  VE_IPrefsMsg, 					
  VE_KCommMsg, 						
  VE_LibReqMsg, 					
  VE_LoadWBMsg, 					
  VE_LocaleLibMsg, 				
  VE_LowFragMsg, 					
  VE_LVDMsg, 							
  VE_MagicMenuMsg, 				
  VE_MakeVPortPatchMsg, 	
  VE_MemManMsg, 					
  VE_ModeProMsg,	 				
  VE_MultiCXMsg, 					
  VE_MultiUserLibMsg,
  VE_MungWallMsg, 				
  VE_MWAMsg, 							
  VE_NewAlertMsg, 				
  VE_NewIconsMsg, 				
  VE_NewMenuMsg, 					
  VE_NewModeMsg, 					
  VE_NickPrefsMsg, 				
  VE_NoCareMsg, 				
  VE_NOG2Msg, 						
  VE_NoReqMsg, 						
  VE_PatchAlertMsg, 			
  VE_PatchAllocMemMsg, 		
  VE_PatchControlMsg, 		
  VE_PatchLoadSegMsg, 		
  VE_PatchOpenWBMsg, 			
  VE_PatchPointerMsg, 		
  VE_PointerXDaveMsg, 		
  VE_PointerXMsg, 				
  VE_PopUpMenuMsg, 				
  VE_PowerCacheMsg, 			
  VE_PowerDataMsg, 				
  VE_PowerSnapMsg, 				
  VE_PPLibMsg, 						
  VE_PPPatcherMsg, 				
  VE_PPDRPMsg, 						
  VE_PPLoadSegMsg,				
  VE_PrintManagerMsg, 		
  VE_Prop3DMsg, 					
  VE_ProWizardMsg, 				
  VE_QMouseMsg, 					
  VE_QuickMenusMsg, 			
  VE_QuietTDMsg,	 				
  VE_ReplexMsg, 					
  VE_ReqChangeMsg, 				
  VE_ReqPatchMsg, 				
  VE_Reset1200Msg, 				
  VE_RomCrackMsg, 				
  VE_RTPatchMsg, 					
  VE_SaferPatchesMsg, 		
  VE_SegTrackerMsg, 			
  VE_SetManMsg, 					
  VE_SetPatchMsg, 				
  VE_SetPatchMrgCopMsg, 	
  VE_SFAPatchMsg, 				
  VE_SmartWBMsg, 					
  VE_SwazInfoMsg, 				
  VE_SwitchNTSCMsg, 			
  VE_SysExceptMsg, 				
  VE_SysiHackMsg, 				
  VE_TagScreensMsg, 			
  VE_TempoTopazMsg, 			
  VE_TimedReqMsg, 				
  VE_ScreenNotifyLibMsg, 			
  VE_ToolsDaemonMsg, 			
  VE_TWAMsg, 							
  VE_TurboTopazMsg, 			
  VE_UrouHackMsg, 				
  VE_VirusInterceptorMsg, 
  VE_WBExtMsg, 						
  VE_WBGaugeMsg,	 				
  VE_WReqMsg, 						
  VE_WTFPatchMsg, 				
  VE_xfdPatchMsg, 				
  VE_xLoadSegMsg, 				
  VE_XOperMsg, 						
  VE_ZoomWindowsMsg, 			
  VE_SerDeviceMsg,
  VE_AudDeviceMsg,
  VE_68060LibMsg,				
  VE_CyberPatcherMsg,
  VE_MiamiMsg,
  VE_SmartCrashMsg,
	VE_LowLevelLibMsg,
	VE_FastExecMsg,
	VE_SlowResetMsg,
	VE_BetterOpenLibsMsg,
	VE_ShowMemMsg,
	VE_PoolMemMsg,
	VE_NoDeleteMsg,
	VE_CMQMsg,
	VE_SnapMsg,
	VE_IdeFixMsg,
	VE_HDSleepMsg,
	VE_NSDPatchMsg,
	VE_FBlitMsg,
	VE_QLayersMsg,
	VE_FTextMsg,
  VE_1230SCSIMsg,
  VE_PrintManager1Msg, 		
  VE_DOSTraceMsg,
  VE_ExplodingLayersMsg,
  VE_AssignZMsg,
  VE_MagicCXMsg,
  VE_PatchHiddenMsg,
  VE_CEDFixMsg,
  VE_CyReqMsg,
  VE_NaeGrayPatchMsg,
  VE_NCommPatchMsg,
  VE_PatchBootMsg,
  VE_PatchInterleavedMsg,
  VE_PatchReplyMsgMsg,
  VE_PatchWBMsg,
  VE_PatchWorkMsg,
	VE_ROMUpdatesMsg,
	VE_NoFastMemMsg,
	VE_ReKickResetMsg,
	VE_MMULibMsg,
	VE_VisualPrefsMsg,
	VE_Dopus5LibMsg,
	VE_DiskLEDMsg,
	VE_PatchAslMsg,
	VE_ReqAttackMsg,
	VE_IconTraceMsg,
	VE_DynamicCacheMsg,
	VE_CEDPatchMsg,
	VE_MungMemMsg,
	VE_ExtraInfoMsg,
	VE_VMMMsg,
	VE_ScalaMsg,
	VE_MCPMsg,
	VE_HSMathLibsMsg,
	VE_RAWBInfoMsg,
	VE_PowerWindowsMsg,
	VE_WindowDaemonMsg,
	VE_NewWPA8Msg,
	VE_PatchWPA8Msg,
	VE_ExecutiveMsg,
	VE_BlazeWCPMsg,
	VE_StackAttackMsg,
	VE_Birdie2000Msg,
	VE_ObtainBetterPensMsg,
	VE_ParallelPatchMsg,
	VE_SerialPatchMsg,
	VE_SystemPatchMsg,
	VE_FastRADMsg,
  VE_QuickLensMsg,
  VE_TitleShadowMsg,
  VE_NotifyLibMsg,
  VE_TopazPatchMsg,
  VE_BootControlMsg,
  VE_PatchMult64Msg,
  VE_FastLayersLibMsg,
  VE_RtgLibMsg,
  VE_WB2FastMsg,
  VE_PowerWBMsg,
  VE_CleverWinMsg,
  VE_CyberGuardMsg,
  VE_MathLibsPatchMsg,
  VE_FPPrefsMsg,
  VE_HDOffMsg,
  VE_AppChangeMsg,
  VE_WinSpeedMsg,
  VE_MagicFramesMsg,
  VE_FakeTopazMsg,
  VE_PatchRAMMsg,
  VE_MuForceMsg,
	VE_MuGuardianAngelMsg,
	VE_DefIconsMsg,
	VE_SysTrackerMsg,
	VE_SnoopSetFunctionMsg,
	VE_ImagePoolLibMsg,
	VE_PpcLibMsg,
	VE_OutLineFontMsg,
	VE_PowerPcLibMsg,
	VE_SashimiMsg,
	VE_WipeOutMsg,
	VE_COPMsg,
	VE_ROMPatchMsg,
	VE_OldIconsMsg,
	VE_TurboSpoolMsg,
	VE_TragicWBMsg,
	VE_PPrefsMsg,
	VE_TrueMultiAssignsMsg,
	VE_CGXLibMsg,
	VE_MagicLayersMsg,
	VE_FixLongMultMsg,
	VE_FakeDefIconMsg,
	VE_DiskSafeMsg,
	VE_ScreenTabMsg,
	VE_BDebugMsg,
	VE_FixGetMsgMsg,
	VE_C_CPRMsg,
	VE_LibWatchMsg,
	VE_PMMsg,
	VE_CentreQuestMsg,
	VE_AssignWedgeMsg,
	VE_AslToRTMsg,
  VE_SaferPatches1Msg,
  VE_MagicTVMsg,
  VE_RPPMsg,
  VE_SnapperMsg,
  VE_MuFastZeroMsg,
  VE_SafeSwitchMsg,
  VE_SetDeviceMsg,
  VE_AtapiDeviceMsg,
  VE_WBLibMsg,
  VE_BlitScrollMsg,
  VE_FFPPatchMsg,
  VE_MemTrailerMsg,
  VE_MinStackMsg,
  VE_MemSpeed2Msg,
  VE_UnixDirs3Msg,
  VE_CopperDemonMsg,
  VE_NoInfoMsg,
  VE_XPKPatchMsg,
  VE_FastFonts2Msg,
  VE_OxyPatcherMsg,
  VE_TRAPMsg,
  VE_AutoUpdateWBMsg,
  VE_mTrackMsg,
  VE_AslSizePatchMsg,
  VE_AmigaOSROMUpdate,
  VE_MuMove4KMsg,
  VE_RamDriveUnitMsg,
  VE_BPPCFixMsg,
  VE_MiamiDeluxeMsg,
  VE_SoftSCSIMsg,
  VE_A1200_HDPatchMsg,
  VE_TurboValMsg,
  VE_MemoryKicktagMsg,
  VE_BootPrefsMsg,
  VE_PrepareEmulMsg,
  VE_ReserveWarmMsg,
  VE_LoadModuleMsg,
  VE_CybBootPrefsMsg,
  VE_PaulaAudioMsg,
  VE_ConsoleDevMsg,
  VE_IconLibMsg,
  VE_PowerIconsMsg,
  VE_IncubatorMsg,
  VE_siegfriedLibMsg,
  VE_AMOSPatcherMsg,
  VE_PatchMSBMsg,
  VE_SetPatchSingBasMsg,
  VE_ForceFastMemMsg,
  VE_DumpTaskMemMsg,
  VE_AbortPktMsg,
  VE_DosWedgeMsg,
  VE_MAX_PATCHES
};

char far PatchNames[VE_MAX_PATCHES][38] =
{
	"*** UNKNOWN VECTOR ***",
	"SnoopDos by Eddy Carroll",
	"amos.library",
	"68040.library",
	"A2232/QuickFix by Chr. Buchner",
	"AddPower by Ian J. Einman",
	"Alert Patch by David Swasbrook",
	"Arq by Martin J. Laubach",
	"AssignX by Steve Tibbett",
	"Audio.Device 34.0 by JG",
	"AutoReboot by Chris Johnsen",
	"BlitzFonts by Hayes Haugen",
	"BorderBlank",
	"BusyPointer by Dan Blender",
	"CacheIt by HoukY-Soft",
	"CheckDebug by Thomas Kessler",
	"CopyMemQuicker by Arthur Hagen",
	"CPUBlit Eddy Carroll",
	"CPUClr by Unicorn",
 	"CPUClr by Carnivore/BeerMacht",
	"CPUClr by Peter Simons",
	"CrM.library by Thomas Schwarz",
	"CrM RTDD by Thomas Schwarz",
	"CycleToMenu by Frederico Giannici",
	"DiskCache by Oliver Wagner",
	"DosPatch by Markus M. Wild",
	"DosPrefs by Norbert Püschel",
	"EasyReqPatch by M. Origgi",
	"Enforcer by Michael Sinz",
	"Enhanced Alert Hook by MJSoft",
	"ExecPatch by Arthur Hagen",
	"explode.library by A.J. Brower",
	"Facction by ASDG Inc.",
	"FasterBlit by Arthur Hagen",
	"FastFonts by Charlie Heath",
	"FastMemCopy by M. Berndt",
	"FastWaitBlit by M. Berndt",
	"FastWaitBlit by Dave Jones",
	"FixReq",
	"ForceIcon by Kai Iske",
	"FrontPubScreen by Nicola Salmoria",
	"GOMF by Christian Johnsen",
	"HD Floppy Driver by Adam Hill",
	"Immune by Xavier Leclercq",
	"IPrefs",
	"KCommodity by Kai Iske",
	"LibReq by Nick Salmoria",
	"LoadWB",
	"locale.library",
	"LowFrag by Eric Sauvageau",
	"LVD by Peter Stuer",
	"MagicMenu by Martin Korndörfer",
	"MakeVPort Patch by S. Vigna",
	"MemMan2",
	"ModePro by Michael Rivers",
	"MultiCX by Martin Berndt",
	"multiuser.lib by G. Uytterhoeven",
	"MungWall",
	"MWA by Stefan Stuntz",
	"New Alert 36.3",
	"NewIcons",
	"NewMenu by Brian Gontowski",
	"NewMode by Andreas Linnemann",
	"NickPrefs 37.9",
	"NoCare by Raymond Hoving",
	"NOG2 by Daniel Weber",
	"NoReq by Steve Tibbett",
	"PatchAlert by C. Beskow",
	"PatchAllocMem by Simon Dick",
	"PatchControl by Stefan Sommerfeld",
	"PatchLoadSeg by P.v.L.",
	"PatchOpenWB by ?",
	"PatchPointer by Steve Lemieux",
	"PointerX by Dave Jones",
	"PointerX by Steve Tibbett",
	"PopUpMenu by Martin Adrian",
	"PowerCache by Michael Berg",
	"PowerData by Michael Berg",
	"PowerSnap by Nico François",
	"powerpacker.library by Nico François",
	"PP Patcher by Michael Berg",
	"PPDRP by CodX",
	"PPLoadSeg by Nico François",
	"Print Manager by Nicola Salmoria",
	"Prop3D by Massimo Tantignone",
	"ProWizard by Nicolas Franck",
	"QMouse by Dan Babcock",
	"QuickMenus by Brian Moats",
	"QuietTD",
	"Replex by Ekke Verheul",
	"ReqChange by Magnus Holmgren",
	"ReqPatch by Raul Sobon",
	"Reset1200 by Carsten Sørensen",
	"ROM Crack by SCA",
	"RTPatch by Nico François",
	"SaferPatches by Martin Adrian",
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
	"TagScreens by M. Berndt",
	"TempoTopaz by Preben Nielsen",
	"TimedReqs by Darrell Grainger",
	"screennotify.library",
	"ToolsDaemon by Nico François",
	"Tron's Window Activator",
	"TurboTopaz by Preben Nielsen",
	"UrouHack by Tinic Urou",
	"Virus Interceptor by J. Eliasson",
	"WBExtender by Martin Berndt",
	"WBGauge by Jean-Michel Forgeas",
	"Wreq by Tuomo Mickelsson",
	"WTFPatch by David Jenkins",
	"xfdPatch by Georg Hörmann",
	"xLoadSeg by Chr. Schneider",
	"Xoper by Werner Günther",
	"ZoomWindows by Dr.Maybe",
	"serial.device",
	"audio.device",
	"68060.library",
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
	"1230 SCSI Device",
	"PrintManager by Stephan Rupprecht",
	"DOSTrace by Peter Stuer",
	"Exp. Layers by Andreas Schildbach",
	"AssignZ by Tobias Abt",
	"MagicCX by Kai Iske",
	"PatchHidden by E.Hambuch",
	"CEDFix by Francesco Doro",
	"CyReq by Georg Steger",
	"NaeGrayPatch by Lee Kindness",
	"NCommPatch by Marius Mortensen",
	"PatchBoot by Thomas Richter",
	"PatchInterleaved by Ulrich Hambuch",
	"PatchReplyMsg by ?",
	"PatchWB by THOR - Software",
	"PatchWork by Richard Körber",
	"SetPatch / NSD / ROM Update",
	"NoFastMem",
	"ReKickReset by Mark Knibbs",
	"mmu.library by Thomas Richter",
	"VisualPrefs by Massimo Tantignone",
	"dopus5.library",
	"DiskLED by Per Johansson",
	"PatchAsl by Flavio Stanchina",
	"ReqAttack by Jacek Piszczek jr.",
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
	"notifyintuition.library by Schober",
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
	"imagepool.library by Linus McCabe",
	"ppc.library by Frank Wille",
	"OutLineFont by Nyberg,Gapen,Rotvel",
	"powerpc.library",
	"Sashimi by Olaf `Olsen' Barthel",
	"WipeOut by Olaf `Olsen' Barthel",
	"COP by THOR-Software",
	"ROM",
	"OldIcons by Kyzer/CSG",
	"TurboSpool by R.Jocham/F.Zeiler",
	"TragicWB by Ben Hutchings",
	"PPrefs by Olaf Gschweng",
	"TrueMultiAssigns by Thomas Richter",
	"CGX Library",
	"Magic Layers by Trond W. Hansen",
	"FixLongMult by Thomas Richter",
	"FakeDefIconMsg by Jason R. Hulance",
	"DiskSafe by Thomas Richter",
	"ScreenTab by Michael Link",
	"BDebug by Ralph Schmidt",
	"FixGetMsg by Frederick H.G. Wright II",
	"SAS/C_CPR by SAS Institute Inc.",
	"LibWatch by Christian Wasner",
	"PM by Michael Sinz",
	"CentreQuest by Damien Stewart",
	"AssignWedge by Olaf `Olsen' Barthel",
	"AslToRT by Thore Böckelmann",
	"SaferPatches by Thomas Richter",
	"MagicTV by Neil Crawforth",
	"RPP by Stephan Rupprecht",
	"Snapper by ToneMaster",
	"MuFastZero by Thomas Richter",
	"SafeSwitch by ?",
	"SetDevice by GVP",
	"atapi.device by Oliver Kastl",
	"workbench.library",
	"BlitScroll by Chris Perver",
	"FFPPatch by Jess Sosnoski",
	"MemTrailer by Peter Keunecke",
	"MinStack by Peter Keunecke",
	"MemSpeed2 by Olly Koenders",
	"UnixDirs3 by Timo Kaikumaa",
	"Copper-Demon by Valenta Ferenc",
	"NoInfo by Chris Hames",
	"XPKPatch by Grzegorz Krolik",
	"FastFonts by THOR-Software",
	"OXYRON-Patcher by Achim Koyen",
	"TRAP by Massimo Tantignone",
	"AutoUpdateWB by Elena Novaretti",
	"mTrack by Vladimir Zidar",
	"AslSizePatch by Simone Tellini",
	"AmigaOS ROM Update",
	"MuMove4K",
	"ramdrive.unit",
	"BPPCFix",
	"Miami Deluxe by Holger Kruse",
	"SoftSCSI",
	"A1200_HDPatch",
	"TurboVal",
	"Memory Kicktag",
	"BootPrefs",
	"PrepareEmul",
	"Reserve Warm",
	"LoadModule",
	"Cyb-BootPrefs",
	"paula.audio (AHI)",
	"console.device by MM",
	"icon.library",
	"PowerIcons by Elena Novaretti",
	"Incubator by Alexander Dimitriadis",
	"siegfried.library by SIEGFRIED SOFT",
	"AMOSPatcher by Niki Murkett",
	"PatchMSB by Peter Keunecke",
	"SetPatchSingBas by Magnus Holmgren",
	"ForceFastMem by Magnus Holmgren",
	"DumpTaskMem by Magne Østlyngen",
	"AbortPkt by Bernhard Fastenrath",
	"DosWedge by David Swasbrook"
};

BOOL hide_known_patches = TRUE;

ULONG max_entries;

enum
{
	EXEC_LIB = 0,
	DOS_LIB,
	UTIL_LIB,
	GFX_LIB,
	INT_LIB,
	GAD_LIB,
	LAY_LIB,
	ICON_LIB,
	AUDIO_DEV,
	CONSOLE_DEV,
	INPUT_DEV,
	SCSI_DEV,
	TIMER_DEV,
	TRACKDISK_DEV,
	MATHIEEE_LIB,
	GAMEPORT_DEV,
	KEYBOARD_DEV,
	RAMDRIVE_DEV,
	CARDDISK_DEV,
	ROMBOOT_DEV,
	MATHFFP_LIB,
	KEYMAP_LIB,
	WB_LIB,
	EXPANSION_LIB,
	REQTOOLS_LIB,
	ASL_LIB,
	CPU_INT,
	EXEC_INT,
	VE_TAG,
	VE_MEM
};

char Dummy[5000];
char Ok[] = "Ok";

ULONG check_sum;

extern struct ExecBase *SysBase;
struct ReqToolsBase *ReqToolsBase = 0;
struct AslBase *AslBase = 0;

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

#define GF_NOLVO      0   /* LVO is invalid for this library base */
#define GF_CODEINLVO  -1  /* LVO is not 'jmp (abs).l' or similar */

BOOL InROM (ULONG address)
{
	BOOL rom = TRUE;

	if ((address < ROMstart OR address > ROMend) AND ((address != 0) AND (address != (ULONG)-1)))
		rom = FALSE;

	if (!rom AND ROMstart1)
	{
		rom = TRUE;
		if (address < ROMstart1 OR address > ROMend1)
			rom = FALSE;
	}

	return (rom);
}

/* Get pointer to function LVO opcodes
   This routine is used by higher level routines...
*/
const void *GetOpcodes(struct Library *lib, WORD lvo)
{
  /* Check if lvo is valid for this library, if not return NULL
  */

  /* Note: lib_NegSize is positive, but lvo is not, so negate lvo
  */
  if (-lvo > lib->lib_NegSize)
  {
    return NULL;
  }

  /* Ok, it's valid, return the ptr to opcodes
  */
  return (const void *) (((UBYTE *) lib) + lvo);
}


/* return TRUE if ptr is valid memory (ROM or RAM)
*/
BOOL ValidMem (ULONG *ptr)
{
  return (BOOL) ((((ULONG)ptr >= ROMstart) && ((ULONG)ptr < ROMend)) || TypeOfMem((void *) ptr));
}

/* Return pointer to function LVO code or special values to tell the
   LVO is special.

   This is useful for visual printing of function address. We can
   indicate that this LVO is not jmp to addr, but opcodes inside
   jumptable, whereas the other routine can be used to get the ptr
   to actual code, no matter what (that is used for actual checking,
   see below).
*/

const void *HSGetFunction (struct Library *lib, WORD lvo)
{
  const UWORD *op;
  const void *ptr;

  /* Get ptr to function opcodes
  */
  op  = GetOpcodes(lib, lvo);

  if (op == NULL)
  {
    return (const void *) GF_NOLVO;
  }

  /* default to no ptr
  */
  ptr = NULL;

  /* Is it a JMP/JSR (abs).l ?
  */
  if ((op[0] == 0x4EF9) ||          /* JMP (abs).l */
      (op[0] == 0x4EB9) ||          /* JSR (abs).l */
      (op[0] == 0x4AFC) ||          /* ILLEGAL    - used by some viruses*/
      ((op[0] & 0xFFF0) == 0x4E40)) /* TRAP #x      fool AV software... */
  {

    /* yep, get the destination

      NOTE: ILLEGAL and TRAP #x could also use some custom way to
            solve the destination address, but we assume they try
            to trick us with unchanged jmp destination...

            We could peek the actual vector from VBR for ILLEGAL and
            TRAP #x, and return *that* ptr instead... that would be more
            like detecting actual virus...

            Anyway ILLEGAL or TRAP #x in first opcode *always* mean
            something evil has patched the system and BIG alarm bells
            should start ringing...
    */
    ptr = (const void *) *((ULONG *) &op[1]);

		if (1)
		{
			UWORD *p;

			p = (UWORD *)ptr;
			if (*p == 0x4EF9)
			{
				ULONG *p1;

				p++;
				p1 = (ULONG *)p;
				ptr = (const void *) (ULONG)*p1;
			}
		}
  }
  else if ((op[0] == 0x60FF) ||     /* BRA.L      - too.. (68020+ opcode) */
           (op[0] == 0x61FF))       /* BSR.L      - too.. (68020+ opcode) */
  {

    /* long offsets at op[1] relative to op[1]...
    */
    ptr = (const void *) (((UBYTE *) &op[1]) + *((ULONG *) &op[1]));

  }

  if (ValidMem((ULONG *)ptr))
  {
    return ptr;
  }

  /* ...Else return special 'code in lvo' flag
  */
  return (const void *) GF_CODEINLVO;
}

/* Get pointer to library/device/resource function LVO's code, or
   NULL if LVO is invalid for this library.
*/
const void *GetFunctionCode(struct Library *lib, WORD lvo)
{
  const UWORD *code;

  /* Get ptr to function code
  */
  code = HSGetFunction(lib, lvo);

  if (code == (const void *) GF_NOLVO)
  {
    /* oops, LVO out of range
    */
    return NULL;
  }
  else if (code == (const void *) GF_CODEINLVO)
  {
    /* Return ptr to the opcodes in jump table
       Note: this is maximum 3 opcodes!

       For example:
       $7000 $4E75 $XXXX     moveq  #0,d0 / rts
       $4C01 $0800 $4E75     muls.l d1,d0 / rts
    */
    return ((UBYTE *) lib) + lvo;
  }

  /* Yep, this is normal jmp, return the jump destination
  */
  return code;
}

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
	if (AslBase)
		CloseLibrary ((struct Library *)AslBase);

	exit (0);
}

/*
BOOL CheckValidAddress (ULONG address)
{
	if ((address < ROMstart OR address > ROMend) AND ((address != 0) AND (address != (ULONG)-1)))
		return (FALSE);
	else
		return (TRUE);
}
*/

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
//	ULONG asl_version = 0x002a0000;				// 42.0
	ULONG asl_version = 0x002d0004;				// 45.4
	ULONG reqtools_version = 0x00270002;	// 39.2

	if (type == CPU_INT)
	{
		ULONG *Int;

		Int = (ULONG *)(vbr + function);
		p = (ULONG *)*Int;
	}
	else
		p = (ULONG *)GetFunctionCode (lib, function);

	check_sum += (ULONG)p;

	if (!InROM ((ULONG)p))
	{
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

		if (type == REQTOOLS_LIB)		// Special check!
		{
			UWORD *p_w;
			UWORD nr = 0;
			ULONG *p_tmp;

			p_w = (UWORD *)p;
			p_tmp = p;
			p_tmp++;
			FOREVER
			{
				nr++;
				if (*p_w == 0x4e75)
					break;

				p_w++;
			}

//					printf ("case %4ld: if (*p == 0x%08lx AND *p_tmp == 0x%08lx AND nr == %ld) return (0); break;\n", function, *p, *p_tmp, nr);
	
			switch (reqtools_version>>16)
			{
				case 39:
				{
					switch (function)
					{
						case   -6: if (*p == 0x4e55ff9c AND *p_tmp == 0x48e73c3a AND nr == 284) return (0); break;
						case  -12: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e536e AND nr == 24) return (0); break;
						case  -18: if (*p == 0x4e550000 AND *p_tmp == 0x48e72022 AND nr == 119) return (0); break;
						case  -24: if (*p == 0x4e550000 AND *p_tmp == 0x42804e5d AND nr == 5) return (0); break;
						case  -30: if (*p == 0x4e550000 AND *p_tmp == 0x48e72032 AND nr == 124) return (0); break;
						case  -36: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e2f02 AND nr == 35) return (0); break;
						case  -42: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f0a AND nr == 30) return (0); break;
						case  -48: if (*p == 0x4e55fef4 AND *p_tmp == 0x48e73c3a AND nr == 264) return (0); break;
						case  -54: if (*p == 0x4e55ffe0 AND *p_tmp == 0x48e73f3a AND nr == 1008) return (0); break;
						case  -60: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e2f0a AND nr == 20) return (0); break;
						case  -66: if (*p == 0x4e550000 AND *p_tmp == 0x48e70038 AND nr == 20) return (0); break;
						case  -72: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f0a AND nr == 21) return (0); break;
						case  -78: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f0a AND nr == 21) return (0); break;
						case  -84: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f02 AND nr == 25) return (0); break;
						case  -90: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f02 AND nr == 24) return (0); break;
						case  -96: if (*p == 0x4e55ffe0 AND *p_tmp == 0x48e73f3a AND nr == 1008) return (0); break;
						case -102: if (*p == 0x4e55ffb8 AND *p_tmp == 0x48e73f3a AND nr == 698) return (0); break;
						case -108: if (*p == 0x4e550000 AND *p_tmp == 0x2f0a2451 AND nr == 9) return (0); break;
						case -114: if (*p == 0x4e55fff0 AND *p_tmp == 0x2f0e2b7c AND nr == 24) return (0); break;
						case -120: if (*p == 0x4e55ffe8 AND *p_tmp == 0x48e73c3a AND nr == 135) return (0); break;
						case -126: if (*p == 0x4e55fff4 AND *p_tmp == 0x48e73f3a AND nr == 152) return (0); break;
						case -132: if (*p == 0x4e550000 AND *p_tmp == 0x48e73f20 AND nr == 50) return (0); break;
						case -138: if (*p == 0x4e550000 AND *p_tmp == 0x48e73022 AND nr == 57) return (0); break;
						case -144: if (*p == 0x4e55ffe0 AND *p_tmp == 0x48e73f3a AND nr == 1008) return (0); break;
						case -150: if (*p == 0x4e550000 AND *p_tmp == 0x48e7003a AND nr == 62) return (0); break;
						case -156: if (*p == 0x4e550000 AND *p_tmp == 0x48e73032 AND nr == 108) return (0); break;
						case -162: if (*p == 0x4e55ffe0 AND *p_tmp == 0x48e73022 AND nr == 86) return (0); break;
						case -168: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e2f02 AND nr == 19) return (0); break;
						case -174: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e41ee AND nr == 13) return (0); break;
					}
					break;
				}
				default:	// 38
				{
					switch (function)
					{
						case   -6: if (*p == 0x526e003e AND *p_tmp == 0x200e4e75 AND nr == 4) return (0); break;
						case  -12: if (*p == 0x7000536e AND *p_tmp == 0x003e4e75 AND nr == 4) return (0); break;
						case  -18: if (*p == 0x48e7202e AND *p_tmp == 0x2a4e2c78 AND nr == 76) return (0); break;
						case  -24: if (*p == 0x70004e75 AND nr == 2) return (0); break;
						case  -30: if (*p == 0x594f48e7 AND *p_tmp == 0x201e2f48 AND nr == 113) return (0); break;
						case  -36: if (*p == 0x48e7000e AND *p_tmp == 0x2a49200d AND nr == 34) return (0); break;
						case  -42: if (*p == 0x594f48e7 AND *p_tmp == 0x001c49f9 AND nr == 28) return (0); break;
						case  -48: if (*p == 0x9efc011c AND *p_tmp == 0x48e7213e AND nr == 275) return (0); break;
						case  -54: if (*p == 0x9efc002c AND *p_tmp == 0x48e7373e AND nr == 964) return (0); break;
						case  -60: if (*p == 0x48e7000e AND *p_tmp == 0x2a4849f9 AND nr == 18) return (0); break;
						case  -66: if (*p == 0x48e73820 AND *p_tmp == 0x200c7604 AND nr == 12) return (0); break;
						case  -72: if (*p == 0x48e73820 AND *p_tmp == 0x7602280b AND nr == 11) return (0); break;
						case  -78: if (*p == 0x48e73820 AND *p_tmp == 0x24097603 AND nr == 7) return (0); break;
						case  -84: if (*p == 0x48e73820 AND *p_tmp == 0x76004211 AND nr == 27) return (0); break;
						case  -90: if (*p == 0x48e73820 AND *p_tmp == 0x76016008 AND nr == 31) return (0); break;
						case  -96: if (*p == 0x9efc002c AND *p_tmp == 0x48e7373e AND nr == 964) return (0); break;
						case -102: if (*p == 0x9efc0134 AND *p_tmp == 0x48e73f3e AND nr == 1923) return (0); break;
						case -108: if (*p == 0x2f114e75 AND nr == 2) return (0); break;
						case -114: if (*p == 0x48e73002 AND *p_tmp == 0x22780004 AND nr == 29) return (0); break;
						case -120: if (*p == 0x9efc001c AND *p_tmp == 0x48e7201e AND nr == 114) return (0); break;
						case -126: if (*p == 0x48e73f3c AND *p_tmp == 0x28482649 AND nr == 115) return (0); break;
						case -132: if (*p == 0x48e73c00 AND *p_tmp == 0x2a014845 AND nr == 34) return (0); break;
						case -138: if (*p == 0x2f0e226e AND *p_tmp == 0x002a2c78 AND nr == 24) return (0); break;
						case -144: if (*p == 0x9efc002c AND *p_tmp == 0x48e7373e AND nr == 964) return (0); break;
						case -150: if (*p == 0x48e700a6 AND *p_tmp == 0x2a6e002a AND nr == 48) return (0); break;
						case -156: if (*p == 0x48e730a2 AND *p_tmp == 0x22680024 AND nr == 89) return (0); break;
						case -162: if (*p == 0x48e730c2 AND *p_tmp == 0x20096700 AND nr == 73) return (0); break;
						case -168: if (*p == 0x2f0e41ee AND *p_tmp == 0x00562c78 AND nr == 12) return (0); break;
						case -174: if (*p == 0x2f0e41ee AND *p_tmp == 0x00562c78 AND nr == 9) return (0); break;
					}
				}
			}
		}
		else if (type == ASL_LIB)		// Special check!
		{
			UWORD *p_w;
			UWORD nr = 0;
			ULONG *p_tmp;

			p_w = (UWORD *)p;
			p_tmp = p;
			p_tmp++;
			FOREVER
			{
				nr++;
				if (*p_w == 0x4e75)
					break;

				p_w++;
			}
//			printf ("case %4ld: if (*p == 0x%08lx AND *p_tmp == 0x%08lx AND nr == %ld) return (0); break;\n", function, *p, *p_tmp, nr);

			switch (asl_version>>16)
			{
				case 42:
				{
					switch (function)
					{
						case   -6: if (*p == 0x526e0022 AND *p_tmp == 0x08ae0003 AND nr == 7) return (0); break;
						case  -12: if (*p == 0x536e0022 AND *p_tmp == 0x082e0003 AND nr == 8) return (0); break;
						case  -18: if (*p == 0x610035f2 AND *p_tmp == 0x4a6e0022 AND nr == 10) return (0); break;
						case  -24: if (*p == 0x70004e75 AND nr == 2) return (0); break;
						case  -30: if (*p == 0x700091c8 AND *p_tmp == 0x4eeeffd0 AND nr == 18) return (0); break;
						case  -36: if (*p == 0x4eeeffca AND *p_tmp == 0x93c94eee AND nr == 14) return (0); break;
						case  -42: if (*p == 0x93c94eee AND *p_tmp == 0xffc42f0b AND nr == 12) return (0); break;
						case  -48: if (*p == 0x48e70110 AND *p_tmp == 0x2e002648 AND nr == 41) return (0); break;
						case  -54: if (*p == 0x2f0b2648 AND *p_tmp == 0x200b6730 AND nr == 30) return (0); break;
						case  -60: if (*p == 0x48e70032 AND *p_tmp == 0x26482449 AND nr == 48) return (0); break;
						case  -66: if (*p == 0x48e70002 AND *p_tmp == 0x2079082c AND nr == 25) return (0); break;
					}
				}
				default:	// 45.2+?
				{
					switch (function)
					{
						case   -6: if (*p == 0x526e0020 AND *p_tmp == 0x200e08ae AND nr == 7) return (0); break;
						case  -12: if (*p == 0x2f07536e AND *p_tmp == 0x0020082e AND nr == 14) return (0); break;
						case  -18: if (*p == 0x48e72106 AND *p_tmp == 0x2a4e4a6d AND nr == 44) return (0); break;
						case  -24: if (*p == 0x70004e75 AND nr == 2) return (0); break;
						case  -30: if (*p == 0x700091c8 AND *p_tmp == 0x4eaeffd0 AND nr == 5) return (0); break;
						case  -36: if (*p == 0x4eaeffca AND *p_tmp>>16 == 0x4e75 AND nr == 3) return (0); break;
						case  -42: if (*p == 0x2f0d93c9 AND *p_tmp == 0x2a484eae AND nr == 13) return (0); break;
						case  -48: if (*p == 0x594f48e7 AND *p_tmp == 0x01102e00 AND nr == 41) return (0); break;
						case  -54: if (*p == 0x594f2e88 AND *p_tmp == 0x2008672e AND nr == 29) return (0); break;
						case  -60: if (*p == 0x9efc0030 AND *p_tmp == 0x48e70136 AND nr == 161) return (0); break;
						case  -66: if (*p == 0x48e70016 AND *p_tmp == 0x2a49200d AND nr == 28) return (0); break;
						case  -72: if (*p == 0x514f48e7 AND *p_tmp == 0x07162e01 AND nr == 54) return (0); break;
						case  -78: if (*p == 0x594f48e7 AND *p_tmp == 0x00162a4e AND nr == 59) return (0); break;
						case  -84: if (*p == 0x594f48e7 AND *p_tmp == 0x00162a4e AND nr == 41) return (0); break;
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

UWORD GetMaxFunction (struct Library *lib)
{
	return (lib->lib_NegSize);
}

VOID CheckVectors (UWORD type, struct Library *lib)
{
	WORD i;
	WORD max_func;
	ULONG p;
	char Type[50] = "???";

	if (type == CPU_INT)
	{
		ULONG p, i, j;
	
  	for (i = 8, j = 0; i <= 124; i += 4)
	  {
			if (i > 44 AND i < 52)
				continue;
			if (i > 60 AND i < 96)
				continue;
			p = CheckAddress (type, lib, i);
			if (p > 0)	// Ikke i ROM?
			{
				if (!(strcmp (PatchName, PatchNames[VE_UnknVecText])))
				{
				  printf ("  %5d ($%03lX)  :  $%08X %s\n", i, i, p, PatchName);
					strcpy (Type, "CPU_INT");
					printf ("  InsertPatch (%s, %ld, ", Type, i);
					printf (", VE_Msg);\n");
				}
				else
				{
					if (all_flag)
					  printf ("  %5d ($%03lX)  :  $%08X %s\n", i, i, p, PatchName);
				}
			}
		}
	}
	else
	{
		max_func = GetMaxFunction (lib) *-1;
		printf ("\n$%08lX: %s %ld.%ld\n", lib, lib->lib_Node.ln_Name, lib->lib_Version, lib->lib_Revision);
		for (i = -6; i >= max_func; i -= 6)
		{
			p = CheckAddress (type, lib, i);
			if (p > 0)	// Ikke i ROM?
			{
				if (!(strcmp (PatchName, PatchNames[VE_UnknVecText])))
				{
//				  printf ("  %5d (-$%03lX)  :  $%08X %s\n", i, i * -1, p, PatchName);

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
						case EXPANSION_LIB: strcpy (Type, "EXPANSION_LIB"); break;
						case WB_LIB: strcpy (Type, "WB_LIB"); break;
						case REQTOOLS_LIB: strcpy (Type, "REQTOOLS_LIB"); break;
						case ASL_LIB: strcpy (Type, "ASL_LIB"); break;
					}
					printf ("  InsertPatch (%s, %5ld, , 0x, VE_Msg);\n", Type, i);
				}
				else
				{
					if (all_flag)
					  printf ("  %5d (-$%03lX)  :  $%08X %s\n", i, i * -1, p, PatchName);
				}
			}
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

/*
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

					case   8: { mystrcpy (FD_name, "Bus Error"); break; }
					case  12: { mystrcpy (FD_name, "Address Error"); break; }
					case  16: { mystrcpy (FD_name, "Illegal Instruction"); break; }
					case  20: { mystrcpy (FD_name, "Division By Zero"); break; }
					case  24: { mystrcpy (FD_name, "CHK / CHK2"); break; }
					case  28: { mystrcpy (FD_name, "TRAPV"); break; }
					case  32: { mystrcpy (FD_name, "Privilege Violation"); break; }
					case  36: { mystrcpy (FD_name, "Trace Trap"); break; }
					case  40: { mystrcpy (FD_name, "Line A Emulator"); break; }
					case  44: { mystrcpy (FD_name, "Line F Emulator"); break; }
					case  52: { mystrcpy (FD_name, "Coproc. Protocol"); break; }
					case  56: { mystrcpy (FD_name, "Format Error"); break; }
					case  60: { mystrcpy (FD_name, "Uninit. Interrupt"); break; }
					case  96: { mystrcpy (FD_name, "Spurious Interrupt"); break; }
					case 100: { mystrcpy (FD_name, "Level 1 Autovector"); break; }
					case 104: { mystrcpy (FD_name, "Level 2 Autovector"); break; }
					case 108: { mystrcpy (FD_name, "Level 3 Autovector"); break; }
					case 112: { mystrcpy (FD_name, "Level 4 Autovector"); break; }
					case 116: { mystrcpy (FD_name, "Level 5 Autovector"); break; }
					case 120: { mystrcpy (FD_name, "Level 6 Autovector"); break; }
					case 124: { mystrcpy (FD_name, "Level 7 Autovector"); break; }


VOID CheckIntCPU (VOID)
{
	ULONG *Int;
	ULONG i, j;
	UBYTE valid;

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
*/

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

		VE_Crypt (CRYPT, (char *)&PatchNames[0], VE_MAX_PATCHES * 38);
		Write (fh, &PatchNames[0], VE_MAX_PATCHES * 38);

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

VOID main (int argc, char **argv)
{
	struct Resident *r;
	ULONG *p;

	vbr = GetVBR ();
	all_flag = FALSE;
	if (argc == 2)
	{
		if (!(stricmp (argv[1], "ALL")))
			all_flag = TRUE;
	}
	
	p = (ULONG *)0x00FFFFEC;
	ROMsize = *p;
	ROMstart = 0x01000000 - ROMsize;
	ROMend = ROMstart + ROMsize;

	ROMstart1 = ROMstart;
	ROMend1 = ROMend;

	if (r =  FindResident ("exec.library"))
	{
		if (r->rt_Version >= 45)	// BB2 exec
		{
			ROMstart1 = (ULONG)r;
			ROMend1 = (ULONG)r->rt_EndSkip;
		}
	}

	max_entries = 0;

	if (!(ReqToolsBase = (struct ReqToolsBase *)OpenLibrary (REQTOOLSNAME, REQTOOLSVERSION)))
	{
		if (!(ReqToolsBase = (struct ReqToolsBase *)OpenLibrary ("VirusExecutor:libs/reqtools.library", REQTOOLSVERSION)))
		{
			sprintf (Dummy, "Hebbe needs '%s' v%ld +", REQTOOLSNAME, REQTOOLSVERSION);
			JEOEasyRequest (NULL, "Error!", Dummy, Ok, NULL);
			CleanUp ();
		}
	}

	if (!(AslBase = (struct AslBase *)OpenLibrary (AslName, 0)))
	{
		sprintf (Dummy, "Hebbe needs '%s'", AslName);
		JEOEasyRequest (NULL, "Error!", Dummy, Ok, NULL);
		CleanUp ();
	}


	check_sum = 0;

/*
	CheckIntRam ();	
	CheckIntCPU ();
*/
	InitPatches ();

	CheckDeviceVectors ();
	CheckVectors (CPU_INT, (struct Library *)SysBase);
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
	CheckVectors (REQTOOLS_LIB, (struct Library *)ReqToolsBase);
	CheckVectors (ASL_LIB, (struct Library *)AslBase);

	SaveData ("-V:VirusExecutor.PatchBrain");
	Execute ("C:Copy -v:VirusExecutor.PatchBrain V:VirusExecutor.PatchBrain", NULL, NULL);

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
	InsertPatch (AUDIO_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (AUDIO_DEV, -30, 14, 0x4eaeffe2, VE_ROMUpdatesMsg);
	InsertPatch (AUDIO_DEV, -6, 0x20, 0x4eaefe26, VE_NewAudioMsg);
	InsertPatch (AUDIO_DEV, -12, 0x22, 0x4eaefe38, VE_NewAudioMsg);
	InsertPatch (AUDIO_DEV, -18, -0x20, 0x4eaefe38, VE_NewAudioMsg);
	InsertPatch (AUDIO_DEV, -30, 0x3a, 0x41ec00a6, VE_NewAudioMsg);
	InsertPatch (AUDIO_DEV, -36, 0x40, 0xd201d201, VE_NewAudioMsg);
  InsertPatch (AUDIO_DEV, -12, 19, 0x84671c22, VE_SaferPatches1Msg);

	InsertPatch (GAMEPORT_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (GAMEPORT_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
  InsertPatch (GAMEPORT_DEV, -12, 19, 0x84671c22, VE_SaferPatches1Msg);

	InsertPatch (KEYBOARD_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (KEYBOARD_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
  InsertPatch (KEYBOARD_DEV, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (KEYBOARD_DEV, -30, 17, 0x36264e70, VE_SlowResetMsg);

	InsertPatch (RAMDRIVE_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (RAMDRIVE_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
  InsertPatch (RAMDRIVE_DEV, -12, 19, 0x84671c22, VE_SaferPatches1Msg);

	InsertPatch (TIMER_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (TIMER_DEV, -30, 14, 0x4eaeffe2, VE_ROMUpdatesMsg);
  InsertPatch (TIMER_DEV, -12, 19, 0x84671c22, VE_SaferPatches1Msg);

	InsertPatch (TRACKDISK_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (TRACKDISK_DEV, -30, 14, 0xb090672a, VE_DiskCacheMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0x24, 0x4ebaf5d6, VE_PowerCacheMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0x1e, 0x4eba0164, VE_FacctionMsg);
	InsertPatch (TRACKDISK_DEV, -30, -0x154, MakeULONG (" HD "), VE_PowerHDFloppyMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0x48, 0x41ed0030, VE_QuietTDMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0x8, 0x526c0334, VE_CacheItMsg);
  InsertPatch (TRACKDISK_DEV, -30, 0, 0x9efc0038, VE_DynamicCacheMsg);
  InsertPatch (TRACKDISK_DEV, -36, 14, 0x206c07a8, VE_DynamicCacheMsg);
  InsertPatch (TRACKDISK_DEV, -12, 19, 0x84671c22, VE_SaferPatches1Msg);

	InsertPatch (CONSOLE_DEV, -18,   0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (CONSOLE_DEV, -30,   0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (CONSOLE_DEV, -30,  16, 0x24690018, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV,  -6, 364, 0x6100f75e, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -12,   6, 0x246b0018, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -24,  48, 0x6100008e, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -36,  46, 0x226b0004, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -42,  66, 0x6100ff7c, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -48,  66, 0x246b0018, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -54, 376, 0x6100e8a8, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -60, 282, 0x6100e8a8, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -66, 152, 0x6100e8a8, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -72,  94, 0x6100e8a8, VE_ROMUpdatesMsg);
	InsertPatch (CONSOLE_DEV, -18,   0, 0x70004e75, VE_ROMUpdatesMsg);
	InsertPatch (CONSOLE_DEV, -30,  34, 0x206e002c, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -12,  19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (CONSOLE_DEV,  -6,   7, 0x4928014a, VE_ConsoleDevMsg);
  InsertPatch (CONSOLE_DEV, -24,   0, 0x70004e75, VE_ConsoleDevMsg);
  InsertPatch (CONSOLE_DEV, -36,   9, 0x492f0e2c, VE_ConsoleDevMsg);
  InsertPatch (CONSOLE_DEV, -54,  13, 0xcc2c5f20, VE_ConsoleDevMsg);
  InsertPatch (CONSOLE_DEV, -60,   9, 0xce2f0e2c, VE_ConsoleDevMsg);
  InsertPatch (CONSOLE_DEV, -66,   5, 0xce2f0e2c, VE_ConsoleDevMsg);
  InsertPatch (CONSOLE_DEV, -72,   5, 0xce2f0e2c, VE_ConsoleDevMsg);

	InsertPatch (INPUT_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (INPUT_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (INPUT_DEV, -30, 0x0, 0x2f3afffa, VE_68040LibMsg);
	InsertPatch (INPUT_DEV, -30, 4, 0x0c690009, VE_68040LibMsg);
  InsertPatch (INPUT_DEV, -12, 19, 0x84671c22, VE_SaferPatches1Msg);

	InsertPatch (SCSI_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (SCSI_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (SCSI_DEV,  -6, 20, 0x00b04eae, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -12,  6, 0x266a0018, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -18, 10, 0x003c4a2d, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -24, 10, 0x003c4a2d, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -30, 12, 0x266a0018, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -36, 4, 0x2a69003c, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -30, -8, MakeULONG ("user"), VE_HDSleepMsg);
	InsertPatch (SCSI_DEV, -30, 0, 0x9efc000c, VE_PowerCacheMsg);
	InsertPatch (SCSI_DEV, -18, 13, 0x2430291c, VE_1230SCSIMsg);
	InsertPatch (SCSI_DEV, -24, 13, 0x2430291c, VE_1230SCSIMsg);
	InsertPatch (SCSI_DEV, -30,  9, 0x2430291c, VE_1230SCSIMsg);
	InsertPatch (SCSI_DEV, -36, 13, 0x2430291c, VE_1230SCSIMsg);
  InsertPatch (SCSI_DEV,  -6,  6, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -12, 14, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -18, 10, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -24, 10, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -30,  6, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -36, 10, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -30, 22, 0x206c07a8, VE_DynamicCacheMsg);
  InsertPatch (SCSI_DEV, -36, 14, 0x206c07a8, VE_DynamicCacheMsg);
  InsertPatch (SCSI_DEV, -30, 548, 0x61000292, VE_VMMMsg);
	InsertPatch (SCSI_DEV, -30,  6, 0x67282029, VE_HDOffMsg);
  InsertPatch (SCSI_DEV, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (SCSI_DEV, -30, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (SCSI_DEV,  -6, 29, 0x04640cd0, VE_AtapiDeviceMsg);
  InsertPatch (SCSI_DEV, -12, 21, 0xe6cb4e20, VE_AtapiDeviceMsg);
  InsertPatch (SCSI_DEV, -30,  7, 0x16640822, VE_AtapiDeviceMsg);

  InsertPatch (EXEC_LIB, -132,  2, 0x01274e75, VE_ROMPatchMsg);
	InsertPatch (EXEC_LIB, -528,  0, 0x42c04e75, VE_ROMPatchMsg);
  InsertPatch (EXEC_LIB,  -36,  7, 0x18660c4a, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB,  -42, 10, 0x270008ae, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB,  -48,  5, 0x2456c04a, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB,  -60,  2, 0x02303d7c, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB,  -66,  9, 0x2a675433, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -180, 11, 0x26700bb0, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -234,  4, 0x20126710, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -240,  0, 0x20102089, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -252,  4, 0x21490004, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -258,  8, 0xc1892348, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -264,  7, 0x04670a21, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -270, 15, 0x056ff420, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -306,  0, 0x226e0114, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -312,  0, 0x226e0114, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -318,  8, 0x226e0114, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -324, 12, 0x522e0126, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -354,  0, 0x41e90014, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -360,  0, 0x522e0127, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -366, 16, 0x522e0126, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -372,  7, 0x08660670, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -378,  0, 0x70072229, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -384,  0, 0x22680014, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -492, 12, 0x22680014, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -558,  0, 0x117c000f, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -570,  0, 0x522e0127, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -600, 34, 0x522e0127, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -606, 12, 0x22680014, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -408, -50, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -552, -52, MakeULONG ("RTPa"), VE_RTPatchMsg);
	InsertPatch (EXEC_LIB, -30, 18, 0x2f3a0336,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -54, 4, 0x2f0d4e6d,	VE_68060LibMsg);
	InsertPatch (EXEC_LIB, -162, 0, 0x2f3a01f0,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -168, 0, 0x2f3a01fa,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -282, 4, 0x2f3a0228,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -396, 0, 0x2f3a0246,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -432, 0, 0x2f3a0240,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -486, 0, 0x2f3a0236,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -648, 26, 0x007c0700,VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -726, 8, 0x700072ff,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -762, 12, 0x22088291,VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -768, 8, 0x22088291,	VE_68060LibMsg);
	InsertPatch (EXEC_LIB, -162,  0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -168,  0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -282,  0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -372,  7, 0x08672a33,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -396,  0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -414,  0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -432,  0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -486,  0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -642, 10, 0xffe24cdf,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -648,  4, 0xc0814681,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -762,  6, 0x22088291,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -768,  6, 0x22088291,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -522, 10, 0x2f4a000c,VE_LocaleLibMsg);
  InsertPatch (EXEC_LIB, -552, 11, 0x36071356,VE_AddPowerMsg);
  InsertPatch (EXEC_LIB, -624, -4, 0x2A417274,VE_CopyMemQuickerMsg);
  InsertPatch (EXEC_LIB, -630, 6, 0x651c722c,	VE_CopyMemQuickerMsg);
  InsertPatch (EXEC_LIB, -408, 11, 0x360713c9,VE_AddPowerMsg);
  InsertPatch (EXEC_LIB, -210, 2, 0x50504d4d,		VE_PPLibMsg);
	InsertPatch (EXEC_LIB, -108, 0, 0x08390001,	VE_AlertPatchMsg);
	InsertPatch (EXEC_LIB, -108, -2, 0xff944aba,VE_EnforcerMsg);
	InsertPatch (EXEC_LIB, -648, 0, 0x522e0127,	VE_EnforcerMsg);
	InsertPatch (EXEC_LIB, -726, -2, 0xfd2a2f3a,VE_EnforcerMsg);
  InsertPatch (EXEC_LIB, -108, 10, 0x48454c50, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -132, 4, 0x0c2e0078, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -132, 6, 0x6706522e, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -132, 0, 0x0c9700fe, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -222, 8, 0x362a000e, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -444, 6, 0x2c012e00, VE_ROMUpdatesMsg);
	InsertPatch (EXEC_LIB, -570, 4, 0x5368000e, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -678, 8, 0x66085268, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -714, 12, 0x64562f0b, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -726, 4, 0x0129670c, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -408, 0x18, 0x7265712e, VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -552, 0x16, 0x7265712e, VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -408, 0x2e, 0x7265712e, VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -552, 0x2c, 0x7265712e, VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -54, 12, 0x223a00ce, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -54, 12, 0x223a19a4, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -54, 12, 0x20542068, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -282, 16, 0x20290032, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -282, 18, 0x20290032, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -624, -8, 0x2a4d422a, VE_FastMemCopyMsg);
  InsertPatch (EXEC_LIB, -630, -4, 0x2a4d422a, VE_FastMemCopyMsg);
  InsertPatch (EXEC_LIB, -210, 2, 0x50504d4d, VE_PPLibMsg);
  InsertPatch (EXEC_LIB, -444, 0x2e, 0x74726163, VE_LibReqMsg);
  InsertPatch (EXEC_LIB, -552, 0xa, 0x266c80fa, VE_LibReqMsg);
  InsertPatch (EXEC_LIB, -408, 0xa, 0x52ac8ad2, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -414, 0xc, 0x52ac8ad2, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -552, 0x8, 0x52ac8ad2, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -108, 0x32, 0x20c7208d, VE_NewAlertMsg);
  InsertPatch (EXEC_LIB, -420, 0x30, 0x60000292, VE_SaferPatchesMsg);
  InsertPatch (EXEC_LIB, -204, 0x12, 0x0cae41fa, VE_ImmuneMsg);
  InsertPatch (EXEC_LIB, -114, -0x7e, MakeULONG ("CHW!"), VE_RomCrackMsg);
  InsertPatch (EXEC_LIB, -444, 0x4, 0x43fa008d, VE_A2232FixMsg);
  InsertPatch (EXEC_LIB, -198, 0x54, MakeULONG ("MemM"), VE_MemManMsg);
  InsertPatch (EXEC_LIB, -630, -4, MakeULONG ("*Art"), VE_CopyMemQuickerMsg);
  InsertPatch (EXEC_LIB, -444, 0x18, 0x52ac0708, VE_PrintManagerMsg);
  InsertPatch (EXEC_LIB, -444, 0x18, 0x52ac0888, VE_PrintManagerMsg);
  InsertPatch (EXEC_LIB, -108, 4, 0x103affe4, VE_AlertPatchMsg);
  InsertPatch (EXEC_LIB, -108, 8, 0x50ed00cc, VE_NOG2Msg);
  InsertPatch (EXEC_LIB, -54, 4, 0x49fa3e1c, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -282, 4, 0x49fa3da6, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -108, 4, 0x103afff8, VE_AlertPatchMsg);
  InsertPatch (EXEC_LIB, -108, 10, 0x60ba4a39, VE_GOMFMsg);
  InsertPatch (EXEC_LIB, -108, 0, 0x4eeefd2a, VE_SFAPatchMsg);
  InsertPatch (EXEC_LIB, -108, -2, 0xff944aba, VE_EnforcerMsg);
  InsertPatch (EXEC_LIB, -726, -2, 0xfd2a2f3a, VE_EnforcerMsg);
  InsertPatch (EXEC_LIB, -198, 4, 0x48e70020, VE_MungWallMsg);
  InsertPatch (EXEC_LIB, -210, 4, 0x48e70020, VE_MungWallMsg);
  InsertPatch (EXEC_LIB, -216, 4, 0x48e70020, VE_MungWallMsg);
  InsertPatch (EXEC_LIB, -408, 0xa, 0x52ad004a, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -552, 8, 0x52ad004a, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -282, 0x1a, 0x41fa0032, VE_SysExceptMsg);
  InsertPatch (EXEC_LIB, -114, 0x12, 0x41ed002f, VE_CheckDebugMsg);
  InsertPatch (EXEC_LIB, -282, 0x6, 0xcd4b2a49, VE_MultiUserLibMsg);
  InsertPatch (EXEC_LIB, -288, 0x1e, 0xd0fc0030, VE_MultiUserLibMsg);
  InsertPatch (EXEC_LIB, -108, 0, 0x08390001, VE_AlertPatchMsg);
  InsertPatch (EXEC_LIB, -36, 0, 0x2c5f082f, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -42, 0x12, 0x41ee0196, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -48, 6, 0x56c04a2e, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -54, 18, 0x302e0126, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -60, 0x12, 0x41ee0196, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -66, 0xe, 0x4bee0126, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -138, 0x16, 0x4bfa000a, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -252, 0, 0x20592251, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -270, 2, 0x12290009, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -306, 8, 0xc0814681, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -312, 0x12, 0xc0814681, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -318, 4, 0x23400016, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -324, 0, 0x41e9001a, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -384, 0, 0x22680014, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -456, 4, 0x2c690014, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -468, 2, 0x0129001e, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -474, 2, 0x0129001e, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -564, 4, 0x5268002c, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -108, 0xe, MakeULONG ("HELP"), VE_EnhancedAlertMsg);
  InsertPatch (EXEC_LIB, -108, 0x2, MakeULONG ("CUB!"), VE_PatchAlertMsg);
  InsertPatch (EXEC_LIB, -726, 0x2, 0x2c6d0008, VE_Reset1200Msg);
  InsertPatch (EXEC_LIB, -420, 0x24, 0x43ec0007, VE_SetManMsg);
  InsertPatch (EXEC_LIB, -402, 0x12, 0x287a005a, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -420, 0x1c, 0xb0aa0002, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -402, 0x22, 0x287a044c, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -420, 0x10, 0xb2ba09bc, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -198, 0x0, 0x08810002, VE_PatchAllocMemMsg);
  InsertPatch (EXEC_LIB, -456, 0x32, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -462, 0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -726, 0xa, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -456, 0x32+0x14, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -462, 0x28+0x14, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -726, 0xa+0x14, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -408, 0xa, 0x52ad009e, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -552, 8, 0x52ad009e, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -456, 0x32+0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -462, 0x28+0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -726, 0xa+0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -216, 0x0, 0xb0bafff2, VE_LowFragMsg);
  InsertPatch (EXEC_LIB, -684, 0x0, 0xb0baffde, VE_LowFragMsg);
  InsertPatch (EXEC_LIB, -468, 0, 0x20570c90, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -810, 0, 0x4a806a14, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, - 96, 0, 0xbffa005e, VE_SnoopDosMsg);	// 3.0
  InsertPatch (EXEC_LIB, -294, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -366, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -390, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -444, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -498, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -552, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -594, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -282, 8, 0x207a001c, VE_SmartCrashMsg);	// 1.1
  InsertPatch (EXEC_LIB,  -84, 5, 0x0028082a, VE_FastExecMsg);	// 2.9
  InsertPatch (EXEC_LIB, -396, 7, 0x80670e22, VE_FastExecMsg);	// 2.9
  InsertPatch (EXEC_LIB, -618, 3, 0x006540b0, VE_FastExecMsg);	// 2.9
  InsertPatch (EXEC_LIB, -366, 36, 0x226c0052, VE_NewMenuMsg);
  InsertPatch (EXEC_LIB, -408, 30, 0x266e0114, VE_BetterOpenLibsMsg);
  InsertPatch (EXEC_LIB, -444, 18, 0x49ed001c, VE_BetterOpenLibsMsg);
  InsertPatch (EXEC_LIB, -552, 28, 0x266e0114, VE_BetterOpenLibsMsg);
  InsertPatch (EXEC_LIB, -198, 12, 0x660650f9, VE_ShowMemMsg);
  InsertPatch (EXEC_LIB, -210, 12, 0x660650f9, VE_ShowMemMsg);
  InsertPatch (EXEC_LIB, -198, 16, 0x020000f8, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -204,  0, 0x22090281, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -210, 18, 0x76f8c283, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -216, 14, 0x01421001, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -684, 10, 0xb0bc0000, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -624, 10, 0x02010003, VE_CMQMsg);
  InsertPatch (EXEC_LIB, -624,  4, 0x32090801, VE_CMQMsg);	// 060
  InsertPatch (EXEC_LIB, -630,  0, 0xe4880c80, VE_CMQMsg);
  InsertPatch (EXEC_LIB, -198,  0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (EXEC_LIB, -198,  0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (EXEC_LIB, -420, 39, 0xc867220c, VE_MagicMenuMsg);
  InsertPatch (EXEC_LIB, -444, 22, 0x41ec8652, VE_PrintManager1Msg);
  InsertPatch (EXEC_LIB, -444, 42, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (EXEC_LIB, -450, 14, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (EXEC_LIB, -726, 44, 0x206a0030, VE_PatchBootMsg);
  InsertPatch (EXEC_LIB, -402, 46, 0x2469000a, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -420, 84, 0x2268000a, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -378,  0, 0x2011672e, VE_PatchReplyMsgMsg);
  InsertPatch (EXEC_LIB, -198, -14, MakeULONG ("NoFa"), VE_NoFastMemMsg);
  InsertPatch (EXEC_LIB, -216, -30, MakeULONG ("NoFa"), VE_NoFastMemMsg);
  InsertPatch (EXEC_LIB, -726, 182, 0x20680004, VE_ReKickResetMsg);
  InsertPatch (EXEC_LIB, -552, -176, MakeULONG ("ASL "), VE_PatchAslMsg);
  InsertPatch (EXEC_LIB, -456, 120, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -462, 110, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -726,  80, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -198, 22, MakeULONG ("Dyna"), VE_DynamicCacheMsg);
  InsertPatch (EXEC_LIB, -216, 16, MakeULONG ("Dyna"), VE_DynamicCacheMsg);
  InsertPatch (EXEC_LIB, -198, 324, MakeULONG ("Mung"), VE_MungMemMsg);
  InsertPatch (EXEC_LIB, -210, 238, MakeULONG ("Mung"), VE_MungMemMsg);
  InsertPatch (EXEC_LIB, -54, 86, 0x2f480012, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -198, 44, 0x61000292, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -210, 456, 0x61000292, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -216, 26, 0x6100ff34, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -282, 60, 0x20690036, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -318, 400, 0x6100ff16, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -762, 249, MakeULONG ("8HRN"), VE_VMMMsg);
  InsertPatch (EXEC_LIB, -768, 169, MakeULONG ("8HRN"), VE_VMMMsg);
  InsertPatch (EXEC_LIB, -198, 24, 0x2c6c2600, VE_ScalaMsg);
  InsertPatch (EXEC_LIB, -564, 504, 0x2669fff8, VE_ScalaMsg);
  InsertPatch (EXEC_LIB, -570, 410, 0x2669fff8, VE_ScalaMsg);
  InsertPatch (EXEC_LIB, -576, 458, 0x2669fff8, VE_ScalaMsg);
  InsertPatch (EXEC_LIB, -108, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -198, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -282, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -552, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -624, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -630, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -282, 14, 0x20680114, VE_SmartCrashMsg); // 1.3.0
  InsertPatch (EXEC_LIB,  -54, 16, 0x4eaeffc4, VE_ExecutiveMsg);
  InsertPatch (EXEC_LIB, -282,  0, 0x4eaeff7c, VE_ExecutiveMsg);
  InsertPatch (EXEC_LIB, -288,  0, 0x4eaeff7c, VE_ExecutiveMsg);
  InsertPatch (EXEC_LIB, -300, 10, 0xb07caaaa, VE_ExecutiveMsg);
	InsertPatch (EXEC_LIB, -444, 78, MakeULONG ("PARP"), VE_ParallelPatchMsg);
	InsertPatch (EXEC_LIB, -444, 78, MakeULONG ("PTCH"), VE_SerialPatchMsg);
  InsertPatch (EXEC_LIB, -198, 14, 0x610000a8, VE_FastRADMsg);
  InsertPatch (EXEC_LIB, -612,  8, 0x6100008c, VE_FastRADMsg);
  InsertPatch (EXEC_LIB, -354, 12, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (EXEC_LIB, -282,  8, 0x2f4b000c, VE_Dopus5LibMsg);
  InsertPatch (EXEC_LIB, -288, 16, 0x2f4e0008, VE_Dopus5LibMsg);
  InsertPatch (EXEC_LIB, -294, 12, 0x2f4e000c, VE_Dopus5LibMsg);
	InsertPatch (EXEC_LIB, -198, 26, 0x2069000a, VE_WB2FastMsg);
	InsertPatch (EXEC_LIB, -444, 54, 0x61007468, VE_PrintManager1Msg);
	InsertPatch (EXEC_LIB, -108, 92, 0x6100f950, VE_CyberGuardMsg);
  InsertPatch (EXEC_LIB, -684, 12, 0x226e0114, VE_PatchRAMMsg);
  InsertPatch (EXEC_LIB, -108,  5, 0x084a8728, VE_MMULibMsg);
//  InsertPatch (EXEC_LIB, -282, 26, 0x206c0d70, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -618,  3, 0x3e518f28, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -648, 17, 0x08670e20, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -726, 33, 0x84598f48, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -762, 13, 0x492a4808, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -768,  9, 0x03661808, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -186, 238, 0x6100020c, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -192, 140, 0x6100020c, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -198, 60, 0x61000e3e, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -204, 52, 0x61000992, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -210, 42, 0x61000baa, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -216, 30, 0x6100083e, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -684, 124, 0x61000e3e, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -690, 62, 0x61000baa, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -696, 32, 0x61000750, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -702, 30, 0x6100069c, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -708, 32, 0x6100062c, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -714, 34, 0x61000492, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -414, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (EXEC_LIB, -444, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (EXEC_LIB, -450, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (EXEC_LIB, -552, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (EXEC_LIB, -420, 46, 0x2269017a, VE_SnoopSetFunctionMsg);
  InsertPatch (EXEC_LIB, -210, 1, 0x09671241, VE_PpcLibMsg);
  InsertPatch (EXEC_LIB, -690, 1, 0x09671241, VE_PpcLibMsg);
  InsertPatch (EXEC_LIB, -282, 6, 0x660447fa, VE_PowerPcLibMsg);
  InsertPatch (EXEC_LIB, -288, 27, 0x80675424, VE_PowerPcLibMsg);
  InsertPatch (EXEC_LIB, -324,  9, 0x24220020, VE_PowerPcLibMsg);
  InsertPatch (EXEC_LIB, -378,  7, 0x08661e49, VE_PowerPcLibMsg);
  InsertPatch (EXEC_LIB, -408, 31, 0x186706B2, VE_PowerPcLibMsg);
  InsertPatch (EXEC_LIB, -552, 31, 0x186706B2, VE_PowerPcLibMsg);
  InsertPatch (EXEC_LIB, -504, 0, 0x2f0c49f9, VE_SashimiMsg);
  InsertPatch (EXEC_LIB, -510, 0, 0x2f0c7079, VE_SashimiMsg);
  InsertPatch (EXEC_LIB, -516, 2, 0x00c06100, VE_SashimiMsg);
  InsertPatch (EXEC_LIB, -198, 19, 0x76588f4c, VE_WipeOutMsg);
  InsertPatch (EXEC_LIB, -210, 19, 0x76588f4c, VE_WipeOutMsg);
  InsertPatch (EXEC_LIB, -684, 19, 0x76588f4c, VE_WipeOutMsg);
  InsertPatch (EXEC_LIB, -690, 19, 0x76588f4c, VE_WipeOutMsg);
  InsertPatch (EXEC_LIB, -696, 19, 0x76588f4c, VE_WipeOutMsg);
  InsertPatch (EXEC_LIB, -702, 19, 0x76588f4c, VE_WipeOutMsg);
  InsertPatch (EXEC_LIB, -708, 19, 0x76588f4c, VE_WipeOutMsg);
  InsertPatch (EXEC_LIB, -714, 19, 0x76588f4c, VE_WipeOutMsg);
  InsertPatch (EXEC_LIB, -108,  4, 0x6100f42a, VE_COPMsg);
  InsertPatch (EXEC_LIB, -114,  4, 0x6100f3d6, VE_COPMsg);
  InsertPatch (EXEC_LIB, -282, 14, 0x6100f20c, VE_COPMsg);
  InsertPatch (EXEC_LIB,  -30, 8, 0x0c6d4e73, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -162, 0, 0x2f3a01de, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -168, 0, 0x2f3a01ea, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -282, 4, 0x2f3a0220, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -396, 0, 0x2f3a023e, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -432, 0, 0x2f3a0238, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -486, 0, 0x2f3a022e, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -618, 8, 0x246b0258, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -762, 4, 0x08000001, VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -282, 9, 0xe061064c, VE_OldIconsMsg);
  InsertPatch (EXEC_LIB, -444, 19, 0xef00104c, VE_TurboSpoolMsg);
  InsertPatch (EXEC_LIB, -108,  5, 0x38000408, VE_DiskSafeMsg);
  InsertPatch (EXEC_LIB, -726, 10, 0xff7c4a6c, VE_DiskSafeMsg);
  InsertPatch (EXEC_LIB, -126, 1, 0x2e00ff01, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -138, 1, 0x2e00ff01, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -198, 1, 0x806b1c66, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -294, 1, 0x0967f60c, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -336, 3, 0xff66f448, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -354, 3, 0x0a66f448, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -390, 1, 0x2e00ff01, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -408, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -420, 1, 0xce6618b0, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -540, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -546, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -558, 3, 0xff224872, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -570, 3, 0x0e66f448, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -588, 3, 0xff28504a, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -594, 1, 0x2e00ff01, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -624, 3, 0xffb1c96d, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -630, 3, 0xff220824, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -654, 3, 0xff220867, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -672, 3, 0xff2a482c, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -684, 1, 0x806b1c66, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -198, 7, 0x7c4cdf03, VE_BDebugMsg);
  InsertPatch (EXEC_LIB, -210, 3, 0xc04eaeff, VE_BDebugMsg);
  InsertPatch (EXEC_LIB, -282, 9, 0x2e4eaeff, VE_BDebugMsg);
  InsertPatch (EXEC_LIB, -288, 5, 0x09660422, VE_BDebugMsg);
	InsertPatch (EXEC_LIB, -372, 7, 0x08660a43, VE_FixGetMsgMsg);
  InsertPatch (EXEC_LIB, -282,  8, 0x005848e7, VE_C_CPRMsg);
  InsertPatch (EXEC_LIB, -288,  8, 0x00084eae, VE_C_CPRMsg);
  InsertPatch (EXEC_LIB, -444, 18, 0x000823c0, VE_C_CPRMsg);
  InsertPatch (EXEC_LIB, -552, 10, 0x00242079, VE_C_CPRMsg);
  InsertPatch (EXEC_LIB, -420, 10, 0x001849f9, VE_LibWatchMsg);
  InsertPatch (EXEC_LIB,  -54,  3, 0xc2660258, VE_PMMsg);
  InsertPatch (EXEC_LIB, -108, 20, 0x2c6c0d68, VE_MuForceMsg);
  InsertPatch (EXEC_LIB, -282, 26, 0x206c0dd4, VE_MuForceMsg);
  InsertPatch (EXEC_LIB,  -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (EXEC_LIB,  -18, 17, 0xd8671826, VE_SaferPatches1Msg);
	InsertPatch (EXEC_LIB, -420, 13, 0x27284926, VE_SaferPatches1Msg);
	InsertPatch (EXEC_LIB, -612,  7, 0xf64e9028, VE_MuFastZeroMsg);
  InsertPatch (EXEC_LIB, -444, 19, 0x5f20404c, VE_SetDeviceMsg);
  InsertPatch (EXEC_LIB, -198, 1, 0x802f0058, VE_MemTrailerMsg);
  InsertPatch (EXEC_LIB, -210, 1, 0x80b3f108, VE_MemTrailerMsg);
  InsertPatch (EXEC_LIB, -198, 15, 0xf82f004e, VE_MemTrailerMsg);
  InsertPatch (EXEC_LIB, -210, 23, 0xf8d1c0b3, VE_MemTrailerMsg);


  InsertPatch (EXEC_LIB, -198, 5, 0x006f0682, VE_MemSpeed2Msg);
  InsertPatch (EXEC_LIB, -198, 17, 0x07220620, VE_mTrackMsg);
  InsertPatch (EXEC_LIB, -210, 19, 0x54670820, VE_mTrackMsg);
  InsertPatch (EXEC_LIB, -288, 17, 0x54671220, VE_mTrackMsg);
  InsertPatch (EXEC_LIB, -768, 7, 0x0f67e443, VE_EnforcerMsg);
  InsertPatch (EXEC_LIB, -198, 13, 0x06663024, VE_ForceFastMemMsg);
  InsertPatch (EXEC_LIB, -198, 21, 0x184e902a, VE_DumpTaskMemMsg);
  InsertPatch (EXEC_LIB, -210, 27, 0x144e904c, VE_DumpTaskMemMsg);
  InsertPatch (EXEC_LIB, -684, 21, 0x204e902a, VE_DumpTaskMemMsg);
  InsertPatch (EXEC_LIB, -690, 23, 0x1c4e904c, VE_DumpTaskMemMsg);

  InsertPatch (UTIL_LIB, -162, 14, 0x206e008c,			VE_LocaleLibMsg);
  InsertPatch (UTIL_LIB, -168, 12, 0x206e008c,			VE_LocaleLibMsg);
  InsertPatch (UTIL_LIB, -174, 10, 0x206e008c,			VE_LocaleLibMsg);
  InsertPatch (UTIL_LIB, -180, 10, 0x206e008c,			VE_LocaleLibMsg);
	InsertPatch (UTIL_LIB, -198, 0,  0x518f4857,			VE_68060LibMsg);
	InsertPatch (UTIL_LIB, -204, 0,  0x518f4857,			VE_68060LibMsg);
  InsertPatch (UTIL_LIB, -198, 102, MakeULONG ("t64_"), VE_PatchMult64Msg);
  InsertPatch (UTIL_LIB, -204, 182, MakeULONG ("t64_"), VE_PatchMult64Msg);
  InsertPatch (UTIL_LIB, -0x8a, 0, 0x4c010800, VE_ROMPatchMsg);
  InsertPatch (UTIL_LIB, -0x90, 0, 0x4c010000, VE_ROMPatchMsg);
  InsertPatch (UTIL_LIB, -0x96, 0, 0x4c410801, VE_ROMPatchMsg);
  InsertPatch (UTIL_LIB, -0x9c, 0, 0x4c410001, VE_ROMPatchMsg);
  InsertPatch (UTIL_LIB, -0xc6, 0, 0x4c010c01, VE_ROMPatchMsg);
  InsertPatch (UTIL_LIB, -0xcc, 0, 0x4c010401, VE_ROMPatchMsg);
  InsertPatch (UTIL_LIB, -198, 5, 0x002a0067, VE_FixLongMultMsg);
  InsertPatch (UTIL_LIB, -204, 5, 0x00672e2e, VE_FixLongMultMsg);
  InsertPatch (UTIL_LIB,  -60, 3, 0x00000167, VE_PatchWorkMsg);
  InsertPatch (UTIL_LIB, -150, 1, 0x8166f648, VE_PatchWorkMsg);
  InsertPatch (UTIL_LIB, -156, 1, 0x8166f648, VE_PatchWorkMsg);
  InsertPatch (UTIL_LIB, -228, 3, 0xff2a4920, VE_PatchWorkMsg);
  InsertPatch (UTIL_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (UTIL_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);

	InsertPatch (GFX_LIB, -0x001E, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0024, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0036, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x003C, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0060, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0072, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0084, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x00B4, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x00C0, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x00D2, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x00D8, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x00DE, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x00EA, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x00F6, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0108, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0120, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x012C, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0132, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0138, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x013E, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0144, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x014A, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x018C, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0192, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x019E, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x01A4, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x01AA, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x01EC, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x01F8, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x01FE, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0204, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x020A, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0210, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0216, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x022E, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0234, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0246, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x024C, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0264, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x026A, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0270, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x027C, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x02A6, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x02B2, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x02B8, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x02D6, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x02DC, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x02F4, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0300, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0306, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x030C, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0312, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x031E, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x032A, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0354, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0372, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0384, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0396, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x039C, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03A8, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03AE, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03C0, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03C6, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03CC, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03DE, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03E4, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03EA, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x03FC, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0402, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0408, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x041A, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
	InsertPatch (GFX_LIB, -0x0420, -20, MakeULONG ("CYBG"), VE_CGXLibMsg);
  InsertPatch (GFX_LIB, -228, 8, 0x671a33fc, VE_SystemPatchMsg);
  InsertPatch (GFX_LIB, -270, 2, 0x486e00c8, VE_SystemPatchMsg);
  InsertPatch (GFX_LIB, -276, 5, 0x0c429122, VE_SystemPatchMsg);
  InsertPatch (GFX_LIB, -294, 7, 0xd4b04163, VE_SystemPatchMsg);
  InsertPatch (GFX_LIB, -462, 3, 0xaa6d482f, VE_SystemPatchMsg);
  InsertPatch (GFX_LIB, -30, 0, 0x2f074eb9,					VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -192, 2, 0x246e01a4, 				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -288, 2, 0x246e01a4, 				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -588, 0x8, 0x67102268, 			VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -636, 0x12, 0x2f202f20, 		VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -660, 0x2, 0x207afff8, 			VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -666, 0x8, 0x102b0008, 			VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -672, 0x14, 0x254b0018, 		VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -702, 0xa, 0x200b618a, 			VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -1056, 0, 0x48e70022,				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -714, 0, 0x2f3afffa, 				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -726, 0, 0x2f3afffa, 				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -732, 0, 0x2f3afffa, 				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -750, 0, 0x2f3afffa, 				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -756, 0, 0x2f3afffa, 				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -798, 0, 0x2f3afffa, 				VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -942, 0x1a, 0xb0aa0040, 		VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -1050, 0x3e, 0x4e922246, 		VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -1056, 0x0, 0x48e70022, 		VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -60, 0x4c, 0x45fb107a, VE_FastFontsMsg);
  InsertPatch (GFX_LIB, -60, 0x6c, 0x17520059, VE_TempoTopazMsg);
  InsertPatch (GFX_LIB, -60, 0x46, 0x175a0059, VE_TurboTopazMsg);
  InsertPatch (GFX_LIB, -60, 0x84, 0x195b0059, VE_BlitzFontsMsg);
  InsertPatch (GFX_LIB, -300, -10, MakeULONG ("CPUC"), VE_CPUClrCarnivoreMsg);
  InsertPatch (GFX_LIB, -300, 0x42, MakeULONG ("FAST"), VE_CPUClrPeterMsg);
  InsertPatch (GFX_LIB, -300, 0x74, MakeULONG ("FAST"), VE_CPUClrPeterMsg);
  InsertPatch (GFX_LIB, -222, 0xa, 0x4e9041f9, VE_MWAMsg);
  InsertPatch (GFX_LIB, -222, 0x8, 0x2f280450, VE_QMouseMsg);
  InsertPatch (GFX_LIB, -30, 0, 0x4a4467fa, VE_CPUBlitMsg);
  InsertPatch (GFX_LIB, -300, 2, 0x08010001, VE_CPUClrUnicornMsg);
  InsertPatch (GFX_LIB, -228, 0x1e, 0x44c00110, VE_FastWaitBlitMsg);
  InsertPatch (GFX_LIB, -210, 0xf8, MakeULONG ("gcop"), VE_SetPatchMrgCopMsg);
  InsertPatch (GFX_LIB, -42, 0xe, 0x9269003e, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -48, 0x10, 0x9269003e, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -54, 0x0, 0x48c067fa, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -198, 0x4, 0x721220c0, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -324, 0x8, 0x00a22a42, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -342, 0x0, 0x13400019, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -348, 0x0, 0x1340001a, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -354, 0x2, 0x1340001c, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -456, 0x0, 0x526e00aa, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -462, 0x0, 0x536e00aa, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -54, 0x0, 0x48c067f8, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -66, 0x8, 0x2f0993c9, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -270, 0x6, 0x4bee00c4, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -300, 0xa, 0x526e00aa, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -306, 0x6, 0x78002044, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -324, 0xa, 0x2a4261c2, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -354, 0x0, 0x1340001c, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -432, 0x2, 0x41ed0048, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -438, 0x2, 0x41ed0048, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -270, 0x2, 0x486e00c8, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -306, 0x2, 0x78002044, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -324, 0x52, 0x0242fff0, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -774, 0x1a, 0x0242fff0, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -786, 0x10, 0x0244fff0, VE_FasterBlitMsg);
  InsertPatch (GFX_LIB, -222, 0x20, 0x41ec0174, VE_TWAMsg);
  InsertPatch (GFX_LIB, -60, 0x10, 0x0c1800b6, VE_ProWizardMsg);
  InsertPatch (GFX_LIB, -216, 0x18, 0x5369001e, VE_MakeVPortPatchMsg);
  InsertPatch (GFX_LIB, -228, 0xe, 0x66f64a39, VE_FastWaitBlitDaveMsg);
  InsertPatch (GFX_LIB, -222,   -6, MakeULONG ("AMOS"), VE_AMOSLibMsg);
  InsertPatch (GFX_LIB, -774,    4, 0x26690004        , VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -774,    0, 0x61064eae				, VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -786,    4, 0x944048c2        , VE_ROMUpdatesMsg);
  InsertPatch (GFX_LIB, -72,     0, 0xbffa005e				, VE_SnoopDosMsg);	// 3.0
  InsertPatch (GFX_LIB, -30,  0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -36,  0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -96,  0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -132, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -234, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -246, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -264, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -294, 0, 0x602e000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -300, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -312, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -678, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -918, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -30,  0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -36,  0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -60,  0, 0x60220000, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -114, 0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -234, 0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -246, 0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -264, 0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -300, 0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -312, 0, 0x6022000b, VE_FBlitMsg);
	InsertPatch (GFX_LIB, -330, 0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -678, 0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -918, 0, 0x6026000b, VE_FBlitMsg);
  InsertPatch (GFX_LIB, -60,  4, 0x4a406700, VE_FTextMsg);
  InsertPatch (GFX_LIB, -222, 26, 0x2c6c0520, VE_MagicCXMsg);
  InsertPatch (GFX_LIB,  -60, 10, 0x266cfdd8, VE_NewIconsMsg);
  InsertPatch (GFX_LIB,  -96, 10, 0x266cfdd8, VE_NewIconsMsg);
  InsertPatch (GFX_LIB, -246, 20, 0x206cfdd8, VE_NewIconsMsg);
  InsertPatch (GFX_LIB, -306, 20, 0x206cfdd8, VE_NewIconsMsg);
  InsertPatch (GFX_LIB, -606, 10, 0x266cfdd8, VE_NewIconsMsg);
  InsertPatch (GFX_LIB, -396, 38, 0x266c0dfe, VE_CEDFixMsg);
  InsertPatch (GFX_LIB, -246, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (GFX_LIB, -306, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (GFX_LIB, -606, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (GFX_LIB, -774, 50, 0x61000088, VE_NewWPA8Msg);
  InsertPatch (GFX_LIB, -786, 82, 0x61000026, VE_NewWPA8Msg);
  InsertPatch (GFX_LIB, -1056, 74, 0x2c6c0044, VE_NewWPA8Msg);
  InsertPatch (GFX_LIB, -774, 24, 0x22690004, VE_PatchWPA8Msg);
  InsertPatch (GFX_LIB, -786, 12, 0x2f4a0008, VE_PatchWPA8Msg);
  InsertPatch (GFX_LIB,  -774, 10, 0x2802760f, VE_BlazeWCPMsg);
  InsertPatch (GFX_LIB,  -786, 14, 0x98807af0, VE_BlazeWCPMsg);
  InsertPatch (GFX_LIB, -1056,  8, 0x94805282, VE_BlazeWCPMsg);
  InsertPatch (GFX_LIB, -1056, 10, 0x94805282, VE_BlazeWCPMsg);
  InsertPatch (GFX_LIB,  -774,  5, 0x023601d4, VE_BlazeWCPMsg);
  InsertPatch (GFX_LIB,  -786,  5, 0x027af098, VE_BlazeWCPMsg);
  InsertPatch (GFX_LIB, -1056,  9, 0x0f944042, VE_BlazeWCPMsg);
  InsertPatch (GFX_LIB,  -60,  8, 0x670000ce, VE_Birdie2000Msg);
  InsertPatch (GFX_LIB, -246,  8, 0x08290003, VE_Birdie2000Msg);
  InsertPatch (GFX_LIB, -306, 14, 0x1c29001c, VE_Birdie2000Msg);
  InsertPatch (GFX_LIB, -606, 14, 0x2e290000, VE_Birdie2000Msg);
  InsertPatch (GFX_LIB, -636, 14, 0x2e290000, VE_Birdie2000Msg);
  InsertPatch (GFX_LIB, -840, 10, 0x2f480018, VE_ObtainBetterPensMsg);
	InsertPatch (GFX_LIB, -60,  9, 0xc84a9167, VE_TitleShadowMsg);
	InsertPatch (GFX_LIB, -60, 26, 0x246a0028, VE_TitleShadowMsg);
  InsertPatch (GFX_LIB, -72, 64, MakeULONG ("paz."), VE_TopazPatchMsg);
  InsertPatch (GFX_LIB, -30, 0, 0x9efc00e8, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -60, 0, 0x9efc0068, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -96, 0, 0x9efc000c, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -114, 0, 0x9efc0028, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -132, 0, 0x9efc0014, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -180, 16, 0x246d002c, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -222, 0, 0x9efc0054, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -234, 46, 0x206a0004, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -246, 0, 0x9efc00c4, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -264, 0, 0x9efc00e0, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -270, 0, 0x9efc003c, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -300, 22, 0x2c6e01a4, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -312, 0, 0x9efc0038, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -318, 110, 0x266a0004, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -324, 60, 0x24690004, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -330, 0, 0x9efc0064, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -402, 0, 0x9efc003c, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -432, 194, 0x2a6e0114, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -438, 150, 0x2a6e0114, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -588, 12, 0x206c0004, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -636, 10, 0x22690004, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -654, 106, 0x2a6e0114, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -678, 0, 0x9efc0054, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -768, 0, 0x9efc00e8, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -774, 0, 0x9efc0014, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -786, 0, 0x9efc0020, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -840, 0, 0x9efc0088, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -852, 0, 0x9efc0030, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -900, 0, 0x9efc0010, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -918, 0, 0x9efc0034, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -924, 0, 0x9efc001c, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -942, 0, 0x9efc0020, VE_RtgLibMsg);
	InsertPatch (GFX_LIB,  -36,  10, 0x00105588, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -960, 138, 0x9efc0078, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -966,  20, 0x9efc0020, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -972,  10, 0x9efc0020, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -984,   2, 0x20690004, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -990,   4, 0x206c0004, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -1056,  0, 0x9efc0020, VE_RtgLibMsg);
  InsertPatch (GFX_LIB, -840, 17, 0xfe28017c, VE_FPPrefsMsg);
  InsertPatch (GFX_LIB, - 72,  28, 0x226c0028, VE_FakeTopazMsg);
  InsertPatch (GFX_LIB, -72, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (GFX_LIB, -78, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (GFX_LIB, -60, 5, 0x3e2e0026, VE_OutLineFontMsg);
  InsertPatch (GFX_LIB, -246, 5, 0x08000866, VE_OldIconsMsg);
  InsertPatch (GFX_LIB, -306, 5, 0x08000866, VE_OldIconsMsg);
  InsertPatch (GFX_LIB, -576, 16, 0xb7ec802c, VE_TragicWBMsg);
  InsertPatch (GFX_LIB, -882, 26, 0x672c0c53, VE_TragicWBMsg);
  InsertPatch (GFX_LIB, -954, 21, 0xfc4a876D, VE_TragicWBMsg);
  InsertPatch (GFX_LIB,   -66, 1, 0x08661a48, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -180, 1, 0x4263044a, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -186, 1, 0x4263044a, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -216, 7, 0x24661220, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -306, 1, 0x426e04b2, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -420, 7, 0x04671ed6, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -576, 1, 0x0866f648, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -678, 3, 0xff302800, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -684, 3, 0xff62180c, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -810, 1, 0x426e04b2, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -888, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -930, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -942, 3, 0xff266800, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -990, 1, 0x8066f648, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB, -1020, 3, 0xff204920, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB, -1050, 3, 0xff2a4820, VE_PatchWorkMsg);
  InsertPatch (GFX_LIB,  -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (GFX_LIB,  -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (GFX_LIB, -192, 9, 0x542948fd, VE_MagicTVMsg);
  InsertPatch (GFX_LIB, -216, 9, 0x542948fd, VE_MagicTVMsg);
  InsertPatch (GFX_LIB, -288, 9, 0x542948fd, VE_MagicTVMsg);
  InsertPatch (GFX_LIB, -852, 9, 0x542948fd, VE_MagicTVMsg);
  InsertPatch (GFX_LIB, -882, 9, 0x542948fd, VE_MagicTVMsg);
  InsertPatch (GFX_LIB, -924, 9, 0x542948fd, VE_MagicTVMsg);
  InsertPatch (GFX_LIB, -768, 15, 0x8467664a, VE_RPPMsg);
  InsertPatch (GFX_LIB, -396, 7, 0xf2285061, VE_BlitScrollMsg);
  InsertPatch (GFX_LIB,  -60, 3, 0x3866104a, VE_FastFontsMsg);
  InsertPatch (GFX_LIB,  -60, 25, 0x3c7e002a, VE_VisualPrefsMsg);
  InsertPatch (GFX_LIB, -306,  5, 0x3e2a492c, VE_VisualPrefsMsg);
  InsertPatch (GFX_LIB, -606,  7, 0x2ec3464b, VE_VisualPrefsMsg);
  InsertPatch (GFX_LIB, -288, 21, 0xc0662e28, VE_RtgLibMsg);
	InsertPatch (GFX_LIB, -222, 7, 0x042f3a00, VE_AMOSPatcherMsg);

  InsertPatch (INT_LIB,  -66, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB,  -78, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB,  -90, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB,  -96, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -198, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -210, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -402, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -510, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -576, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -612, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -768, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -774, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -822, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (INT_LIB, -612,  0, 0x602a0000, VE_FBlitMsg);
  InsertPatch (INT_LIB, -54, 40, 0x00384e91,	VE_ToolsDaemonMsg);
  InsertPatch (INT_LIB, -264, 46, 0x4eb00800,	VE_ToolsDaemonMsg);
  InsertPatch (INT_LIB, -702, 46, 0x4eb00800,	VE_ToolsDaemonMsg);
  InsertPatch (INT_LIB, -78, 5, 0x40660470,		VE_IPrefsMsg);
  InsertPatch (INT_LIB, -78, 0, 0x6100e406,		VE_IPrefsMsg);
  InsertPatch (INT_LIB, -78, 0, 0x61002162, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -96, 48, 0x4e55ffa4,	VE_IPrefsMsg);
  InsertPatch (INT_LIB, -612, 40, 0x41faffcc,	VE_IPrefsMsg);
  InsertPatch (INT_LIB, -66, 32, 0x202c0382,	VE_PowerSnapMsg);
  InsertPatch (INT_LIB, -96, 0, 0x2017b0ba, VE_ROMUpdatesMsg);
  InsertPatch (INT_LIB, -612, 4, 0x2f3afff6, VE_ROMUpdatesMsg);
  InsertPatch (INT_LIB, -276, 0x6a, MakeULONG ("Amig"), VE_LoadWBMsg);
  InsertPatch (INT_LIB, -588, 0x3a, 0x4dec0016, VE_NoReqMsg);
  InsertPatch (INT_LIB, -588, 0x16, 0x226dff74, VE_AssignXMsg);
  InsertPatch (INT_LIB, -348, 0x4, 0x7c002849, VE_WReqMsg);
  InsertPatch (INT_LIB, -96, 4, 0x2c7afff6, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -348, 0x20, 0x53ac809a, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -588, 4, 0x61000082, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -348, 0, 0x13fc00ff, VE_AutoRebootMsg);
  InsertPatch (INT_LIB, -270, 4, 0x48e73830, VE_NickPrefsMsg);
  InsertPatch (INT_LIB, -606, 4, 0x48e73830, VE_NickPrefsMsg);
  InsertPatch (INT_LIB, -348, 0x22, 0x53ac89a6, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -588, 4, 0x6100008c, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -348, 0xc, 0x487afe58, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -348, 0xe, 0x487afdec, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -588, 0xc, 0x487afec4, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -588, 0xa, 0x487afe88, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -588, 10, 0x205f7000,  VE_RTPatchMsg);
  InsertPatch (INT_LIB, -612, 0x8, 0x207afde0, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -612, 0x4, 0x207afdee, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -612, -224, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (INT_LIB, -198, 0xd4, MakeULONG ("SysI"), VE_TagScreensMsg);
  InsertPatch (INT_LIB, -198, 0xd4, MakeULONG ("Cygn"), VE_TagScreensMsg);
  InsertPatch (INT_LIB, -612, -20, MakeULONG ("usEd"), VE_TagScreensMsg);
  InsertPatch (INT_LIB, -54, 0x2c, 0x4eaefdcc, VE_PopUpMenuMsg);
  InsertPatch (INT_LIB, -180, 0x18, 0x4eaefdcc, VE_PopUpMenuMsg);
  InsertPatch (INT_LIB, -192, 0x22, 0x4eaefdcc, VE_PopUpMenuMsg);
  InsertPatch (INT_LIB, -264, 0x36, 0x4eaefdcc, VE_PopUpMenuMsg);
  InsertPatch (INT_LIB, -348, 0x8, 0x2a6c8712, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -588, 0xa, 0x2a6c8b4e, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -54, 0x1e, 0x4eaefd5a, VE_PopUpMenuMsg);
  InsertPatch (INT_LIB, -180, 0x12, 0x4eaefd5a, VE_PopUpMenuMsg);
  InsertPatch (INT_LIB, -192, 0x18, 0x4eaefd5a, VE_PopUpMenuMsg);
  InsertPatch (INT_LIB, -60, 10, 0x43e93662, VE_KCommMsg);
  InsertPatch (INT_LIB, -66, 10, 0x43e93b12, VE_KCommMsg);
  InsertPatch (INT_LIB, -72, 0x12, 0xb1e93bda, VE_KCommMsg);
  InsertPatch (INT_LIB, -96, 0xc, 0x202c3c1e, VE_KCommMsg);
  InsertPatch (INT_LIB, -198, 0x18, 0x4a2c449b, VE_KCommMsg);
  InsertPatch (INT_LIB, -246, 0x4, 0x3bf6614e, VE_KCommMsg);
  InsertPatch (INT_LIB, -252, 0x4, 0x3bf2615a, VE_KCommMsg);
  InsertPatch (INT_LIB, -270, 0x16, 0x45ed3662, VE_KCommMsg);
  InsertPatch (INT_LIB, -336, 0x4, 0x3bfe6136, VE_KCommMsg);
  InsertPatch (INT_LIB, -342, 0x4, 0x3bfa6142, VE_KCommMsg);
  InsertPatch (INT_LIB, -348, 0x4, 0x3c066008, VE_KCommMsg);
  InsertPatch (INT_LIB, -588, 0x4, 0x3c0a4e71, VE_KCommMsg);
  InsertPatch (INT_LIB, -612, 0x4, 0x3bee616e, VE_KCommMsg);
  InsertPatch (INT_LIB, -348, 0x0, 0x207a000c, VE_FixReqMsg);
  InsertPatch (INT_LIB, -348, 0x2, MakeULONG ("RP12"), VE_ReqPatchMsg);
  InsertPatch (INT_LIB, -72, 0x8, 0x4bfa01c6, VE_ZoomWindowsMsg);
  InsertPatch (INT_LIB, -204, 0xc, 0x2a7a0246, VE_ZoomWindowsMsg);
  InsertPatch (INT_LIB, -66, 0x1a, 0x206c0008, VE_ArqMsg);
  InsertPatch (INT_LIB, -96, 0x4, 0x487afd64, VE_ArqMsg);
  InsertPatch (INT_LIB, -348, 0xc, 0x48ef0d0e, VE_ArqMsg);
  InsertPatch (INT_LIB, -588, 0x12, 0x20064281, VE_ArqMsg);
  InsertPatch (INT_LIB, -348, 0xe, 0x487afdc8, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -588, 0x26, 0x487afe64, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -612, 0x8, 0x207afde2, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -198, 0xc, 0x02401000, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -60, 10, 0x43e9375a, VE_KCommMsg);
  InsertPatch (INT_LIB, -66, 10, 0x43e93c0a, VE_KCommMsg);
  InsertPatch (INT_LIB, -72, 0x12, 0xb1e93cd2, VE_KCommMsg);
  InsertPatch (INT_LIB, -198, 0xe, 0x4a2c448f, VE_KCommMsg);
  InsertPatch (INT_LIB, -246, 0x4, 0x3cee614e, VE_KCommMsg);
  InsertPatch (INT_LIB, -252, 0x4, 0x3cea615a, VE_KCommMsg);
  InsertPatch (INT_LIB, -270, 0x16, 0x45ed375a, VE_KCommMsg);
  InsertPatch (INT_LIB, -336, 0x4, 0x3cf66136, VE_KCommMsg);
  InsertPatch (INT_LIB, -342, 0x4, 0x3cf26142, VE_KCommMsg);
  InsertPatch (INT_LIB, -348, 0x4, 0x3cfe6008, VE_KCommMsg);
  InsertPatch (INT_LIB, -588, 0x4, 0x3d024e71, VE_KCommMsg);
  InsertPatch (INT_LIB, -612, 0xe, 0x4a2c448f, VE_KCommMsg);
  InsertPatch (INT_LIB, -66, 0x64, 0x41727100, VE_ArqMsg);
  InsertPatch (INT_LIB, -96, 0x8, 0x487afdb4, VE_ArqMsg);
  InsertPatch (INT_LIB, -348, 0xa, 0x48ed0f02, VE_ArqMsg);
  InsertPatch (INT_LIB, -588, 0x1a, 0x2440200a, VE_ArqMsg);
  InsertPatch (INT_LIB, -198, 2, 0x2a7afbd4, VE_SwitchNTSCMsg);
  InsertPatch (INT_LIB, -612, 2, 0x2a7afff8, VE_SwitchNTSCMsg);
  InsertPatch (INT_LIB, -60, 4, 0x227a069a, VE_QMouseMsg);
  InsertPatch (INT_LIB, -66, 4, 0x227a063a, VE_QMouseMsg);
  InsertPatch (INT_LIB, -72, 12, 0x227a0604, VE_QMouseMsg);
  InsertPatch (INT_LIB, -96, 0, 0x227a070a, VE_QMouseMsg);
  InsertPatch (INT_LIB, -198, 0, 0x303c0440, VE_QMouseMsg);
  InsertPatch (INT_LIB, -246, 0, 0x303c043c, VE_QMouseMsg);
  InsertPatch (INT_LIB, -252, 0, 0x303c0438, VE_QMouseMsg);
  InsertPatch (INT_LIB, -270, 8, 0x2a7a06f2, VE_QMouseMsg);
  InsertPatch (INT_LIB, -336, 0, 0x303c044c, VE_QMouseMsg);
  InsertPatch (INT_LIB, -342, 0, 0x303c0448, VE_QMouseMsg);
  InsertPatch (INT_LIB, -612, 0, 0x303c0444, VE_QMouseMsg);
  InsertPatch (INT_LIB, -72, 4, 0x4eba0558, VE_QuickMenusMsg);
  InsertPatch (INT_LIB, -204, 4, 0x4eba053c, VE_QuickMenusMsg);
  InsertPatch (INT_LIB, -306, 4, 0x4eba0520, VE_QuickMenusMsg);
  InsertPatch (INT_LIB, -312, 4, 0x4eba0504, VE_QuickMenusMsg);
  InsertPatch (INT_LIB, -306, 0, 0x48e70004, VE_WTFPatchMsg);
  InsertPatch (INT_LIB, -312, 0, 0x48e70004, VE_WTFPatchMsg);
  InsertPatch (INT_LIB, -96, 0x4, 0x487afd8c, VE_ArqMsg);
  InsertPatch (INT_LIB, -348, 0xc, 0x487aff12, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -588, 0xc, 0x487aff7e, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -612, 0x4, 0x207aff5e, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -270, 0, 0x2f0db07c, VE_PointerXMsg);
  InsertPatch (INT_LIB, -66, 8, 0x43e9148e, VE_KCommMsg);
  InsertPatch (INT_LIB, -78, 0x10, 0x102c42ec, VE_KCommMsg);
  InsertPatch (INT_LIB, -96, 0xa, 0x202c160c, VE_KCommMsg);
  InsertPatch (INT_LIB, -198, 8, 0x4a284334, VE_KCommMsg);
  InsertPatch (INT_LIB, -210, 0x6, 0x6100fae2, VE_KCommMsg);
  InsertPatch (INT_LIB, -246, 0x4, 0x15e4614a, VE_KCommMsg);
  InsertPatch (INT_LIB, -252, 0x4, 0x15e06156, VE_KCommMsg);
  InsertPatch (INT_LIB, -336, 0x4, 0x15ec6132, VE_KCommMsg);
  InsertPatch (INT_LIB, -342, 0x4, 0x15e8613e, VE_KCommMsg);
  InsertPatch (INT_LIB, -348, 0x4, 0x15f46008, VE_KCommMsg);
  InsertPatch (INT_LIB, -588, 0x4, 0x15f84e71, VE_KCommMsg);
  InsertPatch (INT_LIB, -612, 0xe, 0x4a2c4334, VE_KCommMsg);
  InsertPatch (INT_LIB, -510, 0x16, 0x226c008c, VE_FrontPubScreenMsg);
  InsertPatch (INT_LIB, -606, 0x1a, 0x2c6c0080, VE_FrontPubScreenMsg);
  InsertPatch (INT_LIB, -612, 0x28, 0x41faffcc, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -198, 0x0, 0x6100002c, VE_GOMFMsg);
  InsertPatch (INT_LIB, -204, 0x0, 0x6100005a, VE_GOMFMsg);
  InsertPatch (INT_LIB, -588, 0x2e, 0x4eaefd2a, VE_SFAPatchMsg);
  InsertPatch (INT_LIB, -198, 0x1c, 0x246c802a, VE_BorderBlankMsg);
  InsertPatch (INT_LIB, -210, 0x12, 0x246c8032, VE_BorderBlankMsg);
  InsertPatch (INT_LIB, -612, 0x24, 0x246c802e, VE_BorderBlankMsg);
  InsertPatch (INT_LIB, -348, 0x10, 0x01c56610, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -588, 0x10, 0x01c5660a, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -348, 6, 0x20290006, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -588, 6, 0x67047000, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -612, 8, 0x207afdda, VE_RTPatchMsg);
  InsertPatch (INT_LIB, -198, 0xa, 0x4a3afa51, VE_AddPowerMsg);
  InsertPatch (INT_LIB, -204, 0xa, 0x28492b09, VE_AddPowerMsg);
  InsertPatch (INT_LIB, -264, 0x0, 0x4a3af976, VE_AddPowerMsg);
  InsertPatch (INT_LIB, -588, 0x4, 0x4a3afc70, VE_AddPowerMsg);
  InsertPatch (INT_LIB, -606, 0x8, 0x28492b09, VE_AddPowerMsg);
  InsertPatch (INT_LIB, -612, 0x8, 0x4a3afa51, VE_AddPowerMsg);
  InsertPatch (INT_LIB, -270, 0x16, 0x43fa0586, VE_PointerXDaveMsg);
  InsertPatch (INT_LIB, -816, 0x24, 0x760061c4, VE_PointerXDaveMsg);
  InsertPatch (INT_LIB, -198, 27, 0x0148412c,	VE_NewModeMsg);
  InsertPatch (INT_LIB, -612, 27, 0x0148412c, VE_NewModeMsg);
  InsertPatch (INT_LIB, -72, 0x14, 0x226c0276, VE_PatchPointerMsg);
  InsertPatch (INT_LIB, -270, 0x6, 0x28032a02, VE_PatchPointerMsg);
  InsertPatch (INT_LIB, -276, 0x3c, 0x487a000a, VE_WBExtMsg);
  InsertPatch (INT_LIB, -588, 0x6e, 0x487a0008, VE_WBExtMsg);
  InsertPatch (INT_LIB, -588, 0x76, 0x487a0008, VE_WBExtMsg);
  InsertPatch (INT_LIB, -606, 0x16, 0x2c7aff6c, VE_WBExtMsg);
  InsertPatch (INT_LIB, -54, 0x22, 0x4e91204d, VE_ToolsDaemonMsg);
  InsertPatch (INT_LIB, -264, 0x2c, 0x4e924a40, VE_ToolsDaemonMsg);
  InsertPatch (INT_LIB, -702, 0x2c, 0x4e924a40, VE_ToolsDaemonMsg);
  InsertPatch (INT_LIB, -66, 0xe, 0x41ec0174, VE_TWAMsg);
  InsertPatch (INT_LIB, -72, 0x14, 0x41ec0174, VE_TWAMsg);
  InsertPatch (INT_LIB, -606, 0x26, 0x660802a8, VE_SmartWBMsg);
  InsertPatch (INT_LIB, -66, 0x18, 0x2c6c8002, VE_ScreenNotifyLibMsg);
  InsertPatch (INT_LIB, -78, 0xa, 0x206c8006, VE_ScreenNotifyLibMsg);
  InsertPatch (INT_LIB, -210, 0x16, 0x206c8006, VE_ScreenNotifyLibMsg);
  InsertPatch (INT_LIB, -552, 0x52, 0x206c8006, VE_ScreenNotifyLibMsg);
  InsertPatch (INT_LIB, -54, 0x14, 0x226c03be, VE_NewMenuMsg);
  InsertPatch (INT_LIB, -54, 12, 0x246b001c, VE_NewMenuMsg);
  InsertPatch (INT_LIB, -72, 0xe, 0x2c6c0390, VE_NewMenuMsg);
  InsertPatch (INT_LIB, -72, 14, 0x4aab001c, VE_NewMenuMsg);
  InsertPatch (INT_LIB, -264, 0xe, 0x45ef0034, VE_NewMenuMsg);
  InsertPatch (INT_LIB, -264, 0, 0x9efc00e8, VE_NewMenuMsg);
  InsertPatch (INT_LIB, -702, 0x10, 0x4aab001c, VE_NewMenuMsg);
  InsertPatch (INT_LIB, -804, 14, 0x2c6c0094, VE_NewMenuMsg);
  InsertPatch (INT_LIB, -204, 0xc, 0x08800006, VE_NoCareMsg);
  InsertPatch (INT_LIB, -270, -4, 0x50545258, VE_PointerXDaveMsg);
  InsertPatch (INT_LIB, -816, -4, 0x50545258, VE_PointerXDaveMsg);
  InsertPatch (INT_LIB, -66, 0x1a, 0x6100fe00, VE_SysiHackMsg);
  InsertPatch (INT_LIB, -72, 0x30, 0xbadc0ded, VE_SysiHackMsg);
  InsertPatch (INT_LIB, -468, 0x12, 0x4a6c006a, VE_SysiHackMsg);
  InsertPatch (INT_LIB, -606, 0x20, 0x206d001e, VE_SysiHackMsg);
  InsertPatch (INT_LIB, -588, 0x1e, 0x206c0140, VE_EasyReqPatchMsg);
  InsertPatch (INT_LIB, -606, 0x1a, 0x43ef0044, VE_EasyReqPatchMsg);
  InsertPatch (INT_LIB, -348, 0x1a, 0x02116610, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -588, 0x1a, 0x0211660a, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -348, 0x4, 0x80812c79, VE_TimedReqMsg);
  InsertPatch (INT_LIB, -588, 0x20, 0x7c01101a, VE_TimedReqMsg);
  InsertPatch (INT_LIB, -198, 0xc8, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -204, 0xaa, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -210, 0x6e, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -246, 0x46, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -252, 0x50, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -264, 0x96, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -276, 0x82, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -342, 0x5a, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -510, 0x78, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -546, 0xb4, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -588, 0x8c, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -606, 0xa0, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -612, 0xbe, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -786, 0x3c, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -90, 0xa, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -96, 0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -198, 0xc8+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -204, 0xaa+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -210, 0x6e+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -246, 0x46+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -252, 0x50+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -264, 0x96+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -276, 0x82+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -342, 0x5a+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -510, 0x78+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -546, 0xb4+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -588, 0x8c+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -606, 0xa0+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -612, 0xbe+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -786, 0x3c+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -270, 0x1a, 0x52ac0da0, VE_BusyPointerMsg);
  InsertPatch (INT_LIB, -816, 0x12, 0x52ac0dbc, VE_BusyPointerMsg);
  InsertPatch (INT_LIB, -210, 0x4, 0x2a4e49f9, VE_PatchOpenWBMsg);
  InsertPatch (INT_LIB, -510, 0x6, 0x2a4e49f9, VE_PatchOpenWBMsg);
  InsertPatch (INT_LIB, -606, 0x6, 0x2a4949f9, VE_PatchOpenWBMsg);
  InsertPatch (INT_LIB, -72, 0x4, 0x41fa0042, VE_WBGaugeMsg);
  InsertPatch (INT_LIB, -150, 0x4, 0x41fa0042, VE_WBGaugeMsg);
  InsertPatch (INT_LIB, -276, 0x4, 0x41fa0042, VE_WBGaugeMsg);
  InsertPatch (INT_LIB, -66, 0x32, 0x28544a94, VE_ArqMsg);
  InsertPatch (INT_LIB, -96, 0x4, 0x487afd88, VE_ArqMsg);
  InsertPatch (INT_LIB, -348, 0x1a, 0x02396610, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -588, 0x12, 0x0239660c, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -72, 0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -90, 0xa+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -96, 0x14+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -150, 0xa, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -198, 0xc8+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -204, 0xaa+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -210, 0x6e+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -246, 0x46+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -252, 0x50+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -264, 0x96+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -276, 0x82+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -342, 0x5a+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -510, 0x78+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -546, 0xb4+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -588, 0x8c+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -606, 0xa0+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -612, 0xbe+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -786, 0x3c+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (INT_LIB, -612, 0x14, 0x2ac82ac9, VE_NewIconsMsg);
  InsertPatch (INT_LIB, -468, 88, 0x00471000, VE_Prop3DMsg);
  InsertPatch (INT_LIB, -198, 0x18, 0x24534e92, VE_UrouHackMsg);
  InsertPatch (INT_LIB, -204, 0x16, 0x24544e92, VE_UrouHackMsg);
  InsertPatch (INT_LIB, -606, 0xa, 0x24544e92, VE_UrouHackMsg);
  InsertPatch (INT_LIB, -612, 0xa, 0x24534e92, VE_UrouHackMsg);
  InsertPatch (INT_LIB, -198, 0x10, 0x24404e92, VE_UrouHackMsg);
  InsertPatch (INT_LIB, -204, 0x1e, 0x24404e92, VE_UrouHackMsg);
  InsertPatch (INT_LIB, -606, 0x16, 0x24404e92, VE_UrouHackMsg);
  InsertPatch (INT_LIB, -612, 0x8, 0x24404e92, VE_UrouHackMsg);
  InsertPatch (INT_LIB, -348,  17, 0xa02b6de3, VE_AMOSLibMsg);
  InsertPatch (INT_LIB, -588,  18, 0x2b6de3a4, VE_AMOSLibMsg);
  InsertPatch (INT_LIB, -510,   0, 0xbffa005e, VE_SnoopDosMsg);	// 3.0
  InsertPatch (INT_LIB, -588, 8, MakeULONG ("lowl"), VE_LowLevelLibMsg);
  InsertPatch (INT_LIB,  -54, 15, 0xe64a4067, VE_MagicMenuMsg);
  InsertPatch (INT_LIB,  -66, 17, 0x40671a20, VE_MagicMenuMsg);
  InsertPatch (INT_LIB,  -72, 15, 0x342a484e, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -150, 27, 0x1c200720, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -180, 27, 0x1c200720, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -192, 27, 0x1c200720, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -198, 19, 0x1a204d2c, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -204, 19, 0x1a204d2c, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -246, 19, 0x1a204d2c, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -252, 19, 0x1a204d2c, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -264, 19, 0x40672220, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -276, 19, 0x40660470, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -306, 19, 0x40671a20, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -312, 17, 0x40660470, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -450, 27, 0x1c204d2c, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -456, 17, 0x40660470, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -558, 15, 0x0c4a2c12, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -606, 21, 0x1c204d22, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -612, 21, 0x1c204d22, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -702, 19, 0x40671e20, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -786, 27, 0x40671e20, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -804, 11, 0x492a4861, VE_MagicMenuMsg);
  InsertPatch (INT_LIB, -450,  8, 0x2c6c008a, VE_SnapMsg);
  InsertPatch (INT_LIB, -450, 10, 0x2c6c0096, VE_SnapMsg);
  InsertPatch (INT_LIB, -588, 400, MakeULONG ("Assi"), VE_AssignZMsg);
  InsertPatch (INT_LIB,  -60, 14, 0x246c07b4, VE_MagicCXMsg);
  InsertPatch (INT_LIB,  -66, 24, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (INT_LIB,  -72, 18, 0x226c016c, VE_MagicCXMsg);
  InsertPatch (INT_LIB,  -72, 26, 0x226c518c, VE_MagicCXMsg);
  InsertPatch (INT_LIB,  -78, 16, 0x206c5174, VE_MagicCXMsg);
  InsertPatch (INT_LIB,  -96, 14, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -150, 14, 0x206c0208, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -198, 28, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -210, 16, 0x206c5170, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -246, 50, 0x206c5168, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -252, 84, 0x206c5168, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -264, 78, 0x266d0012, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -270, 46, 0x206c004e, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -270, 26, 0x266c07b4, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -276, 20, 0x2c6c042c, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -336, 16, 0x206c516c, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -342, 16, 0x206c5168, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -348, 34, 0x206c519c, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -468, 24, 0x2a6c0132, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -552, 66, 0x2c6c0520, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -588, 30, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -606, 20, 0x20690076, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -612, 22, 0x2c6c0520, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -816, 20, 0x266c07b4, VE_MagicCXMsg);
  InsertPatch (INT_LIB, -438, 34, 0x2c6c02ec, VE_NewIconsMsg);
  InsertPatch (INT_LIB, -612, 10, 0x6100efe0, VE_NewIconsMsg);
  InsertPatch (INT_LIB, -348, 130, MakeULONG ("CyRe"), VE_CyReqMsg);
  InsertPatch (INT_LIB, -588, 338, MakeULONG ("CyRe"), VE_CyReqMsg);
  InsertPatch (INT_LIB, -552, 16, 0x226c0140, VE_NaeGrayPatchMsg);
  InsertPatch (INT_LIB, -204, 20, 0x6100ff74, VE_NCommPatchMsg);
  InsertPatch (INT_LIB, -342, 16, 0x6100fe8c, VE_NCommPatchMsg);
  InsertPatch (INT_LIB, -612, 36, 0x4fef0010, VE_PatchInterleavedMsg);
  InsertPatch (INT_LIB, -612, 36, 0x4fef0010, VE_PatchInterleavedMsg);
  InsertPatch (INT_LIB, -414 ,24, 0x2c6c0568, VE_PatchWBMsg);
  InsertPatch (INT_LIB, -420 ,26, 0x20680114, VE_PatchWBMsg);
  InsertPatch (INT_LIB, -78, 0, 0x610021da, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -96, 22, 0x4eaefebc, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -78, 10, 0x2f4e0018, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB, -210, 10, 0x2f4e0018, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB,  -18, 298, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB,  -72,  50, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB,  -78,  30, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB,  -90,  60, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB,  -96,  70, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -150,  40, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -198, 270, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -204, 240, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -210, 180, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -246, 140, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -252, 150, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -264, 220, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -276, 200, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -342, 160, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -510, 190, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -546, 250, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -588, 210, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -606, 230, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -612, 260, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -786, 130, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (INT_LIB, -348,  6, 0x61085379, VE_ReqAttackMsg);
  InsertPatch (INT_LIB, -360, 12, 0xfff02a4f, VE_ReqAttackMsg);
  InsertPatch (INT_LIB, -372,  8, 0x588f9179, VE_ReqAttackMsg);
  InsertPatch (INT_LIB, -588,  6, 0x61085379, VE_ReqAttackMsg);
  InsertPatch (INT_LIB, -594, 16
, 0xfff02a4f, VE_ReqAttackMsg);
  InsertPatch (INT_LIB, -600, 20, 0x000a5379, VE_ReqAttackMsg);
  InsertPatch (INT_LIB, -348, 280, MakeULONG ("CyRe"), VE_CyReqMsg);
  InsertPatch (INT_LIB, -588, 488, MakeULONG ("CyRe"), VE_CyReqMsg);
  InsertPatch (INT_LIB,  -54, 182, MakeULONG ("CEDI"), VE_CEDPatchMsg);
  InsertPatch (INT_LIB, -264, 212, MakeULONG ("CEDI"), VE_CEDPatchMsg);
  InsertPatch (INT_LIB, -612, 152, MakeULONG ("CEDI"), VE_CEDPatchMsg);
  InsertPatch (INT_LIB, -276, 574, 0x206a0010, VE_VMMMsg);
  InsertPatch (INT_LIB, -348, 38, 0x226c26d4, VE_ScalaMsg);
  InsertPatch (INT_LIB, -588, 24, 0x226c26d4, VE_ScalaMsg);
  InsertPatch (INT_LIB, -612, 36, 0x2c6c1658, VE_ScalaMsg);
  InsertPatch (INT_LIB, -786, 28, 0x246c2758, VE_ScalaMsg);
  InsertPatch (INT_LIB,  -42, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB,  -60, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB,  -66, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB,  -72, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB,  -78, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB,  -90, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB,  -96, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -204, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -210, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -264, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -270, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -276, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -348, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -444, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -468, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -510, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -588, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -606, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -612, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -636, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (INT_LIB, -66, 34, 0x61000412, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -72, 80, 0x610003ac, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -78, 14, 0x61000400, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -198, 20, 0x22680020, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -204, 58, 0x61000488, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -210, 34, 0x610004c6, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -306, 58, 0x61000230, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -312, 58, 0x6100026e, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -348, 28, 0x610001dc, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -372, 80, 0x61000358, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -450, 44, 0x61000328, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -462, 32, 0x610002da, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -588, 20, 0x61000218, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -606, 72, 0x6100043c, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -612, 34, 0x610004ec, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -66, 34, 0x61000460, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -72, 80, 0x610003ea, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -78, 30, 0x6100043e, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -204, 58, 0x610004c4, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -210, 34, 0x61000502, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -276, 48, 0x610001ca, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -306, 58, 0x6100026e, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -312, 58, 0x610002ac, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -348, 28, 0x6100021a, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -372, 80, 0x61000396, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -450, 44, 0x61000366, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -462, 32, 0x61000318, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -588, 20, 0x61000256, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -606, 54, 0x6100048a, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -612, 42, 0x61000528, VE_PowerWindowsMsg);
  InsertPatch (INT_LIB, -204, 14, 0x286d03a6, VE_WindowDaemonMsg);
  InsertPatch (INT_LIB, -606, 14, 0x286d03aa, VE_WindowDaemonMsg);
  InsertPatch (INT_LIB, -816, 12, 0x286d03ae, VE_WindowDaemonMsg);
  InsertPatch (INT_LIB,  -66, 36, 0x61000204, VE_ExecutiveMsg);
  InsertPatch (INT_LIB,  -72, 82, 0x61000204, VE_ExecutiveMsg);
  InsertPatch (INT_LIB, -204,  6, 0x670e2f0d, VE_ExecutiveMsg);
  InsertPatch (INT_LIB, -210, 64, 0x61000204, VE_ExecutiveMsg);
  InsertPatch (INT_LIB, -510, 50, 0x61000204, VE_ExecutiveMsg);
  InsertPatch (INT_LIB, -606,  6, 0x670c2f0d, VE_ExecutiveMsg);
  InsertPatch (INT_LIB,  -72, 2, 0x2468002e, VE_Birdie2000Msg);
  InsertPatch (INT_LIB, -276, 2, 0x06b90000, VE_Birdie2000Msg);
  InsertPatch (INT_LIB, -66, 28, 0x4a94671a, VE_QuickLensMsg);
  InsertPatch (INT_LIB,  -66,  74, MakeULONG ("fy_i"), VE_NotifyLibMsg);
	InsertPatch (INT_LIB,  -72, 108, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB,  -78, 150, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -198,  60, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -204,  94, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -210, 136, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -246,  20, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -252,  34, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -510,  40, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -552, 122, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -606,  80, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB, -612,  46, MakeULONG ("fy_i"), VE_NotifyLibMsg);
  InsertPatch (INT_LIB,  -72, 14, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB,  -72, 16, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB,  -78, 18, 0x2f4e0018, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB,  -78, 20, 0x2f4e0018, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB, -210, 18, 0x2f4e0018, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB, -210, 20, 0x2f4e0018, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB,  -90, 386, MakeULONG ("ootC"), VE_BootControlMsg);
  InsertPatch (INT_LIB, -588, 276, MakeULONG ("ootC"), VE_BootControlMsg);
  InsertPatch (INT_LIB, -822, 386, MakeULONG ("ootC"), VE_BootControlMsg);
  InsertPatch (INT_LIB,  -78,  0, 0x6100f2ec, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -612, 20, 0x4eaeffdc, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -606, 16, 0x2f4e0024, VE_Dopus5LibMsg);
  InsertPatch (INT_LIB,  -90, 16, 0x6100fc7c, VE_RtgLibMsg);
	InsertPatch (INT_LIB,  -96, 12, 0x70044eae, VE_RtgLibMsg);
  InsertPatch (INT_LIB, -198,  0, 0x9efc0010, VE_RtgLibMsg);
  InsertPatch (INT_LIB, -612,  0, 0x9efc0014, VE_RtgLibMsg);
  InsertPatch (INT_LIB, -822,  0, 0x9efc00b4, VE_RtgLibMsg);
  InsertPatch (INT_LIB, -90, 16, 0x6100fbbe, VE_RtgLibMsg);
  InsertPatch (INT_LIB, -822, 0, 0x9efc00bc, VE_RtgLibMsg);
  InsertPatch (INT_LIB,  -72,  6, 0x670e2f08, VE_PowerWBMsg);
  InsertPatch (INT_LIB, -204,  6, 0x672e2028, VE_PowerWBMsg);
  InsertPatch (INT_LIB, -276,  4, 0x000a2f39, VE_PowerWBMsg);
  InsertPatch (INT_LIB, -606, 14, 0x508f4cdf, VE_PowerWBMsg);
  InsertPatch (INT_LIB, -816,  2, 0x2408487a, VE_PowerWBMsg);
	InsertPatch (INT_LIB, -606, 120, 0x2068001e, VE_CleverWinMsg);
  InsertPatch (INT_LIB, -204, 38, 0x2268001a, VE_WinSpeedMsg);
  InsertPatch (INT_LIB, -606, 10, 0x2268001a, VE_WinSpeedMsg);
  InsertPatch (INT_LIB, -606, 0, 0x9efc0054, VE_DefIconsMsg);
  InsertPatch (INT_LIB, -66, 18, 0x2a6c023c, VE_ImagePoolLibMsg);
  InsertPatch (INT_LIB, -348, 27, 0x0acb4e61, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -588,  7, 0xfe2a4e2c, VE_ReqChangeMsg);
  InsertPatch (INT_LIB, -198, 14, 0x302b000e, VE_PPrefsMsg);
  InsertPatch (INT_LIB, -474, 26, 0x41ec8208, VE_PPrefsMsg);
  InsertPatch (INT_LIB, -612, 32, 0x3028000c, VE_PPrefsMsg);
  InsertPatch (INT_LIB,  -66, 17, 0x4070322a, VE_ModeProMsg);
  InsertPatch (INT_LIB,  -78, 21, 0x2e703261, VE_ModeProMsg);
  InsertPatch (INT_LIB, -198, 19, 0x0c2a4841, VE_ModeProMsg);
  InsertPatch (INT_LIB, -204, 21, 0x582a4841, VE_ModeProMsg);
  InsertPatch (INT_LIB, -510, 21, 0x542f4800, VE_ModeProMsg);
  InsertPatch (INT_LIB, -516, 25, 0x142a4841, VE_ModeProMsg);
  InsertPatch (INT_LIB, -606, 17, 0x202a4841, VE_ModeProMsg);
  InsertPatch (INT_LIB, -612, 31, 0x7c20de20, VE_ModeProMsg);
  InsertPatch (INT_LIB,  -42, 0, 0x4e550000, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -438, 0, 0x4e550000, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -66,  2, 0x3f3eb1f9, VE_MagicLayersMsg);
  InsertPatch (INT_LIB, -72,  2, 0x3f3e2268, VE_MagicLayersMsg);
  InsertPatch (INT_LIB, -78, 18, 0xfe022400, VE_MagicLayersMsg);
  InsertPatch (INT_LIB, -438, 6, 0xfff22854, VE_FakeDefIconMsg);
  InsertPatch (INT_LIB, -606, 8, 0xfffe49fa, VE_FakeDefIconMsg);
  InsertPatch (INT_LIB,  -66, 17, 0xe42c6c0d, VE_ScreenTabMsg);
  InsertPatch (INT_LIB,  -72, 17, 0xe42c6c0d, VE_ScreenTabMsg);
  InsertPatch (INT_LIB, -204, 15, 0x482f4800, VE_ScreenTabMsg);
  InsertPatch (INT_LIB, -246, 17, 0xe42c6c0d, VE_ScreenTabMsg);
  InsertPatch (INT_LIB, -252, 17, 0xe42c6c0d, VE_ScreenTabMsg);
  InsertPatch (INT_LIB, -450, 17, 0xe42c6c0d, VE_ScreenTabMsg);
  InsertPatch (INT_LIB, -606, 17, 0x482f4800, VE_ScreenTabMsg);
  InsertPatch (INT_LIB, -612, 17, 0x482f4800, VE_ScreenTabMsg);
  InsertPatch (INT_LIB,  -72, 3, 0xff28487e, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -150, 3, 0xff28484a, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -240, 1, 0x0967f648, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -264, 3, 0xff200966, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -270, 7, 0x10631820, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -378, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -384, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -390, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -492, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -570, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -582, 1, 0x3ad0da67, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -588, 1, 0x0967f648, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -678, 3, 0xff240926, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -786, 1, 0x0967f648, VE_PatchWorkMsg);
  InsertPatch (INT_LIB, -606, 1, 0x3affda67, VE_CentreQuestMsg);
  InsertPatch (INT_LIB, -588, 7, 0x3e2a4849, VE_AssignWedgeMsg);
  InsertPatch (INT_LIB, -348, 15, 0x00704e7a, VE_AslToRTMsg);
  InsertPatch (INT_LIB, -588, 17, 0x6e2f4900, VE_AslToRTMsg);
  InsertPatch (INT_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (INT_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (INT_LIB, -204,  5, 0x48264e4a, VE_SnapperMsg);
  InsertPatch (INT_LIB, -606, 11, 0xe42b49ff, VE_SnapperMsg);
  InsertPatch (INT_LIB, -348, 7, 0xf02a4f2c, VE_PatchControlMsg);
  InsertPatch (INT_LIB, -588, 7, 0xf02a4f24, VE_PatchControlMsg);
  InsertPatch (INT_LIB, -204, 16, 0xfdcc4cdf, VE_SafeSwitchMsg);
  InsertPatch (INT_LIB, -246, 17, 0xc04a8067, VE_SafeSwitchMsg);
  InsertPatch (INT_LIB, -252, 17, 0xc04a8067, VE_SafeSwitchMsg);
  InsertPatch (INT_LIB, -336, 17, 0xc04a8067, VE_SafeSwitchMsg);
  InsertPatch (INT_LIB, -342, 17, 0xc04a8067, VE_SafeSwitchMsg);
  InsertPatch (INT_LIB, -606, 16, 0xfdcc4cdf, VE_SafeSwitchMsg);
  InsertPatch (INT_LIB, -786, 17, 0xc04a8067, VE_SafeSwitchMsg);
  InsertPatch (INT_LIB, -588,  7, 0x146a0861, VE_AMOSLibMsg);
	InsertPatch (INT_LIB, -210, 11, 0xfc93c94e, VE_CopperDemonMsg);
  InsertPatch (INT_LIB,  -42,  3, 0x3e26492e, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB,  -66,  3, 0x3e2a4820, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB,  -72,  5, 0x3e9bcd97, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB,  -78, 17, 0x7c9bcd26, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -204,  3, 0x1c2a4820, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -210, 15, 0x1f93c92c, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -228,  3, 0x2e244920, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -414,  5, 0x0e93c949, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -438,  5, 0x3e2a482f, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -468, 13, 0x282a4849, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -510,  5, 0x3a93c949, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -606, 17, 0x6842af00, VE_VisualPrefsMsg);
  InsertPatch (INT_LIB, -612, 17, 0x442a4841, VE_VisualPrefsMsg);
	InsertPatch (INT_LIB, -714, 17, 0xcc2a484e, VE_TRAPMsg);
  InsertPatch (INT_LIB, -204,  9, 0x4842824a, VE_IPrefsMsg);
  InsertPatch (INT_LIB, -606, 15, 0x4699cc42, VE_IPrefsMsg);
  InsertPatch (INT_LIB,  -78, 17, 0xb84e902e, VE_FPPrefsMsg);
  InsertPatch (INT_LIB, -210, 17, 0x2693c92c, VE_FPPrefsMsg);
  InsertPatch (INT_LIB, -612, 21, 0x20604220, VE_FPPrefsMsg);
  InsertPatch (INT_LIB, -588, 7, 0xfa6196b0, VE_siegfriedLibMsg);

  InsertPatch (DOS_LIB, -744, 2, 0x2c7aff26, VE_LocaleLibMsg);
  InsertPatch (DOS_LIB, -750, 2, 0x2c7aff18, VE_LocaleLibMsg);
  InsertPatch (DOS_LIB, -978, 2, 0x207afff8, VE_LocaleLibMsg);
  InsertPatch (DOS_LIB, -30, 0, 0x207afffa, VE_ROMUpdatesMsg);
  InsertPatch (DOS_LIB, -360, 2, 0xd1c8d1c8, VE_ROMUpdatesMsg);
  InsertPatch (DOS_LIB, -432, 0xe, 0x49fa007e, VE_ROMUpdatesMsg);
  InsertPatch (DOS_LIB, -432, 0x8, 0x4feffef8, VE_ROMUpdatesMsg);
  InsertPatch (DOS_LIB, -150, 0x24, MakeULONG ("expl"), VE_ExplodeLibMsg);
  InsertPatch (DOS_LIB, -768, 0x2e, MakeULONG ("expl"), VE_ExplodeLibMsg);
  InsertPatch (DOS_LIB, -150, -8, 0x4c464400, VE_LVDMsg);
  InsertPatch (DOS_LIB, -756, -0x22, 0x4c464400, VE_LVDMsg);
  InsertPatch (DOS_LIB, -768, -0x16, 0x4c464400, VE_LVDMsg);
  InsertPatch (DOS_LIB, -150, -8,0x50464c2e, VE_PatchLoadSegMsg);
  InsertPatch (DOS_LIB, -30, 10, 0x262f0004, VE_PPPatcherMsg);
  InsertPatch (DOS_LIB, -36, 4, 0x2f012417, VE_PPPatcherMsg);
  InsertPatch (DOS_LIB, -48, 10, 0x246f0004, VE_PPPatcherMsg);
  InsertPatch (DOS_LIB, -102, 10, 0x246f0004, VE_PPPatcherMsg);
  InsertPatch (DOS_LIB, -30, 0x24+0x3e, 0x45edfff0, VE_PPDRPMsg);
  InsertPatch (DOS_LIB, -36, 0x12, 0x4a6a0018, VE_PPDRPMsg);
  InsertPatch (DOS_LIB, -48, 0x6, 0x6100fb96, VE_PPDRPMsg);
  InsertPatch (DOS_LIB, -102, 0x0, 0x4e55fee0, VE_PPDRPMsg);
  InsertPatch (DOS_LIB, -822, 0x52, 0x4e924cdf, VE_DosPatchMsg);
  InsertPatch (DOS_LIB, -828, 0x1a, 0x4e926008, VE_DosPatchMsg);
  InsertPatch (DOS_LIB, -834, 0x30, 0x4e90241f, VE_DosPatchMsg);
  InsertPatch (DOS_LIB, -150, -0x1e, 0x706f7765, VE_PPLoadSegMsg);
  InsertPatch (DOS_LIB, -768, -0x14, 0x706f7765, VE_PPLoadSegMsg);
  InsertPatch (DOS_LIB, -30, 0x2e, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -36, -0x5e, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -42, 0x32, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -66, -0xb4, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -102, 0xb0, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -108, 0x9a, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -150, 0x3c, 0x303cff6a, VE_VirusInterceptorMsg);
  InsertPatch (DOS_LIB, -756, 0x18, 0x303cff6a, VE_VirusInterceptorMsg);
  InsertPatch (DOS_LIB, -768, 0x2a, 0x303cff6a, VE_VirusInterceptorMsg);
  InsertPatch (DOS_LIB, -150, 0x22, MakeULONG ("expl"), VE_ExplodeLibMsg);
  InsertPatch (DOS_LIB, -768, 0x2c, MakeULONG ("expl"), VE_ExplodeLibMsg);
  InsertPatch (DOS_LIB, -150, 0x18, 0x303cff6a, VE_ReplexMsg);
  InsertPatch (DOS_LIB, -822, 10, 0x43edff00, VE_KCommMsg);
  InsertPatch (DOS_LIB, -150, 0x22, 0x43724d2e, VE_CrMLibMsg);
  InsertPatch (DOS_LIB, -768, 0x2c, 0x43724d2e, VE_CrMLibMsg);
  InsertPatch (DOS_LIB, -30, 0x76, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -36, -0x76, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -42, 0x2, 0x52544444, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -66, -0xd0, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -102, 0x100, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -108, 0xea, 0x43724d21, VE_CrmDataDecrMsg);
  InsertPatch (DOS_LIB, -30, 0x14, 0x52ac9690, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -36, 0xe, 0x52ac9690, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -48, 0x14, 0x52ac9690, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -102, 0x14, 0x52ac9690, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -378, 0xe, 0x52ac9690, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -150, 0x0, 0x207afffa, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -156, 0xa, 0x207afff0, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -768, 0x0, 0x207afffa, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -30, 0x0, 0x4a3afd95, VE_AddPowerMsg);
  InsertPatch (DOS_LIB, -84, 0x0, 0x4a3afdcb, VE_AddPowerMsg);
  InsertPatch (DOS_LIB, -120, 0x12, 0x4a3aff43, VE_AddPowerMsg);
  InsertPatch (DOS_LIB, -612, 0x0, 0x4a3aff0f, VE_AddPowerMsg);
  InsertPatch (DOS_LIB, -84, 0x2e, 0x7041d080, VE_ForceIconMsg);
  InsertPatch (DOS_LIB, -30, -0x60, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -72, -0x36, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -78, -0x66, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -84, -0x48, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -120, -0x30, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -150, -0x42, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -180, -0x6c, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -186, -0x7e, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -396, -0x72, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -444, -0x4e, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -618, -0x24, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -624, -0x2a, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -642, -0x3c, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -768, -0x5a, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -822, -0x54, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -996, -0x78, 0x444f5350, VE_DosPrefsMsg);
  InsertPatch (DOS_LIB, -150, 0x1e, 0x43fa021d, VE_xLoadSegMsg);
  InsertPatch (DOS_LIB, -768, 0x28, 0x43fa021d, VE_xLoadSegMsg);
  InsertPatch (DOS_LIB, -138, 17,   0x0c200361, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -150, 0x16, 0x2c6c0154, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -156, 0x16, 0x2c6c02a0, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -186, 0x1e, 0x2c6c0154, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -498, 0x1c, 0x2c6c015c, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -504, 0x20, 0x206c02a0, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -756, 9,    0x082a4949, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -762, 0x18, 0x2c6c02a0, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -768, 0x1c, 0x2c6c0154, VE_MultiUserLibMsg);
  InsertPatch (DOS_LIB, -84, 0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -714, 0x1e, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -84, 0x14+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -714, 0x1e+0x14, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -30, 0x12, 0x52ac19f8, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -36, 0xe, 0x52ac19f8, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -48, 0xc, 0x52ac19f8, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -102, 0x12, 0x52ac19f8, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -378, 0xe, 0x52ac19f8, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -30, 0x12, 0x52ac170c, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -36, 0xe, 0x52ac170c, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -48, 0xc, 0x52ac170c, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -102, 0x12, 0x52ac170c, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -378, 0xe, 0x52ac170c, VE_PowerDataMsg);
  InsertPatch (DOS_LIB, -84, 0x14+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -714, 0x1e+0x28, 0x4d756c74, VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -150, -4, MakeULONG ("XFDP"), VE_xfdPatchMsg);
  InsertPatch (DOS_LIB, -768, -4, MakeULONG ("XFDP"), VE_xfdPatchMsg);
  InsertPatch (DOS_LIB, -30, 114, 0x303cffe2, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -72,  24, 0x303cffe2, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -84,  96, 0x303cffe2, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -126, 42, 0x303cffe2, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -150, 78, 0x303cffe2, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -222, 60, 0x303cffe2, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB,  -30, 0, 0xbffa005e, VE_SnoopDosMsg);	// 3.0
  InsertPatch (DOS_LIB,  -72, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB,  -78, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB,  -84, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -120, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -126, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -150, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -222, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -444, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -504, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -606, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -768, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -900, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -906, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -912, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB, -918, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (DOS_LIB,  -30, 23, 0x52672620, VE_SlowResetMsg);
  InsertPatch (DOS_LIB,  -36, 27, 0xf24e903c, VE_SlowResetMsg);
  InsertPatch (DOS_LIB,  -42, 21, 0x52672620, VE_SlowResetMsg);
  InsertPatch (DOS_LIB,  -48, 15, 0x017a0152, VE_SlowResetMsg);
  InsertPatch (DOS_LIB,  -72, 19, 0x52672620, VE_SlowResetMsg);
  InsertPatch (DOS_LIB,  -78, 21, 0x52673220, VE_SlowResetMsg);
  InsertPatch (DOS_LIB,  -84, 21, 0x52672620, VE_SlowResetMsg);
  InsertPatch (DOS_LIB,  -90, 33, 0x52670620, VE_SlowResetMsg);
  InsertPatch (DOS_LIB, -378, 23, 0x52672620, VE_SlowResetMsg);
  InsertPatch (DOS_LIB, -450, 27, 0xf64e9038, VE_SlowResetMsg);
  InsertPatch (DOS_LIB,  -72, 12, 0x2f41001c, VE_NoDeleteMsg);
  InsertPatch (DOS_LIB,  -30, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB,  -36, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB,  -42, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB,  -48, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB,  -72, 10, 0x4e52ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB,  -78, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB,  -84, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB,  -90, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -102, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -114, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -120, 10, 0x4e52ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -126, 10, 0x4e52ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -150, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -174, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -180, 10, 0x4e52ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -210, 10, 0x4e52ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -222, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -324, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -330, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -390, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -432, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -444, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -606, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -642, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -732, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -756, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -768, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -888, 10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB, -996 ,10, 0x4e53ff20, VE_DOSTraceMsg);
  InsertPatch (DOS_LIB,  -30, 20, 0x206c00ac, VE_MagicCXMsg);
  InsertPatch (DOS_LIB,  -72, 16, 0x206c009c, VE_MagicCXMsg);
  InsertPatch (DOS_LIB,  -78, 20, 0x206c0042, VE_MagicCXMsg);
  InsertPatch (DOS_LIB,  -84, 20, 0x206c00a4, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -120, 16, 0x206c0098, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -150, 16, 0x206c00a0, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -180, 20, 0x206c00b0, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -186, 20, 0x206c00b8, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -396, 20, 0x206c00b4, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -618, 20, 0x206c00bc, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -624, 20, 0x206c00c0, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -768, 20, 0x206c00a8, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -822, 20, 0x206c00c4, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -822, 26, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -840, 22, 0x206c0042, VE_MagicCXMsg);
  InsertPatch (DOS_LIB, -102, 20, 0x2c6c02ec, VE_NewIconsMsg);
  InsertPatch (DOS_LIB, -108, 18, 0x2c6c02ec, VE_NewIconsMsg);
  InsertPatch (DOS_LIB, -240, 26, 0x206c0354, VE_NewIconsMsg);
  InsertPatch (DOS_LIB, -108, 436, MakeULONG ("Hidd"), VE_PatchHiddenMsg);
  InsertPatch (DOS_LIB, -432, 384, MakeULONG ("Hidd"), VE_PatchHiddenMsg);
  InsertPatch (DOS_LIB, -822, 136, MakeULONG ("Hidd"), VE_PatchHiddenMsg);
  InsertPatch (DOS_LIB, -828,  68, MakeULONG ("Hidd"), VE_PatchHiddenMsg);
  InsertPatch (DOS_LIB,  -30, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -36, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -42, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -48, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -72, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -84,40, 0x33fc0033, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -90, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -96, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -102, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -108, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -150, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -156, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -414, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -552, 6, 0x4e900c79, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -18, 298, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (DOS_LIB,  -84,  90, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -150,  20, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -714, 100, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (DOS_LIB, -768,  10, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (DOS_LIB,  -30, 466, MakeULONG ("kLED"), VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -42, 434, MakeULONG ("kLED"), VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -48, 402, MakeULONG ("kLED"), VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -72, 370, MakeULONG ("kLED"), VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -84,  36, 0x33fc0033, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB,  -96, 498, MakeULONG ("kLED"), VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -102,  18, 0x33fc0033, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -108,  68, 0x33fc0033, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -150,  50, 0x33fc0033, VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -552, 338, MakeULONG ("kLED"), VE_DiskLEDMsg);
  InsertPatch (DOS_LIB, -150, 244, 0x6100ff16, VE_VMMMsg);
  InsertPatch (DOS_LIB, -768, 244, 0x6100ff16, VE_VMMMsg);
  InsertPatch (DOS_LIB,  -30, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (DOS_LIB,  -36, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (DOS_LIB,  -84, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (DOS_LIB, -102, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (DOS_LIB, -150, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (DOS_LIB, -258, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (DOS_LIB, -378, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (DOS_LIB, -138, 2, 0xb8ba0204, VE_StackAttackMsg);
  InsertPatch (DOS_LIB, -498, 4, 0x2f022b41, VE_StackAttackMsg);
  InsertPatch (DOS_LIB, -504, 2, 0xb4ba002a, VE_StackAttackMsg);
  InsertPatch (DOS_LIB, -606, 4, 0x2f022b41, VE_StackAttackMsg);
  InsertPatch (DOS_LIB, -720, 20, 0x2f4e0020, VE_Dopus5LibMsg);
  InsertPatch (DOS_LIB, -720, 22, 0x2f4e0020, VE_Dopus5LibMsg);
  InsertPatch (DOS_LIB, -678, 0, 0xbffa005e, VE_SnoopDosMsg);	// 3.7
  InsertPatch (DOS_LIB,  -30, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (DOS_LIB,  -36, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (DOS_LIB,  -84, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (DOS_LIB,  -90, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (DOS_LIB, -378, 3, MakeULONG ("« Sy"), VE_SysTrackerMsg);
  InsertPatch (DOS_LIB, -150,  7, 0x4e28412f, VE_PpcLibMsg);
  InsertPatch (DOS_LIB, -150,  9, 0x4e2a4126, VE_PpcLibMsg);
  InsertPatch (DOS_LIB, -156, 21, 0x82676e20, VE_PpcLibMsg);
  InsertPatch (DOS_LIB, -156, 11, 0x87677420, VE_PpcLibMsg);
  InsertPatch (DOS_LIB, -768,  7, 0x4e28412a, VE_PpcLibMsg);
  InsertPatch (DOS_LIB, -768,  9, 0x4e28422a, VE_PpcLibMsg);
  InsertPatch (DOS_LIB,  -78,  0, 0x9efc0108, VE_TrueMultiAssignsMsg);
  InsertPatch (DOS_LIB, -822,  0, 0x9efc0018, VE_TrueMultiAssignsMsg);
  InsertPatch (DOS_LIB, -828, 17, 0xa04a8067, VE_TrueMultiAssignsMsg);
  InsertPatch (DOS_LIB, -834, 17, 0x7026404a, VE_TrueMultiAssignsMsg);
  InsertPatch (DOS_LIB,  -30,  6, 0xff8c2854, VE_FakeDefIconMsg);
  InsertPatch (DOS_LIB, -102,  6, 0xffc02854, VE_FakeDefIconMsg);
  InsertPatch (DOS_LIB, -108,  8, 0xfffe49fa, VE_FakeDefIconMsg);
  InsertPatch (DOS_LIB, -30,  6, 0x2e0249f9, VE_DiskSafeMsg);
  InsertPatch (DOS_LIB, -36, 17, 0x942e014e, VE_DiskSafeMsg);
  InsertPatch (DOS_LIB, -42,  7, 0x032a017c, VE_DiskSafeMsg);
  InsertPatch (DOS_LIB, -48, 13, 0x032a017c, VE_DiskSafeMsg);
  InsertPatch (DOS_LIB, -102, 1, 0x02c07c00, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -108, 1, 0x02c07c00, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -114, 1, 0x02c07c00, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -138, 3, 0xffff806d, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -240, 3, 0xff93c92c, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -366, 3, 0xff2002c0, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -390, 1, 0x02c07c00, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -432, 3, 0x00662408, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -504, 1, 0x8367f648, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -666, 3, 0xff204f43, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -822, 1, 0x02c07c00, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -828, 1, 0x01c07c00, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -834, 1, 0x01c07c00, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -906, 3, 0x0b67f408, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -990, 3, 0x00662408, VE_PatchWorkMsg);
  InsertPatch (DOS_LIB, -138, 13, 0x754a8067, VE_BDebugMsg);
  InsertPatch (DOS_LIB, -498, 13, 0x754a8067, VE_BDebugMsg);
  InsertPatch (DOS_LIB, -150, 33, 0x3c221f20, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -156,  5, 0x3c221f20, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -768, 11, 0x3c221f20, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (DOS_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (DOS_LIB, -828, 5, 0x41204d49, VE_TrueMultiAssignsMsg);
  InsertPatch (DOS_LIB, -834, 5, 0x41204d49, VE_TrueMultiAssignsMsg);
  InsertPatch (DOS_LIB, -150,  7, 0x3e284e2f, VE_PpcLibMsg);
  InsertPatch (DOS_LIB, -156,  5, 0x0e240128, VE_PpcLibMsg);
  InsertPatch (DOS_LIB, -768,  9, 0x4e2e012f, VE_PpcLibMsg);
  InsertPatch (DOS_LIB, -138, 3, 0xfa6c0428, VE_MinStackMsg);
  InsertPatch (DOS_LIB, -498, 1, 0x81660470, VE_MinStackMsg);
  InsertPatch (DOS_LIB, -504, 3, 0x7e6c0424, VE_MinStackMsg);
  InsertPatch (DOS_LIB,  -30, 1, 0x862c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB,  -72, 1, 0xdc2c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB,  -78, 1, 0x2a2c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB,  -84, 3, 0x742c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -120, 1, 0x3c2c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -150, 1, 0xbc2c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -180, 1, 0x742c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -186, 1, 0x602c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -396, 1, 0x4e2c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -438, 1, 0xcc2c0361, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -444, 3, 0x962c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -618, 1, 0x102c0261, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -624, 1, 0xec2c0261, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -768, 1, 0x962c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -822, 5, 0x862e882c, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB, -996, 3, 0xac2c0161, VE_UnixDirs3Msg);
  InsertPatch (DOS_LIB,  -30, 10, 0x2c6c0014, VE_XPKPatchMsg);
  InsertPatch (DOS_LIB,  -36, 10, 0x2c6c0014, VE_XPKPatchMsg);
  InsertPatch (DOS_LIB,  -48, 10, 0x2c6c0014, VE_XPKPatchMsg);
  InsertPatch (DOS_LIB, -102, 10, 0x2c6c0014, VE_XPKPatchMsg);
  InsertPatch (DOS_LIB, -378, 10, 0x2c6c0014, VE_XPKPatchMsg);
  InsertPatch (DOS_LIB, -30, 3, 0x1e93c949, VE_AutoUpdateWBMsg);
  InsertPatch (DOS_LIB, -36, 5, 0x1e93c949, VE_AutoUpdateWBMsg);
  InsertPatch (DOS_LIB, -72, 5, 0x3e93c949, VE_AutoUpdateWBMsg);
  InsertPatch (DOS_LIB, -78, 7, 0x3e93c949, VE_AutoUpdateWBMsg);
  InsertPatch (DOS_LIB, -90, 3, 0x3e93c949, VE_AutoUpdateWBMsg);
  InsertPatch (DOS_LIB, -150, 17, 0x022f0b47, VE_IncubatorMsg);
  InsertPatch (DOS_LIB, -768, 33, 0x022f0b47, VE_IncubatorMsg);
  InsertPatch (DOS_LIB, -150, 9, 0x5f2f0061, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -156, 5, 0x1e221f20, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -768, 5, 0xe82f0161, VE_SegTrackerMsg);
  InsertPatch (DOS_LIB, -264, 6, 0x0044eb9, VE_AbortPktMsg);

  InsertPatch (DOS_LIB,   -30, 13, 0x8c67402f, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,   -72,  8, 0xff7e2079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,   -78,  3, 0x8c67402f, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,   -84,  8, 0xff602f39, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -120,  8, 0xff882079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -150,  8, 0xff6a2079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -180,  8, 0xfeb02079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -186, 15, 0xd867064e, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -396,  8, 0xfe9c2079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -558,  8, 0xfea62079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -618,  8, 0xff6c2079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -624,  8, 0xff622079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -642,  8, 0xff742079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -768,  8, 0xff142079, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -822,  3, 0xda672a4f, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -882,  7, 0x24200267, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -888,  6, 0x67064ef9, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -894,  6, 0x67064ef9, VE_DosWedgeMsg);
  InsertPatch (DOS_LIB,  -996,  8, 0xfe922079, VE_DosWedgeMsg);


	InsertPatch (GAD_LIB, -60, 0x6, 0x4a3af8ca, VE_AddPowerMsg);
	InsertPatch (GAD_LIB, -66, 0x6, 0x4a3af8e4, VE_AddPowerMsg);
	InsertPatch (GAD_LIB, -30, 0x10, 0x4e934cdf, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -36, 0x18, 0x02400107, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -42, 0xe, 0x02400107, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -72, 0x40, 0x02410107, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -78, 0x10, 0xb0a9000e, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -102, 0x36, 0x02410107, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -108, 0x10, 0xb2a9000e, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -48, 14, 0x2f4e0034, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -48, 0, 0x9efc0030, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -54, 18, 0x2c6c0390, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -54, 8, 0x2f4e0008, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -60, 0, 0x9efc0010, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -66, 0, 0x9efc0010, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -30, 0x64, MakeULONG ("Mult"), VE_MultiCXMsg);
	InsertPatch (GAD_LIB, -30, 0x64+0x14, MakeULONG ("Mult"), VE_MultiCXMsg);
	InsertPatch (GAD_LIB, -30, 0x64+0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
	InsertPatch (GAD_LIB, -30, 0x42, 0x0c402452, VE_Prop3DMsg);
	InsertPatch (GAD_LIB, -36, 0x2e, 0xc06b0010, VE_Prop3DMsg);
	InsertPatch (GAD_LIB, -126, 0x26, 0x4eb01800, VE_Prop3DMsg);
	InsertPatch (GAD_LIB, -132, 0x2a, 0xbbeb0012, VE_Prop3DMsg);
  InsertPatch (GAD_LIB,  -30 ,24, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (GAD_LIB,  -36 ,14, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (GAD_LIB,  -42 ,26, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (GAD_LIB, -126 ,94, 0x206d0058, VE_MagicCXMsg);
  InsertPatch (GAD_LIB, -132 ,18, 0x226c0142, VE_MagicCXMsg);
  InsertPatch (GAD_LIB, -174 ,26, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (GAD_LIB,  -18, 298, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (GAD_LIB,  -30, 170, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (GAD_LIB,  -30, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (GAD_LIB,  -36, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (GAD_LIB,  -30, 102, 0x20680008, VE_MagicFramesMsg);
  InsertPatch (GAD_LIB,  -66, 17, 0x14264943, VE_ModeProMsg);
  InsertPatch (GAD_LIB,  -30, 7, 0x00000e64, VE_PatchWorkMsg);
  InsertPatch (GAD_LIB,  -42, 1, 0x0a67f648, VE_PatchWorkMsg);
  InsertPatch (GAD_LIB,  -84, 1, 0x0967f648, VE_PatchWorkMsg);
  InsertPatch (GAD_LIB, -114, 1, 0x9067f648, VE_PatchWorkMsg);
  InsertPatch (GAD_LIB, -174, 1, 0x0a67f648, VE_PatchWorkMsg);
  InsertPatch (GAD_LIB,  -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (GAD_LIB,  -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (GAD_LIB,  -30,  3, 0x1e2e0070, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB,  -36, 15, 0x102a4860, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB,  -42, 13, 0x2c2a4820, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB, -102,  7, 0x3e2a4943, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB, -126, 19, 0x6890882c, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB, -132, 13, 0x042a484e, VE_VisualPrefsMsg);

	InsertPatch (LAY_LIB, -0x001E, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0024, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x002A, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0030, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0036, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x003C, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0042, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0048, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x004E, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0054, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x005A, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x006C, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0072, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x007E, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0084, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0090, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x0096, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x009C, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x00A2, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x00A8, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x00AE, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x00B4, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x00BA, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x00C0, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x00C6, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -0x00CC, -20, MakeULONG ("CYBL"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB,  -48, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -168, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -180, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -186, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
	InsertPatch (LAY_LIB, -216, 16, 0x2f290004, VE_SystemPatchMsg);
	InsertPatch (LAY_LIB, -90, 4, 0x2f2a0044, VE_ROMUpdatesMsg);
	InsertPatch (LAY_LIB, -36, 10, 0x4a2c44ab, VE_KCommMsg);
	InsertPatch (LAY_LIB, -90, 0x10, 0x02400040, VE_KCommMsg);
	InsertPatch (LAY_LIB, -180, 0xe, 0x4a2c44ab, VE_KCommMsg);
	InsertPatch (LAY_LIB, -186, 10, 0x4a2c44ab, VE_KCommMsg);
	InsertPatch (LAY_LIB, -36, 10, 0x4a2c449f, VE_KCommMsg);
	InsertPatch (LAY_LIB, -180, 0xe, 0x4a2c449f, VE_KCommMsg);
	InsertPatch (LAY_LIB, -186, 10, 0x4a2c449f, VE_KCommMsg);
	InsertPatch (LAY_LIB, -36, 8, 0x4a2c4334, VE_KCommMsg);
	InsertPatch (LAY_LIB, -90, 0xe, 0x02400040, VE_KCommMsg);
	InsertPatch (LAY_LIB, -180, 0xc, 0x4a2c4334, VE_KCommMsg);
	InsertPatch (LAY_LIB, -186, 8, 0x4a2c4334, VE_KCommMsg);
	InsertPatch (LAY_LIB, -36, 322, MakeULONG ("QLay"), VE_QLayersMsg);
	InsertPatch (LAY_LIB, -60, 424, MakeULONG ("QLay"), VE_QLayersMsg);
	InsertPatch (LAY_LIB, -66, 406, MakeULONG ("QLay"), VE_QLayersMsg);
	InsertPatch (LAY_LIB, -180, 384, MakeULONG ("QLay"), VE_QLayersMsg);
	InsertPatch (LAY_LIB, -186, 312, MakeULONG ("QLay"), VE_QLayersMsg);
	InsertPatch (LAY_LIB,  -36 ,182, 0x206b4200, VE_ExplodingLayersMsg);
	InsertPatch (LAY_LIB,  -90 ,140, 0x206b4200, VE_ExplodingLayersMsg);
	InsertPatch (LAY_LIB, -180 ,152, 0x206b4200, VE_ExplodingLayersMsg);
	InsertPatch (LAY_LIB, -186 ,116, 0x206b4200, VE_ExplodingLayersMsg);
  InsertPatch (LAY_LIB,  -36 ,32, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (LAY_LIB,  -90 ,16, 0x206c51bc, VE_MagicCXMsg);
  InsertPatch (LAY_LIB, -180 ,22, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (LAY_LIB, -186 ,36, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (LAY_LIB,  -36, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (LAY_LIB,  -60, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (LAY_LIB,  -66, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (LAY_LIB, -180, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (LAY_LIB, -186, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (LAY_LIB, -36, 14, 0x610006f8, VE_PowerWindowsMsg);
  InsertPatch (LAY_LIB, -48, 14, 0x610007e8, VE_PowerWindowsMsg);
  InsertPatch (LAY_LIB, -168, 12, 0x610007a8, VE_PowerWindowsMsg);
  InsertPatch (LAY_LIB, -186, 192, 0x2268000c, VE_PowerWindowsMsg);
  InsertPatch (LAY_LIB, -36, 14, 0x61000754, VE_PowerWindowsMsg);
  InsertPatch (LAY_LIB, -48, 14, 0x6100084c, VE_PowerWindowsMsg);
  InsertPatch (LAY_LIB, -168, 12, 0x6100080c, VE_PowerWindowsMsg);
  InsertPatch (LAY_LIB, -186, 208, 0x2268000c, VE_PowerWindowsMsg);
  InsertPatch (LAY_LIB, -90, 4, 0x204970ff, VE_Birdie2000Msg);
  InsertPatch (LAY_LIB,   -6, 18, 0x254e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -12, 14, 0x254e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -18, 14, 0x254e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -24, 14, 0x254e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -30, 14, 0x206dfffc, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -36, 28, 0x6100ff0c, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -42, 22, 0x61002442, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -48,  6, 0x246b0044, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -54,  6, 0x246b0044, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -60, 10, 0x6100fea8, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -66, 14, 0x6100febc, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -72, 14, 0x2c6e0022, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -78, 10, 0x2c6e0022, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -84, 14, 0x206b009c, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -90,  8, 0x206b0044, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB,  -96,  6, 0x2c6e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -102,  6, 0x2c6e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -108, 22, 0x2c6e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -114, 24, 0x2c6e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -120,  6, 0x2c6e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -126, 14, 0x6100fee0, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -132, 30, 0x20680004, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -138,  6, 0x2c6e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -144, 10, 0x2c6e0026, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -150,  8, 0x610000d6, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -156,  6, 0x610000f6, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -162,  6, 0x61000138, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -168, 14, 0x246b0044, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -174, 18, 0x2c6e0022, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -180,  0, 0x9efc0010, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -186,  0, 0x9efc000c, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -192, 14, 0x61002328, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -198, 24, 0x2c6e0022, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -204, 28, 0x246b005e, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -210, 386, 0x61000f00, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -216, 54, 0x61000330, VE_FastLayersLibMsg);
  InsertPatch (LAY_LIB, -36, 26, 0x6100fe0a, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -42, 42, 0x9efc0048, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -48, 0, 0x9efc000c, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -54, 0, 0x9efc000c, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -60, 10, 0x6100f7aa, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -66, 14, 0x6100f792, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -72, 24, 0x6100f7aa, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -90, 0, 0x9efc000c, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -108, 74, 0x266b0004, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -114, 6, 0x2f4e0018, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -126, 0, 0x9efc0094, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -168, 0, 0x9efc000c, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -180, 0, 0x9efc0020, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -186, 0, 0x9efc001c, VE_RtgLibMsg);
  InsertPatch (LAY_LIB, -192, 0, 0x9efc001c, VE_RtgLibMsg);
  InsertPatch (LAY_LIB,  -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (LAY_LIB,  -18, 17, 0xd8671826, VE_SaferPatches1Msg);

	InsertPatch (ICON_LIB, -42, 0x72, 0x7041d080, VE_ForceIconMsg);
	InsertPatch (ICON_LIB, -42, 0x34, 0xb2280008, VE_ForceIconMsg);
	InsertPatch (ICON_LIB, -48, 0x32, 0xb2280008, VE_ForceIconMsg);
	InsertPatch (ICON_LIB, -42, 0xa, 0x2f4a0020, VE_NewIconsMsg);
	InsertPatch (ICON_LIB, -48, 0xa, 0x48ef4100, VE_NewIconsMsg);
	InsertPatch (ICON_LIB, -54, 0x8, 0x2f4e0014, VE_NewIconsMsg);
  InsertPatch (ICON_LIB, -42, 20, 0x226c04d8, VE_MagicCXMsg);
  InsertPatch (ICON_LIB,  -42, 10, 0x266cfdd8, VE_NewIconsMsg);
  InsertPatch (ICON_LIB,  -48, 10, 0x6100eb7c, VE_NewIconsMsg);
  InsertPatch (ICON_LIB,  -54, 10, 0x6100ee96, VE_NewIconsMsg);
  InsertPatch (ICON_LIB, -120, 50, 0x2c6c02ec, VE_NewIconsMsg);
  InsertPatch (ICON_LIB, -132, 52, 0x2c6c02f0, VE_NewIconsMsg);
  InsertPatch (ICON_LIB,  -96, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (ICON_LIB, -102, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (ICON_LIB,   -6, 40, 0x2c690026, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -12, 66, 0x2c6d0026, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -18, 26, 0x2c690026, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -24, 40, 0x2c6d0026, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -30, 74, 0x2f4e0018, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -36, 54, 0x2f4e0018, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -42, 94, 0x2f4e0018, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -48, 62, 0x226b005e, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -54, 28, 0x226a000a, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -60, 34, 0x2f4e0018, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -66, 32, 0x2f4e0018, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -72, 40, 0x6100faf2, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -78, 44, 0x226b005e, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -84, 46, 0x6100e876, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -90, 18, 0x226b005e, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -96, 10, 0x2f4e0018, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -102, 10, 0x2f4e0028, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -108, 32, 0x6100ff2e, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -114, 26, 0x2c6d0026, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -120, 192, 0x2f480044, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -126, 144, 0x2c6d0032, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -132, 44, 0x226b005e, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -138, 24, 0x6100e876, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -144, 34, 0x246a0002, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -150, 28, 0x2c6d002e, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -156, 58, 0x226d005e, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -162, 104, 0x2c6d0032, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -168, 48, 0x2c6d0026, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -174, 64, 0x6100f73a, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -180, 156, 0x266a0004, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -186, 76, 0x2f490040, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -192, 38, 0x2c6d002e, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -198, 154, 0x26680030, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -84, 46, 0x6100e87a, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -138, 24, 0x6100e87a, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -156, 8, 0x2f490050, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -174, 50, 0x2f48000c, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -198, 124, 0x2f480018, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -42, 68, 0x6100013c, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -48, 78, 0x6100fe54, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -78, 40, 0x6100013c, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -84, 50, 0x6100fe54, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -96, 34, 0x6100fdd8, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -102, 36, 0x2f480008, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -120, 40, 0x610000ce, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -126, 34, 0x2c6c000c, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -132, 68, 0x610000ce, VE_IconTraceMsg);
  InsertPatch (ICON_LIB,  -72, 12, 0x2f490010, VE_IconLibMsg);	// Bag 2
  InsertPatch (ICON_LIB,  -84, 46, 0x6100e82c, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -138, 24, 0x6100e82c, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -150, 14, 0x2f490020, VE_IconLibMsg);
  InsertPatch (ICON_LIB, -192, 36, 0x2c6d002e, VE_IconLibMsg);
  InsertPatch (ICON_LIB,  -84, 24, 0x2f4e0020, VE_Dopus5LibMsg);
  InsertPatch (ICON_LIB,  -84, 16, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (ICON_LIB, -138, 20, 0x2f4e0018, VE_Dopus5LibMsg);
  InsertPatch (ICON_LIB, -138, 20, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (ICON_LIB, -138, 37, 0xec20404a, VE_Dopus5LibMsg);
  InsertPatch (ICON_LIB,  -42,  0, 0x60105359, VE_MCPMsg);
  InsertPatch (ICON_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (ICON_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (ICON_LIB,  -132,  0, 0x60105359, VE_MCPMsg);
  InsertPatch (ICON_LIB,  -180,  0, 0x60105359, VE_MCPMsg);
  InsertPatch (ICON_LIB, -84, 8, MakeULONG ("NoIn"), VE_NoInfoMsg);
  InsertPatch (ICON_LIB,  -90, 37, 0x1e4a8a67, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -150, 19, 0x3499cc2c, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -156, 23, 0x004a8d66, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -162, 19, 0x342a4a2f, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -180, 19, 0x3497cb95, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -186, 25, 0x387c004a, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB,  -90, 31, 0x1e4a8a67, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -150, 25, 0x34204d2e, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -156, 17, 0x004a8d66, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -162, 39, 0xc0720020, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -168, 31, 0x4c2e8d2a, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -180, 13, 0x3497cb95, VE_PowerIconsMsg);
  InsertPatch (ICON_LIB, -186, 19, 0x387c004a, VE_PowerIconsMsg);

	InsertPatch (MATHIEEE_LIB, -30, 0, 0xf2004400, VE_68060LibMsg);
	InsertPatch (MATHIEEE_LIB, -36, 0, 0xf2004000, VE_68060LibMsg);
	InsertPatch (MATHIEEE_LIB, -42, 0, 0xf2004400, VE_68060LibMsg);
	InsertPatch (MATHIEEE_LIB, -66, 0, 0xf2004400, VE_68060LibMsg);
	InsertPatch (MATHIEEE_LIB, -72, 0, 0xf2004400, VE_68060LibMsg);
	InsertPatch (MATHIEEE_LIB, -78, 0, 0xf2004400, VE_68060LibMsg);
	InsertPatch (MATHIEEE_LIB, -84, 0, 0xf2004400, VE_68060LibMsg);
	InsertPatch (MATHIEEE_LIB, -90, 4, 0xf2004403, VE_68060LibMsg);
  InsertPatch (MATHIEEE_LIB, -30, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (MATHIEEE_LIB, -78, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (MATHIEEE_LIB, -84, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (MATHIEEE_LIB, -30, 320, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -36, 300, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -42, 280, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -42,   1, 0x806b1c4a,				 VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -48, 224, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -48,   1, 0x80650866,				 VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -54, 208, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -54,   0, 0x0880001f,				 VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -60, 200, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -60,   0, 0x0840001f,				 VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -66, 192, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -72, 168, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -78, 144, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -84, 120, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -90,  96, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -96,  40, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -96,   1, 0x806dd4f2,				 VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -54, 0, 0xd080d080, VE_MathLibsPatchMsg);
  InsertPatch (MATHIEEE_LIB, -60, 0, 0x4a806706, VE_MathLibsPatchMsg);
  InsertPatch (MATHIEEE_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (MATHIEEE_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (MATHIEEE_LIB, -42, 3, 0x0ab08167, VE_PatchMSBMsg);
  InsertPatch (MATHIEEE_LIB, -48, 3, 0xf866ec4e, VE_PatchMSBMsg);
  InsertPatch (MATHIEEE_LIB, -42, 3, 0x0ab0816d, VE_PatchMSBMsg);
  InsertPatch (MATHIEEE_LIB, -96, 3, 0xd4f20044, VE_PatchMSBMsg);
  InsertPatch (MATHIEEE_LIB, -90, 3, 0x00f23c90, VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -96, 3, 0x00f23c90, VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -78, 6, 0x4423f200, VE_SetPatchSingBasMsg);

  InsertPatch (MATHFFP_LIB, -30, 0, 0x12006b34, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -36, 0, 0xf2004000, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -42, 0, 0x4a016a1e, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -48, 0, 0x70004a01, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -54, 0, 0x0200007f, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -60, 0, 0x4a006704, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -66, 4, 0x18016b00, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -72, 4, 0x18016700, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -78, 0, 0xd080670a, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -84, 0, 0xd080670a, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB,  -6, 0, 0x526e0020, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -12, 0, 0x536e0020, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -18, 0, 0x70004e75, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -24, 0, 0x70004e75, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -30, 1, 0x0112006b, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -42, 1, 0x016a184a, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -48, 1, 0x0142c16d, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -60, 1, 0x0066044e, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -66, 1, 0x80660420, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -72, 1, 0x80660460, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -78, 1, 0x8066044e, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -84, 1, 0x8066044e, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -90, 1, 0x0112006b, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -96, 1, 0x0112006b, VE_HSMathLibsMsg);
  InsertPatch (MATHFFP_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (MATHFFP_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (MATHFFP_LIB, -36, 1, 0x8067306a, VE_FFPPatchMsg);
  InsertPatch (MATHFFP_LIB, -42, 1, 0x016a1a4a, VE_FFPPatchMsg);

  InsertPatch (EXPANSION_LIB, -30, 16, 0x6100b336, VE_MMULibMsg);
  InsertPatch (EXPANSION_LIB, -30, 86, 0x61000082, VE_MMULibMsg);
	InsertPatch (EXPANSION_LIB, -30, 14, 0x2f490014, VE_68060LibMsg);
  InsertPatch (EXPANSION_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (EXPANSION_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);

	InsertPatch (WB_LIB, -72, 124, 0x2c6c0568, VE_PatchWBMsg);
	InsertPatch (WB_LIB, -78,  74, 0x2c6c0568, VE_PatchWBMsg);

  InsertPatch (WB_LIB,   -6,  34, 0x6100ff04, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -12,  26, 0x6100fdf8, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -18,  22, 0x2c6900dc, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -24,  38, 0x206d0832, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -30, 142, 0x6100fc2c, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -36,  28, 0x286b0004, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -42,  30, 0x61006594, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -48, 104, 0x61007ce0, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -54,  12, 0x6100f7dc, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -60,   4, 0x61002b26, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -60,   4, 0x61003004, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -66,  12, 0x6100fe18, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -72,  16, 0x2f4a002c, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -78,  54, 0x6100fc9a, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -84,  94, 0x61002320, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -90,  56, 0x610001ea, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -96,  98, 0x2c6d00f8, VE_WBLibMsg);
  InsertPatch (WB_LIB, -102,  68, 0x2c6d00dc, VE_WBLibMsg);
  InsertPatch (WB_LIB, -108,  46, 0x2c6d00dc, VE_WBLibMsg);
  InsertPatch (WB_LIB, -114,  22, 0x2f4e002c, VE_WBLibMsg);
  InsertPatch (WB_LIB, -120,  62, 0x246a003a, VE_WBLibMsg);
  InsertPatch (WB_LIB, -126,  72, 0x2c6d00dc, VE_WBLibMsg);
  InsertPatch (WB_LIB, -132,  68, 0x2c6d00dc, VE_WBLibMsg);

  InsertPatch (WB_LIB,  -18,  10, 0x2f4e0010, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -24,  30, 0x2f4e0008, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -30,  14, 0x2f4e0018, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -42,  16, 0x2f4e0018, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -48,  18, 0x2f4e001c, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -54,   6, 0x2f4e0004, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -60,   4, 0x61002aa6, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -66,   6, 0x2f4e0004, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -72,  16, 0x2f4a0024, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -78,  10, 0x2f4e0010, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -84,  14, 0x2f4e0014, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -90,  12, 0x2f4e0014, VE_WBLibMsg);
  InsertPatch (WB_LIB, -102,  22, 0x2f480020, VE_WBLibMsg);
  InsertPatch (WB_LIB, -108,  22, 0x2f490024, VE_WBLibMsg);
  InsertPatch (WB_LIB, -126,  12, 0x2f4a0024, VE_WBLibMsg);
  InsertPatch (WB_LIB, -132,  22, 0x2f480020, VE_WBLibMsg);
  InsertPatch (WB_LIB,  -48,  16, 0x2f4a0028, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -48,  22, 0x2f4a0020, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -54,  12, 0x2f4e0014, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -60,  14, 0x2f4c0030, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -60,  40, 0x2f4c0040, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -66,  14, 0x2f4e0020, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -66,  12, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -72,  14, 0x2f4a002c, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -78,  14, 0x2f4e0020, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB,  -90, 376, MakeULONG ("Extr"), VE_ExtraInfoMsg);
  InsertPatch (WB_LIB,  -60,   0, 0x60105359, VE_MCPMsg);
	InsertPatch (WB_LIB,  -90, 171, MakeULONG ("RAWB"), VE_RAWBInfoMsg);
	InsertPatch (WB_LIB,  -90,  18, 0x2c6d0062, VE_SwazInfoMsg);
	InsertPatch (WB_LIB,  -60,   4, 0x2f0e2f0c, VE_ForceIconMsg);
	InsertPatch (WB_LIB,  -66,  26, 0x4e912e00, VE_ForceIconMsg);

// Bag 2
  InsertPatch (WB_LIB,  -6, 10, 0x2f4e000c, VE_WBLibMsg);
  InsertPatch (WB_LIB, -12, 14, 0x2f4e000c, VE_WBLibMsg);
  InsertPatch (WB_LIB, -24, 28, 0x2f4e0008, VE_WBLibMsg);
  InsertPatch (WB_LIB, -30, 14, 0x2f480018, VE_WBLibMsg);
  InsertPatch (WB_LIB, -48, 20, 0x2f4e0024, VE_WBLibMsg);
  InsertPatch (WB_LIB, -60,  4, 0x61002c60, VE_WBLibMsg);
  InsertPatch (WB_LIB, -72, 26, 0x2f4e0038, VE_WBLibMsg);
  InsertPatch (WB_LIB, -78,  4, 0x2f4e0004, VE_WBLibMsg);
  InsertPatch (WB_LIB, -96, 32, 0x2f480024, VE_WBLibMsg);
  InsertPatch (WB_LIB, -48, 16, 0x2f4a0024, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -54, 12, 0x2f4e0010, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -60, 44, 0x2f4e0044, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -72, 82, 0x2668002e, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -60,  4, 0x61003050, VE_WBLibMsg);
  InsertPatch (WB_LIB, -72, 34, 0x2f4e0040, VE_WBLibMsg);
  InsertPatch (WB_LIB, -96, 88, 0x2f490038, VE_WBLibMsg);
  InsertPatch (WB_LIB, -48, 14, 0x2f4a0028, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -48, 14, 0x2f4a0024, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -54, 12, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -60, 23, 0x349bcd78, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -90,  8, 0x2f4a002c, VE_Dopus5LibMsg);
	InsertPatch (WB_LIB, -90, 24, 0x45ec0860, VE_RAWBInfoMsg);
	InsertPatch (WB_LIB, -90, 22, 0x45ec8912, VE_RAWBInfoMsg);
  InsertPatch (WB_LIB, -90, 24, 0x45ec0878, VE_RAWBInfoMsg);
	InsertPatch (WB_LIB, -60, -52, MakeULONG ("AppC"), VE_AppChangeMsg);
	InsertPatch (WB_LIB, -84, -18, MakeULONG ("CYBI"), VE_CGXLibMsg);
  InsertPatch (WB_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (WB_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);

  InsertPatch (ASL_LIB, -30, 78, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (ASL_LIB, -36,  6, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (ASL_LIB, -42,  4, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (ASL_LIB, -48, 74, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (ASL_LIB, -54,  6, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (ASL_LIB, -60,  2, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (ASL_LIB, -48, 19, 0x00706642, VE_AslToRTMsg);
  InsertPatch (ASL_LIB, -54, 25, 0x116100db, VE_AslToRTMsg);
  InsertPatch (ASL_LIB, -60, 11, 0x5a2a4841, VE_AslToRTMsg);
  InsertPatch (ASL_LIB, -30, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (ASL_LIB, -36, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (ASL_LIB, -42, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (ASL_LIB, -48, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (ASL_LIB, -54, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (ASL_LIB, -60, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (ASL_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);
  InsertPatch (ASL_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (ASL_LIB, -60, 23, 0x03700030, VE_AslSizePatchMsg);
  InsertPatch (ASL_LIB, -12,  3, 0x16600420, VE_ReqChangeMsg);
  InsertPatch (ASL_LIB, -18,  3, 0x18700361, VE_ReqChangeMsg);
  InsertPatch (ASL_LIB, -30, 11, 0xca93c94e, VE_ReqChangeMsg);
  InsertPatch (ASL_LIB, -36,  3, 0xca93c94e, VE_ReqChangeMsg);
  InsertPatch (ASL_LIB, -42, 23, 0x36672208, VE_ReqChangeMsg);
  InsertPatch (ASL_LIB, -48, 17, 0x36672208, VE_ReqChangeMsg);
  InsertPatch (ASL_LIB, -54, 11, 0x9e200867, VE_ReqChangeMsg);
  InsertPatch (ASL_LIB, -60, 11, 0xb0cb4ec1, VE_ReqChangeMsg);

  InsertPatch (REQTOOLS_LIB,  -30, 19, 0x00706942, VE_AslToRTMsg);
  InsertPatch (REQTOOLS_LIB,  -36, 29, 0x38204d61, VE_AslToRTMsg);
  InsertPatch (REQTOOLS_LIB,  -48, 21, 0x3b264841, VE_AslToRTMsg);
  InsertPatch (REQTOOLS_LIB,  -54, 19, 0x6726482a, VE_AslToRTMsg);
  InsertPatch (REQTOOLS_LIB,  -60,  3, 0x0c2a4841, VE_AslToRTMsg);
  InsertPatch (REQTOOLS_LIB,  -96, 31, 0xd9264841, VE_AslToRTMsg);
  InsertPatch (REQTOOLS_LIB, -144, 31, 0x21264841, VE_AslToRTMsg);
  InsertPatch (REQTOOLS_LIB, -72, 7, 0x5170254e, VE_SaferPatchesMsg);
  InsertPatch (REQTOOLS_LIB, -66, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (REQTOOLS_LIB, -12, 19, 0x84671c22, VE_SaferPatches1Msg);

  InsertPatch (REQTOOLS_LIB, -18, 17, 0xd8671826, VE_SaferPatches1Msg);
  InsertPatch (REQTOOLS_LIB, -66, 21, 0xfd670c4c, VE_ReqAttackMsg);

  InsertPatch (CPU_INT,   8,  0, 0x082f0002, VE_68060LibMsg);
  InsertPatch (CPU_INT,   8,  8, 0x2a400880, VE_MMULibMsg);
  InsertPatch (CPU_INT,   8, 21, 0x00664430, VE_MMULibMsg);
  InsertPatch (CPU_INT,  44, 35, 0x442010e9, VE_68060LibMsg);
  InsertPatch (CPU_INT,  44, 15, 0x40f32748, VE_68040LibMsg);
  InsertPatch (CPU_INT, 100, 17, 0x0e675ac2, VE_SystemPatchMsg);
  InsertPatch (CPU_INT, 104, 17, 0x0e6720c2, VE_SystemPatchMsg);
  InsertPatch (CPU_INT, 108, 17, 0x0e6758c2, VE_SystemPatchMsg);
  InsertPatch (CPU_INT, 112, 17, 0x0e670000, VE_SystemPatchMsg);
  InsertPatch (CPU_INT, 116, 17, 0x0e673cc2, VE_SystemPatchMsg);
  InsertPatch (CPU_INT, 120, 17, 0x0e673cc2, VE_SystemPatchMsg);
  InsertPatch (CPU_INT,  44, 15, 0x00200967, VE_CyberPatcherMsg);
  InsertPatch (CPU_INT,   8,  0, 0x2f00202f, VE_COPMsg);
  InsertPatch (CPU_INT,  12,  1, 0x002f0830, VE_COPMsg);
  InsertPatch (CPU_INT,  16,  3, 0xc0206f00, VE_COPMsg);
  InsertPatch (CPU_INT,  20,  1, 0x002f0830, VE_COPMsg);
  InsertPatch (CPU_INT,  24,  1, 0x002f0830, VE_COPMsg);
  InsertPatch (CPU_INT,  28,  1, 0x002f0830, VE_COPMsg);
  InsertPatch (CPU_INT,  32,  9, 0x20007c07, VE_COPMsg);
  InsertPatch (CPU_INT,  36, 10, 0xff7c0839, VE_COPMsg);
  InsertPatch (CPU_INT,  40,  1, 0x002f0830, VE_COPMsg);
  InsertPatch (CPU_INT,  44,  9, 0x1648e780, VE_COPMsg);
  InsertPatch (CPU_INT,  52,  1, 0x002f0830, VE_COPMsg);
  InsertPatch (CPU_INT,  56,  8, 0x670a2f7c, VE_COPMsg);
  InsertPatch (CPU_INT,  60,  1, 0x002f0830, VE_COPMsg);
  InsertPatch (CPU_INT,  96,  1, 0x002f0830, VE_COPMsg);
  InsertPatch (CPU_INT, 100,  3, 0x64602a61, VE_COPMsg);
  InsertPatch (CPU_INT, 104,  9, 0x0e487800, VE_COPMsg);
  InsertPatch (CPU_INT, 108,  7, 0x6c602048, VE_COPMsg);
  InsertPatch (CPU_INT, 112,  3, 0x70601a48, VE_COPMsg);
  InsertPatch (CPU_INT, 116,  3, 0x74601448, VE_COPMsg);
  InsertPatch (CPU_INT, 120,  3, 0x78600e08, VE_COPMsg);
  InsertPatch (CPU_INT, 124,  1, 0x734e7148, VE_COPMsg);
  InsertPatch (CPU_INT,  44, 12, 0x42c02940, VE_OxyPatcherMsg);
  InsertPatch (CPU_INT,   8,  0, 0x46fc2700, VE_CyberGuardMsg);
  InsertPatch (CPU_INT,   8, 13, 0x0c673c48, VE_BDebugMsg);
  InsertPatch (CPU_INT,   8,  0, 0x2F00302F, VE_VMMMsg);
  InsertPatch (CPU_INT,   8,  0, 0x3e2f0048, VE_EnforcerMsg);
  InsertPatch (CPU_INT,   8,  0, 0x007c0700, VE_EnforcerMsg);
  InsertPatch (CPU_INT,   8,  0, 0x08af0000, VE_EnforcerMsg);
  InsertPatch (CPU_INT, 100, 34*4, 0x4eaefebc, VE_EnforcerMsg);
  InsertPatch (CPU_INT, 100,  3, 0xf02f0020, VE_EnforcerMsg);

  InsertPatch (EXEC_INT,  6,  9, 0x3a673008, VE_SystemPatchMsg);
  InsertPatch (EXEC_INT,  0,  4, 0x009c0801, VE_MiamiMsg);
  InsertPatch (EXEC_INT, 11,  4, 0x317c0800, VE_MiamiMsg);
  InsertPatch (EXEC_INT,  0, 11, 0x022c4908, VE_SerDeviceMsg);
  InsertPatch (EXEC_INT, 11,  9, 0x186be431, VE_SerDeviceMsg);
  InsertPatch (EXEC_INT,  2,  0, 0x33fc0004, VE_PowerPcLibMsg);
  InsertPatch (EXEC_INT,  7, 43, 0x10660a21, VE_PaulaAudioMsg);
  InsertPatch (EXEC_INT,  8,  3, 0x80009c4e, VE_PaulaAudioMsg);
  InsertPatch (EXEC_INT,  9,  3, 0x80009c4e, VE_PaulaAudioMsg);
  InsertPatch (EXEC_INT, 10,  3, 0x80009c4e, VE_PaulaAudioMsg);

  InsertPatch (VE_TAG, 0,  10, 0x012C00CE, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x01020070, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x022c0070, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x012c0068, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x012c006a, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x022D0969, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x002d0086, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x812c0305, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x002c0085, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x012b030a, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x012d0850, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x002d00af, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x012c00dd, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x012d00c9, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x002c0000, VE_AmigaOSROMUpdate);
  InsertPatch (VE_TAG, 0,  10, 0x01010008, VE_FastRADMsg);
  InsertPatch (VE_TAG, 0,  10, 0x01280468, VE_MuMove4KMsg);
  InsertPatch (VE_TAG, 0,  10, 0x0128047f, VE_MuMove4KMsg);
  InsertPatch (VE_TAG, 0, 383, MakeULONG ("IDE-"), VE_IdeFixMsg);
  InsertPatch (VE_TAG, 0, 391, MakeULONG ("IDE-"), VE_IdeFixMsg);
  InsertPatch (VE_TAG, 0,  26, MakeULONG ("ramd"), VE_RamDriveUnitMsg);
  InsertPatch (VE_TAG, 0,  53, MakeULONG ("CFix"), VE_BPPCFixMsg);
  InsertPatch (VE_TAG, 0, 106, 0x434f5000, VE_COPMsg);
  InsertPatch (VE_TAG, 0,  26, MakeULONG ("Soft"), VE_SoftSCSIMsg);
  InsertPatch (VE_TAG, 0,  10, 0x01010068, VE_A1200_HDPatchMsg);
  InsertPatch (VE_TAG, 0,  40, MakeULONG ("Turb"), VE_TurboValMsg);
  InsertPatch (VE_TAG, 0, -44, MakeULONG ("FE02"), VE_FastExecMsg);
  InsertPatch (VE_TAG, 0,  47, MakeULONG ("ory "), VE_MemoryKicktagMsg);
  InsertPatch (VE_TAG, 0, -11, MakeULONG ("Boot"), VE_BootPrefsMsg);
  InsertPatch (VE_TAG, 0,  58, MakeULONG ("Prep"), VE_PrepareEmulMsg);
  InsertPatch (VE_TAG, 0,  44, MakeULONG ("Warm"), VE_ReserveWarmMsg);
  InsertPatch (VE_TAG, 0, 837, MakeULONG ("Afte"), VE_BootControlMsg);
  InsertPatch (VE_TAG, 0,-104, MakeULONG ("Boot"), VE_BootControlMsg);
  InsertPatch (VE_TAG, 0,  26, MakeULONG ("OXYP"), VE_OxyPatcherMsg);
  InsertPatch (VE_TAG, 0,  64, MakeULONG ("adMo"), VE_LoadModuleMsg);
  InsertPatch (VE_TAG, 0, 123, MakeULONG ("yb-B"), VE_CybBootPrefsMsg);
  InsertPatch (VE_TAG, 0, 123, MakeULONG ("yb-B"), VE_CybBootPrefsMsg);
  InsertPatch (VE_TAG, 0,  10, 0x01280007, VE_EnhancedAlertMsg);

  InsertPatch (VE_MEM, 0,  28, MakeULONG ("Amig"), VE_AmigaOSROMUpdate);
  InsertPatch (VE_MEM, 0,  32, MakeULONG ("Amig"), VE_AmigaOSROMUpdate);
  InsertPatch (VE_MEM, 0,  40, MakeULONG ("Amig"), VE_AmigaOSROMUpdate);
  InsertPatch (VE_MEM, 0,  44, MakeULONG ("Amig"), VE_AmigaOSROMUpdate);
  InsertPatch (VE_MEM, 0,  48, MakeULONG ("Amig"), VE_AmigaOSROMUpdate);
  InsertPatch (VE_MEM, 0,  52, MakeULONG ("Amig"), VE_AmigaOSROMUpdate);
  InsertPatch (VE_MEM, 0, 340, MakeULONG ("tRAD"), VE_FastRADMsg);
  InsertPatch (VE_MEM, 0, 357, MakeULONG ("IDE-"), VE_IdeFixMsg);
  InsertPatch (VE_MEM, 0, 365, MakeULONG ("IDE-"), VE_IdeFixMsg);
  InsertPatch (VE_MEM, 0, -26, MakeULONG ("ramd"), VE_RamDriveUnitMsg);
  InsertPatch (VE_MEM, 0,  76, MakeULONG ("MuMo"), VE_MuMove4KMsg);
  InsertPatch (VE_MEM, 0,  27, MakeULONG ("CFix"), VE_BPPCFixMsg);
  InsertPatch (VE_MEM, 0,  34, 0x434f5000, VE_COPMsg);
  InsertPatch (VE_MEM, 0,  24, MakeULONG ("Miam"), VE_MiamiMsg);
  InsertPatch (VE_MEM, 0,  30, MakeULONG ("Delu"), VE_MiamiDeluxeMsg);
  InsertPatch (VE_MEM, 0,  62, MakeULONG ("Soft"), VE_SoftSCSIMsg);
  InsertPatch (VE_MEM, 0,  51, MakeULONG ("00_H"), VE_A1200_HDPatchMsg);
  InsertPatch (VE_MEM, 0,  72, MakeULONG ("Turb"), VE_TurboValMsg);
  InsertPatch (VE_MEM, 0,  -8, MakeULONG ("FE02"), VE_FastExecMsg);
  InsertPatch (VE_MEM, 0, -53, MakeULONG ("Boot"), VE_BootPrefsMsg);
  InsertPatch (VE_MEM, 0,  32, MakeULONG ("Prep"), VE_PrepareEmulMsg);
  InsertPatch (VE_MEM, 0,  76, MakeULONG ("Warm"), VE_ReserveWarmMsg);
  InsertPatch (VE_MEM, 0, 811, MakeULONG ("Afte"), VE_BootControlMsg);
  InsertPatch (VE_MEM, 0,-130, MakeULONG ("Boot"), VE_BootControlMsg);
  InsertPatch (VE_MEM, 0,  64, MakeULONG ("OXYP"), VE_OxyPatcherMsg);
  InsertPatch (VE_MEM, 0,  24, MakeULONG ("« Lo"), VE_LoadModuleMsg);
  InsertPatch (VE_MEM, 0,  28, 0x00000c30, VE_EnhancedAlertMsg);
}
