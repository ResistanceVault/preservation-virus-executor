#define _DEBUG1	0
#define PATCHBRAIN_REV_OK 50

#include <JEO:JEO.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <libraries/reqtools.h>
#include <proto/asl.h>
#include <proto/reqtools.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <proto/layers.h>
#include <proto/icon.h>
#include <proto/mathieeesingbas.h>
#include <proto/mathffp.h>
#include <proto/keymap.h>
#include <proto/expansion.h>
#include <proto/wb.h>
#include <utility/hooks.h>
#include "VE_Prototypes.h"
#include <proto/xvs.h>
#include "PatchWindow.h"
#include <JEO:raw.h>
#include "VE_PatchOffsets.h"

extern char far Audio_dev_fd[6][MAX_LVO_LEN]; 
extern char far Console_dev_fd[12][MAX_LVO_LEN];
extern char far Gameport_dev_fd[6][MAX_LVO_LEN];
extern char far Input_dev_fd[7][MAX_LVO_LEN];
extern char far Keyboard_dev_fd[6][MAX_LVO_LEN];
extern char far Ramdrive_dev_fd[8][MAX_LVO_LEN];
extern char far Scsi_dev_fd[6][MAX_LVO_LEN];
extern char far Timer_dev_fd[11][MAX_LVO_LEN];
extern char far Trackdisk_dev_fd[6][MAX_LVO_LEN];

extern BOOL morphOS_flag;

patch_values far pv[PATCH_END];

char ExecIntTxt[][12] =
{
	"Serial Out",		//  0
	"Disk Block",		//  1
	"Soft Int",			//  2
	"CIA A", 				//  3
	"Copper",    		//  4
	"Vert. Blank",	//  5
	"Blit Ready",		//  6
	"Audio Ch. 0",	//  7
	"Audio Ch. 1",	//  8
	"Audio Ch. 2",	//  9
	"Audio Ch. 3",	// 10
	"Serial In",		// 11
	"Disk Sync",		// 12
	"CIA B",				// 13
	"Int. Enable",	// 14
	"NMI"						// 15
};

VOID MemoryMonitor (ULONG address, WORD off);

BOOL tmp_mode;

BOOL fd_flag = FALSE;
char far PatchNames[1000][PATCH_NAME_LENGTH];
VEPatch far ve_patch[5000];
ULONG max_entries;
UWORD ve_max_patches = 0;
BOOL known;
ULONG patch_count;

UWORD patchBrainVersion;
UWORD patchBrainRevision;

char VE_PatchName[300];

ULONG CalculatePatchChecksum (VOID);
UWORD patchCount;

ULONG far old_patchAddress[1000];
ULONG pa_old_count = 0;

ULONG far new_patchAddress[1000];
ULONG pa_new_count = 0;

typedef struct
{
  struct List list;
} Sjef;

Sjef patch;

typedef struct
{
  struct Node nd;
  char Name[100];
  ULONG address;
	WORD off;
} Patch;

struct Node *PatchFindTheOne (char *Name)
{
  struct Node *nd;

  nd = patch.list.lh_Head;
  while (nd->ln_Succ)
  {
    if (stricmp (nd->ln_Name, Name) > 0)
    {
      nd = nd->ln_Pred;
      return (nd);
    }
    nd = nd->ln_Succ;
  }
  return (FALSE);
}

BOOL InsertPatch (char *Name, ULONG address, LONG off)
{
  Patch *bp = 0;

  if (!(bp = AllocMem (sizeof (Patch), MEMF_PUBLIC)))
    return (FALSE);

  mystrcpy (bp->Name, Name);
  bp->address = address;
  bp->off = off;
  bp->nd.ln_Name=bp->Name;
	AddTail (&patch.list, &bp->nd);	// Alltid legge til....
	patchCount++;
  return (TRUE);
}

VOID KillPatch (VOID)
{
  ULONG s;
  struct Node *nd;

  nd = patch.list.lh_Head;
  for (s = 0; s < patchCount; s++)
  {
    Remove (nd);
    FreeMem ((UBYTE *)nd, sizeof (Patch));
    nd = 0;
    nd = patch.list.lh_Head;
  }
  patchCount = 0;
}

VOID ClosePatch (VOID)
{
  Patch *bp;

  KillPatch ();
  bp = (Patch *)patch.list.lh_Head;
  while (bp = (Patch *)RemHead (&patch.list))
  {
    FreeMem (bp, sizeof (Patch));
    bp = 0;
  }
}

BOOL HandlePatchIDCMP (VOID)
{
	struct IntuiMessage	*m;
	UWORD	code;
 	ULONG class;
	struct IntuiMessage	tmpmsg;
  BOOL ret = TRUE;

	while (m = GT_GetIMsg (PatchWnd->UserPort))
	{
		CopyMem ((char *)m, (char *)&tmpmsg, (long)sizeof(struct IntuiMessage));
		GT_ReplyIMsg (m);
		code = tmpmsg.Code;
		class = tmpmsg.Class;
		switch (tmpmsg.Class)
		{
			case IDCMP_DISKINSERTED:
				DiskInserted ();
				Status (GLS (&L_PLEASE_WAIT));
				break;
			case IDCMP_DISKREMOVED:
				DiskRemoved ();
				Status (GLS (&L_PLEASE_WAIT));
				break;
			case	IDCMP_RAWKEY:
			{
				switch (code)
				{
					case ESC:
						ret = FALSE;
						break;
					case RETURN:
						ret = FALSE;
						break;
					case ENTER:
						ret = FALSE;
						break;
				}
				break;
			}
			case	IDCMP_CLOSEWINDOW:
			{
				ret = FALSE;
				break;
			}
			case	IDCMP_GADGETUP:
			{
			  struct Gadget *gadget;
				UWORD nr;

			  gadget = (struct Gadget *)tmpmsg.IAddress;
				nr = gadget->GadgetID;

			  switch (nr)
			  {
					case GD_PATCHLIST:
					{
						struct Node *nd;
					  Patch *bp;
						UWORD n;

						nd = patch.list.lh_Head;
						for (n = 0; n < tmpmsg.Code; n++)
							nd = nd->ln_Succ;
					  bp = (Patch *)nd;
						MemoryMonitor (bp->address, bp->off);
					  break;
					}				
					case GD_EXIT:
					{
						ret = FALSE;
						break;
					}
				}
				break;
			}
		}
	}
	return (ret);
}

VOID ShowPatches (VOID)
{
	ULONG	waitsigs, gotsigs;
	BOOL quit = 1;

	if (!OpenPatchWindow ())
	{
		SetWindowTitles (PatchWnd, GLS (&L_LIBRARY_VECTORS), Ver);
		GT_SetGadgetAttrs (PatchGadgets[GD_PATCHLIST], PatchWnd, NULL , GTLV_Labels,
										 &patch.list, TAG_DONE, NULL);
		waitsigs = (1L << PatchWnd->UserPort->mp_SigBit);

		while (quit)
		{
			gotsigs = Wait (waitsigs);
			quit = HandlePatchIDCMP ();
		}
		if (PatchWnd)
			ClosePatchWindow ();
	}
}

