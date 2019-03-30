#define __USE_SYSBASE 1

#include <JEO:JEO.h>
#include <exec/lists.h>
#include <proto/xvs.h>
#include <libraries/reqtools.h>
//#define MWDEBUG 1
//#include <JEO:memwatch.h>

UWORD CenterWindowLeft (UWORD width);
UWORD CenterWindowTop (UWORD height);

#define  OFFY (Scr->WBorTop + Scr->RastPort.TxHeight - 10)
extern struct TextAttr topaz8;
extern struct xvsVirusList *viruslist;

#define BUFSIZE	1024
#define PATCHBRAINSIZE	40
#define PATCH_NAME_LENGTH 38

VOID PF (char *String);
VOID ClearBar (VOID);

#define CRYPT	0
#define DECRYPT	1

VOID VE_Crypt (BOOL mode, UBYTE *Buffer, LONG crypt_size);

extern char xvs_name[];
extern char xfdmaster_name[];
extern char xadmaster_name[];

extern char SEG_SEM[];

extern BOOL enforcer_flag;
extern BOOL restart_flag;

extern UWORD diskShowY;
extern UWORD processorFlag;
extern struct ExecBase *SysBase;
extern struct TextFont *textfont;

extern char Tmp1[], Tmp2[], Tmp3[], Tmp4[];
extern BOOL aminet_flag;
extern BOOL icon;			// iconified
extern char FullName[];
extern ULONG fib_len;

extern char VE[];
extern char VEAssign[];
extern char DataName[];
extern char ProgramVersion[];

extern char xvs_Version[];
extern char patch_Version[];
extern char xfd_Version[];
extern char xad_Version[];

BOOL FindSegAddress (ULONG address, char *Name);
extern char N_A_Msg[];

extern ULONG asl_version, reqtools_version;

#define MAX_LVO_LEN 26

extern BOOL decrypt_flag;

enum
{
	EXEC_LIB = 0,	// LVO
	DOS_LIB,		// LVO
	UTIL_LIB,		// LVO
	GFX_LIB,		// LVO
	INT_LIB,		// LVO
	GAD_LIB,		// LVO
	LAY_LIB,		// LVO
	ICON_LIB,		// LVO
	AUDIO_DEV,
	CONSOLE_DEV,
	INPUT_DEV,
	SCSI_DEV,
	TIMER_DEV,
	TRACKDISK_DEV,
	MATHIEEE_LIB,	// LVO
	GAMEPORT_DEV,
	KEYBOARD_DEV,
	RAMDRIVE_DEV,
	CARDDISK_DEV,
	ROMBOOT_DEV,
	MATHFFP_LIB,	// LVO
	KEYMAP_LIB,		// LVO
	WB_LIB,			// LVO
	EXPANSION_LIB,	// LVO
	REQTOOLS_LIB,
	ASL_LIB,
	CPU_INT,
	EXEC_INT,
	VE_TAG,
	VE_MEM,
	PATCH_END
};

typedef struct
{
  struct List list;
} ext_Sjef;
extern ext_Sjef ext_sjef;

typedef struct
{
  struct Node nd;
  char Ext[50];
} Extbase;

typedef struct
{
	UWORD type,
		  name_off;
	WORD  function,
		  offset;
	ULONG data;
	
} VEPatch;

extern char AMOSJoshuaVirus[];
extern char AMOSJoshuaCloneVirus[];
extern char MUIVirus[];
extern BOOL xfdmaster_flag;
extern BOOL xadmaster_flag;

extern char ve_UnknVecText[];

extern UBYTE Ver[];

extern char PatchBrain[];
extern UWORD patchBrainVersion;
extern UWORD patchBrainRevision;
extern ULONG check_sum;

extern ULONG max_entries;

enum
{
  S_NO_SOUND = -1,
  S_MESSAGE,
  S_ERROR,
  S_VIRUS
};

extern char Sounds[][12];

enum
{
  MEMORY = 0,
  RESOURCES,
  DEVICES,
  INTRS,
  LIBS,
  PORTS,
  TASKS,
  PATCHES
};

enum
{
  LOG_TEXT = 0,
  LOG_TEXT_LINE,
  LOG_LINE_TEXT,
  LOG_LINE_TEXT_LINE,
  LOG_DATE,
  LOG_END
};

