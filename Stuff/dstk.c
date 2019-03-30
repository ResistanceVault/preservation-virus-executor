/* dstk - dump registers & stack of running task(s)

  notes:
  - only 68060 FPU code is currently tested, 68881 was tested with UAE.
  - similarities to CBM (rest in peace) tool tstat are purely coincidental.
  - if TASKS/M argument is omitted, all tasks are dumped.
  - PC, SR, data & address registers are always dumped.
  - FPU registers are dumped, if available.
  - TASKS/M can be:
     o task name
     o cli number
     o cli command name
     o hex task address
  - dumping self is not possible, figure it out why. :)
  - DUMPSTACK/S make the program dump used stack area as hexdump.
  - *CHECK/S enable address resolving for specific item, uses SegTracker
    if available.
  - ALLCHECK/S enable cheking everything.
  - VERBOSE/S make the program dump some extra information.
*/

#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include <exec/types.h>
#include <exec/tasks.h>
#include <exec/resident.h>
#include <exec/execbase.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>

#include <clib/alib_protos.h>

extern struct ExecBase *SysBase;

typedef const char (* __asm SegTrack(register __a0 ULONG,
                                     register __a1 ULONG *,
                                     register __a2 ULONG *));

struct SegSem
{
  struct SignalSemaphore seg_Semaphore;
  SegTrack              *seg_Find;
};


#define OUTBUFSIZE 4096

struct OutNode
{
  struct MinNode   ob_Node;
  unsigned int     ob_Index;
  unsigned char    ob_Buf[OUTBUFSIZE];
};

/* FPU stack frame types
*/
typedef enum
{
  FRAMETYPE_NOFPU,
  FRAMETYPE_FPU881,
  FRAMETYPE_FPU882,
  FRAMETYPE_FPU040,
  FRAMETYPE_FPU060,
  FRAMETYPE_NUMOF
} FRAMETYPE;

static const char * const fpufts[FRAMETYPE_NUMOF] =
{
  "no fpu",
  "68881",
  "68882",
  "FPU040",
  "FPU060"
};


/* protos
*/
int main(void);
void getresname(const struct Resident *res, unsigned char *buf, int len);
void getnodename(const struct Node *node, unsigned char *buf, int len);
void getlibname(const struct Library *lib, unsigned char *buf, int len);
void gettaskname(const struct Task *task, unsigned char *buf, int len);
void getcustomname(ULONG offs, unsigned char *buf, int len);
void getcianame(ULONG offs, unsigned char *buf, int len);
int chktask(struct Task *task, ULONG addr);
void DumpSeg(ULONG addr);
void *dumpfpuregs(void *sp);
void *dumpcpuregs(void *sp);
void dumptask(struct Task *task);
void PrintP(char *fmt, void *arg);
void PrintO(char *fmt, ...);
void DumpO(void);


/* globals
*/
struct MinList outList;

struct Task *thistask;
struct SegSem *segSem;

FRAMETYPE frametype;

#define TEMPLATE "TASKS/M,DUMPSTACK/S,PCCHECK/S,DREGCHECK/S,AREGCHECK/S,STACKCHECK/S,ALLCHECK/S,VERBOSE/S"
struct args
{
  const char **ARG_TASKS;
  LONG         ARG_DUMPSTACK;
  LONG         ARG_PCCHECK;
  LONG         ARG_DREGCHECK;
  LONG         ARG_AREGCHECK;
  LONG         ARG_STACKCHECK;
  LONG         ARG_ALLCHECK;
  LONG         ARG_VERBOSE;
} args;


const char verstag[] = "$VER: dstk 0.1.0 (16.08.2001) by Harry \"Piru\" Sintonen <sintonen@iki.fi>";

