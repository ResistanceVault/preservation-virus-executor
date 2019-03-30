#include <JEO:JEO.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <exec/memory.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <time.h>
#include <exec/lists.h>
#include <JEO:raw.h>
#include <JEO:virus.h>
#include "VE_Prototypes.h"
#include "VE_Window.h"
#include <ctype.h>

#include <proto/xvs.h>
#include <proto/xfdmaster.h>

extern struct xvsFileInfo *fileinfo;
extern struct ExecBase *SysBase;

#define ARC_PATH_SIZE	12
#define DONE	-5
#define MAX_DEPTH	500

extern ULONG dirCount;
extern ULONG fileCount;

BPTR far p_lock[MAX_DEPTH];

ULONG p_dir_depth;
ULONG p_cur;
ULONG p_max_depth;

VOID p_fib_Close (VOID);

fib_Sjef p_fib_sjef;
ULONG p_fib_len;

//*************************************************************************************

VOID p_fib_SlettBase (VOID)
{
  ULONG s;
  struct Node *nd;

  nd = p_fib_sjef.list.lh_Head;
  for (s = 0; s < p_fib_len; s++)
  {
    Remove (nd);
    FreeMem ((UBYTE *)nd, sizeof (Filbase));
    nd = 0;
    nd = p_fib_sjef.list.lh_Head;
  }
  p_fib_len = 0;
}

VOID p_fib_Close (VOID)
{
  Filbase *bp;

  p_fib_SlettBase ();
  bp = (Filbase *)p_fib_sjef.list.lh_Head;
  while (bp = (Filbase *)RemHead (&p_fib_sjef.list))
  {
    FreeMem (bp, sizeof (Filbase));
    bp = 0;
  }
}

struct Node *VE_p_FindTheOne (UBYTE *Name)	// Legger den alfabetisk i bufferen
{
  struct Node *nd;