VOID FindMemList (VOID);
ULONG CalculateChecksum (UBYTE mode);
BOOL CheckJoshua (UWORD *Buffer, LONG size);
BOOL CheckJoshuaClone (UWORD *Buffer, LONG size);
VOID CheckPatches (BYTE mode);
VOID UpDateFileCount (VOID);
VOID Do_file_sizes (BOOL mode);
VOID StripFile (char *FullName, char *FileName);
VOID FileStat (char *FullName);
BOOL HandleListIDCMP (VOID);
VOID ShowStuff (UWORD mode);
VOID PrefsWindow (VOID);
VOID FindLibraryVersion (char *String, APTR Lib);
ULONG JEOSoundRequest (char *Title, char *TextFormat, char *GadgetFormat, BYTE sound);
VOID PlaySound (BYTE sound);
UBYTE CheckLoadWB (VOID);	// Joshua Trojan + clone
BOOL LoadPatchData (VOID);
BYTE Check_checksum (char *FileName);
VOID CPUSpeed (VOID);
VOID ViewMem (ULONG start, WORD off);
VOID OpenLog (VOID);
VOID CloseLog (VOID);
VOID WriteToLog (char *Text, UWORD mode);
VOID TimerCheck (VOID);
VOID ext_Open (VOID);
VOID ext_Close (VOID);
VOID UpDateDriveInfo (VOID);

VOID s_fib_Open (VOID);
VOID s_fib_Close (VOID);
VOID s_AddDir (VOID);
VOID Add_files (VOID);
VOID s_ListFiles (VOID);
VOID s_LoadFiles (VOID);
VOID s_SaveFiles (VOID);
VOID Check_changed (VOID);
VOID Update_files (VOID);
VOID UnIconify (VOID);
VOID Iconify (VOID);

extern BOOL s_save_flag;

extern UWORD ROM_version;
extern UWORD ROM_revision;
extern UWORD hw_ROM_version;
extern UWORD hw_ROM_revision;

extern char ROMVersion[];
extern char HW_ROMVersion[];
extern char OSVer[];

extern ULONG ROM_start, ROM_end, ROM_size;
extern ULONG hw_ROM_start, hw_ROM_end, hw_ROM_size;
extern ULONG exec_start, exec_end, exec_size;
extern MD5Data exec_md5;

extern struct Process *pr;

extern ULONG timeStart, msecs;
extern struct RastPort *rp;
extern ULONG dirCount, fileCount, archiveCount, cFound, cFiles, virusCount;
extern ULONG len, exeFiles, dataFiles, skippedFiles, archived;
extern char CrunchName[];
extern BOOL	filestat;

extern UWORD mainX;
extern BOOL debug;
extern BOOL JEO_debug;
extern struct xvsBootInfo *bootinfo;
extern struct xvsFileInfo *fileinfo;
extern struct xvsSectorInfo *sectorinfo;

typedef struct
{
	APTR warm, check, mem, tag, cool, cold;
	ULONG DoIO, CBump, TrackIO;
	ULONG Int_sum;
} Vectors;

extern Vectors *vec, *tmp_vec;

extern UBYTE Ready[];

typedef struct
{
	char Name[108];
	int size;
	char Future[100];
} File_sizes;

extern File_sizes *fs[];

extern BOOL wb_flag;

typedef struct
{
  ULONG bootVir,		// bootblock virus found
        fileVir,		// filevirus found
        checked,		// total bootblocks checked
        fileChecked;	// total files checked
  UBYTE FileCheckPath[108],
		PicturesPath[108],
		ConvertTextPath[108];
  BOOL	checkData;
  UWORD	version;
  struct rtScreenModeRequester screenmode;
  BOOL 	check_crunched,
  		extract_archives, n_a1, n_a2,
  		skip_encrypted,
  		hide_known_patches,
  		overwrite_crunched_files,
  		fast_startup,
  		skip_days;
  UWORD days_count;
  BOOL	wb_flag;

  BOOL	fw_include_data, fw_exclude_icons, fw_confirm;
  char	FWDirPath[256];
  UWORD fw_count;

  BOOL  fast_file_checking, logging, iconify;

  char Log_path[128];
  UBYTE	dump;	// Memory monitor 0 - 2
  BOOL address_mode_flag;
  char xfdUpdateMD5[33], xadUpdateMD5[33];
  UBYTE future[339];
} Prefs;

extern Prefs *prefs;

typedef struct
{
	ULONG  patch_start,
				 patch_end;
	BOOL   flag;
} patch_values;

extern patch_values far pv[];

typedef struct
{
  struct Node nd;
  char FileName[256];
  LONG size;
  MD5Data md5;
} s_Base;

typedef struct
{
  struct List list;
} fib_Sjef;
extern fib_Sjef fib_sjef;

typedef struct
{
  struct Node nd;
  UBYTE FileName[500];
  ULONG size;
  ULONG flag;
} Filbase;

