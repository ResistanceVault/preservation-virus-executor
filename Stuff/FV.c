#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>

VOID main (VOID)
{
	LONG size, i;
	BOOL start = FALSE;
	UBYTE *Buf = 0;
	BPTR fh;

	size = FileSize ("dl:test/1");
	if (size > 0)
	{
		char Hold[20];

		Buf = AllocMem (size, MEMF_PUBLIC);
		if (fh = Open ("dl:test/1", MODE_OLDFILE))
		{
			Read (fh, Buf, size);
			Close (fh);
		}

		for (i = 0; i < size; i++)
		{
			if (i < (size - 10))
			{
				if (Buf[i] == '(')
				if (Buf[i+1] == 'a')
				if (Buf[i+2] == 'p')
				if (Buf[i+3] == 'p')
				if (Buf[i+4] == 'e')
				if (Buf[i+5] == 'n')
				if (Buf[i+6] == 'd')
				if (Buf[i+7] == ' ')
				if (Buf[i+8] == '"')
				{
					i += 9;
					if (Buf[i] == '\\')
					if (Buf[i+1] == '0')
					if (Buf[i+2] == '\\')
					if (Buf[i+3] == '0')
					if (Buf[i+4] == '\\')
					if (Buf[i+5] == 3)
					if (Buf[i+6] == '\\')
					if (Buf[i+7] == 243)
					{
						printf ("Found! %ld\n", Buf[i+7]);
						flag = TRUE;
						break;
					}
				}
			}
		}
	}
	FreeMem (Buf, size);
}
