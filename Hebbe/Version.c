#include <exec/types.h>

UWORD patchBrainVersion = 1;
UWORD patchBrainRevision = 35; // <<<<< HUSK **********************************

#define PATCHBRAINSIZE	30
char PatchBrain[PATCHBRAINSIZE] = "\0$VER: 1.35 (13.05.2001)";	// ALWAYS 30 chars
