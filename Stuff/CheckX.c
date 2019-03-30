#define NAME	     "CheckX"
#define REVISION     "42"
#define DISTRIBUTION "(Freeware) "

/*
When someone uses the here shown methods for own programs, he has to
contact me first and always to mention me in his program documentation!

This program scans for crunched, linked files and archived files. It
decrunches them and saves the result files to another directory-tree.
The scanning routines are recursiv and thus check really all stuff. Linked
files are saved with .1, .2, ... additions.

The main purpose it was written for is to scan for crunched files and to
test the decrunch routines. So the logging may take some more time, but is
very stable, as the last log-entry is always the file which possibly
crashed the machine.

Address crunched programs are decrunched as well, but not saved. Use
xfdDecrunch or xfdDecrunchAddr to generate decrunched executable files
of this type (with correct header information).

Especially in UNARCHIVE mode this tool may need lots of memory (when T: is
assigned to ram disk). Keep that in mind! Every checked file is completely
loaded into memory!

CheckX cannot scan files, which are read-protected. You get XFD error 106
as result in that case. Unprotect files and scan again when you want.

NOTE: SaveDir must exist already, only sub-dirs are created.

The program must be compiled and linked without startup-code. You can set
the pure file protection bit and make it resident, because it is multi-
reentrant (no global variables, except library bases).
This means it can be called multiple times as well, but this is not
recommended, as said above CheckX needs lots of memory.

Disk archives are handled this way:
They are unarchived to RD0: in depth1, to RD1: in depth2 (when archive is
in RD0:) and to RD2: in depth3, ....

NOTE: accessing RD0:, RD1 or RD2: during this time may produce errors.
This includes starting CheckX twice, when scanning disk archives.

For high density archives the archive names are RH1:, RH2:, ...
For the moment this is disabled, as DMS seems to be impossible to WRITE
to RHx: instead of DF0: for HD disks.

Splitted DMS-Archives may produce strange errors, but can be checked
correctly, when the two parts are directly after another (e.g. no other
track archive is between them). In this case the first file shows some
XPKCERR_READWRITE and the second one the correct stuff.
*/

/* Programmheader

	Name:		CheckX
	Author:		SDI
	Distribution:	Freeware
	Description:	scans and decrunches crunched files with xfd
	Compileropts:	-
	Linkeropts:	-l xpkmaster amiga -gsi
	Plans:		add new archivers, add virus check with xvs.library

 1.0   14.12.96 : first Version
 1.1   28.12.96 : moved PassRequest into xpkmaster.library
 1.2   12.02.97 : now also decrunches Exe-Files
 1.3   15.06.97 : added length output as test
 1.4   21.11.97 : renamed from Decrunch, got really new program
 1.5   22.11.97 : bug-fixes
 1.6   29.11.97 : added unarchiving feature
 1.7   30.11.97 : bug-fixes
 1.8   06.12.97 : xpkmaster.library now only required with ASKPWD option
 1.9   07.12.97 : Added archive copy for weird archive names, better error
	codes
 1.10  08.12.97 : fixed error codes a bit
 1.11  11.12.97 : disabled DOS requests, added Zip-Archives, added TaskID
	to temporary filenames
 1.12  12.12.97 : added Arc, ZOO and LhASFX archives
 1.13  13.12.97 : fixed Arc recognition
 1.14  19.12.97 : deletes copied arc before scan
 1.15  22.12.97 : crunched archives are unarchived correctly now
 1.16  02.01.98 : opens dos.library itself, no startup-code required
 1.17  23.01.98 : added disk crunchers
 1.18  24.01.98 : some fixes
 1.19  01.02.98 : little bug-fix in argument-option use
 1.20  06.02.98 : better error output, added automount
 1.21  08.02.98 : little bug-fix
 1.22  10.02.98 : fixed archive copy conditions, added PRINTALL
 1.23  12.02.98 : fixed help text, bug fixes with unlinking and FreeMem
 1.24  13.02.98 : added unstripping
 1.25  04.03.98 : added PRINTEXEC
 1.26  13.03.98 : added high density DMS support
 1.27  19.03.98 : added LOUD keyword
 1.28  23.03.98 : RDx no longer depends on archive depth, but on dddepth
 1.29  10.04.98 : bug fixes
 1.30  26.04.98 : bug fixes
 1.31  09.05.98 : now uses no longer adress 4 for SysBase
 1.32  31.05.98 : better output
 1.33  04.06.98 : added HEADER addition for address files
 1.34  08.08.98 : bug fix with SAVE
 1.35  24.09.98 : added xvs.library virus checks
 1.36  18.10.98 : xvs is opened global and only once
 1.37  30.10.98 : renamed from CheckXFD
 1.38  11.11.98 : fixed format drive bug using a delay and an error report
 1.39  14.11.98 : format error with AUTOMOUNT removed
 1.40  16.11.98 : removed HEADER addition stuff
 1.41  18.11.98 : better RDx: access and mount
 1.42  23.11.98 : fixed bug with hunk stripping
*/

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/xfdmaster.h>
#include <proto/xpkmaster.h>
#include <proto/xvs.h>
#include <libraries/xfdmaster.h>
#include <dos/dostags.h>
#include <dos/doshunks.h>
#include <exec/memory.h>
#include "SDI_defines.h" /* make version string */

#define SDI_TO_ANSI
#include "SDI_ASM_STD_protos.h"

#define PARAM   "FROM,LOG,SAVE/K,ALL/S,ASKPWD/S,AUTOMOUNT/S,PRINTALL/S," \
		"PRINTEXEC/S,LOUD/S,"				 \
		"NODECRUNCH/S,NOUNLINK/S,NOUNARCHIVE/S,NOUNTRACK/S,"	 \
		"NOTRACKCUT/S,NOSTRIP/S,NOVIRUS/S"

#ifdef __SASC
  #define XpkBase	xpkbase
  #define ASSIGN_XPK
#else
  struct Library *	XpkBase		= 0;
  #define ASSIGN_XPK	XpkBase = xpkbase;
#endif
struct xfdMasterBase *	xfdMasterBase	= 0;
struct DosLibrary *	DOSBase		= 0;
struct ExecBase *	SysBase		= 0;
struct xvsBase *	xvsBase		= 0;

struct Args {
  STRPTR from;
  STRPTR log;
  STRPTR save;
  ULONG  all;
  ULONG  askpwd;
  ULONG  automount;
  ULONG  printall;
  ULONG  printexec;
  ULONG	 loud;
  ULONG  nodecrunch;
  ULONG  nounlink;
  ULONG  nounarchive;
  ULONG  nountrack;
  ULONG  notrackcut;
  ULONG  nostrip;
  ULONG  novirus;
};

struct UnArcData { /* This is allocated to save stack space */
  STRPTR uad_ArcName;
  UBYTE  uad_DestDir[30];
  UBYTE  uad_DoDestDir[30];
  UBYTE  uad_CopyArcName[30];
  UBYTE  uad_Command[200];
};

