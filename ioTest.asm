.eq PIO_A_DATA 00h
.eq PIO_A_CONTROL 02h
.eq PIO_B_DATA 01h
.eq PIO_B_CONTROL 03h
.eq PIO_SET_INPUT_MODE 4fh
.eq PIO_SET_BIT_MODE 0cfh
.eq PIO_ENABLE_INTERRUPTS 83h
.eq PIO_DISABLE_INTERRUPTS 03h
	; Configure PIO A in mode 1 byte input
	ld a, PIO_SET_INPUT_MODE
	out (PIO_A_CONTROL)
	; Configure PIO B in Bit Mode 3, All Outputs
	ld a, PIO_SET_BIT_MODE	; Send 11xx (mode 3) | 1111 (set mode) to PIO Control A (IO addr 02h)
	out (PIO_B_CONTROL)
	ld a, 00h	; Follow the set mode command with 00h to PIO Control A to configure all 8 bits as outputs
	out (PIO_B_CONTROL)
	; Enable interrupts for Port A
	ld a, PIO_ENABLE_INTERRUPTS
	out (PIO_A_CONTROL)

	; Disable interrupts for Port B
	ld a, PIO_DISABLE_INTERRUPTS
	out (PIO_B_CONTROL)

	in (PIO_A_DATA)	; Read from PIO A to activate the ready line

	im 1		; Interrupt mode 1 (Maskeble interrupts reset to 0038h)
	ei		; Enable interrupts

loop:
	halt
	jp loop		; Interrupts are expected to return after the halt so we must loop

