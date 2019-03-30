*****************************************************************
* -> a0: buffer	  <- a0: new
* -> d0: hunk #	  <- d0: new pos
* -> d1: buflen   <- d1: errorcode

	XDEF	_GetHunkX


_GetHunkX	movem.l	d2-d4/a2,-(a7)
		move.l	a0,d3
		movea.l	a0,a1
		lea	(a0,d1.l),a2
		move.l	(16,a0),d1
		addq.l	#1,d1
		sub.l	(12,a0),d1
		move.w	d1,d2
		lsl.l	#2,d1
		lea	(20,a0,d1.l),a0

.NextHunk	cmpa.l	a2,a0
		bcc	.Exit0
		move.l	(a0),d1
		cmpi.w	#$3e9,d1
		beq.b	.Code
		cmpi.w	#$3ea,d1
		bne.b	.NoCode
.Code		subq.w	#1,d0
		bmi	.CodeOk
		subq.w	#1,d2
		beq.b	.NoMoreHunks
.Debug		addq.w	#4,a0
		move.l	(a0)+,d1
		lsl.l	#2,d1
		adda.l	d1,a0
		bra.b	.NextHunk

.NoCode		cmpi.w	#$3eb,d1
		bne.b	.NoBSS
		subq.w	#1,d0
		bmi.b	.BSSOk
		subq.w	#1,d2
		bne.b	.BSS
.NoMoreHunks	moveq	#-1,d0
		bra	.Truncated
.BSS		addq.w	#8,a0
		bra.b	.NextHunk

.NoBSS		cmpi.w	#$3ec,d1
		beq.b	.Reloc
		cmpi.w	#$3f0,d1
		bne.b	.NoReloc
.Reloc		addq.w	#4,a0
.NextReloc	cmpa.l	a2,a0
		bcc.b	.Exit0
		move.l	(a0)+,d1
		beq.b	.NextHunk
		lsl.l	#2,d1
		lea	(4,a0,d1.l),a0
		bra.b	.NextReloc

.NoReloc	cmpi.w	#$3f7,d1
		beq.b	.RelocS
		cmpi.w	#$3fc,d1
		bne.b	.NoRelocS
.RelocS		addq.w	#4,a0
.NextRelocS	cmpa.l	a2,a0
		bcc.b	.Exit0
		moveq	#0,d1
		move.w	(a0)+,d1
		beq.b	.RelocSPad
		add.l	d1,d1
		lea	(2,a0,d1.l),a0
		bra.b	.NextRelocS
.RelocSPad	move.l	a0,d1
		sub.l	a1,d1
		andi.w	#$2,d1
		beq.b	.NoOdd
		addq.w	#2,a0
.NoOdd		bra	.NextHunk

.NoRelocS	cmpi.w	#$3e8,d1
		beq.b	.Debug
		cmpi.w	#$3f1,d1
		beq.b	.Debug
		cmpi.w	#$3f2,d1
		beq.b	.End
		moveq	#-1,d1		;#XVSERR_UNSUPPORTEDHUNK,d1
		moveq	#0,d0
		bra.b	.Exit
.End		addq.w	#4,a0
		bra	.NextHunk

.BSSOk		moveq	#0,d0
		bra.b	.BothOk
.CodeOk		move.l	(4,a0),d0
		lsl.l	#2,d0
.BothOk		lea	(8,a0,d0.l),a1	;code ende
		moveq	#1,d0
		cmpa.l	a2,a1
		bcs.b	.Exit

.Exit0		moveq	#0,d0
.Truncated	moveq	#-2,d1		;#XVSERR_FILETRUNCATED,d1
.Exit
		cmp.b	#1,d0
		bne.b	.end
		move.l	a0,d4
		sub.l	d3,d4
		move.l	d4,d0
		sub.l	#4,d0
.end
		movem.l	(a7)+,d2-d4/a2
		rts

	END