int main(void)
{
  struct RDArgs *rdargs;
  const char **tp, *t;

  struct Task *task;
  struct Node *node, *find;

  /* process args
  */
  memset(&args, 0, sizeof(args));
  rdargs = ReadArgs(TEMPLATE, (LONG *) &args, NULL);
  if (!rdargs)
  {
    PrintFault(IoErr(), NULL);

    return RETURN_ERROR;
  }
  if (args.ARG_ALLCHECK)
  {
    args.ARG_PCCHECK    = -1;
    args.ARG_DREGCHECK  = -1;
    args.ARG_AREGCHECK  = -1;
    args.ARG_STACKCHECK = -1;
  }

  thistask = FindTask(NULL);
  NewList((struct List *) &outList);

  frametype = FRAMETYPE_NOFPU;
  if (SysBase->AttnFlags & AFF_PRIVATE)
  {
    if ((SysBase->AttnFlags & 0x80) && (SysBase->AttnFlags & AFF_FPU40))
    {
      frametype = FRAMETYPE_FPU060;
    }
    else if ((SysBase->AttnFlags & AFF_68040) && (SysBase->AttnFlags & AFF_FPU40))
    {
      frametype = FRAMETYPE_FPU040;
    }
    else if (SysBase->AttnFlags & AFF_68882)
    {
      frametype = FRAMETYPE_FPU882;
    }
    else if (SysBase->AttnFlags & AFF_68881)
    {
      frametype = FRAMETYPE_FPU881;
    }
  }

  if (args.ARG_VERBOSE)
  {
    Printf("FPU frame: %s\n", fpufts[frametype]);
  }

  /* Avoid PatchWork hit
  */
  Forbid();
  segSem = (struct SegSem *) FindSemaphore("SegTracker");
  Permit();

  tp = args.ARG_TASKS;

  Forbid();

  if (tp)
  {
    while ((t = *tp++))
    {
      /* first try to locate the task by name
      */
      task = FindTask(t);
      if (!task)
      {
        LONG val;
        /* try locating by CLI number
        */
        if (StrToLong(t, &val) > -1)
        {
          task = (struct Task *) FindCliProc(val);
        }
        if (!task)
        {
          LONG maxcli;
          unsigned char buf[64];

          /* try locating by CLI command name
          */
          maxcli = MaxCli();
          for (val = 1; val <= maxcli; val++)
          {
            node = (struct Node *) FindCliProc(val);
            gettaskname((struct Task *) node, buf, -sizeof(buf));
            if (!stricmp(t, buf))
            {
              task = (struct Task *) node;
              break;
            }
          }

          if (!task)
          {
            /* direct task address
            */
            if ((t[0] == '0') && ((t[1] & 0xdf) == 'X'))
            {
              t += 2;
            }
            else if (t[0] == '$')
            {
              t += 1;
            }
            find = (struct Node *) strtoul(t, NULL, 16);

            /* try finding the task from ready/wait list
            */
            node = (struct Node *) &SysBase->TaskReady.lh_Head;
            while ((node = node->ln_Succ)->ln_Succ)
            {
              if (node == find)
              {
                task = (struct Task *) node;
                break;
              }
            }
            if (!task)
            {
              node = (struct Node *) &SysBase->TaskWait.lh_Head;
              while ((node = node->ln_Succ)->ln_Succ)
              {
                if (node == find)
                {
                  task = (struct Task *) node;
                  break;
                }
              }
            }
          }
        }
      }

      dumptask(task);
    }
  }
  else
  {
    /* dump all tasks, except self
    */
    node = (struct Node *) &SysBase->TaskReady.lh_Head;
    while ((node = node->ln_Succ)->ln_Succ)
    {
      dumptask((struct Task *) node);
    }
    node = (struct Node *) &SysBase->TaskWait.lh_Head;
    while ((node = node->ln_Succ)->ln_Succ)
    {
      dumptask((struct Task *) node);
    }
  }

  Permit();

  /* dump possible pending text
  */
  DumpO();

  FreeArgs(rdargs);

  return RETURN_OK;
}


void getresname(const struct Resident *res, unsigned char *buf, int len)
{
  const unsigned char *name;
  unsigned char *p;
  int i;

  name = res->rt_IdString;
  if (!name)
  {
    name = res->rt_Name;
    if (!name)
    {
      name = "<unknown>";
    }
  }
  if ((name[0] == '$') && (!strncmp(name + 1, "VER:" , 4)))
  {
    name += 5;
    while (*name == ' ') name++;
  }

  for (i = 0, p = buf; p < &buf[len - 1]; i++)
  {
    if (name[i] < ' ')
    {
      break;
    }
    *p++ = name[i];
  }
  *p = '\0';
}

void getnodename(const struct Node *node, unsigned char *buf, int len)
{
  const unsigned char *name;
  unsigned char *p;
  int i;

  name = node->ln_Name;
  if (!name)
  {
    name = "<unknown>";
  }

  for (i = 0, p = buf; p < &buf[len - 1]; i++)
  {
    if (name[i] < ' ')
    {
      break;
    }
    *p++ = name[i];
  }
  *p = '\0';
}

void getlibname(const struct Library *lib, unsigned char *buf, int len)
{
  const unsigned char *name;
  unsigned char *p;
  int i;

  name = lib->lib_IdString;
  if (!name)
  {
    name = lib->lib_Node.ln_Name;
    if (!name)
    {
      name = "<unknown>";
    }
  }
  if ((name[0] == '$') && (!strncmp(name + 1, "VER:" , 4)))
  {
    name += 5;
    while (*name == ' ') name++;
  }

  for (i = 0, p = buf; p < &buf[len - 1]; i++)
  {
    if (name[i] < ' ')
    {
      break;
    }
    *p++ = name[i];
  }
  *p = '\0';
}

