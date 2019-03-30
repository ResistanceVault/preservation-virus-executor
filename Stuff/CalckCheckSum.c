#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>

ULONG sum;

VOID main (VOID)
{
	BPTR fh;
	UWORD *Buffer = 0;
	int size;
	ULONG i;
	ULONG tmp = 0x99EF090B;
	ULONG tmp1 = 0x65CD87AA;

	size = FileSize (VE);
	if (size > 0)
	{
		if (Buffer = AllocMem (size, MEMF_CLEAR))
		{
			if (fh = Open (VE, MODE_OLDFILE))
			{
				Read (fh, Buffer, size);
				Close (fh);
				for (i = 0; i < size / 2; i++)
				{
					if (i < size - 4)
					{
						if (Buffer[i] == 0x99EF)
						if (Buffer[i + 1] == 0x090B)
						if (Buffer[i + 2] == 0x2F7C)
						if (Buffer[i + 3] == 0x65CD)
						if (Buffer[i + 4] == 0x87AA)
						{
							i += 200;
							continue;
							printf ("Found tmp start! %ld\n", i);
						}		
					}
					ve_checksum += Buffer[i] / (0x9A + 126) - (i * 3);
				}
			}
			FreeMem (Buffer, size);
		}
	}
	printf ("\n Checksum = %08lX\n\n", sum);
}
