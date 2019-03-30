#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>

char VE[] = "-V:VE";

char VE_checksum[33];
ULONG start = 0xAE203642;

VOID main (VOID)
{
	BPTR fh;
	UBYTE *Buf = 0;
	int size;
	BYTE flag = 0;	// OK
	MD5Data md5;
  MD5_CTX mdContext;
  ULONG i;
  char Hold[9];
  ULONG check[6];
  char FileName[] = "-v:ve";

 	size = FileSize (FileName);
	if (size > 0)
	{
		if (Buf = AllocMem (size, MEMF_CLEAR))
		{
			if (fh = Open (FileName, MODE_OLDFILE))
			{
				ULONG *p, j;

				Read (fh, Buf, size);
				Close (fh);

				md5.md5[0] = 0;
			  MD5Init (&mdContext);

				for (i = 0; i < (size - 4); i++)
				{
					p = (ULONG *)&Buf[i];
					if (*p == start)
						i += 24;
				  MD5Update (&mdContext, (UBYTE *)p, 2);
				}
			  MD5Final (&mdContext);
				VE_checksum[0] = 0;
			  for (i = 0; i < 16; i++)
			  {
			    SPrintF (Hold, "%02lx", mdContext.digest[i]);
    			strcat (VE_checksum, Hold);
			  }

				printf ("ULONG VE_CHECKSUM[6] =\n{\n");
				printf ("  0x48e73838,\n");
				for (i = 1; i < 5; i++)
				{
					strncpy (Hold, &VE_checksum[(i - 1) * 8], 8);
					Hold[8] = 0;
					check[i] = strtoul (Hold, NULL, 16);
					printf ("  0x%08lx,\n", check[i]);
				}
				printf ("  0x4ed12f0e\n");
				printf ("};\n");
			}
			FreeMem (Buf, size);
		}
	}
}