typedef struct
{
	ULONG type;
	ULONG checkSum;
	UWORD offset;
	UBYTE Data[4];
} OtherBoots;
extern OtherBoots *otherboots[];

#define UTILITY			0x00000001
#define VIRUSKILLER		0x00000002
#define	INTRO			0x00000004
#define LOADER			0x00000008

#define GREY	0
#define BLACK	1
#define WHITE	2
#define BLUE	3
#define GREEN	4
#define YELLOW	5
#define RED		6

#define NORMAL_OFS		  0
#define NORMAL_FFS		  1
#define NORMAL_OFS_INT	  2
#define NORMAL_FFS_INT	  3
#define NORMAL_OFS_DC	  4
#define NORMAL_FFS_DC	  5
#define DATABASE		255	

#define NODISKINDRIVE	5
#define WRITEPROTECTED	6

VOID SaveROM (VOID);
VOID CleanUp (VOID);
VOID Status (char *Streng);
VOID DiskStatus (UBYTE *Streng, WORD unit);
WORD VE_ReadBoot (WORD unit);
VOID ViewBoot (UWORD unit);
WORD CheckForNormal (WORD unit);
BOOL CheckForNoInstall (WORD unit);
BOOL CheckForFlashback (WORD unit);
BOOL CheckForQuarterback (WORD unit);
LONG Install (WORD unit, WORD mode);
VOID VE_CheckBoot (WORD unit);
BOOL FInstall (WORD unit);
VOID Menus (BOOL status);
VOID All (BOOL mode);
VOID MenuPick (USHORT code);
VOID RawKey (USHORT code);
VOID MouseButtons (USHORT code);
BYTE WhoChanged (VOID);
VOID DiskInserted (VOID);
VOID ClearDiskBuffer (WORD unit);
VOID DiskRemoved (VOID);
VOID ViewXY (VOID);
VOID VE_About (VOID);
VOID CheckMemVectors (UWORD *AnalyzeBuf, ULONG i);
VOID Analyze (UWORD *AnalyzeBuf, WORD unit);
VOID AnalyzeWrite (UWORD *Buf, UWORD start);
WORD CheckBootVirus (WORD u);
ULONG CheckOtherBoots (ULONG *Buffer);
BOOL CheckIt (UWORD start, UBYTE *String, UWORD unit);
BOOL Print (UBYTE *String);
LONG CheckCrunchers (UBYTE *Name, ULONG *FileBuffer, ULONG h);
//BOOL CheckFileVirus (APTR *Address, LONG size);
BOOL Check (UBYTE *Name, LONG size);
BOOL Scan (UBYTE *Dir, LONG count);
VOID CheckFiles (UBYTE *Dir, BOOL mode);
BOOL MemSaddam (APTR start);
BOOL MemBGS9 (APTR start);
BOOL MemButonic (APTR start);
BOOL MemCCCP (VOID);
BOOL FileVirusInMemRequest (UBYTE *Virus, BOOL mode);
BOOL VirusInMemRequest (UBYTE *Virus, BOOL mode);
VOID CheckMemory (BYTE mode);
VOID BaseCleanUp (VOID);
BOOL InsertBase (UBYTE *Name, UBYTE *Buf);
VOID DumpList (VOID);
BOOL DeleteOne (UBYTE *Name);
VOID DeleteAll (VOID);
BOOL SaveBase (BOOL end);
VOID LoadBase (VOID);
BOOL CheckNameBufExist (UBYTE *Name, UBYTE *Buf);
VOID MergeBase (VOID);
VOID ShowMouseName (WORD mouseX, UWORD mouseY);
WORD FindXY (WORD mouseX, UWORD mouseY);
VOID EditName (VOID);
VOID UpdateNameAndBuf (WORD nr);
WORD GetBaseBoot (UBYTE *WTitle);
BOOL CheckBootExist (WORD unit);
BOOL CheckNameExist (UBYTE *Name);
BOOL CheckRecorded (WORD unit);
VOID ViewBase (VOID);
BOOL DeleteOneRecord (VOID);
VOID RecordBoot (UWORD unit);
VOID Record (VOID);
BOOL LoadBaseBoot (VOID);
VOID UpDateVirusCount (VOID);
VOID PrintTime (VOID);
BOOL CheckNotBoot (UWORD unit, UBYTE *Msg);

WORD TCheckBootVirus (WORD u);
WORD TCheckOtherBoots (WORD u);
BOOL TCheckIt (UWORD start, UBYTE *String, WORD unit);
BOOL TCheckBootExist (WORD unit);
BOOL TCheckRecorded (WORD unit);
struct Node *FindTheOne (UBYTE *Name);
BOOL TInsertBase (UBYTE *Name, UBYTE *Buf);
BOOL TSaveBase (BOOL end);
VOID TLoadBase (VOID);
VOID fib_Open (VOID);
VOID fib_Close (VOID);
VOID fib_SlettBase (VOID);
VOID fib_Insett (char *FullName, LONG size, ULONG flag);
VOID tderror (WORD error, WORD unit);