struct CrunchMemList {
  struct CrunchMemList * cml_Next;
  APTR			 cml_MemoryRegion;
  ULONG			 cml_MemorySize;
};

#define XFDCHECKFLAG_SAVE		(1<< 0)
#define XFDCHECKFLAG_ALL		(1<< 1)
#define XFDCHECKFLAG_ASKPWD		(1<< 2)
#define XFDCHECKFLAG_AUTOMOUNT		(1<< 3)
#define XFDCHECKFLAG_PRINTALL		(1<< 4)
#define XFDCHECKFLAG_PRINTEXEC		(1<< 5)
#define XFDCHECKFLAG_LOUD		(1<< 6)
#define XFDCHECKFLAG_NODECRUNCH		(1<< 7)
#define XFDCHECKFLAG_NOUNLINK		(1<< 8)
#define XFDCHECKFLAG_NOUNARCHIVE	(1<< 9)
#define XFDCHECKFLAG_NOUNTRACK		(1<<10)
#define XFDCHECKFLAG_NOTRACKCUT		(1<<11)
#define XFDCHECKFLAG_NOSTRIP		(1<<12)

#define XFDCALLFLAGS	(XFDCHECKFLAG_SAVE|XFDCHECKFLAG_ALL|		  \
			 XFDCHECKFLAG_ASKPWD|XFDCHECKFLAG_AUTOMOUNT|	  \
			 XFDCHECKFLAG_PRINTALL|XFDCHECKFLAG_PRINTEXEC|	  \
			 XFDCHECKFLAG_LOUD|				  \
			 XFDCHECKFLAG_NODECRUNCH|XFDCHECKFLAG_NOUNLINK|   \
			 XFDCHECKFLAG_NOUNARCHIVE|XFDCHECKFLAG_NOUNTRACK| \
			 XFDCHECKFLAG_NOTRACKCUT|XFDCHECKFLAG_NOSTRIP)

#define XFDSAVEFLAGS	(XFDCHECKFLAG_LINKED|XFDCHECKFLAG_CRUNCHED|	  \
			 XFDCHECKFLAG_STRIPPED)

#define XFDCHECKFLAG_NAMEPRINTED	(1<<20)
#define XFDCHECKFLAG_CRUNCHED		(1<<21)
#define XFDCHECKFLAG_LINKED		(1<<22)
#define XFDCHECKFLAG_ADDRESS		(1<<23)
#define XFDCHECKFLAG_ARCCOPY		(1<<24)
#define XFDCHECKFLAG_DIRCHANGE		(1<<25)
#define XFDCHECKFLAG_NOFREEMEM		(1<<26)
#define XFDCHECKFLAG_STRIPPED		(1<<27)
#define XFDCHECKFLAG_HIGHDENSITY	(1<<28)

#define ARCTYPE_LHA	  1
#define ARCTYPE_LZX	  2
#define ARCTYPE_ZIP	  3
#define ARCTYPE_ZOO	  4
#define ARCTYPE_LHA_SFX	  5
#define ARCTYPE_ARC	  6

/* types greater than 100 are disk archives */
#define ARCTYPE_DMS	101
#define ARCTYPE_ZOOM	102

#define XFDCERR_NOMEMORY	100
#define XFDCERR_UNARCFAIL	101
#define XFDCERR_UNARCERR	102
#define XFDCERR_NOUNARCER	103
#define XFDCERR_EXAMINEERR	104
#define XFDCERR_OPENERR		105
#define XFDCERR_READWRITE	106
#define XFDCERR_SCANERR		107
#define XFDCERR_BREAK		108
#define XFDCERR_OPENDIR		109
#define XFDCERR_NORAD		110
#define XFDCERR_NODOS		111 /* not a dos disk */
#define XFDCERR_RESOURCE	112

struct FileData {
  struct CrunchMemList * fd_MemList;
  STRPTR		 fd_Name;
  ULONG  		 fd_LogFileFH;
  ULONG  		 fd_SaveDirL;
  ULONG			 fd_Flags;
  UBYTE  		 fd_RecurseDepth;
  UBYTE  		 fd_LinkNum;
  UBYTE			 fd_ArchiveDepth;
  UBYTE			 fd_DDDepth;
  UBYTE			 fd_HDDepth;
};

LONG DoDirectoryScan(STRPTR, STRPTR, struct FileData *);
LONG DoFileOpen(struct FileData *);
void DoMount(ULONG, ULONG);
BPTR GetRad(struct FileData *, STRPTR);
LONG DoArcNameCheck(struct UnArcData *, STRPTR, ULONG, ULONG,
     struct FileData *);
LONG CallSystem(STRPTR, ULONG);
LONG DoFileUnArchive(struct FileData *, APTR, ULONG);
LONG DoGetVirus(struct FileData *, APTR, ULONG);
LONG DoFileUnLink(struct FileData *, APTR, ULONG);
LONG DoFileUnCrunch(struct FileData *, APTR, ULONG);
LONG DoFileStrip(struct FileData *, APTR, ULONG);
void PrintXFDFile(struct FileData *);
void PrintXFDErr(struct FileData *, LONG);
void PrintXFDTxt(struct FileData *, STRPTR, ...);
LONG AddCrunchMemList(struct FileData *, APTR, ULONG);
void FreeCrunchMemList(struct FileData *, APTR);
LONG SaveUncrFile(struct FileData *, APTR, ULONG);
LONG DeleteAll(STRPTR);
ULONG OpenParentDir(struct FileData *);
ULONG OpenNewDir(struct FileData *, STRPTR);

/* All memory regions must be in mem list. All unneeded memory must be freed
   as fast as possible (after unlinking, decrunching), as well as the
   MemoryList structure.
   
   The program has a loop like scan routine system, which is called for
   every file:

   A) Scan files, directories and sub directories and call following for
      every file:
   1) Test if it is an archive. When yes decrunch and start with point A.
   2) Check for viruses.
   3) Test if file is linked. When yes unlink and call point 1 for both
      parts.
   4) Test if file is crunched. When yes, decrunch and start again with
      point 1.
   5) Try stripping useless stuff. When succesful start with point 1 again.
   6) Possibly save file (with SAVE option) or end loop here.
*/

