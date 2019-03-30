#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>

VOID main ()
{
	struct InfoData *id;
	char DirName[100] = "cd0:";
	
	if (id = AllocMem (sizeof (struct InfoData), MEMF_PUBLIC))
	{
		BPTR lock;
	
		if (lock = Lock (DirName, ACCESS_READ))
		{
			if (Info (lock, id))
			{
				printf ("DiskState: %08lx\n", id->id_DiskState);
			}
			UnLock (lock);
		}
		FreeMem (id, sizeof (struct InfoData));
	}
}
