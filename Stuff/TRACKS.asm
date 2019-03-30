
.OpenDosLib	lea	doslibraryMsg(PC),a1
		jsr	-$60(a6)
		tst.l	d0
		beq.s	lbC00003C
		move.l	D0,A0
		move.l	$16(A0),A0
		moveq	#0,D0
		rts

lbC00003C	moveq	#-$1,D0
		rts

doslibraryMsg	dc.b	'dos.library',0