/* main routine, do argument parsing */
LONG start(void)
{
  LONG error = RETURN_FAIL;
  struct DosLibrary *dosbase;

  SysBase = (*((struct ExecBase **) 4));

  { /* test for WB and reply startup-message */
    struct Process *task;
    if(!(task = (struct Process *) FindTask(0))->pr_CLI)
    {
      WaitPort(&task->pr_MsgPort);
      Forbid();
      ReplyMsg(GetMsg(&task->pr_MsgPort));
      return RETURN_FAIL;
    }
  }

  if((dosbase = (struct DosLibrary *) OpenLibrary("dos.library", 37)))
  {
    struct xfdMasterBase *xfdmasterbase;

    DOSBase = dosbase;

    if((xfdmasterbase = (struct xfdMasterBase *)
    OpenLibrary("xfdmaster.library", 37)))
    {
      struct Args Args;
      struct RDArgs *rda;

      xfdMasterBase = xfdmasterbase;

      memset(&Args, 0, sizeof(struct Args));

      if((rda = (struct RDArgs *) AllocDosObject(DOS_RDARGS, 0)))
      {
        rda->RDA_ExtHelp =
        "FROM        source file or directory - may contain patterns\n"
        "LOG         log file name\n"
        "SAVE        directory, where decrunched files are saved\n"
        "ALL         scan deep into directories\n"
        "ASKPWD      ask for password when needed (needs xpkmaster.library)\n"
	"PRINTALL    print all filenames\n"
        "PRINTEXEC   print names of all executable files\n"
	"LOUD        show output of archiver programs\n"
	"AUTOMOUNT   automatically mount RDx: device when needed\n"
        "NODECRUNCH  do not decrunch files with xfdmaster\n"
        "NOUNLINK    do not unlink files with xfdmaster\n"
        "NOUNARCHIVE do not call archiver for unarchiving file archives\n"
        "NOUNTRACK   do not call archiver for unarchiving track archives\n"
        "NOTRACKCUT  do not call archiver for partially track archives\n"
        "NOSTRIP     do not strip useless hunks\n"
        "NOVIRUS     do not scan with xvs.library for viruses\n";
        if(ReadArgs(PARAM, (LONG *) &Args, rda))
        {
          ULONG flags = 0, log = 0;
          struct xvsBase *xvsbase = 0;

          if(!Args.from)	Args.from = "";
          if(Args.all)		flags |= XFDCHECKFLAG_ALL;
          if(Args.save)		flags |= XFDCHECKFLAG_SAVE;
          if(Args.askpwd)	flags |= XFDCHECKFLAG_ASKPWD;
          if(Args.automount)	flags |= XFDCHECKFLAG_AUTOMOUNT;
	  if(Args.printall)	flags |= XFDCHECKFLAG_PRINTALL;
	  if(Args.printexec)	flags |= XFDCHECKFLAG_PRINTEXEC;
	  if(Args.loud)		flags |= XFDCHECKFLAG_LOUD;
          if(Args.nodecrunch)	flags |= XFDCHECKFLAG_NODECRUNCH;
          if(Args.nounlink) 	flags |= XFDCHECKFLAG_NOUNLINK;
          if(Args.nounarchive)	flags |= XFDCHECKFLAG_NOUNARCHIVE;
          if(Args.nountrack)	flags |= XFDCHECKFLAG_NOUNTRACK;
          if(Args.notrackcut)	flags |= XFDCHECKFLAG_NOTRACKCUT;
          if(Args.nostrip)	flags |= XFDCHECKFLAG_NOSTRIP;
          if(!Args.novirus)
          {
            if((xvsbase = (struct xvsBase *) OpenLibrary("xvs.library", 33)));
              xvsBase = xvsbase;
	  }

          if(!Args.log || (log = Open(Args.log, MODE_READWRITE)))
          {
	    struct FileData fd;
	    struct Process *task;
	    APTR win;
	
	    task = (struct Process *) FindTask(0);
	    win = task->pr_WindowPtr;
	    task->pr_WindowPtr = (APTR) -1;
	    /* prevent dos requests */

	    if(log)
	      SetFileSize(log, 0, OFFSET_BEGINNING);

	    memset(&fd, 0, sizeof(struct FileData));
    	    fd.fd_Flags = flags;
    	    fd.fd_LogFileFH = log;
	
	    error = DoDirectoryScan(Args.from, Args.save, &fd);
	    task->pr_WindowPtr = win;

            if(log)
              Close(log);
          }

	  if(xvsbase)
	    CloseLibrary((struct Library *) xvsbase);
          FreeArgs(rda);
        }
        FreeDosObject(DOS_RDARGS, rda);
      }
      CloseLibrary((struct Library *) xfdmasterbase);
    }
    CloseLibrary((struct Library *) dosbase);
  }

  return (error ? RETURN_FAIL : RETURN_OK);
}

/* This scans a directory and calls DoFileOpen for every file. It
   automatically creates SAVE destination directories when necessary. */
LONG DoDirectoryScan(STRPTR name, STRPTR sav, struct FileData *fd)
{
  struct AnchorPath *APath;
  LONG error = XFDCERR_SCANERR;
  ULONG retval;

  if(!(fd->fd_Flags & XFDCHECKFLAG_SAVE) || !sav ||
  (fd->fd_SaveDirL = Lock(sav, SHARED_LOCK)))
  {
    if((APath = (struct AnchorPath *) AllocMem(sizeof(struct AnchorPath) +
    512, MEMF_PUBLIC|MEMF_CLEAR)))
    {
      fd->fd_Name = APath->ap_Buf;
      APath->ap_Strlen = 256;
      for(retval = MatchFirst(name, APath); !retval;
      retval = MatchNext(APath))
      {
        if(APath->ap_Flags & APF_DIDDIR)
        {
          OpenParentDir(fd);
          APath->ap_Flags &= ~APF_DIDDIR; /* clear flag */
        }
        else if(APath->ap_Info.fib_DirEntryType > 0)
        {
          if(fd->fd_Flags & XFDCHECKFLAG_ALL)
          {
            OpenNewDir(fd, APath->ap_Info.fib_FileName);
            APath->ap_Flags |= APF_DODIR;
          }
        }
        else
        {
          PrintXFDErr(fd, DoFileOpen(fd));
	  fd->fd_Flags &= XFDCALLFLAGS;
          fd->fd_LinkNum = 0;

          while(fd->fd_MemList)
            FreeCrunchMemList(fd, fd->fd_MemList->cml_MemoryRegion);
        }
        if((fd->fd_Flags & XFDCHECKFLAG_SAVE) && !fd->fd_SaveDirL)
        {
          error = XFDCERR_OPENDIR; break;
        }
        if(CTRL_C)
        {
          error = XFDCERR_BREAK; break;
        }
      }
      MatchEnd(APath);

      if(retval == ERROR_NO_MORE_ENTRIES)
        error = 0;

      FreeMem(APath, sizeof(struct AnchorPath) + 512);
    }
    else
      error = XFDCERR_NOMEMORY;

    if(sav && fd->fd_SaveDirL)
      UnLock(fd->fd_SaveDirL);
  }
  else
    error = XFDCERR_OPENDIR;

  return error;
}

