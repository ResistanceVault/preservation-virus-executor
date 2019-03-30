#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_EXPANSION_LIB (VOID)
{
  InsertPatch (EXPANSION_LIB, -30, 16, 0x6100b336, VE_MMULibMsg);
  InsertPatch (EXPANSION_LIB, -30, 86, 0x61000082, VE_MMULibMsg);
}
