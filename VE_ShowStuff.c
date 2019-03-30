#include <JEO:JEO.h>
#include <ctype.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include "VE_Prototypes.h"
#include "VE_PatchOffsets.h"
#include "VE_Window.h"
#include "ShowWindow.h"
#include <JEO:raw.h>

GLOBAL struct ExecBase *SysBase;
UWORD showCount;

#define SHOW_ONE 0
#define SHOW_ALL 1

typedef struct
{
  struct List list;
} Sjef;

Sjef show;

typedef struct
{
  struct Node nd;
  char Name[100];
} Show;

/*
BOOL CheckShowNameExist (char *Name)
{
  struct Node *nd;
  Show *bp;

  nd = show.list.lh_Head;
  bp = (Show *)nd;
  while (nd->ln_Succ)
  {
    if (!(strcmp (bp->Name, Name)))
      return (TRUE);

    nd = nd->ln_Succ;
    bp = (Show *)nd;
  }
  return (FALSE);
}
*/

struct Node *ShowFindTheOne (char *Name)
{
  struct Node *nd;

  nd = show.list.lh_Head;
  while (nd->ln_Succ)
  {
    if (stricmp (nd->ln_Name, Name) > 0)
    {
      nd = nd->ln_Pred;
      return (nd);
    }
    nd = nd->ln_Succ;
  }
  return (FALSE);
}

BOOL InsertShow (char *Name)
{
  struct Node *nd;
  Show *bp = 0;

  if (!(bp = (Show *)AllocMem (sizeof (Show), MEMF_PUBLIC | MEMF_CLEAR)))
    return (FALSE);

  strcpy (bp->Name, Name);
  bp->nd.ln_Name=bp->Name;

  if (!(nd = ShowFindTheOne (Name)))
		AddTail(&show.list, &bp->nd);
  else
    Insert (&show.list, &bp->nd, nd);

  return (TRUE);
}

VOID KillShow (VOID)
{
  ULONG s;
  struct Node *nd;

  nd = show.list.lh_Head;
  for (s = 0; s < showCount; s++)
  {
    Remove (nd);
    FreeMem ((UBYTE *)nd, sizeof (Show));
    nd = 0;
    nd = show.list.lh_Head;
  }
  showCount = 0;
}

VOID CloseShow (VOID)
{
  Show *bp;

  KillShow ();
  bp = (Show *)show.list.lh_Head;
  while (bp = (Show *)RemHead (&show.list))
  {
    FreeMem (bp, sizeof (Show));
    bp = 0;
  }
}

BOOL HandleShowIDCMP (VOID)
{
	struct IntuiMessage	*m;
	UWORD	code;
 	ULONG class;
	struct IntuiMessage	tmpmsg;
  BOOL ret = TRUE;

	while (m = GT_GetIMsg (ShowWnd->UserPort))
	{
		CopyMem ((char *)m, (char *)&tmpmsg, (long)sizeof(struct IntuiMessage));
		GT_ReplyIMsg (m);
		code = tmpmsg.Code;
		class = tmpmsg.Class;
		switch (tmpmsg.Class)
		{
			case IDCMP_DISKINSERTED:
				DiskInserted ();
				All (OFF);
				Status (GLS (&L_PLEASE_WAIT));
				break;
			case IDCMP_DISKREMOVED:
				DiskRemoved ();
				Status (GLS (&L_PLEASE_WAIT));
				break;
			case	IDCMP_RAWKEY:
			{
				switch (code)
				{
					case ESC:
						ret = FALSE;
						break;
				}
				break;
			}
			case	IDCMP_CLOSEWINDOW:
			{
				ret = FALSE;
				break;
			}
			case	IDCMP_GADGETUP:
			{
			  struct Gadget *gadget;
				UWORD nr;

			  gadget = (struct Gadget *)tmpmsg.IAddress;
				nr = gadget->GadgetID;

			  switch (nr)
			  {
					case GD_EXIT:
					ret = FALSE;
					break;
				}
				break;
			}
		}
	}
	return (ret);
}


/*
	struct	List MemList;
	struct	List ResourceList;
	struct	List DeviceList;
	struct	List IntrList;
	struct	List LibList;
	struct	List PortList;
	struct	List TaskReady;
	struct	List TaskWait;
*/

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
        name = "";
      }
    }
  }
  strncpy(buf, name, i);
  buf[i] = '\0';
}

UWORD GetWidth (UWORD width, UWORD new)
{
	if (new > width)
		return (new);
	else
		return (width);
}