void gettaskname(const struct Task *task, unsigned char *buf, int len)
{
  const unsigned char *name;
  int i;
  int prefix;

  name = task->tc_Node.ln_Name;
  if (!name)
  {
    name = "<unknown>";
  }

  if (len < 0)
  {
    len = -len;
    prefix = 0;
  }
  else
  {
    prefix = 1;
  }
  i = len - 1;

  if (task->tc_Node.ln_Type == NT_PROCESS)
  {
    struct CommandLineInterface *cli;

    cli = (struct CommandLineInterface *) BADDR(((struct Process *) task)->pr_CLI);
    if (cli && ((struct Process *) task)->pr_TaskNum)
    {
      unsigned char tmp[20];
      int x;

      if (prefix)
      {
        RawDoFmt("CLI(%lu):", &((struct Process *) task)->pr_TaskNum,
                 (void (* const )()) "\x16\xC0Nu", tmp);
        x = strlen(tmp);
        x = (i < x) ? i : x;
        strncpy(buf, tmp, x);
        buf += x;
        i -= x;
        len -= x;
      }

      name = BADDR(cli->cli_CommandName);
      if (name && name[0])
      {
        const unsigned char *last;

        i = *name++;
        last = name;
        for (x = 0; x < i; x++)
        {
          if ((name[x] == ':') || (name[x] == '/'))
          {
            last = &name[x + 1];
          }
        }
        i -= last - name;
        name = last;

        if (i > (len - 1))
        {
          i = len - 1;
        }
      }
      else
      {
        name = "no command loaded";
      }
    }
  }
  strncpy(buf, name, i);
  buf[i] = '\0';
}

void getcustomname(ULONG offs, unsigned char *buf, int len)
{
  const char * const regnames[] =
  {
    "BLTDDAT",     "DMACONR",     "VPOSR",       "VHPOSR",
    "DSKDATR",     "JOY0DAT",     "JOT1DAT",     "CLXDAT",
    "ADKCONR",     "POT0DAT",     "POT1DAT",     "POTGOR",
    "SERDATR",     "DSKBYTR",     "INTENAR",     "INTREQR",
    "DSKPTH",      "DSKPTL",      "DSKLEN",      "DSKDAT",
    "REFPTR",      "VPOSW",       "VHPOSW",      "COPCON",
    "SERDAT",      "SERPER",      "POTGO",       "JOYTEST",
    "STREQU",      "STRVBL",      "STRHOR",      "STRLONG",
    "BLTCON0",     "BLTCON1",     "BLTAFWM",     "BLTALWM",
    "BLTCPTH",     "BLTCPTL",     "BLTBPTH",     "BLTBPTL",
    "BLTAPTH",     "BLTAPTL",     "BLTDPTH",     "BLTDPTL",
    "BLTSIZE",     "BLTCON0L",    "BLTSIZV",     "BLTSIZH",
    "BLTCMOD",     "BLTBMOD",     "BLTAMOD",     "BLTDMOD",
    "RESERVED_068","RESERVED_06A","RESERVED_06C","RESERVED_06E",
    "BLTCDAT",     "BLTBDAT",     "BLTADAT",     "RESERVED_076",
    "SPRHDAT",     "BPLHDAT",     "LISAID",      "DSKSYNC",
    "COP1LCH",     "COP1LCL",     "COP2LCH",     "COP2LCL",
    "COPJMP1",     "COPJMP2",     "COPINS",      "DIWSTRT",
    "DIWSTOP",     "DDFSTRT",     "DDFSTOP",     "DMACON",
    "CLXCON",      "INTENA",      "INTREQ",      "ADKCON",
    "AUD0LCH",     "AUD0LCL",     "AUD0LEN",     "AUD0PER",
    "AUD0VOL",     "AUD0DAT",     "RESERVED_0AC","RESERVED_0AE",
    "AUD1LCH",     "AUD1LCL",     "AUD1LEN",     "AUD1PER",
    "AUD1VOL",     "AUD1DAT",     "RESERVED_0BC","RESERVED_0BE",
    "AUD2LCH",     "AUD2LCL",     "AUD2LEN",     "AUD2PER",
    "AUD2VOL",     "AUD2DAT",     "RESERVED_0CC","RESERVED_0CE",
    "AUD3LCH",     "AUD3LCL",     "AUD3LEN",     "AUD3PER",
    "AUD3VOL",     "AUD3DAT",     "RESERVED_0DC","RESERVED_0DE",
    "BPL1PTH",     "BPL1PTL",     "BPL2PTH",     "BPL2PTL",
    "BPL3PTH",     "BPL3PTL",     "BPL4PTH",     "BPL4PTL",
    "BPL5PTH",     "BPL5PTL",     "BPL6PTH",     "BPL6PTL",
    "BPL7PTH",     "BPL7PTL",     "BPL8PTH",     "BPL8PTL",
    "BPLCON0",     "BPLCON1",     "BPLCON2",     "BPLCON3",
    "BPL1MOD",     "BPL2MOD",     "BPLCON4",     "CLXCON2",
    "BPL1DAT",     "BPL2DAT",     "BPL3DAT",     "BPL4DAT",
    "BPL5DAT",     "BPL6DAT",     "BPL7DAT",     "BPL8DAT",
    "SPR0PTH",     "SPR0PTL",     "SPR1PTH",     "SPR1PTL",
    "SPR2PTH",     "SPR2PTL",     "SPR3PTH",     "SPR3PTL",
    "SPR4PTH",     "SPR4PTL",     "SPR5PTH",     "SPR5PTL",
    "SPR6PTH",     "SPR6PTL",     "SPR7PTH",     "SPR7PTL",
    "SPR0POS",     "SPR0CTL",     "SPR0DATA",    "SPR0DATB",
    "SPR1POS",     "SPR1CTL",     "SPR1DATA",    "SPR1DATB",
    "SPR2POS",     "SPR2CTL",     "SPR2DATA",    "SPR2DATB",
    "SPR3POS",     "SPR3CTL",     "SPR3DATA",    "SPR3DATB",
    "SPR4POS",     "SPR4CTL",     "SPR4DATA",    "SPR4DATB",
    "SPR5POS",     "SPR5CTL",     "SPR5DATA",    "SPR5DATB",
    "SPR6POS",     "SPR6CTL",     "SPR6DATA",    "SPR6DATB",
    "SPR7POS",     "SPR7CTL",     "SPR7DATA",    "SPR7DATB",
    "COLOR00",     "COLOR01",     "COLOR02",     "COLOR03",
    "COLOR04",     "COLOR05",     "COLOR06",     "COLOR07",
    "COLOR08",     "COLOR09",     "COLOR10",     "COLOR11",
    "COLOR12",     "COLOR13",     "COLOR14",     "COLOR15",
    "COLOR16",     "COLOR17",     "COLOR18",     "COLOR19",
    "COLOR20",     "COLOR21",     "COLOR22",     "COLOR23",
    "COLOR24",     "COLOR25",     "COLOR26",     "COLOR27",
    "COLOR28",     "COLOR29",     "COLOR30",     "COLOR31",
    "HTOTAL",      "HSSTOP",      "HBSTRT",      "HBSTOP",
    "VTOTAL",      "VSSTOP",      "VBSTRT",      "VBSTOP",
    "SPRHSTRT",    "SPRHSTOP",    "BPLHSTRT",    "BPLHSTOP",
    "HHPOSW",      "HHPOSR",      "BEAMCON0",    "HSSTRT",
    "VSSTRT",      "HCENTER",     "DIWHIGH",     "BPLHMOD",
    "SPRHPTH",     "SPRHPTL",     "BPLHPTH",     "BPLHPTL",
    "RESERVED_1F0","RESERVED_1F2","RESERVED_1F4","RESERVED_1F6",
    "RESERVED_1F8","RESERVED_1FA","FMODE",       "NO-OP"
  };

  strncpy(buf, regnames[offs >> 1], len - 1);
  buf[len - 1] = '\0';
}

