#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_ICON_LIB (VOID)
{
	InsertPatch (ICON_LIB, -42, 0x72, 0x7041d080, VE_ForceIconMsg);
	InsertPatch (ICON_LIB, -42, 0x34, 0xb2280008, VE_ForceIconMsg);
	InsertPatch (ICON_LIB, -48, 0x32, 0xb2280008, VE_ForceIconMsg);
	InsertPatch (ICON_LIB, -42, 0xa, 0x2f4a0020, VE_NewIconsMsg);
	InsertPatch (ICON_LIB, -48, 0xa, 0x48ef4100, VE_NewIconsMsg);
	InsertPatch (ICON_LIB, -54, 0x8, 0x2f4e0014, VE_NewIconsMsg);
  InsertPatch (ICON_LIB, -42, 20, 0x226c04d8, VE_MagicCXMsg);
  InsertPatch (ICON_LIB,  -42, 10, 0x266cfdd8, VE_NewIconsMsg);
  InsertPatch (ICON_LIB,  -48, 10, 0x6100eb7c, VE_NewIconsMsg);
  InsertPatch (ICON_LIB,  -54, 10, 0x6100ee96, VE_NewIconsMsg);
  InsertPatch (ICON_LIB, -120, 50, 0x2c6c02ec, VE_NewIconsMsg);
  InsertPatch (ICON_LIB, -132, 52, 0x2c6c02f0, VE_NewIconsMsg);
  InsertPatch (ICON_LIB,  -96, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (ICON_LIB, -102, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (ICON_LIB, -6, 40, 0x2c690026, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -12, 66, 0x2c6d0026, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -18, 26, 0x2c690026, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -24, 40, 0x2c6d0026, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -30, 74, 0x2f4e0018, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -36, 54, 0x2f4e0018, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -42, 94, 0x2f4e0018, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -48, 62, 0x226b005e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -54, 28, 0x226a000a, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -60, 34, 0x2f4e0018, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -66, 32, 0x2f4e0018, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -72, 40, 0x6100faf2, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -78, 44, 0x226b005e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -84, 46, 0x6100e876, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -90, 18, 0x226b005e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -96, 10, 0x2f4e0018, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -102, 10, 0x2f4e0028, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -108, 32, 0x6100ff2e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -114, 26, 0x2c6d0026, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -120, 192, 0x2f480044, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -126, 144, 0x2c6d0032, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -132, 44, 0x226b005e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -138, 24, 0x6100e876, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -144, 34, 0x246a0002, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -150, 28, 0x2c6d002e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -156, 58, 0x226d005e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -162, 104, 0x2c6d0032, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -168, 48, 0x2c6d0026, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -174, 64, 0x6100f73a, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -180, 156, 0x266a0004, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -186, 76, 0x2f490040, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -192, 38, 0x2c6d002e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -198, 154, 0x26680030, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -84, 46, 0x6100e87a, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -138, 24, 0x6100e87a, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -156, 8, 0x2f490050, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -174, 50, 0x2f48000c, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -198, 124, 0x2f480018, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -42, 68, 0x6100013c, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -48, 78, 0x6100fe54, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -78, 40, 0x6100013c, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -84, 50, 0x6100fe54, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -96, 34, 0x6100fdd8, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -102, 36, 0x2f480008, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -120, 40, 0x610000ce, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -126, 34, 0x2c6c000c, VE_IconTraceMsg);
  InsertPatch (ICON_LIB, -132, 68, 0x610000ce, VE_IconTraceMsg);
// Bag 2
  InsertPatch (ICON_LIB, -72, 12, 0x2f490010, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -84, 46, 0x6100e82c, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -138, 24, 0x6100e82c, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -150, 14, 0x2f490020, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -192, 36, 0x2c6d002e, VE_ROMUpdatesMsg);
  InsertPatch (ICON_LIB, -84, 24, 0x2f4e0020, VE_Dopus5LibMsg);
  InsertPatch (ICON_LIB, -138, 20, 0x2f4e0018, VE_Dopus5LibMsg);
  InsertPatch (ICON_LIB, -138, 20, 0x2f4e001c, VE_Dopus5LibMsg);
}