VOID ShowStuff (UWORD mode)
{
  struct Node *node;
	ULONG	waitsigs, gotsigs;
	BOOL quit = 1;
	char Hold[110];
	struct Task *task;
	UWORD width = 640;
	char Hold1[50];

  NewList (&show.list);
  showCount = 0;

	All (OFF);

	switch (mode)
	{
		case XVSLIST_BOOTVIRUSES:
		{
			mystrcpy (Hold1, GLS (&L_BOOTBLOCK_C));
			if (viruslist = xvsCreateVirusList (mode))
				SPrintF (Hold, GLS (&L_VIRUS_LIST), Hold1, viruslist->xvsvl_Count); 
			width = 30;
			Status (Hold);
			break;
		}
		case XVSLIST_LINKVIRUSES:
		{
			mystrcpy (Hold1, GLS (&L_LINK_C));
			if (viruslist = xvsCreateVirusList (mode))
				SPrintF (Hold, GLS (&L_VIRUS_LIST), Hold1, viruslist->xvsvl_Count); 
			width = 30;
			Status (Hold);
			break;
		}
		case XVSLIST_FILEVIRUSES:
		{
			mystrcpy (Hold1, GLS (&L_FILE_C));
			if (viruslist = xvsCreateVirusList (mode))
				SPrintF (Hold, GLS (&L_VIRUS_LIST), Hold1, viruslist->xvsvl_Count); 
			width = 30;
			Status (Hold);
			break;
		}
		case XVSLIST_DATAVIRUSES:
		{
			mystrcpy (Hold1, GLS (&L_DATA_C));
			if (viruslist = xvsCreateVirusList (mode))
				SPrintF (Hold, GLS (&L_VIRUS_LIST), Hold1, viruslist->xvsvl_Count); 
			width = 30;
			Status (Hold);
			break;
		}
// *************************************************************************
		case TASKS:
		{
			UWORD val;
			LONG maxcli;
			char Buf[64];
			ULONG check[500];
			BOOL go_flag;

			Status (GLS (&L_SHOWING_TASKS));

		  Forbid();
			maxcli = MaxCli();
			for (val = 1; val < maxcli; val++)
			{
				node = (struct Node *) FindCliProc (val);
				if (node)
				{
					check[val] = (ULONG)node;
		      gettaskname ((struct Task *) node, Buf, -sizeof (Buf));

					if (*node->ln_Name)
					{
						char Hold[200];

						SPrintF (Hold, "%s [%s]", node->ln_Name, Buf);
						SPrintF (Dummy, "%-51s ", Hold);
					}
					else
						SPrintF (Dummy, "%-51s ", GLS (&L_WARNING_NO_NAME));

					SPrintF (Hold, " $%08lx ", node);
					strcat (Dummy, Hold);

					SPrintF (Hold, " %3ld ", node->ln_Pri);
					strcat (Dummy, Hold);
				
					if (node->ln_Type == 13)
						SPrintF (Hold, GLS (&L_PROCESS));
					else if (node->ln_Type == 1)
						SPrintF (Hold, GLS (&L_TASK));
					strcat (Dummy, Hold);

					InsertShow (Dummy);
					showCount++;
				}
			}
			Permit ();

			if (task = FindTask (0))	// Min egen killer :)
			{
				go_flag = TRUE;
				for (val = 1; val < maxcli; val++)
				{
					if ((ULONG)task == check[val])
					{
						go_flag = FALSE;
						break;
					}
				}
				if (go_flag)
				{
					if (*task->tc_Node.ln_Name)
						SPrintF (Dummy, "%-51s ", task->tc_Node.ln_Name);

					SPrintF (Hold, " $%08lx ", task);
					strcat (Dummy, Hold);

					SPrintF (Hold, " %3ld ", task->tc_Node.ln_Pri);
					strcat (Dummy, Hold);
				
					if (task->tc_Node.ln_Type == 13)
						SPrintF (Hold, GLS (&L_PROCESS));
					else if (task->tc_Node.ln_Type == 1)
						SPrintF (Hold, GLS (&L_TASK));
					strcat (Dummy, Hold);

					InsertShow (Dummy);
					showCount++;
				}
			}

			Forbid ();
			node = SysBase->TaskWait.lh_Head;
			while (node->ln_Succ)
			{
				go_flag = TRUE;
				for (val = 1; val < maxcli; val++)
				{
					if ((ULONG)node == check[val])
					{
						go_flag = FALSE;
						break;
					}
				}
				if (go_flag)
				{
					if (*node->ln_Name)
						SPrintF (Dummy, "%-51s ", node->ln_Name);
					else
						SPrintF (Dummy, "%-51s ", GLS (&L_WARNING_NO_NAME));

					SPrintF (Hold, " $%08lx ", node);
					strcat (Dummy, Hold);

					SPrintF (Hold, " %3ld ", node->ln_Pri);
					strcat (Dummy, Hold);
				
					if (node->ln_Type == 13)
						SPrintF (Hold, GLS (&L_PROCESS));
					else if (node->ln_Type == 1)
						SPrintF (Hold, GLS (&L_TASK));
					strcat (Dummy, Hold);

					InsertShow (Dummy);
					showCount++;
				}
				node = node->ln_Succ;
			}
			Permit ();
			Forbid ();
			node = SysBase->TaskReady.lh_Head;
			while (node->ln_Succ)
			{
				showCount++;
				if (*node->ln_Name)
					SPrintF (Dummy, "%-51s ", node->ln_Name);
				else
					SPrintF (Dummy, "%-51s ", GLS (&L_WARNING_NO_NAME));

				SPrintF (Hold, " $%08lx ", node);
				strcat (Dummy, Hold);

				SPrintF (Hold, " %3ld ", node->ln_Pri);
				strcat (Dummy, Hold);
				
				if (node->ln_Type == 13)
					SPrintF (Hold, GLS (&L_PROCESS));
				else if (node->ln_Type == 1)
					SPrintF (Hold, GLS (&L_TASK));
				strcat (Dummy, Hold);

				InsertShow (Dummy);

				node = node->ln_Succ;
			}
			Permit ();

			SPrintF (Dummy, GLS (&L_SHOWING_TASKS_1), showCount); 
			Status (Dummy);
			mystrcpy (Hold, GLS (&L_NAME_ADDRESS_PRI_TYPE));
			break;
		}
		case LIBS:
		{
			Status (GLS (&L_SHOWING_LIBRARIES));

			Forbid ();
			node = SysBase->LibList.lh_Head;
			while (node->ln_Succ)
			{
				showCount++;
				SPrintF (Dummy, "%-50s ", node->ln_Name);

				SPrintF (Hold, " $%08lx", node);
				strcat (Dummy, Hold);

				SPrintF (Hold, " %4ld ", node->ln_Pri);
				strcat (Dummy, Hold);

				FindLibraryVersion (Hold, node);
				strcat (Dummy, Hold);

				InsertShow (Dummy);
				node = node->ln_Succ;
			}
			Permit ();
			SPrintF (Dummy, GLS (&L_SHOWING_LIBRARIES_1), showCount); 
			Status (Dummy);
			mystrcpy (Hold, GLS (&L_NAME_ADDRESS_PRI_VER));
			break;
		}
		case DEVICES:
		{
			Status (GLS (&L_SHOWING_DEVICES));

			Forbid ();
			node = SysBase->DeviceList.lh_Head;
			while (node->ln_Succ)
			{
				showCount++;
				SPrintF (Dummy, "%-50s ", node->ln_Name);

				SPrintF (Hold, " $%08lx", node);
				strcat (Dummy, Hold);

				SPrintF (Hold, " %4ld ", node->ln_Pri);
				strcat (Dummy, Hold);

				FindLibraryVersion (Hold, node);
				strcat (Dummy, Hold);

				InsertShow (Dummy);

				node = node->ln_Succ;
			}
			Permit ();
			SPrintF (Dummy, GLS (&L_SHOWING_DEVICES_1), showCount); 
			Status (Dummy);
			mystrcpy (Hold, GLS (&L_NAME_ADDRESS_PRI_VER));
			break;
		}
		case PATCHES:
		{
			UWORD i;

			Status (GLS (&L_SHOWING_PATCHES));
			width = 0;
			for (i = 1; i < ve_max_patches; i++)	// Vi skipper unknown
			{
				showCount++;
				strcpy (Dummy, PatchNames[i]);
				InsertShow (Dummy);

				width = GetWidth (width, strlen (Dummy));
			}
			SPrintF (Dummy, GLS (&L_SHOWING_PATCHES_1), showCount); 
			Status (Dummy);
			mystrcpy (Hold, GLS (&L_NAME));
			break;
		}
	}

	if (!OpenShowWindow (showCount, width))
	{
		SetWindowTitles (ShowWnd, Hold, Ver);
		if (mode > 10)
			GT_SetGadgetAttrs (ShowGadgets[GD_SHOWLIST], ShowWnd, NULL , GTLV_Labels, &viruslist->xvsvl_List, TAG_DONE, NULL);
		else
			GT_SetGadgetAttrs (ShowGadgets[GD_SHOWLIST], ShowWnd, NULL , GTLV_Labels, &show.list, TAG_DONE, NULL);

		waitsigs = (1L << ShowWnd->UserPort->mp_SigBit);
		while (quit)
		{
			gotsigs = Wait (waitsigs);
			quit = HandleShowIDCMP ();
		}
		CloseShowWindow ();
	}
	CloseShow ();
	All (ON);
	Status (GLS (&L_READY));
}