void getciareg(ULONG offs, unsigned char *buf, int len)
{
  const char * const regnames[] =
  {
    "PRA",         "PRB",         "DDRA",        "DDRB",
    "TALO",        "TAHI",        "TBLO",        "TBHI",
    "TODLO",       "TODMID",      "TODHI",       "RESERVED",
    "SDR",         "ICR",         "CRA",         "CRB"
  };

  strncpy(buf, regnames[offs >>8], len - 1);
  buf[len - 1] = '\0';
}

int chktask(struct Task *task, ULONG addr)
{
  unsigned char buf[64];

  if ((addr >= (ULONG) task) && (addr < ((ULONG) task) + sizeof(struct Task)))
  {
    gettaskname(task, buf, sizeof(buf));
    PrintO("----> $%08lx - %s : Task %08lx, Offset %08lx\n", addr, buf, task, addr - (ULONG) task);

    return 1;
  }
  if ((addr >= (ULONG) task->tc_SPLower) && (addr < (ULONG) task->tc_SPUpper))
  {
    gettaskname(task, buf, sizeof(buf));
    PrintO("----> $%08lx - %s : Stack %08lx, Offset %08lx\n", addr, buf, task->tc_SPLower, addr - (ULONG) task->tc_SPLower);

    return 1;
  }

  return 0;
}

