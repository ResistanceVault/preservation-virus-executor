#include <JEO:JEO.h>
#include <ctype.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <devices/trackdisk.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include "VE_Prototypes.h"
#include "VE_Window.h"
#include <proto/xfdmaster.h>
#include <proto/xvs.h>

#define TD_READ CMD_READ
#define BLOCKSIZE TD_SECTOR
#define NUMHEADS 2
#define NUMCYLS 80

SHORT error;
BYTE *SectorBuffer = 0;
SHORT testval;

ULONG diskChangeCount;

ULONG numsecs;

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

VOID ReadCylSec (WORD cyl, WORD sec, WORD hd)
{
  LONG offset;
  
  diskreq->iotd_Req.io_Length = BLOCKSIZE;	// Alltid 512
  diskreq->iotd_Req.io_Data = (APTR)SectorBuffer;
  diskreq->iotd_Req.io_Command = ETD_READ;
  diskreq->iotd_Count = diskChangeCount;
  offset = TD_SECTOR * (sec + numsecs * hd + numsecs * NUMHEADS * cyl);
  diskreq->iotd_Req.io_Offset = offset;
  DoIO ((struct IORequest *)diskreq);
}

VOID WriteCylSec (WORD cyl, WORD sec, WORD hd)
{
  LONG offset;
  
  diskreq->iotd_Req.io_Length = BLOCKSIZE;	// Alltid 512
  diskreq->iotd_Req.io_Data = (APTR)SectorBuffer;
  diskreq->iotd_Req.io_Command = ETD_WRITE;
  diskreq->iotd_Count = diskChangeCount;
  offset = TD_SECTOR * (sec + numsecs * hd + numsecs * NUMHEADS * cyl);
  diskreq->iotd_Req.io_Offset = offset;
  DoIO ((struct IORequest *)diskreq);
}

/*
SeekFullRange (WORD howmany)
{
  int i;

  for (i = 0; i < howmany; i++)
  {
    diskreq->iotd_Req.io_Offset = 
      ((NUMCYLS - 1) * numsecs * NUMHEADS - 1) * 512;
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
  }
  return (0);
}
*/

