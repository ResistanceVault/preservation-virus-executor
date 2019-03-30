#include <JEO:JEO.h>

char Dummy[10000];

main ()
{
	UBYTE *p;
	ULONG i, j;
	char Hold[500];
	char Svar[50];
	int len;

	printf ("Tast søkeord: ");
	gets (Svar);
	len = strlen (Svar);
	if (len > 1)
	{
		printf ("Søker i CHIP RAM...\n");
		for (i = 0x00000000, j = 0; i < 0x001FFFFF; i++)
		{
			p = (UBYTE *)i;
			Hold[j] = *p;
			if (j == len)
			{
				Hold[j] = 0;
				j = 0;
				if (!(strcmp (Svar, Hold)))
					printf ("Adresse: %08lx -> '%s'\n", i, Hold);
			}
			else
				j++;
		}
		printf ("Søker i FAST RAM...\n");
		for (i = 0x08000000, j = 0; i < 0x0A700000; i++)
		{
			p = (UBYTE *)i;
			Hold[j] = *p;
			if (j == len)
			{
				Hold[j] = 0;
				j = 0;
				if (!(strcmp (Svar, Hold)))
					printf ("Adresse: %08lx -> '%s'\n", i, Hold);
			}
			else
				j++;
		}
	}
}
