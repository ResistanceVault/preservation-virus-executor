#include <JEO:JEO.h>
#include "VE_Window.h"
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include "VE_Prototypes.h"
#include <intuition/gadgetclass.h>
#include "PrefsWindow.h"

struct Window         *PrefsWnd = NULL;
struct Gadget         *PrefsGList = NULL;
struct Gadget         *PrefsGadgets[Prefs_CNT];
extern Class          *initGet( void );
struct IClass         *getClass = NULL;
struct _Object        *getImage = NULL;
UWORD                  PrefsLeft = 78;
UWORD                  PrefsTop = 56;
UWORD                  PrefsWidth = 471;
UWORD                  PrefsHeight = 181;

char P_WinInfo[100];
char P_File_checking[30];
char P_Misc[30];

struct IntuiText PrefsIText[] = {
	2, 0, JAM1, 16, 13, &topaz8, (UBYTE *)P_File_checking, &PrefsIText[1],
	2, 0, JAM1, 16, 138, &topaz8, (UBYTE *)P_Misc, NULL 
};

UWORD PrefsGTypes[] = {
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,	INTEGER_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,	STRING_KIND, 	GENERIC_KIND,
	CHECKBOX_KIND
};

char  P1[50];	// "Decruch crunched files"
char  P2[50]; // "Overwrite crunched files"
char  P3[50]; // "Skip encrypted files"
char  P4[50];	// "Check data files"
char  P5[50];	// "Extract archives" 
char  P6[50];	// "N/A"
char  P7[50]; // "N/A"
char  P8[50]; // "Skip files older than"
char  P9[50]; // "day(s)"
char P10[50]; // "Write to log file"
char P11[50]; // "Fast startup"
char P12[50]; // "Fast file checking"

struct NewGadget PrefsNGad[] = {
	16,  26, 26, 11, (UBYTE *)P1, NULL, GD_P_DECRUNCH_FILES, PLACETEXT_RIGHT, NULL, NULL,
	16,  37, 26, 11, (UBYTE *)P2, NULL, GD_P_OVERWRITE_CRUNCHED_FILES, PLACETEXT_RIGHT, NULL, NULL,
	16,  48, 26, 11, (UBYTE *)P3, NULL, GD_P_SKIP_ENCRYPTED, PLACETEXT_RIGHT, NULL, NULL,
	16,  59, 26, 11, (UBYTE *)P4, NULL, GD_P_CHECK_DATA_FILES, PLACETEXT_RIGHT, NULL, NULL,
	16,  70, 26, 11, (UBYTE *)P5, NULL, GD_P_EXTRACT_ARCHIVES, PLACETEXT_RIGHT, NULL, NULL,
	16,  81, 26, 11, (UBYTE *)P6, NULL, GD_P_NA, PLACETEXT_RIGHT, NULL, NULL,
	16,  92, 26, 11, (UBYTE *)P7, NULL, GD_P_NA1, PLACETEXT_RIGHT, NULL, NULL,
	16, 103, 26, 11, (UBYTE *)P8, NULL, GD_P_SKIP_DAYS, PLACETEXT_RIGHT, NULL, NULL,
	 0, 103, 60, 12, (UBYTE *)P9, NULL, GD_P_DAYS_COUNT, PLACETEXT_RIGHT, NULL, NULL,
	16, 114, 26, 11, (UBYTE *)P12, NULL, GD_P_FAST_FILE_CHECKING, PLACETEXT_RIGHT, NULL, NULL,

	16, 150, 26, 11, (UBYTE *)P10, NULL, GD_P_LOGGING, PLACETEXT_RIGHT, NULL, NULL,
	0, 150, 60, 12, (UBYTE *)"", NULL, GD_P_LOG_PATH, PLACETEXT_LEFT, NULL, NULL,
	426, 149, 60, 14, NULL, NULL, GD_P_FILE, 0, NULL, NULL,
	16, 161, 26, 11, (UBYTE *)P11, NULL, GD_P_ICONIFY, PLACETEXT_RIGHT, NULL, NULL
};

