#include <JEO:JEO.h>
#include	<exec/types.h>
#include	<exec/execbase.h>
#include	<exec/tasks.h>
#include	<exec/memory.h>
#include	<exec/alerts.h>
#include	<exec/ports.h>
#include	<exec/libraries.h>
#include	<exec/semaphores.h>
#include	<exec/resident.h>
#include	<dos/dos.h>
#include	<dos/dosextens.h>
#include	<dos/rdargs.h>
#include	<devices/timer.h>
#include	<workbench/startup.h>
#include	<workbench/workbench.h>
#include	<libraries/configvars.h>
#include	<clib/exec_protos.h>
#include	<pragmas/exec_pragmas.h>
#include	<clib/dos_protos.h>
#include	<pragmas/dos_pragmas.h>
#include	<string.h>

#define EXECBASE (*(struct ExecBase **)4)

main ()
{
	UWORD	*ROM;
	ULONG	index;

	/* Ok, lets find the ROM start address... */
	ROM = (UWORD *)(0xFFF80000 & ((ULONG)(EXECBASE->LibNode.lib_Node.ln_Name)));

	printf ("%08lx\n", ROM);

	for (index = 0;index < (512*1024/2); index++)
	{
		if (ROM[index] == RTC_MATCHWORD)
		{
			struct Resident *romTag = (struct Resident *)(&ROM[index]);

			if (romTag == romTag->rt_MatchTag)
			{
				char	*from;

				index = (((ULONG)(romTag->rt_EndSkip))-((ULONG)ROM))/2-1;
				printf ("%08lX\n", index);
				from = romTag->rt_IdString;

				 // This simple check makes sure that there
				 // is a version/date on the module.  If not,
				 // it is assumed to be a "special" one.

				while ((*from) AND (*from != '('))
					from++;
				if (*from)
				{
					from = romTag->rt_IdString;

					printf ("%08lX: %s\n", romTag, from);
				}
			}
		}
	}
}
