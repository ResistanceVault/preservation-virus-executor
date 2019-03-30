;/*
Delete td.o quiet
sc5 -j73 -v -q5e td
slink LIB:c.o+"td.o" to td LIB LIB:sc.lib LIB:JEO.lib
copy td.c -V:td.c
copy td c:
quit
*/
 
#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>

#define MAXRELOC 100

ULONG *Buf = 0;
LONG size;

VOID CleanUp (UBYTE *Ret)
{
	if (Ret[0])
		printf ("\n  %s\n\n", Ret);

  if (Buf)
    FreeMem (Buf, size);

	exit (0);
}

VOID main (int argc, char **argv)
{
	UBYTE Name[256];
  struct FileHandle *fh;
  ULONG i, start, h;
  UWORD *Buf1 = 0;

	if (argc != 2)
		CleanUp ("Wrong arguments");

	strcpy (Name, argv[1]);
  size = FileSize (Name);
  if (size == ERROR OR size == 0)	/* file not found or empty file */
    CleanUp ("File not found or file is empty");

  if (Buf = AllocMem (size, NULL))
	{
	  if ((fh = (struct FileHandle *)Open (Name, MODE_OLDFILE)) != NULL)
  	{
	    Read ((BPTR)fh, Buf, size);
	    Buf1 = (UWORD *)Buf;
  	  Close ((BPTR)fh);
		  if (Buf[0] == 0x000003F3)	// Executable
		  {
				h = Buf[2];
				printf ("%ld hunks in this file\n", h);
			  start = ((6 + h) + 1);

				for (i = 0; i < start; i++)
					printf ("%2ld: %08lx %ld\n", i, Buf[i], Buf[i]);

				printf ("start of first hunk: %ld\n", start);			  
				for (i = start; i < start + 10; i++)
					printf ("%2ld: %08lx\n", i, Buf[i]);
			}
			else
				CleanUp ("Not an executable file");
		}
		else
			CleanUp ("Error reading file");
	}
	else
		CleanUp ("Not enough memory");

	CleanUp ("");
}
