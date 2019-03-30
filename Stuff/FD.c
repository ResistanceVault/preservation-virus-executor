#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>

BOOL Find_FD_Name (char *LibName, WORD offset, char *FDName)
{
	BPTR fh;
	LONG size;
	char *Buf;
	ULONG i;
	char Hold[200];
	char FileName[108];
	BOOL flag = FALSE;

	strcpy (Hold, LibName);
	for (i = 0; Hold[i] != 0; i++)
	{
		if (Hold[i] == '.')
		{
			Hold[i] = '_';
			Hold[i+4] = 0;
			break;
		}			
	}

	sprintf (FileName, "FD:%s.fd", Hold);
	size = FileSize (FileName);
	if (size > 0)
	{
		if (Buf = AllocMem (size, MEMF_PUBLIC))
		{
			if (fh = Open (FileName, MODE_OLDFILE))
			{
				BOOL cmd = FALSE;
				BOOL skip = FALSE;
				char Cmd[50];
				UWORD j;
				WORD bias = 0;

				Read (fh, Buf, size);
				Close (fh);

				Hold[0] = j = 0;
				for (i = 0; i < size; i++)
				{
					if (skip)
					{
						if (Buf[i] == '\n')
						{
							skip = FALSE;
							j = 0;
						}
						else
							continue;
					}
					else if (cmd)
					{
						Cmd[j] = Buf[i];
						if (Cmd[j] == '\n')
						{
							Cmd[j] = 0;
							if (!(strnicmp (Cmd, "bias", 4)))
								bias = atoi (&Cmd[5]) * -1;
							cmd = FALSE;
							j = 0;
						}
						else
						{
							j++;
							continue;
						}
					}
					else if (bias)
					{
						Hold[j] = Buf[i];
						if (Hold[j] == '\n')
						{
							UWORD k;

							if (bias == offset)	// Found?
							{
								Hold[j] = 0;
								for (k = 0; Hold[k] != 0; k++)							
								{
									FDName[k] = Hold[k];
									if (FDName[k] == '(')
									{
										FDName[k] = 0;
										return (TRUE);
									}
								}
							}
							bias -= 6;
							j = 0;
						}
						else
							j++;
					}

					if (Buf[i] == '*')	// Skip
					{
						skip = TRUE;
						continue;
					}

					if (i < size - 2)
					{
						if (Buf[i] == '#' AND Buf[i+1] == '#') // Command
						{
							cmd = TRUE;
							Cmd[0] = 0;
							j = 0;
							i++;
						}
					}
				}
				flag = TRUE;
			}
			FreeMem (Buf, size);
		}
	}
	return (flag);
}

main ()
{
	char FDName[50];

	if (Find_FD_Name ("exec.library", -906, FDName))
		printf ("%ld = '%s'\n", -906, FDName);
	else
		printf ("Not found\n");
}
