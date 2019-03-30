#define _DEBUG1 0

#include <JEO:JEO.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <exec/resident.h>
#include <libraries/reqtools.h>
#include <intuition/intuition.h>
#include <proto/reqtools.h>
#include <proto/dos.h>
#include "VE_Prototypes.h"
#include "VE_PatchOffsets.h"
#include <proto/xvs.h>
#include <ctype.h>

BOOL ValidMem (ULONG *ptr);

ULONG mem_address[30];	// 15!
UBYTE max_mem = 0;

extern struct ExecBase *SysBase;
extern BOOL filecheck_progress;
extern VEPatch far ve_patch[5000];
extern BOOL soft_ROM_flag;

#define X	73
#define Y	23

ULONG exec_check;

BOOL Check_tag_mem_known (ULONG ptr, char *Name, UBYTE type)
{
	UWORD i;

	if (pv[type].flag)	// Any patches here?
	{
		char *p_tmp;
		ULONG *val;

		for (i = pv[type].patch_start; i < pv[type].patch_end; i++)
		{
			p_tmp = (char *)ptr;
			if (ve_patch[i].offset != 0)
				p_tmp += ve_patch[i].offset;
			if (TypeOfMem (p_tmp))
			{
				val = (ULONG *)p_tmp;

				if (*val == ve_patch[i].data)
				{
					char Hold[20];

					mystrcpy (Hold, PatchNames[ve_patch[i].name_off]);
					Hold[18] = 0;
					SPrintF (Name, "%-18s  ", Hold);
					return (TRUE);
				}
			}
		}
	}

/*

	for (i = 0;; i++)
	{
		if (TagPtrList[i].off == 0x7fff)
			break;

		off = (char *)ptr;
		off += TagPtrList[i].off;

		if (TypeOfMem (off))
		{
			p = (ULONG *)off;
			if (*p == TagPtrList[i].id)
			{
				SPrintF (Name, "%-18s  ", TagPtrList[i].Name);
				return (TRUE);
			}
		}
	}
*/
	SPrintF (Name, "%-18s  ", GLS (&L_UNKNOWN_STARS));
	return (FALSE);
}

BOOL InROM (ULONG address)
{
	BOOL rom = TRUE;

	if ((address < hw_ROM_start OR address > hw_ROM_end) AND ((address != 0) AND (address != (ULONG)-1)))
		rom = FALSE;

	if (!rom AND exec_start)
	{
		rom = TRUE;
		if (address < exec_start OR address > exec_end)
			rom = FALSE;
	}

	return (rom);
}

/*
ULONG GetVBR (VOID)
{
  static const UWORD getvbr[3] = { 0x4E7A, 0x0801, 0x4E73 }; // movec vbr,d0 / rte 

  if (SysBase->AttnFlags & AFF_68010)
    return (ULONG) Supervisor((ULONG (* const)(void)) getvbr);
  else
    return NULL;
}
*/