/* Open a file and call DoFileUnArchive to scan */
LONG DoFileOpen(struct FileData *fd)
{
  struct FileInfoBlock *fib;
  LONG ret = 0;

  if((fib = (struct FileInfoBlock *) AllocDosObject(DOS_FIB, 0)))
  {
    ULONG fh;

    if(fd->fd_ArchiveDepth) /* prevent scan errors */
      SetProtection(fd->fd_Name, 0); /* set RWED bits */
    if((fh = Open(fd->fd_Name, MODE_OLDFILE)))
    {
      if(ExamineFH(fh, fib))
      {
        APTR mem;

	if(!fib->fib_Size)
          ;
        else if((mem = AllocMem(fib->fib_Size, MEMF_ANY)))
        {
          if(Read(fh, mem, fib->fib_Size) != fib->fib_Size)
            ret = XFDCERR_READWRITE;
          else if(!(ret = AddCrunchMemList(fd, mem, fib->fib_Size)))
          {
            if((fd->fd_Flags & XFDCHECKFLAG_PRINTALL) ||
            ((fd->fd_Flags & XFDCHECKFLAG_PRINTEXEC) && fib->fib_Size > 4
            && *((ULONG *)mem) == HUNK_HEADER))
              PrintXFDFile(fd);
            ret = DoFileUnArchive(fd, mem, fib->fib_Size);
          }
        }
        else
          ret = XFDCERR_NOMEMORY;
      }
      else
        ret = XFDCERR_EXAMINEERR;
      Close(fh);
    }
    else
      ret = XFDCERR_OPENERR;
  
    FreeDosObject(DOS_FIB, fib);
  }
  else
    ret = XFDCERR_NOMEMORY;

  return ret;
}

/* for SAVE option: open parent directory and try to delete the directory
   we leave. This only succeeds, when the directory is empty. */
ULONG OpenParentDir(struct FileData *fd)
{
  ULONG g;
  UBYTE name[300];

  if((g = fd->fd_SaveDirL))
  {
    NameFromLock(g, name, 300);
    fd->fd_SaveDirL = ParentDir(g);
    UnLock(g);

    DeleteFile(name);
  }
  else
    return -1;

  return fd->fd_SaveDirL;
}

/* Open a new subdirectory for SAVE option */
ULONG OpenNewDir(struct FileData *fd, STRPTR name)
{
  ULONG g;

  if(fd->fd_SaveDirL)
  {
    g = CurrentDir(fd->fd_SaveDirL);
    if(!(fd->fd_SaveDirL = Lock(name, SHARED_LOCK)))
      if((fd->fd_SaveDirL = CreateDir(name)))
        ChangeMode(CHANGE_LOCK, fd->fd_SaveDirL, SHARED_LOCK);
    UnLock(CurrentDir(g));
  }
  else
    return -1;

  return fd->fd_SaveDirL;
}

void DoMount(ULONG deep, ULONG hd)
{
  ULONG fh;
  UBYTE buf[50], buf2[60];

  sprintf(buf, "T:CheckMountList_%08lx_%03ld", FindTask(0), deep);

  if((fh = Open(buf, MODE_NEWFILE)))
  {
    FPrintf(fh,
    "R%lc%ld:\n"
    "\tDevice\t\t= ramdrive.device\n"
    "\tUnit\t\t= %3ld\n"
    "\tSurfaces\t=   2\n"
    "\tBlocksPerTrack\t=  %ld\n"
    "\tReserved\t=   2\n"
    "\tInterleave\t=   0\n"
    "\tLowCyl\t\t=   0\n"
    "\tHighCyl\t\t=  79\n"
    "\tBootPri\t\t= -10\n"
    "\tBuffers\t\t=   5\n"
    "\tBufMemType\t=   1\n#\n", hd ? 'H' : 'D', deep, (hd ? 50 : 10) + deep,
    hd ? 22 : 11);
    Close(fh);

    sprintf(buf2, "Mount R%lc%ld: FROM %s", hd ? 'H' : 'D', deep, buf);
    /* I could do this myself, but I don't know, if it is the correct
       method, so I use Mount command. */
    CallSystem(buf2, 0);
    DeleteFile(buf);
  }
}

/* Try to get RDx: disk and initialize it. */
BPTR GetRad(struct FileData *fd, STRPTR rname)
{
  UBYTE name[20];
  sprintf(name, NAME "%03ld", fd->fd_ArchiveDepth);

  if(Inhibit(rname, 1))
  {
    Format(rname, name, ID_DOS_DISK);
    Inhibit(rname, 0);
    return Lock(rname, SHARED_LOCK);
  }
  else
  {
    LONG i, m = 0;
    UBYTE name2[20];
    struct DosList *dosl;
    if((dosl = LockDosList(LDF_DEVICES|LDF_READ)))
    {
      i = strlen(rname);
      name2[--i] = 0;
      while(i--)
        name2[i] = rname[i];
      dosl = FindDosEntry(dosl, name2, LDF_DEVICES);
      UnLockDosList(LDF_DEVICES|LDF_READ);
      if(!dosl && (fd->fd_Flags & XFDCHECKFLAG_AUTOMOUNT))
      {
	DoMount(fd->fd_ArchiveDepth, fd->fd_Flags &
	XFDCHECKFLAG_HIGHDENSITY);
        m = 1;
      }
      for(i = 0; i < 50; ++i)
      {
        if(Inhibit(rname, 1))
        {
          Format(rname, name, ID_DOS_DISK);
          Inhibit(rname, 0);
          return Lock(rname, SHARED_LOCK);
        }
        Delay(10);
      }
      /* clear flag if mount failed */
      if(m)
        fd->fd_Flags &= ~XFDCHECKFLAG_AUTOMOUNT;
    }
  }

  return 0;
}