/* if address is identified, print it's properties to buffer
   *MUST BE CALLED WITHIN FORBID*
*/
void DumpSeg(ULONG addr)
{
  const unsigned char *name;
  unsigned char buf[64];
  ULONG segnum, offset, seglist;
  struct Node *node;

  if (segSem)
  {
    name = (*segSem->seg_Find)(addr, &segnum, &offset);
    if (name)
    {
      PrintO("----> $%08lx - %s : Hunk %lu, Offset %08lx", addr, name, segnum, offset);

      /* now find the SegList address - this is magic
      */
      (void) (*segSem->seg_Find)(addr, &seglist, &seglist);

      /* This "kludge" is for compatibility reasons.
         Check if result is the same as either the hunk
         or the offset. If so, do not print it...
      */
      if ((seglist != segnum) && (seglist != offset))
      {
        PrintO(", SegList %08lx\n", seglist);
      }
      else
      {
        PrintO("\n");
      }
      return;
    }
  }

  /* SegTracker not available, or failed to find segment.
     try locating the stuff from resident list:
  */
  do
  {
    struct Resident *r, **p;

    p = (struct Resident **) SysBase->ResModules;
    for (;;)
    {
      r = *p++;
      if (!r)
      {
        break;
      }
      if (((LONG) r) < 0)
      {
        p = (struct Resident **) (((ULONG) r) & 0x7fffffff);
        continue;
      }
      if ((addr >= (ULONG) r) && (addr < (ULONG) r->rt_EndSkip))
      {
        getresname(r, buf, sizeof(buf));
        
        PrintO("----> $%08lx - %s : RomTag %08lx, Offset %08lx\n", addr, buf, r, addr - (ULONG) r);

        return;
      }
    }
  } while (0);

  /* If still nothing found, check resource bases
  */
  node = (struct Node *) &SysBase->ResourceList.lh_Head;
  while ((node = node->ln_Succ)->ln_Succ)
  {
    if ((addr >= (ULONG) node) && (addr < (((ULONG) node) + sizeof(struct Node))))
    {
      getnodename(node, buf, sizeof(buf));
      PrintO("----> $%08lx - %s : Resource %08lx, Offset %08lx\n", addr, buf, node, addr - (ULONG) node);

      return;
    }
  }

  /* If still nothing found, check library bases
  */
  node = (struct Node *) &SysBase->LibList.lh_Head;
  while ((node = node->ln_Succ)->ln_Succ)
  {
    if ((addr >= (((ULONG) node) - ((struct Library *) node)->lib_NegSize)) && (addr < (((ULONG) node) + ((struct Library *) node)->lib_PosSize)))
    {
      getlibname((struct Library *) node, buf, sizeof(buf));
      PrintO("----> $%08lx - %s : Library %08lx, Offset %08lx\n", addr, buf, node, addr - (ULONG) node);

      return;
    }
  }

  /* If still nothing found, check device bases
  */
  node = (struct Node *) &SysBase->DeviceList.lh_Head;
  while ((node = node->ln_Succ)->ln_Succ)
  {
    if ((addr >= (((ULONG) node) - ((struct Library *) node)->lib_NegSize)) && (addr < (((ULONG) node) + ((struct Library *) node)->lib_PosSize)))
    {
      getlibname((struct Library *) node, buf, sizeof(buf));
      PrintO("----> $%08lx - %s : Device %08lx, Offset %08lx\n", addr, buf, node, addr - (ULONG) node);

      return;
    }
  }

  /* If still nothing found, check ports
  */
  node = (struct Node *) &SysBase->PortList.lh_Head;
  while ((node = node->ln_Succ)->ln_Succ)
  {
    if ((addr >= (ULONG) node) && (addr < ((ULONG) node) + sizeof(struct MsgPort)))
    {
      getnodename(node, buf, sizeof(buf));
      PrintO("----> $%08lx - %s : Port %08lx, Offset %08lx\n", addr, buf, node, addr - (ULONG) node);

      return;
    }
  }

  /* If still nothing found, check semaphores
  */
  node = (struct Node *) &SysBase->SemaphoreList.lh_Head;
  while ((node = node->ln_Succ)->ln_Succ)
  {
    if (addr >= (ULONG) node && addr < (((ULONG) node) + sizeof(struct SignalSemaphore)))
    {
      getnodename(node, buf, sizeof(buf));
      PrintO("----> $%08lx - %s : Semaphore %08lx, Offset %08lx\n", addr, buf, node, addr - (ULONG) node);

      return;
    }
  }

  /* If still nothing found, check tasks & task stacks
  */
  if (chktask(thistask, addr))
  {
    return;
  }
  node = (struct Node *) &SysBase->TaskReady.lh_Head;
  while ((node = node->ln_Succ)->ln_Succ)
  {
    if (chktask((struct Task *) node, addr))
    {
      return;
    }
  }
  node = (struct Node *) &SysBase->TaskWait.lh_Head;
  while ((node = node->ln_Succ)->ln_Succ)
  {
    if (chktask((struct Task *) node, addr))
    {
      return;
    }
  }

  /* If still nothing found, check interrupts
  */
  node = (struct Node *) &SysBase->IntrList.lh_Head;
  while ((node = node->ln_Succ)->ln_Succ)
  {
    if ((addr >= (ULONG) node) && (addr < ((ULONG) node) + sizeof(struct Interrupt)))
    {
      getnodename(node, buf, sizeof(buf));
      PrintO("----> $%08lx - %s : Interrupt %08lx, Offset %08lx\n", addr, buf, node, addr - (ULONG) node);

      return;
    }
  }

  /* could check execbase: KickTagPtr, KickMemPtr, ex_MemHandlers
  */

  /* could check dosbase: assigns, locks, mounted devs, resident cmds
  */

  /* could check expansionbase: configdevs
  */

  /* could check intuibase: screens & windows, boopsiclasses
  */

  /* could check gfxbase: fonts
  */

  /* could check inputbase: inputhandlers
  */

  /* could check cxbase: commodities
  */

  /* If still nothing found, check custom, cia-a & cia-b register addresses
  */
  if ((addr >= 0xdff000) && (addr < 0xdff200))
  {
    getcustomname(addr - 0xdff000, buf, sizeof(buf));
    PrintO("----> $%08lx - %s : Custom 00DFF000, Offset %08lx\n", addr, buf, addr - 0xdff000);

    return;
  }
  if ((addr >= 0xbfe001) && (addr < 0xbff000))
  {
    getciareg(addr - 0xbfe001, buf, sizeof(buf));
    PrintO("----> $%08lx - %s : CIA-A 00BFE001, Offset %08lx\n", addr, buf, addr - 0xbfe001);

    return;
  }
  if ((addr >= 0xbfd000) && (addr < 0xbfe000))
  {
    getciareg(addr - 0xbfd000, buf, sizeof(buf));
    PrintO("----> $%08lx - %s : CIA-B 00BFD000, Offset %08lx\n", addr, buf, addr - 0xbfd000);

    return;
  }
}

