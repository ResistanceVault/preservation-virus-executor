#include <JEO:JEO.h>
#include <ctype.h>
#include <exec/execbase.h>
#include <exec/resident.h>
#include <exec/memory.h>
#include <proto/dos.h>

ULONG hw_ROMstart, hw_ROMend, hw_ROMsize;
ULONG ROMstart;
ULONG exec_start, exec_end, exec_size;

extern struct ExecBase *SysBase;

VOID GetOSVersionNumber (ULONG start, char *OSVerNumber)
{
  UBYTE *ptr;
  ULONG i, j;

  for (i = start + 100, j = 0; i < start + 200; i++)
  {
    ptr = (UBYTE *)i;
    if (isdigit(*ptr) OR *ptr == '.')
    {
      if (*ptr == '.' AND j == 0)
        continue;
      OSVerNumber[j] = *ptr;
      j++;
    }
    else if (j > 0)
      break;
  }
  OSVerNumber[j] = 0;
}

ULONG GetKickStartVersion (ULONG *ROMStart)
{
	ULONG *p;

	p = ROMStart;
	p += 3;
	return (*p);
}

VOID Do_Kicks (VOID)
{
	ULONG hw_ROM_data, ROM_data;
	ULONG i, rom;
	ULONG *p;
	UWORD *p1;
	struct Resident *r;
	UWORD hw_ROM_version, hw_ROM_revision;
	UWORD ROM_version, ROM_revision;
	char HW_OSVer[10], OSVer[10];
	ULONG exec_start, exec_end, exec_size;

	p = (ULONG *)0x00FFFFEC;
	hw_ROMsize = *p;
	hw_ROMstart = 0x01000000 - hw_ROMsize;
	hw_ROMend = hw_ROMstart + hw_ROMsize;

	hw_ROM_data = GetKickStartVersion ((ULONG *)hw_ROMstart);
	hw_ROM_version = hw_ROM_data >> 16;
	hw_ROM_revision = (UWORD)hw_ROM_data;
 	GetOSVersionNumber (hw_ROMstart, HW_OSVer);

	printf ("\n  Found HW-ROM at $%08lX - $%08lX (%ldKb)\n", hw_ROMstart, hw_ROMend - 1, hw_ROMsize / 1024);
	printf ("  HW ROM: %s (%ld.%ld)\n\n", HW_OSVer, hw_ROM_version, hw_ROM_revision);

// *********************************** SOFTROM ***************************************

	ROMstart = FALSE;
	p1 = (UWORD *)GetSoftROMAddress ();
	if (*p1)
	{
		rom = (ULONG)p1;
		for (i = rom; i < rom + 0x10000; i++, p1++)
		{
			if (*p1 == 0x1114)
			{
				UWORD *p;

				p = p1;
				p++;
				if (*p++ == 0x4ef9)
				{
					if (*p++ == i >> 16)
					{
						p++;
						if (*p++ == 0x0000)
						{
							if (*p++ == 0xffff)
							{
								ROMstart = (ULONG)p1;
								
								break;
							}
						}
					}
				}
			}
		}
	}

	if (ROMstart)
	{
		ROM_data = GetKickStartVersion ((ULONG *)ROMstart);
		ROM_version = ROM_data >> 16;
		ROM_revision = (UWORD)ROM_data;
	 	GetOSVersionNumber (ROMstart, OSVer);

		printf ("\n  Found kickstart at $%08lX\n", ROMstart);
		printf ("  Kickstart: %s (%ld.%ld)\n\n", OSVer, ROM_version, ROM_revision);
	}

	if (r =  FindResident ("exec.library"))
	{
		if (r->rt_Version >= 45)	// BB2 exec
		{
			exec_start = (ULONG)r;
			exec_end = (ULONG)r->rt_EndSkip;
			exec_size = exec_end - exec_start;

			printf ("\n\n  Exec Res (Type: $%02lx)\n", r->rt_Type);
			printf ("  Found Exec Res at $%08lX - $%08lX (%ldKb)\n\n", exec_start, exec_end - 1, exec_size / 1024);
		}
	}
}

main ()
{
	Do_Kicks ();
}