// ***********************************************************************

typedef char (* __asm SegTrack(register __a0 ULONG, register __a1 ULONG *, register __a2 ULONG *));
struct SegSem
{
	struct SignalSemaphore seg_Semaphore;
  SegTrack *seg_Find;
};

char SEG_SEM[] = "SegTracker";

BOOL FindSegAddress (ULONG address, char *Name)
{
	BOOL ret = FALSE;
	struct SegSem *segSem;

	if (segSem = (struct SegSem *)FindSemaphore(SEG_SEM))
  {
  	char *p;
		ULONG tmp[4];

		tmp[0] = address;
		Forbid();
    p = (*segSem->seg_Find) (tmp[0], &tmp[2], &tmp[3]);
    Permit();
		if (p)
		{
			strcpy (Name, p);
			ret = TRUE;
		}
	}
	return (ret);
}

// ***********************************************************************

#define GF_NOLVO      0   /* LVO is invalid for this library base */
#define GF_CODEINLVO  -1  /* LVO is not 'jmp (abs).l' or similar */

/* Get pointer to function LVO opcodes
   This routine is used by higher level routines...
*/

BOOL InROM (ULONG address);

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
	if ((((ULONG)ptr >= (ULONG)hw_ROM_start) AND ((ULONG)ptr < hw_ROM_end)) OR TypeOfMem (ptr))
		return TRUE;
	if (morphOS_flag)
	{
		if (*ptr>>24 == 0x10)		// 0x10xxxxxx
			return (TRUE);
	}
	return FALSE;
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
  const UWORD *op, *p;
  const void *ptr;

  /* Get ptr to function opcodes
  */

  op  = GetOpcodes (lib, lvo);
  if (!op)
    return (const void *) GF_NOLVO;

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

		p = (UWORD *)ptr;		// JMP check
		if (*p == 0x4EF9)
		{
			ULONG *p1;

			p++;
			p1 = (ULONG *)p;
			ptr = (const void *) (ULONG)*p1;
		}
	}
  else if ((op[0] == 0x60FF) ||     /* BRA.L      - too.. (68020+ opcode) */
           (op[0] == 0x61FF))       /* BSR.L      - too.. (68020+ opcode) */
  {

    /* long offsets at op[1] relative to op[1]...
    */
    ptr = (const void *)(((UBYTE *)&op[1]) + *((ULONG *)&op[1]));
  }

  if (ValidMem((ULONG *)ptr))
    return ptr;

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

ULONG vbr;
char AddressErrorTxt[] = "<<< INVALID ADDRESS!!!";
ULONG check_sum = 0;