/* dump saved fpu registers
*/
void *dumpfpuregs(void *sp)
{
  ULONG *fs;
  ULONG fpiar;

  fs = (ULONG *) sp;
  fpiar = 0;
  switch (frametype)
  {
    case FRAMETYPE_NOFPU:
      break;

    case FRAMETYPE_FPU881:
    case FRAMETYPE_FPU882:
    case FRAMETYPE_FPU040:
      if (((UBYTE *) sp)[0])
      {
        fs = (ULONG *) (((UBYTE *) fs) + 2);
        fpiar = fs[2];
        PrintP("FPCR %08lx FPSR %08lx FPIAR %08lx\n"
               "FPx: %08lx%08lx%08lx %08lx%08lx%08lx %08lx%08lx%08lx %08lx%08lx%08lx\n"
               "FPx: %08lx%08lx%08lx %08lx%08lx%08lx %08lx%08lx%08lx %08lx%08lx%08lx\n",
               fs);
        fs += 3 + (3 * 8);

        /* is it mid frame?
        */
        if (((UBYTE *) sp)[0] == 0x90)
        {
          /* skip mid frame data
          */
          if (args.ARG_VERBOSE)
          {
            PrintO("%s mid frame data: %08lx %08lx %08lx",
                   fpufts[frametype], fs[0], fs[1], fs[2]);
          }
          fs += 3;
        }

        /* align
        */
        fs = (ULONG *) (((UBYTE *) fs) + 2);
      }
      break;

    case FRAMETYPE_FPU060:
      
      if (((UBYTE *) sp)[2])
      {
        fs += 1;
        fpiar = fs[2];
        PrintP("FPCR %08lx FPSR %08lx FPIAR %08lx\n"
               "FP03: %08lx%08lx%08lx %08lx%08lx%08lx %08lx%08lx%08lx %08lx%08lx%08lx\n"
               "FP47: %08lx%08lx%08lx %08lx%08lx%08lx %08lx%08lx%08lx %08lx%08lx%08lx\n",
               fs);
        fs += 3 + (3 * 8);
      }
      break;
  }


  /* simulate frestore
  */

  if (frametype && args.ARG_VERBOSE)
  {
    PrintO("%s frame type: ", fpufts[frametype]);
  }

  switch (frametype)
  {
    case FRAMETYPE_NOFPU:

      break;

    case FRAMETYPE_FPU881:

      /* 68881 frame types:
         0x0018 Null Frame, 4 bytes
         0x0118 Not Ready, Come Again Frame, 28? bytes
         0x0218 Illegal, Take Format Exception Frame, 28? bytes
         0xXX18 Idle Frame, 28 bytes
         0x00B4 Busy Frame, 184 bytes
      */
      /* is it valid 68881 FPU frame?
      */
      if (((UBYTE *) fs)[0])
      {
        /* ok, it's 68881 frame, determine type
        */
        switch (((UBYTE *) fs)[1])
        {
          case 0x18:
            if (args.ARG_VERBOSE) { PrintO("idle"); }
            fs += 7;  break;   /* idle */
          case 0xB4:
            if (args.ARG_VERBOSE) { PrintO("busy"); }
            fs += 46; break;   /* busy */
          default:
            PrintO("**INVALID FPU881 FRAME TYPE %02lx**", ((UBYTE *) fs)[1]);
            break;
        }
      }
      else
      {
        /* null frame, skip 4 bytes
        */
        if (args.ARG_VERBOSE) { PrintO("null"); }
        fs += 1;
      }
      break;

    case FRAMETYPE_FPU882:

      /* 68882 frame types:
         0x0038 Null Frame, 4 bytes
         0x0138 Not Ready, Come Again Frame, 60? bytes
         0x0238 Illegal, Take Format Exception Frame, 60? bytes
         0xXX38 Idle Frame, 60 bytes
         0x00D4 Busy Frame, 216 bytes
      */
      /* is it valid 68882 FPU frame?
      */
      if (((UBYTE *) fs)[0])
      {
        /* ok, it's 68882 frame, determine type
        */
        switch (((UBYTE *) fs)[1])
        {
          case 0x38:
            if (args.ARG_VERBOSE) { PrintO("idle"); }
            fs += 15; break;   /* idle */
          case 0xD4:
            if (args.ARG_VERBOSE) { PrintO("busy"); }
            fs += 54; break;   /* busy */
          default: PrintO("**INVALID FPU882 FRAME TYPE %02lx**", ((UBYTE *) fs)[1]); break;
        }
      }
      else
      {
        /* null frame, skip 4 bytes
        */
        if (args.ARG_VERBOSE) { PrintO("null"); }
        fs += 1;
      }
      break;
      
    case FRAMETYPE_FPU040:

      /* 68040 frame types:
         0x4160 Busy Frame, 100 bytes
         0x00xx Null Frame, 4 bytes
         0x4100 Idle Frame, 4 bytes
         0x4130 Unimplemented Floating Point Exception Frame, 52 bytes
      */
      /* is it valid 040 FPU frame?
      */
      if (((UBYTE *) fs)[0] == 0x41)
      {
        /* ok, it's 040 frame, determine type
        */
        switch (((UBYTE *) fs)[1])
        {
          case 0x60:
            if (args.ARG_VERBOSE) { PrintO("busy"); }
            fs += 25; break;
          case 0x00:
            if (args.ARG_VERBOSE) { PrintO("idle"); }
            fs += 1;  break;
          case 0x30:
            if (args.ARG_VERBOSE) { PrintO("except"); }
            fs += 13; break;
          default: PrintO("**INVALID FPU040 FRAME TYPE %02lx**", ((UBYTE *) fs)[1]); break;
        }
      }
      else if (((UBYTE *) fs)[0] == 0x00)
      {
        /* null frame, skip 4 bytes
        */
        if (args.ARG_VERBOSE) { PrintO("null"); }
        fs += 1;
      }
      else
      {
        PrintO("**INVALID FPU040 FRAME VERSION %02lx**", ((UBYTE *) fs)[0]); break;
      }
      break;

    case FRAMETYPE_FPU060:

      /* 68060 frame types:
         0x00 Null Frame, 12 bytes
         0x60 Idle Frame, 12 bytes
         0xE0 Exception Frame, 12 bytes
      */
      /* is it valid 060 FPU frame?
      */
      switch (((UBYTE *) fs)[2])
      {
        case 0x00:
          if (args.ARG_VERBOSE) { PrintO("null"); }
          fs += 3; break;
        case 0x60:
          if (args.ARG_VERBOSE) { PrintO("idle"); }
          fs += 3; break;
        case 0xE0:
          if (args.ARG_VERBOSE) { PrintO("except"); }
          fs += 3; break;
        default: PrintO("**INVALID FPU060 FRAME TYPE %02lx**", ((UBYTE *) fs)[2]); break;
      }
      break;
  }

  if (frametype && args.ARG_VERBOSE)
  {
    PrintO("\n");
  }

  if (fpiar && args.ARG_PCCHECK)
  {
    DumpSeg(fpiar);
  }

  return (void *) fs;
}

