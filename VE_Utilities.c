#include <JEO:JEO.h>
#include <ctype.h>
#include <exec/memory.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include "VE_Prototypes.h"
#include <JEO:raw.h>

BOOL VE_GetFileName (UBYTE *Dir, UBYTE *Name, UBYTE *Message, UBYTE *OkText)
{
  struct rtFileRequester *filereq;

  if (filereq = rtAllocRequestA (RT_FILEREQ, NULL))
  {
		strcpy (filereq->Dir, Dir);
    if (rtFileRequest (filereq, Name, Message, RT_ReqPos, REQPOS_CENTERSCR,
                                               RT_TextAttr, &topaz8,
                                               RTFI_OkText, OkText, TAG_END))
    {
      if (*filereq->Dir)
      {
        strcpy (Dir, filereq->Dir);
        if (Dir[strlen (Dir) - 1] != ':')
          strcat (Dir, "/");
      }
      else
        mystrcpy (Dir, ":");
    }
    else
    {
      rtFreeRequest (filereq);
      return (FALSE);
    }
    rtFreeRequest (filereq);
  }
  else
    return (FALSE);

  return (TRUE);
}

VOID StripNumbers (UBYTE *Name)
{
	UWORD i;

	for (i = 0; Name[i] != 0; i++)
	{
		if (isdigit (Name[i]))
		{
			Name[i] = 0;
			break;
		}
	}
}

VOID FixName (UBYTE *Dir, UBYTE *FileName, UBYTE *PicName, UWORD ext_len)
{
	UWORD len, len1;
	UBYTE Hold[20];
	UWORD ext;

	ext_len = 5;
	len = strlen (PicName);
	len1 = strlen (FileName);
	if (len1 > len)
	{
		if (!(strnicmp (FileName, PicName, len)))
		{
			CopyMem (&FileName[len], Hold, len1 - len + 1);
			{
				ext = atoi (Hold);
				if (strlen (Hold) < ext_len)
				{
					SPrintF (Dummy, "Rename \"%s%s\" TO \"%s%s%05ld\"", Dir, FileName, Dir, PicName, ext);
					Status (Dummy);
					Execute (Dummy, NULL, NULL);
				}
			}
		}
	}
}

VOID Rename_pics (VOID)
{
	UBYTE Dir[108] = "RAM:";
	static UBYTE FileName[100] = "";
  BOOL success;
  BPTR lock = 0;
  struct FileInfoBlock *fileinfoblock = 0;


	All (OFF);
	if (prefs->PicturesPath[0])	// Er den ok?
	{
		if (Exists (prefs->PicturesPath))	// F eks df0: som er tatt ut!
			mystrcpy (Dir, prefs->PicturesPath);
	}

	if (VE_GetFileName (Dir, FileName, GLS (&L_SELECT_FILE_TO_FIX), GLS (&L_FIX_IT)))
	{
		All (OFF);
		mystrcpy (prefs->PicturesPath, Dir);
		StripNumbers (FileName);

	  fileinfoblock = (struct FileInfoBlock *) AllocMem (sizeof (struct FileInfoBlock), MEMF_CLEAR);
	  if (lock = Lock (Dir, ACCESS_READ))
	  {
		  if (success = Examine (lock, fileinfoblock)) // Kunne ikke examinere
			{
			  Status (GLS (&L_PLEASE_WAIT));
			  for (;;)
			  {
			    success = ExNext (lock, fileinfoblock);	// FindNext File/Dir
			    if (!(success))				// End of directory ?
			      break;
			    if (fileinfoblock->fib_DirEntryType < 0)	// < 0 = a file
			    	FixName (Dir, fileinfoblock->fib_FileName, FileName, 5);
				}
			}
		}
	}
error:
  if (lock)
    UnLock (lock);
  if (fileinfoblock)
    FreeMem (fileinfoblock, sizeof (struct FileInfoBlock));
	All (ON);
}

//*****************************************************************************

enum
{
	ISO = 0,
	IBN,
	IBM,
	IBMM, 
};

#define MAX_SETS	7

UBYTE CharSets[][MAX_SETS + 1] =
{
	"Ê¯Â-∆ÿ≈",
	"ëõÜÑíùè",
	"ëîÜÑíôè",
	"ëõÜÑíùè"
};

BOOL Check_ASCII (UBYTE *Buffer)
{
  if (Buffer[0] == 0x00)		// Executable file
  if (Buffer[1] == 0x00)
  if (Buffer[2] == 0x03)
  if (Buffer[3] == 0xF3)
    return (FALSE);
  if (Buffer[0] == 0xE3)		// Icon file
  if (Buffer[1] == 0x10)
  if (Buffer[2] == 0x00)
  if (Buffer[3] == 0x01)
    return (FALSE );
  if (Buffer[0] == 0x0F)		// Font header file
  if (Buffer[1] == 0x00)
  if (Buffer[2] == 0x00)
    return (FALSE);
  if (Buffer[0] == 0xF3)		// info file
  if (Buffer[1] == 0x4C)
  if (Buffer[2] == 0x00)
  if (Buffer[3] == 0x12)
    return (FALSE);
  if (Buffer[0] == 0x44)		// BootBlock file
  if (Buffer[1] == 0x4F)
  if (Buffer[2] == 0x53)
  if (Buffer[3] == 0x00)
    return (FALSE);
  if (Buffer[0] == 0x46)		// Anc data
  if (Buffer[1] == 0x56)
  if (Buffer[2] == 0x4C)
  if (Buffer[3] == 0x30)
    return (FALSE);
  if (Buffer[0] == 0x00)		// Object file
  if (Buffer[1] == 0x00)
  if (Buffer[2] == 0x03)
  if (Buffer[3] == 0xE7 OR Buffer[3] == 0xFA)
    return (FALSE);
	if (!(strncmp, &Buffer[2], "-lh", 3))	// LHA, LZH archive
    return (FALSE);
	if (!(strncmp, Buffer, "PK", 2))	// ZIP archive
    return (FALSE);
	if (!(strncmp, Buffer, "LZX", 3))	// LZX archive
    return (FALSE);
	if (!(strncmp, Buffer, "DMS", 3))	// DMS archive
    return (FALSE);
	if (!(strncmp, Buffer, "Warp", 4))	// WRP archive
    return (FALSE);
	if (!(strncmp, Buffer, "PP20", 4))	// PP archive
    return (FALSE);
	if (!(strncmp, Buffer, "FORM", 4))	// IFF files
    return (FALSE);
	if (!(strncmp, Buffer, "PP20", 4))	// PP archive
    return (FALSE);

  return (TRUE);
}

