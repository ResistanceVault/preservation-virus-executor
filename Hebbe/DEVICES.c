#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_DEVICES (VOID)
{
	InsertPatch (AUDIO_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (AUDIO_DEV, -30, 14, 0x4eaeffe2, VE_ROMUpdatesMsg);
	InsertPatch (AUDIO_DEV, -6, 0x20, 0x4eaefe26, VE_NewAudioMsg);
	InsertPatch (AUDIO_DEV, -12, 0x22, 0x4eaefe38, VE_NewAudioMsg);
	InsertPatch (AUDIO_DEV, -18, -0x20, 0x4eaefe38, VE_NewAudioMsg);
	InsertPatch (AUDIO_DEV, -30, 0x3a, 0x41ec00a6, VE_NewAudioMsg);
	InsertPatch (AUDIO_DEV, -36, 0x40, 0xd201d201, VE_NewAudioMsg);

	InsertPatch (GAMEPORT_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (GAMEPORT_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);

	InsertPatch (KEYBOARD_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (KEYBOARD_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);

	InsertPatch (RAMDRIVE_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (RAMDRIVE_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);

	InsertPatch (TIMER_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (TIMER_DEV, -30, 14, 0x4eaeffe2, VE_ROMUpdatesMsg);

	InsertPatch (TRACKDISK_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (TRACKDISK_DEV, -30, 14, 0xb090672a, VE_DiskCacheMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0x24, 0x4ebaf5d6, VE_PowerCacheMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0x1e, 0x4eba0164, VE_FacctionMsg);
	InsertPatch (TRACKDISK_DEV, -30, -0x154, MakeULONG (" HD "), VE_PowerHDFloppyMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0x48, 0x41ed0030, VE_QuietTDMsg);
	InsertPatch (TRACKDISK_DEV, -30, 0x8, 0x526c0334, VE_CacheItMsg);
  InsertPatch (TRACKDISK_DEV, -30, 0, 0x9efc0038, VE_DynamicCacheMsg);
  InsertPatch (TRACKDISK_DEV, -36, 14, 0x206c07a8, VE_DynamicCacheMsg);

	InsertPatch (CONSOLE_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (CONSOLE_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (CONSOLE_DEV, -30, 0x10, 0x24690018, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -6, 364, 0x6100f75e, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -12, 6, 0x246b0018, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -24, 48, 0x6100008e, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -36, 46, 0x226b0004, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -42, 66, 0x6100ff7c, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -48, 66, 0x246b0018, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -54, 376, 0x6100e8a8, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -60, 282, 0x6100e8a8, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -66, 152, 0x6100e8a8, VE_ROMUpdatesMsg);
  InsertPatch (CONSOLE_DEV, -72, 94, 0x6100e8a8, VE_ROMUpdatesMsg);

	InsertPatch (INPUT_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (INPUT_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (INPUT_DEV, -30, 0x0, 0x2f3afffa, VE_68040LibMsg);
	InsertPatch (INPUT_DEV, -30, 4, 0x0c690009, VE_68040LibMsg);

	InsertPatch (SCSI_DEV, -18, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (SCSI_DEV, -30, 0, 0x41fafffe, VE_ROMUpdatesMsg);
	InsertPatch (SCSI_DEV,  -6, 20, 0x00b04eae, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -12,  6, 0x266a0018, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -18, 10, 0x003c4a2d, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -24, 10, 0x003c4a2d, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -30, 12, 0x266a0018, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -36, 4, 0x2a69003c, VE_IdeFixMsg);
	InsertPatch (SCSI_DEV, -30, -8, MakeULONG ("user"), VE_HDSleepMsg);
	InsertPatch (SCSI_DEV, -30, 0, 0x9efc000c, VE_PowerCacheMsg);
	InsertPatch (SCSI_DEV, -18, 13, 0x2430291c, VE_1230SCSIMsg);
	InsertPatch (SCSI_DEV, -24, 13, 0x2430291c, VE_1230SCSIMsg);
	InsertPatch (SCSI_DEV, -30,  9, 0x2430291c, VE_1230SCSIMsg);
	InsertPatch (SCSI_DEV, -36, 13, 0x2430291c, VE_1230SCSIMsg);
  InsertPatch (SCSI_DEV,  -6,  6, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -12, 14, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -18, 10, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -24, 10, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -30,  6, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -36, 10, 0x2c6d0024, VE_ROMUpdatesMsg);
  InsertPatch (SCSI_DEV, -30, 22, 0x206c07a8, VE_DynamicCacheMsg);
  InsertPatch (SCSI_DEV, -36, 14, 0x206c07a8, VE_DynamicCacheMsg);
  InsertPatch (SCSI_DEV, -30, 548, 0x61000292, VE_VMMMsg);
	InsertPatch (SCSI_DEV, -30,  6, 0x67282029, VE_HDOffMsg);
}