void *dumpcpuregs(void *sp)
{
  ULONG *fs;
  ULONG pc;
  int i;

  fs = sp;

  /* dump PC & SR
  */
  pc = fs[0];
  PrintP("PC:   %08lx SR: %04x\n", fs);
  fs = (ULONG *) (((UBYTE *) fs) + 6);

  /* dump d0-d7/a0-a6
  */
  PrintP("Data: %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n"
         "Addr: %08lx %08lx %08lx %08lx %08lx %08lx %08lx ",
         fs);
  /* dump a7
  */
  PrintO("%08lx\n", fs + 8 + 7);

  if (args.ARG_PCCHECK)
  {
    DumpSeg(pc);
  }

  if (args.ARG_DREGCHECK)
  {
    for (i = 0; i < 8; i++)
    {
      DumpSeg(fs[i]);
    }
  }

  if (args.ARG_AREGCHECK)
  {
    for (i = 0; i < 7; i++)
    {
      DumpSeg(fs[8 + i]);
    }
    DumpSeg((ULONG) (fs + 8 + 7));
  }
  fs += 8 + 7;

  return (void *) fs;
}

/* dump task stack to buffer
*/
void dumptask(struct Task *task)
{
  void *sp;
  int i, first;
  const char *type;
  char name[32];

  if ((!task) ||
      (task == thistask))
  {
    return;
  }

  sp = task->tc_SPReg;

  gettaskname(task, name, sizeof(name));

  switch (task->tc_Node.ln_Type)
  {
    case NT_TASK:
      type = "Task";
      break;

    case NT_PROCESS:
    {
      struct CommandLineInterface *cli;

      cli = (struct CommandLineInterface *) BADDR(((struct Process *) task)->pr_CLI);
      if (cli && ((struct Process *) task)->pr_TaskNum)
      {
        type = "CLI";
      }
      else
      {
        type = "Process";
      }
      break;
    }
    default:
      type = "Unknown";
      break;
  }

  PrintO("Task: %08lx Type: %s Name: \"%s\" tc_SPLower: %08lx tc_SPUpper: %08lx\n",
         task, type, name, task->tc_SPLower, task->tc_SPUpper);

  /* dump fpu registers
  */
  sp = dumpfpuregs(sp);

  /* dump cpu registers
  */
  sp = dumpcpuregs(sp);

  /* dump stack
  */
  if (args.ARG_DUMPSTACK)
  {
    int imax;

    if (args.ARG_VERBOSE)
    {
      PrintO("stack dump:\n");
    }

    imax = ((ULONG) task->tc_SPUpper - (ULONG) sp) >> 2;
    first = 1;
    for (i = 0; i < imax; i++)
    {
      if ((i & 7) == 0)
      {
        PrintO("%sStck: ", first ? "" : "\n");
        first = 0;
      }
      PrintO("%08lx ", ((ULONG *) sp) [i]);
    }
    /* if unaligned stack, dump last word
    */
    if (((ULONG) task->tc_SPUpper - (ULONG) sp) & 2)
    {
      if ((i & 7) == 0)
      {
        PrintO("%sStck: ", first ? "" : "\n");
      }
      PrintO("%04lx", ((UWORD *) task->tc_SPUpper)[-1]);
    }
    PrintO("\n");
  }

  if (args.ARG_STACKCHECK)
  {
    /* dump stack traceback
    */
    if ((!args.ARG_DUMPSTACK) || (args.ARG_VERBOSE))
    {
      PrintO("stack traceback:\n");
    }

    if ((sp < task->tc_SPLower) || (sp >= task->tc_SPUpper))
    {
      PrintO("error: sp out of stack bounds!\n");
      return;
    }

    for (/**/; sp < task->tc_SPUpper; sp = ((char *) sp) + 2)
    {
      DumpSeg(*((ULONG *) sp));
    }
  }
}


