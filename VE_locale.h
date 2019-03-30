#include <exec/types.h>

struct LocText {
  ULONG id;
  char *text;
};

#ifdef LOCALE_TEXT

#define LOCALE_START

/*
	CATALOG	 VirusExecutor
	VERSION  1.84
	CATDATE  29-01-00
*/

// Global.c
struct LocText L_NO_DISK		= { 0, "No disk in this drive." };
struct LocText L_MESSAGE 		= { 1, "Message"};
struct LocText L_OK			 	= { 2, "Ok"};
struct LocText L_READY			= { 3, "Ready."};
struct LocText L_PLEASE_WAIT	= { 4, "Please wait..."};

// VE.c
struct LocText L_UNKNOWN_TRACKDISK0	= {  5, "Trackdisk error %ld: %s"};
struct LocText L_UNKNOWN_TRACKDISK1	= {  6, "Unknown trackdisk error ->%ld<-\nunit = %ld\nPlease contact author!"};
struct LocText L_UNKNOWN_TRACKDISK2	= {  7, "Unknown trackdisk error %ld!"};
struct LocText L_PROF_FILE			= {  8, "A ProfFileSystem installed disk."};
struct LocText L_FLASHBACK			= {  9, "*"};
struct LocText L_ABACKUP			= { 10, "*"};
struct LocText L_ERROR_IN_CHECKSUM	= { 11, "Error in checksum!"};
struct LocText L_UNINST				= { 12, "Uninstalled AmigaDOS disk."};
struct LocText L_INSTALLED_DOS		= { 13, "Installed AmigaDOS disk"};
struct LocText L_FMS				= { 14, "Installed FMS disk v%s."};
struct LocText L_CHECKING_BOOTBLOCK	= { 15, "Checking bootblock..."};
struct LocText L_BAD_FORMAT_DISK	= { 16, "A badly formated disk?"};
struct LocText L_VIRUS_1			= { 17, "%s virus!"};
struct LocText L_VIRUS_FOUND_IN		= { 18, "%s virus found in %s"};
struct LocText L_KILL_VIRUS_CANCEL	= { 19, "Kill virus|Cancel"};
struct LocText L_WARNING			= { 20, "WARNING!"};
struct LocText L_UTILITY_BOOT		= { 21, "An utility boot."};
struct LocText L_VIRUS_KILLER		= { 22, "A virus killer boot."};
struct LocText L_INTRO_BOOT			= { 23, "An intro boot."};
struct LocText L_LOADER				= { 24, "A loader."};
struct LocText L_ANALYZING			= { 25, "Analyzing..."};
struct LocText L_NOT_A_BOOT_DISK	= { 26, "Not a bootable disk."};
struct LocText L_RETCODE_NAME		= { 27, "retcode = %ld Name = %s"};
struct LocText L_INSTALLING_TO		= { 28, "Installing '%s' to %s"};
struct LocText L_INSTALLING			= { 29, "Installing %s"};
struct LocText L_YOU_HAVE_THE_OLD	= { 30, "\nYou have the old %s v%s\nPlease get %s v%s or later!\n"};
struct LocText L_USING				= { 31, "Using external libraries:"};
struct LocText L_KNOWS_BOOT			= { 32, "*"};
struct LocText L_KNOWS_LINK			= { 33, "*"};
struct LocText L_KNOWS_FILE			= { 34, "*"};
struct LocText L_KNOWS_DECRUNCHERS	= { 35, "Knows %lD decrunchers types.\n\n"};
struct LocText L_KNOWS_PATCHES		= { 36, "Knows %lD patch entries from %lD programs.\n\n"};
struct LocText L_I_HAVE_CHECKED		= { 37, "*"};
struct LocText L_BOOTBLOCK			= { 38, "bootblock"};
struct LocText L_BOOTBLOCKS			= { 39, "bootblocks"};
struct LocText L_FILE				= { 40, "file"};
struct LocText L_FILES				= { 41, "files"};
struct LocText L_ABOUT				= { 42, "About..."};
struct LocText L_LOADING_PATCH_DATA	= { 43, "Loading and decrypting patch data..."};
struct LocText L_PATCH_ERROR		= { 44, "Error: '%s' is not a valid patch file!"};
struct LocText L_ERROR_LOADING		= { 45, "Error loading '%s'!"};
struct LocText L_BB_VIRUS			= { 46, "BB virus"};
struct LocText L_FILE_VIRUS			= { 47, "File virus"};
struct LocText L_NA					= { 48, "N/A"};
struct LocText L_NO_MEM				= { 49, "Not enough memory!"};
struct LocText L_SAVING_PREFERENCES	= { 50, "Saving preferences..."};
struct LocText L_BOOTBLOCK_C		= { 51, "Bootblock"};
struct LocText L_LINK_C				= { 52, "Link"};
struct LocText L_FILE_C				= { 53, "File"};
struct LocText L_VIRUS_LIST			= { 54, "%s virus (%lD)"};
struct LocText L_SELECT_SCREEN_MODE	= { 55, "Select a screen mode:"};
struct LocText L_SMALL_SCREEN_SIZE	= { 56, "VirusExecutor can't handle this small screen size!"};
struct LocText L_SHIT				= { 57, "*"};
struct LocText L_SCREEN_RESTART_MSG	= { 58, "*"};
struct LocText L_NO_PICK_SCREEN		= { 59, "You didn't pick a screen mode."};
struct LocText L_VE_REQUIERS		= { 60, "*"};
struct LocText L_ERROR				= { 61, "Error!"};
struct LocText L_VE_RUNNING			= { 62, "*"};
struct LocText L_VE_LIB_NEED		= { 63, "VirusExecutor needs '%s' v%ld+!"};
struct LocText L_XVS_MODIFIED		= { 64, "Error: xvs.library has been modified!"};
struct LocText L_CHECKING_DRIVES	= { 65, "Checking disk drives..."};

