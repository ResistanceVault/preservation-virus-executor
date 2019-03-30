#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>

main ()
{
	UBYTE *address;

	address = AllocMem (1024 * 1024, MEMF_CLEAR);
	printf ("%08lx\n", (ULONG)address);
}
