
_LVOSetFunction	EQU	-$000001A4
_LVOForbid	EQU	-$00000084
_LVOInitRequester	EQU	-$0000008A
_LVOWait	EQU	-$0000013E
_LVOCloseLibrary	EQU	-$0000019E
_LVOCacheClearU	EQU	-$0000027C
_LVOReleaseSemaphoreList	EQU	-$0000024C
_LVOEasyRequestArgs	EQU	-$0000024C
_LVOPermit	EQU	-$0000008A
_LVOPubScreenStatus	EQU	-$00000228
_LVOOpenLibrary	EQU	-$00000228
_LVOGetPrefs	EQU	-$00000084
_LVOFindTask	EQU	-$00000126
****************************************************************************
	SECTION	snoopsf000000,CODE
ProgStart
	MOVEA.L	(4).W,A6	;2C780004
	LEA	(IntLibText,PC),A1	;43FA010C
	MOVE.L	#$00000025,D0	;203C00000025
	JSR	(_LVOOpenLibrary,A6)	;4EAEFDD8
	MOVE.L	D0,(IntBase).L	;23C00840EB3E
	BEQ.W	Exit	;67000072
	MOVEA.L	(4).W,A6	;2C780004
	JSR	(_LVOForbid,A6)	;4EAEFF7C
	JSR	(_LVOCacheClearU,A6)	;4EAEFD84
	MOVEA.L	(4).L,A1	;227900000004
	MOVE.W	#_LVOSetFunction,D0	;303CFE5C
	EXT.L	D0	;48C0
	ADDA.L	D0,A1	;D3C0
	ADDQ.L	#2,A1	;5489
	LEA	(NewCode,PC),A2	;45FA0056
	MOVE.L	(A1),(OldFunction).L	;23D10840EB3A
	MOVE.L	A2,(A1)	;228A
	MOVEA.L	(4).L,A6	;2C7900000004
	JSR	(_LVOPermit,A6)	;4EAEFF76
	MOVE.L	#$00001000,D0	;203C00001000
	JSR	(_LVOWait,A6)	;4EAEFEC2
	MOVEA.L	(4).W,A6	;2C780004
	JSR	(_LVOForbid,A6)	;4EAEFF7C
	JSR	(_LVOCacheClearU,A6)	;4EAEFD84
	MOVEA.L	(4).L,A1	;227900000004
	MOVE.W	#_LVOSetFunction,D0	;303CFE5C
	EXT.L	D0	;48C0
	ADDA.L	D0,A1	;D3C0
	ADDQ.L	#2,A1	;5489
	MOVE.L	(OldFunction).L,(A1)	;22B90840EB3A
	MOVEA.L	(4).L,A6	;2C7900000004
	JSR	(_LVOPermit,A6)	;4EAEFF76
	MOVEA.L	(IntBase,PC),A1	;227A0088
	JSR	(_LVOCloseLibrary,A6)	;4EAEFE62
Exit	MOVEQ	#0,D0	;7000
	RTS	;4E75

NewCode	MOVEM.L	D0-D7/A0-A6,-(SP)	;48E7FFFE
	MOVE.L	D0,D6	;2C00
	MOVE.L	A1,D5	;2A09
	MOVEQ	#0,D4	;7800
	MOVE.W	A0,D4	;3808
	EXT.L	D4	;48C4
	NEG.L	D4	;4484
	MOVEA.L	(4).L,A6	;2C7900000004
	SUBA.L	A1,A1	;93C9
	JSR	(_LVOFindTask,A6)	;4EAEFEDA
	MOVEA.L	D0,A0	;2040
	MOVE.L	(10,A0),D0	;2028000A
	LEA	(OutputData,PC),A3	;47FA0124
	MOVE.L	D0,(A3)+	;26C0
	MOVE.L	D4,(A3)+	;26C4
	MOVEA.L	(4).W,A1	;22780004
	MOVEA.L	($017A,A1),A1	;2269017A
lbC0000C2	MOVE.L	A1,D0	;2009
	TST.L	D0	;4A80
	BEQ.W	Unknown	;6700000C
	CMP.L	A1,D5	;BA89
	BEQ.W	lbC0000DC	;6700000E
	MOVEA.L	(A1),A1	;2251
	BRA.B	lbC0000C2	;60EE

Unknown	LEA	(UNKNOWN.MSG,PC),A1	;43FA0116
	MOVE.L	A1,(A3)+	;26C9
	BRA.B	Request	;6004

lbC0000DC	MOVE.L	(10,A1),(A3)+	;26E9000A
Request	MOVE.L	D6,(A3)	;2686
	MOVEA.L	(IntBase,PC),A6	;2C7A002A
	MOVEQ	#0,D0	;7000
	MOVEA.L	D0,A0	;2040
	LEA	(lbL000124,PC),A1	;43FA0038
	MOVEA.L	#0,A2	;247C00000000
	LEA	(OutputData).L,A3	;47F90840EC08
	JSR	(_LVOEasyRequestArgs,A6)	;4EAEFDB4
	MOVEM.L	(SP)+,D0-D7/A0-A6	;4CDF7FFF
	MOVE.L	(OldFunction).L,-(SP)	;2F390840EB3A
	RTS	;4E75

OldFunction	dc.l	0	;00000000
IntBase	dc.l	0	;00000000
IntLibText	dc.b	'intuition.library',0	;696E74756974696F6E2E6C69627261727900
lbL000124	dc.l	$00000014,0	;0000001400000000
	dc.l	SetFunctionSn.MSG	;0840EB6D
	dc.l	Tasksattempst.MSG	;0840EB88
	dc.l	Ahaaha.MSG,$24564552	;0840EBFC24564552
	dc.b	$3A	;3A
SetFunctionSn.MSG	dc.b	'SetFunction()-Snooper V0.9',0	;53657446756E6374696F6E28292D536E6F6F7065722056302E3900
Tasksattempst.MSG	dc.b	'Task "%s" attemps to patch a library-function!',$A	;5461736B202225732220617474656D707320746F2070617463682061206C6962726172792D66756E6374696F6E210A
	dc.b	'Function-offset: -%ld',$A	;46756E6374696F6E2D6F66667365743A202D256C640A
	dc.b	'Library-name   : %s',$A	;4C6962726172792D6E616D652020203A2025730A
	dc.b	'Newfunction at : $%08lx',$A,0	;4E657766756E6374696F6E206174203A20242530386C780A00
	dc.b	0	;00
	dc.b	0	;00
Ahaaha.MSG	dc.b	'Aha aha...',0,0	;416861206168612E2E2E0000
OutputData	dc.l	0,0,0,0,0	;0000000000000000000000000000000000000000
UNKNOWN.MSG	dc.b	'UNKNOWN !!',0,0	;554E4B4E4F574E2021210000

	end
