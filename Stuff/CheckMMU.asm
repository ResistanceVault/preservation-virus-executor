
_LVOReschedule	EQU	-$30
_LVOCloseLibrary	EQU	-$19E
_LVODispatch	EQU	-$3C
_LVOOpenLibrary	EQU	-$228
**********************************************************************
	SECTION CheckMMU000000,CODE
ProgStart
	MOVE.L	4,A6
	LEA	name3(PC),A1
	MOVEQ	#0,D0
	JSR	_LVOOpenLibrary(A6)
	MOVE.L	D0,store2
	LEA	store(PC),A2
	MOVE.L	4,A0
	ADD.L	#$142,A0
	MOVE.L	(A0),A1
	MOVE.L	A1,A0
loop	MOVE.L	(A0),D0
	BEQ	fine
	MOVE.L	$14(A0),D1
	MOVE.L	$18(A0),D2
	MOVE.L	D1,(A2)+
	SUB.L	D1,D2
	ASR.L	#1,D2
	MOVE.L	D2,(A2)+
	MOVE.L	D0,A0
	BRA	loop

fine	MOVE.W	$F80000,D3
	LEA	store(PC),A0
loop3	MOVE.L	(A0)+,A1
	MOVE.L	(A0)+,D0
	BEQ	spicca
loop2	MOVE.W	(A1)+,D1
	CMP.W	D3,D1
	BEQ	iva
rep	SUB.L	#1,D0
	BNE	loop2
	BRA	loop3

spicca	MOVE.L	store2(PC),A6
	JSR	_LVODispatch(A6)
	MOVE.L	D0,D1
	MOVE.L	#$35,D3
	LEA	name4(PC),A0
	MOVE.L	A0,D2
	JSR	_LVOReschedule(A6)
	BRA	spicc

iva	MOVE.W	(A1),D1
	MOVE.W	$F80002,D4
	CMP.W	D4,D1
	BNE	rep
	SUB.L	#2,A1
	MOVE.L	A1,store3
	CLR.L	(A1)
	MOVE.L	store2(PC),A6
	JSR	_LVODispatch(A6)
	MOVE.L	D0,D1
	MOVE.L	$F80000,D0
	BEQ	mmuok
	LEA	name2(PC),A0
	BRA	conti

mmuok	LEA	name1(PC),A0
conti	MOVE.L	A0,D2
	MOVE.L	store3(PC),A1
	MOVE.L	$F80000,D3
	MOVE.L	D3,(A1)
	MOVE.L	#$23,D3
	JSR	_LVOReschedule(A6)
spicc	MOVE.L	4,A6
	MOVE.L	store2(PC),A1
	JSR	_LVOCloseLibrary(A6)
	CLR.L	D0
	RTS

store3	dc.l	0
store2	dc.l	0
store	dc.l	0
	dc.l	0
	dc.l	0
	dc.l	0
	dc.l	0
	dc.l	0
	dc.l	0
	dc.l	0
	dc.l	0
	dc.l	0
name1	dc.b	'MMU is present and working 100%.',$D,$A,0
name2	dc.b	'You don''t have an MMU.          ',$D,$A,0
name3	dc.b	'dos.library',0
name4	dc.b	'MMU not active, start it with CPU FASTROM command.',$D,$A,0
name5	dc.b	0
	dc.b	0
	dc.b	0
	END
