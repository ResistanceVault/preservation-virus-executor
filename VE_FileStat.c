#include <JEO:JEO.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <exec/memory.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include "VE_Prototypes.h"

BOOL filestat;
extern char Archive[];
extern char ArcFile[];
extern char FileTime[];

VOID StripFile (char *FullName, char *FileName)
{
	WORD i, len;

	if (len = strlen (FullName))
	{
		for (i = len - 1; i >= 0; i--)
		{
			if (FullName[i] == '/' OR FullName[i] == ':')
				break;
		}

		if (++i > 0)
		{
			CopyMem (&FullName[i], FileName, len - i);
			FileName[len-i] = 0;
		}
		else
			strcpy (FileName, FullName);
	}
}

#define Y1	240
#define Y2	250
#define Y3	260
#define Y4	270

#define X1	 10
#define X2	214
#define X3	418
#define NEXT	180

VOID ClearFileStatus (BOOL mode)
{
	if ((!prefs->fast_file_checking) AND filestat)
	{
		SetAPen (VirusWnd->RPort, 0);
		RectFill (VirusWnd->RPort, mainX+3, 200 + OFFY, mainX + 628, 276 + OFFY);
		if (mode)	// Vise alt
		{
			if (!prefs->fast_file_checking AND filestat)
			{
				StyleIt2 (rp, mainX+3, 200 + OFFY , mainX + 628, 226 + OFFY, BLACK, WHITE, GADGET_STYLE);
				StyleIt2 (rp, mainX+3, 230 + OFFY , mainX + 628, 276 + OFFY, BLACK, WHITE, GADGET_STYLE);
			}
			JEOWrite (rp, mainX + X1, Y1 + OFFY, GLS (&L_DIRECTORIES), BLACK);
			JEOWrite (rp, mainX + X1, Y2 + OFFY, GLS (&L_CRUNCHED), BLACK);
			JEOWrite (rp, mainX + X1, Y3 + OFFY, GLS (&L_ARCHIVED), BLACK);
			JEOWrite (rp, mainX + X1, Y4 + OFFY, GLS (&L_VIRUS_FOUND), BLACK);

			JEOWrite (rp, mainX + X2, Y1 + OFFY, GLS (&L_FILES_1), BLACK);
			JEOWrite (rp, mainX + X2, Y2 + OFFY, GLS (&L_DECRUNCHED), BLACK);
			JEOWrite (rp, mainX + X2, Y3 + OFFY, GLS (&L_EXTRACTED), BLACK);
			JEOWrite (rp, mainX + X2, Y4 + OFFY, GLS (&L_TIME), BLACK);

			JEOWrite (rp, mainX + X3, Y1 + OFFY, GLS (&L_CHECKED), BLACK);
			JEOWrite (rp, mainX + X3, Y3 + OFFY, "CHIP:", BLACK);
			JEOWrite (rp, mainX + X3, Y4 + OFFY, "FAST:", BLACK);
			FileStat ("");
		}
	}
}

VOID FileStat (char *FullName)
{
	char Hold[5];
	ULONG timeEnd = 0;
	static BOOL check_flag1 = FALSE;
	static BOOL check_flag2 = FALSE;
	BOOL go_flag;

	if ((!prefs->fast_file_checking) AND filestat)
	{
		UBYTE nr1, nr2, nr3;
		static char FileName[500] = "";
		char S[77];
		UWORD y = 210;
		ULONG timeStart2;

		CurrentTime (&timeEnd, &msecs);
		timeStart2 = timeStart;
		timeEnd -= timeStart2;
		timeStart2 = timeEnd / 60;
		timeEnd %= 60;
		msecs = timeStart2 / 60;
		timeStart2 %= 60;

		SPrintF (FileTime, "%2ld:%02ld:%02ld", msecs, timeStart2, timeEnd);
		if (*FullName)	// Skal vi oppdatere navnet??
		{
			if (strcmp (FullName, FileName))	// Not the same?
			{
				StripFile (FullName, FileName);
				if (pack_checking)
					SPrintF (Dummy, "(%s) %s", Archive, ArcFile);
				else
					strcpy (Dummy, FileName);

				if (strlen (Dummy) >= 76)
				{
					strncpy (S, Dummy, 76);
					S[74] = '>';
					S[75] = '>';
					S[76] = 0;
				}
				else
					JEOCopy (S, Dummy, 76, ' ');

				JEOWrite (rp, mainX + 10, y + OFFY, S, BLACK);
			}
		}

		y += 10;
		JEOCopy (S, CrunchName, 50, ' ');
		JEOWrite (rp, mainX + 10, y + OFFY, S, BLACK);

		nr1 = ConvertNumber (Tmp1, dirCount);
		nr2 = ConvertNumber (Tmp2, fileCount);
		nr3 = ConvertNumber (Tmp3, exeFiles + dataFiles);
		JEOWrite (rp, mainX + X1 + NEXT - (nr1 * 8), Y1 + OFFY, Tmp1, BLACK);	// Directories:
		JEOWrite (rp, mainX + X2 + NEXT - (nr2 * 8), Y1 + OFFY, Tmp2, BLACK);	// Files:
		JEOWrite (rp, mainX + X3 + NEXT - (nr3 * 8), Y1 + OFFY, Tmp3, BLACK);	// Checked:

		nr1 = ConvertNumber (Tmp1, cFound);
		nr2 = ConvertNumber (Tmp2, cFiles);
		JEOWrite (rp, mainX + X1 + NEXT - (nr1 * 8), Y2 + OFFY, Tmp1, BLACK);	// Crunched:
		JEOWrite (rp, mainX + X2 + NEXT - (nr2 * 8), Y2 + OFFY, Tmp2, BLACK);	// Decruched:

		nr1 = ConvertNumber (Tmp1, archived);
		nr2 = ConvertNumber (Tmp2, archiveCount);
		nr3 = ConvertNumber (Tmp3, AvailMem (MEMF_CHIP));
		JEOWrite (rp, mainX + X1 + NEXT - (nr1 * 8), Y3 + OFFY, Tmp1, BLACK);	// Archived:
		JEOWrite (rp, mainX + X2 + NEXT - (nr2 * 8), Y3 + OFFY, Tmp2, BLACK);	// Extracted:
		JEOWrite (rp, mainX + X3 + NEXT - (nr3 * 8), Y3 + OFFY, Tmp3, BLACK); // CHIP:

		nr1 = ConvertNumber (Tmp1, virusCount);
		nr2 = ConvertNumber (Tmp2, AvailMem (MEMF_FAST));
		JEOWrite (rp, mainX + X1 + NEXT - (nr1 * 8), Y4 + OFFY, Tmp1, BLACK);	// Virus found:
		JEOWrite (rp, mainX + X2 + NEXT - (strlen (FileTime) * 8), Y4 + OFFY, FileTime, BLACK);
		JEOWrite (rp, mainX + X3 + NEXT - (nr2 * 8), Y4 + OFFY, Tmp2, BLACK);	// FAST:
	}

	SPrintF (Hold, "%02ld", timeEnd);
	go_flag = FALSE;
	if (Hold[1] == '0' AND (!check_flag1))
	{
		check_flag1 = TRUE;
		check_flag2 = FALSE;
		go_flag = TRUE;
	}
	else if (Hold[1] == '5' AND (!check_flag2))
	{
		check_flag2 = TRUE;
		check_flag1 = FALSE;
		go_flag = TRUE;
	}

	if (go_flag)
		TimerCheck ();
}
