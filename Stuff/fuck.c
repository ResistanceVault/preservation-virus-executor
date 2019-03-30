#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <proto/xfdmaster.h>

struct xfdMasterBase *xfdMasterBase	= 0;

main ()
{
	char *Buffer;
	int size;
	BPTR fh;
	char FileName[] = "RAM:1"; 
	size = 2704;

	xfdMasterBase = (struct xfdMasterBase *) OpenLibrary (XFDM_NAME, NULL);
	Buffer = AllocMem (size, MEMF_PUBLIC);

	if (fh = Open (FileName, MODE_OLDFILE))
	{
		struct xfdBufferInfo *bufferinfo = 0;

		Read (fh, Buffer, size);
		Close (fh);

		if (bufferinfo = xfdAllocObject (XFDOBJ_BUFFERINFO))
		{
			bufferinfo->xfdbi_SourceBuffer = Buffer;
			bufferinfo->xfdbi_SourceBufLen = size;
	    bufferinfo->xfdbi_Flags = XFDFF_RECOGEXTERN;

			if (xfdRecogBuffer (bufferinfo))	 // Crunched?
			{
				printf ("%s -> %s\n", FileName, bufferinfo->xfdbi_PackerName);

				bufferinfo->xfdbi_TargetBufMemType = MEMF_ANY;
				if (xfdDecrunchBuffer (bufferinfo))		// DECRUNCHER!!!
				{
					printf ("Decruncha %ld\n", bufferinfo->xfdbi_TargetBufLen);

					if (fh = Open ("RAM:TEST", MODE_NEWFILE))
					{
						Write (fh, bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufLen);
						Close (fh);
					}
					FreeMem (bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufLen);
				}
				else
					printf ("ERROR\n");
			}
			xfdFreeObject (bufferinfo);
		}
	}
	FreeMem (Buffer, size);
	CloseLibrary((struct Library *)xfdMasterBase);
}
