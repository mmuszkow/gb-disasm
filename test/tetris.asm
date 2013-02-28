; ROM: tetris.gb
; look at genRandom

	SECTION "ROM0", HOME[0]

	INCBIN "tetris.gb",$0,$100-$0
	NOP
	JP jmp_150

	INCBIN "tetris.gb",$104,$150-$104
jmp_150:
	JP jmp_20c

	INCBIN "tetris.gb",$153,$20c-$153
jmp_20c:
	XOR A
	LD HL,$DFFF
	LD C,$10
	LD B,$00
jmp_214:
	LD [HLD],A
	DEC B
	JR NZ,jmp_214
	DEC C
	JR NZ,jmp_214
jmp_21b:
	LD A,$01
	DI
	LDH [$0F],A
	LDH [$FF],A
	XOR A
	LDH [$42],A
	LDH [$43],A
	LDH [$A4],A
	LDH [$41],A
	LDH [$01],A
	LDH [$02],A
	LD A,$80
	LDH [$40],A
jmp_233:
	LDH A,[$44]
	CP $94
	JR NZ,jmp_233
	LD A,$03
	LDH [$40],A
	LD A,$E4
	LDH [$47],A
	LDH [$48],A
	LD A,$C4
	LDH [$49],A
	LD HL,$FF26
	LD A,$80
	LD [HLD],A
	LD A,$FF
	LD [HLD],A
	LD [HL],$77
	LD A,$01
	LD [$2000],A
	LD SP,$CFFF
	XOR A
	LD HL,$DFFF
	LD B,$00
jmp_260:
	LD [HLD],A
	DEC B
	JR NZ,jmp_260
	LD HL,$CFFF
	LD C,$10
	LD B,$00
jmp_26b:
	LD [HLD],A
	DEC B
	JR NZ,jmp_26b
	DEC C
	JR NZ,jmp_26b
	LD HL,$9FFF
	LD C,$20
	XOR A
	LD B,$00
jmp_27a:
	LD [HLD],A
	DEC B
	JR NZ,jmp_27a
	DEC C
	JR NZ,jmp_27a
	LD HL,$FEFF
	LD B,$00
jmp_286:
	LD [HLD],A
	DEC B
	JR NZ,jmp_286
	LD HL,$FFFE
	LD B,$80
jmp_28f:
	LD [HLD],A
	DEC B
	JR NZ,jmp_28f
	LD C,$B6
	LD B,$0C
	LD HL,$2A7F
jmp_29a:
	LD A,[HLI]
	LD [C],A
	INC C
	DEC B
	JR NZ,jmp_29a
	CALL sub_2795
	CALL $7FF3
	LD A,$09
	LDH [$FF],A
	LD A,$37
	LDH [$C0],A
	LD A,$1C
	LDH [$C1],A
	LD A,$24
	LDH [$E1],A
	LD A,$80
	LDH [$40],A
	EI
	XOR A
	LDH [$0F],A
	LDH [$4A],A
	LDH [$4B],A
	LDH [$06],A
jmp_2c4:
	CALL sub_29a6
	CALL $2f8 ; TODO: check this call
	CALL $7FF0
	LDH A,[$80]
	AND $0F
	CP $0F
	JP Z,jmp_21b
	LD HL,$FFA6
	LD B,$02
jmp_2db:
	LD A,[HL]
	AND A
	JR Z,jmp_2e0
	DEC [HL]
jmp_2e0:
	INC L
	DEC B
	JR NZ,jmp_2db
	LDH A,[$C5]
	AND A
	JR Z,jmp_2ed
	LD A,$09
	LDH [$FF],A
jmp_2ed:
	LDH A,[$85]
	AND A
	JR Z,jmp_2ed
	XOR A
	LDH [$85],A
	JP jmp_2c4
	
    INCBIN "tetris.gb",$2f8,$2041-$2f8
genRandom: ; 0x2041
	LD H,$03
rng_seed:
	LDH A,[$04] ; div timer
	LD B,A
rng_loop: ; (a%7)*4
	XOR A
rng_seven:
	DEC B
	JR Z,rng_next
	INC A
	INC A
	INC A
	INC A
	CP $1C
	JR Z,rng_loop
	JR rng_seven
rng_next: ; avoid generating same number twice
	LD D,A
	LDH A,[$AE]
	LD E,A
	DEC H
	JR Z,rng_end
	OR D
	OR C
	AND $FC
	CP C
	JR Z,rng_seed
rng_end:
	;LD A,D ; let's break the generator :)
	LD A, $8
	LDH [$AE],A
	;LD A,E
	LD [$C213],A
	CALL $2696
	LDH A,[$9A]
	LDH [$99],A
	RET
	
INCBIN "tetris.gb",$2071,$2795-$2071

sub_2795:
	LD HL,$9BFF
	LD BC,$0400
jmp_279b:
	LD A,$2F
	LD [HLD],A
	DEC BC
	LD A,B
	OR C
	JR NZ,jmp_279b
	RET


	INCBIN "tetris.gb",$27a4,$29a6-$27a4
sub_29a6:
	LD A,$20
	LDH [$00],A
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	CPL
	AND $0F
	SWAP A
	LD B,A
	LD A,$10
	LDH [$00],A
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	LDH A,[$00]
	CPL
	AND $0F
	OR B
	LD C,A
	LDH A,[$80]
	XOR C
	AND C
	LDH [$81],A
	LD A,C
	LDH [$80],A
	LD A,$30
	LDH [$00],A
	RET

INCBIN "tetris.gb",$29E3,$4000-$29E3

SECTION "bank1",DATA,BANK[$1]
INCBIN "tetris.gb",$4000,$4000
