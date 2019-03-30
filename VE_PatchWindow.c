#include <JEO:JEO.h>
#include <dos.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/gadtools.h>
#include <utility/hooks.h>
#include <graphics/gfxmacros.h>
#include <proto/graphics.h>
#include "PatchWindow.h"


#include "VE_Prototypes.h"

extern UWORD mainX;

struct Window         *PatchWnd = NULL;
struct Gadget         *PatchGList = NULL;
struct Gadget         *PatchGadgets[2];
UWORD                  PatchTop = 40;
UWORD                  PatchWidth = 640;
UWORD                  PatchHeight = 202;
UBYTE                 *PatchWdt =
 NULL;

char P_Exit[15];
extern struct LocText L_EXIT;
char *GLS (struct LocText *loctext);

UWORD PatchGTypes[] = {
	LISTVIEW_KIND,
	BUTTON_KIND
};

struct NewGadget PatchNGad[] = {
	4, 2, 625, 176, NULL, NULL, GD_PATCHLIST, 0, NULL, NULL,
	277, 177, 84, 20, (UBYTE *)P_Exit, NULL, GD_EXIT, PLACETEXT_IN, NULL, NULL
};

struct Hook renderHook;

ULONG PatchGTags[] = {
	(GTLV_ShowSelected), NULL, GTLV_CallBack, (ULONG)&renderHook, (TAG_DONE),
	(TAG_DONE)
};


static UWORD GhostPattern[2] =
{
     0x4444,
     0x1111,
};


/* Apply a ghosting pattern to a given rectangle in a rastport */
static VOID Ghost(struct RastPort *rp, UWORD pen, UWORD x0, UWORD y0, UWORD x1, UWORD y1)
{
    SetABPenDrMd(rp,pen,0,JAM1);
    SetAfPt(rp,GhostPattern,1);
    RectFill(rp,x0,y0,x1,y1);
    SetAfPt(rp,NULL,0);
}

/* Erase any part of "oldExtent" which is not covered by "newExtent" */
VOID FillOldExtent(struct RastPort *rp,
                   struct Rectangle *oldExtent,
                   struct Rectangle *newExtent)
{
    if (oldExtent->MinX < newExtent->MinX)
        RectFill(rp,oldExtent->MinX,
                    oldExtent->MinY,
                    newExtent->MinX-1,
                    oldExtent->MaxY);

    if (oldExtent->MaxX > newExtent->MaxX)
        RectFill(rp,newExtent->MaxX+1,
                    oldExtent->MinY,
                    oldExtent->MaxX,
                    oldExtent->MaxY);

    if (oldExtent->MaxY > newExtent->MaxY)
        RectFill(rp,oldExtent->MinX,
                    newExtent->MaxY+1,
                    oldExtent->MaxX,
                    oldExtent->MaxY);

    if (oldExtent->MinY < newExtent->MinY)
        RectFill(rp,oldExtent->MinX,
                    oldExtent->MinY,
                    oldExtent->MaxX,
                    newExtent->MinY-1);
}

