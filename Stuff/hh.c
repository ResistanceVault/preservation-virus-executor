#include <JEO:JEO.h>
#include <ctype.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <proto/socket.h>
#include <bsdsocket.h>

struct Library *socket = 0;


main ()
{
	if (socket = OpenLibrary ("bsdsocket.library", NULL))
	{
	}
	printf ("%08lx\n", socket);

	if (socket)
		CloseLibrary (socket);
}
