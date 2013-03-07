SECTION "rom0", HOME[0]

    ; 0D54 is change level routine

    INCBIN "mario.gb",$0,$1c3c-$0
    ; setting life number routine
    ; gb-disasm mario.gb -nc -s 1C3C -a
	CP $FF
	LD A,[$DA15]
	JR Z,jmp_1c75
	CP $99
	JR Z,jmp_1c67
	PUSH AF
	LD A,$08
	LD [$DFE0],A
	LDH [$D3],A
	POP AF
	; was ADD A,$01, changed to 2xNOP
	; let's do not increase life number if "live up" item found
	NOP
	NOP
jmp_1c52:
	; DAA was here (check meaning in google), changed to NOP
	NOP
	LD [$DA15],A ; 0xDA15 is place where lives number is stored
	LD A,[$DA15]
	LD B,A
	AND $0F
	LD [$9807],A
	LD A,B
	AND $F0
	SWAP A
	LD [$9806],A
jmp_1c67:
	XOR A
	LD [$C0A3],A
	RET
jmp_1c6c:
	LD A,$39
	LDH [$B3],A
	LD [$C0A4],A
	JR jmp_1c67
jmp_1c75:
	AND A
	JR Z,jmp_1c6c
    ; SUB $01 was here, changed to 2x NOP
	; let's do not decrease the lives number
	NOP
	NOP
	JR jmp_1c52

	INCBIN "mario.gb",$1c7c,$3d48-$1c7c
	; initial lives count, default 2, lets change to FF
	; gb-disasm mario.gb -nc -nj -s 3D48 -e 3D4B -a
	LD A,$FF
	LD [HLI],A

	INCBIN "mario.gb",$3d4b,$4000-$3d4b

SECTION "bank1",DATA,BANK[$1]
	INCBIN "mario.gb",$4000,$4000
	
SECTION "bank2",DATA,BANK[$2]
	INCBIN "mario.gb",$8000,$4000
	
SECTION "bank3",DATA,BANK[$3]
	INCBIN "mario.gb",$C000,$4000
