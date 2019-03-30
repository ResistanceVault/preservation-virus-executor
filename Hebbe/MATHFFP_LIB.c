#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_MATHFFP_LIB (VOID)
{
  InsertPatch (MATHFFP_LIB, -30, 0, 0x12006b34, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -36, 0, 0xf2004000, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -42, 0, 0x4a016a1e, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -48, 0, 0x70004a01, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -54, 0, 0x0200007f, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -60, 0, 0x4a006704, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -66, 4, 0x18016b00, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -72, 4, 0x18016700, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -78, 0, 0xd080670a, VE_MathLibsPatchMsg);
  InsertPatch (MATHFFP_LIB, -84, 0, 0xd080670a, VE_MathLibsPatchMsg);
}