VOID OpenOtherBoots (VOID);
VOID CloseOtherBoots (VOID);

VOID Rename_pics (VOID);
VOID Convert_PC_text (VOID);

USHORT Check_RAW (struct Window *w);
BOOL Check_file_intuition (VOID);
BOOL Check_pack_files (UBYTE *PackName, UWORD count);
VOID CheckSector (ULONG startcyl, ULONG endcyl);

extern UBYTE *Drive[];
extern UBYTE Ok[];
extern UBYTE Beskjed[];
extern struct Screen *Scr;
extern LONG drive[4];
extern UBYTE *Buffer[4];
extern UBYTE *Dummy;
extern UBYTE PleaseWait[];
extern struct Window *VirusWnd;
extern ULONG fileCount;
extern UWORD pack_checking;
extern ULONG p_dirCount;
extern BOOL checkArchives;
extern UBYTE TDError[][44];
extern char TDErrorMsg[50];
extern UWORD viewCol[];

extern struct MsgPort *diskport;
extern struct IOExtTD *diskreq;

extern char far PatchNames[][PATCH_NAME_LENGTH];
extern UWORD ve_max_patches;

char *GLS (struct LocText *loctext);
// Global
extern struct LocText L_NO_DISK;	
extern struct LocText L_MESSAGE;
extern struct LocText L_OK;	 	
extern struct LocText L_READY;
extern struct LocText L_PLEASE_WAIT;
extern struct LocText L_WARNING;
extern struct LocText L_EXIT;
extern struct LocText L_BOOTBLOCK;
extern struct LocText L_BOOTBLOCKS;
extern struct LocText L_NO_MEM;

// VE_Files.c
extern struct LocText L_ENTER_PW;
extern struct LocText L_FILE_NAME_ERROR;
extern struct LocText L_INFECTED_DELETE;		
extern struct LocText L_VIRUS_WARNING;			
extern struct LocText L_DELETE_FILE_AUTO;		
extern struct LocText L_FILE_NOT_DELETED;		
extern struct LocText L_DATA;					
extern struct LocText L_VIRUS_INFECTED_ARCHIVE;	
extern struct LocText L_FILE_VIRUS_WARNING;	
extern struct LocText L_FILE;					
extern struct LocText L_LINK_VIRUS_WARNING;	
extern struct LocText L_VIRUS_INFECTED_FILE;	
extern struct LocText L_REMOVE_VIRUS_AUTO;		
extern struct LocText L_SAVE_NEW_FILE;			
extern struct LocText L_ERROR_WRITING_FILE;	
extern struct LocText L_ERROR_OPENING_FILE;	
extern struct LocText L_ERROR_VIRUS_NOT_REMOVED;
extern struct LocText L_ERROR_REMOVING_VIRUS;	
extern struct LocText L_DELETE_FILE_STOP;		
extern struct LocText L_FILE_NOT_DELETED;		
extern struct LocText L_UNLINKING_FILE;		
extern struct LocText L_ERROR_UNLINKING;		
extern struct LocText L_EXTRACTING;			
extern struct LocText L_CHECKING;				
extern struct LocText L_ERROR_READING_FILE;
extern struct LocText L_FILE_CHECK_PAUSED;		
extern struct LocText L_CONTINUE_STOP;			
extern struct LocText L_USER_BREAK;			
extern struct LocText L_ERROR65;				
extern struct LocText L_ERROR_LOCKING_DIR;		
extern struct LocText L_CHECK_FILES_VIRUS;		
extern struct LocText L_ERROR46;				
extern struct LocText L_FILE_SCAN_END;			
extern struct LocText L_DIR;					
extern struct LocText L_DIRS;					
extern struct LocText L_ARCHIVE;				
extern struct LocText L_ARCHIVES;				
extern struct LocText L_MINUTE;				
extern struct LocText L_MINUTES;				
extern struct LocText L_SECOND;				
extern struct LocText L_SECONDS;				
extern struct LocText L_FOUND;					
extern struct LocText L_NO_KNOWN;				
extern struct LocText L_FILE_CHECK_STATUS;
extern struct LocText L_FILE;
extern struct LocText L_FILES;
extern struct LocText L_FILE1;
extern struct LocText L_FOUND_A_CRUNCHED_FILE;
extern struct LocText L_DO_IT_TURN_OFF;
extern struct LocText L_DATA_VIRUS_WARNING;
extern struct LocText L_ARCHIVE_IS_CRYPTED;
extern struct LocText L_ARCHIVE_IS_CORRUPT;
extern struct LocText L_FILE_SCAN_END1;
extern struct LocText L_ARCHIVE_ERROR_1;
extern struct LocText L_SAVING_FILE;
extern struct LocText L_DELETE_ARCHIVE;
extern struct LocText L_DO_FILE_CHECK_VOL;
extern struct LocText L_CONTINUE_SKIP_STOP;
extern struct LocText L_VIRUS_REMOVED_DIFF;
extern struct LocText L_HIDDEN_EXE_FOUND;
extern struct LocText L_DECODE_CONTINUE;
extern struct LocText L_FOUND_A_SHELL_SCRIPT;
extern struct LocText L_ERROR_X;