struct OutNode *getNode(void)
{
  struct OutNode *node;

  /* get tail of the list
  */
  node = (struct OutNode *) RemTail((struct List *) &outList);

  /* if the tail node is full, allocate new one
  */
  if (node && (node->ob_Index >= OUTBUFSIZE))
  {
    AddTail((struct List *) &outList, (struct Node *) node);
    node = NULL;
  }

  if (!node)
  {
    /* allocate new node
    */
    node = AllocMem(sizeof(struct OutNode), MEMF_ANY);
    if (!node)
    {
      return NULL;
    }
    node->ob_Index = 0;
  }
  AddTail((struct List *) &outList, (struct Node *) node);

  return node;
}

struct putchdata
{
  struct OutNode *pcd_curnode;
};

void __asm __saveds putchproc(register __d0 UBYTE Char,
                              register __a3 struct putchdata *putchdata)
{
  struct OutNode *node;

  node = putchdata->pcd_curnode;
  if (node)
  {
    node->ob_Buf[node->ob_Index] = Char;
    node->ob_Index++;
    if (node->ob_Index >= OUTBUFSIZE)
    {
      putchdata->pcd_curnode = getNode();
    }
  }
}

void PrintP(char *fmt, void *arg)
{
  struct putchdata putchdata;

  putchdata.pcd_curnode = getNode();

  RawDoFmt(fmt, arg, putchproc, &putchdata);
}

void PrintO(char *fmt, ...)
{
  PrintP(fmt, &fmt + 1);
}

/* output & free all pending buffers
*/
void DumpO(void)
{
  struct OutNode *node, *next;
  BPTR outfh;

  outfh = Output();

  node = (struct OutNode *) outList.mlh_Head;
  while ((next = (struct OutNode *) node->ob_Node.mln_Succ))
  {
    Remove((struct Node *) node);
    Write(outfh, node->ob_Buf, node->ob_Index);
    FreeMem(node, sizeof(struct OutNode));

    node = next;
  }

  Flush(outfh);
}
