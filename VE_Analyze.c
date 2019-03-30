#define DEBUG 0

#include <JEO:JEO.h>
#include <proto/dos.h>
//#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include "VE_Prototypes.h"

#define START_OFF	10

#define BUFSIZE		1024
#define VALUE		  30
#define MAX		(BUFSIZE / 2)

#define COLD	0x002A
#define COOL	0x002E
#define WARM	0x0032
#define MEM		0x0222
#define CHECK	0x022A
#define TAG		0x0226

#define _LVOOpen	-0x001E
#define _LVOClose	-0x0024
#define _LVORead	-0x002A
#define _LVOWrite	-0x0030
#define _LVOSeek	-0x0042

BOOL EncryptBB (UWORD *AnalyzeBuf, UWORD i, WORD unit);

BOOL decrypt_flag = FALSE;
BYTE dbra_reg, lea_reg, val_reg, eor_dreg, eor_areg, not_reg, and_reg;
ULONG and_val = 0;

extern UBYTE *Dummy;
extern UBYTE *Drive[];
extern UWORD viewCol[];

extern BOOL format;
extern UWORD bootblockWrite;
extern UWORD writeLen;
extern UWORD writeOffset;
extern UWORD writeDoIOV12;
extern UWORD writeDoIO;
extern UWORD CMDWrite;

UWORD writeDoIOV13;
UWORD changeDoIO;
UWORD trackdiskWedge;

UWORD writeTab[] =
{
  0x117C, 0x137C, 0x157C, 0x177C, 0x197C, 0x1B7C, 0x1D7C, 0x217C, 0x237C,
  0x257C, 0x277C, 0x297C, 0x2B7C, 0x2D7C, 0x317C, 0x337C, 0x357C, 0x377C,
  0x397C, 0x3B7C, 0x3D7C
};

BOOL memVirus;

#if DEBUG
VOID ShowMe (UWORD off, UBYTE *String)
{
  printf ("%04lx: %s\n", off * 2, String);
}
#endif

VOID CheckMemVectors (UWORD *AnalyzeBuf, ULONG i)
{
  UWORD v;

  v = AnalyzeBuf[i];
  if ((v == WARM) OR (v == CHECK))
    return;
  if ((v == COLD) OR (v == COOL) OR (v == MEM) OR (v == TAG))
  {
    if ((AnalyzeBuf[i-1] == 0x2D40) OR (AnalyzeBuf[i-1] == 0x2D48) OR (AnalyzeBuf[i-1] == 0x2D49 OR AnalyzeBuf[i-1] == 0x4DEE))
    {
      memVirus = TRUE;
#if DEBUG 
      SPrintF (Dummy, "Setter $%02lx Capture til en ukjent verdi (1)", v);
      ShowMe (i - 1, Dummy);
#endif
    }
    else if (AnalyzeBuf[i-3] == 0x2D7C)
    {
      if (AnalyzeBuf[i-2] != 0 OR AnalyzeBuf[i-1] != 0);
      {
        memVirus = TRUE;
#if DEBUG 
        ShowMe (i - 1, "Setter Capture til ????????");
#endif
      }
    }
    else if (AnalyzeBuf[i-2] == 0x2D7A)
    {
#if DEBUG 
      ShowMe (i - 2, "Setter Capture til en ukjent verdi (2)");
#endif
      memVirus = TRUE;
    }
  }
}