// VE_Mem.c
extern struct LocText L_REBOOT_CANCEL;
extern struct LocText L_NO_ENTRIES_FOUND;
extern struct LocText L_ADDRESS_NAME;	
extern struct LocText L_LOOK_FOR_TEXT;			
extern struct LocText L_IT_MIGHT_MEMVIRUS;
extern struct LocText L_VIEW_MEMORY;
extern struct LocText L_ERROR_CLR_VECTORS;
extern struct LocText L_MEM_VEC_OK;
extern struct LocText L_VECTOR_CHECK;
extern struct LocText L_VIRUS_DETECTED_IN_MEM;
extern struct LocText L_VIRUS_REMOVED_MEM;
extern struct LocText L_NO_KNOWN_VIRUS_MEM;	
extern struct LocText L_GREAT;	
extern struct LocText L_MEMORY_CHECK;
extern struct LocText L_EXEC_INTERRUPT;
extern struct LocText L_KICKMEM_ENTRIES;
extern struct LocText L_KICKSTART_AT;
extern struct LocText L_RESET_RESIDENT;
extern struct LocText L_LIST_KICK_MEM_TAG;
extern struct LocText L_KICKTAG_ENTRIES;
extern struct LocText L_UNKNOWN_STARS;
extern struct LocText L_CPU_INTERRUPT_VECTORS;
extern struct LocText L_VIEWING_MEMORY_AT1;
extern struct LocText L_UNKNOWN;
extern struct LocText L_PROGRAMS_FOUND_IN;
extern struct LocText L_KNOWN;
extern struct LocText L_LIST_KICKTAGPTR;
extern struct LocText L_LIST_KICKMEMPTR;
extern struct LocText L_RESTORE_VECS_CONTINUE;
extern struct LocText L_XVS_MODIFIED;
extern struct LocText L_XVS_DISABLED;
extern struct LocText L_EXEC_MODIFIED;

// VE_Window.c
extern struct LocText L_PROJECT;
extern struct LocText L_ABOUT;
extern struct LocText L_PREFS;
extern struct LocText L_MEMORY_CHECK;
extern struct LocText L_FILE_CHECK;
extern struct LocText L_SECTOR_CHECK;
extern struct LocText L_SELECT_SCREEN;
extern struct LocText L_QUIT;
extern struct LocText L_SHOW;
extern struct LocText L_BOOTBLOCK_VIRUS;
extern struct LocText L_LINK_VIRUS;
extern struct LocText L_FILE_VIRUS;
extern struct LocText L_DATA_VIRUS;
extern struct LocText L_TASKS;
extern struct LocText L_LIBRARIES;
extern struct LocText L_DEVICES;
extern struct LocText L_PATCHES;
extern struct LocText L_INSTALL;
extern struct LocText L_BOOTBLOCK_DATABASE;
extern struct LocText L_RECORD;
extern struct LocText L_VIEW;
extern struct LocText L_EDIT_NAME;
extern struct LocText L_MERGE;
extern struct LocText L_DELETE_ONE;
extern struct LocText L_DELETE_ALL;
extern struct LocText L_SAVE;
extern struct LocText L_UTILITIES;
extern struct LocText L_RENAME_PICTURES;
extern struct LocText L_CONVERT_IBN;
extern struct LocText L_SAVE_ROM_TO_FILE;
extern struct LocText L_COLDREBOOT;
extern struct LocText L_FILE_WATCH;
extern struct LocText L_CHECK_NOW;
extern struct LocText L_ADD_A_DIRECTORY;
extern struct LocText L_ADD_UPDATE_STARTUP;
extern struct LocText L_UPDATE_FILES;
extern struct LocText L_REMOVE_FILES;
extern struct LocText L_ICONIFY;
extern struct LocText L_FILE_VIRUS_MENU;
extern struct LocText L_CHECK_VALID_VOLUMES;

