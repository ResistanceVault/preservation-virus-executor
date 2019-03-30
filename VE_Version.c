#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <libraries/reqtools.h>
#include "VE_ProtoTypes.h"

ULONG start_dummy = 0xAE203642;
#include "VE_CHECKSUM.h"

char ProgramVersion[20] = "2.35";
char VerString[] = "$VER: VirusExecutor 2.35 (10.01.2004)";
char VE_Checksum[33];

BYTE Check_checksum (char *FileName)
{
	BPTR fh;
	UBYTE *Buf = 0;
	int size;
	BYTE flag = 0;	// OK
	MD5Data md5;
  MD5_CTX mdContext;
  ULONG i;
  char Hold[9];
  ULONG check[4];

 	size = FileSize (FileName);
	if (size > 0)
	{
		if (Buf = AllocMem (size, MEMF_CLEAR))
		{
			if (fh = Open (FileName, MODE_OLDFILE))
			{
				ULONG *p;

				Read (fh, Buf, size);
				Close (fh);

				md5.md5[0] = 0;
			  MD5Init (&mdContext);

				for (i = 0; i < (size - 4); i++)
				{
					p = (ULONG *)&Buf[i];
					if (*p == start_dummy)
						i += 24;
				  MD5Update (&mdContext, (UBYTE *)p, 2);
				}
			  MD5Final (&mdContext);
				
				VE_Checksum[0] = 0;
			  for (i = 0; i < 16; i++)
			  {
			  	if (flag)
			  		break;
			    SPrintF (Hold, "%02lx", mdContext.digest[i]);
			    strcat (VE_Checksum, Hold);
			  }

		  	flag = FALSE;
				for (i = 0; i < 4; i++)
				{
					strncpy (Hold, &VE_Checksum[i * 8], 8);
					Hold[8] = 0;
					check[i] = strtoul (Hold, NULL, 16);
					if (check[i] != VE_CHECKSUM[i+1])
					{
						flag = TRUE;
						break;
					}
				}
			}
			FreeMem (Buf, size);
		}
	}
	else
		return (TRUE);	// ERROR

	return (flag);	// 0 = OK
}
