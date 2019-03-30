#include <JEO:JEO.h>
#include <ctype.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <libraries/reqtools.h>
#include <proto/reqtools.h>
#include "VE_Prototypes.h"
#include <JEO:raw.h>
#include "PrefsWindow.h"
#include <intuition/gadgetclass.h>

BOOL prefsGadgetFlag;

VOID PrefsActivateGadget (UWORD nr)
{
  ActivateGadget (PrefsGadgets[nr], PrefsWnd, NULL);
}

VOID PrefsInitGadget (UWORD num, LONG tagtype, LONG tagvalue)
{
  GT_SetGadgetAttrs (PrefsGadgets[num], PrefsWnd, NULL, tagtype, tagvalue, TAG_DONE);
}

VOID PrefsGadget (BOOL status)
{
  if (status == PÅ)
  {
    if (prefsGadgetFlag != PÅ)
    {
      AddGList (PrefsWnd, PrefsGList, -1, -1, NULL);
      prefsGadgetFlag = PÅ;
			ClearPointer (PrefsWnd);
    }
  }
  else	/* AV */
  {
    if (prefsGadgetFlag != AV)
    {
      RemoveGList (PrefsWnd, PrefsGList, -1);
			prefsGadgetFlag = AV;
			rtSetWaitPointer (PrefsWnd);
    }
  }
}

VOID DoLogPath (char *Path)
{
	if (*Path)
	{
		BPTR lock;

		if ((Path[strlen (Path) - 1] != ':') AND (Path[strlen (Path) - 1] != '/'))
			strcat (Path, "/");

		if (lock = Lock (Path, ACCESS_READ))
		{
			NameFromLock (lock, Path, 128);
			UnLock (lock);
			strcpy (prefs->Log_path, Path);
		}
	}
	PrefsInitGadget (GD_P_LOG_PATH, GTST_String, (LONG)"");
	PrefsInitGadget (GD_P_LOG_PATH, GTST_String, (LONG)prefs->Log_path);
}