//VE_Analyze.c
extern struct LocText L_SEEMS_TO_KICKWORK;
extern struct LocText L_UNKNOWN_BOOTBLOCK;
extern struct LocText L_ANALYZE_PERCENT_VIRUS;
extern struct LocText L_WRITES_TO_BOOTBLOCK1;
extern struct LocText L_WRITES_TO_UNKNOWN;
extern struct LocText L_DESTROYS_THE_DISK;
extern struct LocText L_COULD_BE_FILEVIRUS;
extern struct LocText L_UNKNOWN_BB;
extern struct LocText L_A_VIRUSKILLER;
extern struct LocText L_A_LOADER;
extern struct LocText L_A_VIRUSCHECKER;
extern struct LocText L_AN_UTILITY_INTRO;
extern struct LocText L_CRYPTED_VIRUS;
extern struct LocText L_HARMLESS_BB;

// VE_FileStat.c
extern struct LocText L_DIRECTORIES;
extern struct LocText L_CRUNCHED;
extern struct LocText L_ARCHIVED;
extern struct LocText L_VIRUS_FOUND;
extern struct LocText L_FILES_1;
extern struct LocText L_DECRUNCHED;
extern struct LocText L_EXTRACTED;
extern struct LocText L_TIME;
extern struct LocText L_CHECKED;

// VE_Install
extern struct LocText L_INSTALL_TO;
extern struct LocText L_ERROR_OPENING_WINDOW;
extern struct LocText L_FILE_SYSTEM;
extern struct LocText L_INT_MODUS;
extern struct LocText L_DS_MODUS;
extern struct LocText L_FROM_DATABASE;
extern struct LocText L_DISK_INSTALLED;
extern struct LocText L_WINSTALL;
extern struct LocText L_WCANCEL;

// VE_Patches
extern struct LocText L_LIBRARY_VECTORS;
extern struct LocText L_LOADING_PATCH_DATA;
extern struct LocText L_PATCH_ERROR;
extern struct LocText L_ERROR_LOADING;
extern struct LocText L_PATCH_REQUIERS;
extern struct LocText L_NON_STANDARD_VECTOR;

// PrefsWindow.c
extern struct LocText L_PREFS_WININFO;
extern struct LocText L_FILE_CHECKING;
extern struct LocText L_MISC;
extern struct LocText L_P1;
extern struct LocText L_P2;
extern struct LocText L_P3;
extern struct LocText L_P4;
extern struct LocText L_P5;
extern struct LocText L_P6;
extern struct LocText L_P7;
extern struct LocText L_P8;
extern struct LocText L_P9;
extern struct LocText L_P10;
extern struct LocText L_P11;
extern struct LocText L_P12;
extern struct LocText L_LOGGING;
extern struct LocText L_ICONIFY_AT_STARTUP;
extern struct LocText L_SKIP_KNOWN_DATA_FILES;
extern struct LocText L_EXTRACT_ARCHIVES;

// VE_record.c
extern struct LocText L_DATABASE_EMPTY;
extern struct LocText L_CHANGES_DATABASE;
extern struct LocText L_SAVE_CANCEL;
extern struct LocText L_DELETING_DATABASE;
extern struct LocText L_DELETE_DATABASE_ALL;
extern struct LocText L_DELETE_CANCEL;
extern struct LocText L_DELETE_DATABASE;
extern struct LocText L_ARE_YOU_SURE;
extern struct LocText L_DATABASE_DELETED;
extern struct LocText L_ERROR_DELETE_DATABASE;
extern struct LocText L_NO_CHANGES_DATABASE;
extern struct LocText L_SAVING_DATABASE;
extern struct LocText L_SAVING_DATABASE_1;
extern struct LocText L_DATABASE_SAVED;
extern struct LocText L_ERROR_SAVING_DATABASE;
extern struct LocText L_LOADING_DATABASE;
extern struct LocText L_LOADING_DATABASE_1;
extern struct LocText L_NO_MEM_DATABASE;
extern struct LocText L_NOT_A_DATABASE;
extern struct LocText L_ERROR_LOADING_DATABASE;
extern struct LocText L_MERGE_DATABASE;
extern struct LocText L_ERROR_MERGING;
extern struct LocText L_NO_MEM_MERGE_DATABASE;
extern struct LocText L_MERGING;
extern struct LocText L_MERGE_END;
extern struct LocText L_ERROR_MERGING_DATABASE;
extern struct LocText L_RECORDED_VIEW;
extern struct LocText L_FREE_VIEW;
extern struct LocText L_SELECT_BB_INSTALL;
extern struct LocText L_CHANGE_NAME;
extern struct LocText L_CHANGE_TO;
extern struct LocText L_CHANGE_MIND;
extern struct LocText L_ALLREADY_EXISTS;
extern struct LocText L_CHANGED_OK;
extern struct LocText L_VIEW_BB;
extern struct LocText L_SELECT_BB_DELETE;
extern struct LocText L_DELETE_ONE_DATABASE;
extern struct LocText L_IS_DELETED;
extern struct LocText L_ERROR_DELETING_BB;
extern struct LocText L_NO_NEED_RECORD;
extern struct LocText L_NO_RECORD_VIRUS;
extern struct LocText L_GIVE_BB_NAME;
extern struct LocText L_NO_MEM_RECORD_BB;
extern struct LocText L_DATABASE;
extern struct LocText L_NO_DISK_ANY_DRIVE;
extern struct LocText L_SELECT_BB_RECORD;
extern struct LocText L_RECORD_A_BB;
extern struct LocText L_MERGING_DATABASE;