VOID List_KickTagPtr (VOID)
{
	ULONG *ptr;
	UBYTE *p_tmp;
	ULONG val, new_val;
	char Hold[50], Hold1[50];
	char *Dummy = 0;
	UWORD *pw;
	ULONG *pl;
	ULONG start, end, size;
	char MyName[40];
	char ResName[100];
	struct Resident *r;

	Dummy = AllocMem (50000, MEMF_PUBLIC);
	strcpy (Dummy, "\nADDRESS    NAME                          ID-STRING\n\n");
	ptr = SysBase->KickTagPtr;
	if (*ptr)
	{
		ULONG i, j;
		BOOL name_flag;

		FOREVER
		{
			if (*ptr == 0)								// Slutt på lista...
				break;

			val = (ULONG)*ptr;						// Hva peker ptr på?
			new_val = Bclr (val, 31);			// Tar vekk bitt 31
			if (val != new_val)						// Tag???		
				ptr = (ULONG *)new_val;

			SPrintF (Hold, "$%08lx: ", *ptr);
			strcat (Dummy, Hold);

			name_flag = Check_tag_mem_known (*ptr, MyName, VE_TAG);

			pw = (UWORD *)*ptr;
			if (*pw++ == 0x4afc)	// ROM tag?
			{
				struct Node *node;
				char Hold[200];

				pl = (ULONG *)pw;
				start = (ULONG)*pl++;		
				end = (ULONG)*pl;
				size = end - start;

				printf ("%08lX - %08lX (%6ld)", start, end, size);
			}

			p_tmp = (UBYTE *)*ptr;
			p_tmp += 14;	// NAME!!!!!!!!!!!!!!
			if (p_tmp)
			{
				CopyMem ((APTR)p_tmp, &val, 4);
				p_tmp = (UBYTE *)val;
				for (i = 0, j = 0;; i++, j++)
				{
					if (i >= 29)
						break;

					if (*p_tmp <= 0x0D)
						break;

					Hold1[j] = *p_tmp++;
				}
				Hold1[j++] = 0;

				strcpy (ResName, Hold1);

				sprintf (Hold, "%-30s ", Hold1);
				printf (" %s ", Hold);
			}
			else
				strcpy (Hold, MyName);	// Copy from my own list or unknown...

			strcat (Dummy, Hold);

			p_tmp = (UBYTE *)*ptr;
			p_tmp += 18;	// NAME + VERSION ID !!!!!!!!!!!!!!
			if (p_tmp)
			{
				CopyMem ((APTR)p_tmp, &val, 4);
				p_tmp = (UBYTE *)val;
				for (i = 0, j = 0;; i++, j++)
				{
					if (j > 39)
						break;

					if (*p_tmp <= 0x0D)
						break;

					Hold[j] = *p_tmp++;
				}
				Hold[j++] = '\n';
				Hold[j++] = 0;

				printf (" %s ", Hold);
			}
			else
				strcpy (Hold, "\n");
			strcat (Dummy, Hold);


			if (r =  FindResident (ResName))
			{
				printf ("%08lX\n", r->rt_Init);
			}

			ptr++;
		}
		JEOSoundRequest (GLS (&L_KICKTAG_ENTRIES), Dummy, GLS (&L_OK), S_NO_SOUND);
	}
	FreeMem (Dummy, 50000);
}

VOID List_KickMemPtr (VOID)
{
	struct Node *node;
	char Hold[200];
	char *Dummy = 0;
	char Name[256];

	Dummy = AllocMem (50000, MEMF_PUBLIC);

	Forbid();
	node = SysBase->KickMemPtr;
	if (node)
	{
		char Tmp[256];
		UWORD i;

		strcpy (Dummy, GLS (&L_ADDRESS_NAME));
		while (node)
		{
			Name[0] = '(';
			i = 0;
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
			Check_tag_mem_known ((ULONG)node, Tmp, VE_MEM);

			strcat (Dummy, Hold);
			strcat (Dummy, Tmp);
			strcat (Dummy, Name);
			strcat (Dummy, "\n");
			node = node->ln_Succ;
		}
		JEOSoundRequest (GLS (&L_KICKMEM_ENTRIES), Dummy, GLS (&L_OK), S_NO_SOUND);
	}
	Permit();

	FreeMem (Dummy, 50000);
}

VOID RestoreVectors (VOID)
{
	BOOL memVirus;

  SysBase->WarmCapture  = 0;
  SysBase->KickCheckSum = 0;
  SysBase->KickMemPtr   = 0;
  SysBase->KickTagPtr   = 0;
  SysBase->CoolCapture  = 0;
	SysBase->ColdCapture  = 0;
  Delay (1);			// Wait for interrupt to refresh 

  vec->warm  = SysBase->WarmCapture;
  vec->check = SysBase->KickCheckSum;
  vec->mem   = SysBase->KickMemPtr;
  vec->tag   = SysBase->KickTagPtr;
  vec->cool  = SysBase->CoolCapture;
	vec->cold  = SysBase->ColdCapture;
	memVirus = FALSE;			// Hopfully 

  if (vec->warm != 0)  memVirus = TRUE;
  if (vec->check != 0) memVirus = TRUE;
  if (vec->mem != 0)   memVirus = TRUE;
  if (vec->tag != 0)   memVirus = TRUE;
  if (vec->cool != 0)  memVirus = TRUE;
  if (vec->cold != 0)  memVirus = TRUE;

  if (memVirus)
  {
		PlaySound (S_ERROR);
    if (rtEZRequestTags (GLS (&L_ERROR_CLR_VECTORS),
                         GLS (&L_REBOOT_CANCEL), NULL, NULL, RT_TextAttr, &topaz8,
	                       RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_WARNING), TAG_END))
      ColdReboot ();
		if (filecheck_progress)
			All (OFF);
  }
}

char KickMemPtr[] = "KickMemPtr";
char KickTagPtr[] = "KickTagPtr";

