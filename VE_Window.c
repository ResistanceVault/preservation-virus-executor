#include <JEO:JEO.h>
#include "VE_Window.h"
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <exec/types.h>
#include "PrefsWindow.h"

#include "VE_Prototypes.h"

struct Screen         *Scr = NULL;
UBYTE                 *PubScreenName = "Workbench";
APTR                   VisualInfo = NULL;
struct Window         *VirusWnd = NULL;
struct Menu           *VirusMenus = NULL;
UWORD                  VirusLeft = 0;
UWORD                  VirusTop = 0;
UWORD                  VirusWidth = 640;
UWORD                  VirusHeight = 256;
UBYTE                 *VirusWdt = NULL;

struct TextAttr topaz8 = {
	( STRPTR )"topaz.font", 8, 0x00, 0x01 };

char w_Project[40];
char w_About[40];
char w_Iconify[40];
char w_Prefs[40];
char w_Memory_check[40];
char w_ColdReboot[40];
char w_File_check[40];
char w_Check_valid_volumes[40];
char w_Sector_check[40];
char w_Select_screen_mode[40];
char w_Quit[40];

char w_File_watch[40];
char w_Check_now[40];
char w_Add_a_directory[40];
char w_Remove_files[40];
char w_Add_update_startup_files[40];

char w_Show[40];
char w_Bootblock_virus[40];
char w_Link_virus[40];
char w_File_virus[40];
char w_Data_virus[40];
char w_Tasks[40];
char w_Libraries[40];
char w_Devices[40];
char w_Patches[40];
char w_Install[40];
char w_Bootblock_database[40];
char w_Record[40];
char w_View[40];
char w_Edit_name[40];
char w_Merge[40];
char w_Delete_one[40];
char w_Delete_all[40];
char w_Save[40];
char w_Utilities[40];
char w_Rename_pictures[40];
char w_Convert_IBN[40];
char w_Save_ROM_to_file[40];
char w_Update_files[40];

struct NewMenu VirusNewMenu[] = {
	NM_TITLE, (STRPTR)w_Project, NULL, 0, NULL, NULL,						// 0
	NM_ITEM, (STRPTR)w_About, NULL, 2, 0L, NULL,										//  0
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,									//  1
	NM_ITEM, (STRPTR)w_Iconify, (STRPTR)"H", 0, 0L, NULL,						//  2
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,									//  1
	NM_ITEM, (STRPTR)w_Prefs, (STRPTR)"P", 0, 0L, NULL,						//  2
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,									//  3
	NM_ITEM, (STRPTR)w_Memory_check, (STRPTR)"M", 0, 0L, NULL,				//  4
	NM_ITEM, (STRPTR)w_ColdReboot, NULL, 0, 0L, NULL,				//  4
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,									//  5
	NM_ITEM, (STRPTR)w_File_check, (STRPTR)"F", 0, 0L, NULL,					//  6
	NM_ITEM, (STRPTR)w_Check_valid_volumes, NULL, 0, 0L, NULL,	//  7
	NM_ITEM, (STRPTR)w_Sector_check, NULL, 0, 0L, NULL,	//  7
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,									//  8
	NM_ITEM, (STRPTR)w_Select_screen_mode, NULL, 0, 0L, NULL,			//  9
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,									// 10
	NM_ITEM, (STRPTR)w_Quit, (STRPTR)"Q", 0, 0L, NULL,								// 11

	NM_TITLE, (STRPTR)w_File_watch, NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)w_Check_now, (STRPTR)"W", 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Add_a_directory, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Add_update_startup_files, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Remove_files, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Save, NULL, 0, 0L, NULL,

	NM_TITLE, (STRPTR)w_Show, NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)w_Bootblock_virus, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Link_virus, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_File_virus, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Data_virus, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,									// 10
	NM_ITEM, (STRPTR)w_Tasks, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Libraries, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Devices, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Patches, NULL, 0, 0L, NULL,

	NM_TITLE, (STRPTR)w_Install, NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)"DF0:", (STRPTR)"0", 0, 0L, NULL,
	NM_ITEM, (STRPTR)"DF1:", (STRPTR)"1", 0, 0L, NULL,
	NM_ITEM, (STRPTR)"DF2:", (STRPTR)"2", 0, 0L, NULL,
	NM_ITEM, (STRPTR)"DF3:", (STRPTR)"3", 0, 0L, NULL,

	NM_TITLE, (STRPTR)w_Bootblock_database, NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)w_Record, (STRPTR)"R", 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_View, (STRPTR)"V", 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Edit_name, (STRPTR)"N", 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Merge, (STRPTR)"T", 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Delete_one, (STRPTR)"D", 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Delete_all, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Save, (STRPTR)"S", 0, 0L, NULL,

	NM_TITLE, (STRPTR)w_Utilities, NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)w_Rename_pictures, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Convert_IBN, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Save_ROM_to_file, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)w_Update_files, (STRPTR)"U", 0, 0L, NULL,
	NM_END, NULL, NULL, 0, 0L, NULL };

