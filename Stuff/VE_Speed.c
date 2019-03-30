;/*
sc5 -j73 -v -ff -q5e -y VE_Speed
quit
*/

#include <JEO:JEO.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <devices/timer.h>
#include <time.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <libraries/reqtools.h>
#include <-V:VE_prototypes.h>

VOID Freq (VOID);

LONG speed_mics = 0, speed_secs = 0;
extern struct ExecBase *SysBase;

BOOL GetTime (VOID)
{
  struct timerequest tr;
  struct MsgPort *tport = 0;

  speed_mics = speed_secs = 0;
  if ((OpenDevice (TIMERNAME, UNIT_MICROHZ, (struct IORequest *)&tr, 0)))
    return (FALSE);
  if (!(tport = (struct MsgPort *)CreatePort (0, 0)))
  {
    CloseDevice ((struct IORequest *)&tr);
    return (FALSE);
  }
  tr.tr_node.io_Message.mn_Node.ln_Type	= NT_MESSAGE;
  tr.tr_node.io_Message.mn_Node.ln_Pri	= 20;
  tr.tr_node.io_Message.mn_Node.ln_Name	= NULL;
  tr.tr_node.io_Message.mn_ReplyPort	= tport;
  tr.tr_node.io_Command			= TR_GETSYSTIME;
  DoIO ((struct IORequest *)&tr);
  speed_mics = tr.tr_time.tv_micro;
  speed_secs = tr.tr_time.tv_secs;

  DeletePort (tport);
  CloseDevice ((struct IORequest *)&tr);

	return (TRUE);
}

DOUBLE CalcMhz (ULONG max)
{
	ULONG tab = 0;
	ULONG high, low;
	LONG sec = 0, i;
	LONG mic = 0;
	ULONG time;
	UWORD processorFlag;
	DOUBLE mhz;

	All (OFF);

  if (!GetTime ())
  	return 11.11;

	Status (GLS (&L_CALCULATING_CPU_SPEED));
	if (max <= 1)
		max = 1;

  SysBase = (*((struct ExecBase **)4));
	processorFlag = SysBase->AttnFlags;
	high = 0;
	low = 0xFFFFFFFF;
	for (i = 0; i < max; i++)
	{
	  Forbid ();
  	GetTime ();
	  sec = speed_secs;
  	mic = speed_mics;
		Freq ();
	  GetTime ();
	  Permit ();

	  sec = speed_secs - sec;
  	mic = speed_mics - mic;

		if (mic < 0)
		{
			mic += 1000000;
			sec--;
		}

	  sprintf (Dummy, "%ld%06ld", sec, mic);
  	time = atoi (Dummy);
		tab += time;

		if (time > high)
			high = time;
		if (time < low)
			low = time;
		if (max > 1)
		{
			sprintf (Dummy, "Calculating CPU speed values (%ld of %ld)...", i + 1, max);
			Status (Dummy);
	    JEOBar (VirusWnd, mainX, 35, mainX + 640, 37, i + 1, max, BLUE);
	  }
	}

	if (max > 1)
	{
		sprintf (Dummy, "\n Low = %ld\nHigh = %ld\n\nPlease send the low and high value to me:\nEmail: virusexecutor@c2i.net\n\nAlso write what kind of processor you have!", low, high);
		JEOSoundRequest (GLS (&L_MESSAGE), Dummy, "Thanks", S_MESSAGE);
	  SetAPen (VirusWnd->RPort, 0);
  	RectFill (VirusWnd->RPort, mainX, 35, mainX + 640, 37);
	}

	Delay (5);
	if (mic < 0)
	{
		mic += 1000000;
		sec--;
	}

	mhz = 0;
	if (processorFlag & AFF_68060)
	{
		if (time > 220000 AND time < 250000)
			mhz = 66;
		if (time > 250000 AND time < 270000)
			mhz = 60;
		if (time > 290000 AND time < 330000)
			mhz = 50;
	}
	else if (processorFlag & AFF_68040)
	{
		if (time > 260000 AND time < 312000)	// 060 mode befor SetPatch
			mhz = 50;
		else if (time > 330000 AND time < 360000)
			mhz = 40;
		else if (time > 400000 AND time < 470000)
			mhz = 33;
		else if (time > 470000 AND time < 550000)
			mhz = 28;
		else if (time > 550000 AND time < 590000)
			mhz = 25;
	}
	else if (processorFlag & AFF_68030)
	{
		if (time > 65000 AND time < 88000)
			mhz = 50;
		else if (time > 88000 AND time < 100000)
			mhz = 40;
		else if (time > 100000 AND time < 140000)
			mhz = 33;
		else if (time > 140000 AND time < 160000)
			mhz = 25;
	}
	else if (processorFlag & AFF_68020)
	{
		if (time > 248000 AND time < 300000)
			mhz = 14.19;
	}
	else if (processorFlag & AFF_68010)
	{
		if (time > 1030000 AND time < 1090000)
			mhz = 7.09;
	}
	else // 68000??
	{
		if (time > 1040000 AND time < 1080001)
			mhz = 7.09;
	}
	return (mhz);
}

VOID CPUSpeed (VOID)
{
	DOUBLE mhz;
	UWORD x1 = 8, y1 = 146;
	WORD col;
	
	mhz = CalcMhz (1);
	if (mhz)
		col = BLACK;
	else
		col = BLUE;

	sprintf (Dummy, "%lg", mhz);
	JEOWrite (rp, mainX + x1 + (5 * 8) - 3, y1, Dummy, col);

	if (!mhz)
	{
		sprintf (Dummy, "Couldn't recognize your CPU speed!\n\nIt will take about a minute to calculate the values.\nDo not touch anything until it's finished!");
		if (JEOSoundRequest (GLS (&L_MESSAGE), Dummy, "Cancel|Calculate|Cancel", S_MESSAGE) == 2)
			mhz = CalcMhz (200);
	}
 	Status (GLS (&L_PLEASE_WAIT));
}
