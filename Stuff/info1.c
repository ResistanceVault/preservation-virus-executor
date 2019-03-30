#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>

struct voldata
{
  UBYTE *vd_name;
  UBYTE vd_buf[1];
};

struct voldata *GetVolData (char *name);

int main ()
{
	struct voldata *vd;

	if (vd = GetVolData ("Workbench"))
	{
		printf ("vd_name \"%s\"\n", vd->vd_name);
		FreeVec (vd);
  }
}

struct voldata *GetVolData (char *name)
{
  struct voldata *vd = NULL;
  ULONG tmpnamelen;
  UBYTE *tmpname;
  struct DosList *doslist, *dosentry;
  struct MsgPort *handler;

  tmpnamelen = strlen(name);
  tmpname = AllocMem(tmpnamelen + 1, MEMF_ANY);
  if (!tmpname)
  {
    return NULL;
  }
  strcpy(tmpname, name);
  if (tmpname[tmpnamelen - 1] == ':')
  {
    tmpname[tmpnamelen - 1] = '\0';
  }

  doslist = LockDosList(LDF_VOLUMES | LDF_DEVICES | LDF_READ);

  dosentry = FindDosEntry(doslist, tmpname, LDF_VOLUMES);

  FreeMem(tmpname, tmpnamelen + 1);

  if (dosentry)
  {
    /* Scan the devicelist for entry with a matching handler */
    handler = dosentry->dol_Task;

    dosentry = doslist;
    while ((dosentry = NextDosEntry(dosentry, LDF_DEVICES)))
    {
      if (dosentry->dol_Task == handler)
      {
        const UBYTE *volname;

        volname = BADDR(dosentry->dol_Name);
        if (volname)
        {
          vd = AllocVec(sizeof(struct voldata) + volname[0], MEMF_PUBLIC);
          if (vd)
          {
            vd->vd_name           = vd->vd_buf;
            CopyMem((APTR) &volname[1], vd->vd_name, volname[0]);
            vd->vd_name[volname[0]] = '\0';

            break;
          }
        }
      }
    }
  }

  UnLockDosList(LDF_VOLUMES | LDF_DEVICES | LDF_READ);

  return vd;
}