struct ColorSpec ScreenColors[] = {
	 0, 0x0A, 0x0A, 0x0A,	// GREY
	 1, 0x00, 0x00, 0x00,	// BLACK
	 2, 0x0F, 0x0F, 0x0F,	// WHITE
	 3, 0x06, 0x08, 0x0B,	// BLUE
	~0, 0x00, 0x00, 0x00 };

UWORD DriPens[] = {
	0xFFFF };

int SetupScreen( void )
{
	screenWidth = prefs->screenmode.DisplayWidth;
	mainX = (screenWidth - 630) / 2;
	if (mainX < 5)
		mainX = 5;

	screenHeight = prefs->screenmode.DisplayHeight;

	if (prefs->wb_flag)
	{
		if ( ! ( Scr = LockPubScreen( PubScreenName )))
			return( 1L );
	}
	else
	{
		if (!(Scr = OpenScreenTags( NULL, SA_Left, 0,
					SA_Top,		0,
					SA_Width,	prefs->screenmode.DisplayWidth,
					SA_Height,	prefs->screenmode.DisplayHeight,
					SA_Depth,	2,
					SA_Colors,	&ScreenColors[0],
					SA_Font,	&topaz8,
					SA_Type,	CUSTOMSCREEN,
					SA_DisplayID,	prefs->screenmode.DisplayID,
					SA_AutoScroll,	prefs->screenmode.AutoScroll,
					SA_Overscan,	prefs->screenmode.OverscanType,
					SA_Pens,	&DriPens[0],
					SA_Title,	Ver,
					TAG_DONE )))
		return( 1L );
	}

	if (!(VisualInfo = GetVisualInfo (Scr, TAG_DONE)))
		return (2L);

	if ( ! ( getClass = initGet()))
		return( 3L );

	if ( ! ( getImage = NewObject( getClass, NULL, GT_VisualInfo, VisualInfo, TAG_DONE )))
		return( 4L );

	return (0L);
}

void CloseDownScreen ( void )
{
	if ( getImage   ) {
		DisposeObject( getImage );
		getImage = NULL;
	}

	if ( getClass   ) {
		FreeClass( getClass );
		getClass = NULL;
	}

	if (VisualInfo)
	{
		FreeVisualInfo (VisualInfo);
		VisualInfo = NULL;
	}

	if (Scr)
	{
		if (prefs->wb_flag)
			UnlockPubScreen (NULL, Scr);
		else
			CloseScreen (Scr);
		Scr = NULL;
	}

	if (prefs)
		prefs->wb_flag = wb_flag;
}

