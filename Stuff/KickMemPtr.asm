vc_KickMemList	movem.l	d2/a2/a3/a6,-(a7)

.OpenDosLib	lea	doslibraryMsg(PC),a1
		move.l	#0,d0
		move.l	4.w,a6
		jsr	-408(a6)
		tst.l	d0
		beq.s	.Exit
		move.l	d0,DosBase

		move.l	546(a6),-(a7)
		move.l	a7,a1
		lea	.HeadText(pc),a0
		bsr	CliPrintF
		addq.w	#4,a7

		move.l	546(a6),d0
		bne.s	.ListNode
		lea	vc_EmptyText(pc),a0
		bsr	CliPrintF
		bra.s	.Exit

.ListNode	move.l	d0,a2
                move.w	14(a2),d2
		move.w	d2,-(a7)
		move.l	a2,-(a7)		
		move.l	a7,a1
		lea	.ListHeadText(pc),a0
		bsr	CliPrintF
		addq.w	#6,a7

		tst.w	d2
		beq.s	.EmptyNode
		lea	16(a2),a3
.ListEntries	move.l	4(a3),-(a7)
		move.l	(a3),-(a7)
		addq.w	#8,a3				
		move.l	a7,a1
		lea	.ListEntryText(pc),a0
		bsr	CliPrintF
		addq.w	#8,a7
                subq.w	#1,d2
		bne.s	.ListEntries

.EmptyNode	move.l	(a2),d0
		bne.s	.ListNode

.Exit		movem.l	(a7)+,d2/a2/a3/a6
		rts

.HeadText	dc.b	10,'KickMem List (KickMemPtr: $%08lx)',10,10,0
.ListHeadText	dc.b	'  List at $%08lx has %d entries',10,0
.ListEntryText	dc.b	'    $%08lx: %ld bytes',10,0
vc_EmptyText	dc.b	'  Empty',10,0

.HeadText	dc.b	'Capture Vectors in RAM',10,10,0
.ColdText	dc.b	'  ColdCapture: $%08lx %s',10,0
.CoolText	dc.b	'  CoolCapture: $%08lx %s',10,0
.WarmText	dc.b	'  WarmCapture: $%08lx %s',10,0
vc_NoneText	dc.b	'  None',10,0
vc_OkText	dc.b	'OK',0
vc_UnknVecText	dc.b	'*** NON-STANDARD VECTOR ***',0
		even

doslibraryMsg	dc.b	'dos.library',0
DosBase		dc.l	0
vc_HideOkFlag	dc.b	0
vc_Reset1200Msg	dc.b	'Reset1200 by Carsten Sørensen',0

		even

CliPrintF	movem.l	d2/a6,-(a7)
		move.l	a0,d1
		move.l	a1,d2
		move.l	DosBase,a6
		jsr	-954(a6)
		movem.l	(a7)+,d2/a6
		rts