/* Test if we need to copy the archive to a temporary file. This is
   necessary, because most archivers are not able to handle archives
   correctly, when there are pattern characters in the path or archive
   name. Also they fail, when the archive name has an incorrect extension.

   For Archivers like Zoo and Arc, which dearchive always to current
   directory, we need to copy always, as the arcname must have an absolute
   path, which is not guarranteed for argument path name, as this may be
   relative to current directory.
   --> except we are already in a decrunched archive, as here the name is
   garanted to be absolute, e.g. start with ':'.
     
   Archives which are crunched with an alien cruncher or are linked must be
   copied as well. (If this really exists ???)

   In these cases the archive file is copied with a correct filename and
   extension, so the archiver works well.
   Problem: This needs mem (or disk space when T: is assigned to hard
   disk).
*/
LONG DoArcNameCheck(struct UnArcData *uad, STRPTR mem, ULONG buflength,
ULONG arc, struct FileData *fd)
{
  STRPTR p;
  LONG err = 0;

  for(p = uad->uad_ArcName; *p; ++p)
  {
    if(*p == '#' || *p == '(' || *p == ')' || *p == '[' || *p == ']'
    || *p == '*' || *p == '~' || *p == '#' || *p == '?' || *p == '|')
      fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
  }
  if((fd->fd_Flags & (XFDCHECKFLAG_CRUNCHED|XFDCHECKFLAG_LINKED)) ||
  ((fd->fd_Flags & XFDCHECKFLAG_DIRCHANGE) && !fd->fd_ArchiveDepth))
    fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
  else if(!(fd->fd_Flags & XFDCHECKFLAG_ARCCOPY))
  {
    if((p -= 4) <= uad->uad_ArcName) /* position of extension */
      p = uad->uad_ArcName+1; /* at least one char as name is needed */
    switch(arc)
    {
    case ARCTYPE_LHA:
      if(stricmp(p, ".lha") && stricmp(p, ".lzh"))
        fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
      break;
    case ARCTYPE_LZX:
      if(stricmp(p, ".lzx")) fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
      break;
    case ARCTYPE_ZIP:
      if(stricmp(p, ".zip")) fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
      break;
    case ARCTYPE_LHA_SFX:
      if(stricmp(p, ".run")) fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
      break;
    case ARCTYPE_DMS:
      if(stricmp(p, ".dms")) fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
      break;
    case ARCTYPE_ARC:
      if(stricmp(p, ".arc")) fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
      break;
    case ARCTYPE_ZOO:
      if(stricmp(p, ".zoo")) fd->fd_Flags |= XFDCHECKFLAG_ARCCOPY;
      break;
    }            
  }
  if(fd->fd_Flags & XFDCHECKFLAG_ARCCOPY)
  {
    ULONG d;
    switch(arc)
    {
    case ARCTYPE_LHA:
      p = ".lha"; break;
    case ARCTYPE_LZX:
      p = ".lzx"; break;
    case ARCTYPE_ZIP:
      p = ".zip"; break;
    case ARCTYPE_ZOO:
      p = ".zoo"; break;
    case ARCTYPE_LHA_SFX:
      p = ".run"; break;
    case ARCTYPE_ARC:
      p = ".arc"; break;
    case ARCTYPE_DMS:
      p = ".dms"; break;
    case ARCTYPE_ZOOM:
      p = ".zom"; break;
    }            
    sprintf(uad->uad_CopyArcName, "T:CheckArc_%08lx_%03ld%s", FindTask(0),
    fd->fd_ArchiveDepth, p);
    uad->uad_ArcName = uad->uad_CopyArcName;
    if((d = Open(uad->uad_ArcName, MODE_NEWFILE)))
    {
      if(Write(d, mem, buflength) != buflength)
        err = XFDCERR_READWRITE;
      Close(d);
    }
    else
      err = XFDCERR_OPENERR;
  }

  return err;
}
/* Call command with NIL: input and output. */
LONG CallSystem(STRPTR com, ULONG loud)
{
  ULONG infh;
  LONG err = 0;

  if((infh = Open("NIL:", MODE_OLDFILE)))
  {
    ULONG outfh;
    if((outfh = Open("NIL:", MODE_NEWFILE)))
    {
      ULONG i;
      if(loud)
        Flush(Output());

      if((i = SystemTags(com, SYS_Input, infh, loud ? TAG_IGNORE :
      SYS_Output, outfh, TAG_DONE)))
      {
        if(i == RETURN_ERROR && IoErr() == ERROR_OBJECT_NOT_FOUND)
	  err = XFDCERR_NOUNARCER;
	else if(i <= RETURN_ERROR)
	  err = XFDCERR_UNARCERR;
	else
	  err = XFDCERR_UNARCFAIL;
      }
      Close(outfh);
    }
    else
      err = XFDCERR_RESOURCE;
    Close(infh);
  }
  else
    err = XFDCERR_RESOURCE;

  return err;
}

