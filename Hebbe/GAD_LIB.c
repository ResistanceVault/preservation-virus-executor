#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_GAD_LIB (VOID)
{
	InsertPatch (GAD_LIB, -60, 0x6, 0x4a3af8ca, VE_AddPowerMsg);
	InsertPatch (GAD_LIB, -66, 0x6, 0x4a3af8e4, VE_AddPowerMsg);
	InsertPatch (GAD_LIB, -30, 0x10, 0x4e934cdf, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -36, 0x18, 0x02400107, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -42, 0xe, 0x02400107, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -72, 0x40, 0x02410107, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -78, 0x10, 0xb0a9000e, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -102, 0x36, 0x02410107, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -108, 0x10, 0xb2a9000e, VE_CycleToMenuMsg);
	InsertPatch (GAD_LIB, -66, 0x22, 0x206c05a6, VE_ModeProMsg);
	InsertPatch (GAD_LIB, -66, 0x22, 0x206c07da, VE_ModeProMsg);
	InsertPatch (GAD_LIB, -48, 14, 0x2f4e0034, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -48, 0, 0x9efc0030, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -54, 18, 0x2c6c0390, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -54, 8, 0x2f4e0008, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -60, 0, 0x9efc0010, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -66, 0, 0x9efc0010, VE_NewMenuMsg);
	InsertPatch (GAD_LIB, -30, 0x64, MakeULONG ("Mult"), VE_MultiCXMsg);
	InsertPatch (GAD_LIB, -30, 0x64+0x14, MakeULONG ("Mult"), VE_MultiCXMsg);
	InsertPatch (GAD_LIB, -30, 0x64+0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
	InsertPatch (GAD_LIB, -30, 0x42, 0x0c402452, VE_Prop3DMsg);
	InsertPatch (GAD_LIB, -36, 0x2e, 0xc06b0010, VE_Prop3DMsg);
	InsertPatch (GAD_LIB, -126, 0x26, 0x4eb01800, VE_Prop3DMsg);
	InsertPatch (GAD_LIB, -132, 0x2a, 0xbbeb0012, VE_Prop3DMsg);
  InsertPatch (GAD_LIB,  -30 ,24, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (GAD_LIB,  -36 ,14, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (GAD_LIB,  -42 ,26, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (GAD_LIB, -126 ,94, 0x206d0058, VE_MagicCXMsg);
  InsertPatch (GAD_LIB, -132 ,18, 0x226c0142, VE_MagicCXMsg);
  InsertPatch (GAD_LIB, -174 ,26, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (GAD_LIB,  -66, 14, 0x2f4e001c, VE_ModeProMsg);
  InsertPatch (GAD_LIB,  -30, -2, 0x000a4ef9, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB,  -42, -2, 0x000a4ef9, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB,  -36, -2, 0x000a4ef9, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB, -102, -2, 0x000a4ef9, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB, -126, -2, 0x000a4ef9, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB, -132, -2, 0x000a4ef9, VE_VisualPrefsMsg);
  InsertPatch (GAD_LIB, -18, 298, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (GAD_LIB, -30, 170, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (GAD_LIB, -30, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (GAD_LIB, -36, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (GAD_LIB, -30, 102, 0x20680008, VE_MagicFramesMsg);
}
