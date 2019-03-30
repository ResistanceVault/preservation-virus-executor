#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>

VOID BstrC (BSTR bstr, char *Buf)
{
	UBYTE *str;
	LONG loop, counter;

	counter = 0;
	str = (UBYTE *)BADDR (bstr);
	for (loop = (LONG)str[0]; loop--; ++counter)
		Buf[counter] = str[counter+1];
	Buf[counter++] = ':';
	Buf[counter] = 0;
}

UBYTE max_volumes;

VOID main ()
{
	BPTR lock;
	struct RootNode *rn;
	struct DosInfo *di;
	struct DeviceList *dl;
	struct InfoData *id;
	char VolumeName[50][80];
	UBYTE count = 0;

	rn = DOSBase->dl_Root;
	di = BADDR (rn->rn_Info);
	dl = BADDR (di->di_DevInfo);
	max_volumes = 0;

	if (id = AllocMem (sizeof (struct InfoData), MEMF_PUBLIC))
	{
		char Hold[80];

		Forbid ();
		while (dl->dl_Next)
		{
			if (dl->dl_Type == DLT_VOLUME)
			{
//				BstrC (dl->dl_Name, Hold);
				SPrintF (Hold, "%b", dl->dl_Name);
				printf ("1 %s\n", Hold);
				if (lock = Lock (Hold, ACCESS_READ))
				{
					BOOL go_flag = TRUE;

					if (Info (lock, id))
					{
						if (id->id_DiskState != ID_VALIDATING)
						{
							switch (id->id_DiskType)
							{
								case ID_NO_DISK_PRESENT: go_flag = FALSE;
								case ID_UNREADABLE_DISK: go_flag = FALSE;
								case ID_NOT_REALLY_DOS: go_flag = FALSE;
								case ID_KICKSTART_DISK: go_flag = FALSE;
							}
						}
					}
					else
						go_flag = FALSE;

					UnLock (lock);
					if (go_flag)
						strcpy (VolumeName[count++], Hold);
				}
			}
			dl = BADDR (dl->dl_Next);
		}
		Permit ();
		max_volumes = count;
		FreeMem (id, sizeof (struct InfoData));
	}
	if (max_volumes)
	{
		BYTE i;

		for (i = max_volumes - 1; i >= 0; i--)
			printf ("'%s'\n", VolumeName[i]);	
	}
}