VOID Analyze (UWORD *AnalyzeBuf, WORD unit)
{
  ULONG i, j;
  BOOL dummyFlag = OFF;
  BOOL CMDRead = FALSE, CMDRead1 = FALSE;
  BOOL buttons = FALSE;
  WORD virus = 0;
  BOOL encrypt = FALSE;
  UWORD virusKiller = FALSE;
  BOOL virusChecker = FALSE;
  BOOL reset = FALSE;
  BOOL fileVirus = FALSE;
  BOOL LVOOpen = FALSE;
  BOOL LVOClose = FALSE;
  BOOL LVORead = FALSE;
  BOOL LVOWrite = FALSE;
  BOOL LVOSeek = FALSE;

  BOOL coldkill = FALSE;
  BOOL coolkill = FALSE;
  BOOL warmkill = FALSE;
  BOOL memkill = FALSE;
  BOOL checkkill = FALSE;
  BOOL tagkill = FALSE;

  BOOL coldchk = FALSE;
  BOOL coolchk = FALSE;
  BOOL warmchk = FALSE;
  BOOL memchk = FALSE;
  BOOL checkchk = FALSE;
  BOOL tagchk = FALSE;

	BOOL encrypt_flag = FALSE;

	dbra_reg = lea_reg = val_reg = eor_dreg = eor_areg = not_reg = not_reg = -1;
	and_val = 0;
#if DEBUG 
    printf ("\n*********************** NEW ANALYZE CHECK *******************\n");
#endif

  writeLen = writeOffset = writeDoIOV12 = writeDoIOV13 = changeDoIO = 0;
  trackdiskWedge = writeDoIO = bootblockWrite = CMDWrite = 0;
  format = memVirus = FALSE;
  Dummy[0] = 0;

  for (i = 0x00C; i < (BUFSIZE / 2) - 4; i++)
  {
		if (encrypt_flag)
		{
			encrypt_flag = FALSE;

			if (!decrypt_flag)
			{
				if (EncryptBB (AnalyzeBuf, i, unit))	// Klarte og decryptere :)
				{
					decrypt_flag = TRUE;	// Så vi ikke tar tilbake...
					ViewBoot (unit);
					i = 0;

					continue;
				}
			}
		}

// ********************************** File ? **********************************
		if (AnalyzeBuf[i] == 0x4EAE)		// jsr
    {
      if (i < MAX - 1)
      {
        switch (AnalyzeBuf[i+1])
        {
          case _LVOOpen:
          {
            LVOOpen = TRUE;
#if DEBUG
            ShowMe (i, "_LVOOpen");
#endif
            break;
          }
          case _LVOClose:
          {
            LVOClose = TRUE;
#if DEBUG
            ShowMe (i, "_LVOClose");
#endif
            break;
          }
          case _LVORead:
          {
            LVORead = TRUE;
 #if DEBUG
            ShowMe (i, "_LVORead");
 #endif
            break;
          }
          case _LVOWrite:
          {
            LVOWrite = TRUE;
#if DEBUG
            ShowMe (i, "_LVOWrite");
#endif
            break;
          }
          case _LVOSeek:
          {
            LVOSeek = TRUE;
#if DEBUG
            ShowMe (i, "_LVOSeek");
#endif
            break;
          }
        }
      }
    }
// ********************************** Buttons *********************************
    // btst #6,$BFE001 Left mouse button 
    // btst #7,$BFE001 Joystick button 
    // btst #2,$DFF016 Right mouse button 

    if ((!buttons) AND i < MAX - 3)
    {
      if (AnalyzeBuf[i] == 0x0839)	// btst 
      {
        if (AnalyzeBuf[i+1] == 0x0007 OR AnalyzeBuf[i+1] == 0x0006)
        if (AnalyzeBuf[i+2] == 0x00BF AND AnalyzeBuf[i+3] == 0xE001)
        {
          buttons = TRUE;
#if DEBUG 
          ShowMe (i, "Buttons");
#endif
          continue;
        }
        if (AnalyzeBuf[i+1] == 0x0002)
        if (AnalyzeBuf[i+2] == 0x00DF AND AnalyzeBuf[i+3] == 0xF016)
        {
          buttons = TRUE;
#if DEBUG 
          ShowMe (i, "Buttons");
#endif
          continue;
        }
      }
    }       
//********************************* _LVODoIO *******************************
    if (AnalyzeBuf[i] == 0x4EAE)		// jsr   _LVODoIO 
    {
      if (i < MAX - 1 AND AnalyzeBuf[i+1] == 0xFE38)
        writeDoIO++;
      continue;
    }
    if (AnalyzeBuf[i] == 0x4EB9)		// jsr   $FC06DC 
    {
      if (i < MAX - 2 AND AnalyzeBuf[i+1] == 0x00FC AND AnalyzeBuf[i+2] == 0x06DC)
        writeDoIOV12++;
      continue;
    }
    if (AnalyzeBuf[i] == 0x4EB9)		// jsr   $FC0718 
    {
      if (i < MAX - 2 AND AnalyzeBuf[i+1] == 0x00FC AND AnalyzeBuf[i+2] == 0x0718)
        writeDoIOV13++;
      continue;
    }
    if (AnalyzeBuf[i] == 0x4EF9)		// jmp 
    {
      if (i < MAX - 2)				// jmp   $FC06DC 
      {
        if (AnalyzeBuf[i+1] == 0x00FC AND AnalyzeBuf[i+2] == 0x06DC)
          writeDoIOV12++;
      }
      if (i < MAX - 2)				// jmp   $FC0718 
      {
        if (AnalyzeBuf[i+1] == 0x00FC AND AnalyzeBuf[i+2] == 0x0718)
          writeDoIOV13++;
      }
      if (i < MAX - 2 AND (!reset))	// jmp	$FC0000 
      {
        if (AnalyzeBuf[i+1] == 0x00FC AND AnalyzeBuf[i+2] == 0x0000)
        {
#if DEBUG 
          ShowMe (i, "jmp $FC0000");
#endif
          reset = TRUE;
        }
      }
      continue;
    }
    if (i < MAX + 3)
    {
      if (AnalyzeBuf[ i ] == 0x2D7C)	// move.l #$FC06DC,-$1C6(a6) 
      if (AnalyzeBuf[i+1] == 0x00FC)
      if (AnalyzeBuf[i+2] == 0x06DC)
      if (AnalyzeBuf[i+3] == 0xFE3A)
      {
 #if DEBUG 
        ShowMe (i, "move.l #$FC06DC,-$1C6(a6)");
 #endif
        writeDoIOV12++;
      }
    }
    if (i < MAX + 3)
    {
      if (AnalyzeBuf[ i ] == 0x2D7C)	// move.l #$FC06DC,-$1C6(a6) 
      if (AnalyzeBuf[i+1] == 0x00FC)
      if (AnalyzeBuf[i+2] == 0x0718)
      if (AnalyzeBuf[i+3] == 0xFE3A)
      {
#if DEBUG 
        ShowMe (i, "move.l #$FC0718,-$1C6(a6)");
 #endif
        writeDoIOV13++;
      }
    }

//**************************** Clear Vectors ? *****************************
    if (i < MAX - 1)
    {
      if (AnalyzeBuf[i] == 0x42AE)	// clr.l  $V(a6) 
      {
        switch (AnalyzeBuf[i+1])
        {
          case COLD:  if (!coldkill) { coldkill = TRUE; virusKiller++; } break;
          case COOL:  if (!coolkill) { coolkill = TRUE; virusKiller++; } break;
          case WARM:  if (!warmkill) { warmkill = TRUE; virusKiller++; } break;
          case MEM:   if (!memkill) { memkill = TRUE; virusKiller++; } break;
          case CHECK: if (!checkkill) { checkkill = TRUE; virusKiller++; } break;
          case TAG:   if (!tagkill) { tagkill = TRUE; virusKiller++; } break;
        }
        if (virusKiller)
        {
#if DEBUG 
          SPrintF (Dummy, "virusKiller clr.l $%lx,(a6)", AnalyzeBuf[i+1]);
          ShowMe (i, Dummy);
#endif
        }
        continue;
      }
    }
    if (i < MAX - 3)
    if (AnalyzeBuf[ i ] == 0x2D7C)
    if (AnalyzeBuf[i+1] == 0x0000)
    if (AnalyzeBuf[i+2] == 0x0000)
    {
      switch (AnalyzeBuf[i+3])
      {
        case COLD:  if (!coldkill) { coldkill = TRUE; virusKiller++; } break;
        case COOL:  if (!coolkill) { coolkill = TRUE; virusKiller++; } break;
        case WARM:  if (!warmkill) { warmkill = TRUE; virusKiller++; } break;
        case MEM:   if (!memkill) { memkill = TRUE; virusKiller++; } break;
        case CHECK: if (!checkkill) { checkkill = TRUE; virusKiller++; } break;
        case TAG:   if (!tagkill) { tagkill = TRUE; virusKiller++; } break;
      }
      if (virusKiller)
      {
#if DEBUG 
        ShowMe (i, "virusKiller move.l #0,$V(a6)");
#endif
      }
      continue;
    }
//******************************** CheckVectors ****************************
    if (i < MAX - 1)
    {
      if (AnalyzeBuf[i] == 0x4AAE)	// tst.l  $V(a6) 
      {
        switch (AnalyzeBuf[i+1])
        {
          case COLD:  if (!coldchk) { coldchk = TRUE; virusChecker++; } break;
          case COOL:  if (!coolchk) { coolchk = TRUE; virusChecker++; } break;
          case WARM:  if (!warmchk) { warmchk = TRUE; virusChecker++; } break;
          case MEM:   if (!memchk) { memchk = TRUE; virusChecker++; } break;
          case CHECK: if (!checkchk) { checkchk = TRUE; virusChecker++; } break;
          case TAG:   if (!tagchk) { tagchk = TRUE; virusChecker++; } break;
        }
        if (virusChecker)
        {
#if DEBUG 
          ShowMe (i, "virusChecker tst.l $V(a6)");
#endif
        }
        continue;
      }
    }
//********************************** RESET *********************************
    if (i < MAX)
    {
      if (AnalyzeBuf[i] == 0x4E70)	// reset 
      {
        reset = TRUE;
#if DEBUG
       	ShowMe (i - 1, "Reset");
#endif
        continue;
      }
    }
//************************* Change DoIO offset ? ****************************
    // move.l  (a0-a7),-$1C6(a0-a7) 
    // move.l  (d0-d7),-$1C6(a0-a7) 

    if (i < MAX)
    {
      if (AnalyzeBuf[i] == 0xFE3A)	// -$1C6 
      {
        for (j = 0x2140; j <= 0x214F; j++)
        {
          if (AnalyzeBuf[i-1] == j)
          {
#if DEBUG
            ShowMe (i - 1, "Change DoIO = TRUE");
#endif
            changeDoIO++;
            break;
          }
        }
        for (j = 0x2340; j <= 0x234F; j++)
        {
          if (AnalyzeBuf[i-1] == j)
          {
#if DEBUG
            ShowMe (i - 1, "Change DoIO = TRUE");
#endif
            changeDoIO++;
            break;
          }
        }
        for (j = 0x2540; j <= 0x254F; j++)
        {
          if (AnalyzeBuf[i-1] == j)
          {
#if DEBUG
            ShowMe (i - 1, "Change DoIO = TRUE");
#endif
            changeDoIO++;
            break;
          }
        }
        for (j = 0x2740; j <= 0x274F; j++)
        {
          if (AnalyzeBuf[i-1] == j)
          {
#if DEBUG
            ShowMe (i - 1, "Change DoIO = TRUE");
#endif
            changeDoIO++;
            break;
          }
        }
        for (j = 0x2940; j <= 0x294F; j++)
        {
          if (AnalyzeBuf[i-1] == j)
          {
#if DEBUG
            ShowMe (i - 1, "Change DoIO = TRUE");
#endif
            changeDoIO++;
            break;
          }
        }
        for (j = 0x2B40; j <= 0x2B4F; j++)
        {
          if (AnalyzeBuf[i-1] == j)
          {
#if DEBUG
            ShowMe (i - 1, "Change DoIO = TRUE");
#endif
            changeDoIO++;
            break;
          }
        }
        for (j = 0x2D40; j <= 0x2D4F; j++)
        {
          if (AnalyzeBuf[i-1] == j)
          {
#if DEBUG
            ShowMe (i - 1, "Change DoIO = TRUE");
#endif
            changeDoIO++;
            break;
          }
        }
        for (j = 0x2F40; j <= 0x2F4F; j++)
        {
          if (AnalyzeBuf[i-1] == j)
          {
#if DEBUG
            ShowMe (i - 1, "Change DoIO = TRUE");
#endif
            changeDoIO++;
            break;
          }
        }

        continue;
      }
    }

//**************************** trackdisk wedge ****************************
    // cmp.w.b #2,$1C(a1) 
    // cmp.w.b #3,$1C(a1) 
    // cmp.w.b #11,$1C(a1) 

    // cmp.w.b #2,$1D(a1) 
    // cmp.w.b #3,$1D(a1) 
    // cmp.w.b #11,$1C(a1) 

    if (i < MAX - 2)
    {
      if (AnalyzeBuf[i] == 0x0C69 OR AnalyzeBuf[i] == 0x0C29)
      if (AnalyzeBuf[i+1] == 0x0002 OR AnalyzeBuf[i+1] == 0x0003 OR AnalyzeBuf[i+1] == 0x000B)
      if (AnalyzeBuf[i+2] == 0x001C OR AnalyzeBuf[i+2] == 0x001D)
      {
#if DEBUG
        ShowMe (i, "trackdisk wedge = TRUE");
#endif
        trackdiskWedge++;
        continue;
      }
    }
//******************************** CMD_WRITE *******************************
    // move.w  #3,$1C(a0-a6) 
    // move.w  #3,$1D(a0-a6) 
    // move.w  #3,$9C(a0-a6) 

    if (i >= 2)
    {
      if (AnalyzeBuf[i] == 0x001C OR AnalyzeBuf[i] == 0x001D OR AnalyzeBuf[i] == 0x009C)
      {
	      if (AnalyzeBuf[i-1] == 0x0003)
	      {
	  	    for (j = 0; j < sizeof (writeTab) / 2; j++)
  	  	  {
      	  	if (AnalyzeBuf[i-2] == writeTab[j])
	      	  {
#if DEBUG
	  	        printf ("------------------------------------\n");
#endif
  	  	      AnalyzeWrite (AnalyzeBuf, i);
#if DEBUG
    	  	    printf ("------------------------------------\n");
#endif
      	  	  continue;
      	  	}
        	}
        }
      }
    }
//********************************** ENCRYPT *******************************
	  if (!encrypt)
    {
    	UWORD k;

			for (k = 0x4618; k <= 0x461E; k++)
			{
				if (AnalyzeBuf[i] == k)
				{
#if DEBUG 
        ShowMe (i, "Crypt not.b (a0-a6)+");
 #endif
					encrypt =	encrypt_flag = TRUE;
  	      break;
				}
			}
			if (encrypt)
				continue;
			switch (AnalyzeBuf[i])	// eor.b.w.l d0-d6,$########
			{
				case 0xB339: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB539: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB739: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB939: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xBB39: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xBD39: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB179: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB379: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB579: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB779: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB979: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xBB79: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xBD79: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB1B9: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB3B9: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB5B9: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB7B9: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xB9B9: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xBBB9: encrypt = TRUE; encrypt_flag = TRUE; continue;
				case 0xBDB9: encrypt = TRUE; encrypt_flag = TRUE; continue;
			}
			if (!encrypt)
			{
				if (AnalyzeBuf[i] >= 0x0458 AND AnalyzeBuf[i] <= 0x045E) // sub.w #$xxxx,(a0-a6)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
// **************************
				if (AnalyzeBuf[i] >= 0x0218 AND AnalyzeBuf[i] <= 0x021E) // and.b #$x,(a0-a6)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
// **************************
				if (AnalyzeBuf[i] >= 0x0A00 AND AnalyzeBuf[i] <= 0x0A07) // eor.b #$75,d0-a7
					{ encrypt =	encrypt_flag = TRUE; continue; }
// **************************
				if (AnalyzeBuf[i] >= 0xB100 AND AnalyzeBuf[i] <= 0xB107) // eor.b d0,d0-d7
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB300 AND AnalyzeBuf[i] <= 0xB307)
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB500 AND AnalyzeBuf[i] <= 0xB507)
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB700 AND AnalyzeBuf[i] <= 0xB707)
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB900 AND AnalyzeBuf[i] <= 0xB907)
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBB00 AND AnalyzeBuf[i] <= 0xBB07)
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBD00 AND AnalyzeBuf[i] <= 0xBD07)
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBF00 AND AnalyzeBuf[i] <= 0xBF07)
					{ encrypt =	encrypt_flag = TRUE; continue; }