  nd = p_fib_sjef.list.lh_Head;
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

VOID p_fib_Rename (UBYTE *Name)
{
  struct Node *nd;
  Filbase *bp = NULL;

  nd = VE_p_FindTheOne (Name);
  nd = nd->ln_Succ;
  bp = (Filbase *)nd;

  strcpy (bp->FileName, Name);
  bp->nd.ln_Name = bp->FileName;
}

VOID p_fib_Insett (UBYTE *Dir, UBYTE *Name, LONG size)
{
  Filbase *bp = NULL;
  struct Node *nd;

  if (bp = (Filbase *)AllocMem (sizeof (Filbase), MEMF_CLEAR))
  {
	 	if ((Dir[strlen (Dir) - 1] != ':') AND (Dir[strlen (Dir) - 1] != '/'))
	  	strcat (Dir, "/");

    sprintf (bp->FileName, "%s%s", Dir, Name);
    bp->size = size;

    bp->nd.ln_Name = Name;
    if (!(nd = VE_p_FindTheOne (Name)))
      AddTail (&p_fib_sjef.list, &bp->nd);
    else
      Insert (&p_fib_sjef.list, &bp->nd, nd);

    p_fib_len++;
  }
}

VOID p_fib_Open (VOID)
{
  p_fib_len = 0;
  NewList (&p_fib_sjef.list);
}

BOOL p_breakFlag;

BOOL p_CheckDirectory (VOID)
{
  ULONG fc;
  struct Node *nd;
  Filbase *bp;
  BOOL ret = TRUE;

  fc = 0;
  nd = p_fib_sjef.list.lh_Head;
  bp = (Filbase *)nd;
  while (nd->ln_Succ)
  {
		ret = Check_file_intuition ();
		if (ret == FALSE)
			goto end;
/*
    if (!Check (bp->FileName, bp->size))
 	  {
   		ret = FALSE;
   		goto end;
    }
*/
    nd = nd->ln_Succ;
    bp = (Filbase *)nd;
    fc++;
  }
end:
  p_fib_SlettBase ();	// Sletter filene i dirbuffer!

  return (ret);
}

LONG p_FindAvailDepth (VOID)
{
	int i;

	for (i = 0; i < MAX_DEPTH; i++)
	{
		if (p_lock[i] == DONE)
			return i;			// Vi bruker neste som er ledig...
	}
	return (ERROR);
}

BOOL p_ScanDirectory (char *Dir)
{
	char Name[300];
  struct FileInfoBlock *fileinfoblock = 0;
  ULONG success;
  int i;
  BOOL go_flag;
  BOOL ret = TRUE;

  p_fib_Open ();

	pack_checking = ON;
  fileinfoblock = (struct FileInfoBlock *) AllocMem (sizeof (struct FileInfoBlock), MEMF_CLEAR);
	p_max_depth = 0;
	p_dir_depth = 0;

	for (i = 0; i < MAX_DEPTH; i++)
		p_lock[i] = DONE;

	go_flag = TRUE;

	if (p_lock[p_dir_depth] = Lock (Dir, ACCESS_READ))
	{
		while (go_flag)
		{
			go_flag = FALSE;
			for (i = 0; i < MAX_DEPTH; i++)
			{
				if (p_lock[i] != DONE)	// Ikke ferdig?
				{
					p_cur = i;
					go_flag = TRUE;
					break;
				}
			}
			if (go_flag)	// Har vi flere dirs og sjekke?
			{
		  	PrintTime ();
	  		if (success = Examine (p_lock[p_cur], fileinfoblock))
 				{
		  		NameFromLock (p_lock[p_cur], Name, 300);	// Vi finner dirnavnet!
					for (;;)
					{
						ret = Check_file_intuition ();
						if (ret == FALSE)
							goto end;

						success = ExNext (p_lock[p_cur], fileinfoblock);	// FindNext File/Dir
  				  if (!success)				// End of directory?
	  		  	{
  			  		UnLock (p_lock[p_cur]);
  			  		p_lock[p_cur] = DONE;
							if (!p_CheckDirectory ())
							{
								ret = FALSE;
								goto end;
							}
			  			break;
		   			}
			  	  if (fileinfoblock->fib_DirEntryType < 0)	// A file?
			    	{
	  		  		fileCount++;
				      p_fib_Insett (Name, fileinfoblock->fib_FileName, fileinfoblock->fib_Size);
  		  		}
		 			  else
  				  {
  				  	char New_dir[300];

  			  		dirCount++;
  			  		NameFromLock (p_lock[p_cur], New_dir, 300);	// Vi får aldri med '/'
	 				  	if (New_dir[strlen (New_dir) - 1] != ':')
	  				  	strcat (New_dir, "/");
	 			  		strcat (New_dir, fileinfoblock->fib_FileName);
	
							p_dir_depth = p_FindAvailDepth ();
					    if ((p_dir_depth == MAX_DEPTH - 1) OR p_dir_depth == ERROR)
					    {
								JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERRORP500), GLS (&L_OK), S_ERROR);
								ret = FALSE;
								goto end;
							}	

  				  	if (p_dir_depth > p_max_depth)
  				  		p_max_depth = p_dir_depth;

	 		  			if (!(p_lock[p_dir_depth] = Lock (New_dir, ACCESS_READ)))
 			  			{
								PF ("ERROR 652!\n");
								p_lock[p_dir_depth] = DONE;
							}
						}
					}
  		  }
  		}
  	}
	}

end:
	pack_checking = OFF;

	for (i = 0; i < MAX_DEPTH; i++)	// Just in case
	{
		if (p_lock[i] > 0)
			UnLock (p_lock[i]);
	}

	FreeMem (fileinfoblock, sizeof (struct FileInfoBlock));
//	sprintf (Dummy, "Max dir_depth = %ld\n\n", p_max_depth);

  p_fib_Close ();

	return (ret);
}
