#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>

WORD Exists (char *Name)
{
  BPTR lock = 0;
  struct FileInfoBlock *fib = 0;
  BOOL flag;

	flag = FALSE;
  if (!(fib = (struct FileInfoBlock *)AllocMem (sizeof (struct FileInfoBlock), MEMF_CLEAR)))
    return (ERROR); 

  if (lock = Lock (Name, ACCESS_READ))
  {
    if (Examine (lock, fib))
      flag = TRUE;
  }

  if (lock)
    UnLock (lock);
  if (fib)
    FreeMem (fib, sizeof (struct FileInfoBlock));

  return (flag);
}

char VE[14] = "VirusExecutor";


#define MAX_VARS 8
char Vars[MAX_VARS][35];

VOID InitVars (VOID)
{
	UBYTE i = 0;

	sprintf (Vars[i++], "%s/JEOPlayPath", VE);
	sprintf (Vars[i++], "%s/PrefsPath", VE);
	sprintf (Vars[i++], "%s/DataBasePath", VE);
	sprintf (Vars[i++], "%s/FileWatchDataPath", VE);
	sprintf (Vars[i++], "%s/GuidePath", VE);
	sprintf (Vars[i++], "%s/LogPath", VE);
	sprintf (Vars[i++], "%s/PatchBrainPath", VE);
	sprintf (Vars[i++], "%s/PrefsPath", VE);
}

VOID main ()
{
	char Buffer[256] = "C:";
	BOOL success;
	LONG len;
	UBYTE i;

	InitVars ();

	for (i = 0; i < MAX_VARS; i++)
	{
		len = GetVar (Vars[i], Buffer, 256, GVF_GLOBAL_ONLY);
		if (len > 0)	// Okay
		{
		}
		else
			printf ("Can't find variabel: %s\n", Vars[i]);
	}

/*
	success = SetVar ("VirusExecutor/JEOPlayPath", Buffer, -1, GVF_GLOBAL_ONLY|GVF_SAVE_VAR);
	printf ("success = %ld\n", success);

	len = GetVar ("VirusExecutor/SoundPlayPath", Buffer, 256, GVF_GLOBAL_ONLY);
	printf ("len = %ld '%s'\n", len , Buffer);
*/
}
