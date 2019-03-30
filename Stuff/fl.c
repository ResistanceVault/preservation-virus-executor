;/*
Delete FL.o quiet
sc5 -j73 -q5e -v FL
slink LIB:c.o+"-V:FL.o" to FL LIB LIB:sc.lib LIB:JEO.lib LIB:reqtools.lib LIB:amiga.lib VERBOSE
quit
*/

#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>

VOID Check_files (UWORD *Buf1, UWORD *Buf2, LONG size1, LONG size2, UWORD wordhits)
{
	int i, j, k, size;
	BOOL flag;
	ULONG start1, start2;
	ULONG *ptr1, *ptr2;

	if (size1 > 15000 AND size2 > 15000)
		size = 15000 / 2;
	else if (size1 > size2)
		size = size1 / 2;
	else
		size = size2 / 2;

	ptr1 = (ULONG *)Buf1;
	ptr2 = (ULONG *)Buf2;
  start1 = ((6 + ptr1[2]) + 1) * 2;
  start2 = ((6 + ptr2[2]) + 1) * 2;
	printf ("start1 = %3ld\nstart2 = %3ld\n\n", start1, start2);

	for (i = start1; i < (size - wordhits); i++)
	{
		for (j = start2; j < (size - wordhits); j++)
		{
			flag = TRUE;
			for (k = 0; k < wordhits; k++)
			{
				if (Buf1[i + k] != Buf2[j + k])
				{
					flag = FALSE;
					break;
				}
			}
			if (flag)
			{
				printf ("Found %ld hits at %04lx and at %04lx\n", wordhits, i, j);
				return;
			}
		}
	}
}

void main (int argc, char **argv)
{
	BPTR fh1 = 0, fh2 = 0;
	char *Buf1 = 0, *Buf2 = 0;
	LONG size1, size2;
	UWORD wordhits;

	if (argc == 4)
	{
		wordhits = atoi (argv[3]);
		if (wordhits > 0)
		{
			size1 = FileSize (argv[1]);
			size2 = FileSize (argv[2]);

			Buf1 = AllocMem (size1, MEMF_CLEAR | MEMF_PUBLIC);
			Buf2 = AllocMem (size2, MEMF_CLEAR | MEMF_PUBLIC);

			if (size1 > 0 AND size2 > 0)
			{
				if (fh1 = Open (argv[1], MODE_OLDFILE))
				{
					if (Read (fh1, Buf1, size1) == size1)
					{
						if (fh2 = Open (argv[2], MODE_OLDFILE))
						{
							if (Read (fh2, Buf2, size2) == size2)
							{
								printf ("Checking...\n");
								Check_files ((UWORD *)Buf1, (UWORD *)Buf2, size1, size2, wordhits);
							}
						}
					}
				}
			}
			else
				printf ("Error opening files\n");
		}
	}

	if (fh1)
		Close (fh1);
	if (fh2)
		Close (fh2);
	if (Buf1)
		FreeMem (Buf1, size1);
	if (Buf2)
		FreeMem (Buf2, size2);
}

