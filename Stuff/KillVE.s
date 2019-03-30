����                                        Forbid		equ -132
Permit		equ -138
PutMsg		equ -366
CacheClearU	equ -636
AllocVec	equ -684
FreeVec		equ -690
ColdReboot	equ -726
DebugData	equ 70
ThisTask	equ 276
AttnFlags	equ 296
ResModules	equ 300
DeviceList	equ 350
LibList		equ 378
TaskReady	equ 406
TaskWait	equ 420


	moveq	#0,d1
	move.l	4.w,a6
	jsr	Forbid(a6)
	move.l	TaskWait(a6),a0
	bsr.s	_find_Executor
	move.l	TaskReady(a6),a0
	bsr.s	_find_Executor
	jsr	Permit(a6)
	rts	

_find_Executor
	move.l	10(a0),a2
	lea	background_name(pc),a1
	
check_Background_name
	cmpm.b	(a2)+,(a1)+
	bne.s	_get_next_task2
	tst.b	-1(a1)
	bne.s	check_Background_name
	bsr.w	kill_Executor

_get_next_task2
	tst.l	(a0)
	beq.s	_no_more_tasks
	move.l	(a0),a0
	bra.b	_find_Executor

_no_more_tasks
	bsr.w	_CacheClearU
	rts		


kill_Executor
	move.l	62(a0),a1
	move.l	-8(a1),a1
	lea	Executor_code(pc),a2
	moveq	#[(end_Executor_code-Executor_code)/2]-1,d0
;	moveq	#4,d0
compare_Executor_code
	cmpm.w	(a2)+,(a1)+
	bne.s	not_Executor
	dbf	d0,compare_Executor_code
	clr.b	15(a0)			;set tc_State to INVALID
	clr.l	22(a0)			;clear all signals
	moveq	#1,d1
;	bra.s	mark_killed_task
not_Executor
	rts

Executor_code
	moveq	#0,d0
	bra.s	_shit
	move.l	4(a7),d0
_shit	move.l	$1208(a4),a2
	move.l	d0,-(a2)
	move.l	-$1828(a4),$1204(a4)
	move.l	4.w,a6
	subq.l	#4,-$1820(a4)
end_Executor_code
background_name	dc.b	'Background CLI',0
	EVEN

_CacheClearU
	movem.l	d0/d1/a0/a1/a6,-(sp)
	move.l	4.w,a6
	jsr	CacheClearU(a6)
	movem.l	(sp)+,d0/d1/a0/a1/a6
	rts

