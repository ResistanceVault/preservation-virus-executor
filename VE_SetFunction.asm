	section code

_LVOOpenLibrary			EQU	-$00000228
_LVOCloseLibrary		EQU	-$0000019E
_LVOEasyRequestArgs		EQU	-$0000024C

_LVOSetFunction			EQU	-$000001A4
_LVOForbid			EQU	-$00000084
_LVOCacheClearU			EQU	-$0000027C
_LVOPermit			EQU	-$0000008A
_LVOFindTask			EQU	-$00000126
****************************************************************************

	xdef _VE_SetFunction
	xdef _VE_RestoreFunction

; 	oldFunc = SetFunction (library, funcOffset, funcEntry)
;     	D0		          A1       A0.W	       D0

_VE_SetFunction
	move.l	a6,-(sp)
	move.l	4.w,a6

	lea	IntLibText(pc),a1
	move.l	#$25,D0
	jsr	_LVOOpenLibrary(a6)
	move.l	d0,(IntBase).l
	beq.w	Exit

	move.l	a6,a1
	move.l	#NewCode,d0
	move.w	#-$01a4,a0
	jsr	_LVOForbid(a6)
	jsr	-$01a4(a6)
	lea	OldFunction,a0
	move.l	d0,(a0)
	jsr	_LVOPermit(a6)
	jsr	_LVOCacheClearU(a6)
Exit
	move.l	(sp)+,a6
	rts

_VE_RestoreFunction
	move.l	a6,-(sp)
	move.l	4.w,a6
	move.l	a6,a1
	lea	OldFunction,a0
	move.l	(a0),d0
	move.w	#-$01a4,a0
	jsr	_LVOForbid(a6)
	jsr	-$01a4(a6)
	jsr	_LVOPermit(a6)

	move.l	IntBase(PC),a1
	jsr	_LVOCloseLibrary(a6)

	move.l	(sp)+,a6
	rts

NewCode	MOVEM.L	D0-D7/A0-A6,-(SP)
	MOVE.L	D0,D6	; Store d0
	MOVE.L	A1,D5	; Store a1
	MOVEQ	#0,D4	;
	MOVE.W	A0,D4	; Store a0
	EXT.L	D4
	NEG.L	D4

	MOVEA.L	(4).w,A6
	SUBA.L	A1,A1
	JSR	_LVOFindTask(A6)
	MOVEA.L	D0,A0
	MOVE.L	10(A0),d0	
	LEA	OutputData(PC),A3
	MOVE.L	D0,(A3)+	; Code
	MOVE.L	D4,(A3)+	; Offset
	MOVEA.L	4.w,A1
	MOVEA.L	$017A(A1),A1	?
lbC0000C2
	MOVE.L	A1,D0
	TST.L	D0
	BEQ.W	Unknown
	CMP.L	A1,D5
	BEQ.W	lbC0000DC
	MOVEA.L	(A1),A1
	BRA.B	lbC0000C2

Unknown	LEA	UNKNOWN.MSG(PC),A1
	MOVE.L	A1,(A3)+
	BRA.B	Request

lbC0000DC
	MOVE.L	10(A1),(A3)+

Request	MOVE.L	D6,(A3)	;2686
	MOVEA.L	IntBase(PC),A6
	MOVEQ	#0,D0
	MOVEA.L	D0,A0
	LEA	lbL000124(PC),A1
	MOVEA.L	#0,A2
	LEA	(OutputData).L,A3
	JSR	_LVOEasyRequestArgs(A6)
	MOVEM.L	(sp)+,D0-D7/A0-A6
	MOVE.L	(OldFunction).L,-(SP)
	RTS

OldFunction	dc.l	0
IntBase		dc.l	0
IntLibText	dc.b	'intuition.library',0

lbL000124	dc.l	$00000014,0
	dc.l	SetFunctionSn.MSG
	dc.l	Tasksattempst.MSG
	dc.l	Ahaaha.MSG,$24564552
	dc.b	$3A
SetFunctionSn.MSG	dc.b	'SetFunction()-Snooper V0.9',0
Tasksattempst.MSG	dc.b	'Task "%s" attemps to patch a library-function!',$A
	dc.b	'Function-offset: -%ld',$A
	dc.b	'Library-name   : %s',$A
	dc.b	'Newfunction at : $%08lx',$A,0
	dc.b	0	;00
	dc.b	0	;00
Ahaaha.MSG	dc.b	'Aha aha...',0,0
OutputData	dc.l	0,0,0,0,0
UNKNOWN.MSG	dc.b	'UNKNOWN !!',0,0

	end