ULONG CheckAddress (UWORD type, struct Library *lib, WORD function)
{	// 0 returns OK!
	ULONG *p;
	char *p_tmp;
	UWORD i;
	ULONG *val;

	VE_PatchName[0] = 0;

	if (type == CPU_INT)
	{
		ULONG *Int;

		vbr = GetVBR ();
		Int = (ULONG *)(vbr + function);
		p = (ULONG *)*Int;
		check_sum += (ULONG)p;
	}
	else if (type == EXEC_INT)
	{
		ULONG *exec;
	
		p = (ULONG *)SysBase;

		exec = (ULONG *)p;
		exec += 22;
		exec += (function * 3);
		p = (ULONG *)*exec;
		check_sum += (ULONG)p;
	}
	else
		p = (ULONG *)GetFunctionCode (lib, function);

  if (ValidMem(p))
	{
		if (!InROM ((ULONG)p))
		{
			BOOL flag;

			check_sum += (ULONG)p;

			if (morphOS_flag)
			{
				if (*p>>24 == 0xFF)		// 0xff??0001
					return (1);
			}
			if (pv[type].flag)	// Any patches here?
			{
				for (i = pv[type].patch_start; i < pv[type].patch_end; i++)
				{
					if (function == ve_patch[i].function)	// Funnet et patch!
					{
						p_tmp = (char *)p;
						if (ve_patch[i].offset != 0)
							p_tmp += ve_patch[i].offset;
						if (TypeOfMem (p_tmp))
						{
							val = (ULONG *)p_tmp;

							if (*val == ve_patch[i].data)
							{
								mystrcpy (VE_PatchName, PatchNames[ve_patch[i].name_off]);
								return ((ULONG)p);
							}
						}
					}
				}
			}

		// Fant ingen kjente?

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

				switch (reqtools_version>>16)
				{
					case 39:
					{
						switch (function)
						{
							case   -6: if (*p == 0x4e55ff9c AND *p_tmp == 0x48e73c3a AND nr == 284) return (0);
							case  -12: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e536e AND nr == 24) return (0);
							case  -18: if (*p == 0x4e550000 AND *p_tmp == 0x48e72022 AND nr == 119) return (0);
							case  -24: if (*p == 0x4e550000 AND *p_tmp == 0x42804e5d AND nr == 5) return (0);
							case  -30: if (*p == 0x4e550000 AND *p_tmp == 0x48e72032 AND nr == 124) return (0);
							case  -36: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e2f02 AND nr == 35) return (0);
							case  -42: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f0a AND nr == 30) return (0);
							case  -48: if (*p == 0x4e55fef4 AND *p_tmp == 0x48e73c3a AND nr == 264) return (0);
							case  -54: if (*p == 0x4e55ffe0 AND *p_tmp == 0x48e73f3a AND nr == 1008) return (0);
							case  -60: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e2f0a AND nr == 20) return (0);
							case  -66: if (*p == 0x4e550000 AND *p_tmp == 0x48e70038 AND nr == 20) return (0);
							case  -72: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f0a AND nr == 21) return (0);
							case  -78: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f0a AND nr == 21) return (0);
							case  -84: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f02 AND nr == 25) return (0);
							case  -90: if (*p == 0x4e550000 AND *p_tmp == 0x2f0b2f02 AND nr == 24) return (0);
							case  -96: if (*p == 0x4e55ffe0 AND *p_tmp == 0x48e73f3a AND nr == 1008) return (0);
							case -102: if (*p == 0x4e55ffb8 AND *p_tmp == 0x48e73f3a AND nr == 698) return (0);
							case -108: if (*p == 0x4e550000 AND *p_tmp == 0x2f0a2451 AND nr == 9) return (0);
							case -114: if (*p == 0x4e55fff0 AND *p_tmp == 0x2f0e2b7c AND nr == 24) return (0);
							case -120: if (*p == 0x4e55ffe8 AND *p_tmp == 0x48e73c3a AND nr == 135) return (0);
							case -126: if (*p == 0x4e55fff4 AND *p_tmp == 0x48e73f3a AND nr == 152) return (0);
							case -132: if (*p == 0x4e550000 AND *p_tmp == 0x48e73f20 AND nr == 50) return (0);
							case -138: if (*p == 0x4e550000 AND *p_tmp == 0x48e73022 AND nr == 57) return (0);
							case -144: if (*p == 0x4e55ffe0 AND *p_tmp == 0x48e73f3a AND nr == 1008) return (0);
							case -150: if (*p == 0x4e550000 AND *p_tmp == 0x48e7003a AND nr == 62) return (0);
							case -156: if (*p == 0x4e550000 AND *p_tmp == 0x48e73032 AND nr == 108) return (0);
							case -162: if (*p == 0x4e55ffe0 AND *p_tmp == 0x48e73022 AND nr == 86) return (0);
							case -168: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e2f02 AND nr == 19) return (0);
							case -174: if (*p == 0x4e550000 AND *p_tmp == 0x2f0e41ee AND nr == 13) return (0);
						}
						break;
					}
					default:	// 38
					{
						switch (function)
						{
							case   -6: if (*p == 0x526e003e AND *p_tmp == 0x200e4e75 AND nr == 4) return (0);
							case  -12: if (*p == 0x7000536e AND *p_tmp == 0x003e4e75 AND nr == 4) return (0);
							case  -18: if (*p == 0x48e7202e AND *p_tmp == 0x2a4e2c78 AND nr == 76) return (0);
							case  -24: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -30: if (*p == 0x594f48e7 AND *p_tmp == 0x201e2f48 AND nr == 113) return (0);
							case  -36: if (*p == 0x48e7000e AND *p_tmp == 0x2a49200d AND nr == 34) return (0);
							case  -42: if (*p == 0x594f48e7 AND *p_tmp == 0x001c49f9 AND nr == 28) return (0);
							case  -48: if (*p == 0x9efc011c AND *p_tmp == 0x48e7213e AND nr == 275) return (0);
							case  -54: if (*p == 0x9efc002c AND *p_tmp == 0x48e7373e AND nr == 964) return (0);
							case  -60: if (*p == 0x48e7000e AND *p_tmp == 0x2a4849f9 AND nr == 18) return (0);
							case  -66: if (*p == 0x48e73820 AND *p_tmp == 0x200c7604 AND nr == 12) return (0);
							case  -72: if (*p == 0x48e73820 AND *p_tmp == 0x7602280b AND nr == 11) return (0);
							case  -78: if (*p == 0x48e73820 AND *p_tmp == 0x24097603 AND nr == 7) return (0);
							case  -84: if (*p == 0x48e73820 AND *p_tmp == 0x76004211 AND nr == 27) return (0);
							case  -90: if (*p == 0x48e73820 AND *p_tmp == 0x76016008 AND nr == 31) return (0);
							case  -96: if (*p == 0x9efc002c AND *p_tmp == 0x48e7373e AND nr == 964) return (0);
							case -102: if (*p == 0x9efc0134 AND *p_tmp == 0x48e73f3e AND nr == 1923) return (0);
							case -108: if (*p == 0x2f114e75 AND nr == 2) return (0);
							case -114: if (*p == 0x48e73002 AND *p_tmp == 0x22780004 AND nr == 29) return (0);
							case -120: if (*p == 0x9efc001c AND *p_tmp == 0x48e7201e AND nr == 114) return (0);
							case -126: if (*p == 0x48e73f3c AND *p_tmp == 0x28482649 AND nr == 115) return (0);
							case -132: if (*p == 0x48e73c00 AND *p_tmp == 0x2a014845 AND nr == 34) return (0);
							case -138: if (*p == 0x2f0e226e AND *p_tmp == 0x002a2c78 AND nr == 24) return (0);
							case -144: if (*p == 0x9efc002c AND *p_tmp == 0x48e7373e AND nr == 964) return (0);
							case -150: if (*p == 0x48e700a6 AND *p_tmp == 0x2a6e002a AND nr == 48) return (0);
							case -156: if (*p == 0x48e730a2 AND *p_tmp == 0x22680024 AND nr == 89) return (0);
							case -162: if (*p == 0x48e730c2 AND *p_tmp == 0x20096700 AND nr == 73) return (0);
							case -168: if (*p == 0x2f0e41ee AND *p_tmp == 0x00562c78 AND nr == 12) return (0);
							case -174: if (*p == 0x2f0e41ee AND *p_tmp == 0x00562c78 AND nr == 9) return (0);
						}
					}
				}
				if ((reqtools_version>>16 == 38) AND ((UWORD)reqtools_version == 1436))
				{
					// PATCH!
					switch (function)
					{
						case  -48: if (*p == 0x4feffee4 AND *p_tmp == 0x48e7213e AND nr == 275) return (0);		// PATCH
						case  -54: if (*p == 0x4fefffd4 AND *p_tmp == 0x48e7373e AND nr == 965) return (0);		// PATCH
						case  -96: if (*p == 0x4fefffd4 AND *p_tmp == 0x48e7373e AND nr == 965) return (0);		// PATCH
						case -102: if (*p == 0x4feffecc AND *p_tmp == 0x48e73f3e AND nr == 1923) return (0);	// PATCH
						case -120: if (*p == 0x4fefffe4 AND *p_tmp == 0x48e7201e AND nr == 114) return (0);		// PATCH
						case -144: if (*p == 0x4fefffd4 AND *p_tmp == 0x48e7373e AND nr == 965) return (0);		// PATCH
						case -150: if (*p == 0x48e700a6 AND *p_tmp == 0x2a6e002a AND nr == 47) return (0);		// PATCH
					}
				}