VOID CheckSector (ULONG startcyl, ULONG endcyl)
{
  SHORT cylinder, head, sector;
  UWORD infected = 0, unit, i;
  ULONG max_sectors;
	BPTR lock = 0;
	struct InfoData *id;
	BOOL flag = FALSE;
	char Units[60] = "";
  WORD uFlag[4];

  for (unit = 0, i = 0; unit < 4; unit++)
		uFlag[unit] = -1;

  for (unit = 0, i = 0; unit < 4; unit++)
  {
    if (viewCol[unit] != BLUE)	// Not empty
    {
      if (drive[unit] != ERROR)
      {
        strcat (Units, Drive[unit]);
        strcat (Units, "|");
        uFlag[i] = unit;
        i++;
      }
    }
  }   
  if (i > 1)	/* More than one disk in the system */
  {
    strcat (Units, GLS (&L_WCANCEL));
    SPrintF (Dummy, GLS (&L_SELECT_DISK_SECTOR));
    if (flag = rtEZRequestTags (Dummy, Units, NULL, NULL, RT_ReqPos, REQPOS_CENTERSCR,
      RTEZ_ReqTitle, GLS (&L_SECTOR_CHECK), RT_TextAttr, &topaz8, TAG_END))
    {
      switch (flag)
      {
        case 1: unit = uFlag[0]; break;
        case 2: unit = uFlag[1]; break;
        case 3: unit = uFlag[2]; break;
        case 4: unit = uFlag[3]; break;
      }
    }
  }
  else if (i == 1)	// Just one disk
    unit = uFlag[0];
  else	// Ingen disk :(
 	{
		JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NO_DISK_ANY_DRIVE), GLS (&L_OK), S_MESSAGE);
 	  return;
  }

	if (id = AllocMem (sizeof (struct InfoData), MEMF_CLEAR))
	{
		lock = Lock (Drive[unit], -2);
		if (lock)
		{
			Info (lock, id);
		  UnLock (lock);
		}

		if (id->id_DiskType != ID_NO_DISK_PRESENT)
		{
			switch (id->id_DiskState)
			{
				case ID_VALIDATED:
				{
					flag = TRUE;	// JEPP!
					if (id->id_NumBlocks > 1760)
						numsecs = 22;	// HD
					else
						numsecs = 11; // DD
					break;
				}
				case ID_WRITE_PROTECTED:
				{
					SPrintF (Dummy, GLS (&L_DISK_IN_DRIVE_PROTECTED), Drive[unit]);
					JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
					break;
				}
				case ID_VALIDATING:
				{
					SPrintF (Dummy, GLS (&L_DISK_NO_VALIDATED), Drive[unit]);
					JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_ERROR);
					break;
				}
			}
		}
		else
		{
	    PlaySound (S_ERROR);
  	  Status (GLS (&L_NO_DISK_ANY_DRIVE));
		}
		FreeMem (id, sizeof (struct InfoData));
	}

	if (!flag)	// Error setting up
		return;
	
	max_sectors = numsecs * NUMHEADS * NUMCYLS;

	SectorBuffer = AllocMem (BLOCKSIZE, MEMF_CHIP|MEMF_CLEAR);
  error = OpenDevice (TD_NAME, unit, (struct IORequest *)diskreq, 0);
	if (!error)
	{
		ULONG sec_count = 0;
		BOOL go_flag = TRUE;

		DiskStatus (GLS (&L_PLEASE_WAIT_SECTOR_CHECK), unit);
	  diskreq->iotd_Req.io_Command = TD_CHANGENUM;
  	DoIO ((struct IORequest *)diskreq);
	  diskChangeCount = diskreq->iotd_Req.io_Actual;
	  MotorOn ();
//	  SeekFullRange (1);
	  for (cylinder = 0; cylinder < 80; cylinder++)
	  {
	    SPrintF (Dummy, GLS (&L_CHECKING_CYLINDER), cylinder, 79 - cylinder);
	    Status (Dummy);
	    JEOBar (VirusWnd, mainX, 35, mainX + 628, 37, cylinder + 1, endcyl + 1, BLUE);
	    for (head = 0; head < NUMHEADS; head++)
	    {
				if (CheckESC (VirusWnd))
				{
					if (!JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_SECTOR_CHECKING_PAUSED), GLS (&L_CONTINUE_CANCEL), S_NO_SOUND))
					{
						go_flag = FALSE;
						break;
					}
					else
						All (OFF);
				}
	      for (sector = 0; sector < numsecs; sector++)
	      {
	        ReadCylSec (cylinder, sector, head);
	        if (diskreq->iotd_Req.io_Error == 0)	// OK!
	        {
						sectorinfo->xvssi_Sector = SectorBuffer;
						sectorinfo->xvssi_Key = sec_count;	// Litt usikker her...
						xvsCheckSector (sectorinfo);
						switch (sectorinfo->xvssi_SectorType)
						{
							case XVSST_UNKNOWN: break;
							case XVSST_DESTROYED:
							{
								SPrintF (Dummy, GLS (&L_SECTOR_DESTROYED), sec_count, sectorinfo->xvssi_Name);
								JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_VIRUS);
								All (OFF);
								break;
							}
							case XVSST_INFECTED:
							{
								infected++;
								SPrintF (Dummy, GLS (&L_BLOCK_INFECTED), sec_count, sectorinfo->xvssi_Name);
								if (JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_REPAIR_SECTOR_CONTINUE), S_VIRUS));
								{
									if (xvsRepairSector (sectorinfo))
										WriteCylSec (cylinder, sector, head);
									else
										JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR_REPAIRING), GLS (&L_OK), S_ERROR);
								}
								All (OFF);
								break;
							}
						}
				    sec_count++;
	        }
	        else
	        {
						tderror (diskreq->iotd_Req.io_Error, unit);
						JEOSoundRequest (GLS (&L_MESSAGE), TDErrorMsg, GLS (&L_OK), S_ERROR);
						go_flag = FALSE;
						break;
	        }
	      }
	    }
	    if (!go_flag)
	    	break;
	  }
	  MotorOff ();
	  CloseDevice ((struct IORequest *)diskreq);
		SPrintF (Dummy, GLS (&L_SECTORS_CHECKED), sec_count, infected);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_NO_SOUND);
		DiskStatus (GLS (&L_SECTOR_CHECK_END), unit);

		ClearBar ();
	}
	FreeMem (SectorBuffer, BLOCKSIZE);
}
