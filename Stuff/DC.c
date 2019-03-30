#include <JEO:JEO.h>
#include <proto/dos.h>
#include <exec/memory.h>
#include <proto/xfdmaster.h>

struct xfdMasterBase *xfdMasterBase	= 0;
char CrunchName[100] = "";
struct xfdBufferInfo *bufferinfo = 0;

VOID CleanUp (VOID)
{
	if (bufferinfo)
		xfdFreeObject (bufferinfo);
	if (xfdMasterBase)
		CloseLibrary((struct Library *)xfdMasterBase);
}

ULONG *result;

ULONG StripHunkFile (APTR Buffer, ULONG size)
{
	UWORD error;

	error = xfdStripHunks (Buffer, size, result, XFDSHF_NAME);
	if (error == XFDERR_OK)
		size = *result;
	else
		printf ("Error!\n");

	return (size);
}

BOOL Decrunch_buffer (char *Name, APTR Buffer, long size)
{
	BOOL flag = FALSE;	// TRUE if decrunched
	UWORD buflen;

	printf ("%ld\n", size);
	size = StripHunkFile (Buffer, size);
	printf ("%ld\n", size);

	CrunchName[0] = 0;
	bufferinfo->xfdbi_SourceBuffer = Buffer;
	bufferinfo->xfdbi_SourceBufLen = size;
	bufferinfo->xfdbi_Flags = 0;
	if (xfdRecogBuffer (bufferinfo))	// Er fila cruncha?
	{
		BOOL crunch_flag = TRUE;

		if (bufferinfo->xfdbi_PackerFlags & XFDPFF_ADDR)	// Vil ikke decrunche
			crunch_flag = FALSE;

	  printf ("Name: %s\nType is '%s'\n", Name, bufferinfo->xfdbi_PackerName);
		if (bufferinfo->xfdbi_PackerFlags & XFDPFF_RELOC)
			printf ("RELOC\n");
		if (bufferinfo->xfdbi_PackerFlags & XFDPFF_ADDR)
			printf ("ADDR\n");
		if (bufferinfo->xfdbi_PackerFlags & XFDPFF_DATA)
			printf ("DATA\n");
		if (bufferinfo->xfdbi_PackerFlags & XFDPFF_PASSWORD)
			printf ("PASSWORD\n");
		if (bufferinfo->xfdbi_PackerFlags & XFDPFF_RELMODE)
			printf ("RELMODE\n");
		if (bufferinfo->xfdbi_PackerFlags & XFDPFF_KEY16)
			printf ("KEY16\n");
		if (bufferinfo->xfdbi_PackerFlags & XFDPFF_KEY32)
			printf ("KEY32\n");

		if (crunch_flag)
		{
			strcpy (CrunchName, bufferinfo->xfdbi_PackerName);
			printf ("%s (%s) %ld\n", Name, CrunchName, size);

			bufferinfo->xfdbi_TargetBufMemType = MEMF_ANY;
			if (xfdDecrunchBuffer (bufferinfo))		// DECRUNCHER!!!
			{
//				FreeMem (bufferinfo->xfdbi_SourceBuffer, bufferinfo->xfdbi_SourceBufLen);
				flag = TRUE;
			}
			else
				printf ("Error decrunching\n\n");
			
		}
	}
	return (flag);
}

main ()
{
	BPTR fh;
	UBYTE *Buffer = 0;
	LONG size;

	xfdMasterBase = (struct xfdMasterBase *) OpenLibrary (XFDM_NAME, NULL);
	bufferinfo = xfdAllocObject (XFDOBJ_BUFFERINFO);

	result = AllocMem (4, MEMF_CLEAR);

	size = FileSize ("wd6:test1/1");
	if (size > 0)
	{
		if (Buffer = AllocMem (size, MEMF_PUBLIC))
		{
			if (fh = Open ("wd6:test1/1", MODE_OLDFILE))
			{
				printf ("Loading...\n");
				Read (fh, Buffer, size);
				Close (fh);
				if (Decrunch_buffer ("wd6:test1/1", Buffer, size))
					FreeMem (bufferinfo->xfdbi_TargetBuffer, bufferinfo->xfdbi_TargetBufLen);
			}
			FreeMem (Buffer, size);
		}
	}
	FreeMem (result, 4);

	CleanUp ();	
}
