#include <JEO:JEO.h>
#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/memory.h>
#include <exec/interrupts.h>
#include <exec/ports.h>
#include <exec/libraries.h>
#include <exec/io.h>
#include <exec/tasks.h>
#include <exec/execbase.h>
#include <exec/devices.h>
#include <devices/trackdisk.h>

#define TD_READ CMD_READ
#define BLOCKSIZE TD_SECTOR
#define NUMHEADS 2
#define NUMCYLS 80

SHORT error;
struct MsgPort *diskport;
struct IOExtTD *diskreq;
BYTE diskbuffer[BLOCKSIZE];
BYTE *diskdata;
SHORT testval;

extern struct MsgPort *CreatePort ();
extern struct IORequest *CreateExtIO ();

ULONG diskChangeCount;

MotorOff (VOID)
{
  diskreq->iotd_Req.io_Length = 0;
  diskreq->iotd_Req.io_Command = TD_MOTOR;
  DoIO ((struct IORequest *)diskreq);
  return (0);
}


MotorOn (VOID)
{
  diskreq->iotd_Req.io_Length = 1;
  diskreq->iotd_Req.io_Command = TD_MOTOR;
  DoIO ((struct IORequest *)diskreq);
  return (0);
}

ReadCylSec (WORD cyl, WORD sec, WORD hd)
{
  LONG offset;
  
  diskreq->iotd_Req.io_Length = BLOCKSIZE;
  diskreq->iotd_Req.io_Data = (APTR) diskbuffer;
  diskreq->iotd_Req.io_Command = ETD_READ;
  diskreq->iotd_Count = diskChangeCount;
  offset = TD_SECTOR * (sec + NUMSECS * hd + NUMSECS * NUMHEADS * cyl);
  diskreq->iotd_Req.io_Offset = offset;
  DoIO ((struct IORequest *)diskreq);
  return (0);
}

SeekFullRange (WORD howmany)
{
  int i;

  for (i = 0; i < howmany; i++)
  {
    diskreq->iotd_Req.io_Offset = 
      ((NUMCYLS - 1) * NUMSECS * NUMHEADS - 1) * 512;
    diskreq->iotd_Req.io_Command = TD_SEEK;
    DoIO ((struct IORequest *)diskreq);
    if (diskreq->iotd_Req.io_Error != 0)
      printf ("\nSeek Cycle NUmber %ld, Error = %ld",
        i, diskreq->iotd_Req.io_Error);
    diskreq->iotd_Req.io_Offset = 0;
    diskreq->iotd_Req.io_Command = TD_SEEK;
    DoIO ((struct IORequest *)diskreq);
    if (diskreq->iotd_Req.io_Error != 0)
      printf ("\nSeek Cycle NUmber %ld, Error = %ld",
        i, diskreq->iotd_Req.io_Error);     
    printf ("\nCompleted a seek");
  }
  return (0);
}

main ()
{
  SHORT cylinder, head, sector;
  
  diskdata = &diskbuffer[0];

  diskport = CreatePort (0, 0);
  if (diskport == NULL)
    exit (FALSE);
  diskreq = (struct IOExtTD *) CreateExtIO (diskport, sizeof (struct IOExtTD));
  if (diskreq == NULL)
  {
    DeletePort (diskport);
    exit (FALSE);
  }
  
  error = OpenDevice (TD_NAME, 0, (struct IORequest *)diskreq, 0);
	if (!error)
	{
	  diskreq->iotd_Req.io_Command = TD_CHANGENUM;
  	DoIO ((struct IORequest *)diskreq);
	  diskChangeCount = diskreq->iotd_Req.io_Actual;
	  MotorOn ();
	  SeekFullRange (1);
	  for (cylinder = 0; cylinder < NUMCYLS - 75; cylinder++)
	  {
	    printf ("\nReading Cylinder = %ld", cylinder);
	    for (head = 0; head < NUMHEADS; head++)
	    {
	      for (sector = 0; sector < NUMSECS; sector++)
	      {
	        ReadCylSec (cylinder, sector, head);
	        if (diskreq->iotd_Req.io_Error != 0)
	        {
	          printf ("\nError At Cyl = %ld, Sc = %ld, Hd = %ld, Error = %ld",
	            cylinder, sector, head, diskreq->iotd_Req.io_Error);               
	          goto end;
	        }
	      }
	    }
	  }
end:
	  MotorOff ();
	  CloseDevice ((struct IORequest *)diskreq);
	  DeleteExtIO ((struct IORequest *)diskreq);
	  DeletePort (diskport);
	  printf ("\n");
	}
}