// VE_Files.c
 struct LocText L_ENTER_PW				= { 66, "Please enter password:"};
 struct LocText L_FILE_NAME_ERROR		= { 67, "File name: %s\n\n    Error: %s"};
 struct LocText L_INFECTED_DELETE		= { 68, "VIRUS: %s\n\nInfected file:\n'%s'\n\nThis type of virus can only be deleted."};
 struct LocText L_VIRUS_WARNING			= { 69, "%s VIRUS WARNING!"};
 struct LocText L_DELETE_FILE_AUTO		= { 70, "Delete file|Auto delete file virus|Stop checking|Continue"};
 struct LocText L_FILE_NOT_DELETED		= { 71, "File '%s' could not be deleted!"};
 struct LocText L_DATA					= { 72, "DATA"};
 struct LocText L_VIRUS_INFECTED_ARCHIVE= { 73, "        VIRUS: %s\n\nInfected file: %s\n      Archive: %s"};
 struct LocText L_FILE_VIRUS_WARNING	= { 74, "FILE VIRUS WARNING!"};
 struct LocText L_FILE1					= { 75, "FILE"};
 struct LocText L_LINK_VIRUS_WARNING	= { 76, "LINK VIRUS WARNING!"};
 struct LocText L_VIRUS_INFECTED_FILE	= { 77, "VIRUS: %s\n\nInfected file:\n'%s'"};
 struct LocText L_REMOVE_VIRUS_AUTO		= { 78, "Remove virus|Auto remove link virus|Stop checking|Continue"};
 struct LocText L_VIRUS_REMOVED			= { 79, "*"};
 struct LocText L_SAVE_NEW_FILE			= { 80, "Save new file|Cancel"};
 struct LocText L_ERROR_WRITING_FILE	= { 81, "Error writing file: '%s'!"};
 struct LocText L_ERROR_OPENING_FILE	= { 82, "Error opening file: '%s'!"};
 struct LocText L_ERROR_VIRUS_NOT_REMOVED= { 83, "Error: %s\n\nVirus could'n be removed!"};
 struct LocText L_ERROR_REMOVING_VIRUS	= { 84, "ERROR REMOVING VIRUS FROM FILE"};
 struct LocText L_DELETE_FILE_STOP		= { 85, "Delete file|Stop checking|Continue"};
 struct LocText L_UNLINKING_FILE		= { 86, "Unlinking %s (%s)..."};
 struct LocText L_ERROR_UNLINKING		= { 87, "Error unlinking file: '%s'!"};
 struct LocText L_EXTRACTING			= { 88, "*"};
 struct LocText L_CHECKING				= { 89, "Checking... '%s'"};
 struct LocText L_ERROR_READING_FILE	= { 90, "Error reading file '%s'!"};
 struct LocText L_FILE_CHECK_PAUSED		= { 91, "File checking paused"};
 struct LocText L_CONTINUE_STOP			= { 92, "Continue|Stop checking"};
 struct LocText L_USER_BREAK			= { 93, "User break!"};
 struct LocText L_ERROR65				= { 94, "ERROR: 65\nContact author!"};
 struct LocText L_ERROR_LOCKING_DIR		= { 95, "ERROR locking dir '%s'!"};
 struct LocText L_CHECK_FILES_VIRUS		= { 96, "Check files for virus."};
 struct LocText L_ERROR46				= { 97, "ERROR: 46! Contact author...\n"};
 struct LocText L_FILE_SCAN_END			= { 98, "*"};
 struct LocText L_DIR					= { 99, "dir"};
 struct LocText L_DIRS					= { 100, "dirs"};
 struct LocText L_ARCHIVE				= { 101, "archive"};
 struct LocText L_ARCHIVES				= { 102, "archives"};
 struct LocText L_MINUTE				= { 103, "minute"};
 struct LocText L_MINUTES				= { 104, "minutes"};
 struct LocText L_SECOND				= { 105, "second"};
 struct LocText L_SECONDS				= { 106, "seconds"};
 struct LocText L_FOUND					= { 107, "\n\nFound %lD virus!"};
 struct LocText L_NO_KNOWN				= { 108, "no known"};
 struct LocText L_FILE_CHECK_STATUS		= { 109, "File check status."};

// VE_Mem.c
 struct LocText L_REBOOT_CANCEL			= { 110, "Reboot|Cancel"};
 struct LocText L_NO_ENTRIES_FOUND		= { 111, "No entries found."};
 struct LocText L_ADDRESS_NAME			= { 112, "*"};
 struct LocText L_LOOK_FOR_TEXT			= { 113, "Look for text: VIRUS etc"};
 struct LocText L_IT_MIGHT_MEMVIRUS		= { 114, "\nIt might be a virus in memory!"};
 struct LocText L_VIEW_MEMORY			= { 115, "LOCALE STRING IS NOT USED!"};
 struct LocText L_ERROR_CLR_VECTORS		= { 116, "Error clearing vectors!!\nIt could be a virus in memory!"};
 struct LocText L_MEM_VEC_OK			= { 117, "\nMemory vectors seems ok."};
 struct LocText L_VECTOR_CHECK			= { 118, "Vector check"};
 struct LocText L_VIRUS_DETECTED_IN_MEM = { 119, "%s virus detected in memory!\n"};
 struct LocText L_VIRUS_REMOVED_MEM		= { 120, "\nVirus removed from memory."};
 struct LocText L_NO_KNOWN_VIRUS_MEM	= { 121, "No known virus detected in memory."};
 struct LocText L_GREAT					= { 122, "Great!"};
 struct LocText L_MEMORY_CHECK			= { 123, "Memory check"};