// **************************
				if (AnalyzeBuf[i] >= 0xB110 AND AnalyzeBuf[i] <= 0xB127) // eor.b d0,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB310 AND AnalyzeBuf[i] <= 0xB327) // eor.b d1,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB510 AND AnalyzeBuf[i] <= 0xB527) // eor.b d2,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB710 AND AnalyzeBuf[i] <= 0xB727) // eor.b d3,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB910 AND AnalyzeBuf[i] <= 0xB927) // eor.b d4,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBB10 AND AnalyzeBuf[i] <= 0xBB27) // eor.b d5,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBD10 AND AnalyzeBuf[i] <= 0xBD27) // eor.b d6,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBF10 AND AnalyzeBuf[i] <= 0xBF27) // eor.b d7,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
// **************************
				if (AnalyzeBuf[i] >= 0xB150 AND AnalyzeBuf[i] <= 0xB167) // eor.w d0,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB350 AND AnalyzeBuf[i] <= 0xB367) // eor.w d1,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB550 AND AnalyzeBuf[i] <= 0xB567) // eor.w d2,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB750 AND AnalyzeBuf[i] <= 0xB767) // eor.w d3,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB950 AND AnalyzeBuf[i] <= 0xB967) // eor.w d4,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBB50 AND AnalyzeBuf[i] <= 0xBB67) // eor.w d5,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBD50 AND AnalyzeBuf[i] <= 0xBD67) // eor.w d6,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBF50 AND AnalyzeBuf[i] <= 0xBF67) // eor.w d7,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
