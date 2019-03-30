#include <JEO:JEO.h>
#include <exec/types.h>
#include <exec/execbase.h>
#include <exec/libraries.h>
#include <exec/semaphores.h>
#include <string.h>

typedef char (* __asm SegTrack(register __a0 ULONG, register __a1 ULONG *, register __a2 ULONG *));
struct SegSem
{
	struct SignalSemaphore seg_Semaphore;
  SegTrack *seg_Find;
};

#define SEG_SEM "SegTracker"

BOOL FindSegAddress (ULONG address, char *Name)
{
	BOOL ret = FALSE;
	struct SegSem *segSem;

	if (segSem = (struct SegSem *)FindSemaphore(SEG_SEM))
  {
  	char *p;
		ULONG tmp[4];

		tmp[0] = address;
		Forbid();
    p = (*segSem->seg_Find) (tmp[0], &tmp[2], &tmp[3]);
    Permit();
		if (p)
		{
			strcpy (Name, p);
			ret = TRUE;
		}
	}
	return (ret);
}

main ()
{
	char Name[100];

	if (FindSegAddress (0x80f3504, Name))
		printf ("%s\n", Name);
	else
		printf ("Not found\n");
}
