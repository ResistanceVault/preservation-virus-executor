#include <JEO.h>
#include <ctype.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>


MD5Data Calculate_MD5_from_file (char *FileName);

char *Dummy;
char Dest[] = "Sources:C_Sources/VE/LiveUpdate";
char MInfo[100];

char MainArc[300];
char OutPutTxt[] = "T:OutPut.txt";
BPTR fh;

MD5Data Calculate_MD5_from_file (char *FileName)
{
	BPTR fh;
	char *Buffer;
	int size;
	MD5Data md5;

	size = FileSize (FileName);
	if (size > 0)
	{
		if (Buffer = AllocMem (size, MEMF_CLEAR))
		{
			if (fh = Open (FileName, MODE_OLDFILE))
			{
				Read (fh, Buffer, size);
				Close (fh);
				md5 = Calculate_MD5 (Buffer, size);
			}
			FreeMem (Buffer, size);
		}
	}
	return (md5);
}

VOID DoLha (char *From)
{
	char Hold[300];

	sprintf (Hold, "c:lha a -q %s %s", MainArc, From);
	Execute (Hold, NULL, fh);
}

BOOL GetVersion (char *Name, char *Version)
{
	LONG size;
	BOOL ret = FALSE;
	char *Buf;
	BPTR fh;

	size = FileSize (OutPutTxt);
	if (size > 0)
	{
		if (Buf = AllocMem (size, MEMF_CLEAR))
		{
			if (fh = Open (OutPutTxt, MODE_OLDFILE))
			{
				Read (fh, Buf, size);
				Buf[size-1] = 0;
				strcpy (Version, &Buf[strlen (Name) + 1]);
				if (*Version)
					ret = TRUE;
				Close (fh);
			}
			FreeMem (Buf, size);
		}
	}
	return ret;
}

VOID GetMD5 (char *MD5, char *FileName)
{
	MD5Data md5;

	md5 = Calculate_MD5_from_file (FileName);

	strcpy (MD5, md5.md5);
}

VOID SaveInfo (char *Name, char *Buf)
{
	BPTR fh;
	char FileName[300];

	SPrintF (FileName, "%s/-%s", Dest, Name);
	if (fh = Open (FileName, MODE_NEWFILE))
	{
		printf ("Saving info '%s'\n%s\n", Name, Buf);
		Write (fh, Buf, strlen (Buf));
		Close (fh);
	}
}

VOID DoStuff (char *Name, char *VersionName)
{
	BPTR fh;
	char MD5[33];
	char Version[100], Hold[100];
	LONG size;

  if (fh = Open (OutPutTxt, MODE_NEWFILE))
  {
		sprintf (Dummy, "Version FILE %s", VersionName);
		Execute (Dummy, NULL, fh);
		Close (fh);

		if (!(strnicmp (Name, "xad", 3)))
		{
			strcpy (Hold, "xadmaster.library");
			GetVersion (Hold, Version);
		}
		else if (!(strnicmp (Name, "Cat", 3)))
		{
			strcpy (Hold, "VirusExecutor.catalog");
			GetVersion (Hold, Version);
		}
		else
			GetVersion (Name, Version);
		size = FileSize (MainArc);
		GetMD5 (MD5, MainArc);
		sprintf (Dummy, "%ld\n%s\n%s\n", size, Version, MD5);
		SaveInfo (Name, Dummy);
	}
}