// VE_ShowWindow.c
 struct LocText L_EXIT					= { 124, "Done"};

// VE_window.c
 struct LocText L_PROJECT				= { 125, "Project"};
 struct LocText L_PREFS					= { 126, "Prefs..."};
 struct LocText L_FILE_CHECK			= { 127, "File check"};
 struct LocText L_SECTOR_CHECK			= { 128, "Sector check"};
 struct LocText L_SELECT_SCREEN			= { 129, "Select screen mode..."};
 struct LocText L_QUIT					= { 130, "Quit"};
 struct LocText L_SHOW					= { 131, "Show"};
 struct LocText L_BOOTBLOCK_VIRUS		= { 132, "Bootblock virus"};
 struct LocText L_LINK_VIRUS			= { 133, "Link virus"};
 struct LocText L_TASKS					= { 134, "Tasks"};
 struct LocText L_LIBRARIES				= { 135, "Libraries"};
 struct LocText L_DEVICES				= { 136, "Devices"};
 struct LocText L_PATCHES				= { 137, "Patches"};
 struct LocText L_INSTALL				= { 138, "Install"};
 struct LocText L_BOOTBLOCK_DATABASE	= { 139, "Bootblock database"};
 struct LocText L_RECORD				= { 140, "Record"};
 struct LocText L_VIEW					= { 141, "View"};
 struct LocText L_EDIT_NAME				= { 142, "Edit name"};
 struct LocText L_MERGE					= { 143, "Merge"};
 struct LocText L_DELETE_ONE			= { 144, "Delete one"};
 struct LocText L_DELETE_ALL			= { 145, "Delete all"};
 struct LocText L_SAVE					= { 146, "Save"};
 struct LocText L_UTILITIES				= { 147, "Utilities"};
 struct LocText L_RENAME_PICTURES		= { 148, "Rename pictures"};
 struct LocText L_CONVERT_IBN			= { 149, "Convert IBN/IBM/IBMM to ISO"};
 struct LocText L_SAVE_ROM_TO_FILE		= { 150, "Save ROM to file"};
 struct LocText L_TRANSLATED_BY			= { 151, "Translated to LANGUAGE by PUT YOUR NAME HERE"};


// VE_Analyze.c
 struct LocText L_SEEMS_TO_KICKWORK		= { 152,  "\n\nSeems to work only with kickstart v%s!"};
 struct LocText L_UNKNOWN_BOOTBLOCK		= { 153, "Unknown bootblock! %ld%% chance for a virus!"};
 struct LocText L_ANALYZE_PERCENT_VIRUS	= { 154, "%s analyze: %ld percent chance for a virus here!"};
 struct LocText L_WRITES_TO_BOOTBLOCK	= { 155, "*"};
 struct LocText L_WRITES_TO_UNKNOWN		= { 156, "\n\nWrites to unknown place on disk!"};
 struct LocText L_DESTROYS_THE_DISK		= { 157, "\n\nDestroys the disk!!"};
 struct LocText L_COULD_BE_FILEVIRUS	= { 158, "\n\nThis could also be a file virus!"};
 struct LocText L_UNKNOWN_BB			= { 159, "Unknown bootblock! It might be %s."};
 struct LocText L_A_VIRUSKILLER			= { 160, "a viruskiller"};
 struct LocText L_A_LOADER				= { 161, "a loader"};
 struct LocText L_A_VIRUSCHECKER		= { 162, "a virus checker"};
 struct LocText L_AN_UTILITY_INTRO		= { 163, "an utility or an intro"};
 struct LocText L_CRYPTED_VIRUS			= { 164, "crypted! Could be a virus!"};
 struct LocText L_HARMLESS_BB			= { 165, "Unknown bootblock! (Seems harmless)"};

// VE_FileStat.c
 struct LocText L_AVAIL_CHIP_FAST		= { 166, "*"};
 struct LocText L_LARGEST_CHIP_FAST		= { 167, "*"};
 struct LocText L_DIRECTORIES_STAT		= { 168, "*"};
 struct LocText L_CRUNCHED_DECRUNCHED	= { 169, "*"};
 struct LocText L_ARCHIVED_EXTRACTED	= { 170, "*"};
 struct LocText L_VIRUS_FOUND_TIME		= { 171, "*"};

// VE_Install
 struct LocText L_INSTALL_TO			= { 172, "Install to %s"};
 struct LocText L_ERROR_OPENING_WINDOW	= { 173, "Error opening window!"};
 struct LocText L_FILE_SYSTEM			= { 174, "File system:"};
 struct LocText L_INT_MODUS				= { 175, "International modus:"};
 struct LocText L_DS_MODUS				= { 176, "Directory cache:"};
 struct LocText L_FROM_DATABASE			= { 177, "From database:"};
 struct LocText L_DISK_INSTALLED		= { 178, "Disk installed."};
 struct LocText L_WINSTALL				= { 179, " Install"};
 struct LocText L_WCANCEL				= { 180, "Cancel"};

// VE_Mem.c
 struct LocText L_EXEC_INTERRUPT		= { 181, "Exec interrupt vectors in RAM"};
 struct LocText L_KICKMEM_ENTRIES		= { 182, "KickMemPtr entries:"};
 struct LocText L_KICKSTART_AT			= { 183, "Kickstart v%s at $%08lx\n\n"};

