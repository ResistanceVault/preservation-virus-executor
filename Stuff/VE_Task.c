;/*
Delete VE_Task.o quiet
sc5 -j73 -v -y VE_Task
quit
*/

#include <JEO:JEO.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <exec/tasks.h>
#include <-V:VE_Prototypes.h>
#include <-V:VE_Window.h>

#define STACK_SIZE 2048

APTR stack = NULL;
struct Task *tc = NULL;
BYTE *TaskName = "VirusExecutor Timetask";

extern struct RastPort *rp;

VOID TimeTask (VOID)
{
  char Buf[25];
  int i = 0;

  FOREVER
  {
	  Delay (30);

		i++;
		sprintf (Buf, "%ld", i);
		Forbid ();
		JEOWrite (rp, mainX + 15, 300, Buf, WHITE);
		Permit ();
	}
  Wait (NULL);
}

VOID StartTask (VOID)
{
  stack = AllocMem (STACK_SIZE, MEMF_CLEAR);
  tc = (struct Task *)AllocMem (sizeof (struct Task), MEMF_CLEAR | MEMF_PUBLIC);

  tc->tc_Node.ln_Type = NT_TASK;
  tc->tc_Node.ln_Name = TaskName;
  tc->tc_SPLower = (APTR)stack;
  tc->tc_SPUpper = (APTR)(STACK_SIZE + (ULONG)stack);
  tc->tc_SPReg = tc->tc_SPUpper;

  AddTask (tc, TimeTask, NULL);
}

VOID EndTask (VOID)
{
  RemTask (tc);
  if (tc)
    FreeMem (tc, sizeof (struct Task));
  if (stack)
    FreeMem (stack, STACK_SIZE);
}
