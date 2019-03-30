;/*
Delete Speed.o quiet
sc5 -j73 -v -y -ff -q5e Speed
slink LIB:c.o+"Speed.o" to Speed LIB LIB:JEO.lib LIB:scmffp.lib LIB:sc.lib LIB:amiga.lib
Copy Speed c:
quit
*/

#include <JEO:JEO.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <devices/timer.h>
#include <time.h>
#include <proto/graphics.h>

VOID Freq (VOID);

LONG mics = 0, secs = 0;
UBYTE Dummy[200];
extern struct ExecBase *SysBase;

#define MHZ_709		1	/* A500, A500+, A600 A2000 */
#define MHZ_1419	2	/* A1200 */
#define MHZ_25		3	/* A3000, A4000/30 */
#define MHZ_33		4	/* A3000 */
#define MHZ_40		5	/* A4000/040 */
#define MHZ_50		6	/* A4000/060 */

ULONG GetTime (VOID)
{
  struct timerequest tr;
  struct MsgPort *tport = 0;

  mics = secs = 0;
  if ((OpenDevice (TIMERNAME, UNIT_MICROHZ, (struct IORequest *)&tr, 0)))
  {
    printf ("Error 1\n");
    return (FALSE);
  }
  if (!(tport = (struct MsgPort *)CreatePort (0, 0)))
  {
    CloseDevice ((struct IORequest *)&tr);
    printf ("Error 2\n");
    return (FALSE);
  }
  tr.tr_node.io_Message.mn_Node.ln_Type	= NT_MESSAGE;
  tr.tr_node.io_Message.mn_Node.ln_Pri	= 20;
  tr.tr_node.io_Message.mn_Node.ln_Name	= NULL;
  tr.tr_node.io_Message.mn_ReplyPort	= tport;
  tr.tr_node.io_Command			= TR_GETSYSTIME;
  DoIO ((struct IORequest *)&tr);
  mics = tr.tr_time.tv_micro;
  secs = tr.tr_time.tv_secs;

  DeletePort (tport);
  CloseDevice ((struct IORequest *)&tr);
}

DOUBLE CalcMhz (ULONG max)
{
	ULONG tab = 0;
	ULONG high, low;
	LONG sec = 0, i, j;
	LONG mic = 0;
	ULONG time;
	UWORD processorFlag;
	DOUBLE mhz;

	if (max <= 1)
		max = 1;

	printf ("[0 p\n");
  SysBase = (*((struct ExecBase **)4));
	processorFlag = SysBase->AttnFlags;
	high = 0;
	low = 0xFFFFFFFF;
	for (i = 0; i < max; i++)
	{
	  Forbid ();
  	GetTime ();
	  sec = secs;
  	mic = mics;

	  Forbid ();
		Freq ();			// LOOP
	  GetTime ();
	  Permit ();

	  sec = secs - sec;
  	mic = mics - mic;

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
			if (i == 0)
				printf ("\n");
			printf ("[A[1K %3ld of %ld (%ld) (%ld)\n", i + 1, max, tab / (i+1), high - low);
		}
	}

	if (max > 1)
	{
		printf ("\n  Low = %ld  <-- Please send the low and high value to:", low);
		printf ("\n High = %ld  <-- Email: virusexecutor@c2i.net -  Thanks :)\n", high);
	}

	Delay (5);
	if (mic < 0)
	{
		mic += 1000000;
		sec--;
	}

	printf ("\n[33m");
	mhz = 0;
	if (processorFlag & AFF_68060)
	{
		if (time > 220000 AND time < 250000)
			mhz = 66;
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
		if (time > 1040000 AND time < 1060001)
			mhz = 7.09;
	}
	else // 68000??
	{
		if (time > 1040000 AND time < 1060001)
			mhz = 7.09;
	}
	printf ("[1 p");
	return (mhz);
}

VOID main (int argc, char **argv)
{
	DOUBLE mhz;
	UBYTE gfx, display;

	mhz = CalcMhz (atoi (argv[1]));

	if (mhz)
	{
		printf ("[32m%lg Mhz\n\n[0m", mhz);
		printf (" If this is NOT right, please let me know!\n Email: virusexecutor.c2i.net or dial my BBS (+47) 69 35 30 97\n\n[0m");
	}
/*
	else
	{
		printf ("[0m\n\n Couldn't recognice your CPU speed!\n\n It will take about a minute to calculate the values.\n\
 Do not touch anything while Speed is working!\n\n Press <RETURN> to start calculating...\n\n ");
		gets (Dummy);
		mhz = CalcMhz (200);
		printf ("[0m\n\n");
	}

	printf ("\n");
	switch (gfx = CheckGfx ())
	{
		case 0x00: printf (" AGNUS 512K"); break;	// 8371
		case 0x01: printf (" AGNUS 1MB "); break;	// 8372A
		case 0x02: printf (" AGNUS 2MB "); break;	// 8372B
		case 0x23: printf (" AGA ALICE "); break;	// ?
		default:   printf (" $%lX ???  ", gfx); break;
	}
	printf ("\n");
	switch (display = CheckDisplay ())
	{
		case 0xFC: printf (" ECS DENISE"); break;	// 8373
		case 0xF8: printf (" AGA LISA  "); break;	// ?
		default:   printf (" STD DENISE"); break;  // 8362
	}
*/
	printf ("\n\n");
}
