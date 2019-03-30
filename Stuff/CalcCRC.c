#include <JEO:JEO.h>

#define POLYNOMIAL 0xafc5e5f3

ULONG crc_table[256];

void main (VOID)
{
	int i, j;
	ULONG crc_accum;

	for (i = 0; i < 256;  i++)
	{
		crc_accum = ((ULONG)i << 24);
		for (j = 0; j < 8; j++)
		{
			if (crc_accum & 0x80000000)
				crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
			else
				crc_accum = (crc_accum << 1);
		}
		crc_table[i] = crc_accum;
		printf ("0x%08lX, ", crc_accum);
    if (!((i+1) % 8))
			printf ("\n");
	}
}