ULONG Convert (UBYTE *Buffer, ULONG size)
{
  ULONG i, j, nr, c;
  BOOL flag;

  for (i = 0, j = 0; i < size; i++)
  {
		switch (Buffer[i])
 	  {
 	  	case 13: break;
    	default:
    	{
    		if (Buffer[i] == 0)
    		{
    			if (i != size - 1)
	    			return (FALSE);
	    		else
	    			return (j);
    		}
				if (!isalpha (Buffer[i]))
				{
					flag = FALSE;
					for (nr = IBN; nr <= IBMM; nr++)
					{
						for (c = 0; c < MAX_SETS; c++)
						{
							if (Buffer[i] == CharSets[nr][c])
							{
								Buffer[j] = CharSets[ISO][c];
								flag = TRUE;
								break;
							}
						}
						if (flag)
							break;
					}
					if (!flag)
						Buffer[j] = Buffer[i];
				}
				else
					Buffer[j] = Buffer[i];

				j++;
				break;
			}
		}
  }
  return (j);
}

VOID Convert_PC_text (VOID)
{
  BPTR fh = 0;
  ULONG newSize;
	UBYTE *Buffer = 0;
	LONG size = 0;
	static char CT_FileName[108] = "";
	char CT_Skuff[108] = "RAM:";
	char Hold[108];
	char Head[8];

	All (OFF);
	if (*prefs->ConvertTextPath)	// Er den ok?
	{
		if (Exists (prefs->ConvertTextPath))	// F eks df0: som er tatt ut!
			mystrcpy (CT_Skuff, prefs->ConvertTextPath);
	}

  if (VE_GetFileName (CT_Skuff, CT_FileName, GLS (&L_CONVERT_IBN), GLS (&L_LOAD_TEXT)))
  {
		All (OFF);
		mystrcpy (prefs->ConvertTextPath, CT_Skuff);
  	SPrintF (Hold, "%s%s", CT_Skuff, CT_FileName);
	  size = FileSize (Hold);
	  if (size > 0)
	  {
		  if (Buffer = AllocMem (size, MEMF_CLEAR | MEMF_PUBLIC))
		  {
			  if (fh = Open (Hold, MODE_OLDFILE))
			  {
			  	SPrintF (Dummy, GLS (&L_LOADING), Hold);
			  	Status (Dummy);
			    Read (fh, Head, 8);
			    if (Check_ASCII (Head))
			    {
				    Seek (fh, 0, OFFSET_BEGINNING);
				    Read (fh, Buffer, size);
				    Close (fh);
				    fh = 0;

				    Status (GLS (&L_CONVERTING));
				    newSize = Convert (Buffer, size);
				    if ((newSize != size) AND newSize > 0)
				    {
					    if (fh = Open (Hold, MODE_NEWFILE))
					    {
					      Status (GLS (&L_SAVING));
								Write (fh, Buffer, newSize);
					      Close (fh);
				  	    fh = 0;
								SPrintF (Dummy, GLS (&L_FILE_IS_NOW_SMALLER), size - newSize);
								JEOReqRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK));
							}
							else
								JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR_SAVING_FILE), GLS (&L_OK), S_ERROR);
						}
						else
							JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NOT_AN_IBN), GLS (&L_OK), S_ERROR);
					}
					else
						JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NOT_AN_ASCII), GLS (&L_OK), S_ERROR);
				}
				else
					JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR_LOADING_FILE), GLS (&L_OK), S_ERROR);
			}
			else
				JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_NO_MEM), GLS (&L_OK), S_ERROR);
		}
		else
			JEOSoundRequest (GLS (&L_MESSAGE), GLS (&L_ERROR_LOADING_FILE), GLS (&L_OK), S_ERROR);
	}
	if (fh)
	  Close (fh);
	if (Buffer)
		FreeMem (Buffer, size);
	All (ON);
}

VOID SaveROM (VOID)
{
	BPTR fh;
	static char CT_FileName[108] = "";
	char CT_Skuff[108] = "RAM:";
	char Hold[108];

	All (OFF);
	SPrintF (CT_FileName, "ROM %s", HW_ROMVersion);
	
  if (VE_GetFileName (CT_Skuff, CT_FileName, GLS (&L_SELECT_FILE), GLS (&L_SAVE_ROM)))
  {
  	SPrintF (Hold, "%s%s", CT_Skuff, CT_FileName);
  	SPrintF (Dummy, GLS (&L_SAVING_ROM_TO),  hw_ROM_start, hw_ROM_end - 1, Hold);
		Status (Dummy);
	
		if (fh = Open (Hold, MODE_NEWFILE))
		{
			Write (fh, (APTR)hw_ROM_start, hw_ROM_size);
			Close (fh);
		}
	}
	All (ON);
}
