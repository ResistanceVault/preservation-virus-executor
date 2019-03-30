#include <JEO:JEO.h>
#include <pragmas/mmu_pragmas.h>

main ()
{
	struct Library *MMUBase;

	if (MMUBase = OpenLibrary ("mmu.library", NULL))
	{
		ULONG type;

		type = GetMMUType ();
		printf ("\n  %ld\n\n", type);
		CloseLibrary (MMUBase);
	}
}
