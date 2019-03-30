vc_KickTagList	movem.l	d2/a2/a6,-(a7)
		move.l	4.w,a6
		move.l	550(a6),-(a7)
		move.l	a7,a1
		lea	.HeadText(pc),a0
		bsr	vc_PrintF
		addq.w	#4,a7

		move.l	550(a6),d0
		beq.s	.Empty
		move.l	d0,a2

		moveq	#0,d2
.Pointer	move.l	(a2)+,d0
		beq.s	.End
		bclr	#31,d0
		beq.s	.Tag
		move.l	d0,a2
		bra.s	.Pointer
.Tag		st	d2
		move.l	d0,a1
		bsr	vc_RomTag
		bra.s	.Pointer

.End		tst.w	d2
		bne.s	.Exit
.Empty		lea	vc_EmptyText(pc),a0
		bsr	vc_PrintF
.Exit		movem.l	(a7)+,d2/a2/a6
		rts

.HeadText	dc.b	10,'KickTag List (KickTagPtr: $%08lx)',10,10,0
		even