// VE_Pack_files.c
 struct LocText L_ERRORP500				= { 184, "*"};

// VE_Patches
 struct LocText L_LIBRARY_VECTORS		= { 185, "Library Vectors in RAM"};

// PrefsWindow.c
 struct LocText L_PREFS_WININFO			= { 186, "Prefs (BETA - Press 'Esc' to return)"};
 struct LocText L_FILE_CHECKING			= { 187, "File checking:"};
 struct LocText L_MISC					= { 188, "Misc:"};
 struct LocText L_P1					= { 189, "Decrunch crunched files"};
 struct LocText L_P2					= { 190, "Check data files"};
 struct LocText L_P3					= { 191, "Extract LHA archives"};
 struct LocText L_P4					= { 192, "Extract LZX archives"};
 struct LocText L_P5					= { 193, "Extract ZIP archives"};
 struct LocText L_P6					= { 194, "Skip encrypted files"};
 struct LocText L_P7					= { 195, "*"};

// VE_record.c
 struct LocText L_DATABASE_EMPTY		= { 196, "Database is empty!"};
 struct LocText L_CHANGES_DATABASE		= { 197, "Changes has been made to the database!\nSave database?"};
 struct LocText L_SAVE_CANCEL			= { 198, "Save|Cancel"};
 struct LocText L_DELETING_DATABASE		= { 199, "Deleting database..."};
 struct LocText L_DELETE_DATABASE_ALL	= { 200, "Delete database?\nYou have %lD bootblocks recorded.\n\nThe database will also be deleted from your drive!"};
 struct LocText L_DELETE_CANCEL			= { 201, "Delete|Cancel"};
 struct LocText L_DELETE_DATABASE		= { 202, "Delete database..."};
 struct LocText L_ARE_YOU_SURE			= { 203, "Are you sure???"};
 struct LocText L_DATABASE_DELETED		= { 204, "Database is deleted."};
 struct LocText L_ERROR_DELETE_DATABASE	= { 205, "Error delete database from disk!"};
 struct LocText L_NO_CHANGES_DATABASE	= { 206, "No changes in the database!"};
 struct LocText L_SAVING_DATABASE		= { 207, "Saving database..."};
 struct LocText L_SAVING_DATABASE_1		= { 208, "Saving database... %lD"};
 struct LocText L_DATABASE_SAVED		= { 209, "The database is saved."};
 struct LocText L_ERROR_SAVING_DATABASE	= { 210, "Error saving database!"};
 struct LocText L_LOADING_DATABASE		= { 211, "Loading database..."};
 struct LocText L_LOADING_DATABASE_1	= { 212, "Loading database... %lD"};
 struct LocText L_NO_MEM_DATABASE		= { 213, "Not enough memory to load the entire database!"};
 struct LocText L_NOT_A_DATABASE		= { 214, "'%s' is not a VirusExecutor database!"};
 struct LocText L_ERROR_LOADING_DATABASE= { 215, "Error loading database!"};
 struct LocText L_MERGE_DATABASE		= { 216, "Merge database..."};
 struct LocText L_ERROR_MERGING			= { 217, "Error merging from '%s'"};
 struct LocText L_NO_MEM_MERGE_DATABASE	= { 218, "Not enough memory to merge the entire database!"};
 struct LocText L_MERGING		    	= { 219, "Merging %3lD..."};
 struct LocText L_MERGE_END				= { 220, "Merged %lD %s of %lD."};
 struct LocText L_ERROR_MERGING_DATABASE= { 221, "Error merging database!"};
 struct LocText L_RECORDED_VIEW			= { 222, "Recorded: %10lD"};
 struct LocText L_FREE_VIEW				= { 223, "Free:     %10lD"};
 struct LocText L_SELECT_BB_INSTALL		= { 224, "Select bootblock to install..."};
 struct LocText L_CHANGE_NAME			= { 225, "Change '%s' to:"};
 struct LocText L_CHANGE_TO				= { 226, "Change to:"};
 struct LocText L_CHANGE_MIND			= { 227, "Did you change your mind?"};
 struct LocText L_ALLREADY_EXISTS		= { 228, "'%s' allready exists!"};
 struct LocText L_CHANGED_OK			= { 229, "Changed '%s' to '%s'."};
 struct LocText L_VIEW_BB				= { 230, "View bootblock..."};
 struct LocText L_SELECT_BB_DELETE		= { 231, "Select bootblock to delete..."};
 struct LocText L_DELETE_ONE_DATABASE	= { 232, "\nDelete '%s' from the database?\n"};
 struct LocText L_IS_DELETED			= { 233, "'%s' is deleted."};
 struct LocText L_ERROR_DELETING_BB		= { 234, "Error deleting '%s'!"};
 struct LocText L_NO_NEED_RECORD		= { 235, "You don't need to record this bootblock!"};
 struct LocText L_NO_RECORD_VIRUS		= { 236, "I wont let you record this virus!"};
 struct LocText L_GIVE_BB_NAME			= { 237, "Give the bootblock from %s a name..."};
 struct LocText L_NO_MEM_RECORD_BB		= { 238, "Not enough memory to record this bootblock!"};
 struct LocText L_DATABASE				= { 239, "Database..."};
 struct LocText L_NO_DISK_ANY_DRIVE		= { 240, "No disk in any drive!"};
 struct LocText L_SELECT_BB_RECORD		= { 241, "Select bootblock\nto record."};
 struct LocText L_RECORD_A_BB			= { 242, "Record a bootblock."};
 struct LocText L_MERGING_DATABASE		= { 243, "Merging database..."};

