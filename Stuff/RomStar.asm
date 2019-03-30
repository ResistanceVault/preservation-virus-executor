	lea.l	ROMEND.l,a0
	move.l	ROMSIZE(a0),d1
	sub.l	d1,a0	       ;rom start
	move.l	a0,d0	
      	rts

ROMEND     EQU         $1000000       
ROMSIZE    EQU         -$14            

           END