// **************************
				if (AnalyzeBuf[i] >= 0xB190 AND AnalyzeBuf[i] <= 0xB1A7) // eor.l d0,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB390 AND AnalyzeBuf[i] <= 0xB3A7) // eor.l d1,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB590 AND AnalyzeBuf[i] <= 0xB5A7) // eor.l d2,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB790 AND AnalyzeBuf[i] <= 0xB7A7) // eor.l d3,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xB990 AND AnalyzeBuf[i] <= 0xB9A7) // eor.l d4,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBB90 AND AnalyzeBuf[i] <= 0xBBA7) // eor.l d5,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBD90 AND AnalyzeBuf[i] <= 0xBDA7) // eor.l d6,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
				if (AnalyzeBuf[i] >= 0xBF90 AND AnalyzeBuf[i] <= 0xBFA7) // eor.l d7,-(a0-a7)+
					{ encrypt =	encrypt_flag = TRUE; continue; }
			}
      if (AnalyzeBuf[i] == 0xB118 OR AnalyzeBuf[i] == 0xB158 OR AnalyzeBuf[i] == 0xB198)
      {
#if DEBUG 
        ShowMe (i, "Crypt 0");
 #endif
				encrypt =	encrypt_flag = TRUE;
        continue;
      }

      // eor.b   #$val,(a0-a6)+ 
      for (j = 0x0A18; j <= 0x0A1E; j++)
      {
        if (AnalyzeBuf[i] == j)
        {
#if DEBUG
          ShowMe (i, "Crypt 1");
#endif
					encrypt =	encrypt_flag = TRUE;
          continue;
        }
      }
      // eor.w   #$val,(a0-a6)+ 
      for (j = 0x0A58; j <= 0x0A5E; j++)
      {
        if (AnalyzeBuf[i] == j)
        {
#if DEBUG
          ShowMe (i, "Crypt 3");
#endif
					encrypt =	encrypt_flag = TRUE;
          continue;
        }
      }
      // eor.l   #$val,(a0-a6)+ 
      for (j = 0x0A98; j <= 0x0A9E; j++)
      {
        if (AnalyzeBuf[i] == j)
        {
#if DEBUG
          ShowMe (i, "Crypt 3");
#endif
					encrypt =	encrypt_flag = TRUE;
          continue;
        }
      }
    }
//******************************** TD_FORMAT *******************************
    // move.w  #$B,$1C(a0-a6) 
    // move.w  #$B,$1D(a0-a6) 
    // move.w  #$B,$9C(a0-a6) 

    if (i >= 2 AND (!format))
    {
      if (AnalyzeBuf[i] == 0x001C OR AnalyzeBuf[i] == 0x001D OR AnalyzeBuf[i] == 0x009C)
      if (AnalyzeBuf[i-1] == 0x000B)
      if (AnalyzeBuf[i-2] == 0x317C OR AnalyzeBuf[i-2] == 0x337C OR
          AnalyzeBuf[i-2] == 0x357C OR AnalyzeBuf[i-2] == 0x377C OR
          AnalyzeBuf[i-2] == 0x397C OR AnalyzeBuf[i-2] == 0x3B7C OR
          AnalyzeBuf[i-2] == 0x3D7C)
      {
#if DEBUG 
        ShowMe (i - 2, "Format code!");
 #endif
        format = TRUE;
        continue;
      }
    }

