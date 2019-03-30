#include <JEO:JEO.h>
#include <libraries/reqtools.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <-V:VE_Prototypes.h>

//*************************************************************************************

ext_Sjef ext_sjef;
ULONG ext_len = 0;

enum
{
	EXT_DATA = 0,
	EXT_NEVER
};

VOID ext_SlettBase (VOID)
{
  ULONG s;
  struct Node *nd;

  nd = ext_sjef.list.lh_Head;
  for (s = 0; s < ext_len; s++)
  {
    Remove (nd);
    FreeMem ((UBYTE *)nd, sizeof (Extbase));
    nd = 0;
    nd = ext_sjef.list.lh_Head;
  }
  ext_len = 0;
}

VOID ext_Close (VOID)
{
  Extbase *bp;

  ext_SlettBase ();
  bp = (Extbase *)ext_sjef.list.lh_Head;
  while (bp = (Extbase *)RemHead (&ext_sjef.list))
  {
    FreeMem (bp, sizeof (Extbase));
    bp = 0;
  }
}

struct Node *ext_FindTheOne (UBYTE *Name)
{
  struct Node *nd;

  nd = ext_sjef.list.lh_Head;
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

VOID ext_Insett (char *ExtName)
{
  Extbase *bp = NULL;
  struct Node *nd;

  if (bp = (Extbase *)AllocMem (sizeof (Extbase), MEMF_CLEAR))
  {
    strcpy (bp->Ext, ExtName);
    bp->nd.ln_Name = bp->Ext;

    if (!(nd = ext_FindTheOne (bp->Ext)))
      AddTail (&ext_sjef.list, &bp->nd);
    else
      Insert (&ext_sjef.list, &bp->nd, nd);

    ext_len++;
  }
}

VOID ext_Open (VOID)
{
  ext_len = 0;
  NewList (&ext_sjef.list);
}

BOOL CheckExt (char *FullName, char *FileName)
{
	int i, len;
	BOOL ext = FALSE;
	char Ext[50];
	BOOL ret = FALSE;

	if (prefs->checkData)	// Er vi i data modus?
		return (FALSE);	// Always check!

	len = strlen (FileName);

	if (len >= 3)
	{
		for (i = len - 2; i > 0; i--)
		{
			if (FileName[i] == '.')
			{
				ext = TRUE;
				break;
			}
		}
		if (ext)
		{
		  struct Node *nd;
		  Extbase *bp;

			i++;
			len = len - i;
			strncpy (Ext, &FileName[i], len);
			Ext[len] = 0;
			JEOtoupper (Ext);

		  nd = ext_sjef.list.lh_Head;
		  bp = (Extbase *)nd;
		  while (nd->ln_Succ)
		  {
				if (!(stricmp (bp->Ext, Ext)))
					return (TRUE);

		    nd = nd->ln_Succ;
		    bp = (Extbase *)nd;
		  }
		}
	}

	if (ext)	// Ikke funnet i lista...
	{
		ULONG tmpBuf[2];
		BPTR fh;

		if (fh = Open (FullName, MODE_OLDFILE))
		{
			printf ("Hebbe\n");
			if (Read (fh, &tmpBuf[0], 8) == 8)
			{
				if (tmpBuf[0] != 0x000003F3)	// Ikke en exe?
				{
					ext_Insett (Ext);						// Da legger vi den inn!
					printf ("Setter inn '%s'\n", Ext);
				}
			}
			Close (fh);
		}
	}
	return (ret);
}
