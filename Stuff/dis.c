#include <JEO:JEO.h>
#include <ctype.h>
#include <proto/exec.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <proto/disassembler.h>

struct DisassemblerBase *DisassemblerBase;
struct DisData *disData;

VOID CleanUp (VOID)
{
	if (DisassemblerBase)
		CloseLibrary ((struct Library *)DisassemblerBase);
	if (disData)
		FreeMem (disData, sizeof (struct DisData));
	exit (0);
}

char MonHold[100];
UBYTE monCount;

__asm far VOID AsmOut (register __d0 UBYTE ch, register __a2 APTR data)
{
	if (ch == '\n')
		disData->ds_UpTo = data;	// Stop!
	MonHold[monCount++] = ch;
}

VOID DoOutput (char *To, char *From)
{
	UBYTE i, j;

	memset (To, ' ', 80);

	To[0] = '$';						// Address
	for (i = 1; i < 9; i++)
		To[i] = From[i-1];
	To[i] = ':';

	for (i = 11, j = 11; i < 27; i++)	// Hex
	{
		if (From[i+1] == ' ')
			continue;
		else
		{
			To[j] = From[i+1];
			j++;
		}
	}

	for (i = 39; i < 50; i++)
	{
		To[i] = From[i];
		if (From[i] == ' ')
			break;
	}

	for (i = i+1, j = 48; From[i] != 0; i++)
	{
		To[j] = From[i];
		j++;
		if (From[i] == ' ')
			break;
	}

	To[74] = 0;
	JEOtoupper (To);
}

main ()
{
	APTR next;
	ULONG i;
	char Out[100];

	if (!(DisassemblerBase = (struct DisassemblerBase *)OpenLibrary (DISASSEMBLER_NAME, 0)))
		printf ("Error Opening\n");

	if (!(disData = AllocMem (sizeof (struct DisData), MEMF_PUBLIC)))
		printf ("Error alloc\n");

	
	disData->ds_From = (APTR)0x0a7efb6a;
	disData->ds_UpTo = (APTR)(0x0a7efb6a + 100);
	disData->ds_PutProc = (VOID *)AsmOut;
	disData->ds_reserved = NULL;

	for (i = 0; i < 16; i++)
	{
		monCount = 0;
		next = Disassemble (disData);
		MonHold[monCount-1] = 0;
		disData->ds_From = (APTR)next;
		disData->ds_UpTo = (APTR)((ULONG)next + 10);
		DoOutput (Out, MonHold);
		printf ("%s\n", MonHold);
	}

	CleanUp ();
}