//				printf ("case %4ld: if (*p == 0x%08lx AND *p_tmp == 0x%08lx AND nr == %ld) return (0);\n", function, *p, *p_tmp, nr);
			}
			else if (type == ASL_LIB)		// Special check!
			{
				UWORD *p_w;
				UWORD nr = 0;
				ULONG *p_tmp;

				if (strncmp (lib->lib_Node.ln_Name, "asl", 3))	// No asl.library
					return (0);

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

//				printf ("%ld: case %4ld: if (*p == 0x%08lx AND *p_tmp == 0x%08lx AND nr == %ld) return (0);\n", asl_version>>16, function, *p, *p_tmp, nr);
				switch (asl_version>>16)
				{
					case 37:
					{
						switch (function)
						{
							case   -6: if (*p == 0x526e0020 AND *p_tmp == 0x200e4e75 AND nr == 4) return (0);
							case  -12: if (*p == 0x536e0020 AND *p_tmp == 0x70004e75 AND nr == 4) return (0);
							case  -18: if (*p == 0x4a6e0020 AND *p_tmp == 0x66f648e7 AND nr == 38) return (0);
							case  -24: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -30: if (*p == 0x700091c8 AND *p_tmp == 0x48e700a0 AND nr == 13) return (0);
							case  -36: if (*p == 0x2008670c AND *p_tmp == 0x22482021 AND nr == 9) return (0);
							case  -42: if (*p == 0x93c94eee AND *p_tmp == 0xffc448e7 AND nr == 23) return (0);
							case  -48: if (*p == 0x48e700a0 AND *p_tmp == 0x4a806700 AND nr == 11) return (0);
							case  -54: if (*p == 0x2008670c AND *p_tmp == 0x22482021 AND nr == 9) return (0);
							case  -60: if (*p == 0x48e73f3c AND *p_tmp == 0x2f0e2448 AND nr == 20) return (0);
							case  -66: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -72: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -78: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -84: if (*p == 0x70004e75 AND nr == 2) return (0);
						}
						break;
					}
					case 39:
					{
						switch (function)
						{
							case   -6: if (*p == 0x526e0022 AND *p_tmp == 0x08ae0003 AND nr == 7) return (0);
							case  -12: if (*p == 0x536e0022 AND *p_tmp == 0x082e0003 AND nr == 8) return (0);
							case  -18: if (*p == 0x61003322 AND *p_tmp == 0x4a6e0022 AND nr == 10) return (0);
							case  -24: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -30: if (*p == 0x700091c8 AND *p_tmp == 0x4eeeffd0 AND nr == 19) return (0);
							case  -36: if	(*p == 0x4eeeffca AND *p_tmp == 0x93c94eee AND nr == 15) return (0);
							case  -42: if (*p == 0x93c94eee AND *p_tmp == 0xffc40000 AND nr == 13) return (0);
							case  -48: if (*p == 0x48e70110 AND *p_tmp == 0x2e002648 AND nr == 41) return (0);
							case  -54: if (*p == 0x2f0b2648 AND *p_tmp == 0x200b6730 AND nr == 30) return (0);
							case  -60: if (*p == 0x48e70032 AND *p_tmp == 0x26482449 AND nr == 48) return (0);
						}
						break;
					}
					case 40:
					{
						switch (function)
						{
							case   -6: if (*p == 0x526e0022 AND *p_tmp == 0x08ae0003 AND nr == 7) return (0);
							case  -12: if (*p == 0x536e0022 AND *p_tmp == 0x082e0003 AND nr == 8) return (0);
							case  -18: if (*p == 0x610033b8 AND *p_tmp == 0x4a6e0022 AND nr == 10) return (0);
							case  -24: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -30: if (*p == 0x700091c8 AND *p_tmp == 0x4eeeffd0 AND nr == 18) return (0);
							case  -36: if (*p == 0x4eeeffca AND *p_tmp == 0x93c94eee AND nr == 14) return (0);
							case  -42: if (*p == 0x93c94eee AND *p_tmp == 0xffc42f0b AND nr == 12) return (0);
							case  -48: if (*p == 0x48e70110 AND *p_tmp == 0x2e002648 AND nr == 41) return (0);
							case  -54: if (*p == 0x2f0b2648 AND *p_tmp == 0x200b6730 AND nr == 30) return (0);
							case  -60: if (*p == 0x48e70032 AND *p_tmp == 0x26482449 AND nr == 48) return (0);
						}
						break;
					}
					case 42:
					{
						switch (function)
						{
							case   -6: if (*p == 0x526e0022 AND *p_tmp == 0x08ae0003 AND nr == 7) return (0);
							case  -12: if (*p == 0x536e0022 AND *p_tmp == 0x082e0003 AND nr == 8) return (0);
							case  -18: if (*p == 0x610035f2 AND *p_tmp == 0x4a6e0022 AND nr == 10) return (0);
							case  -24: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -30: if (*p == 0x700091c8 AND *p_tmp == 0x4eeeffd0 AND nr == 18) return (0);
							case  -36: if (*p == 0x4eeeffca AND *p_tmp == 0x93c94eee AND nr == 14) return (0);
							case  -42: if (*p == 0x93c94eee AND *p_tmp == 0xffc42f0b AND nr == 12) return (0);
							case  -48: if (*p == 0x48e70110 AND *p_tmp == 0x2e002648 AND nr == 41) return (0);
							case  -54: if (*p == 0x2f0b2648 AND *p_tmp == 0x200b6730 AND nr == 30) return (0);
							case  -60: if (*p == 0x48e70032 AND *p_tmp == 0x26482449 AND nr == 48) return (0);
							case  -66:
							{
								if (*p == 0x48e70002 AND nr == 25)
								{
									p_tmp += 2;
									if (*p_tmp == 0x00044eae)
										return (0);
								}
								break;
							}
						}
						break;
					}
					case 44:
					{
						switch (function)
						{
							case   -6: if (*p == 0x526e0020 AND *p_tmp == 0x200e08ae AND nr == 7) return (0);
							case  -12: if (*p == 0x2f07536e AND *p_tmp == 0x0020082e AND nr == 14) return (0);
							case  -18: if (*p == 0x48e72106 AND *p_tmp == 0x2a4e4a6d AND nr == 44) return (0);
							case  -24: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -30: if (*p == 0x700091c8 AND *p_tmp == 0x4eaeffd0 AND nr == 5) return (0);
							case  -36: if (*p == 0x4eaeffca AND *p_tmp == 0x4e752f0d AND nr == 3) return (0);
							case  -42: if (*p == 0x2f0d93c9 AND *p_tmp == 0x2a484eae AND nr == 13) return (0);
							case  -48: if (*p == 0x594f48e7 AND *p_tmp == 0x01102e00 AND nr == 41) return (0);
							case  -54: if (*p == 0x594f2e88 AND *p_tmp == 0x2008672e AND nr == 29) return (0);
							case  -60: if (*p == 0x9efc0030 AND *p_tmp == 0x48e70136 AND nr == 161) return (0);
							case  -66: if (*p == 0x48e70016 AND *p_tmp == 0x2a49200d AND nr == 28) return (0);
							case  -72: if (*p == 0x514f48e7 AND *p_tmp == 0x07162e01 AND nr == 54) return (0);
							case  -78: if (*p == 0x594f48e7 AND *p_tmp == 0x00162a4e AND nr == 59) return (0);
							case  -84: if (*p == 0x594f48e7 AND *p_tmp == 0x00162a4e AND nr == 41) return (0);
						}
						break;
					}
					case 45:
					{
						switch (function)
						{
							case   -6: if (*p == 0x526e0020 AND *p_tmp == 0x200e08ae AND nr == 7) return (0);
							case  -12: if (*p == 0x2f07536e AND *p_tmp == 0x0020082e AND nr == 14) return (0);
							case  -18: if (*p == 0x48e72106 AND *p_tmp == 0x2a4e4a6d AND nr == 44) return (0);
							case  -24: if (*p == 0x70004e75 AND nr == 2) return (0);
							case  -30: if (*p == 0x700091c8 AND *p_tmp == 0x4eaeffd0 AND nr == 5) return (0);
							case  -36: if (*p == 0x4eaeffca AND *p_tmp>>16 == 0x4e75 AND nr == 3) return (0);
							case  -42: if (*p == 0x2f0d93c9 AND *p_tmp == 0x2a484eae AND nr == 13) return (0);
							case  -48: if (*p == 0x594f48e7 AND *p_tmp == 0x01102e00 AND nr == 41) return (0);
							case  -54: if (*p == 0x594f2e88 AND *p_tmp == 0x2008672e AND nr == 29) return (0);
							case  -60: if (*p == 0x9efc0030 AND *p_tmp == 0x48e70136 AND nr == 161) return (0);
							case  -66: if (*p == 0x48e70016 AND *p_tmp == 0x2a49200d AND nr == 28) return (0);
							case  -72: if (*p == 0x514f48e7 AND *p_tmp == 0x07162e01 AND nr == 54) return (0);
							case  -78: if (*p == 0x594f48e7 AND *p_tmp == 0x00162a4e AND nr == 59) return (0);
							case  -84: if (*p == 0x594f48e7 AND *p_tmp == 0x00162a4e AND nr == 41) return (0);
						}
					}
				}
			}