//******************************* Offset ********************************
/*
    // clr.l  $2C(a1) 

    if (i >= 2 AND (!CMDRead))
    {
      if (AnalyzeBuf[i] == 0x002C)
      if (AnalyzeBuf[i-1] == 0x42A9)
      {
        CMDRead = TRUE;
        continue;
      }
    }
*/
//******************************* CMD_READ *********************************
    // move.w  #2,$1C(a0-a6) 
    // move.w  #2,$1D(a0-a6) 
    // move.w  #2,$9C(a0-a6) 

    if (i >= 2 AND (!CMDRead))
    {
      if (AnalyzeBuf[i] == 0x001C OR AnalyzeBuf[i] == 0x001D OR AnalyzeBuf[i] == 0x009C)
      if (AnalyzeBuf[i-1] == 0x0002)
      if (AnalyzeBuf[i-2] == 0x317C OR AnalyzeBuf[i-2] == 0x337C OR
          AnalyzeBuf[i-2] == 0x357C OR AnalyzeBuf[i-2] == 0x377C OR
          AnalyzeBuf[i-2] == 0x397C OR AnalyzeBuf[i-2] == 0x3B7C OR
          AnalyzeBuf[i-2] == 0x3D7C)
      {
        CMDRead = TRUE;
        continue;
      }
    }
    if (i >= 1 AND (!CMDRead))
    {
      if (AnalyzeBuf[ i ] == 0x0002)	// move.w  #2,(a1) 
      if (AnalyzeBuf[i-1] == 0x32BC)
      {
        CMDRead = TRUE;
        continue;
      }
    }

    // move.w	#$7FFF,$(9A,96),(a0-a5) 
    if (i >= 2 AND (!CMDRead1))
    {
      if (AnalyzeBuf[i] == 0x0096 OR AnalyzeBuf[i] == 0x009A)
      if (AnalyzeBuf[i-1] == 0x7FFF)
      if (AnalyzeBuf[i-2] == 0x317C OR AnalyzeBuf[i-2] == 0x337C OR
      AnalyzeBuf[i-2] == 0x357C OR AnalyzeBuf[i-2] == 0x377C OR
      AnalyzeBuf[i-2] == 0x397C OR AnalyzeBuf[i-2] == 0x3B7C)
      {
        CMDRead1 = TRUE;
        continue;
      }
    }

    // move.w	#$7FFF,($DFF096, DFF09A) 
    if (i >= 3 AND (!CMDRead1))
    {
      if (AnalyzeBuf[i] == 0xF096 OR AnalyzeBuf[i]== 0xF09A)
      if (AnalyzeBuf[i-1] == 0x00DF)
      if (AnalyzeBuf[i-2] == 0x7FFF)
      if (AnalyzeBuf[i-3] == 0x33FC)
      {
        CMDRead1 = TRUE;
        continue;
      }
    }

    // move.w	#$7FFF,(a0-a6) 
    if (i >= 1 AND (!CMDRead1))
    {
      if (AnalyzeBuf[i] == 0x7FFF)
      if (AnalyzeBuf[i-1] == 0x30BC OR AnalyzeBuf[i-1] == 0x32BC OR
      AnalyzeBuf[i-1] == 0x34BC OR AnalyzeBuf[i-1] == 0x36BC OR
      AnalyzeBuf[i-1] == 0x38BC OR AnalyzeBuf[i-1] == 0x3ABC OR
      AnalyzeBuf[i-1] == 0x3CBC)
      {
        CMDRead1 = TRUE;
        continue;
      }
    }

    // move.l	#$55555555,(d0-d6) 
    if (i >= 2 AND (!CMDRead1))
    {
      if (AnalyzeBuf[i] == 0x5555)
      if (AnalyzeBuf[i-1] == 0x5555)
      if (AnalyzeBuf[i-2] == 0x203C OR AnalyzeBuf[i-2] == 0x223C OR
      AnalyzeBuf[i-2] == 0x243C OR AnalyzeBuf[i-2] == 0x263C OR
      AnalyzeBuf[i-2] == 0x283C OR AnalyzeBuf[i-2] == 0x2B3C OR
      AnalyzeBuf[i-2] == 0x2C3C)
      {
        CMDRead1 = TRUE;
        continue;
      }
    }

//******************************** MEMCheck ************************************
    CheckMemVectors (AnalyzeBuf, i);
//**************************************************************************
  }

  if (bootblockWrite >= 2)	// Writes to bootblock 
  {
    if (memVirus)
    {
      if ((virusChecker > 2) OR (virusKiller > 2))
        virus = 20;
      else
        virus = 99;
    }
    else if (virusChecker)
      virusKiller = TRUE;
    else
      virus = 1;
  }
  else if (writeOffset)
  {
    if (memVirus)
      virus = 99;
    else if (virusChecker)
      virusKiller = TRUE;
    else
      virus = 1;
  }
  else if (writeLen)
  {
    if (memVirus)
      virus = 99;
    else if (virusChecker)
      virusKiller = TRUE;
    else
      virus = 1;
  }
  else if (CMDWrite)
  {
    if (memVirus)
      virus = 99;
    else if (virusChecker)
      virusKiller = TRUE;
    else
      virus = 1;
  }
  else
    virus = 0;

  if (LVOOpen)
  if (LVOClose)
  if (LVOWrite)
  {
    fileVirus = TRUE;
    virus += 70;
  }

  if (format)
    virus += 80;
  else if (trackdiskWedge)
    virusKiller += 19;

	if (buttons)
	{
		if (!memVirus)
			virus -= 15;
	}
	else if (memVirus)
	{
		if (bootblockWrite >= 2)
			virus += 99;
		else if (writeOffset)
			virus += 75;
		else if (writeLen)
			virus += 50;

		if (CMDWrite AND (!format))
			virus += 90;
   	else if (format)
   		virus += 50;
  }

	if (changeDoIO AND (virusKiller <= 20))
		virus += 39;

	if (virus > 0)		// Maybe a VIRUS 
	{
  	char Hold[100];

    if (virus >= 100)
      virus = 99;

    SPrintF (Dummy, GLS (&L_UNKNOWN_BOOTBLOCK), virus);
    DiskStatus (Dummy, unit);

    SPrintF (Dummy, GLS (&L_ANALYZE_PERCENT_VIRUS), Drive[unit], virus);

    if (bootblockWrite >= 2)
    {
    	SPrintF (Hold, GLS (&L_WRITES_TO_BOOTBLOCK1), 100);
      strcat (Dummy, Hold);
    }
    else if (writeOffset)
    {
    	SPrintF (Hold, GLS (&L_WRITES_TO_BOOTBLOCK1), 99);
      strcat (Dummy, Hold);
    }
    else if (writeLen)
    {
    	SPrintF (Hold, GLS (&L_WRITES_TO_BOOTBLOCK1), 95);
      strcat (Dummy, Hold);
    }
    else if (CMDWrite AND (!format))
      strcat (Dummy, GLS (&L_WRITES_TO_UNKNOWN));

    if (format)
      strcat (Dummy, GLS (&L_DESTROYS_THE_DISK));
    if (fileVirus)
      strcat (Dummy, GLS (&L_COULD_BE_FILEVIRUS));

    if (writeDoIOV12 AND (!writeDoIO))
    {
    	SPrintF (Hold, GLS (&L_SEEMS_TO_KICKWORK), "1.2");
      strcat (Dummy, Hold);
    }

    if (writeDoIOV13 AND (!writeDoIO))
    {
    	SPrintF (Hold, GLS (&L_SEEMS_TO_KICKWORK), "1.3");
      strcat (Dummy, Hold);
    }

    rtEZRequestTags (Dummy, GLS (&L_OK),
      NULL, NULL, RTEZ_Flags, EZREQF_CENTERTEXT, RT_ReqPos, REQPOS_CENTERSCR, RTEZ_ReqTitle, GLS (&L_WARNING), RT_TextAttr, &topaz8, TAG_END);
  }
  else		// Not a VIRUS!!!
  {
    Dummy[0] = 0;
    dummyFlag = OFF;

    if (virusKiller)
    {
    	SPrintF (Dummy, GLS (&L_UNKNOWN_BB), GLS (&L_A_VIRUSKILLER));
      DiskStatus (Dummy, unit);
    }
    else if (CMDRead OR (CMDRead1 AND (!buttons)))
    {
    	SPrintF (Dummy, GLS (&L_UNKNOWN_BB), GLS (&L_A_LOADER));
      DiskStatus (Dummy, unit);
    }
    else if (virusChecker AND reset)
    {
    	SPrintF (Dummy, GLS (&L_UNKNOWN_BB), GLS (&L_A_VIRUSKILLER));
      DiskStatus (Dummy, unit);
    }
    else if (virusChecker)
   	{
    	SPrintF (Dummy, GLS (&L_UNKNOWN_BB), GLS (&L_A_VIRUSCHECKER));
      DiskStatus (Dummy, unit);
    }
    else if (reset OR buttons)
   	{
    	SPrintF (Dummy, GLS (&L_UNKNOWN_BB), GLS (&L_AN_UTILITY_INTRO));
      DiskStatus (Dummy, unit);
    }
    else  if (encrypt)
   	{
    	SPrintF (Dummy, GLS (&L_UNKNOWN_BB), GLS (&L_CRYPTED_VIRUS));
      DiskStatus (Dummy, unit);
    }
    else
      DiskStatus (GLS (&L_HARMLESS_BB), unit);
  }
}