// VE_Sector.c
 struct LocText L_DISK_IN_DRIVE_PROTECTED	= { 244, "Disk in drive %s is write protected!"};
 struct LocText L_DISK_NO_VALIDATED			= { 245, "Disk in drive %s is not validated!"};
 struct LocText L_PLEASE_WAIT_SECTOR_CHECK	= { 246, "Please wait, sector checking in progress..."};
 struct LocText L_CHECKING_CYLINDER			= { 247, "Checking cylinder: %ld - %ld to go..."};
 struct LocText L_SECTOR_CHECKING_PAUSED	= { 248, "Sector checking paused"};
 struct LocText L_CONTINUE_CANCEL			= { 249, "Continue|Cancel"};
 struct LocText L_SECTOR_DESTROYED			= { 250, "Sector %lD is destroyed with: %s"};
 struct LocText L_BLOCK_INFECTED			= { 251, "Block %ld is infected with: %s"};
 struct LocText L_REPAIR_SECTOR_CONTINUE	= { 252, "Repair sector|Continue"};
 struct LocText L_ERROR_REPAIRING			= { 253, "Error repairing sector!"};
 struct LocText L_SECTORS_CHECKED			= { 254, "%lD sectors checked, %lD infections found."};
 struct LocText L_SECTOR_CHECK_END			= { 255, "Sector check finished."};

// VE_ShowStuff.c
 struct LocText L_SHOWING_TASKS			= { 256, "Showing tasks..."};
 struct LocText L_PROCESS				= { 257, "Process "};
 struct LocText L_TASK					= { 258, "Task    "};
 struct LocText L_WARNING_NO_NAME		= { 259, "*** WARNING - NO NAME! ***"};
 struct LocText L_SHOWING_TASKS_1		= { 260, "Showing tasks (%lD)..."};
 struct LocText L_NAME_ADDRESS_PRI_TYPE = { 261, "Name                                                Address   Pri  Type"};
 struct LocText L_SHOWING_LIBRARIES		= { 262, "Showing libraries..."};
 struct LocText L_SHOWING_LIBRARIES_1	= { 263, "Showing libraries (%lD)..."};
 struct LocText L_NAME_ADDRESS_PRI_VER	= { 264, "Name                                              Address   Pri  Version"};
 struct LocText L_SHOWING_DEVICES		= { 265, "Showing devices..."};
 struct LocText L_SHOWING_DEVICES_1		= { 266, "Showing devices (%lD)..."};
 struct LocText L_SHOWING_PATCHES		= { 267, "Showing patches..."};
 struct LocText L_SHOWING_PATCHES_1		= { 268, "Showing patches (%lD)..."};
 struct LocText L_NAME					= { 269, "Name"};

// VE_Utilities
 struct LocText L_SELECT_FILE_TO_FIX	= { 270, "Select file to fix..."};
 struct LocText L_FIX_IT				= { 271, "Fix it"};
 struct LocText L_LOAD_TEXT				= { 272, "Load text"};
 struct LocText L_LOADING				= { 273, "Loading '%s'..."};
 struct LocText L_CONVERTING			= { 274, "Converting..."};
 struct LocText L_SAVING				= { 275, "Saving..."};
 struct LocText L_FILE_IS_NOW_SMALLER	= { 276, "File is now %lD chars smaller."};
 struct LocText L_ERROR_SAVING_FILE		= { 277, "Error saving file!"};
 struct LocText L_NOT_AN_IBN			= { 278, "Not an IBN/IBM/IBMM text file!"};
 struct LocText L_NOT_AN_ASCII			= { 279, "Not an ASCII file!"};
 struct LocText L_ERROR_LOADING_FILE	= { 280, "Error loading file!"};
 struct LocText L_SELECT_FILE			= { 281, "Select file..."};
 struct LocText L_SAVE_ROM				= { 282, "Save ROM"};
 struct LocText L_SAVING_ROM_TO			= { 283, "Saving ROM (%08lx - %08lx) to '%s'"};


// VE_Patches.c
 struct LocText L_PATCH_REQUIERS		= { 284, "\n\nThis version of VirusExecutor needs v%ld.%02ld+ of PatchBrain!"};

// VE.c
 struct LocText L_VE_MODIFIED			= { 285, "Error: VirusExecutor has been modified!"};

// VE_Mem.c
 struct LocText L_KICKTAG_ENTRIES		= { 286, "KickTagPtr entries:"};

// VE.c
 struct LocText L_VE_NEEDS_XFDMASTER 	= { 287, "%s needs %s to decrunch crunched files!\nPlease install the latest version of %s.\n\nThe 'Decrunch crunched files' option is now disabled..."};

// VE_Mem.c
 struct LocText L_UNKNOWN_STARS			= { 288, "*** UNKNOWN ***"};
 struct LocText L_CPU_INTERRUPT_VECTORS	= { 289, "CPU interrupt vectors in RAM (VBR: $%08lx)"};
 struct LocText L_VIEWING_MEMORY_AT		= { 290, "*"};
 struct LocText L_UNKNOWN				= { 291, "Unknown"};
 struct LocText L_PROGRAMS_FOUND_IN		= { 292, "program(s) found in"};
 struct LocText L_KNOWN					= { 293, "Known"};
 struct LocText L_LIST_KICKTAGPTR		= { 294, "List KickTagPtr|"};
 struct LocText L_LIST_KICKMEMPTR		= { 295, "List KickMemPtr|"};
 struct LocText L_RESTORE_VECS_CONTINUE	= { 296, "Restore vectors|Continue"};