/*
			else if (type == XVS_LIB)		// Special check!
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
// printf ("case %4ld: if (*p == 0x%08lx AND *p_tmp == 0x%08lx AND nr == %ld) return (0); break;\n", function, *p, *p_tmp, nr);

				switch (function)
				{
					case   -6: if (*p == 0x61340000 AND *p_tmp == 0x00001027 AND nr == 45) return (0); break;
					case  -12: if (*p == 0x613a0000 AND *p_tmp == 0x00006134 AND nr == 48) return (0); break;
					case  -18: if (*p == 0x61400000 AND *p_tmp == 0x0000613a AND nr == 51) return (0); break;
					case  -24: if (*p == 0x70004e75 AND nr == 2) return (0); break;
					case  -30: if (*p == 0x614c0000 AND *p_tmp == 0x00007000 AND nr == 5) return (0); break;
					case  -36: if (*p == 0x61520000 AND *p_tmp == 0x0000614c AND nr == 8) return (0); break;
					case  -42: if (*p == 0x61580000 AND *p_tmp == 0x00006152 AND nr == 11) return (0); break;
					case  -48: if (*p == 0x615e0000 AND *p_tmp == 0x00006158 AND nr == 14) return (0); break;
					case  -54: if (*p == 0x61640000 AND *p_tmp == 0x0000615e AND nr == 17) return (0); break;
					case  -60: if (*p == 0x616a0000 AND *p_tmp == 0x00006164 AND nr == 20) return (0); break;
					case  -66: if (*p == 0x61700000 AND *p_tmp == 0x0000616a AND nr == 23) return (0); break;
					case  -72: if (*p == 0x61760000 AND *p_tmp == 0x00006170 AND nr == 26) return (0); break;
					case  -78: if (*p == 0x617c0000 AND *p_tmp == 0x00006176 AND nr == 29) return (0); break;
					case  -84: if (*p == 0x613e0000 AND *p_tmp == 0x0000617c AND nr == 32) return (0); break;
					case  -90: if (*p == 0x61440000 AND *p_tmp == 0x0000613e AND nr == 35) return (0); break;
					case  -96: if (*p == 0x614a0000 AND *p_tmp == 0x00006144 AND nr == 38) return (0); break;
					case -102: if (*p == 0x61500000 AND *p_tmp == 0x0000614a AND nr == 41) return (0); break;
					case -108: if (*p == 0x61560000 AND *p_tmp == 0x00006150 AND nr == 44) return (0); break;
				}
			}
*/
/*
			if (1)
			{
				char Hold[256];
				char Name[] = "ram:VE_Patches";
				ULONG *p1;

			  MakeDir (Name);

				SPrintF (Hold, "%s/%s", Name, lib->lib_Node.ln_Name);
			  MakeDir (Hold);

				SPrintF (Hold, "%s/%s/%ld", Name, lib->lib_Node.ln_Name, function);
				if (lock = Open (Hold, MODE_NEWFILE))
				{
					Write (lock, p, 1024);
					Close (lock);
				}
				SPrintF (Hold, "%s/%s/%ld_NEG", Name, lib->lib_Node.ln_Name, function);
				if (lock = Open (Hold, MODE_NEWFILE))
				{
					p1 = p;
					p1 -= 512 / 4;
					Write (lock, p1, 512);
					Close (lock);
				}
			}
*/
			flag = FALSE;
			known = FALSE;

			mystrcpy (VE_PatchName, GLS (&L_NON_STANDARD_VECTOR));

			new_patchAddress[pa_new_count] = (ULONG)p;	// Legger inn nye patcher...
			pa_new_count++;

			return ((ULONG)p);
		}
	}
	else
	{
		if (p == (APTR)0xffffffff)
			return (0);	// OK
		else
		{
			mystrcpy (VE_PatchName, AddressErrorTxt);
			return ((ULONG)p);
		}
	}
	return (0); // OK
}

char *FD_Buf = 0;
LONG fd_size = 0;

BOOL fd_exists;

VOID Open_fd (char *LibName)
{
	BPTR fh;
	char Hold[108];
	char FileName[108];
	UBYTE i;

	fd_flag = FALSE;
	if (!fd_exists)
		return;

	strcpy (Hold, LibName);
	for (i = 0; Hold[i] != 0; i++)
	{
		if (Hold[i] == '.')
		{
			Hold[i] = '_';
			Hold[i+4] = 0;
			break;
		}			
	}
	SPrintF (FileName, "FD:%s.fd", Hold);
	fd_size = FileSize (FileName);
	if (fd_size > 0)
	{
		if (FD_Buf = AllocMem (fd_size, MEMF_PUBLIC))
		{
			if (fh = Open (FileName, MODE_OLDFILE))
			{
				Read (fh, FD_Buf, fd_size);
				Close (fh);
				fd_flag = TRUE;
			}
		}
	}
}

VOID Close_fd (VOID)
{
	if (FD_Buf)
	{
		FreeMem (FD_Buf, fd_size);
		FD_Buf = 0;
	}
		
	fd_flag = FALSE;
}

