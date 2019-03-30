#include <JEO:JEO.h>
#include <exec/execbase.h>

extern struct ExecBase *SysBase;

main ()
{
	ULONG flags = 0, i;

	SysBase = (*((struct ExecBase **) 4));
	flags = SysBase->AttnFlags;

	for (i = 0; i < 200; i++)
	{
		if (flags & (1L<<i))
			printf ("Found %ld\n", i);
	}
}