VOID CheckVectors (BYTE mode)
{
  BOOL memVirus = FALSE;
  BOOL tag_ptr = FALSE;
  BOOL mem_ptr = FALSE;
  UBYTE S1[35], S2[35], S3[35], S4[35], S5[35], S6[35];
	UWORD svar;
  BOOL go_flag = FALSE;
  UBYTE warm_cool_cold = 0;
  UWORD tag_count, mem_count;

//  vec->TrackIO = GetFunction ((struct Library *)deviceOffset, _TrackDiskIO);
//  SetFunction ((struct Library *)deviceOffset, _TrackDiskIO, (ULONG (*) ())rightTrackIO);

  vec->warm  = SysBase->WarmCapture;
  vec->check = SysBase->KickCheckSum;
  vec->mem   = SysBase->KickMemPtr;
  vec->tag   = SysBase->KickTagPtr;
  vec->cool  = SysBase->CoolCapture;
  vec->cold  = SysBase->ColdCapture;

	if (mode == 1)	// Skal vi vise uansett?
		go_flag = TRUE;
	else if (memcmp (vec, tmp_vec, sizeof (Vectors)))	// Forandringer?
		go_flag = TRUE;
	if (!go_flag)
		return;
	
  SPrintF (S1, "WarmCapture    = $%08lx ", vec->warm);
  SPrintF (S2, "KickCheckSum   = $%08lx ", vec->check);
  SPrintF (S3, "%s     = $%08lx ",KickMemPtr, vec->mem);
  SPrintF (S4, "%s     = $%08lx ",KickTagPtr, vec->tag);
  SPrintF (S5, "CoolCapture    = $%08lx ", vec->cool);
  SPrintF (S6, "ColdCapture    = $%08lx ", vec->cold);

  if (vec->warm != 0)  { warm_cool_cold++; memVirus = TRUE; strcat (S1, "<--"); } else strcat (S1, "   ");
  if (vec->check != 0 AND vec->check != (APTR)0xFFFFFFFF) { memVirus = TRUE; strcat (S2, "<--"); } else strcat (S2, "   ");
  if (vec->mem != 0)   { memVirus = TRUE; strcat (S3, "<--"); } else strcat (S3, "   ");
  if (vec->tag != 0)   { memVirus = TRUE; strcat (S4, "<--"); } else strcat (S4, "   ");
  if (vec->cool != 0)  { warm_cool_cold++; memVirus = TRUE; strcat (S5, "<--"); } else strcat (S5, "   ");
  if (vec->cold != 0)
  {
		ULONG *p;
		BOOL flag = FALSE;

		warm_cool_cold++;
		memVirus = TRUE;

		p = (ULONG *)vec->cold;

		if (ValidMem ((ULONG *)p))
		{
			if (*p == 0x61000414)
			{
				p++;
				if (*p == 0x20404ed0)	// Amiga OS Update
				{
					warm_cool_cold--;
					strcat (S6, "   ");
					flag = TRUE;
				}
			}

			if (!flag)
				strcat (S6, "<--");
		}
	}
	else
		strcat (S6, "   ");

  SPrintF (Dummy, "%s\n%s\n%s\n%s\n%s\n%s\n", S1, S2, S3, S4, S5, S6);

	tag_count = mem_count = 0;
  if (memVirus)
  {
		ULONG *ptr;
		ULONG val, new_val;
		struct Node *node;
		char Tmp[19];

		ptr = SysBase->KickTagPtr;
		if (ptr)
		{
			FOREVER
			{
				if (*ptr == 0)
					break;
				val = (ULONG)*ptr;						// Hva peker ptr på?
				new_val = Bclr (val, 31);			// Tar vekk bitt 31
				if (val != new_val)						// Tag???		
					ptr = (ULONG *)new_val;

				if (!Check_tag_mem_known (*ptr, Tmp, VE_TAG))
					tag_ptr = TRUE;

				tag_count++;
				ptr++;
			}
		}

//		Forbid();
		node = SysBase->KickMemPtr;
		while (node)
		{
			if (!Check_tag_mem_known ((ULONG)node, Tmp, VE_MEM))
				mem_ptr = TRUE;

			node = node->ln_Succ;
			mem_count++;
		}
//		Permit();

		if (memVirus)	// Går for alt!
		{
			char Hold[100] = "";
			char Choices[100] = "";

	  	if (tag_ptr)	// Noe resident som vi ikke kjenner?
		    SPrintF (Hold, "\n%s %s %s.", GLS (&L_UNKNOWN), GLS (&L_PROGRAMS_FOUND_IN), KickTagPtr);
			else if (tag_count)	// Er det noen her?
		    SPrintF (Hold, "\n%s %s %s.", GLS (&L_KNOWN), GLS (&L_PROGRAMS_FOUND_IN), KickTagPtr);
			if (*Hold)
				strcat (Dummy, Hold);

	  	if (mem_ptr)	// Her også???
		    SPrintF (Hold, "\n%s %s %s.", GLS (&L_UNKNOWN), GLS (&L_PROGRAMS_FOUND_IN), KickMemPtr);
		  else if (mem_count)	// Er det noen her?
		    SPrintF (Hold, "\n%s %s %s.", GLS (&L_KNOWN), GLS (&L_PROGRAMS_FOUND_IN), KickMemPtr);
			if (*Hold)
				strcat (Dummy, Hold);

			if (tag_count)
	  		strcat (Choices, GLS (&L_LIST_KICKTAGPTR));
			if (mem_count)
	  		strcat (Choices, GLS (&L_LIST_KICKMEMPTR));
	  	strcat (Choices, GLS (&L_RESTORE_VECS_CONTINUE));
	  	
		  if (warm_cool_cold)
		    strcat (Dummy, GLS (&L_IT_MIGHT_MEMVIRUS));

			if (mode != 1)	// Alltid vise alt!
			{
				if ((warm_cool_cold == FALSE) AND (tag_ptr == FALSE) AND (mem_ptr == FALSE))
					svar = FALSE;
				else
					svar = TRUE;
			}
			else
				svar = TRUE;


			if (svar)
			{
				if (mode == 0)
				{
				  if (warm_cool_cold)
						PlaySound (S_ERROR);
				  else
						PlaySound (S_MESSAGE);
				}
			}
	    while (svar)
  	  {
			  svar = (rtEZRequestTags (Dummy, Choices, NULL, NULL, RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_VECTOR_CHECK), RT_WaitPointer, TRUE, RTEZ_Flags, EZREQF_CENTERTEXT, RT_TextAttr, &topaz8, TAG_END));
				if (filecheck_progress)
					All (OFF);
	      if (svar)
  	    {
    	    switch (svar)
      	  {
  	        case 1:
  	        {
  	        	if (tag_count)
								List_KickTagPtr ();
  	        	else if (mem_count)
								List_KickMemPtr ();
							else
							{
								RestoreVectors ();
								svar = FALSE;
							}
							break;
						}
  	        case 2:
  	        {
  	        	if (tag_count)
								List_KickMemPtr ();
							else
							{
								RestoreVectors ();
								svar = FALSE;
							}
							break;
						}

        	  case 3:	// Null-stille
	          {
							RestoreVectors ();
							svar = FALSE;
	          }
    	    }
      	}
      }
	  }
	}

  if (!memVirus)	// Ikke virus??
  {
		if (mode == 1)	// Ikke virus, viser kun når velger fra menyen!
		{
  	  strcat (Dummy, GLS (&L_MEM_VEC_OK));
	  	rtEZRequestTags (Dummy, GLS (&L_OK), NULL, NULL, RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_VECTOR_CHECK), RT_WaitPointer, TRUE, RTEZ_Flags, EZREQF_CENTERTEXT, RT_TextAttr, &topaz8, TAG_END);
			if (filecheck_progress)
				All (OFF);
			List_KickMemPtr ();
		}
	}
	memcpy (tmp_vec, vec, sizeof (Vectors));
}