ULONG PrefsGTags[] = {
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(GTIN_Number), 0, (GTIN_MaxChars), 5, (STRINGA_Justification), (GACT_STRINGRIGHT), (TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(GTST_MaxChars), 128, (TAG_DONE),
	(TAG_DONE)
};

void PrefsRender( void )
{
	UWORD		offx, offy;

	offx = PrefsWnd->BorderLeft;
	offy = PrefsWnd->BorderTop;

	DrawBevelBox( PrefsWnd->RPort, offx + 9, offy +   8, 445, 122, GT_VisualInfo, VisualInfo, TAG_DONE );
	DrawBevelBox( PrefsWnd->RPort, offx + 9, offy + 133, 445,  43, GT_VisualInfo, VisualInfo, TAG_DONE );
	PrintIText( PrefsWnd->RPort, PrefsIText, offx, offy );
}

int OpenPrefsWindow( void )
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;
	UWORD my_Left, my_Top;
	UWORD path_off;

	mystrcpy (P_WinInfo, GLS (&L_PREFS_WININFO));
	mystrcpy (P_File_checking, GLS (&L_FILE_CHECKING));
	mystrcpy (P_Misc, GLS (&L_MISC));

	mystrcpy (P1, GLS (&L_P1));
	mystrcpy (P2, GLS (&L_P8));
	mystrcpy (P3, GLS (&L_P6));
	mystrcpy (P4, GLS (&L_P2));
	mystrcpy (P5, GLS (&L_EXTRACT_ARCHIVES));
	mystrcpy (P6, "N/A");
	mystrcpy (P7, "N/A");
	mystrcpy (P8, GLS (&L_P10));
	mystrcpy (P9, GLS (&L_P11));
	PrefsNGad[GD_P_DAYS_COUNT].ng_LeftEdge = (strlen (P8) * 8) + 56;
	mystrcpy (P12, GLS (&L_P12));

	mystrcpy (P10, GLS (&L_LOGGING));
	path_off = (strlen (P10) * 8) + 56;
	PrefsNGad[GD_P_LOG_PATH].ng_LeftEdge = path_off;
	PrefsNGad[GD_P_LOG_PATH].ng_Width = 420 - path_off;

	mystrcpy (P11, GLS (&L_ICONIFY_AT_STARTUP));

	if ( ! ( g = CreateContext( &PrefsGList )))
		return( 1L );
 
	for( lc = 0, tc = 0; lc < Prefs_CNT; lc++ ) {

		CopyMem((char * )&PrefsNGad[ lc ], (char * )&ng, (long)sizeof( struct NewGadget ));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr   = &topaz8;
		ng.ng_LeftEdge  += offx;
		ng.ng_TopEdge   += offy;

		PrefsGadgets[ lc ] = g = CreateGadgetA((ULONG)PrefsGTypes[ lc ], g, &ng, ( struct TagItem * )&PrefsGTags[ tc ] );

		if (PrefsGTypes[ lc ] == GENERIC_KIND ) {
			g->Flags             |= GFLG_GADGIMAGE | GFLG_GADGHIMAGE;
			g->Activation        |= GACT_RELVERIFY;
			g->GadgetRender       = (APTR)getImage;
			g->SelectRender       = (APTR)getImage;
		}

		while( PrefsGTags[ tc ] ) tc += 2;
		tc++;

		if ( NOT g )
			return( 2L );
	}

	my_Left = (prefs->screenmode.DisplayWidth / 2) - (PrefsWidth / 2);
	my_Top = (256 / 2) - (PrefsHeight / 2);

	if ( ! ( PrefsWnd = OpenWindowTags( NULL,
				WA_Left,	my_Left,
				WA_Top,		my_Top,
				WA_Width,	PrefsWidth,
				WA_Height,	PrefsHeight + offy,
				WA_IDCMP,	CHECKBOXIDCMP|IDCMP_GADGETDOWN|IDCMP_MENUPICK|IDCMP_DISKINSERTED|IDCMP_DISKREMOVED|IDCMP_REFRESHWINDOW|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
				WA_Flags,	WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_CLOSEGADGET|WFLG_RMBTRAP,
				WA_Gadgets,	PrefsGList,
				WA_Title,	P_WinInfo,
				WA_PubScreen,	Scr,
				TAG_DONE )))
	return( 4L );

	GT_RefreshWindow( PrefsWnd, NULL );

	PrefsRender();

	return( 0L );
}

void ClosePrefsWindow( void )
{
	if ( PrefsWnd        ) {
		CloseWindow( PrefsWnd );
		PrefsWnd = NULL;
	}

	if ( PrefsGList      ) {
		FreeGadgets( PrefsGList );
		PrefsGList = NULL;
	}
}

