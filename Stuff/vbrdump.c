#include <JEO:JEO.h>
#include <exec/types.h>
#include <exec/execbase.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <exec/memory.h>

int main(void);

APTR GetVBR2(void);


#define VBRLEN	0x100

int main(void)
{
  APTR LocalVBR, VBR;
  LONG res;

  LocalVBR = AllocVec(VBRLEN, MEMF_PUBLIC);
  if (!LocalVBR)
  {
    return RETURN_FAIL;
  }

  Disable();
  VBR = GetVBR2();
  CopyMem(VBR, LocalVBR, VBRLEN);
  Enable();

  res = Write(Output(), LocalVBR, VBRLEN);
  FreeVec(LocalVBR);

  return res == VBRLEN ? RETURN_OK : RETURN_ERROR;
}

APTR GetVBR2(void)
{
  static const UWORD getvbr[3] = { 0x4E7A, 0x0801, 0x4E73 }; /* movec vbr,d0 / rte */

  if (SysBase->AttnFlags & AFF_68010)
    return (APTR) Supervisor((ULONG (* const)(void)) getvbr);
  else
    return NULL;
}