#define D_B	1
#define D_W	2
#define D_L	3

UWORD Find_len (UWORD *Buf, UWORD start)
{
	UWORD j;
	BYTE len_reg;

//	MOVE.W
	len_reg = -1;
	for (j = start; j < start + START_OFF; j++)
	{
		switch (Buf[j])
		{
			case 0x303C: len_reg = 0; break;
			case 0x323C: len_reg = 1; break;
			case 0x343C: len_reg = 2; break;
			case 0x363C: len_reg = 3; break;
			case 0x383C: len_reg = 4; break;
			case 0x3A3C: len_reg = 5; break;
			case 0x3C3C: len_reg = 6; break;
			case 0x3E3C: len_reg = 7; break;
		}
		if (len_reg != -1)
		{
			if (len_reg == dbra_reg)	// Samme som dbra?
				return (Buf[j+1]);
		}
	}

	// MOVE.L
	len_reg = -1;
	for (j = start; j < start + START_OFF; j++)
	{
		switch (Buf[j])
		{
			case 0x203C: len_reg = 0; break;
			case 0x223C: len_reg = 1; break;
			case 0x243C: len_reg = 2; break;
			case 0x263C: len_reg = 3; break;
			case 0x283C: len_reg = 4; break;
			case 0x2A3C: len_reg = 5; break;
			case 0x2C3C: len_reg = 6; break;
			case 0x2E3C: len_reg = 7; break;
		}
		if (len_reg != -1 AND Buf[j+1] == 0x0000)
		{
			if (len_reg == dbra_reg)	// Samme som dbra?
			{
				return (Buf[j+2]);
			}
		}
	}

	return (FALSE);
}

UBYTE Find_eor (UWORD *Buf, UWORD start)
{
	UWORD j, k, reg;

	if (start < 0)
		start = 0;
	for (j = start; j < start + START_OFF; j++)
	{
		// *********************** EOR.B ***************************************
		for (k = 0xB118, reg = 0; k <= 0xB11E; k++, reg++)	// eor.b d0,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 0;
				eor_areg = reg;
				return (D_B);
			}
		}
		for (k = 0xB318, reg = 0; k <= 0xB31E; k++, reg++)	// eor.b d0,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 1;
				eor_areg = reg;
				return (D_B);
			}
		}

		for (k = 0xB518, reg = 0; k <= 0xB51E; k++, reg++)	// eor.b d0,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 2;
				eor_areg = reg;
				return (D_B);
			}
		}
		for (k = 0xB718, reg = 0; k <= 0xB71E; k++, reg++)	// eor.b d1,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 3;
				eor_areg = reg;
				return (D_B);
			}
		}
		for (k = 0xB918, reg = 0; k <= 0xB91E; k++, reg++)	// eor.b d0,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 4;
				eor_areg = reg;
				return (D_B);
			}
		}
		for (k = 0xBB18, reg = 0; k <= 0xBB1E; k++, reg++)	// eor.b d1,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 5;
				eor_areg = reg;
				return (D_B);
			}
		}
		for (k = 0xBD18, reg = 0; k <= 0xBD1E; k++, reg++)	// eor.b d0,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 6;
				eor_areg = reg;
				return (D_B);
			}
		}
		for (k = 0xBF18, reg = 0; k <= 0xBF1E; k++, reg++)	// eor.b d0,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 7;
				eor_areg = reg;
				return (D_B);
			}
		}

		// *********************** EOR.L ***************************************
		for (k = 0xB198, reg = 0; k <= 0xB19E; k++, reg++)	// eor.l d0,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 0;
				eor_areg = reg;
				return (D_L);
			}
		}
		for (k = 0xB398, reg = 0; k <= 0xB39E; k++, reg++)	// eor.l d1,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				eor_dreg = 1;
				eor_areg = reg;
				return (D_L);
			}
		}

	}
	return (FALSE);
}

UWORD Find_address (UWORD *Buf, UWORD start)
{
	UWORD j;

	if (start < 0)
		start = 0;

	for (j = start; j < start + START_OFF; j++)
	{
		switch (Buf[j])		// LEA Address,a0 - a6
		{
			case 0x41FA: lea_reg = 0; break;
			case 0x43FA: lea_reg = 1; break;
			case 0x45FA: lea_reg = 2; break;
			case 0x47FA: lea_reg = 3; break;
			case 0x49FA: lea_reg = 4; break;
			case 0x4BFA: lea_reg = 5; break;
			case 0x4DFA: lea_reg = 6; break;
		}
		if (lea_reg != -1)
		{
			if ((lea_reg == eor_areg) OR (lea_reg == not_reg) OR  (lea_reg == and_reg))	// Samme?
			{
				UWORD address;

				address = Buf[j+1] + 2;
				address += (j*2);
				return (address);
			}
		}
	}

	return (0);
}

UWORD Find_copy_address (UWORD *Buf, UWORD start, UBYTE a_reg)
{
	UWORD j;
	BYTE cpy_reg = -1;

	if (start < 0)
		start = 0;

	for (j = start; j < start + START_OFF; j++)
	{
		switch (Buf[j])		// LEA Address,a0 - a6
		{
			case 0x41FA: cpy_reg = 0; break;
			case 0x43FA: cpy_reg = 1; break;
			case 0x45FA: cpy_reg = 2; break;
			case 0x47FA: cpy_reg = 3; break;
			case 0x49FA: cpy_reg = 4; break;
			case 0x4BFA: cpy_reg = 5; break;
			case 0x4DFA: cpy_reg = 6; break;
		}
		if (cpy_reg != -1)
		{
			if (cpy_reg == a_reg)
			{
				UWORD address;

				address = Buf[j+1] + 2;
				address += (j*2);
//				printf ("Found cpy LEA %lx(PC),a%ld\n", address, a_reg);
				return (address);
			}
		}
	}

	return (0);
}