/* Tests if a file is an archive. When yes, the archive is unarchived and
   DoDirectoryScan is called for the result files. Else DoFileUnLink is
   called.
*/
LONG DoFileUnArchive(struct FileData *fd, APTR buffer, ULONG buflength)
{
  ULONG arc = 0;
  STRPTR mem = (STRPTR) buffer;
  LONG err = 0;

  if(buflength > 7 && mem[2] == '-' && mem[3] == 'l' && mem[4] == 'h'
  && mem[6] == '-')
  { /* This is an LhA-File */
    arc = ARCTYPE_LHA;
    PrintXFDTxt(fd, "LhA-Archive");
  }
  else if(buflength > 4 && mem[0] == 'L' && mem[1] == 'Z' && mem[2] == 'X'
  && !mem[3])
  { /* This is an LzX-File */
    arc = ARCTYPE_LZX;
    PrintXFDTxt(fd, "LzX-Archive");
  }
  else if(buflength > 4 && mem[0] == 'P' && mem[1] == 'K' && mem[2] == 3
  && mem[3] == 4)
  {
    arc = ARCTYPE_ZIP;
    PrintXFDTxt(fd, "Zip-Archive");
  }
  else if(buflength > 4 && mem[0] == 'Z' && mem[1] == 'O' && mem[2] == 'O'
  && mem[3] == ' ' && mem[5] == '.')
  {
    arc = ARCTYPE_ZOO;
    fd->fd_Flags |= XFDCHECKFLAG_DIRCHANGE;
    PrintXFDTxt(fd, "ZOO-Archive");
  }
  else if(buflength > 4 && mem[0x2C] == 'S' && mem[0x2D] == 'F' &&
  mem[0x2E] == 'X' && mem[0x2F] == '!' && *((ULONG *)mem) == 0x3F3)
  {
    arc = ARCTYPE_LHA_SFX;
    PrintXFDTxt(fd, "LhA-SFX-Archive");
  }
  else if(buflength > 2 && mem[0] == 0x1A && mem[1] && mem[1] < 9)
  { /* This detection is very unsecure !!! */
    arc = ARCTYPE_ARC;
    fd->fd_Flags |= XFDCHECKFLAG_DIRCHANGE;
    PrintXFDTxt(fd, "Arc-Archive");
  }
  else if(buflength > 20 && mem[0] == 'D' && mem[1] == 'M' && mem[2] == 'S'
  && mem[3] == '!')
  {
    STRPTR ad = "";

    arc = ARCTYPE_DMS;

    if(((ULONG *) mem)[2] & (1<<4))
    {
      ad = " HighDensity"; fd->fd_Flags |= XFDCHECKFLAG_HIGHDENSITY;
      arc = 0;
    }

    if(mem[17] || mem[19] != 79)
    {
      PrintXFDTxt(fd, "DMS-Archive%s (cutted: %02ld to %02ld)", ad, mem[17],
      mem[19]);
      if(fd->fd_Flags & XFDCHECKFLAG_NOTRACKCUT)
        arc = 0;
    }
    else
      PrintXFDTxt(fd, "DMS-Archive%s", ad);
  }
  else if(buflength > 4 && mem[0] == 'Z' && mem[1] == 'O' && mem[2] == 'M'
  && mem[3] == '5')
  {
    arc = ARCTYPE_ZOOM;
    PrintXFDTxt(fd, "Zoom-Archive");
  }

  if(arc)
  {
    if((arc < 100 && !(fd->fd_Flags & XFDCHECKFLAG_NOUNARCHIVE)) ||
    (arc >= 100 && !(fd->fd_Flags & XFDCHECKFLAG_NOUNTRACK)))
    {
      struct UnArcData *uad;

      if((uad = (struct UnArcData *) AllocMem(sizeof(struct UnArcData),
      MEMF_ANY)))
      { /* AllocMem to prevent stack overflow */

	uad->uad_ArcName = fd->fd_Name;
	err = DoArcNameCheck(uad, mem, buflength, arc, fd);

        FreeCrunchMemList(fd, mem);
        if(!err && !OpenNewDir(fd, FilePart(fd->fd_Name)))
	  err = XFDCERR_OPENERR;
        if(!err)
        {
          ULONG destdir, storedir = 0;

	  if(arc > 100)
	  {
	    if(fd->fd_Flags & XFDCHECKFLAG_HIGHDENSITY)
              sprintf(uad->uad_DestDir, "RH%ld:", fd->fd_HDDepth);
	    else
              sprintf(uad->uad_DestDir, "RD%ld:", fd->fd_DDDepth);
            strcpy(uad->uad_DoDestDir, uad->uad_DestDir);

            if((destdir = GetRad(fd, uad->uad_DestDir)))
            {
	      UnLock(destdir);

              switch(arc)
	      {
	      case ARCTYPE_DMS:
	        sprintf(uad->uad_Command, "DMS WRITE \"%s\" TO %s%s",
	        uad->uad_ArcName, uad->uad_DestDir, fd->fd_Flags &
	        XFDCHECKFLAG_HIGHDENSITY ? " HD" : "");
	        break;
	      case ARCTYPE_ZOOM:
	        sprintf(uad->uad_Command, "ZOOM NOPROMPT FROM \"%s\" TO %s",
	        uad->uad_ArcName, uad->uad_DestDir);
	        break;
	      }

              if((err = CallSystem(uad->uad_Command, fd->fd_Flags &
              XFDCHECKFLAG_LOUD) == XFDCERR_UNARCERR))
              {
                PrintXFDErr(fd, err); err = 0;
              }

	      if(!(destdir = Lock(uad->uad_DestDir, SHARED_LOCK)) && !err)
                err = XFDCERR_NODOS;
            }
	    else
	      err = XFDCERR_NORAD;
	  }
	  else
	  {
            sprintf(uad->uad_DestDir, "T:CheckX_%08lx_%03ld",
            FindTask(0), fd->fd_ArchiveDepth);
            sprintf(uad->uad_DoDestDir, "T:CheckX_%08lx_%03ld/#?",
            FindTask(0), fd->fd_ArchiveDepth);

	    if((destdir = CreateDir(uad->uad_DestDir)))
	    {
	      ChangeMode(CHANGE_LOCK, destdir, SHARED_LOCK);
	      if(fd->fd_Flags & XFDCHECKFLAG_DIRCHANGE)
                storedir = CurrentDir(destdir);

              switch(arc)
	      {
	      case ARCTYPE_LHA: case ARCTYPE_LHA_SFX:
	        sprintf(uad->uad_Command, "Lha -IMmq x \"%s\" %s/",
	        uad->uad_ArcName, uad->uad_DestDir);
	        break;
	      case ARCTYPE_LZX:
	        sprintf(uad->uad_Command, "LzX -m x \"%s\" %s/",
	        uad->uad_ArcName, uad->uad_DestDir);
	        break;
	      case ARCTYPE_ZIP:
	        sprintf(uad->uad_Command, "UnZip \"%s\" -d %s/",
	        uad->uad_ArcName, uad->uad_DestDir);
	        break;
	      case ARCTYPE_ZOO:
	        sprintf(uad->uad_Command, "Zoo x//qqq \"%s\"",
	        uad->uad_ArcName);
	        break;
	      case ARCTYPE_ARC:
	        sprintf(uad->uad_Command, "Arc xwn \"%s\"",
	        uad->uad_ArcName);
	        break;
	      }

              if((err = CallSystem(uad->uad_Command, fd->fd_Flags &
              XFDCHECKFLAG_LOUD) == XFDCERR_UNARCERR))
              {
                PrintXFDErr(fd, err); err = 0;
              }
            }
	    else
	      err = XFDCERR_OPENDIR;
          }
  	  if(fd->fd_Flags & XFDCHECKFLAG_ARCCOPY)
  	  { /* save some memory */
	    fd->fd_Flags &= ~XFDCHECKFLAG_ARCCOPY;
	    DeleteFile(uad->uad_ArcName);
	  }
	  if(destdir)
	  {
	    if(!err)
	    {
	      struct FileData fdp;

	      CopyMem(fd, &fdp, sizeof(struct FileData));
	      fdp.fd_Flags = (fdp.fd_Flags&XFDCALLFLAGS)|XFDCHECKFLAG_ALL;
	      ++fdp.fd_ArchiveDepth;
	      ++fdp.fd_RecurseDepth;
	      fdp.fd_MemList = 0;

	      if(arc > 100)
	      {
	        if(fd->fd_Flags & XFDCHECKFLAG_HIGHDENSITY)
	          ++fdp.fd_HDDepth;
	        else
	          ++fdp.fd_DDDepth;
	      }

	      err = DoDirectoryScan(uad->uad_DoDestDir, 0, &fdp);
	      fd->fd_SaveDirL = fdp.fd_SaveDirL;
	    }

	    if(fd->fd_Flags & XFDCHECKFLAG_DIRCHANGE)
	      CurrentDir(storedir);
	    UnLock(destdir);
	    if(arc < 100)
	      DeleteAll(uad->uad_DestDir);
	  }

          if(!OpenParentDir(fd))
	    err = XFDCERR_OPENDIR;
        }
  	if(fd->fd_Flags & XFDCHECKFLAG_ARCCOPY)
	  DeleteFile(uad->uad_ArcName);

        FreeMem(uad, sizeof(struct UnArcData));
      }
      else
        err = XFDCERR_NOMEMORY;
    }
  }
  else
    err = DoGetVirus(fd, buffer, buflength);

  return err;
}

LONG DoGetVirus(struct FileData *fd, APTR buffer, ULONG buflength)
{
  if(xvsBase)
  {
    APTR mem;
      
    if((mem = AllocMem(buflength, MEMF_ANY)))
    {
      struct xvsFileInfo *fi;
      if((fi = (struct xvsFileInfo *) xvsAllocObject(XVSOBJ_FILEINFO)))
      {
        ULONG i;
	/* xvs may modify the buffer! */
        CopyMem(buffer, mem, buflength);
        fi->xvsfi_File = mem;
        fi->xvsfi_FileLen = buflength;
        i = xvsCheckFile(fi);
        if(i == XVSFT_DATAVIRUS)
 	  PrintXFDTxt(fd, "Data-Virus '%s'", fi->xvsfi_Name);
        else if(i == XVSFT_FILEVIRUS)
 	  PrintXFDTxt(fd, "File-Virus '%s'", fi->xvsfi_Name);
        else if(i == XVSFT_LINKVIRUS)
 	  PrintXFDTxt(fd, "Link-Virus '%s'", fi->xvsfi_Name);
        xvsFreeObject(fi);
      }
      else
        PrintXFDErr(fd, XFDCERR_NOMEMORY);
      FreeMem(mem, buflength);
    }
    else
      PrintXFDErr(fd, XFDCERR_NOMEMORY);
  }

  return DoFileUnLink(fd, buffer, buflength);
}

