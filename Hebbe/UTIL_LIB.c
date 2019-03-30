#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_UTIL_LIB (VOID)
{
  InsertPatch (UTIL_LIB, -162,  14, 0x206e008c, VE_IPrefsMsg);
  InsertPatch (UTIL_LIB, -168,  12, 0x206e008c, VE_IPrefsMsg);
  InsertPatch (UTIL_LIB, -174,  10, 0x206e008c, VE_IPrefsMsg);
  InsertPatch (UTIL_LIB, -180,  10, 0x206e008c, VE_IPrefsMsg);
	InsertPatch (UTIL_LIB, -198,   0, 0x518f4857, VE_68060LibMsg);
	InsertPatch (UTIL_LIB, -204,   0, 0x518f4857, VE_68060LibMsg);
  InsertPatch (UTIL_LIB,  -60,  28, 0x6100cf52, VE_PatchWorkMsg);
  InsertPatch (UTIL_LIB, -150,  22, 0x6100ceea, VE_PatchWorkMsg);
  InsertPatch (UTIL_LIB, -156,  22, 0x6100ce90, VE_PatchWorkMsg);
  InsertPatch (UTIL_LIB, -228,  24, 0x6100d01e, VE_PatchWorkMsg);
  InsertPatch (UTIL_LIB, -198, 102, MakeULONG ("t64_"), VE_PatchMult64Msg);
  InsertPatch (UTIL_LIB, -204, 182, MakeULONG ("t64_"), VE_PatchMult64Msg);
}
