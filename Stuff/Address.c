#include <JEO:JEO.h>

char far Dummy[20000];

main ()
{
	UBYTE *p;
	UBYTE *tmp;
	ULONG i, j, k;
	ULONG address;
	char Head[20] = "";
	char Hex[70];
	char Ascii[20] = "";
	char temp;
	char Hold[100];
	char Search[10];
	BOOL found = FALSE;	
	UWORD back_lines = 12;

	printf ("Enter address: ");
  scanf ("%x",&address);

	printf ("Enter search string (4 chars): ");
  gets (Search);
  gets (Search);

	printf ("Viewing memory at address $%08lX\n\n", address);

	strcpy (Dummy, " Off  HEX                                              ASCII\n");
	strcat (Dummy, " ---  -----------------------------------------------  ----------------\n");
	for (i = address - (16 * back_lines), j = 0, k = 0; i < address + 1; i++, j++, k++)
	{
		p = (UBYTE *)i;

		if (!(j % 16) AND j > 0)
		{
			sprintf (Head, " %03ld:", (k - (16 * back_lines) - 16));
			sprintf (Hold, "%s %s %s\n", Head, Hex, Ascii);
			strcat (Dummy, Hold);
			Hex[0] = Ascii[0] = 0;
		}
		sprintf (Hold, "%02lX ", *p);
		strcat (Hex, Hold);

    if ((*p >= 32 AND *p <= 127) OR (*p >= 161 AND *p <= 255))
      temp = *p;
    else
      temp = '.';

		if (!found)
		{
			tmp = p;
			if (Search[0] == *tmp)
			{
				tmp++;
				if (Search[1] == *tmp)
				{
					tmp++;
					if (Search[2] == *tmp)
					{
						tmp++;
						if (Search[3] == *tmp)
						{
							printf ("FOUND at %ld\n", (k - (16 * back_lines)));
							found = TRUE;
						}
					}
				}
			}
		}

		sprintf (Hold, "%c", temp);
		strcat (Ascii, Hold);
	}

	strcat (Dummy, "\n");
	Hex[0] = Ascii[0] = 0;
	for (i = address, j = 0; i < address + 1 + (16 * 64); i++, j++)
	{
		p = (UBYTE *)i;

		if (!(j % 16) AND j > 0)
		{
			sprintf (Head, " %03ld:", j - 16);
			sprintf (Hold, "%s %s %s\n", Head, Hex, Ascii);
			strcat (Dummy, Hold);
			Hex[0] = Ascii[0] = 0;
		}
		sprintf (Hold, "%02lX ", *p);
		strcat (Hex, Hold);

    if ((*p >= 32 AND *p <= 127) OR (*p >= 161 AND *p <= 255))
      temp = *p;
    else
      temp = '.';

		if (!found)
		{
			tmp = p;
			if (Search[0] == *tmp)
			{
				tmp++;
				if (Search[1] == *tmp)
				{
					tmp++;
					if (Search[2] == *tmp)
					{
						tmp++;
						if (Search[3] == *tmp)
						{
							printf ("FOUND at %ld\n", j);
							found = TRUE;
						}
					}
				}
			}
		}

		sprintf (Hold, "%c", temp);
		strcat (Ascii, Hold);
	}
	if (!found)
		printf ("%s\n", Dummy);
}
