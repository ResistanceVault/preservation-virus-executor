;Work:sc/c/asm Freq.asm
;oml LIB:JEO.lib r Freq.o

	SECTION CODE

	xdef _Freq

_Freq
	move.w	#8,d1		
start	move.w	#$fff,d0	; Inner loop
loop	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	dbf	d0,loop
	dbf	d1,start
	rts

	END