VOID PrefsWindow (VOID)
{
	ULONG Signals;
	ULONG class;
	USHORT Code;
	APTR Address;
	struct IntuiMessage *message;
	BOOL go_flag;

	if (OpenPrefsWindow ())
		return;

	prefsGadgetFlag = ON;

// File checking
	if (prefs->check_crunched)
		PrefsInitGadget (GD_P_DECRUNCH_FILES, (GTCB_Checked), ON);
	
	if (prefs->overwrite_crunched_files)
		PrefsInitGadget (GD_P_OVERWRITE_CRUNCHED_FILES, (GTCB_Checked), ON);
	if (prefs->skip_encrypted)
		PrefsInitGadget (GD_P_SKIP_ENCRYPTED, (GTCB_Checked), ON);

	if (!prefs->check_crunched)
	  PrefsInitGadget (GD_P_OVERWRITE_CRUNCHED_FILES, GA_Disabled, TRUE);

	if (!xfdmaster_flag)
 	{
		PrefsInitGadget (GD_P_DECRUNCH_FILES, GA_Disabled, TRUE);
 	  PrefsInitGadget (GD_P_OVERWRITE_CRUNCHED_FILES, GA_Disabled, TRUE);
		if (!xadmaster_flag)
		  PrefsInitGadget (GD_P_SKIP_ENCRYPTED, GA_Disabled, TRUE);
	}

	if (prefs->checkData)
		PrefsInitGadget (GD_P_CHECK_DATA_FILES, (GTCB_Checked), ON);

	if (prefs->extract_archives)
		PrefsInitGadget (GD_P_EXTRACT_ARCHIVES, (GTCB_Checked), ON);

	PrefsInitGadget (GD_P_NA, (GTCB_Checked), OFF);
	PrefsInitGadget (GD_P_NA, GA_Disabled, TRUE);
	PrefsInitGadget (GD_P_NA1, (GTCB_Checked), OFF);
	PrefsInitGadget (GD_P_NA1, GA_Disabled, TRUE);

	if (!xadmaster_flag)
		PrefsInitGadget (GD_P_EXTRACT_ARCHIVES, GA_Disabled, TRUE);

	if (prefs->skip_days)
		PrefsInitGadget (GD_P_SKIP_DAYS, (GTCB_Checked), ON);
	else
 	  PrefsInitGadget (GD_P_DAYS_COUNT, GA_Disabled, TRUE);
	PrefsInitGadget (GD_P_DAYS_COUNT, GTIN_Number, prefs->days_count);

	if (prefs->fast_file_checking)
		PrefsInitGadget (GD_P_FAST_FILE_CHECKING, (GTCB_Checked), ON);

// Misc

// Logging
	PrefsInitGadget (GD_P_LOG_PATH, GTST_String, (LONG)prefs->Log_path);
	if (prefs->logging)
		PrefsInitGadget (GD_P_LOGGING, (GTCB_Checked), ON);
	else
 	  PrefsInitGadget (GD_P_LOG_PATH, GA_Disabled, TRUE);

	if (prefs->iconify)
		PrefsInitGadget (GD_P_ICONIFY, (GTCB_Checked), ON);

	go_flag = TRUE;
	while (go_flag)
	{
		Signals = Wait (1 << PrefsWnd->UserPort->mp_SigBit);

		if (Signals & (1<<PrefsWnd->UserPort->mp_SigBit))
		{
			while (message = (struct IntuiMessage *)GetMsg (PrefsWnd->UserPort))
			{
        class = message->Class;
        Code = message->Code;
        Address = message->IAddress;
        ReplyMsg ((struct Message *)message);
		
				switch (class)
				{
					case IDCMP_DISKINSERTED:
						All (OFF);
						DiskInserted ();
						All (ON);
						break;
					case IDCMP_DISKREMOVED:
						DiskRemoved ();
						break;
					case IDCMP_CLOSEWINDOW:
						go_flag = FALSE;
						break;
					case IDCMP_RAWKEY:
					{
						switch (Code)
						{
							case ESC:
								go_flag = FALSE;
								break;
						}
						break;
					}
					case	IDCMP_GADGETUP:
					{
					  struct Gadget *gadget;
						UWORD nr;

					  gadget = (struct Gadget *)Address;
						nr = gadget->GadgetID;

						switch (nr)
						{
							case GD_P_DECRUNCH_FILES:
							{
								prefs->check_crunched ^= 1;
								if (prefs->check_crunched)
							    PrefsInitGadget (GD_P_OVERWRITE_CRUNCHED_FILES, GA_Disabled, FALSE);
							  else
							    PrefsInitGadget (GD_P_OVERWRITE_CRUNCHED_FILES, GA_Disabled, TRUE);
								break;
							}
							case GD_P_OVERWRITE_CRUNCHED_FILES:
								prefs->overwrite_crunched_files ^= 1;
								break;
							case GD_P_SKIP_ENCRYPTED:
								prefs->skip_encrypted ^= 1;
								break;
							case GD_P_CHECK_DATA_FILES:
								prefs->checkData ^= 1;
								break;
							case GD_P_EXTRACT_ARCHIVES:
								prefs->extract_archives ^= 1;
								break;
							case GD_P_SKIP_DAYS:
							{
								prefs->skip_days ^= 1;

								if (prefs->skip_days)
								{
							 	  PrefsInitGadget (GD_P_DAYS_COUNT, GA_Disabled, FALSE);
									PrefsActivateGadget (GD_P_DAYS_COUNT);
							 	}
							 	else
							 	  PrefsInitGadget (GD_P_DAYS_COUNT, GA_Disabled, TRUE);
							 	break;
						 	}
							case GD_P_DAYS_COUNT:
							{
								prefs->days_count = GetNumber (gadget);
								PrefsInitGadget (GD_P_DAYS_COUNT, GTIN_Number, 0);
								PrefsInitGadget (GD_P_DAYS_COUNT, GTIN_Number, (ULONG)prefs->days_count);
								break;
							}
							case GD_P_FAST_FILE_CHECKING:
							{
								prefs->fast_file_checking ^= 1;
								if (prefs->fast_file_checking)
								{
									prefs->logging = TRUE;
									PrefsInitGadget (GD_P_LOGGING, (GTCB_Checked), ON);
									PrefsInitGadget (GD_P_LOG_PATH, GA_Disabled, 0);
									if (!Exists (prefs->Log_path))
									{
										char Hold[6] = "RAM:";

										DoLogPath (Hold);
									}
									sprintf (Dummy, GLS (&L_THIS_OPTION_WILL_DISABLE), prefs->Log_path);
									PrefsGadget (OFF);
									JEOSoundRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK), S_MESSAGE);
									PrefsGadget (ON);
								}
								break;
							}
							case GD_P_LOGGING:
							{
								prefs->logging ^= 1;
						 	  PrefsInitGadget (GD_P_LOG_PATH, GA_Disabled, prefs->logging ^ 1);
								if (!prefs->logging)
								{
									prefs->fast_file_checking = OFF;
									PrefsInitGadget (GD_P_FAST_FILE_CHECKING, (GTCB_Checked), OFF);
								}
								break;
							}

							case GD_P_LOG_PATH:
							{

								char Hold[128];
					      strcpy (Hold, GetString (gadget));
								DoLogPath (Hold);

								break;
							}

							case GD_P_FILE:
							{
							  char Hold[129] = "";
							  BOOL get_path_flag = FALSE;

								PrefsGadget (OFF);
								if (!*prefs->Log_path)
								{
								  BPTR lock;

									if (lock = Lock (Hold, ACCESS_READ))
									{
										NameFromLock (lock, Hold, 128);
										UnLock (lock);
									}
								}
								else
									strcpy (Hold, prefs->Log_path);

								if (GetDirName (Hold, GLS (&L_SELECT_LOG_PATH), NULL))
								{
									get_path_flag = TRUE;

									if (*Hold)
						      {
									  BPTR lock;

						        if ((Hold[strlen (Hold) - 1] != ':') AND (Hold[strlen (Hold) - 1] != '/'))
						          strcat (Hold, "/");

										if (lock = Lock (Hold, ACCESS_READ))
										{
											NameFromLock (lock, Hold, 128);
											UnLock (lock);
											strcpy (prefs->Log_path, Hold);
										}
									}
						      strcpy (prefs->Log_path, Hold);
								}
								PrefsGadget (ON);
								if (get_path_flag)
								{
									if (prefs->Log_path[strlen (prefs->Log_path)-1] == '/')
										prefs->Log_path[strlen (prefs->Log_path)-1] = 0;

								  PrefsInitGadget (GD_P_LOG_PATH, GTST_String, (LONG)"");
								  PrefsInitGadget (GD_P_LOG_PATH, GTST_String, (LONG)prefs->Log_path);
										
									if (!prefs->logging)
									{
										prefs->logging = ON;
										PrefsInitGadget (GD_P_LOGGING, (GTCB_Checked), ON);
								 	  PrefsInitGadget (GD_P_LOG_PATH, GA_Disabled, FALSE);
								 	}
								}
								break;
							}

							case GD_P_ICONIFY:
							{
								prefs->iconify ^= 1;
								break;
							}
						}
						break;
					}
					case	IDCMP_GADGETDOWN:
					{
					  struct Gadget *gadget;
						UWORD nr;

					  gadget = (struct Gadget *)Address;
						nr = gadget->GadgetID;
//				  	SPrintF (Dummy, "GADGETDOWN %ld\n", nr);
//				  	JEOReqRequest (GLS (&L_MESSAGE), Dummy, GLS (&L_OK));

						break;
					}	
				}
			}
		}
	}

	if (PrefsWindow)
		ClosePrefsWindow ();

	CloseLog ();
	if (prefs->logging)
		OpenLog ();
}
