#include <JEO:JEO.h>

VOID rev (char *s)
{
	int c, i, j;

	for (i = 0, j = strlen (s) - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

main ()
{
	int i;
	char JEO[] = "Dette er en test for Jan Erik Olausen for å se om det funker ;)";

	for (i = 0; i < 500000; i++)
		rev (JEO);
}
