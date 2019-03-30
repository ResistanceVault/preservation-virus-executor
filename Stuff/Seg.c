#include <JEO:JEO.h>
#include <proto/dos.h>

main ()
{
	BPTR seglist;

	seglist = LoadSeg ("v:virusexecutor");
	printf ("%08lX\n", seglist);
}
