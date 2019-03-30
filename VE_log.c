#include <JEO:JEO.h>
#include <proto/dos.h>
#include "VE_Prototypes.h"

BPTR log_fh = 0;

char LogFileName[300];

VOID OpenLog (VOID)
{
	if (!log_fh)
	{
		char Hold[128];

		strcpy (Hold, prefs->Log_path);
		if ((Hold[strlen (Hold) - 1] != ':') AND (Hold[strlen (Hold) - 1] != '/'))
			strcat (Hold, "/");

		SPrintF (LogFileName, "%s%s.log", Hold, VE);
		log_fh = Open (LogFileName, MODE_READWRITE);
	}
}

VOID CloseLog (VOID)
{
	if (log_fh)
	{
		Close (log_fh);
		log_fh = 0;
	}
}

VOID WriteToLog (char *Text, UWORD mode)
{
	char Line[80];
	char Hold[500];
	char Date[20], Time[20];
	static BOOL line_flag = FALSE;
	LONG log_size;

	if (prefs->logging AND log_fh)	// Alt setup OK?
	{
		UBYTE i;

		for (i = 0; i < 75; i++)
			Line[i] = '-';
		Line[i] = 0;

		switch (mode)
		{
			case LOG_END:
			{
				Hold[0] = '\n';
				for (i = 1; i < 75; i++)
					Hold[i] = '*';
				Hold[i++] = '\n';
				Hold[i++] = '\n';
				Hold[i] = 0;
				
				line_flag = TRUE;
				break;
			}
			case LOG_DATE:
			{
				GetDateTime (Date, Time, 1);
				SPrintF (Hold, " %s at %s\n %s\n", Date, Time, Text);
				line_flag = FALSE;
				break;
			}
			case LOG_TEXT:
			{
				SPrintF (Hold, "%s\n", Text);
				line_flag = FALSE;
				break;
			}
			case LOG_TEXT_LINE:
			{
				SPrintF (Hold, "%s\n%s\n", Text, Line);
				line_flag = TRUE;
				break;
			}
			case LOG_LINE_TEXT:
			{
				if (!line_flag)
				{
					SPrintF (Hold, "%s\n%s\n", Line, Text);
					line_flag = FALSE;
				}
				else
					SPrintF (Hold, "%s\n", Text);
				break;
			}
			case LOG_LINE_TEXT_LINE:
			{
				if (!line_flag)
				{
					SPrintF (Hold, "%s\n%s\n%s\n", Line, Text, Line);
					line_flag = TRUE;
				}
				else
				{
					SPrintF (Hold, "%s\n%s\n", Text, Line);
					line_flag = TRUE;
				}
				break;
			}
		}
		log_size = FileSize (LogFileName);
		if (log_size > 0)
			Seek (log_fh, 0, OFFSET_END);

		Write (log_fh, &Hold, strlen (Hold));
	}
}
