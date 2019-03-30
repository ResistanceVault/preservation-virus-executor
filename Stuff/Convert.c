;/*
sc5 -j73 -v Convert
slink LIB:c.o+"Convert.o" to Convert LIB LIB:sc.lib LIB:reqtools.lib LIB:JEO.lib
quit
*/

#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>

char *Buffer = 0;
int size;

typedef struct
{
	WORD function;
	char Offset[10];
	char Data[10];
	char Name[50];
	
} VEPatch;

VEPatch patch;

//  InsertPatch (INT_LIB, -1056, 0, 0x48e70022,				ve_SetPatchMsg);

char Dummy[500];

VOID Make_C (VOID)
{
	sprintf (Dummy, "  InsertPatch (INT_LIB, %ld, %s, %s, %s);\n", 
		patch.function , patch.Offset, patch.Data, patch.Name);
	printf ("%s", Dummy);
}

VOID Convert (VOID)
{
	int i, j;
	char Hold[80];
	WORD line;

	line = 1;
	for (i = 0, j = 0; i < size; i++)
	{
		Hold[j] = Buffer[i];
		if (Hold[j] == '\n')	// SLutt på linja
		{
			Hold[j] = 0;
			j = 0;
			
			switch (line)
			{
				case 1: patch.function = atoi (Hold); break;
				case 2:	strcpy (patch.Offset, Hold); break;
				case 3:	strcpy (patch.Data, Hold); break;
				case 4:	strcpy (patch.Name, Hold); break;
				case 5: Make_C (); break;
			}
			line++;
			if (line > 5)	// Begynne på nytt_
				line = 1;
		}
		else
			j++;
	}
}

main ()
{
	char FileName[] = "-v:-INT.data";
	BPTR fh;

	size = FileSize (FileName);
	if (size > 0)
	{
		if (Buffer = AllocMem (size, MEMF_CLEAR))
		{
			if (fh = Open (FileName, MODE_OLDFILE))
			{
				Read (fh, Buffer, size);
				Close (fh);

				Convert ();
			}
			FreeMem (Buffer, size);
		}
	}
}
