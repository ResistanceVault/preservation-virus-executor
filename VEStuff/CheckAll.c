#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>
#include "CheckAll.h"

struct Screen         *Scr = NULL;
APTR                   VisualInfo = NULL;
struct Window         *CheckAllWnd = NULL;
struct Gadget         *CheckAllGList = NULL;
struct Gadget         *CheckAllGadgets[CheckAll_CNT];
UWORD                  CheckAllLeft = 179;
UWORD                  CheckAllTop = 74;
UWORD                  CheckAllWidth = 352;
UWORD                  CheckAllHeight = 193;
UBYTE                 *CheckAllWdt = (UBYTE *)"Window name";

struct TextAttr topaz8 = {
	( STRPTR )"topaz.font", 8, 0x00, 0x01 };

UWORD CheckAllGTypes[CheckAll_CNT] = {
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND
};

#define GADGY 20

char  VolumNames[CheckAll_CNT][40];

struct NewGadget CheckAllNGad[CheckAll_CNT] = {
	23, GADGY + (11 *  0), 26, 11, (UBYTE *)VolumNames[0],  NULL,  0, 2, NULL, NULL,
	23, GADGY + (11 *  1), 26, 11, (UBYTE *)VolumNames[1],  NULL,  1, 2, NULL, NULL,
	23, GADGY + (11 *  2), 26, 11, (UBYTE *)VolumNames[2],  NULL,  2, 2, NULL, NULL,
	23, GADGY + (11 *  3), 26, 11, (UBYTE *)VolumNames[3],  NULL,  3, 2, NULL, NULL,
	23, GADGY + (11 *  4), 26, 11, (UBYTE *)VolumNames[4],  NULL,  4, 2, NULL, NULL,
	23, GADGY + (11 *  5), 26, 11, (UBYTE *)VolumNames[5],  NULL,  5, 2, NULL, NULL,
	23, GADGY + (11 *  6), 26, 11, (UBYTE *)VolumNames[6],  NULL,  6, 2, NULL, NULL,
	23, GADGY + (11 *  7), 26, 11, (UBYTE *)VolumNames[7],  NULL,  7, 2, NULL, NULL,
	23, GADGY + (11 *  8), 26, 11, (UBYTE *)VolumNames[8],  NULL,  8, 2, NULL, NULL,
	23, GADGY + (11 *  9), 26, 11, (UBYTE *)VolumNames[9],  NULL,  9, 2, NULL, NULL,
	23, GADGY + (11 * 10), 26, 11, (UBYTE *)VolumNames[10], NULL, 10, 2, NULL, NULL,
	23, GADGY + (11 * 11), 26, 11, (UBYTE *)VolumNames[11], NULL, 11, 2, NULL, NULL,
	23, GADGY + (11 * 12), 26, 11, (UBYTE *)VolumNames[12], NULL, 12, 2, NULL, NULL,
	23, GADGY + (11 * 13), 26, 11, (UBYTE *)VolumNames[13], NULL, 13, 2, NULL, NULL,
	23, GADGY + (11 * 14), 26, 11, (UBYTE *)VolumNames[14], NULL, 14, 2, NULL, NULL,
	23, GADGY + (11 * 15), 26, 11, (UBYTE *)VolumNames[15], NULL, 15, 2, NULL, NULL,
	23, GADGY + (11 * 16), 26, 11, (UBYTE *)VolumNames[16], NULL, 16, 2, NULL, NULL,
	23, GADGY + (11 * 17), 26, 11, (UBYTE *)VolumNames[17], NULL, 17, 2, NULL, NULL,
	23, GADGY + (11 * 18), 26, 11, (UBYTE *)VolumNames[18], NULL, 18, 2, NULL, NULL,
	23, GADGY + (11 * 19), 26, 11, (UBYTE *)VolumNames[19], NULL, 19, 2, NULL, NULL
};

ULONG CheckAllGTags[CheckAll_CNT]= {
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE),
	(TAG_DONE)
};

UWORD DriPens[] = {
	0xFFFF };

int OpenCheckAllWindow (UBYTE max)
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;


	if (!(g = CreateContext (&CheckAllGList)))
		return (1);

	for (lc = 0, tc = 0; lc < max; lc++)
	{
		CopyMem((char * )&CheckAllNGad[lc], (char *)&ng, (long)sizeof (struct NewGadget));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr   = &topaz8;
		ng.ng_LeftEdge  += offx;
		ng.ng_TopEdge   += offy;

		CheckAllGadgets[lc] = g = CreateGadgetA((ULONG)CheckAllGTypes[lc], g, &ng, (struct TagItem *)&CheckAllGTags[tc]);

		while (CheckAllGTags[tc])
			tc += 2;
		tc++;

		if (NOT g)
			return (2);
	}

	if (!(CheckAllWnd = OpenWindowTags( NULL,
				WA_Left,	CheckAllLeft,
				WA_Top,		CheckAllTop,
				WA_Width,	CheckAllWidth,
				WA_Height,	CheckAllHeight + offy,
				WA_IDCMP,	CHECKBOXIDCMP|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW,
				WA_Flags,	WFLG_CLOSEGADGET|WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_RMBTRAP,
				WA_Gadgets,	CheckAllGList,
				WA_Title,	CheckAllWdt,
				WA_PubScreen,	Scr,
				TAG_DONE )))
		return( 4L );

	GT_RefreshWindow (CheckAllWnd, NULL);

	return (0);
}

void CloseCheckAllWindow( void )
{
	if ( CheckAllWnd        ) {
		CloseWindow( CheckAllWnd );
		CheckAllWnd = NULL;
	}

	if ( CheckAllGList      ) {
		FreeGadgets( CheckAllGList );
		CheckAllGList = NULL;
	}
}


main ()
{
	UBYTE max, i;

	max = 20;

	for (i = 0; i < max; i++)
		mystrcpy (VolumNames[i], "Test");

	SetupScreen ();
	OpenCheckAllWindow (max);
	Delay (200);
	CloseCheckAllWindow ();
	CloseDownScreen ();
}
