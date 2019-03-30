#define PATCHBRAINSIZE	30

VOID InsertPatch (UWORD type, WORD function, WORD offset, ULONG data, UWORD name_off);
ULONG MakeULONG (char *Id);

VOID Insert_DEVICES (VOID);

VOID Insert_EXEC_LIB (VOID);
VOID Insert_UTIL_LIB (VOID);
VOID Insert_GFX_LIB (VOID);
VOID Insert_INT_LIB (VOID);
VOID Insert_DOS_LIB (VOID);
VOID Insert_GAD_LIB (VOID);
VOID Insert_LAY_LIB (VOID);
VOID Insert_ICON_LIB (VOID);
VOID Insert_MATHIEEE_LIB (VOID);
VOID Insert_MATHFFP_LIB (VOID);
VOID Insert_EXPANSION_LIB (VOID);
VOID Insert_WB_LIB (VOID);

extern char far PatchNames[][];
extern char PatchBrain[];
extern UWORD patchBrainVersion;
extern UWORD patchBrainRevision;

enum
{
	EXEC_LIB = 0,
	DOS_LIB,
	UTIL_LIB,
	GFX_LIB,
	INT_LIB,
	GAD_LIB,
	LAY_LIB,
	ICON_LIB,
	AUDIO_DEV,
	CONSOLE_DEV,
	INPUT_DEV,
	SCSI_DEV,
	TIMER_DEV,
	TRACKDISK_DEV,
	MATHIEEE_LIB,
	GAMEPORT_DEV,
	KEYBOARD_DEV,
	RAMDRIVE_DEV,
	CARDDISK_DEV,
	ROMBOOT_DEV,
	MATHFFP_LIB,
	KEYMAP_LIB,
	WB_LIB,
	EXPANSION_LIB
};