ULONG Find_val (UWORD *Buf, UWORD start)
{
	UWORD j;
	BYTE cpy_reg = -1;
	UWORD k, k_start, k_end, reg;

	if (start < 0)
		start = 0;

	for (j = start; j < start + START_OFF; j++)	// move.l
	{
		switch (Buf[j])
		{
			case 0x203C: val_reg = 0; break;
			case 0x223C: val_reg = 1; break;
			case 0x243C: val_reg = 2; break;
			case 0x263C: val_reg = 3; break;
			case 0x283C: val_reg = 4; break;
			case 0x2A3C: val_reg = 5; break;
			case 0x2C3C: val_reg = 6; break;
			case 0x2E3C: val_reg = 7; break;
		}
		if (val_reg != -1)
		{
			if (val_reg == eor_dreg)	// Samme?
			{
				UWORD lo_bits, hi_bits, k, reg;
				BOOL flag = FALSE;
				ULONG val;
									
				hi_bits = Buf[j+1];
				lo_bits = Buf[j+2];
				val = UWordToULong (hi_bits, lo_bits);
				return (val);
			}
		}
	}

	for (j = start; j < start + START_OFF; j++)	// move.b address
	{
		switch (Buf[j])
		{
			case 0x103A: val_reg = 0; break;
			case 0x123A: val_reg = 1; break;
			case 0x143A: val_reg = 2; break;
			case 0x163A: val_reg = 3; break;
			case 0x183A: val_reg = 4; break;
			case 0x1A3A: val_reg = 5; break;
			case 0x1C3A: val_reg = 6; break;
			case 0x1E3A: val_reg = 7; break;
		}
		if (val_reg != -1)
		{
			if (val_reg == eor_dreg)	// Samme?
			{
				UWORD lo_bits, hi_bits, k, reg;
				BOOL flag = FALSE;
				UWORD val;
				WORD addr;
				
				addr = Buf[j+1] + 2;
				addr += j*2 ;
				val = Buf[addr/2] >> 8;
				return (val);
			}
		}
	}

	for (j = start; j < start + START_OFF; j++)	// moveq $x,d0-d7
	{
		switch (Buf[j]>>8)
		{
			case 0x70: val_reg = 0; break;
			case 0x72: val_reg = 1; break;
			case 0x74: val_reg = 2; break;
			case 0x76: val_reg = 3; break;
			case 0x78: val_reg = 4; break;
			case 0x7A: val_reg = 5; break;
			case 0x7C: val_reg = 6; break;
			case 0x7E: val_reg = 7; break;
		}
		if (val_reg != -1)
		{
			if (val_reg == eor_dreg)	// Samme?
			{
				ULONG val;
									
				val = ((UBYTE)Buf[j]);
				return (val);
			}
		}
	}

	val_reg = -1;
	for (j = start; j < start + START_OFF; j++)	// moveq $x,d0-d7
	{
		k_start = 0x1010;
		k_end = k_start + 7;
		for (cpy_reg = 0; cpy_reg <= 7; cpy_reg++)
		{
			for (k = k_start, reg = 0; k <= k_end; k++, reg++)	// move.b	(a0-a6),d0
			{
				if (Buf[j] == k)
				{
					val_reg = reg;
					break;
				}
			}
			if (val_reg == -1)
			{
				k_start += 0x0200;
				k_end = k_start + 7;
			}
			else
			{
				if (val_reg == eor_dreg)	// Samme?
				{
					UWORD address;
					UBYTE val;

//					printf ("val_reg = d%ld, cpy_reg = a%ld\n", val_reg, cpy_reg);
					address = Find_copy_address (Buf, start, cpy_reg);
					val = Buf[address / 2]>>8;
//					printf ("VALUE = %lx\n", val);
					
					return (val);
				}
			}
		}
	}

	return (0);
}

BOOL Find_not (UWORD *Buf, UWORD start)// 	not.b	(ax)+
{
	UWORD j, k;
	UBYTE reg;

	for (j = start; j < start + START_OFF; j++)
	{
		for (k = 0x4618, reg = 0; k <= 0x461E; k++, reg++)	// eor.b d0,(a0-a6)+
		{
			if (Buf[j] == k)
			{
				not_reg = reg;
				return (TRUE);
				
				break;
			}
		}
	}
	return (FALSE);
}

UBYTE Find_and (UWORD *Buf, UWORD start)	// and.b	#$xx(ax)+
{
	UWORD j, k;
	UBYTE reg;

	for (j = start; j < start + START_OFF; j++)
	{
		for (k = 0x0218, reg = 0; k <= 0x021E; k++, reg++)	// and.b #$xx(a0-a6)+
		{
			if (Buf[j] == k)
			{
				and_reg = reg;
				and_val = Buf[j+1];
//				and_val = Buf[j+1] >> 8;
				return (D_B);
				
				break;
			}
		}
	}
	return (FALSE);
}

UBYTE Find_sub (UWORD *Buf, UWORD start)	// sub.w	#$xxxx(ax)+
{
	UWORD j, k;
	UBYTE reg;

	for (j = start; j < start + START_OFF; j++)
	{
		for (k = 0x0458, reg = 0; k <= 0x045E; k++, reg++)	// sub.w #$xxxx(a0-a6)+
		{
			if (Buf[j] == k)
			{
				and_reg = reg;
				and_val = Buf[j+1];
				return (D_W);
				
				break;
			}
		}
	}
	return (FALSE);
}

ULONG Do_crypt_code (VOID)
{
	return (JEOSoundRequest (GLS (&L_MESSAGE), "Possible decryption code found!", "Decrypt|Skip", S_MESSAGE));
}

