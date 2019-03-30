����                                        ; 	oldFunc = SetFunction (library, funcOffset, funcEntry)
;     	D0	                A1       A0.W	       D0

InstallPatch
	move.l	4.w,a6
	movea.w	#$fe5c,a0
	move.l	#PatchCode,d0
	move.l	a6,a1
	jsr	(-$1a4,a6)
	move.l	d0,oldFunc
	rts
	
;****************************************************************************

PatchCode
	bra.b	Start

Hebbe	dc.b	'VirusExecutor Patch',0

Start	movem.l	d0-d7/a0-a6,-(sp)
	incbin	"-v:VE_PatchCode.o"

	movem.l	(sp)+,d0-d7/a0-a6
	rts

;****************************************************************************

RemovePatch
	move.l	4.w,a6
	movea.w	#$fe5c,a0
	move.l	#oldFunc,d0
	move.l	a6,a1
	jsr	(-$1a4,a6)
	rts

oldFunc	dc.l	0

