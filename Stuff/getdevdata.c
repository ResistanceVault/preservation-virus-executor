;/*
sc resopt link optimize debug=line idir=include: nostartup nostackcheck opt data=far getdevdata.c
quit
*/

/*
** Example on how to get device info from mounted device/volume
**
** Written by Harry "Piru" Sintonen <sintonen@iki.fi>
** Public Domain.
**
*/

#include <exec/memory.h>
#include <exec/errors.h>
#include <exec/io.h>
#include <dos/dos.h>
#include <dos/filehandler.h>

#define __USE_SYSBASE
#include <proto/exec.h>
#include <proto/dos.h>

#include <string.h>

struct LocalData
{
  struct ExecBase	*ld_SysBase;
  struct DosLibrary	*ld_DOSBase;
};

#define SysBase		ld->ld_SysBase
#define DOSBase		ld->ld_DOSBase


struct voldata
{
  UBYTE          *vd_name;
  UBYTE           vd_buf[1];
};

struct devdata
{
  UBYTE          *dd_name;
  ULONG           dd_unit;
  ULONG           dd_flags;
  struct MsgPort *dd_FileSystemPort;
  struct DosEnvec dd_envec;
  UBYTE           dd_buf[1];
};


/* protos
*/
struct voldata *GetVolData(struct LocalData *ld, const UBYTE *name);
struct devdata *GetDevData(struct LocalData *ld, const UBYTE *name);


int main(int argc, char **argv)
{
  struct LocalData _ld = {0}, *ld = &_ld;
  int ret = RETURN_FAIL;

  SysBase = *((struct ExecBase **) 4);

  DOSBase = (APTR) OpenLibrary("dos.library", 37);
  if (DOSBase)
  {
    struct devdata *dd;
    struct voldata *vd;

    vd = GetVolData(ld, "Workbench");
    if (vd)
    {
      Printf("vd_name \"%s\"\n", (LONG) vd->vd_name);

      FreeVec(vd);
    }

    dd = GetDevData(ld, argv[1]);
    Printf("%s\n", argv[1]);
    if (dd)
    {
      Printf("\"%s\" unit %lu flags 0x%lx\n", (LONG) dd->dd_name,
             dd->dd_unit, dd->dd_flags);
      Printf("filesystem port 0x%lx\n", (ULONG) dd->dd_FileSystemPort);

      Printf("de_SizeBlock(bytes): %lu\n", dd->dd_envec.de_SizeBlock * 4);
      Printf("de_BlocksPerTrack:   %lu\n", dd->dd_envec.de_BlocksPerTrack);
      Printf("de_LowCyl:           %lu\n", dd->dd_envec.de_LowCyl);
      Printf("de_HighCyl:          %lu\n", dd->dd_envec.de_HighCyl);
      if (dd->dd_envec.de_TableSize >= DE_MAXTRANSFER)
      {
        Printf("de_MaxTransfer:      %lu\n", dd->dd_envec.de_MaxTransfer);
      }

      ret = RETURN_OK;

      FreeVec(dd);
    }
    else
    {
      ret = RETURN_WARN;
    }
  }

  return ret;
}


struct voldata *GetVolData(struct LocalData *ld, const UBYTE *name)
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


struct devdata *GetDevData(struct LocalData *ld, const UBYTE *name)
{
  struct devdata *dd = NULL;
  ULONG tmpnamelen;
  UBYTE *tmpname;
  struct DosList *doslist;

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

  doslist = LockDosList(LDF_DEVICES | LDF_READ);

  doslist = FindDosEntry(doslist, tmpname, LDF_DEVICES);

  FreeMem(tmpname, tmpnamelen + 1);

  if (doslist)
  {
    struct FileSysStartupMsg *fssm;

    fssm = BADDR(doslist->dol_misc.dol_handler.dol_Startup);
    if (fssm > (struct FileSysStartupMsg *) 0x400 &&
        fssm->fssm_Device && fssm->fssm_Environ)
    {
      const UBYTE *devname;

      devname = BADDR(fssm->fssm_Device);

      dd = AllocVec(sizeof(struct devdata) + devname[0], MEMF_PUBLIC);
      if (dd)
      {
        struct DosEnvec *DosEnvec;

        dd->dd_name           = dd->dd_buf;
        CopyMem((APTR) &devname[1], dd->dd_name, devname[0]);
        dd->dd_name[devname[0]] = '\0';
        dd->dd_unit           = fssm->fssm_Unit;
        dd->dd_flags          = fssm->fssm_Flags;
        dd->dd_FileSystemPort = doslist->dol_Task;

        DosEnvec = BADDR(fssm->fssm_Environ);
        tmpnamelen = DosEnvec->de_TableSize * sizeof(ULONG);
        if (tmpnamelen >= sizeof(struct DosEnvec))
        {
          tmpnamelen = sizeof(struct DosEnvec);
        }
        CopyMem(DosEnvec, &dd->dd_envec, tmpnamelen);
      }
    }
  }

  UnLockDosList(LDF_DEVICES | LDF_READ);

  return dd;
}
