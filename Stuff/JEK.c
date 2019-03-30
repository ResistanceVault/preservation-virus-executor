/*
Hallo JEK.... Prøv om denne her funker...
Den rutina bruker 0.0000468 sek på min A4000/040 25 MHz
*/

#include <JEO:JEO.h>

#define TALL_LEFT		 0
#define TALL_RIGHT	 1
#define TALL_CENTER 2

BOOL Fyll_tegn (char *String, char fyll_tegn, int tall, UWORD len, UBYTE mode)
{
	char Tall_hold[12];	// ULONG + evt - og /0
	int tall_len;

	memset (String, fyll_tegn, len);
/*
	memset makroen over kan erstattes av:

	for (i = 0; i < len; i++)
		String[i] = fyll_tegn;
*/
	String[len] = 0;

	sprintf (Tall_hold, "%d", tall);
	tall_len = strlen (Tall_hold);
	if (tall_len > len)
		return (FALSE);	// tall lengden > ønsket lengde

	if (mode == TALL_RIGHT)
		strncpy (&String[len - tall_len], Tall_hold, tall_len);
	else if (mode == TALL_LEFT)
		strncpy (String, Tall_hold, tall_len);
	else	// CENTER
		strncpy (&String[(len - tall_len) / 2], Tall_hold, tall_len);

	return (TRUE);
}

main ()
{
  char String[50];
  int tall = 12345;
	int len = 20;

	Fyll_tegn (String, '*', tall, len, TALL_LEFT);
	printf ("  LEFT: %s\n", String);
	Fyll_tegn (String, '0', tall, len, TALL_RIGHT);
	printf (" RIGHT: %s\n", String);
	Fyll_tegn (String, '*', tall, len, TALL_CENTER);
	printf ("CENTER: %s\n", String);
	Fyll_tegn (String, '_', 1234, len, TALL_CENTER);
	printf ("CENTER: %s\n", String);
	Fyll_tegn (String, '.', 123, len, TALL_CENTER);
	printf ("CENTER: %s\n", String);
	Fyll_tegn (String, '-', 12, len, TALL_CENTER);
	printf ("CENTER: %s\n", String);
}
