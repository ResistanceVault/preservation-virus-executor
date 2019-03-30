;/*
sc5 -j73 -q5e -v MemPtr
slink LIB:c.o+"MemPtr.o" to MemPtr LIB LIB:sc.lib LIB:JEO.lib LIB:amiga.lib
copy MemPtr c:
quit
*/

#include <JEO:JEO.h>
#include <exec/execbase.h>
#include <proto/exec.h>
#include <exec/memory.h>

extern struct ExecBase *SysBase;

BOOL CheckMemList (ULONG add);

ULONG mem_address[30];
UBYTE max_mem = 0;

struct
{
  WORD off;
  ULONG id;
  char Name[19];

} MemPtrList[] =
  {
//		  0,	0x0a7e58b8,								"ROMUpdate",

    357,	MakeID ('I','D','E','-'), "IDE-fix",
    365,	MakeID ('I','D','E','-'), "IDE-fix",
     72,	MakeID ('T','u','r','b'), "TurboVal",
    -26,	MakeID ('r','a','m','d'), "ramdrive.unit",
     24,	MakeID ('M','i','a','m'), "Miami",
     30,	MakeID ('D','e','l','u'), "Miami Deluxe",
     28,	MakeID ('A','m','i','g'), "AmigaOS ROM Update",
     32,	MakeID ('A','m','i','g'), "AmigaOS ROM Update",
     40,	MakeID ('A','m','i','g'), "AmigaOS ROM Update",
     44,	MakeID ('A','m','i','g'), "AmigaOS ROM Update",
     48,	MakeID ('A','m','i','g'), "AmigaOS ROM Update",
     52,	MakeID ('A','m','i','g'), "AmigaOS ROM Update",
     62,	MakeID ('S','o','f','t'), "SoftSCSI",
     -8,	MakeID ('F','E','0','2'), "FastExec",
    -53,	MakeID ('B','o','o','t'), "BootPrefs",
     32,	MakeID ('P','r','e','p'), "PrepareEmul",
     27,	MakeID ('C','F','i','x'), "BPPCFix",
     76,	MakeID ('W','a','r','m'), "Reserve Warm",
    340,	MakeID ('t','R','A','D'), "FastRAD",
    811,	MakeID ('A','f','t','e'), "BootControl",
   -130,	MakeID ('B','o','o','t'), "BootControl",
		 64, 	MakeID ('O','X','Y','P'),	"OXYPatchPortInit",
		 76,	MakeID ('M','u','M','o'), "MuMove4K",
		 24,	MakeID ('«',' ','L','o'), "LoadModule",
	   34,  MakeID ('C','O','P',0), 	"COP",
	   51,  MakeID ('0','0','_','H'), "A1200_HDPatch",
		 28,  0x00000c30, 							"Enhanced Alert H.",
		 (WORD)0x8000,0,""
 	};

BOOL Check_mem_known (ULONG ptr, char *Name)
{
	UBYTE *off;
	ULONG *p;
	UWORD i;

	for (i = 0; MemPtrList[i].off != (WORD)0x8000; i++)
	{
		off = (char *)ptr;
		off += MemPtrList[i].off;

		if (CheckMemList ((ULONG)off))
		{
			p = (ULONG *)off;
			if (*p == MemPtrList[i].id)
			{
				sprintf (Name, "%-18s  ", MemPtrList[i].Name);
				return (TRUE);
			}
		}
	}
	sprintf (Name, "%-18s  ", "*** UNKNOWN ***");
	return (FALSE);
}

VOID FindMemList (VOID)
{
	struct MemHeader *mh;
	ULONG end;

	max_mem = 0;
	Forbid ();
	mh = (struct MemHeader*)SysBase->MemList.lh_Head;

	Forbid ();
	for (mh = (struct MemHeader*)SysBase->MemList.lh_Head; mh->mh_Node.ln_Succ != NULL; mh = (struct MemHeader*)mh->mh_Node.ln_Succ)
  {
		end = (ULONG)mh->mh_Upper - (ULONG)mh->mh_Lower + 31 + (ULONG)mh;
		mem_address[max_mem] = (ULONG)mh;
		max_mem++;
		mem_address[max_mem] = end;
		max_mem++;
	}
	Permit ();
}

BOOL CheckMemList (ULONG add)
{
	UBYTE i;
	
	for (i = 0; i < max_mem; i += 2)
	{
		if (add >= mem_address[i] AND add <= mem_address[i+1])
			return (TRUE);
	}
	return (FALSE);
}

main ()
{
	ULONG *ptr;
	UBYTE *p_tmp;
	ULONG val, new_val;
	int i, j;
	char Hold[100];
	char Tmp[50];
	BOOL known;
	char String[256];
	struct Node *node;
	char *Dummy;
	char Name[500];

	Dummy = AllocMem (50000, MEMF_CLEAR);

	FindMemList ();
	Forbid();
	node = SysBase->KickMemPtr;
	if (node)
	{
		char Tmp[256];
		UWORD i;

		printf ("\n-----------------------------------------------------------\n\n");
		while (node)
		{
			i = 0;
			Name[0] = '(';
			if (node->ln_Name)
			{
				for (i = 0; node->ln_Name[i] != 0; i++)
				{
					if (node->ln_Name[i] == 13)
						break;
					Name[i + 1] = node->ln_Name[i];
				}
			}
			Name[i + 1] = ')';
			Name[i + 2] = 0;

			SPrintF (Hold, "$%08lx: ", node);
			known = Check_mem_known ((ULONG)node, Tmp);

			strcat (Dummy, Hold);
			strcat (Dummy, Tmp);
			strcat (Dummy, Name);
			strcat (Dummy, "\n");

			printf ("%s%s%s\n", Hold, Tmp, Name);
			if (!known)
			{
				int i, j;
				char *p;
				BOOL flag = FALSE;

				printf (" Enter search string (8 char, case sensitiv): ");
				gets (String);

				for (j = (ULONG)node - 200; j < (ULONG)node + 1000; j++)
				{
					p = (char *)j;
					flag = TRUE;
					for (i = 0; i < strlen (String); i++)
					{
						if (String[i] != *p)
						{
							flag = FALSE;
							break;
						}
						p++;
					}
					if (flag)
						break;
				}
				if (flag)
					printf ("\n Found '%s' at %ld\n\n", String, j - (ULONG)node);
				else
					printf ("\n Not found!\n\n");
			}
			
			node = node->ln_Succ;

			printf ("-----------------------------------------------------------\n\n");
		}
	}
	else
		strcpy (Dummy, "No entries.");

	Permit();
	FreeMem (Dummy, 50000);
}