// VE_Sector.c
extern struct LocText L_DISK_IN_DRIVE_PROTECTED;
extern struct LocText L_DISK_NO_VALIDATED;
extern struct LocText L_PLEASE_WAIT_SECTOR_CHECK;
extern struct LocText L_CHECKING_CYLINDER;
extern struct LocText L_SECTOR_CHECKING_PAUSED;
extern struct LocText L_CONTINUE_CANCEL;
extern struct LocText L_SECTOR_DESTROYED;
extern struct LocText L_BLOCK_INFECTED;
extern struct LocText L_REPAIR_SECTOR_CONTINUE;
extern struct LocText L_ERROR_REPAIRING;
extern struct LocText L_SECTORS_CHECKED;
extern struct LocText L_SECTOR_CHECK_END;
extern struct LocText L_SELECT_DISK_SECTOR; 

// VE_ShowStuff.c
extern struct LocText L_SHOWING_TASKS;
extern struct LocText L_PROCESS;
extern struct LocText L_TASK;
extern struct LocText L_WARNING_NO_NAME;
extern struct LocText L_SHOWING_TASKS_1;
extern struct LocText L_NAME_ADDRESS_PRI_TYPE;
extern struct LocText L_SHOWING_LIBRARIES;
extern struct LocText L_SHOWING_LIBRARIES_1;
extern struct LocText L_NAME_ADDRESS_PRI_VER;
extern struct LocText L_SHOWING_DEVICES;
extern struct LocText L_SHOWING_DEVICES_1;
extern struct LocText L_SHOWING_PATCHES;
extern struct LocText L_SHOWING_PATCHES_1;
extern struct LocText L_NAME;
extern struct LocText L_BOOTBLOCK_C;
extern struct LocText L_LINK_C;
extern struct LocText L_FILE_C;
extern struct LocText L_VIRUS_LIST;
extern struct LocText L_DATA_C;

// VE_Utilities
extern struct LocText L_SELECT_FILE_TO_FIX;
extern struct LocText L_FIX_IT;
extern struct LocText L_LOAD_TEXT;
extern struct LocText L_LOADING;
extern struct LocText L_CONVERTING;
extern struct LocText L_SAVING;
extern struct LocText L_FILE_IS_NOW_SMALLER;
extern struct LocText L_ERROR_SAVING_FILE;
extern struct LocText L_NOT_AN_IBN;
extern struct LocText L_NOT_AN_ASCII;
extern struct LocText L_ERROR_LOADING_FILE;
extern struct LocText L_SELECT_FILE;
extern struct LocText L_SAVE_ROM;
extern struct LocText L_SAVING_ROM_TO;

// VE_Speed.c
extern struct LocText L_CALCULATING_CPU_SPEED;

// VE_Watch.c
extern struct LocText L_ADDING_TO_FILE_WATCH;
extern struct LocText L_NO_FILES_TO_CHECK;
extern struct LocText L_FW_CHECKING;
extern struct LocText L_FILE_DOESNT_EXIST;
extern struct LocText L_REMOVE_FROM_LIST_CONT;
extern struct LocText L_FILE_OLD_NEW_DIFF;
extern struct LocText L_FILE_SIZE_CHANGED;
extern struct LocText L_ACCEPT_CHANGE_CONTINUE;
extern struct LocText L_FILE_CRC_OLD_NEW;
extern struct LocText L_CRC_CHANGED;
extern struct LocText L_EXE;
extern struct LocText L_ICON;
extern struct LocText L_FW_ERROR;
extern struct LocText L_FILE_NAME_SIZE_TYPE;
extern struct LocText L_ADD_FILE_CANCEL_SKIP;
extern struct LocText L_SAVE_CONTINUE;
extern struct LocText L_ADDED_FILES_TO_WATCH;
extern struct LocText L_SELECT_A_DIRECTORY;
extern struct LocText L_SAVING_WATCH;
extern struct LocText L_LOADING_WATCH;
extern struct LocText L_FILE_WATCH_CHANGED;
extern struct LocText L_REMOVE_FROM_LIST;
extern struct LocText L_REMOVE_CANCEL;
extern struct LocText L_SELECT_FILE_TO_REMOVE;
extern struct LocText L_PLEASE_ADD_FIRST;
extern struct LocText L_WATCH_CHECK_PAUSED;