main ()
{
	char VersionName[300];
	ULONG i, cpu;
	char CatalogName[30];

	Dummy = AllocMem (10000, MEMF_CLEAR);

	printf ("\n");
// ***************************************************************************
// *************************** VirusExecutor *********************************
// ***************************************************************************
  if (fh = Open (OutPutTxt, MODE_NEWFILE))
  {
  	BOOL go_flag = TRUE;

		SPrintF (MainArc, "%s/VirusExecutor.lha", Dest);
		if (Exists (MainArc))
		{
			if (!DeleteFile (MainArc))
				go_flag = FALSE;
		}
		if (go_flag)
		{
			SPrintF (VersionName, "V:VirusExecutor");
			DoLha (VersionName);
			DoLha ("V:VirusExecutor.readme");
			DoLha ("V:VirusExecutor.guide");
			Close (fh);

			DoStuff ("VirusExecutor", VersionName);
		}
		else
			Close (fh);
	}

// ***************************************************************************
// ************************** LOCALE: ****************************************
// ***************************************************************************
	for (i = 0; i < 14; i++)
	{
		switch (i)
		{
			case  0: strcpy (CatalogName, "czech"); break;
			case  1: strcpy (CatalogName, "norsk"); break;
			case  2: strcpy (CatalogName, "nederlands"); break;
			case  3: strcpy (CatalogName, "español"); break;
			case  4: strcpy (CatalogName, "italiano"); break;
			case  5: strcpy (CatalogName, "polski"); break;
			case  6: strcpy (CatalogName, "magyar"); break;
			case  7: strcpy (CatalogName, "slovak"); break;
			case  8: strcpy (CatalogName, "português"); break;
			case  9: strcpy (CatalogName, "français"); break;
			case 10: strcpy (CatalogName, "russian"); break;
			case 11: strcpy (CatalogName, "deutsch"); break;
			case 12: strcpy (CatalogName, "dansk"); break;
			case 13: strcpy (CatalogName, "svenska"); break;
		}

	  if (fh = Open (OutPutTxt, MODE_NEWFILE))
  	{
  		BOOL go_flag = TRUE;

			SPrintF (MainArc, "%s/Catalog_%s.lha", Dest, CatalogName);
			if (Exists (MainArc))
			{
				if (!DeleteFile (MainArc))
					go_flag = FALSE;
			}
			if (go_flag)
			{
				char Name[30];

				SPrintF (VersionName, "CAT_%s:VirusExecutor.catalog", CatalogName);
				DoLha (VersionName);
				Close (fh);

				SPrintF (Name, "Catalog_%s", CatalogName);
				DoStuff (Name, VersionName);
			}
			else
				Close (fh);
		}
	}


// ***************************************************************************
// ****************************** PatchBrain *********************************
// ***************************************************************************
  if (fh = Open (OutPutTxt, MODE_NEWFILE))
  {
  	BOOL go_flag = TRUE;

		SPrintF (MainArc, "%s/PatchBrain.lha", Dest);
		if (Exists (MainArc))
		{
			if (!DeleteFile (MainArc))
				go_flag = FALSE;
		}
		if (go_flag)
		{
			SPrintF (VersionName, "V:VirusExecutor.PatchBrain");
			DoLha (VersionName);
			DoLha ("V:VEPatchBrain.readme");
			Close (fh);

			DoStuff ("PatchBrain", VersionName);
		}
		else
			Close (fh);
	}
// ***************************************************************************
// ***************************** xvs.library *********************************
// ***************************************************************************
  if (fh = Open (OutPutTxt, MODE_NEWFILE))
  {
  	BOOL go_flag = TRUE;

		SPrintF (MainArc, "%s/xvslibrary.lha", Dest);
		if (Exists (MainArc))
		{
			if (!DeleteFile (MainArc))
				go_flag = FALSE;
		}
		if (go_flag)
		{
			SPrintF (VersionName, "NF:xvs.library");
			DoLha (VersionName);
			DoLha ("NF:xvslibrary.readme");
			DeleteFile (OutPutTxt);
			Close (fh);

			DoStuff ("xvs.library", VersionName);
		}
		else
			Close (fh);
	}

// ***************************************************************************
// ************************** xfdmaster.library ******************************
// ***************************************************************************
  if (fh = Open (OutPutTxt, MODE_NEWFILE))
  {
  	BOOL go_flag = TRUE;

		SPrintF (MainArc, "%s/xfdmaster.lha", Dest);
		if (Exists (MainArc))
		{
			if (!DeleteFile (MainArc))
				go_flag = FALSE;
		}
		if (go_flag)
		{
			SPrintF (VersionName, "NF:xfdmaster.library");
			DoLha (VersionName);
			DoLha ("NF:xfdmaster.readme");
			DoLha ("NF:xfd");
			Close (fh);

			DoStuff ("xfdmaster.library", VersionName);
		}
		else
			Close (fh);
	}

// ***************************************************************************
// ************************** xadmasterxxx.library ***************************
// ***************************************************************************

	for (i = 0; i < 3; i++)
	{
		switch (i)
		{
			case 0: cpu = 0; break;
			case 1: cpu = 20; break;
			case 2: cpu = 60; break;
		}

	  if (fh = Open (OutPutTxt, MODE_NEWFILE))
	  {
	  	BOOL go_flag = TRUE;
			char Hold[50];

			SPrintF (MainArc, "%s/xadmaster%03ld.lha", Dest, cpu);
			if (Exists (MainArc))
			{
				if (!DeleteFile (MainArc))
					go_flag = FALSE;
			}
			if (go_flag)
			{
				SPrintF (VersionName, "XAD%03ld:xadmaster.library", cpu);
				DoLha (VersionName);
				SPrintF (Dummy, "XAD%03ld:xadmaster%03ld.readme", cpu, cpu);
				DoLha (Dummy);
				SPrintF (Dummy, "XAD%03ld:xad", cpu);
				DoLha (Dummy);
				Close (fh);

				SPrintF (Hold, "xadmaster%03ld.library", cpu);
				DoStuff (Hold, VersionName);
			}
			else
				Close (fh);
		}
	}

	FreeMem (Dummy, 10000);
}