char ROMUpdateName[61] = "";

VOID Check_ROMUpdate (VOID)
{
	ULONG *ptr;
	UBYTE *p_tmp;
	ULONG val, new_val;
	char Hold[61];
	static BOOL checked = FALSE;

	if (checked)
		return;
	
	ptr = SysBase->KickTagPtr;
	if (ptr)
	{
		ULONG i, j;
		BOOL space = FALSE;
		BOOL known;

		FOREVER
		{
			space = known = FALSE;

			if (*ptr == 0)								// Slutt på lista...
				break;

			val = (ULONG)*ptr;						// Hva peker ptr på?
			new_val = Bclr (val, 31);			// Tar vekk bitt 31
			if (val != new_val)						// Tag???		
				ptr = (ULONG *)new_val;

			p_tmp = (UBYTE *)*ptr;
			p_tmp += 18;
			CopyMem ((APTR)p_tmp, &val, 4);
			p_tmp = (UBYTE *)val;
			for (i = 0, j = 0;; i++, j++)	// 0x0D 0x0A 0x00
			{
				if (j > 60)
					break;
				if (space)
				{
					Hold[j] = ' ';
					continue;
				}
				if (*p_tmp <= 0x0D)
					break;

				Hold[j] = *p_tmp;
				p_tmp++;
			}
			Hold[j++] = 0;
			if (!(strncmp (Hold, "ROMUpdate", 9)))
			{
				mystrcpy (ROMUpdateName, Hold);
				break;
			}
			ptr++;
		}
	}
	checked = TRUE;
}