int OpenVirusWindow (VOID)
{
  ULONG menuPen;
 	UWORD	offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;

  if (ROM_version < 39)
    menuPen = 0;
  else
    menuPen = 1;

  if (!(VirusMenus = CreateMenus( VirusNewMenu, GTMN_FrontPen, menuPen, TAG_DONE)))
    return (3);

	mystrcpy (w_Project, 						GLS (&L_PROJECT));
	mystrcpy (w_About, 							GLS (&L_ABOUT));
	mystrcpy (w_Iconify, 						GLS (&L_ICONIFY));
	mystrcpy (w_Prefs, 							GLS (&L_PREFS));
	mystrcpy (w_Memory_check, 			GLS (&L_MEMORY_CHECK));
	mystrcpy (w_ColdReboot,	 				GLS (&L_COLDREBOOT));
	mystrcpy (w_File_check, 				GLS (&L_FILE_CHECK));
	mystrcpy (w_Check_valid_volumes,	GLS (&L_CHECK_VALID_VOLUMES));
	mystrcpy (w_Sector_check, 			GLS (&L_SECTOR_CHECK));
	mystrcpy (w_Select_screen_mode, GLS (&L_SELECT_SCREEN));
	mystrcpy (w_Quit, 							GLS (&L_QUIT));
	
	mystrcpy (w_File_watch, 					GLS (&L_FILE_WATCH));
	mystrcpy (w_Check_now, 						GLS (&L_CHECK_NOW));
	mystrcpy (w_Add_a_directory, 			GLS (&L_ADD_A_DIRECTORY));
	mystrcpy (w_Add_update_startup_files,	GLS (&L_ADD_UPDATE_STARTUP));
	mystrcpy (w_Remove_files, 				GLS (&L_REMOVE_FILES));

	mystrcpy (w_Show, 							GLS (&L_SHOW));
	mystrcpy (w_Bootblock_virus,		GLS (&L_BOOTBLOCK_VIRUS));
	mystrcpy (w_Link_virus, 				GLS (&L_LINK_VIRUS));
	mystrcpy (w_File_virus, 				GLS (&L_FILE_VIRUS_MENU));
	mystrcpy (w_Data_virus, 				GLS (&L_DATA_VIRUS));
	mystrcpy (w_Tasks, 							GLS (&L_TASKS));
	mystrcpy (w_Libraries, 					GLS (&L_LIBRARIES));
	mystrcpy (w_Devices, 						GLS (&L_DEVICES));
	mystrcpy (w_Patches, 						GLS (&L_PATCHES));
	mystrcpy (w_Install, 						GLS (&L_INSTALL));
	mystrcpy (w_Bootblock_database, GLS (&L_BOOTBLOCK_DATABASE));
	mystrcpy (w_Record, 						GLS (&L_RECORD));
	mystrcpy (w_View, 							GLS (&L_VIEW));
	mystrcpy (w_Edit_name, 					GLS (&L_EDIT_NAME));
	mystrcpy (w_Merge, 							GLS (&L_MERGE));
	mystrcpy (w_Delete_one, 				GLS (&L_DELETE_ONE));
	mystrcpy (w_Delete_all, 				GLS (&L_DELETE_ALL));
	mystrcpy (w_Save, 							GLS (&L_SAVE));
	mystrcpy (w_Utilities, 					GLS (&L_UTILITIES));
	mystrcpy (w_Rename_pictures, 		GLS (&L_RENAME_PICTURES));
	mystrcpy (w_Convert_IBN, 				GLS (&L_CONVERT_IBN));
	mystrcpy (w_Save_ROM_to_file, 	GLS (&L_SAVE_ROM_TO_FILE));
	mystrcpy (w_Update_files,				GLS (&L_UPDATE_FILES));

  LayoutMenus( VirusMenus, VisualInfo, TAG_DONE );

	if (prefs->wb_flag)
	{
		VirusLeft = VirusTop = 0;
		screenWidth = 640;
		mainX = 5;

		if (prefs->screenmode.DisplayHeight > 276)
			screenHeight = 276;

	  if (!(VirusWnd = OpenWindowTags (NULL,
			WA_Left,	VirusLeft,
			WA_Top,		VirusTop,
			WA_Width,	screenWidth,
			WA_Height,	screenHeight + offy,
			WA_IDCMP,	IDCMP_CLOSEWINDOW|IDCMP_MOUSEBUTTONS|IDCMP_MENUPICK|IDCMP_RAWKEY|IDCMP_DISKINSERTED|IDCMP_DISKREMOVED|IDCMP_REFRESHWINDOW,
			WA_Flags,	WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_NEWLOOKMENUS|WFLG_SMART_REFRESH|WFLG_ACTIVATE,
			WA_PubScreen,	Scr,
			TAG_DONE )))
  		  return (4);
  }
  else
  {
	  if (!(VirusWnd = OpenWindowTags (NULL,
			WA_Left,	VirusLeft,
			WA_Top,		VirusTop,
			WA_Width,	screenWidth,
			WA_Height,	screenHeight + offy,
			WA_IDCMP,	IDCMP_MOUSEBUTTONS|IDCMP_MENUPICK|IDCMP_RAWKEY|IDCMP_DISKINSERTED|IDCMP_DISKREMOVED|IDCMP_REFRESHWINDOW,
			WA_Flags,	WFLG_NEWLOOKMENUS|WFLG_SMART_REFRESH|WFLG_BACKDROP|WFLG_BORDERLESS|WFLG_ACTIVATE,
			WA_CustomScreen,	Scr,
			TAG_DONE )))
  		  return (4);
	}

//	if (prefs->wb_flag)
//	VirusWnd->IFont = (struct TextFont *)&topaz8;

  SetMenuStrip (VirusWnd, VirusMenus);
  GT_RefreshWindow (VirusWnd, NULL);

  return (0);
}

void CloseVirusWindow( void )
{
	if ( VirusMenus      ) {
		ClearMenuStrip( VirusWnd );
		FreeMenus( VirusMenus );
		VirusMenus = NULL;	}

	if ( VirusWnd        ) {
		CloseWindow( VirusWnd );
		VirusWnd = NULL;
	}
}