// FW_DirWindow.c
extern struct LocText L_INCLUDE;
extern struct LocText L_ICONS;
extern struct LocText L_CONFIRM;
extern struct LocText L_ADD;
extern struct LocText L_DONE;
extern struct LocText L_ADD_FILES_FROM;

// VE_Update.c
extern struct LocText L_SOCKET_ERROR;
extern struct LocText L_CONNECTION_REFUSED;
extern struct LocText L_UNKNOWN_HOST;
extern struct LocText L_IS_AVAILABLE;
extern struct LocText L_DOWNLOAD_F_CANCEL;
extern struct LocText L_CONNECTING_TO_AMINET;
extern struct LocText L_CHECKING_FOR_UPDATES;
extern struct LocText L_ERROR_SENDING;
extern struct LocText L_README_NOT_FOUND;
extern struct LocText L_ERROR_RECIVING_README;
extern struct LocText L_RECIVING;
extern struct LocText L_DOWNLOAD_COMPLETED;
extern struct LocText L_PLEASE_SELECT;
extern struct LocText L_USA;
extern struct LocText L_NO_TCP;
extern struct LocText L_UP_NAME;
extern struct LocText L_UP_VERSION;
extern struct LocText L_UP_AMINET_VERSION;
extern struct LocText L_UPDATE_CHECK_FINISHED;
extern struct LocText L_UP_RECIVED;
extern struct LocText L_UP_UPDATE;
extern struct LocText L_UP_UPDATES;
extern struct LocText L_VHT;
extern struct LocText L_CHECKING_FOR_UPDATES_1;
extern struct LocText L_CONNECTING_TO_SERVER;		
extern struct LocText L_ERROR_CONNECTING_TO_SERVER;	
extern struct LocText L_ERROR_TALKING_TO_SERVER;			
extern struct LocText L_CHECKING_VERSION;						
extern struct LocText L_ERROR_READING_INFO_DATA;			
extern struct LocText L_INFO_DATA_IS_CURRUPT;				
extern struct LocText L_NEW_VERSION_FOUND;						
extern struct LocText L_NO_NEW_VERSION;							
extern struct LocText L_DOWNLOADING_PST;							
extern struct LocText L_DOWNLOADING_CPS;							
extern struct LocText L_DOWNLOAD_COMPLETE;						
extern struct LocText L_CHECKSUM_ERROR;							
extern struct LocText L_PLEASE_INSTALL;							
extern struct LocText L_OLD;													
extern struct LocText L_NEW;													
extern struct LocText L_STATUS;											
extern struct LocText L_COULDNT_FIND_FILE_ON_SERVER;	
extern struct LocText L_FOUND_UPDATES;								
extern struct LocText L_INSTALL_CANCEL;							
extern struct LocText L_YOU_HAVE_A_SPECIAL;					
extern struct LocText L_INSTALLING_1;								
extern struct LocText L_FINISHED;										
extern struct LocText L_ABORTED;											
extern struct LocText L_README_FILES_ARE_LOCATED;		
extern struct LocText L_INSTALLING_2;								
extern struct LocText L_NO_NEW_MD5;
extern struct LocText L_YOUR_FILES_ARE_UP_TO_DATE;

// VE_Monitor.c
extern struct LocText L_MEMORY_AREA;
extern struct LocText L_MEMORY_MONITOR;
extern struct LocText L_NA;
extern struct LocText L_VE_NEEDS_DIS_LIB;

// VE_Watch.c
extern struct LocText L_FILE_MD5_OLD_NEW;
extern struct LocText L_MD5_CHANGED;
extern struct LocText L_THIS_VERSION_REQUIRES;

// VE_Prefs.c
extern struct LocText L_SELECT_LOG_PATH;

// VE_Prefs.c
extern struct LocText L_START;
extern struct LocText L_SELECT_VOLUMES_TO_CHECK;
extern struct LocText L_THIS_OPTION_WILL_DISABLE;

