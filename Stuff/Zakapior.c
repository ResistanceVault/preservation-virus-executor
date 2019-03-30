UWORD ZakapiorData[20] =
{
	0x610C,
	0x646F,
	0x732E,
	0x6C69,
	0x6272,
	0x6172,
	0x7900,
	0x225F,
	0x7025,
	0x4EAE,
	0xFDD8,
	0x2C00,
	0x6744,
	0x41FA,
	0x01DC,
	0x2086,
	0x283C,
	0x0000,
	0x0196,
	0x7200
};

BOOL CheckZakapior (UWORD *Buffer, LONG size)
{
	int i;
	UWORD val;

	if (size > 500)	// Zakapior
	{
		for (i = 0; i < (size - 500) / 2; i++)
		{
			if (i > 10000)	// Skal vi gidde mere?
				break;
			if (Buffer[i] == 0x48e7)
			if (Buffer[i+1] == 0xfffe)
			if (Buffer[i+2] == 0x2c78)
			if (Buffer[i+3] == 0x0004)
			if (Buffer[i+8] == 0x303c)
			{
				val = Buffer[i+9];
				if (Buffer[i+10] == 0xb158)
				if (Buffer[i+11] == 0xb3c8)
				if (Buffer[i+12] == 0x66fa)
				if (Buffer[i+13] == 0x4eae)
				if (Buffer[i+14] == 0xfd84)
				{
					int j, k;
					BOOL z_flag = FALSE;

					for (j = i+15, k = 0; j < i+15+20; j++, k++)
					{
						if (Buffer[j] == (val ^ ZakapiorData[k]))
							z_flag = TRUE;
						else
							break;
					}
					if (z_flag)
						return (TRUE);
				}
			}
		}
	}
	return (FALSE);
}