static ULONG __asm RenderHook(register __a1 struct LVDrawMsg *msg,
                              register __a2 struct Node *node)
{
struct RastPort   *rp;
UBYTE              state;
struct TextExtent  extent;
ULONG              fit;
WORD               x,y;
WORD               slack;
ULONG              apen;
ULONG              bpen;
UWORD             *pens;
STRPTR             name;

    geta4();

    if (msg->lvdm_MethodID != LV_DRAW)
        return(LVCB_UNKNOWN);

    rp    = msg->lvdm_RastPort;
    state = msg->lvdm_State;
    pens  = msg->lvdm_DrawInfo->dri_Pens;

    name = node->ln_Name;

		if ((strlen (name) > 40) AND ((name[38] == '*') OR name[38] == '['))
		{
	    apen = pens[HIGHLIGHTTEXTPEN];
  	  bpen = pens[FILLPEN];
	    if ((state == LVR_NORMAL) || (state == LVR_NORMALDISABLED))
  	  {
	  	   apen = pens[HIGHLIGHTTEXTPEN];    /* this is normally TEXTPEN */
	       bpen = pens[BACKGROUNDPEN];
  	  }
  	}
  	else
  	{
	    apen = pens[FILLTEXTPEN];
  	  bpen = pens[FILLPEN];
	    if ((state == LVR_NORMAL) || (state == LVR_NORMALDISABLED))
  	  {
	  	   apen = pens[TEXTPEN];
	       bpen = pens[BACKGROUNDPEN];
  	  }
  	}


    SetABPenDrMd(rp,apen,bpen,JAM2);

    fit = TextFit(rp,name,strlen(name),&extent,NULL,1,
                  msg->lvdm_Bounds.MaxX-msg->lvdm_Bounds.MinX-3,
                  msg->lvdm_Bounds.MaxY-msg->lvdm_Bounds.MinY+1);

    slack = (msg->lvdm_Bounds.MaxY - msg->lvdm_Bounds.MinY) - (extent.te_Extent.MaxY - extent.te_Extent.MinY);

    x = msg->lvdm_Bounds.MinX - extent.te_Extent.MinX + 2;
    y = msg->lvdm_Bounds.MinY - extent.te_Extent.MinY + ((slack+1) / 2);

    extent.te_Extent.MinX += x;
    extent.te_Extent.MaxX += x;
    extent.te_Extent.MinY += y;
    extent.te_Extent.MaxY += y;

    Move(rp,x,y);
    Text(rp,name,fit);

    SetAPen(rp,bpen);
    FillOldExtent(rp,&msg->lvdm_Bounds,&extent.te_Extent);

    if ((state == LVR_NORMALDISABLED) || (state == LVR_SELECTEDDISABLED))
    {
        Ghost(rp,pens[BLOCKPEN],msg->lvdm_Bounds.MinX, msg->lvdm_Bounds.MinY,
                                msg->lvdm_Bounds.MaxX, msg->lvdm_Bounds.MaxY);
    }

    return(LVCB_OK);
}

int OpenPatchWindow( void )
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		offx = Scr->WBorLeft, offy = Scr->WBorTop + Scr->RastPort.TxHeight + 1;

	renderHook.h_Entry = (HOOKFUNC)RenderHook;

	mystrcpy (P_Exit, GLS (&L_EXIT));
	if ( ! ( g = CreateContext( &PatchGList )))
		return( 1L );

	for( lc = 0, tc = 0; lc < Patch_CNT; lc++ ) {

		CopyMem((char * )&PatchNGad[ lc ], (char * )&ng, (long)sizeof( struct NewGadget ));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr   = &topaz8;
		ng.ng_LeftEdge  += offx;
		ng.ng_TopEdge   += offy;

		PatchGadgets[ lc ] = g = CreateGadgetA((ULONG)PatchGTypes[ lc ], g, &ng, ( struct TagItem * )&PatchGTags[ tc ] );

		while( PatchGTags[ tc ] ) tc += 2;
		tc++;

		if ( NOT g )
			return( 2L );
	}

	if ( ! ( PatchWnd = OpenWindowTags( NULL,
				WA_Left,	mainX,
				WA_Top,		PatchTop,
				WA_Width,	PatchWidth,
				WA_Height,	PatchHeight + offy,
				WA_IDCMP,	LISTVIEWIDCMP|BUTTONIDCMP|IDCMP_MENUPICK|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW|IDCMP_RAWKEY|IDCMP_DISKINSERTED|IDCMP_DISKREMOVED,
				WA_Flags,	WFLG_DRAGBAR|WFLG_CLOSEGADGET|WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_RMBTRAP,
				WA_Gadgets,	PatchGList,
				WA_CustomScreen,	Scr,
				TAG_DONE )))
	return( 4L );

	GT_RefreshWindow( PatchWnd, NULL );

	return( 0L );
}

void ClosePatchWindow( void )
{
	if ( PatchWnd        ) {
		CloseWindow( PatchWnd );
		PatchWnd = NULL;
	}

	if ( PatchGList      ) {
		FreeGadgets( PatchGList );
		PatchGList = NULL;
	}
}