// VE_Files.c
 struct LocText L_FOUND_A_CRUNCHED_FILE	= { 297, "Found a crunched file!\n\nAre you sure you want to overwrite ALL\ncrunched files with the decrunched buffer?"};
 struct LocText L_DO_IT_TURN_OFF		= { 298, "Do it!|Turn off the 'Overwrite'-flag"};

// PrefsWindow.c
 struct LocText L_P8					= { 299, "Overwrite crunched files"};

// VE_Speed.c
 struct LocText L_CALCULATING_CPU_SPEED	= { 300, "Calculating processor speed..."};

// VE_Sector.c
 struct LocText L_SELECT_DISK_SECTOR	= { 301, "Select a drive to process the sector checking."};

// VE_Mem.c
struct LocText L_I_HAVE_CHECKED_S		= { 302, "I have checked %s %s and %s %s so far."};

// VE_Window.c
struct LocText L_COLDREBOOT				= { 303, "ColdReboot..."};

// VE.c
 struct LocText L_REBOOT_THE_MACHINE	= { 304, "Reboot the machine?\n\nAll external memory and peripherals will be RESET,\nand the machine will start its power up diagnostics."};
 struct LocText L_CANCEL_REBOOT_CANCEL	= { 305, "Cancel|Reboot|Cancel"};

// VE_Fiels.c
 struct LocText L_DATA_VIRUS_WARNING	= { 306, "DATA VIRUS WARNING!"};


// PrefsWindow.c
 struct LocText L_P9					= { 307, "*"};
 struct LocText L_P10					= { 308, "Skip files older than"};
 struct LocText L_P11					= { 309, "day(s)"};

// VE_Patches.c
 struct LocText L_NON_STANDARD_VECTOR	= { 310, "*** NON-STANDARD VECTOR ***"};

// VE_Watch.c
 struct LocText L_ADDING_TO_FILE_WATCH	= { 311, "Adding '%s' to the integrity data base..."};
 struct LocText L_NO_FILES_TO_CHECK		= { 312, "No files to check!"};
 struct LocText L_FW_CHECKING			= { 313, "Checking '%s'..."};
 struct LocText L_FILE_DOESNT_EXIST		= { 314, "File doesn't exist!\n\n'%s'"};
 struct LocText L_REMOVE_FROM_LIST_CONT	= { 315, "Remove from list|Continue"};
 struct LocText L_FILE_OLD_NEW_DIFF		= { 316, "    File: '%s'\n\nOld size: %lD\nNew size: %lD\n\n    Diff: %lD"};
 struct LocText L_FILE_SIZE_CHANGED		= { 317, "FILE SIZE HAS CHANGED!"};
 struct LocText L_ACCEPT_CHANGE_CONTINUE= { 318, "Accept change|Continue"};
 struct LocText L_FILE_CRC_OLD_NEW		= { 319, "*"};
 struct LocText L_CRC_CHANGED			= { 320, "*"};
 struct LocText L_EXE					= { 321, "EXECUTABLE"};
 struct LocText L_ICON					= { 322, "ICON"};
 struct LocText L_FW_ERROR				= { 323, "ERROR???"};
 struct LocText L_FILE_NAME_SIZE_TYPE	= { 324, "File name: %s\n     Size: %lD\n\n     Type: %s"};
 struct LocText L_ADD_FILE_CANCEL_SKIP	= { 325, "Add file|Cancel|Skip file"};
 struct LocText L_SAVE_CONTINUE			= { 326, "Save|Continue"};
 struct LocText L_ADDED_FILES_TO_WATCH	= { 327, "Added %lD %s to the integrity data base"};
 struct LocText L_SELECT_A_DIRECTORY	= { 328, "Select a directory..."};
 struct LocText L_SAVING_WATCH			= { 329, "Saving 'File integrity' data..."};
 struct LocText L_LOADING_WATCH			= { 330, "Loading 'File integrity' data..."};

// FW_DirWindow.c
 struct LocText L_INCLUDE				= { 331, "Include data files"};
 struct LocText L_ICONS					= { 332, "Exclude icons"};
 struct LocText L_CONFIRM				= { 333, "Confirm every file"};
 struct LocText L_ADD					= { 334, "Add"};
 struct LocText L_DONE					= { 335, "Done"};

// VE_Window.c
 struct LocText L_FILE_WATCH			= { 336, "File integrity"};
 struct LocText L_CHECK_NOW				= { 337, "Check now!"};
 struct LocText L_ADD_A_DIRECTORY		= { 338, "Add a directory"};
 struct LocText L_ADD_UPDATE_STARTUP	= { 339, "Add/update startup files"};

// FW_DirWindow.c
 struct LocText L_ADD_FILES_FROM		= { 340, "Add files from a directory"};

// VE_FileStat.c
 struct LocText L_AVAIL_CHIP_FAST_1		= { 341, "*"};
 struct LocText L_DIRECTORIES_STAT_1	= { 342, "*"};

// VE_Window.c
 struct LocText L_UPDATE_FILES					= { 343, "Update files"};

// VE_Analyze.c
 struct LocText L_WRITES_TO_BOOTBLOCK1	= { 344, "\n\nWrites to bootblock! (%ld percent sure)"};

