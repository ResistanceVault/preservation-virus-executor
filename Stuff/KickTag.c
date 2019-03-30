;/*
sc5 -j73 -v KickTag
slink LIB:c.o+"KickTag.o" to KickTag LIB LIB:sc.lib LIB:JEO.lib LIB:amiga.lib
copy Kicktag c:
quit
*/

#include <JEO:JEO.h>
#include <exec/execbase.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>

extern struct ExecBase *SysBase;

char Dummy[500];

BOOL CheckMemList (ULONG add);

ULONG mem_address[30];
UBYTE max_mem = 0;

#define MakeID(a,b,c,d) ((ULONG)(a)<<24 | (LONG)(b)<<16 | (LONG)(c)<<8 | (LONG)(d))

// ****************************** TAG ***********************************

char ROMUpdate[]	 			= "AmigaOS ROM Update";
char IDEfix[] 				 	= "IDE-fix";
char TurboVal[]  				= "TurboVal";
char RamDrive[]  				= "ramdrive.unit";
char Miami[]				 		= "Miami";
char MiamiDeluxe[]			= "Miami DeLuxe";
char SoftSCSI[]					= "SoftSCSI";
char FastExec[]					= "FastExec";
char MemoryKicktag[]		= "Memory Kicktag";
char BootPrefs[]				= "BootPrefs";
char PrepareEmul[]			= "PrepareEmul";
char BPPCFix[]					= "BPPCFix";
char ReserveWarm[]			= "Reserve Warm";
char FastRAD[]					= "FastRAD";
char BootControl[]			= "BootControl";
char OXYPatchPortInit[]	= "OXYPatchPortInit";
char MuMove4K[]					= "MuMove4K";
char LoadModule[]				= "LoadModule";
char COP[]							= "COP";
char A1200_HDPatch[]		= "A1200_HDPatch";

struct
{
  WORD off;
  ULONG id;
  char *Name;

} TagPtrList[] =
  {
		 10, 0x012C00CE,							ROMUpdate,
     10, 0x01020070, 							ROMUpdate,
     10, 0x022c0070, 							ROMUpdate,
     10, 0x012c0068, 							ROMUpdate,
     10, 0x012c006a, 							ROMUpdate,
     10, 0x022D0969, 							ROMUpdate,
		 10, 0x002d0086, 							ROMUpdate,
		 10, 0x812c0305, 							ROMUpdate,
		 10, 0x002c0085, 							ROMUpdate,
		 10, 0x012b030a, 							ROMUpdate,
		 10, 0x012d0850, 							ROMUpdate,
		 10, 0x002d00af, 							ROMUpdate,
		 10, 0x012c00dd, 							ROMUpdate,
		 10, 0x012d00c9, 							ROMUpdate,
		 10, 0x01010008, 							FastRAD,
		 10, 0x01280468,							MuMove4K,
		 10, 0x0128047f,							MuMove4K,
		 10, 0x01010068,							A1200_HDPatch,
    383, MakeID ('I','D','E','-'), IDEfix,
    391, MakeID ('I','D','E','-'), IDEfix,
     40, MakeID ('T','u','r','b'), TurboVal,
     26, MakeID ('r','a','m','d'), RamDrive,
     47, MakeID ('o','r','y',' '), MemoryKicktag,
     26, MakeID ('S','o','f','t'), SoftSCSI,
    -44, MakeID ('F','E','0','2'), FastExec,
    -11, MakeID ('B','o','o','t'), BootPrefs,
     58, MakeID ('P','r','e','p'), PrepareEmul,
     53, MakeID ('C','F','i','x'), BPPCFix,
     44, MakeID ('W','a','r','m'), ReserveWarm,
    837, MakeID ('A','f','t','e'), BootControl,
   -104, MakeID ('B','o','o','t'), BootControl,
		 26, MakeID ('O','X','Y','P'), OXYPatchPortInit,
		 64, MakeID ('a','d','M','o'), LoadModule,
	  106, MakeID ('C','O','P',0), 	 COP,
		 10, 0x01280007, 							"Enhanced Alert Hook",
	   (WORD)0x8000,  NULL, NULL
 	};

BOOL Check_tag_known (ULONG ptr, char *Name)
{
	char *off;
	ULONG *p;
	UWORD i;


	// SJEKKE RAM!!!!!!!!!!!!!!!!!!!
	for (i = 0; ; i++)
	{
		if (TagPtrList[i].off == 0x8000)
			break;
		off = (char *)ptr;
		off += TagPtrList[i].off;

		if (CheckMemList ((ULONG)off))
		{
			p = (ULONG *)off;
			if (*p == TagPtrList[i].id)
			{
				sprintf (Name, "%-18s  ", TagPtrList[i].Name);
				return (TRUE);
			}
		}
	}

	off = (char *)ptr;
	off += 10;
	p = (ULONG *)off;

	sprintf (Name, "%-18s 10, 0x%08lx ", "*** UNKNOWN ***", *p);
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

	FindMemList ();
	ptr = SysBase->KickTagPtr;
	if (ptr)
	{
		printf ("\n-----------------------------------------------------------\n\n");
		Dummy[0] = 0;
		FOREVER
		{
			known = FALSE;
			Dummy[0] = 0;

			if (*ptr == 0)
				break;
			val = (ULONG)*ptr;						// Hva peker ptr på?
			new_val = Bclr (val, 31);			// Tar vekk bitt 31
			if (val != new_val)						// Tag???		
				ptr = (ULONG *)new_val;

			sprintf (Hold, "%08lX: ", *ptr);
			known = Check_tag_known (*ptr, Tmp);

			strcat (Dummy, Hold);
			strcat (Dummy, Tmp);

			p_tmp = (UBYTE *)*ptr;
			p_tmp += 18;
			CopyMem ((APTR)p_tmp, &val, 4);
			p_tmp = (UBYTE *)val;
			for (i = 0, j = 0;; i++, j++)	// 0x0D 0x0A 0x00
			{
				if (*p_tmp <= 0x0D)
					break;
				if (j > 40)
					break;

				Hold[j] = *p_tmp;
				p_tmp++;
			}
			Hold[j++] = '\n';
			Hold[j++] = 0;
			strcat (Dummy, Hold);
			printf (" %s", Dummy);

			if (!known)
			{
				int i, j;
				char *p;
				BOOL flag = FALSE;

				printf (" Enter search string (8 char, case sensitiv): ");
				gets (String);

				for (j = (ULONG)*ptr; j < (ULONG)*ptr + 1000; j++)
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
					printf ("\n Found '%s' at %ld\n\n", String, j - (ULONG)*ptr);
				else
					printf ("\n Not found!\n\n");
			}

			ptr++;
			printf ("-----------------------------------------------------------\n\n");
		}
	}
	else
		strcpy (Dummy, "No entries.");
}