/* Tries to unlink a file. When the file was linked, we call DoFileUnArchive
   for the two parts to check if they may be archives, else we call
   DoFileUnCrunch.
*/
LONG DoFileUnLink(struct FileData *fd, APTR buffer, ULONG buflength)
{
  LONG ret = XFDCERR_NOMEMORY;
  struct xfdLinkerInfo *xli;

  if((xli = (struct xfdLinkerInfo *) xfdAllocObject(XFDOBJ_LINKERINFO)))
  {
    xli->xfdli_Buffer = buffer;
    xli->xfdli_BufLen = buflength;
    if(xfdRecogLinker(xli))
    {
      PrintXFDTxt(fd, xli->xfdli_LinkerName);
      if(fd->fd_Flags & XFDCHECKFLAG_NOUNLINK)
        ret = DoFileUnCrunch(fd, buffer, buflength);
      else if(xfdUnlink(xli))
      {
        ULONG flags;
	fd->fd_Flags |= XFDCHECKFLAG_LINKED;
        ++fd->fd_RecurseDepth;
        ++fd->fd_LinkNum;
	flags = fd->fd_Flags;
	fd->fd_Flags |= XFDCHECKFLAG_NOFREEMEM;
        PrintXFDErr(fd, DoFileUnArchive(fd, xli->xfdli_Save1,
        xli->xfdli_SaveLen1));
        fd->fd_Flags = flags; /* XFDCHECKFLAG_NOFREEMEM is cleared */
        PrintXFDErr(fd, DoFileUnArchive(fd, xli->xfdli_Save2,
        xli->xfdli_SaveLen2));
        ret = 0;
        --fd->fd_RecurseDepth;
      }
      else
        ret = xli->xfdli_Error;
    }
    else
      ret = DoFileUnCrunch(fd, buffer, buflength);

    xfdFreeObject(xli);
  }
  return ret;
}

/* Tries to decrunch a file. When it is crunched, we decrunch it and call
   DoFileUnArchive to start the loop again. Else we call unstripping.
*/
LONG DoFileUnCrunch(struct FileData *fd, APTR buffer, ULONG buflength)
{
  LONG ret = XFDCERR_NOMEMORY;
  struct xfdBufferInfo *xbi;

  if((xbi = (struct xfdBufferInfo *) xfdAllocObject(XFDOBJ_BUFFERINFO)))
  {
    xbi->xfdbi_SourceBuffer = buffer;
    xbi->xfdbi_SourceBufLen = buflength;
    xbi->xfdbi_Flags = XFDFF_RECOGEXTERN;
    if(xfdRecogBuffer(xbi))
    {
      struct Library *xpkbase;
      STRPTR buf = 0;
      ULONG buflen = 0;

      PrintXFDTxt(fd, xbi->xfdbi_PackerFlags & XFDPFF_ADDR ? "%s (ADDRESS)" :
      "%s", xbi->xfdbi_PackerName);

      if(fd->fd_Flags & XFDCHECKFLAG_ASKPWD && (xpkbase =
      OpenLibrary(XPKNAME, 4)))
      {
	ASSIGN_XPK
        if(xbi->xfdbi_PackerFlags & XFDPFF_PASSWORD)
        {
          buflen = (xbi->xfdbi_MaxSpecialLen == 0xFFFF) ? 256 :
	    xbi->xfdbi_MaxSpecialLen;
          if((buf = (STRPTR) AllocMem(buflen, MEMF_ANY|MEMF_CLEAR)))
	  {
            if(!XpkPassRequestTags(XPK_PasswordBuf, buf,
            XPK_PassBufSize, buflen, TAG_DONE))
	      xbi->xfdbi_Special = buf;
	  }
        }
        else if(xbi->xfdbi_PackerFlags & XFDPFF_KEY16)
        {
	  if(!XpkPassRequestTags(XPK_Key16BitPtr, &buflen, TAG_DONE))
	    xbi->xfdbi_Special = &buflen;
        }
        else if(xbi->xfdbi_PackerFlags & XFDPFF_KEY32)
        {
	  if(!XpkPassRequestTags(XPK_Key32BitPtr, &buflen, TAG_DONE))
            xbi->xfdbi_Special = &buflen;
        }
	CloseLibrary(xpkbase);
      }
      if(fd->fd_Flags & XFDCHECKFLAG_NODECRUNCH)
        ret = DoFileStrip(fd, buffer, buflength);
      else if(xfdDecrunchBuffer(xbi))
      {
        if((xbi->xfdbi_PackerFlags & XFDPFF_ADDR) &&
        !(fd->fd_Flags & XFDCHECKFLAG_ADDRESS))
        {
	  fd->fd_Flags |= XFDCHECKFLAG_ADDRESS;
          PrintXFDErr(fd, SaveUncrFile(fd, buffer, buflength));
        }
	fd->fd_Flags |= XFDCHECKFLAG_CRUNCHED;
        FreeCrunchMemList(fd, buffer);
        ++fd->fd_RecurseDepth;
        if(!(ret = AddCrunchMemList(fd, xbi->xfdbi_TargetBuffer,
        xbi->xfdbi_TargetBufLen)))
        {
          PrintXFDErr(fd, DoFileUnArchive(fd, xbi->xfdbi_TargetBuffer,
            xbi->xfdbi_TargetBufSaveLen));
        }
        --fd->fd_RecurseDepth;
      }
      else
        ret = xbi->xfdbi_Error;

      if(buf)
        FreeMem(buf, buflen);
    }
    else
      ret = DoFileStrip(fd, buffer, buflength);

    xfdFreeObject(xbi);
  }
  return ret;
}

/* Tries to strip useless hunks in a file. When there are some, we remove
   them and call DoFileUnArchive to start the loop again. Else we finish.
   When either unlinking or uncrunching happend in before loops, we may
   save the file when there was SAVE option.
*/
LONG DoFileStrip(struct FileData *fd, APTR buffer, ULONG buflength)
{
  LONG ret = 0;
  ULONG reslength = buflength;

  if(*((ULONG *) buffer) == 0x000003F3 && !(fd->fd_Flags &
  XFDCHECKFLAG_NOSTRIP))
    xfdStripHunks(buffer, buflength, &reslength,
    XFDSHF_NAME|XFDSHF_SYMBOL|XFDSHF_DEBUG);
    /* errors are not interpreted */

  if(buflength > reslength)
  {
    fd->fd_Flags |= XFDCHECKFLAG_STRIPPED;
    PrintXFDTxt(fd, "%ld bytes stripped", buflength-reslength);
    ret = DoFileUnArchive(fd, buffer, reslength);
  }

  if(!ret && !(fd->fd_Flags & XFDCHECKFLAG_ADDRESS))
    ret = SaveUncrFile(fd, buffer, buflength);
  FreeCrunchMemList(fd, buffer);

  return ret;
}

/* Print file name */
void PrintXFDFile(struct FileData *fd)
{
  STRPTR name = fd->fd_Name;
  UBYTE i;

  if(fd->fd_ArchiveDepth) /* skip that ugly T:CheckX.. or RD?: */
  {
    if(*name == 'R' && name[1] == 'D')
    {
      while(*name != ':')
        ++name;
      ++name;
    }
    else
    {
      while(*name != '/')
        ++name;
      ++name;
    }
  }

  if(fd->fd_LogFileFH)
  {
    for(i = 0; i < fd->fd_ArchiveDepth; ++i)
      FPutC(fd->fd_LogFileFH, '*');
    FPrintf(fd->fd_LogFileFH, "%s\n", name);
  }
  for(i = 0; i < fd->fd_ArchiveDepth; ++i)
    FPutC(Output(), '*');
  Printf("%s\n", name);
  fd->fd_Flags |= XFDCHECKFLAG_NAMEPRINTED;
}

