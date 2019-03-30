#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_MATHIEEE_LIB (VOID)
{
	InsertPatch (MATHIEEE_LIB, -30, 0, 0xf2004400, VE_ROMUpdatesMsg);
	InsertPatch (MATHIEEE_LIB, -36, 0, 0xf2004000, VE_ROMUpdatesMsg);
	InsertPatch (MATHIEEE_LIB, -42, 0, 0xf2004400, VE_ROMUpdatesMsg);
	InsertPatch (MATHIEEE_LIB, -66, 0, 0xf2004400, VE_ROMUpdatesMsg);
	InsertPatch (MATHIEEE_LIB, -72, 0, 0xf2004400, VE_ROMUpdatesMsg);
	InsertPatch (MATHIEEE_LIB, -78, 0, 0xf2004400, VE_ROMUpdatesMsg);
	InsertPatch (MATHIEEE_LIB, -84, 0, 0xf2004400, VE_ROMUpdatesMsg);
	InsertPatch (MATHIEEE_LIB, -90, 4, 0xf2004403, VE_ROMUpdatesMsg);
  InsertPatch (MATHIEEE_LIB, -30, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (MATHIEEE_LIB, -78, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (MATHIEEE_LIB, -84, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (MATHIEEE_LIB, -30, 320, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -36, 300, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -42, 280, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -48, 224, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -54, 208, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -60, 200, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -66, 192, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -72, 168, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -78, 144, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -84, 120, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -90,  96, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -96,  40, MakeULONG ("math"), VE_HSMathLibsMsg);
  InsertPatch (MATHIEEE_LIB, -54, 0, 0xd080d080, VE_MathLibsPatchMsg);
  InsertPatch (MATHIEEE_LIB, -60, 0, 0x4a806706, VE_MathLibsPatchMsg);
}
