#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_WB_LIB (VOID)
{
	InsertPatch (WB_LIB, -72, 124, 0x2c6c0568, VE_PatchWBMsg);
	InsertPatch (WB_LIB, -78,  74, 0x2c6c0568, VE_PatchWBMsg);
  InsertPatch (WB_LIB, -6, 34, 0x6100ff04, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -12, 26, 0x6100fdf8, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -18, 22, 0x2c6900dc, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -24, 38, 0x206d0832, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -30, 142, 0x6100fc2c, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -36, 28, 0x286b0004, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -42, 30, 0x61006594, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -48, 104, 0x61007ce0, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -54, 12, 0x6100f7dc, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -60, 4, 0x61002b26, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -66, 12, 0x6100fe18, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -72, 16, 0x2f4a002c, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -78, 54, 0x6100fc9a, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -84, 94, 0x61002320, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -90, 56, 0x610001ea, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -96, 98, 0x2c6d00f8, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -102, 68, 0x2c6d00dc, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -108, 46, 0x2c6d00dc, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -114, 22, 0x2f4e002c, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -120, 62, 0x246a003a, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -126, 72, 0x2c6d00dc, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -132, 68, 0x2c6d00dc, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -18, 10, 0x2f4e0010, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -24, 30, 0x2f4e0008, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -30, 14, 0x2f4e0018, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -42, 16, 0x2f4e0018, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -48, 18, 0x2f4e001c, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -54, 6, 0x2f4e0004, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -60, 4, 0x61002aa6, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -66, 6, 0x2f4e0004, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -72, 16, 0x2f4a0024, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -78, 10, 0x2f4e0010, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -84, 14, 0x2f4e0014, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -90, 12, 0x2f4e0014, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -102, 22, 0x2f480020, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -108, 22, 0x2f490024, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -126, 12, 0x2f4a0024, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -132, 22, 0x2f480020, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -48, 16, 0x2f4a0028, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -54, 12, 0x2f4e0014, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -60, 14, 0x2f4c0030, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -66, 14, 0x2f4e0020, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -72, 14, 0x2f4a002c, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -78, 14, 0x2f4e0020, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -90, 376, MakeULONG ("Extr"), VE_ExtraInfoMsg);
  InsertPatch (WB_LIB, -60, 0, 0x60105359, VE_MCPMsg);
	InsertPatch (WB_LIB, -90, 171, MakeULONG ("RAWB"), VE_RAWBInfoMsg);
	InsertPatch (WB_LIB, -90, 18, 0x2c6d0062, VE_SwazInfoMsg);
	InsertPatch (WB_LIB, -60,  4, 0x2f0e2f0c, VE_ForceIconMsg);
	InsertPatch (WB_LIB, -66, 26, 0x4e912e00, VE_ForceIconMsg);
// Bag 2
  InsertPatch (WB_LIB, -6, 10, 0x2f4e000c, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -12, 14, 0x2f4e000c, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -24, 28, 0x2f4e0008, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -30, 14, 0x2f480018, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -48, 20, 0x2f4e0024, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -60, 4, 0x61002c60, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -72, 26, 0x2f4e0038, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -78, 4, 0x2f4e0004, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -96, 32, 0x2f480024, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -48, 16, 0x2f4a0024, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -54, 12, 0x2f4e0010, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -60, 44, 0x2f4e0044, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -72, 82, 0x2668002e, VE_Dopus5LibMsg);
// 3.9
  InsertPatch (WB_LIB, -60, 4, 0x61003050, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -72, 34, 0x2f4e0040, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -96, 88, 0x2f490038, VE_ROMUpdatesMsg);
  InsertPatch (WB_LIB, -48, 14, 0x2f4a0028, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -54, 12, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (WB_LIB, -90,  8, 0x2f4a002c, VE_Dopus5LibMsg);
	InsertPatch (WB_LIB, -90, 24, 0x45ec0860, VE_RAWBInfoMsg);
	InsertPatch (WB_LIB, -90, 22, 0x45ec8912, VE_RAWBInfoMsg);
	InsertPatch (WB_LIB, -60, -52, MakeULONG ("AppC"), VE_AppChangeMsg);
}
