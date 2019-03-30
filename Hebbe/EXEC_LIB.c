#include <JEO:JEO.h>
#include "prototypes.h"

VOID Insert_EXEC_LIB (VOID)
{
// ****************** VIRUS
  InsertPatch (EXEC_LIB, -456, 16, 0x00282f29, VE_NeuroticDeathMsg);

  InsertPatch (EXEC_LIB,  -36,  7, 0x18660c4a, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB,  -42, 10, 0x270008ae, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB,  -48,  5, 0x2456c04a, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB,  -60,  2, 0x02303d7c, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB,  -66,  9, 0x2a675433, VE_SystemPatchMsg);
//  InsertPatch (EXEC_LIB, -132,  0, 0x00000000, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -180, 11, 0x26700bb0, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -234,  4, 0x20126710, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -240,  0, 0x20102089, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -252,  4, 0x21490004, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -258,  8, 0xc1892348, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -264,  7, 0x04670a21, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -270, 15, 0x056ff420, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -306,  0, 0x226e0114, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -312,  0, 0x226e0114, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -318,  8, 0x226e0114, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -324, 12, 0x522e0126, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -354,  0, 0x41e90014, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -360,  0, 0x522e0127, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -366, 16, 0x522e0126, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -378,  0, 0x70072229, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -384,  0, 0x22680014, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -492, 12, 0x22680014, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -558,  0, 0x117c000f, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -570,  0, 0x522e0127, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -600, 34, 0x522e0127, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -606, 12, 0x22680014, VE_SystemPatchMsg);
  InsertPatch (EXEC_LIB, -408, -50, MakeULONG ("RTPa"), VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -552, -52, MakeULONG ("RTPa"), VE_RTPatchMsg);
	InsertPatch (EXEC_LIB, -30, 18, 0x2f3a0336,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -54, 4, 0x2f0d4e6d,	VE_68060LibMsg);
	InsertPatch (EXEC_LIB, -162, 0, 0x2f3a01f0,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -168, 0, 0x2f3a01fa,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -282, 4, 0x2f3a0228,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -396, 0, 0x2f3a0246,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -432, 0, 0x2f3a0240,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -486, 0, 0x2f3a0236,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -648, 26, 0x007c0700,VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -726, 8, 0x700072ff,	VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -762, 12, 0x22088291,VE_68060LibMsg);
  InsertPatch (EXEC_LIB, -768, 8, 0x22088291,	VE_68060LibMsg);
	InsertPatch (EXEC_LIB, -162, 0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -168, 0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -282, 0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -396, 0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -432, 0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -486, 0, 0x2f3afffa,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -648, 4, 0xc0814681,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -762, 6, 0x22088291,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -768, 6, 0x22088291,	VE_68040LibMsg);
  InsertPatch (EXEC_LIB, -522, 10, 0x2f4a000c,VE_IPrefsMsg);
  InsertPatch (EXEC_LIB, -552, 11, 0x36071356,VE_AddPowerMsg95);
  InsertPatch (EXEC_LIB, -624, -4, 0x2A417274,VE_CopyMemQuickerMsg);
  InsertPatch (EXEC_LIB, -630, 6, 0x651c722c,	VE_CopyMemQuickerMsg);
  InsertPatch (EXEC_LIB, -408, 11, 0x360713c9,VE_AddPowerMsg95);
  InsertPatch (EXEC_LIB, -210, 2, 0x50504d4d,		VE_PPLibMsg);
	InsertPatch (EXEC_LIB, -108, 14, 0x58b801bc,VE_AlertPatch1xMsg);
	InsertPatch (EXEC_LIB, -108, 0, 0x08390001,	VE_AlertPatchMsg);
	InsertPatch (EXEC_LIB, -108, -2, 0xff944aba,VE_EnforcerMsg);
	InsertPatch (EXEC_LIB, -648, 0, 0x522e0127,	VE_EnforcerMsg);
	InsertPatch (EXEC_LIB, -726, -2, 0xfd2a2f3a,VE_EnforcerMsg);
  InsertPatch (EXEC_LIB, -108, 10, 0x48454c50, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -132, 4, 0x0c2e0078, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -132, 6, 0x6706522e, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -132, 0, 0x0c9700fe, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -222, 8, 0x362a000e, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -444, 6, 0x2c012e00, VE_ROMUpdatesMsg);
	InsertPatch (EXEC_LIB, -570, 4, 0x5368000e, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -678, 8, 0x66085268, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -714, 12, 0x64562f0b, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -726, 4, 0x0129670c, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -408, 0x18, 0x7265712e, VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -552, 0x16, 0x7265712e, VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -408, 0x2e, 0x7265712e, VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -552, 0x2c, 0x7265712e, VE_RTPatchMsg);
  InsertPatch (EXEC_LIB, -54, 12, 0x223a00ce, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -54, 12, 0x223a19a4, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -54, 12, 0x20542068, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -282, 16, 0x20290032, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -282, 18, 0x20290032, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -624, -8, 0x2a4d422a, VE_FastMemCopyMsg);
  InsertPatch (EXEC_LIB, -630, -4, 0x2a4d422a, VE_FastMemCopyMsg);
  InsertPatch (EXEC_LIB, -210, 2, 0x50504d4d, VE_PPLibMsg);
  InsertPatch (EXEC_LIB, -444, 0x2e, 0x74726163, VE_LibReqMsg);
  InsertPatch (EXEC_LIB, -552, 0xa, 0x266c80fa, VE_LibReqMsg);
  InsertPatch (EXEC_LIB, -408, 0xa, 0x52ac8ad2, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -414, 0xc, 0x52ac8ad2, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -552, 0x8, 0x52ac8ad2, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -108, 0x32, 0x20c7208d, VE_NewAlertMsg);
  InsertPatch (EXEC_LIB, -420, 0x30, 0x60000292, VE_SaferPatchesMsg);
  InsertPatch (EXEC_LIB, -204, 0x12, 0x0cae41fa, VE_ImmuneMsg);
  InsertPatch (EXEC_LIB, -114, -0x7e, MakeULONG ("CHW!"), VE_RomCrackMsg);
  InsertPatch (EXEC_LIB, -444, 0x4, 0x43fa008d, VE_A2232FixMsg);
  InsertPatch (EXEC_LIB, -198, 0x54, MakeULONG ("MemM"), VE_MemManMsg);
  InsertPatch (EXEC_LIB, -630, -4, MakeULONG ("*Art"), VE_CopyMemQuickerMsg);
  InsertPatch (EXEC_LIB, -444, 0x18, 0x52ac0708, VE_PrintManagerMsg);
  InsertPatch (EXEC_LIB, -444, 0x18, 0x52ac0888, VE_PrintManagerMsg);
  InsertPatch (EXEC_LIB, -108, 4, 0x103affe4, VE_AlertPatchMsg);
  InsertPatch (EXEC_LIB, -108, 8, 0x50ed00cc, VE_NOG2Msg);
  InsertPatch (EXEC_LIB, -54, 4, 0x49fa3e1c, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -282, 4, 0x49fa3da6, VE_XOperMsg);
  InsertPatch (EXEC_LIB, -108, 4, 0x103afff8, VE_AlertPatchMsg);
  InsertPatch (EXEC_LIB, -108, 10, 0x60ba4a39, VE_GOMFMsg);
  InsertPatch (EXEC_LIB, -108, 0, 0x4eeefd2a, VE_SFAPatchMsg);
  InsertPatch (EXEC_LIB, -108, -2, 0xff944aba, VE_EnforcerMsg);
  InsertPatch (EXEC_LIB, -726, -2, 0xfd2a2f3a, VE_EnforcerMsg);
  InsertPatch (EXEC_LIB, -198, 4, 0x48e70020, VE_MungWallMsg);
  InsertPatch (EXEC_LIB, -210, 4, 0x48e70020, VE_MungWallMsg);
  InsertPatch (EXEC_LIB, -216, 4, 0x48e70020, VE_MungWallMsg);
  InsertPatch (EXEC_LIB, -408, 0xa, 0x52ad004a, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -552, 8, 0x52ad004a, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -282, 0x1a, 0x41fa0032, VE_SysExceptMsg);
  InsertPatch (EXEC_LIB, -114, 0x12, 0x41ed002f, VE_CheckDebugMsg);
  InsertPatch (EXEC_LIB, -282, 0x6, 0xcd4b2a49, VE_MultiUserMsg);
  InsertPatch (EXEC_LIB, -288, 0x1e, 0xd0fc0030, VE_MultiUserMsg);
  InsertPatch (EXEC_LIB, -108, 0, 0x08390001, VE_AlertPatchMsg);
  InsertPatch (EXEC_LIB, -36, 0, 0x2c5f082f, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -42, 0x12, 0x41ee0196, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -48, 6, 0x56c04a2e, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -54, 18, 0x302e0126, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -60, 0x12, 0x41ee0196, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -66, 0xe, 0x4bee0126, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -138, 0x16, 0x4bfa000a, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -252, 0, 0x20592251, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -270, 2, 0x12290009, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -306, 8, 0xc0814681, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -312, 0x12, 0xc0814681, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -318, 4, 0x23400016, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -324, 0, 0x41e9001a, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -384, 0, 0x22680014, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -456, 4, 0x2c690014, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -468, 2, 0x0129001e, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -474, 2, 0x0129001e, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -564, 4, 0x5268002c, VE_ExecPatchMsg);
  InsertPatch (EXEC_LIB, -108, 0xe, MakeULONG ("HELP"), VE_EnhancedAlertMsg);
  InsertPatch (EXEC_LIB, -108, 0x2, MakeULONG ("CUB!"), VE_PatchAlertMsg);
  InsertPatch (EXEC_LIB, -726, 0x2, 0x2c6d0008, VE_Reset1200Msg);
  InsertPatch (EXEC_LIB, -420, 0x24, 0x43ec0007, VE_SetManMsg);
  InsertPatch (EXEC_LIB, -402, 0x12, 0x287a005a, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -420, 0x1c, 0xb0aa0002, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -402, 0x22, 0x287a044c, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -420, 0x10, 0xb2ba09bc, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -198, 0x0, 0x08810002, VE_PatchAllocMemMsg);
  InsertPatch (EXEC_LIB, -456, 0x32, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -462, 0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -726, 0xa, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -456, 0x32+0x14, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -462, 0x28+0x14, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -726, 0xa+0x14, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -408, 0xa, 0x52ad009e, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -552, 8, 0x52ad009e, VE_ReqChangeMsg);
  InsertPatch (EXEC_LIB, -456, 0x32+0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -462, 0x28+0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -726, 0xa+0x28, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -216, 0x0, 0xb0bafff2, VE_LowFragMsg);
  InsertPatch (EXEC_LIB, -684, 0x0, 0xb0baffde, VE_LowFragMsg);
  InsertPatch (EXEC_LIB, -468, 0, 0x20570c90, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, -810, 0, 0x4a806a14, VE_ROMUpdatesMsg);
  InsertPatch (EXEC_LIB, - 96, 0, 0xbffa005e, VE_SnoopDosMsg);	// 3.0
  InsertPatch (EXEC_LIB, -294, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -366, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -390, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -444, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -498, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -552, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -594, 0, 0xbffa005e, VE_SnoopDosMsg);
  InsertPatch (EXEC_LIB, -282, 8, 0x207a001c, VE_SmartCrashMsg);	// 1.1
  InsertPatch (EXEC_LIB, -618, 0, 0xf0fc0400, VE_FastExecMsg);	// 2.9
  InsertPatch (EXEC_LIB, -366, 36, 0x226c0052, VE_NewMenuMsg);
  InsertPatch (EXEC_LIB, -408, 30, 0x266e0114, VE_BetterOpenLibsMsg);
  InsertPatch (EXEC_LIB, -444, 18, 0x49ed001c, VE_BetterOpenLibsMsg);
  InsertPatch (EXEC_LIB, -552, 28, 0x266e0114, VE_BetterOpenLibsMsg);
  InsertPatch (EXEC_LIB, -198, 12, 0x660650f9, VE_ShowMemMsg);
  InsertPatch (EXEC_LIB, -210, 12, 0x660650f9, VE_ShowMemMsg);
  InsertPatch (EXEC_LIB, -198, 16, 0x020000f8, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -204,  0, 0x22090281, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -210, 18, 0x76f8c283, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -216, 14, 0x01421001, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -684, 10, 0xb0bc0000, VE_PoolMemMsg);
  InsertPatch (EXEC_LIB, -624, 10, 0x02010003, VE_CMQMsg);
  InsertPatch (EXEC_LIB, -624,  4, 0x32090801, VE_CMQMsg);	// 060
  InsertPatch (EXEC_LIB, -630,  0, 0xe4880c80, VE_CMQMsg);
  InsertPatch (EXEC_LIB, -198,  0, 0x602a000b, VE_FBlitMsg);
  InsertPatch (EXEC_LIB, -198,  0, 0x6022000b, VE_FBlitMsg);
  InsertPatch (EXEC_LIB, -420, 20, MakeULONG ("Magi"), VE_MagicMenuMsg);
  InsertPatch (EXEC_LIB, -444, 22, 0x41ec8652, VE_PrintManager1Msg);
  InsertPatch (EXEC_LIB, -444, 42, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (EXEC_LIB, -450, 14, 0x206c0520, VE_MagicCXMsg);
  InsertPatch (EXEC_LIB, -726, 44, 0x206a0030, VE_PatchBootMsg);
  InsertPatch (EXEC_LIB, -402, 46, 0x2469000a, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -420, 84, 0x2268000a, VE_PatchControlMsg);
  InsertPatch (EXEC_LIB, -378,  0, 0x2011672e, VE_PatchReplyMsgMsg);
  InsertPatch (EXEC_LIB, -138, 26, 0x6100f354, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -198, 26, 0x6100faba, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -294, 32, 0x6100f502, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -336, 26, 0x6100f480, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -354, 24, 0x6100fb24, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -390, 28, 0x6100f60e, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -408, 16, 0x6100f3b8, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -420, 62, 0x6100f16c, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -540, 18, 0x6100f2f8, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -546, 18, 0x6100f03e, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -558, 34, 0x6100f414, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -570, 24, 0x6100f292, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -588, 36, 0x6100f218, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -594, 26, 0x6100f588, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -624, 54, 0x6100f94a, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -630, 32, 0x6100f890, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -654, 40, 0x6100f77e, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -672, 58, 0x6100f6c4, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -684, 26, 0x6100fa14, VE_PatchWorkMsg);
  InsertPatch (EXEC_LIB, -198, -14, MakeULONG ("NoFa"), VE_NoFastMemMsg);
  InsertPatch (EXEC_LIB, -216, -30, MakeULONG ("NoFa"), VE_NoFastMemMsg);
  InsertPatch (EXEC_LIB, -726, 182, 0x20680004, VE_ReKickResetMsg);
  InsertPatch (EXEC_LIB, -108, 22, 0x2c6c0064, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -618, 126, 0x610000be, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -648, 122, 0x6100f4a2, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -726, 10, 0x6100048c, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -762, 52, 0x6100f4a2, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -768, 32, 0x6100f49a, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -552, -176, MakeULONG ("ASL "), VE_MagicASLMsg);
  InsertPatch (EXEC_LIB, -456, 120, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -462, 110, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -726,  80, MakeULONG ("Mult"), VE_MultiCXMsg);
  InsertPatch (EXEC_LIB, -198, 22, MakeULONG ("Dyna"), VE_DynamicCacheMsg);
  InsertPatch (EXEC_LIB, -216, 16, MakeULONG ("Dyna"), VE_DynamicCacheMsg);
  InsertPatch (EXEC_LIB, -198, 324, MakeULONG ("Mung"), VE_MungMemMsg);
  InsertPatch (EXEC_LIB, -210, 238, MakeULONG ("Mung"), VE_MungMemMsg);
  InsertPatch (EXEC_LIB, -54, 86, 0x2f480012, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -198, 44, 0x61000292, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -210, 456, 0x61000292, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -216, 26, 0x6100ff34, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -282, 60, 0x20690036, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -318, 400, 0x6100ff16, VE_VMMMsg);
  InsertPatch (EXEC_LIB, -762, 249, MakeULONG ("8HRN"), VE_VMMMsg);
  InsertPatch (EXEC_LIB, -768, 169, MakeULONG ("8HRN"), VE_VMMMsg);
  InsertPatch (EXEC_LIB, -198, 24, 0x2c6c2600, VE_ScalaMsg);
  InsertPatch (EXEC_LIB, -564, 504, 0x2669fff8, VE_ScalaMsg);
  InsertPatch (EXEC_LIB, -570, 410, 0x2669fff8, VE_ScalaMsg);
  InsertPatch (EXEC_LIB, -576, 458, 0x2669fff8, VE_ScalaMsg);
  InsertPatch (EXEC_LIB, -108, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -198, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -282, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -552, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -624, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -630, 0, 0x60105359, VE_MCPMsg);
  InsertPatch (EXEC_LIB, -282, 14, 0x20680114, VE_SmartCrashMsg); // 1.3.0
  InsertPatch (EXEC_LIB,  -54, 16, 0x4eaeffc4, VE_ExecutiveMsg);
  InsertPatch (EXEC_LIB, -282,  0, 0x4eaeff7c, VE_ExecutiveMsg);
  InsertPatch (EXEC_LIB, -288,  0, 0x4eaeff7c, VE_ExecutiveMsg);
  InsertPatch (EXEC_LIB, -300, 10, 0xb07caaaa, VE_ExecutiveMsg);
	InsertPatch (EXEC_LIB, -444, 78, MakeULONG ("PARP"), VE_ParallelPatchMsg);
	InsertPatch (EXEC_LIB, -444, 78, MakeULONG ("PTCH"), VE_SerialPatchMsg);
  InsertPatch (EXEC_LIB, -198, 14, 0x610000a8, VE_FastRADMsg);
  InsertPatch (EXEC_LIB, -612,  8, 0x6100008c, VE_FastRADMsg);
  InsertPatch (EXEC_LIB, -354, 12, 0x2f4e001c, VE_Dopus5LibMsg);
  InsertPatch (EXEC_LIB, -282,  8, 0x2f4b000c, VE_Dopus5LibMsg);
  InsertPatch (EXEC_LIB, -288, 16, 0x2f4e0008, VE_Dopus5LibMsg);
  InsertPatch (EXEC_LIB, -294, 12, 0x2f4e000c, VE_Dopus5LibMsg);
	InsertPatch (EXEC_LIB, -198, 26, 0x2069000a, VE_WB2FastMsg);
	InsertPatch (EXEC_LIB, -444, 54, 0x61007468, VE_PrintManager1Msg);
	InsertPatch (EXEC_LIB, -108, 92, 0x6100f950, VE_CyberGuardMsg);
  InsertPatch (EXEC_LIB, -684, 12, 0x226e0114, VE_PatchRAMMsg);
  InsertPatch (EXEC_LIB, -108, 20, 0x2c6c0d04, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -282, 26, 0x206c0d70, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -648, 122, 0x6100f44c, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -726, 10, 0x61000648, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -762, 52, 0x6100f44c, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -768, 32, 0x6100f444, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -648, 122, 0x6100f44a, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -762, 52, 0x6100f44a, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -768, 32, 0x6100f442, VE_MMULibMsg);
  InsertPatch (EXEC_LIB, -186, 238, 0x6100020c, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -192, 140, 0x6100020c, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -198, 60, 0x61000e3e, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -204, 52, 0x61000992, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -210, 42, 0x61000baa, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -216, 30, 0x6100083e, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -684, 124, 0x61000e3e, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -690, 62, 0x61000baa, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -696, 32, 0x61000750, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -702, 30, 0x6100069c, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -708, 32, 0x6100062c, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -714, 34, 0x61000492, VE_MuGuardianAngelMsg);
  InsertPatch (EXEC_LIB, -414, 3, MakeULONG ("� Sy"), VE_SysTrackerMsg);
  InsertPatch (EXEC_LIB, -444, 3, MakeULONG ("� Sy"), VE_SysTrackerMsg);
  InsertPatch (EXEC_LIB, -450, 3, MakeULONG ("� Sy"), VE_SysTrackerMsg);
  InsertPatch (EXEC_LIB, -552, 3, MakeULONG ("� Sy"), VE_SysTrackerMsg);
  InsertPatch (EXEC_LIB, -420, 46, 0x2269017a, VE_SnoopSetFunctionMsg);
  InsertPatch (EXEC_LIB, -282, 9, 0xe061064c, VE_OldIconsMsg);
}
