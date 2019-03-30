#include <JEO:JEO.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include "ShowWindow.h"
#include "VE_Prototypes.h"

extern UWORD screenWidth, screenHeight;
extern UWORD mainX;
extern struct LocText L_EXIT;
char *GLS (struct LocText *loctext);

struct Window         *ShowWnd = NULL;
struct Gadget         *ShowGList = NULL;
struct Gadget         *ShowGadgets[2];
UWORD                  ShowLeft = 0;
UWORD                  ShowTop = 40;
UWORD                  ShowWidth = 640;
UWORD                  ShowHeight = 202;
UBYTE                 *ShowWdt = NULL;

char SS_Exit[30];

UWORD ShowGTypes[] = {
	LISTVIEW_KIND,
	BUTTON_KIND
};

struct NewGadget ShowNGad[] = {
	4, 2, 625, 176, NULL, NULL, GD_SHOWLIST, 0, NULL, NULL,
	277, 177, 84, 20, (UBYTE *)SS_Exit, NULL, GD_EXIT, PLACETEXT_IN, NULL, NULL
};

ULONG ShowGTags[] = {
	(GTLV_ReadOnly), TRUE, (TAG_DONE),
	(TAG_DONE)
};

int OpenShowWindow (ULONG lines, UWORD width)
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;
	UWORD new_width;

	ShowHeight = screenHeight - 60 + offy;

	ShowNGad[0].ng_Height = ShowHeight - 40;
	ShowNGad[1].ng_TopEdge = ShowHeight - 37;

	if (width == 640)
	{
		ShowWidth = 640;
		ShowNGad[0].ng_Width = 625;
	}
	else
	{
		new_width = (width * 8) + 20;
		if (new_width < ShowWidth)
		{
			ShowWidth = new_width + 6;
			ShowNGad[0].ng_Width = new_width - 10;
		}
	}
	ShowNGad[1].ng_LeftEdge = (ShowWidth / 2) - (ShowNGad[1].ng_Width / 2);

	ShowLeft = CenterWindowLeft (ShowWidth);

	mystrcpy (SS_Exit, GLS (&L_EXIT));

	if ( ! ( g = CreateContext( &ShowGList )))
		return( 1L );

	for( lc = 0, tc = 0; lc < Show_CNT; lc++ ) {

		CopyMem((char * )&ShowNGad[ lc ], (char * )&ng, (long)sizeof( struct NewGadget ));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr   = &topaz8;
		ng.ng_LeftEdge  += offx;
		ng.ng_TopEdge   += offy;

		ShowGadgets[ lc ] = g = CreateGadgetA((ULONG)ShowGTypes[ lc ], g, &ng, ( struct TagItem * )&ShowGTags[ tc ] );

		while( ShowGTags[ tc ] ) tc += 2;
		tc++;

		if ( NOT g )
			return( 2L );
	}

	if ( ! ( ShowWnd = OpenWindowTags( NULL,
				WA_Left,	ShowLeft,
				WA_Top,		ShowTop,
				WA_Width,	ShowWidth,
				WA_Height,	ShowHeight,
				WA_IDCMP,	LISTVIEWIDCMP|BUTTONIDCMP|IDCMP_MENUPICK|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW|IDCMP_RAWKEY|IDCMP_DISKINSERTED|IDCMP_DISKREMOVED,
				WA_Flags,	WFLG_DRAGBAR|WFLG_CLOSEGADGET|WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_RMBTRAP,
				WA_Gadgets,	ShowGList,
				WA_CustomScreen,	Scr,
				TAG_DONE )))
	return( 4L );

	GT_RefreshWindow( ShowWnd, NULL );

	return( 0L );
}

void CloseShowWindow( void )
{
	if ( ShowWnd        ) {
		CloseWindow( ShowWnd );
		ShowWnd = NULL;
	}

	if ( ShowGList      ) {
		FreeGadgets( ShowGList );
		ShowGList = NULL;
	}
}

