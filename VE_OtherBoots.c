#include <JEO:JEO.h>
#include <exec/memory.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include "VE_Prototypes.h"

OtherBoots *otherBoots[100];
UWORD otherBootLen;

BOOL CheckIt (UWORD start, UBYTE *String, UWORD unit)
{
  LONG i, j, len;
  BOOL flag = TRUE;

  len = strlen (String);
  for (i = start, j = 0; i < start + len; i++, j++)
  {
    if (Buffer[unit][i] != String[j])
    {
      flag = FALSE;
      break;
    }
  }
  return (flag);
}

BOOL Insert_boot (ULONG nr, ULONG type, ULONG checkSum, UWORD offset, UBYTE *Data)
{
	if ((otherBoots[nr] = AllocMem (sizeof (OtherBoots), MEMF_CLEAR)))
	{
		otherBoots[nr]->type = type;
		otherBoots[nr]->checkSum = checkSum;
		otherBoots[nr]->offset = offset;
		strncpy (otherBoots[nr]->Data, Data, 4);
		return (TRUE);
	}
	return (FALSE);	// No memory
}

VOID OpenOtherBoots (VOID)
{
  ULONG i = 0;

	Insert_boot (i++, UTILITY, 0x0f0fde06, 775, "1.0 ");
	Insert_boot (i++, VIRUSKILLER, 0x07db6fcb, 775, "1.0a");
  Insert_boot (i++, UTILITY, 0xa53adb32, 786, "UTIL");
  Insert_boot (i++, UTILITY, 0x4f0e9091, 982, "SLIP");
  Insert_boot (i++, UTILITY, 0x5b4ac95e, 657, "V3.2");
  Insert_boot (i++, UTILITY, 0xf3310df2, 911, ".CK2");
  Insert_boot (i++, UTILITY, 0x5b2f2e2f, 818, "v1.1");
  Insert_boot (i++, UTILITY, 0x8fd1b654, 662, "1.30");
  Insert_boot (i++, VIRUSKILLER, 0x0ad49df2, 766, "1.0a");
  Insert_boot (i++, UTILITY, 0x335c4763, 380, "PAL ");
  Insert_boot (i++, VIRUSKILLER, 0x6fa25de2, 177, "!!! ");
  Insert_boot (i++, UTILITY, 0xae13b266, 863, "1.05");
  Insert_boot (i++, UTILITY, 0xcec08b6f, 853, "v1.0");
  Insert_boot (i++, VIRUSKILLER, 0xf5e7a890, 93, "1.01");
  Insert_boot (i++, VIRUSKILLER, 0x05c2aeee, 105, "2.01");
  Insert_boot (i++, VIRUSKILLER, 0x95bdc7c6, 173, "!! -");
  Insert_boot (i++, VIRUSKILLER, 0x37fcbb02, 434, "V2.0");
  Insert_boot (i++, VIRUSKILLER, 0x2ee36b4f, 663, "V1.0");
  Insert_boot (i++, UTILITY, 0x04567e55, 859, "60 s");
  Insert_boot (i++, UTILITY, 0x4e4a68e6, 490, "V2.0");
  Insert_boot (i++, UTILITY, 0x6dc23bd2, 830, "3.0 ");
  Insert_boot (i++, UTILITY, 0x1b15cc44, 265, "Hold");
  Insert_boot (i++, UTILITY, 0xfc7adff1, 846, "1.0 ");
  Insert_boot (i++, VIRUSKILLER, 0x7d3e2bf6, 615, "1989");
  Insert_boot (i++, UTILITY, 0x5a82b114, 358, "driv");
  Insert_boot (i++, VIRUSKILLER, 0x45327604, 747, "V1.2");
  Insert_boot (i++, VIRUSKILLER, 0xf5ad4f65, 100, "V1.0");
  Insert_boot (i++, UTILITY, 0x5be1df1d, 534, "2.0 ");
  Insert_boot (i++, VIRUSKILLER, 0xd01c6ebd, 437, " V4a");
  Insert_boot (i++, VIRUSKILLER, 0x26cdf3fc, 179, "SUSP");
  Insert_boot (i++, VIRUSKILLER, 0xe9a0f3f8, 792, "1.0 ");
  Insert_boot (i++, UTILITY, 0x13a6d1dc, 219, "Rese");
  Insert_boot (i++, UTILITY, 0xb9340686, 748, "ANAR");
  otherBootLen = i;
}

VOID CloseOtherBoots (VOID)
{
  UWORD i;

  for (i = 0; i < otherBootLen; i++)
  {
  	if (otherBoots)
		{
			FreeMem (otherBoots[i], sizeof (OtherBoots));
			otherBoots[i] = 0;
	 	}
	}
}

BOOL CheckNotBoot (UWORD u, UBYTE *Messy)
{
	ULONG version;
	UWORD ver, rev;
	char Hold[5];

  if (CheckIt (0, "KICK", u))	// A kickstart disk?
	{
		CopyMem (&Buffer[u][524], &version, 4);
		ver = version>>16; 
		rev = (UWORD)version;

		if (version == 0xFFFFFFFF)	// Kick 1.1?
		{
			CopyMem (&Buffer[u][528], &version, 4);
			ver = version>>16; 
			rev = (UWORD)version;
		}
		switch (ver)
		{
			case 30: mystrcpy (Hold, "1.0"); break;
			case 31: mystrcpy (Hold, "1.1"); break;
			case 33: mystrcpy (Hold, "1.2"); break;
			case 34: mystrcpy (Hold, "1.3"); break;
			case 36: mystrcpy (Hold, "1.4"); break;
			default: mystrcpy (Hold, "?.?"); break;
		}

		SPrintF (Dummy, "Kickstart v%s (%ld.%ld)", Hold, ver, rev);

		strcpy (Messy, Dummy);
		return (TRUE);
	}
	return (FALSE);
}

ULONG CheckOtherBoots (ULONG *Buffer)
{
	UWORD i, off;
	UBYTE *Data;

/*

  Data = (UBYTE *)Buffer;

	for (i = 0; i < otherBootLen; i++)
	{
		if (otherBoots[i]->checkSum == Buffer[1])
		{
		  off = otherBoots[i]->offset;
			if (!(strncmp (&Data[off], otherBoots[i]->Data, 4)))
				return (otherBoots[i]->type);
		}
	}
*/
	return (FALSE);
}