BOOL Find_FD_Name (WORD offset, char *FDName)
{
	ULONG i;
	char Hold[200];
	BOOL flag = FALSE;

	switch (offset)
	{
		case -6: strcpy (FDName, "LibOpen"); return (TRUE);
		case -12: strcpy (FDName, "LibClose"); return (TRUE);
		case -18: strcpy (FDName, "LibExpunge"); return (TRUE);
		case -24: strcpy (FDName, "LibExtFunc"); return (TRUE);
	}

	if (fd_flag)
	{
		BOOL cmd = FALSE;
		BOOL skip = FALSE;
		char Cmd[50];
		UWORD j;
		WORD bias = 0;

		Hold[0] = j = 0;
		for (i = 0; i < fd_size; i++)
		{
			if (skip)
			{
				if (FD_Buf[i] == '\n')
				{
					skip = FALSE;
					j = 0;
				}
				else
					continue;
			}
			else if (cmd)
			{
				Cmd[j] = FD_Buf[i];
				if (Cmd[j] == '\n')
				{
					Cmd[j] = 0;
					if (!(strnicmp (Cmd, "bias", 4)))
						bias = atoi (&Cmd[5]) * -1;
					cmd = FALSE;
					j = 0;
				}
				else
				{
					j++;
					continue;
				}
			}
			else if (bias)
			{
				Hold[j] = FD_Buf[i];
				if (Hold[j] == '\n')
				{
					UWORD k;

					if (bias == offset)	// Found?
					{
						Hold[j] = 0;
						for (k = 0; Hold[k] != 0; k++)							
						{
							FDName[k] = Hold[k];
							if (FDName[k] == '(')
							{
								FDName[k] = 0;
								flag = TRUE;
								break;
							}
						}
					}
					bias -= 6;
					j = 0;
				}
				else
					j++;
			}

			if (FD_Buf[i] == '*')	// Skip
			{
				skip = TRUE;
				continue;
			}

			if (i < fd_size - 2)
			{
				if (FD_Buf[i] == '#' AND FD_Buf[i+1] == '#') // Command
				{
					cmd = TRUE;
					Cmd[0] = 0;
					j = 0;
					i++;
				}
			}
		}
		flag = TRUE;
	}
	return (flag);
}

BOOL p_view_flag = FALSE;