enum
{
  VE_UnknVecText = 0,
  VE_SnoopDosMsg, 				
  VE_AMOSLibMsg, 					
  VE_68040LibMsg,				
  VE_AlertPatch1xMsg, 		
  VE_A2232FixMsg, 				
  VE_AddPowerMsg, 				
  VE_AddPowerMsg95, 			
  VE_AlertPatchMsg, 			
  VE_ArqMsg, 							
  VE_AssignXMsg, 					
  VE_NewAudioMsg, 				
  VE_AutoRebootMsg, 			
  VE_BlitzFontsMsg, 			
  VE_BorderBlankMsg, 			
  VE_BusyPointerMsg, 			
  VE_CacheItMsg, 					
  VE_CheckDebugMsg, 			
  VE_CopyMemQuickerMsg, 	
  VE_CPUBlitMsg, 					
  VE_CPUClrUnicornMsg, 		
  VE_CPUClrCarnivoreMsg, 	
  VE_CPUClrPeterMsg, 			
  VE_CrMLibMsg,	 					
  VE_CrmDataDecrMsg, 			
  VE_CycleToMenuMsg	, 		
  VE_DiskCacheMsg, 				
  VE_DosPatchMsg, 				
  VE_DosPrefsMsg, 				
  VE_EasyReqPatchMsg, 		
  VE_EnforcerMsg, 				
  VE_EnhancedAlertMsg, 		
  VE_ExecPatchMsg, 				
  VE_ExplodeLibMsg, 			
  VE_FacctionMsg, 				
  VE_FasterBlitMsg, 			
  VE_FastFontsMsg, 				
  VE_FastMemCopyMsg, 			
  VE_FastWaitBlitMsg, 		
  VE_FastWaitBlitDaveMsg, 
  VE_FixReqMsg,						
  VE_ForceIconMsg, 				
  VE_FrontPubScreenMsg, 	
  VE_GOMFMsg, 						
  VE_PowerHDFloppyMsg, 		
  VE_ImmuneMsg,	 					
  VE_IPrefsMsg, 					
  VE_KCommMsg, 						
  VE_LibReqMsg, 					
  VE_LoadWBMsg, 					
  VE_LocaleLibMsg, 				
  VE_LowFragMsg, 					
  VE_LVDMsg, 							
  VE_MagicMenuMsg, 				
  VE_MakeVPortPatchMsg, 	
  VE_MemManMsg, 					
  VE_ModeProMsg,	 				
  VE_MultiCXMsg, 					
  VE_MultiUserMsg, 				
  VE_MungWallMsg, 				
  VE_MWAMsg, 							
  VE_NewAlertMsg, 				
  VE_NewIconsMsg, 				
  VE_NewMenuMsg, 					
  VE_NewModeMsg, 					
  VE_NickPrefsMsg, 				
  VE_NoCareMsg, 				
  VE_NOG2Msg, 						
  VE_NoReqMsg, 						
  VE_PatchAlertMsg, 			
  VE_PatchAllocMemMsg, 		
  VE_PatchControlMsg, 		
  VE_PatchLoadSegMsg, 		
  VE_PatchOpenWBMsg, 			
  VE_PatchPointerMsg, 		
  VE_PointerXDaveMsg, 		
  VE_PointerXMsg, 				
  VE_PopUpMenuMsg, 				
  VE_PowerCacheMsg, 			
  VE_PowerDataMsg, 				
  VE_PowerSnapMsg, 				
  VE_PPLibMsg, 						
  VE_PPPatcherMsg, 				
  VE_PPDRPMsg, 						
  VE_PPLoadSegMsg,				
  VE_PrintManagerMsg, 		
  VE_Prop3DMsg, 					
  VE_ProWizardMsg, 				
  VE_QMouseMsg, 					
  VE_QuickMenusMsg, 			
  VE_QuietTDMsg,	 				
  VE_ReplexMsg, 					
  VE_ReqChangeMsg, 				
  VE_ReqPatchMsg, 				
  VE_Reset1200Msg, 				
  VE_RomCrackMsg, 				
  VE_RTPatchMsg, 					
  VE_SaferPatchesMsg, 		
  VE_SegTrackerMsg, 			
  VE_SetManMsg, 					
  VE_SetPatchMsg, 				
  VE_SetPatchMrgCopMsg, 	
  VE_SFAPatchMsg, 				
  VE_SmartWBMsg, 					
  VE_SwazInfoMsg, 				
  VE_SwitchNTSCMsg, 			
  VE_SysExceptMsg, 				
  VE_SysiHackMsg, 				
  VE_TagScreensMsg, 			
  VE_TempoTopazMsg, 			
  VE_TimedReqMsg, 				
  VE_ScreenNotifyLibMsg, 			
  VE_ToolsDaemonMsg, 			
  VE_TWAMsg, 							
  VE_TurboTopazMsg, 			
  VE_UrouHackMsg, 				
  VE_VirusInterceptorMsg, 
  VE_WBExtMsg, 						
  VE_WBGaugeMsg,	 				
  VE_WReqMsg, 						
  VE_WTFPatchMsg, 				
  VE_xfdPatchMsg, 				
  VE_xLoadSegMsg, 				
  VE_XOperMsg, 						
  VE_ZoomWindowsMsg, 			
  VE_CyberGraphicLibMsg,	
  VE_CyberGraphicIntMsg,	
  VE_NeuroticDeathMsg,
  VE_SerDevice,
  VE_AudDevice,
  VE_68060LibMsg,				
  VE_CyberPatcherMsg,
  VE_MiamiMsg,
  VE_SmartCrashMsg,
  VE_LowLevelLibMsg,
  VE_FastExecMsg,
  VE_SlowResetMsg,
  VE_BetterOpenLibsMsg,
  VE_ShowMemMsg,
  VE_PoolMemMsg,
  VE_NoDeleteMsg,
  VE_CMQMsg,
  VE_SnapMsg,
  VE_IdeFixMsg,
  VE_HDSleepMsg,
  VE_NSDPatchMsg,
  VE_FBlitMsg,
  VE_QLayersMsg,
  VE_FTextMsg,
  VE_CyberLayersLibMsg,
  VE_1230SCSIMsg,
  VE_PrintManager1Msg, 		
  VE_DOSTraceMsg,
  VE_ExplodingLayersMsg,
  VE_AssignZMsg,
  VE_MagicCXMsg,
  VE_PatchHiddenMsg,
  VE_CEDFixMsg,
  VE_CyReqMsg,
  VE_NaeGrayPatchMsg,
  VE_NCommPatchMsg,
  VE_PatchBootMsg,
  VE_PatchInterleavedMsg,
  VE_PatchReplyMsgMsg,
  VE_PatchWBMsg,
  VE_PatchWorkMsg,
  VE_ROMUpdatesMsg,
  VE_NoFastMemMsg,
  VE_ReKickResetMsg,
  VE_MMULibMsg,
  VE_VisualPrefsMsg,
  VE_Dopus5LibMsg,
  VE_DiskLEDMsg,
  VE_MagicASLMsg,
  VE_ReqAttackMsg,
  VE_IconTraceMsg,
  VE_DynamicCacheMsg,
  VE_CEDPatchMsg,
  VE_MungMemMsg,
  VE_ExtraInfoMsg,
  VE_VMMMsg,
  VE_ScalaMsg,
  VE_MCPMsg,
  VE_HSMathLibsMsg,
  VE_RAWBInfoMsg,
  VE_PowerWindowsMsg,
  VE_WindowDaemonMsg,
  VE_NewWPA8Msg,
  VE_PatchWPA8Msg,
  VE_ExecutiveMsg,
  VE_BlazeWCPMsg,
  VE_StackAttackMsg,
  VE_Birdie2000Msg,
  VE_ObtainBetterPensMsg,
  VE_ParallelPatchMsg,
  VE_SerialPatchMsg,
  VE_SystemPatchMsg,
  VE_FastRADMsg,
  VE_QuickLensMsg,
  VE_TitleShadowMsg,
  VE_FastIPrefsMsg,
  VE_TopazPatchMsg,
  VE_BootControlMsg,
  VE_PatchMult64Msg,
  VE_FastLayersLibMsg,
  VE_RtgLibMsg,
  VE_WB2FastMsg,
  VE_PowerWBMsg,
  VE_CleverWinMsg,
  VE_CyberGuardMsg,
  VE_MathLibsPatchMsg,
  VE_FPPrefsMsg,
  VE_HDOffMsg,
  VE_AppChangeMsg,
  VE_WinSpeedMsg,
  VE_MagicFramesMsg,
  VE_FakeTopazMsg,
  VE_PatchRAMMsg,
  VE_MuForceMsg,
  VE_MuGuardianAngelMsg,
  VE_DefIconsMsg,
  VE_SysTrackerMsg,
  VE_SnoopSetFunctionMsg,
  VE_AslToRTMsg,
  VE_OutLineFontMsg,
  VE_MPLSPatchMsg,
  VE_KeyReqMsg,
  VE_OldIconsMsg,
  VE_MAX_PATCHES
};
