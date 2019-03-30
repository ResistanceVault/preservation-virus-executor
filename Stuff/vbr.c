#include <JEO:JEO.h>

main ()
{
	ULONG vbr;

	vbr = GetVBR1 ();
	printf ("%08lX\n", vbr);
	vbr = GetVBR ();
	printf ("%08lX\n", vbr);
}