BOOL EncryptBB (UWORD *Buf, UWORD start, WORD unit)
{
	UWORD i;
	UWORD len = 0;
	ULONG val;
	BOOL go_flag = TRUE;
	UBYTE mode = 0;
	UWORD address;
	
//	printf ("start = %ld $%04lx\n\n", start, start);

	for (i = start; i < BUFSIZE / 2; i++)
	{
		if (!go_flag)
			break;
		switch (Buf[i])
		{
			case 0x51C8: dbra_reg = 0; break;
			case 0x51C9: dbra_reg = 1; break;
			case 0x51CA: dbra_reg = 2; break;
			case 0x51CB: dbra_reg = 3; break;
			case 0x51CC: dbra_reg = 4; break;
			case 0x51CD: dbra_reg = 5; break;
			case 0x51CE: dbra_reg = 6; break;
			case 0x51CF: dbra_reg = 7; break;
		}
		if (dbra_reg != -1)
		{
//			printf ("Found DBRA	d%ld\n", dbra_reg);
			go_flag = FALSE;
			start = i - START_OFF;
			if (start < 0)
				start = 0;

			len = Find_len (Buf, start);	// len_reg må være dbra_reg
			if (len)
			{
//				printf ("Found LEN	#$%x,d%ld\n", len, dbra_reg);

				mode = Find_eor (Buf, start);	// Found EOR
				if (mode AND (eor_dreg != dbra_reg))
				{
/*
					switch (mode)
					{
						case D_B:	printf ("Found EOR.B	d%ld,(a%ld)+\n", eor_dreg, eor_areg); break;
						case D_W:	printf ("Found EOR.W	d%ld,(a%ld)+\n", eor_dreg, eor_areg); break;
						case D_L:	printf ("Found EOR.L	d%ld,(a%ld)+\n", eor_dreg, eor_areg); break;
					}
*/
					address = Find_address (Buf, start);
					if (address)
					{
//						printf ("Found LEA	%lx(PC),a%ld\n", address, eor_areg);

						val = Find_val (Buf, start);
						if (val)
						{
//							printf ("Found value #$%x,d%ld\n", val, eor_dreg);
							if (Do_crypt_code ())
							{
								switch (mode)
								{
									case D_B:
									{
										UBYTE new;
										UBYTE *Buffer = 0;

										Buffer = (UBYTE *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("Startoff: %$lx Data: %02lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Eor_b (val, Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
									case D_W:
									{
										UWORD new;
										UWORD *Buffer = 0;

										address /= 2;
										Buffer = (UWORD *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("Startoff: %$lx Data: %04lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Eor_w (val, Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
									case D_L:
									{
										ULONG new;
										ULONG *Buffer = 0;
	
										Buffer = (ULONG *)&Buf[address / 2];
//										printf ("Alt i orden!\n\n");
//										printf ("Startoff: $%lx Data: $%08lx\n", address, Buffer[0]);
//										printf ("Len: %ld $%04lx\n", len, len);
										for (i = 0; i < len; i++)
										{
											new = Eor_l (val, Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
								}
							}
						}
					}
				}
				else if (mode = Find_not (Buf, start))	// Found NOT
				{
					if (mode)
					{
/*
						switch (mode)
						{
							case D_B:	printf ("Found NOT.B	(a%ld)+\n", not_reg); break;
							case D_W:	printf ("Found NOT.W	(a%ld)+\n", not_reg); break;
							case D_L:	printf ("Found NOT.L	(a%ld)+\n", not_reg); break;
						}
*/
						address = Find_address (Buf, start);
						if (address)
						{
//							printf ("Found LEA	%lx(PC),a%ld\n", address, not_reg);

							if (Do_crypt_code ())
							{
								switch (mode)
								{
									case D_B:
									{
										UBYTE new;
										UBYTE *Buffer = 0;

										Buffer = (UBYTE *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%02lx\n", Buffer[0]);
//										printf ("Startoff: $%lx Data: %lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Not_b (Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
									case D_W:
									{
										UWORD new;
										UWORD *Buffer = 0;

										address /= 2;
										Buffer = (UWORD *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%04lx\n", Buffer[0]);
//										printf ("Startoff: %$lx Data: %04lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Not_w (Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
									case D_L:
									{
										ULONG new;
										ULONG *Buffer = 0;
	
										address /= 4;
										Buffer = (ULONG *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%08lx\n", Buffer[0]);
//										printf ("Startoff: %$lx Data: %08lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Not_l (Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
								}
							}
						}
					}
				}
				else if (mode = Find_and (Buf, start))	// Found AND
				{
					if (mode)
					{
/*						switch (mode)
						{
							case D_B:	printf ("Found AND.B	#$%lx(a%ld)+\n", and_val, and_reg); break;
							case D_W:	printf ("Found AND.W	#$%lx(a%ld)+\n", and_val, and_reg); break;
							case D_L:	printf ("Found AND.L	#$%lx(a%ld)+\n", and_val, and_reg); break;
						}
*/
						address = Find_address (Buf, start);
						if (address)
						{
//							printf ("Found LEA	%lx(PC),a%ld\n", address, and_reg);
	
							if (Do_crypt_code ())
							{
								switch (mode)
								{
									case D_B:
									{
										UBYTE new;
										UBYTE *Buffer = 0;
										UBYTE val;
	
										val = and_val;
										Buffer = (UBYTE *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%02lx\n", Buffer[0]);
//										printf ("Startoff: $%lx Data: %02lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = And_b (val, Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
									case D_W:
									{
										UWORD new;
										UWORD *Buffer = 0;

										address /= 2;
										Buffer = (UWORD *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%04lx\n", Buffer[0]);
//										printf ("Startoff: $%lx Data: %04lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Not_w (Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
									case D_L:
									{
										ULONG new;
										ULONG *Buffer = 0;

										address /= 4;
										Buffer = (ULONG *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%08lx\n", Buffer[0]);
//										printf ("Startoff: $%lx Data: %08lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Not_l (Buffer[i]);
											Buffer[i] = new;
										}
										return (TRUE);
									}
								}
							}
						}
					}
				}
				else if (mode = Find_sub (Buf, start))	// Found AND
				{
					if (mode)
					{
/*
						switch (mode)
						{
							case D_B:	printf ("Found SUB.B	#$%lx(a%ld)+\n", and_val, and_reg); break;
							case D_W:	printf ("Found SUB.W	#$%lx(a%ld)+\n", and_val, and_reg); break;
							case D_L:	printf ("Found SUB.L	#$%lx(a%ld)+\n", and_val, and_reg); break;
						}
*/
						address = Find_address (Buf, start);
						if (address)
						{
//							printf ("Found LEA	%lx(PC),a%ld\n", address, and_reg);
	
							if (Do_crypt_code ())
							{
								switch (mode)
								{
									case D_B:
									{
										UBYTE new;
										UBYTE *Buffer = 0;
										UBYTE val;
	
										val = and_val;
										Buffer = (UBYTE *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%02lx\n", Buffer[0]);
//										printf ("Startoff: $%lx Data: %02lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Buffer[i] - val;
											Buffer[i] = new;
										}
										return (TRUE);
									}
									case D_W:
									{
										UWORD new;
										UWORD *Buffer = 0;

										address /= 2;
										Buffer = (UWORD *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%04lx\n", Buffer[0]);
//										printf ("Startoff: $%lx Data: %04lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Buffer[i] - val;
											Buffer[i] = new;
										}
										return (TRUE);
									}
									case D_L:
									{
										ULONG new;
										ULONG *Buffer = 0;

										address /= 4;
										Buffer = (ULONG *)Buf;
//										printf ("Alt i orden!\n\n");
//										printf ("%08lx\n", Buffer[0]);
//										printf ("Startoff: $%lx Data: %08lx\n", address, Buffer[address]);
//										printf ("Len: %ld %04lx\n", len, len);
										for (i = address; i < address + len; i++)
										{
											new = Buffer[i] - val;
											Buffer[i] = new;
										}
										return (TRUE);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return (FALSE);
}
