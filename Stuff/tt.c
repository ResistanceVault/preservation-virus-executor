#include <proto/dos.h>
#include <proto/exec.h>
#include <JEO:JEO.h>
#include <ctype.h>

#include <pragmas/mmu_pragmas.h>

#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <devices/trackdisk.h>
#include <devices/timer.h>
#include <proto/locale.h>
#include <libraries/locale.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <-V:VE_Prototypes.h>
#include <-V:VE_PatchOffsets.h>
#include <JEO:raw.h>
#include <-V:Status.h>
#include <-V:VE_Window.h>
#include "-V:List.h"
#include <proto/xfdmaster.h>
//#include <proto/xadmaster.h>
#include <proto/xvs.h>
#define LOCALE_TEXT
#include <-V:VE_locale.h>

#include "updater.h"
#define VERSTRING "1.2.4"

extern struct SignalSemaphore upd_child_sema;
extern int upd_going;

int main(void)
{
   struct File test;

   test.host = "hem.passagen.se";
/*
   If author have no homepage aminet can be used
   test.host = "aminet:";
   test.path = "/comm/tcp/TView.lha";
*/
   test.path = "/mcdrill/myprogs/TView.lha";
   test.name = "TView.lha";
   test.program_name = "TView";

   // Call the Updater with debug info
   if(!upd_going) // useless in this example, but essential in gui program.
      upd_Updater(&test, VERSTRING, 1);

   /*
   .
   .
   .
   Your program */
   Delay(200); // Updater must obtain the semaphore first.
   /*
   .
   .
   .
   Your programs exit routines
   */

   // Wait for updater child to exit
   ObtainSemaphore(&upd_child_sema);

   return 0;
}
