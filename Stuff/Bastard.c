#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>

main (int argc, char **argv)
{
	BPTR fh;
	ULONG i, start;
	LONG size;
	UWORD *Buf;
	BOOL flag = FALSE;

	if (argc == 2)
	{
		if (fh = Open (argv[1], MODE_OLDFILE))
		{
			size = FileSize (argv[1]);
			if (size > 800)
			{
				if (Buf = AllocMem (size, MEMF_CLEAR))
				{
					ULONG code_size = 0;

					Read (fh, Buf, size);
					for (i = 0; i < 200; i++)
					{
						if (Buf[i] == 0x0000 AND Buf[i+1] == 0x03e9)	// Code hunk
						{
							i += 2;
							code_size += Buf[i] * 0x10000;
							code_size += Buf[i+1];
							printf ("%08lx\n", code_size);
							i += 2;
							if (Buf[i] == 0x6100)	// Jump
							{
								i++;
								start = i + Buf[i] / 2;
								if (start < size - 100)
								{
									printf ("%04lx%04lx\n", Buf[start], Buf[start+1]);
									if (Buf[start] == 0x48e7 AND Buf[start+1] == 0xfffe)
									{
										start += 2;
										if (Buf[start] == 0xe040 AND Buf[++start] == 0xe340 AND Buf[++start] == 0xe442 AND Buf[++start] == 0x7874) 
										{
											start += 11;
											if (Buf[start] == 0x41fa AND Buf[start+2] == 0x41e8) 
											{
												start += 22;
												if (Buf[start] == 0x2c78 AND Buf[++start] == 0x0004)	// move.l 4,a6
												{
													start += 17;
													if (Buf[start] == 0x4eae AND Buf[++start] == 0xfd84)	// jsr
													{
														flag = TRUE;
														break;
													}
												}
											}
										}
									}
								}
							}
						}
					}
					FreeMem (Buf, size);
				}
			}
			Close (fh);
		}
	}
	if (flag)
		printf ("Found!\n");
}