typedef unsigned long (*FuncPtr)();

VOID CheckMemory (BYTE mode)
{
	static BOOL first_run = TRUE;
  ULONG count;
	struct xvsMemoryInfo *memoryinfo = 0;
	UBYTE i;
	char Hold[100];
	char Button[30];
	BYTE sound;

	if (aminet_flag)
  	return;

	if (exec_start)
	{
		MD5Data md5;

		md5 = Calculate_MD5 ((char *)exec_start, exec_size);
		if (strcmp (md5.md5, exec_md5.md5))	// Ops!
			JEOSoundRequest (GLS (&L_WARNING), GLS (&L_EXEC_MODIFIED), GLS (&L_OK), S_ERROR);
	}

	if (!xvsSelfTest ())
	{
		if (icon)
			UnIconify ();

		JEOSoundRequest (GLS (&L_WARNING), GLS (&L_XVS_MODIFIED), GLS (&L_OK), S_ERROR);
//		CleanUp ();
	}

 	memoryinfo = xvsAllocObject (XVSOBJ_MEMORYINFO);
	memoryinfo->xvsmi_Count = 10;
	count = xvsSurveyMemory (memoryinfo);
	if (count > 5)	// Max nr of viruses removed by xvs
	{
		if (icon)
			UnIconify ();

	  JEOSoundRequest (GLS (&L_WARNING), GLS (&L_XVS_DISABLED), GLS (&L_OK), S_ERROR);
	}

	if (soft_ROM_flag)
	{
	  SPrintF (Dummy, GLS (&L_KICKSTART_AT), ROMVersion, ROM_start);
		Dummy[strlen (Dummy) - 1] = 0;
	  SPrintF (Hold, GLS (&L_KICKSTART_AT), HW_ROMVersion, hw_ROM_start);
	  strcat (Dummy, Hold);
	}
	else
	  SPrintF (Dummy, GLS (&L_KICKSTART_AT), HW_ROMVersion, hw_ROM_start);

	if (ROM_version >= 40)
	{
		Check_ROMUpdate ();
		if (*ROMUpdateName)	// Fant vi en ROM update?
		{
			Dummy[strlen (Dummy) - 1] = 0;
			SPrintF (Hold, "%s\n\n", ROMUpdateName);
			strcat (Dummy, Hold);
		}
	}

	if (count > 0)
	{
		sound = S_VIRUS;
		
		if (icon)
			UnIconify ();
		mystrcpy (Button, GLS (&L_OK));
		if (count)
		{
			WriteToLog ("", LOG_DATE);
			for (i = 0; i < count; i++)
			{
				SPrintF (Hold, GLS (&L_VIRUS_DETECTED_IN_MEM), memoryinfo->xvsmi_NameArray[i]);
				WriteToLog (Hold, LOG_LINE_TEXT);
				strcat (Dummy, Hold);
			}
			WriteToLog ("", LOG_END);
		}
		SPrintF (Hold, GLS (&L_VIRUS_REMOVED_MEM));
		strcat (Dummy, Hold);
	}
	else
	{
		sound = S_NO_SOUND;
	  strcat (Dummy, GLS (&L_NO_KNOWN_VIRUS_MEM));
	  mystrcpy (Button, GLS (&L_GREAT));
	}

	if (sound == S_VIRUS OR mode)
	  JEOSoundRequest (GLS (&L_MEMORY_CHECK), Dummy, Button, sound);

	CheckVectors (mode);
	if (first_run)
	{
		All (OFF);
		first_run = FALSE;
		CheckPatches (mode);
		if (!filecheck_progress)
			All (ON);
	}
	else
		CheckPatches (mode);
	xvsFreeObject (memoryinfo);
}