VOID Do_stuff (BOOL action, WORD type, ULONG p, ULONG i, char *Library)
{
	UWORD max_fd;
	WORD fd_off;

	if ((p > 0) AND (action))	// Ikke i ROM?
	{
		if (tmp_mode == OFF AND known)
			return;
		else		// Show all!
		{
			char FD_name[50] = "";
			BOOL go_flag;

			if ((action) AND (!p_view_flag))
			{
				UBYTE i;
				p_view_flag = TRUE;
				InsertPatch ("", 0, 0);
				InsertPatch (Library, 0, 0);
				
				for (i = 0; Library[i] != 0; i++)
					Dummy[i] = '-';
				Dummy[i] = 0;
				InsertPatch (Dummy, 0, 0);
			}
			patch_count++;
			go_flag = FALSE;

			if (type == CPU_INT)
			{
				switch (i)
				{
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
				}
			}
			else if (type == EXEC_INT)
				mystrcpy (FD_name, ExecIntTxt[i]);
			else
			{
				fd_off = ((i * -1) - 6) / 6;

				switch (type)
				{
// ***************************** DEVICES ********************************
					case AUDIO_DEV:
					{
						max_fd = sizeof (Audio_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Audio_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					case CONSOLE_DEV:
					{
						max_fd = sizeof (Console_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Console_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					case GAMEPORT_DEV:
					{
						max_fd = sizeof (Gameport_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Gameport_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					case INPUT_DEV:
					{
						max_fd = sizeof (Input_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Input_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					case KEYBOARD_DEV:
					{
						max_fd = sizeof (Keyboard_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Keyboard_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					case RAMDRIVE_DEV:
					{
						max_fd = sizeof (Ramdrive_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Ramdrive_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					case SCSI_DEV:
					{
						max_fd = sizeof (Scsi_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Scsi_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					case TIMER_DEV:
					{
						max_fd = sizeof (Timer_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Timer_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					case TRACKDISK_DEV:
					{
						max_fd = sizeof (Trackdisk_dev_fd) / MAX_LVO_LEN;
						if ((fd_off < max_fd) AND (fd_off >= 0))
						{
							mystrcpy (FD_name, Trackdisk_dev_fd[fd_off]);
							if (FD_name[0] != '?')
								go_flag = TRUE;
						}
						break;
					}
					default:	// Libraries
					{
						if (Find_FD_Name (i, FD_name))
						{
							FD_name[25] = 0;
							go_flag = TRUE;
						}
						else
							FD_name[0] = 0;

						if (type == EXEC_LIB AND morphOS_flag)
						{
							switch (i)
							{
								case -0x318: mystrcpy (FD_name, "NewSetFunction"); go_flag = TRUE; break;
								case -0x31e: mystrcpy (FD_name, "NewCreateLibrary"); go_flag = TRUE; break;
								case -0x324: mystrcpy (FD_name, "NewPPCStackSwap"); go_flag = TRUE; break;
								case -0x336: mystrcpy (FD_name, "NewRawDoFmt"); go_flag = TRUE; break;
								case -0x33c: mystrcpy (FD_name, "CahceFLushDataArea"); go_flag = TRUE; break;
								case -0x342: mystrcpy (FD_name, "CacheInvalidInstArea"); go_flag = TRUE; break;
								case -0x348: mystrcpy (FD_name, "CacheInvalidDataArea"); go_flag = TRUE; break;
								case -0x34e: mystrcpy (FD_name, "CahceFlushDataInstArea"); go_flag = TRUE; break;
								case -0x354: mystrcpy (FD_name, "CacheTrashCacheArea"); go_flag = TRUE; break;
								case -0x35a: mystrcpy (FD_name, "AllocTaskPooled"); go_flag = TRUE; break;
								case -0x360: mystrcpy (FD_name, "FreeTaskPooled"); go_flag = TRUE; break;
								case -0x366: mystrcpy (FD_name, "AllocVecTaskPooled"); go_flag = TRUE; break;
								case -0x36c: mystrcpy (FD_name, "FreeVecTaskPooled"); go_flag = TRUE; break;
							}
							break;
						}
						break;
					}
				}
				if (!*FD_name)
					go_flag = FALSE;

				if (!go_flag)
			  	SPrintF (FD_name, "%5ld (-$%03lx)", i, i * -1);
			}

			if (!(strcmp (VE_PatchName, GLS (&L_NON_STANDARD_VECTOR))))
			{
				char Name[200];

				if (FindSegAddress (p, Name))
				{
					StripFile (Name, Name);
					Name[26] = 0;	// Just in case
					SPrintF (VE_PatchName, "[ST] %s", Name);
				}
			}
		  SPrintF (Dummy, " %-25s: $%08lx %s", FD_name, p, VE_PatchName);
		  InsertPatch (Dummy, p, i);
		}
	}
}

UWORD GetMaxFunction (struct Library *lib)
{
	return (lib->lib_NegSize);
}

VOID CheckPatchVectors (BOOL action, WORD type, struct Library *lib)
{
	WORD i;
	WORD max_func;
	ULONG p;
	char Library[60];

	p_view_flag = FALSE;
	if (type == CPU_INT)
	{
		ULONG i;
	
		vbr = GetVBR ();
		SPrintF (Library, GLS (&L_CPU_INTERRUPT_VECTORS), vbr);
  	for (i = 8; i <= 124; i += 4)
	  {
			if (i > 44 AND i < 52)
				continue;
			if (i > 60 AND i < 96)
				continue;
			known = TRUE;
			p = CheckAddress (type, lib, i);
			Do_stuff (action, type, p, i, Library);
		}
	}
	else if (type == EXEC_INT)
	{
		ULONG i;
	
		SPrintF (Library, GLS (&L_EXEC_INTERRUPT), vbr);
  	for (i = 0; i < 16; i++)
	  {
			known = TRUE;
			p = CheckAddress (type, lib, i);
			Do_stuff (action, type, p, i, Library);
		}
	}
	else
	{
		max_func = GetMaxFunction (lib) * -1;

		SPrintF (Library, "$%08lx: %s %ld.%ld", lib, lib->lib_Node.ln_Name, lib->lib_Version, lib->lib_Revision);
		for (i = -6; i >= max_func; i -= 6)
		{
			known = TRUE;
			p = CheckAddress (type, lib, i);
			if (morphOS_flag)
			{
				if (p == 1)	// MorphOS
					break;
			}
			Do_stuff (action, type, p, i, Library);
		}
	}
}

ULONG FindDeviceAddress (char *DeviceName)
{
  struct Node *node;
  ULONG dev_address = 0;

//	Forbid ();
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
//	Permit ();

	return (dev_address);
}

VOID CheckDeviceVectors (BOOL action)
{
	ULONG dev_address = 0;

	dev_address = FindDeviceAddress ("audio.device");
	if (dev_address)
		CheckPatchVectors (action, AUDIO_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("carddisk.device");
	if (dev_address)
		CheckPatchVectors (action, CARDDISK_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("console.device");
	if (dev_address)
		CheckPatchVectors (action, CONSOLE_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("gameport.device");
	if (dev_address)
		CheckPatchVectors (action, GAMEPORT_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("input.device");
	if (dev_address)
		CheckPatchVectors (action, INPUT_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("keyboard.device");
	if (dev_address)
		CheckPatchVectors (action, KEYBOARD_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("ramdrive.device");
	if (dev_address)
		CheckPatchVectors (action, RAMDRIVE_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("romboot.device");
	if (dev_address)
		CheckPatchVectors (action, ROMBOOT_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("scsi.device");
	if (dev_address)
		CheckPatchVectors (action, SCSI_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("timer.device");
	if (dev_address)
		CheckPatchVectors (action, TIMER_DEV, (struct Library *)dev_address);

	dev_address = FindDeviceAddress ("trackdisk.device");
	if (dev_address)
		CheckPatchVectors (action, TRACKDISK_DEV, (struct Library *)dev_address);
}

ULONG CalculatePatchChecksum (VOID)
{
	ULONG tmp_checksum;
	ULONG checksum = 0;

	pa_new_count = 0;	// Nullstiller nye...
	tmp_checksum = check_sum;	// Take copy
	check_sum = 0;

	CheckPatchVectors (0, CPU_INT, (struct Library *)SysBase);
	CheckPatchVectors (0, EXEC_INT, (struct Library *)SysBase);
	CheckDeviceVectors (0);
	CheckPatchVectors (0, EXEC_LIB, (struct Library *)SysBase);
	CheckPatchVectors (0, UTIL_LIB, (struct Library *)ReqToolsBase->UtilityBase);
	CheckPatchVectors (0, GFX_LIB,  (struct Library *)ReqToolsBase->GfxBase);
	CheckPatchVectors (0, INT_LIB,  (struct Library *)ReqToolsBase->IntuitionBase);
	CheckPatchVectors (0, DOS_LIB,  (struct Library *)ReqToolsBase->DOSBase);
	CheckPatchVectors (0, GAD_LIB,  (struct Library *)ReqToolsBase->GadToolsBase);
	CheckPatchVectors (0, LAY_LIB,  (struct Library *)LayersBase);
	CheckPatchVectors (0, ICON_LIB, (struct Library *)IconBase);
	CheckPatchVectors (0, MATHIEEE_LIB, (struct Library *)MathIeeeSingBasBase);
	CheckPatchVectors (0, MATHFFP_LIB, (struct Library *)MathBase);
	CheckPatchVectors (0, KEYMAP_LIB, (struct Library *)KeymapBase);
	CheckPatchVectors (0, EXPANSION_LIB, (struct Library *)ExpansionBase);
	CheckPatchVectors (0, WB_LIB, (struct Library *)WorkbenchBase);
	if (asl_version)
		CheckPatchVectors (0, ASL_LIB, (struct Library *)AslBase);
	CheckPatchVectors (0, REQTOOLS_LIB, (struct Library *)ReqToolsBase);
//	CheckPatchVectors (0, XVS_LIB, (struct Library *)AslBase);
	checksum = check_sum;			// New checksum??
	check_sum = tmp_checksum;	// restore
	return (checksum);
}

#define MEGA (i * crypt_size - ((b + (c * 4)) ^ 6) / (2 + (crypt_size + 2) * 3) + (d * 3))

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

BOOL LoadPatchData (VOID)
{
	BPTR fh;
	ULONG ve_head;
	BOOL ret = FALSE;
	char PatchName[300];
	ULONG i;

	for (i = 0; i < PATCH_END; i++)
	{
		pv[i].patch_start = 0;
		pv[i].patch_end = 0;
		pv[i].flag = FALSE;
	}

	SPrintF (PatchName, "%s%s.PatchBrain", VEAssign, VE); 
	if (fh = Open (PatchName, MODE_OLDFILE))
	{
		ULONG size;
		
		size = FileSize (PatchName);

		Read (fh, &ve_head, 4);	// Header
		if (ve_head == 0x56455041)	// VEPA
		{
			UBYTE nr, tmp_nr = 0;

			Status (GLS (&L_LOADING_PATCH_DATA));

			Read (fh, Dummy, PATCHBRAINSIZE);
			Read (fh, &patchBrainVersion, 2);
			Read (fh, &patchBrainRevision, 2);

			if (patchBrainRevision < PATCHBRAIN_REV_OK)
			{
				char Hold[100];

				SPrintF (Dummy, GLS (&L_ERROR_LOADING), PatchName);
				SPrintF (Hold, GLS (&L_PATCH_REQUIERS), 1, PATCHBRAIN_REV_OK);
				strcat (Dummy, Hold);
				JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
				Close (fh);
				CleanUp ();
			}

			Read (fh, &max_entries, 4);
			Read (fh, &ve_max_patches, 2);
			Read (fh, &PatchNames[0], ve_max_patches * PATCH_NAME_LENGTH);
			VE_Crypt (DECRYPT, (char *)&PatchNames[0], ve_max_patches * PATCH_NAME_LENGTH);

			for (i = 0; i < max_entries; i++)
			{
		    JEOBar (VirusWnd, mainX, 35 + OFFY, mainX + 628, 37 + OFFY, i + 1, max_entries, BLUE);
				Read (fh, &ve_patch[i], sizeof (VEPatch));
			}
			VE_Crypt (DECRYPT, (UBYTE *)&ve_patch[0], sizeof (VEPatch) * max_entries);

			ret = TRUE;
			Status (GLS (&L_PLEASE_WAIT));

			for (i = 0; i < max_entries; i++)
			{
				nr = ve_patch[i].type;
				if (pv[nr].flag == OFF)
				{
					pv[nr].patch_start = i;	// Start off
					pv[nr].flag = ON;
					if (pv[tmp_nr].flag)	// Write end off
						pv[tmp_nr].patch_end = pv[nr].patch_start;
				}
				else
					tmp_nr = nr;
			}
			pv[nr].patch_end = max_entries;
			ClearBar ();
		}
		else
		{
			SPrintF (Dummy, GLS (&L_PATCH_ERROR), PatchName);
			JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
		}
		Close (fh);
	}
	else
	{
		SPrintF (Dummy, GLS (&L_ERROR_LOADING), PatchName);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
	}

	return (ret);
}

extern BOOL filecheck_progress;

VOID CheckPatches (BYTE mode)
{
	BOOL play = FALSE;
	UWORD i;
	BPTR lock;
	static BOOL xpkpatch = FALSE;
	char XPKPatchName[] = "XPKPatch";

	if (FindPort (XPKPatchName))
	{
		if (!xpkpatch)
		{
			JEOWrite (rp, mainX + 8, 140 + OFFY, XPKPatchName, BLUE);
			xpkpatch = TRUE; 
		}
	}
	else if (xpkpatch)	// Remove text?
	{
		JEOWrite (rp, mainX + 8, 140 + OFFY, "        ", BLUE);
	  xpkpatch = FALSE; 
	}

	fd_exists = FALSE;
	if (mode)
	{
		struct Process *task;
		APTR old;

		task = (struct Process*)FindTask (NULL);
		old = task->pr_WindowPtr;
		task->pr_WindowPtr = (void *)-1;

		task->pr_WindowPtr = old;
	}
	patch_count = 0;
	tmp_mode = mode;
	if (mode == FALSE)	// Ikke vise dersom alt ok
	{
		ULONG checkSum;

		if ((checkSum = CalculatePatchChecksum ()) != check_sum)	// Forandringer?
		{
			UWORD j;
			BOOL flag = FALSE;

			check_sum = checkSum;
			play = TRUE;
			mode = TRUE;

			if (pa_new_count)	// Any new unknown patches?
			{
				for (j = 0; j < pa_new_count; j++)	// Går igjenom nye ukjente patcher...
				{
					if (new_patchAddress[j] != old_patchAddress[j])
						flag = TRUE;
					old_patchAddress[j] = new_patchAddress[j];	// Copy
				}
				if (!flag)	// No new unknown
					mode = FALSE;
			}
		}
	}
	if (mode)
	{
  	APTR win_ptr;

		All (OFF);
	  NewList (&patch.list);
  	patchCount = 0;

		check_sum = 0;
		pa_new_count = 0;	// Nullstiller nye...

		CheckPatchVectors (1, CPU_INT, (struct Library *)SysBase);
		CheckPatchVectors (1, EXEC_INT, (struct Library *)SysBase);

		CheckDeviceVectors (1);

		win_ptr = pr->pr_WindowPtr;
		pr->pr_WindowPtr = (APTR)-1;
		if (lock = Lock ("FD:", ACCESS_READ))
		{
			UnLock (lock);
			fd_exists = TRUE;
		}
		else
			fd_exists = FALSE;
		pr->pr_WindowPtr = win_ptr;

		Open_fd (SysBase->LibNode.lib_Node.ln_Name);
		CheckPatchVectors (1, EXEC_LIB, (struct Library *)SysBase);
		Close_fd ();

		Open_fd (ReqToolsBase->UtilityBase->lib_Node.ln_Name);
		CheckPatchVectors (1, UTIL_LIB, (struct Library *)ReqToolsBase->UtilityBase);
		Close_fd ();

		Open_fd (ReqToolsBase->GfxBase->LibNode.lib_Node.ln_Name);
		CheckPatchVectors (1, GFX_LIB,  (struct Library *)ReqToolsBase->GfxBase);
		Close_fd ();

		Open_fd (ReqToolsBase->IntuitionBase->LibNode.lib_Node.ln_Name);
		CheckPatchVectors (1, INT_LIB,  (struct Library *)ReqToolsBase->IntuitionBase);
		Close_fd ();

		Open_fd (ReqToolsBase->DOSBase->dl_lib.lib_Node.ln_Name);
		CheckPatchVectors (1, DOS_LIB,  (struct Library *)ReqToolsBase->DOSBase);
		Close_fd ();

		Open_fd (ReqToolsBase->GadToolsBase->lib_Node.ln_Name);
		CheckPatchVectors (1, GAD_LIB,  (struct Library *)ReqToolsBase->GadToolsBase);
		Close_fd ();

		Open_fd (LayersBase->lib_Node.ln_Name);
		CheckPatchVectors (1, LAY_LIB,  (struct Library *)LayersBase);
		Close_fd ();

		Open_fd (IconBase->lib_Node.ln_Name);
		CheckPatchVectors (1, ICON_LIB, (struct Library *)IconBase);
		Close_fd ();

		Open_fd (MathIeeeSingBasBase->lib_Node.ln_Name);
		CheckPatchVectors (1, MATHIEEE_LIB, (struct Library *)MathIeeeSingBasBase);
		Close_fd ();

		Open_fd (MathBase->lib_Node.ln_Name);
		CheckPatchVectors (1, MATHFFP_LIB, (struct Library *)MathBase);
		Close_fd ();

		Open_fd (KeymapBase->lib_Node.ln_Name);
		CheckPatchVectors (1, KEYMAP_LIB, (struct Library *)KeymapBase);
		Close_fd ();

		Open_fd (ExpansionBase->LibNode.lib_Node.ln_Name);
		CheckPatchVectors (1, EXPANSION_LIB, (struct Library *)ExpansionBase);
		Close_fd ();

		Open_fd ("wb_lib");
		CheckPatchVectors (1, WB_LIB, (struct Library *)WorkbenchBase);
		Close_fd ();

		if (asl_version)
		{
			Open_fd (AslBase->lib_Node.ln_Name);
			CheckPatchVectors (1, ASL_LIB, (struct Library *)AslBase);
			Close_fd ();
		}

		Open_fd (ReqToolsBase->LibNode.lib_Node.ln_Name);
		CheckPatchVectors (1, REQTOOLS_LIB, (struct Library *)ReqToolsBase);
		Close_fd ();

//		Open_fd (xvsBase->lib_Node.ln_Name);
//		CheckPatchVectors (1, XVS_LIB, (struct Library *)xvsBase);
//		Close_fd ();

		for (i = 0; i < pa_new_count; i++)	// Kopiere over...
			old_patchAddress[i] = new_patchAddress[i];
		pa_old_count = pa_new_count;

		InsertPatch ("", 0, 0);

		if (patch_count)	// Er det noen her i det hele tatt?
		{
			if (!tmp_mode)
				PlaySound (S_ERROR);
			else
				PlaySound (S_NO_SOUND);

			if (icon)
				UnIconify ();
			ShowPatches ();
		}
		if (!filecheck_progress)
			All (ON);
	}
	ClosePatch ();
}
