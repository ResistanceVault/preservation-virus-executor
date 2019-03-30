	section code

_LVOSetFunction			EQU	-$000001A4
_LVOForbid			EQU	-$00000084
_LVOCacheClearU			EQU	-$0000027C
_LVOPermit			EQU	-$0000008A
_LVOFindTask			EQU	-$00000126
****************************************************************************

	xref _FuncOutput
	xdef _VE_SetFunction
; 	oldFunc = SetFunction (library, funcOffset, funcEntry)
;     	D0		          A1       A0.W	       D0

_VE_SetFunction
	move.l	4.w,a6
	move.l	a6,a1
	move.l	#NewCode,d0
	move.w	#-$01a4,a0
	jsr	_LVOForbid(a6)
	jsr	-$01a4(a6)
	lea	OldFunction,a0
	move.l	d0,(a0)
	jsr	_LVOPermit(a6)
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
Request

	LEA	OutputData.l,A3
	jsr	(_FuncOutput)

	MOVEM.L	(SP)+,D0-D7/A0-A6
	MOVE.L	(OldFunction).L,-(SP)
	RTS

OldFunction	dc.l	0
OutputData	dc.l	0,0,0,0,0
UNKNOWN.MSG	dc.b	'UNKNOWN !!',0,0

	end
