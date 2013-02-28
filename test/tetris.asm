; ROM: tetris.gb (MD5: 982ed5d2b12a0377eb14bcdc4123744e)
; cmd: gb-disasm tetris.gb -s 2041 -nc -a > tetris.asm
; -s  -> start address, determined through bgb debugger
; -nc -> don't follow call instructions
; -a  -> print assembly

SECTION "rom0", HOME[0]

	INCBIN "tetris.gb",$0,$2041-$0
genRandom:
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

	INCBIN "tetris.gb",$2071,$4000-$2071

SECTION "bank1",DATA,BANK[$1]
	INCBIN "tetris.gb",$4000,$4000
