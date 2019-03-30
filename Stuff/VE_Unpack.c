;/*
sc5 -j73 -v -y VE_Unpack
quit
*/

#include <JEO:JEO.h>
#include <proto/dos.h>
#include <-V:VE_Prototypes.h>

UWORD Check_cruncher (UBYTE *FileName)
{
  UWORD len, i, j;

  len = strlen (FileName);
  if (len < 4)
    return (FALSE);

  for (i = len - 4, j = 0; i < len; i++, j++)
    Dummy[j] = FileName[i];

  Dummy[j] = 0;
  if (!(stricmp (Dummy, ".LHA")))
    return (PACK_LHA);
  else if (!(stricmp (Dummy, ".LZH")))
    return (PACK_LHA);
  else if (!(stricmp (Dummy, ".LZX")))
    return (PACK_LZX);
  else if (!(stricmp (Dummy, ".ARC")))
    return (PACK_ARC);
  else if (!(stricmp (Dummy, ".ZIP")))
    return (PACK_ZIP);

	return (FALSE);
}

VOID test ()
{}


VOID UnpackFile (UBYTE *Temp, UBYTE *FileName)
{
	BPTR lock = 0;
	UWORD pack;
	UBYTE Pack[5];

	pack = Check_cruncher (FileName);

	if (pack)
	{
  	lock = CreateDir (Temp);
	  if (lock)
  	  UnLock (lock);
		switch (pack)
		{
			case PACK_LHA: strcpy (Pack, "LHA"); break;
			case PACK_LZX: strcpy (Pack, "LZX"); break;
			case PACK_ARC: strcpy (Pack, "ARC"); break;
			case PACK_ZIP: strcpy (Pack, "ZIP"); break;
		}
		sprintf (Dummy, "C:LHA x -q \"%s\" \"%s\"", FileName, Temp);
		printf ("Utpakker: '%s'...\n", Dummy);
		Execute (Dummy, 0, 0);
		CheckFiles (Temp);
  }
}