// VE_Update.c
 struct LocText L_SOCKET_ERROR					= { 345, "Socket error!"};
 struct LocText L_CONNECTION_REFUSED		= { 346, "Connection refused!"};
 struct LocText L_UNKNOWN_HOST					= { 347, "Unknown host!"};
 struct LocText L_IS_AVAILABLE					= { 348, "*"};
 struct LocText L_DOWNLOAD_F_CANCEL			= { 349, "*"};
 struct LocText L_CONNECTING_TO_AMINET	= { 350, "*"};
 struct LocText L_CHECKING_FOR_UPDATES	= { 351, "*"};
 struct LocText L_ERROR_SENDING					= { 352, "Error sending host request!"};
 struct LocText L_README_NOT_FOUND			= { 353, "*"};
 struct LocText L_ERROR_RECIVING_README	= { 354, "*"};
 struct LocText L_RECIVING							= { 355, "*"};
 struct LocText L_DOWNLOAD_COMPLETED		= { 356, "*"};
 struct LocText L_PLEASE_SELECT					= { 357, "*"};
 struct LocText L_USA										= { 358, "*"};
 struct LocText L_NO_TCP								= { 359, "No TCP/IP stack seems to be running!"};

// VE.c
 struct LocText L_FILE_WATCH_CHANGED		= { 360, "File integrity data has been changed!"};

// VE_Window.c
 struct LocText L_REMOVE_FILES					= { 361, "Remove files..."};

// VE_Watch.c
 struct LocText L_REMOVE_FROM_LIST		= { 362, "Remove '%s'?"};
 struct LocText L_REMOVE_CANCEL			= { 363, "Remove file|Cancel"};
 struct LocText L_SELECT_FILE_TO_REMOVE	= { 364, "Select file to remove from the file integrity list..."};
 struct LocText L_PLEASE_ADD_FIRST		= { 365, "Please add some files first!"};
 struct LocText L_WATCH_CHECK_PAUSED	= { 366, "File integrity checking paused"};
 struct LocText L_UP_NAME				= { 367, "Name"};
 struct LocText L_UP_VERSION			= { 368, "Version"};
 struct LocText L_UP_AMINET_VERSION		= { 369, "Aminet version"};
 struct LocText L_UPDATE_CHECK_FINISHED	= { 370, "Update check finished."};
 struct LocText L_UP_RECIVED			= { 371, "Recived %ld %s."};
 struct LocText L_UP_UPDATE				= { 372, "update"};
 struct LocText L_UP_UPDATES			= { 373, "updates"};

// VE.c
 struct LocText L_VE_NEEDS_XADMASTER 	= { 374, "%s needs %s v%ld+ to extract LhA+ archive!\nPlease install the latest version of %s.\n\nThe 'Extract # archives' option is now disabled..."};

// VE_Files.c
 struct LocText L_ARCHIVE_IS_CRYPTED	= { 375, "ARCHIVE IS CRYPTED!\n\n'%s'"};
 struct LocText L_ARCHIVE_IS_CORRUPT	= { 376, "ARCHIVE IS CORRUPT!\n\n'%s'"};
 struct LocText L_FILE_SCAN_END1		= { 377, "Scanned %lD %s and %lD %s\n\nChecked %lD exe %s (%lD crunched)\nChecked %lD data %s\nChecked %lD %s\n\nTime used: %s "};

// VE_FileStat.c
 struct LocText L_VIRUS_FOUND_TIME1		= { 378, "*"};

// VE_Files.c
 struct LocText L_ARCHIVE_ERROR			= { 379, "*"};
 struct LocText L_SAVING_FILE			= { 380, "Saving '%s'..."};

// PrefsWindow.c
 struct LocText L_P12					= { 381, "Fast file checking"};

// VE_Mem.c
 struct LocText L_VIEWING_MEMORY_AT1	= { 382, "*"};

// VE.c
 struct LocText L_KNOWS_DATA			= { 383, "*"};
 struct LocText L_DATA_C				= { 384, "Data"};

// VE_Windows.c
 struct LocText L_DATA_VIRUS			= { 385, "Data virus"};

// VE_Monitor.c
 struct LocText L_MEMORY_AREA			= { 386, "Memory area: $%08lx-$%08lx '%s'"};
 struct LocText L_MEMORY_MONITOR		= { 387, "Memory monitor"};

// PrefsWindow.c
 struct LocText L_LOGGING				= { 388, "Write to log file"};

// VE_Window.c
 struct LocText L_ICONIFY				= { 389, "Iconify"};

// PrefsWindow.c
 struct LocText L_ICONIFY_AT_STARTUP	= { 390, "Iconify at startup"};

// VE.c
 struct LocText L_COULDNT_CREATE_ICON	= { 391, "Couldn't create icon!"};
 struct LocText L_NO_WB					= { 392, "No Workbench screen available!"};

// VE_Mem.c
 struct LocText L_XVS_DISABLED			= { 393, "xvs.library has been disabled!"};
 struct LocText L_EXEC_MODIFIED			= { 394, "Error: exec.library has been modified!"};

// VE_Watch.c
 struct LocText L_FILE_MD5_OLD_NEW		= { 395, "   File: '%s'\n\nOld MD5: $%s\nNew MD5: $%s"};
 struct LocText L_MD5_CHANGED			= { 396, "MD5-128 CHECKSUM HAS CHANGED!"};
 struct LocText L_THIS_VERSION_REQUIRES	= { 397, "This version requires that you delete your old file integrity data!"};

// VE_Prefs.c
 struct LocText L_SELECT_LOG_PATH		= { 398, "Select log path..."};

// VE_Monitor.c
 struct LocText L_VE_NEEDS_DIS_LIB	 	= { 399, "%s needs %s to disassemble memory!"};

