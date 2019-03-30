;/*
Delete VE_File_sizes.o quiet
sc5 -j73 -v -y -q5e VE_File_sizes
quit
*/

#include <JEO:JEO.h>
#include <proto/dos.h>
//#include <proto/intuition.h>
//#include <proto/graphics.h>
//#include <proto/gadtools.h>
#include <exec/memory.h>
//#include <libraries/reqtools.h>
//#include <proto/reqtools.h>
//#include <exec/lists.h>
#include <-V:VE_Prototypes.h>
//#include <-V:VE_Window.h>
//#include <ctype.h>

File_sizes *fs[1000];
ULONG fs_count;

#define MEGA (i * sizeof (File_sizes) - ((b + (c * 3)) ^ 7) / (2 + (sizeof (File_sizes) + 1) * 2) + (d * 4))

VOID FS_crypt (UBYTE *Buffer, char *Pass)
{
  ULONG i, a, b, d , e;
  ULONG c = 55;

	Status ("Crypting...");

	for (i = 0; Pass[i] != 0; i++)
		c += (Pass[i] * 55);
	
  a = b = d = e = 1;
  for (i = 0; i < sizeof (File_sizes); i++)
  {
    Buffer[i] = Buffer[i] + MEGA;
    a = b = d = e += 1;
    if (a == 9)
      a = 0;
    if (b == 20)
      b = 0;
    if (c == 4)
      c = 0;
    if (d == 3)
      d = 0;
    if (e == 8)
      e = 0;
  }
}

VOID FS_decrypt (UBYTE *Buffer, char *Pass)
{
  ULONG i, a, b, d , e;
  ULONG c = 55;

	Status ("Decrypting...");

	for (i = 0; Pass[i] != 0; i++)
		c += (Pass[i] * 55);

  a = b = c = d = e = 1;
  for (i = 0; i < sizeof (File_sizes); i++)
  {
    Buffer[i] = Buffer[i] - MEGA;
    a = b = c = d = e += 1;
    if (a == 9)
      a = 0;
    if (b == 20)
      b = 0;
    if (c == 4)
      c = 0;
    if (d == 3)
      d = 0;
    if (e == 8)
      e = 0;
  }
}

BOOL Save_fs_file (char *FileName)
{
  struct FileHandle *fh = 0;
	BOOL ret = TRUE;
	char *Buf = 0;
	char *MegaBuffer = 0;
	int i;

	Status (" Saving file...");

	if (Buf = AllocMem (sizeof (File_sizes), MEMF_CLEAR))
	{
	  if ((fh = (struct FileHandle *)Open (FileName, MODE_NEWFILE)) != NULL)
		{
			for (i = 0; i < fs_count; i++)
			{
				CopyMem (fs[i], Buf, sizeof (File_sizes));
				FS_crypt (Buf, "Hebbe");
 			  Write ((BPTR)fh, Buf, sizeof (File_sizes));
 			}
			Close ((BPTR)fh);
			fh = 0;
		}
	}

	if (Buf)
	{
		FreeMem (Buf, sizeof (File_sizes));
		Buf = 0;
	}
	return (ret);
}

BOOL Update_file_sizes (VOID)
{
  BOOL success = 0;
  BPTR lock = 0;
  struct FileInfoBlock *fileinfoblock = 0;
  char Dir[] = "C:";
  BOOL ret = TRUE;

  fileinfoblock = (struct FileInfoBlock *) AllocMem (sizeof (struct FileInfoBlock), MEMF_CLEAR);
 
  if (!(lock = Lock (Dir, ACCESS_READ)))	// Kunne ikke locke
  {
    ret = FALSE;
    goto error;
  }
  if ((success = Examine (lock, fileinfoblock)) != ERROR) // Kunne ikke examinere
  {
    ret = FALSE;
    goto error;
  }

  Status ("Updating file sizes from 'C:'");
	fs_count = 0;
  for (;;)
  {
		success = ExNext (lock, fileinfoblock);	// FindNext File/Dir
    if (!(success))				// End of directory?
      break;
    if (fileinfoblock->fib_DirEntryType < 0)	// < 0 = a file
    {
    	strcpy (fs[fs_count]->Name, fileinfoblock->fib_FileName);
    	fs[fs_count]->size = fileinfoblock->fib_Size;
      fs_count++;
    }
  }

error:
  if (lock)
    UnLock (lock);
  if (fileinfoblock)
    FreeMem (fileinfoblock, sizeof (struct FileInfoBlock));

  return (ret);
}

VOID Check_file_sizes (VOID)
{
	
}

VOID Do_file_sizes (BOOL mode)
{
	int i;

	for (i = 0; i < 1000; i++)
		fs[i] = AllocMem (sizeof (File_sizes), MEMF_CLEAR);
 
	if (mode)
	{
		if (Update_file_sizes ())
		{
			if (Save_fs_file ("RAM:TEST"))
				Status (Ready);
		}
	}
	else
		Check_file_sizes ();

	for (i = 0; i < 1000; i++)
		FreeMem (fs[i], sizeof (File_sizes));
}