/* Print Error - Error 100 and up are CheckX special errors. */
void PrintXFDErr(struct FileData *fd, LONG err)
{
  if(err)
  {
    STRPTR txt = "", txt2 = " XFD-Error %ld: %s\n"; 
    UBYTE i;

    if(err >= XFDCERR_NOMEMORY && err <= XFDCERR_RESOURCE)
    {
      txt2 = " CheckX-Error %ld: %s\n";
      switch(err)
      {
      case XFDCERR_NOMEMORY:	txt = "not enough memory";	break;
      case XFDCERR_UNARCFAIL:	txt = "unarchiver failed";	break;
      case XFDCERR_UNARCERR:	txt = "unarchiver error";	break;
      case XFDCERR_NOUNARCER:	txt = "unarchiver missing";	break;
      case XFDCERR_EXAMINEERR:	txt = "examining failed";	break;
      case XFDCERR_OPENERR:	txt = "opening file failed";	break;
      case XFDCERR_READWRITE:	txt = "read or write failed";	break;
      case XFDCERR_SCANERR:	txt = "directory scan failed";	break;
      case XFDCERR_BREAK:	txt = "user break";		break;
      case XFDCERR_OPENDIR:	txt = "opening directory failed"; break;
      case XFDCERR_NORAD:	txt = "accessing ramdrive failed"; break;
      case XFDCERR_NODOS:	txt = "not a dos disk";		break;
      case XFDCERR_RESOURCE:	txt = "needed resource not available"; break;
      }
    }
    else
      txt = xfdGetErrorText(err);
    if(!(fd->fd_Flags & XFDCHECKFLAG_NAMEPRINTED))
      PrintXFDFile(fd);
    if(fd->fd_LogFileFH)
    {
      for(i = 0; i < fd->fd_RecurseDepth; ++i)
        FPutC(fd->fd_LogFileFH, ' ');
      FPrintf(fd->fd_LogFileFH, txt2, err, txt);
      Flush(fd->fd_LogFileFH);
    }
    for(i = 0; i < fd->fd_RecurseDepth; ++i)
      FPutC(Output(), ' ');
    Printf(txt2, err, txt);
  }
}

/* Print type text */
void PrintXFDTxt(struct FileData *fd, STRPTR txt, ...)
{
  UBYTE i;

  if(!(fd->fd_Flags & XFDCHECKFLAG_NAMEPRINTED))
    PrintXFDFile(fd);
  if(fd->fd_LogFileFH)
  {
    for(i = 0; i <= fd->fd_RecurseDepth; ++i)
      FPutC(fd->fd_LogFileFH, ' ');
    VFPrintf(fd->fd_LogFileFH, txt, &txt+1);
    FPutC(fd->fd_LogFileFH, '\n');
    Flush(fd->fd_LogFileFH);
  }
  for(i = 0; i <= fd->fd_RecurseDepth; ++i)
    FPutC(Output(), ' ');
  VPrintf(txt, &txt+1);
  FPutC(Output(), '\n');
}

/* Add memory to the memory list. */
LONG AddCrunchMemList(struct FileData *fd, APTR reg, ULONG size)
{
  struct CrunchMemList *ml;

  if((ml = (struct CrunchMemList *) AllocMem(sizeof(struct CrunchMemList),
  MEMF_ANY)))
  {
    ml->cml_Next = fd->fd_MemList;
    ml->cml_MemoryRegion = reg;
    ml->cml_MemorySize = size;
    fd->fd_MemList = ml;
  }
  else
  {
    FreeMem(reg, size);
    return XFDCERR_NOMEMORY;
  }
  return 0;
}

/* Free memory from the memory list. */
void FreeCrunchMemList(struct FileData *fd, APTR reg)
{
  struct CrunchMemList mc, *ml = &mc;

  if(fd->fd_Flags & XFDCHECKFLAG_NOFREEMEM)
    return;

  for(mc.cml_Next = fd->fd_MemList; ml; ml = ml->cml_Next)
  {
    if(ml->cml_Next->cml_MemoryRegion == reg)
    {
      struct CrunchMemList *m;
      m = ml->cml_Next;
      ml->cml_Next = m->cml_Next;
      FreeMem(m->cml_MemoryRegion, m->cml_MemorySize);
      FreeMem(m, sizeof(struct CrunchMemList));
    }
  }

  fd->fd_MemList = mc.cml_Next;
}

/* Save file, when SAVE option and file was crunched or linked. */
LONG SaveUncrFile(struct FileData *fd, APTR buf, ULONG size)
{
  ULONG filefh, cd;
  LONG ret = 0;
  UBYTE name[50];

  if(!(fd->fd_SaveDirL && (fd->fd_Flags & XFDSAVEFLAGS)))
    return 0;

  sprintf(name, (fd->fd_LinkNum ? "%s.%ld" : "%s"), FilePart(fd->fd_Name),
  fd->fd_LinkNum);
  ++fd->fd_LinkNum;

  cd = CurrentDir(fd->fd_SaveDirL);

  if((filefh = Open(name, MODE_NEWFILE)))
  {
    if(Write(filefh, buf, size) != size)
      ret = XFDCERR_READWRITE;
    Close(filefh);
  }
  else
    ret = XFDCERR_OPENERR;
  CurrentDir(cd);

  return ret;
}

/* This deletes a temporary directory and all files in it.
   It returns 0 on error and gets name of directory to delete.
*/
LONG DeleteAll(STRPTR name)
{
  LONG retval;
  struct AnchorPath *APath;
  STRPTR fname;

  if((APath = (struct AnchorPath *) AllocMem(sizeof(struct AnchorPath) +
  512, MEMF_PUBLIC|MEMF_CLEAR)))
  {
    APath->ap_Strlen = 256;

    fname = (STRPTR) APath->ap_Buf + 256;
    for(retval = MatchFirst(name, APath); !retval; retval = MatchNext(APath))
    {
      if(*fname)
      {
        SetProtection(fname, 0); /* set RWED bits */
        DeleteFile(fname);
      }

      CopyMem(APath->ap_Buf, fname, 256);

      if(APath->ap_Flags & APF_DIDDIR)
        APath->ap_Flags &= ~APF_DIDDIR; /* clear flag */
      else if(APath->ap_Info.fib_DirEntryType > 0) /* a directory */
      {
        APath->ap_Flags |= APF_DODIR; *fname = 0;
      }
    }
    MatchEnd(APath);

    FreeMem(APath, sizeof(struct AnchorPath) + 512);
  }

  return DeleteFile(name); /* when this returns error, then any of the
  above functions may have failed --> directory not empty. */
}

