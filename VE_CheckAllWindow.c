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
#include "VE_CheckAllWindow.h"

struct Window         *CheckAllWnd = NULL;
struct Gadget         *CheckAllGList = NULL;
struct Gadget         *CheckAllGadgets[CheckAll_CNT+2];
UWORD                  CheckAllLeft = 143;
UWORD                  CheckAllTop = 35;
UWORD                  CheckAllWidth = 352;
UWORD                  CheckAllHeight = 100;

char VolumNames[CheckAll_CNT][51];
char CheckAllWdtName[50];

BOOL volumFlag[CheckAll_CNT];

UWORD CheckAllGTypes[] = {
	BUTTON_KIND,
	BUTTON_KIND,
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

char CA_Start[21];
char CA_Cancel[21];

struct NewGadget CheckAllNGad[] = {
	 40, 0, 96, 24, (UBYTE *)CA_Start, NULL, 0, PLACETEXT_IN, NULL, NULL,
	210, 0, 96, 24, (UBYTE *)CA_Cancel, NULL, 1, PLACETEXT_IN, NULL, NULL,
	23, GADGY + (11 *  0), 26, 11, (UBYTE *)VolumNames[0],  NULL,  2, 2, NULL, NULL,
	23, GADGY + (11 *  1), 26, 11, (UBYTE *)VolumNames[1],  NULL,  3, 2, NULL, NULL,
	23, GADGY + (11 *  2), 26, 11, (UBYTE *)VolumNames[2],  NULL,  4, 2, NULL, NULL,
	23, GADGY + (11 *  3), 26, 11, (UBYTE *)VolumNames[3],  NULL,  5, 2, NULL, NULL,
	23, GADGY + (11 *  4), 26, 11, (UBYTE *)VolumNames[4],  NULL,  6, 2, NULL, NULL,
	23, GADGY + (11 *  5), 26, 11, (UBYTE *)VolumNames[5],  NULL,  7, 2, NULL, NULL,
	23, GADGY + (11 *  6), 26, 11, (UBYTE *)VolumNames[6],  NULL,  8, 2, NULL, NULL,
	23, GADGY + (11 *  7), 26, 11, (UBYTE *)VolumNames[7],  NULL,  9, 2, NULL, NULL,
	23, GADGY + (11 *  8), 26, 11, (UBYTE *)VolumNames[8],  NULL, 10, 2, NULL, NULL,
	23, GADGY + (11 *  9), 26, 11, (UBYTE *)VolumNames[9],  NULL, 11, 2, NULL, NULL,
	23, GADGY + (11 * 10), 26, 11, (UBYTE *)VolumNames[10], NULL, 12, 2, NULL, NULL,
	23, GADGY + (11 * 11), 26, 11, (UBYTE *)VolumNames[11], NULL, 13, 2, NULL, NULL,
	23, GADGY + (11 * 12), 26, 11, (UBYTE *)VolumNames[12], NULL, 14, 2, NULL, NULL,
	23, GADGY + (11 * 13), 26, 11, (UBYTE *)VolumNames[13], NULL, 15, 2, NULL, NULL,
	23, GADGY + (11 * 14), 26, 11, (UBYTE *)VolumNames[14], NULL, 16, 2, NULL, NULL,
	23, GADGY + (11 * 15), 26, 11, (UBYTE *)VolumNames[15], NULL, 17, 2, NULL, NULL,
	23, GADGY + (11 * 16), 26, 11, (UBYTE *)VolumNames[16], NULL, 18, 2, NULL, NULL,
	23, GADGY + (11 * 17), 26, 11, (UBYTE *)VolumNames[17], NULL, 19, 2, NULL, NULL,
	23, GADGY + (11 * 18), 26, 11, (UBYTE *)VolumNames[18], NULL, 20, 2, NULL, NULL,
	23, GADGY + (11 * 19), 26, 11, (UBYTE *)VolumNames[19], NULL, 21, 2, NULL, NULL
};

ULONG CheckAllGTags[]= {
	(TAG_DONE),
	(TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE),
	(GTCB_Checked), TRUE, (TAG_DONE)
};

int OpenCheckAllWindow (UBYTE max)
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;

	mystrcpy (CA_Start, GLS (&L_START));
	mystrcpy (CA_Cancel, GLS (&L_WCANCEL));

	if (!(g = CreateContext (&CheckAllGList)))
		return (1);

	for (lc = 0, tc = 0; lc < max + 2; lc++)
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

	mystrcpy (CheckAllWdtName, GLS (&L_SELECT_VOLUMES_TO_CHECK));

	CheckAllLeft = CenterWindowLeft (CheckAllWidth);
	CheckAllTop = CenterWindowTop (CheckAllHeight);

	if (!(CheckAllWnd = OpenWindowTags( NULL,
				WA_Left,	CheckAllLeft,
				WA_Top,		CheckAllTop,
				WA_Width,	CheckAllWidth,
				WA_Height,	CheckAllHeight + offy,
				WA_IDCMP,	BUTTONIDCMP|CHECKBOXIDCMP|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW,
				WA_Flags,	WFLG_CLOSEGADGET|WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_RMBTRAP,
				WA_Gadgets,	CheckAllGList,
				WA_Title,	CheckAllWdtName,
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

VOID CheckAllInitGadget (UWORD num, LONG tagtype, LONG tagvalue)
{
  GT_SetGadgetAttrs (CheckAllGadgets[num], CheckAllWnd, NULL, tagtype, tagvalue, TAG_DONE);
}


BOOL HandleCheckAll (UBYTE max_vol)
{
	ULONG Signals;
	ULONG class;
	USHORT Code;
	APTR Address;
	struct IntuiMessage *message;
	BOOL go_flag;
	BYTE count;

	count = max_vol;
	go_flag = TRUE;
	while (go_flag)
	{
		Signals = Wait (1 << CheckAllWnd->UserPort->mp_SigBit);

		if (Signals & (1<<CheckAllWnd->UserPort->mp_SigBit))
		{
			while (message = (struct IntuiMessage *)GetMsg (CheckAllWnd->UserPort))
			{
        class = message->Class;
        Code = message->Code;
        Address = message->IAddress;
        ReplyMsg ((struct Message *)message);
		
				switch (class)
				{
					case IDCMP_CLOSEWINDOW:
					{
						go_flag = FALSE;
						break;
					}
					case IDCMP_RAWKEY:
					{
						switch (Code)
						{
							case ESC:
								go_flag = FALSE;
								break;
						}
						break;
					}
					case	IDCMP_GADGETUP:
					{
					  struct Gadget *gadget;
						UWORD nr;


					  gadget = (struct Gadget *)Address;
						nr = gadget->GadgetID;

						if (nr >= 2)
						{
							volumFlag[nr-2]	^= 1;

							if (volumFlag[nr-2] == OFF)
								count--;
							else
								count++;
							if (!count)
						    CheckAllInitGadget (0, GA_Disabled, TRUE);
					    else
						    CheckAllInitGadget (0, GA_Disabled, FALSE);
						}
						else
						{
							switch (nr)
							{
								case 0: return (count);	// Start
								case 1: return (FALSE); // Cancel
							}
						}

						break;
					}
				}
			}
		}
	}
	return (FALSE);
}