// VE_Window.c
 struct LocText L_FILE_VIRUS_MENU		= { 400, "File virus"};

// VE.c
  struct LocText L_KNOWS_VIRUS			= { 401, "Knows %3ld virus.\n\n"};

// VE_Window.c
  struct LocText L_CHECK_VALID_VOLUMES	= { 402, "Check ALL valid volumes"};

// PrefsWindow.c
  struct LocText L_SKIP_KNOWN_DATA_FILES= { 403, "*"};
  struct LocText L_EXTRACT_ARCHIVES		= { 404, "Extract archives"};

// VE_Files.c
  struct LocText L_DO_FILE_CHECK_VOL	= { 405, "Do file check on these volumes?"};
  struct LocText L_CONTINUE_SKIP_STOP	= { 406, "Continue|Skip this volume|Stop"};
  struct LocText L_VIRUS_REMOVED_DIFF	= { 407, "Virus removed!\n\nOld size: %lD\nNew size: %lD\n\n    Diff: %lD"};
  struct LocText L_HIDDEN_EXE_FOUND		= { 408, "Hidden executable file found in script!\n\n'%s'"};
  struct LocText L_DECODE_CONTINUE		= { 409, "Decode script|Continue"};
  struct LocText L_FOUND_A_SHELL_SCRIPT	= { 410, "Found a Shell Script in:\n\n'%s'"};

// VE_FileStat.c
  struct LocText L_DIRECTORIES			= { 411, "Directories:"};
  struct LocText L_CRUNCHED				= { 412, "Crunched:"};
  struct LocText L_ARCHIVED				= { 413, "Archived:"};
  struct LocText L_VIRUS_FOUND			= { 414, "Virus found:"};
  struct LocText L_FILES_1				= { 415, "Files:"};
  struct LocText L_DECRUNCHED			= { 416, "Decrunched:"};
  struct LocText L_EXTRACTED			= { 417, "Extracted:"};
  struct LocText L_TIME						= { 418, "Time:"};
  struct LocText L_CHECKED				= { 419, "Checked:"};

// VE_files.c
 struct LocText L_ERROR_X					= { 420, "Error: %s"};
 struct LocText L_ARCHIVE_ERROR_1	= { 421, "Archive: %s\n   Type: %s\n   File: %s\n\n  Error: %s"};

// VE_Update.c
 struct LocText L_CHECKING_FOR_UPDATES_1			= { 422, "Checking for updates (%s)..."};
 struct LocText L_CONNECTING_TO_SERVER				= { 423, "Connecting to server..."};
 struct LocText L_ERROR_CONNECTING_TO_SERVER	= { 424, "Error connecting to server!"};
 struct LocText L_ERROR_TALKING_TO_SERVER			= { 425, "Error talking to server!"};
 struct LocText L_CHECKING_VERSION						= { 426, "Checking version..."};
 struct LocText L_ERROR_READING_INFO_DATA			= { 427, "Error reading info data!"};
 struct LocText L_INFO_DATA_IS_CURRUPT				= { 428, "Info data is currupt!"};
 struct LocText L_NEW_VERSION_FOUND						= { 429, "New version found..."};
 struct LocText L_NO_NEW_VERSION							= { 430, "No new version..."};
 struct LocText L_DOWNLOADING_PST							= { 431, "Downloading (%3ld%%)..."};
 struct LocText L_DOWNLOADING_CPS							= { 432, "Downloading %s (%3ld%%, %lD cps)..."};
 struct LocText L_DOWNLOAD_COMPLETE						= { 433, "Download complete."};
 struct LocText L_CHECKSUM_ERROR							= { 434, "Checksum ERROR!"};
 struct LocText L_PLEASE_INSTALL							= { 435, "Please install '%s'..."};
 struct LocText L_OLD													= { 436, "Old"};
 struct LocText L_NEW													= { 437, "New"};
 struct LocText L_STATUS											= { 438, "Status"};
 struct LocText L_COULDNT_FIND_FILE_ON_SERVER	= { 439, "Couldn't read file on server!"};
 struct LocText L_FOUND_UPDATES								= { 440, "Found %ld %s..."};
 struct LocText L_INSTALL_CANCEL							= { 441, "Install|Cancel"};
 struct LocText L_YOU_HAVE_A_SPECIAL					= { 442, "You have a special AmigaOS version of the %s\n\nDo you want to replace it with this new shareware version?"};
 struct LocText L_INSTALLING_1								= { 443, "Installing..."};
 struct LocText L_FINISHED										= { 444, "Finished!"};
 struct LocText L_ABORTED											= { 445, "Aborted!"};
 struct LocText L_README_FILES_ARE_LOCATED		= { 446, ".readme files are located in %s/\n\nYou have to restart VirusExecutor!"};
 struct LocText L_INSTALLING_2								= { 447, "Installing '%s'..."};


// VE_CheckAll.c
 struct LocText L_START												= { 448, "Start"};
 struct LocText L_SELECT_VOLUMES_TO_CHECK			= { 449, "Select volumes to check..."};

// VE_Update.c
 struct LocText L_NO_NEW_MD5									= { 450, "You have this update! (MD5)"};

// VE_Prefs.c
 struct LocText L_THIS_OPTION_WILL_DISABLE		= { 451, "This option will disable ALL requesters during a file scan...\nLogging is now turned on!\n\nLog-file: %s"};

// VE_Update.c
 struct LocText L_YOUR_FILES_ARE_UP_TO_DATE		= { 452, "Your files are up to date."};

#define LOCALE_END
#endif



