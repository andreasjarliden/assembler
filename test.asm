	; Send 11xx (mode 3) | 1111 (set mode) to PIO Control A (IO addr 02h)
	ld a, 0ffh
	out (02h)
	; Follow the set mode command with 00h to PIO Control A to configure all 8 bits as outputs
	ld a, 00h
	out (02h)
loop:
	; Send FFh to PIO Data A
	ld a, 0ffh
	out (00h)
	; Send 00h to PIO Data A
	ld a, 00h
	out (00h)
	; TODO comments on line not working
	; TODO would like to do jp loop
	jp 8		
