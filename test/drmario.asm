	SECTION "ROM0", HOME[0]

	INCBIN "drmario.gb",$0,$100-$0
	NOP
	JP jmp_150

	INCBIN "drmario.gb",$104,$150-$104
jmp_150:
	JP jmp_1e8
sub_153:
	LD BC,$D00E
	LD A,[BC]
	AND A
	JR NZ,jmp_167
	LDH A,[$CF]
	CP $FE
	JR NZ,jmp_164
	LD A,$01
	JR jmp_165
jmp_164:
	XOR A
jmp_165:
	LD [BC],A
	RET

jmp_167:
	LD A,[$D046]
	LDH [$01],A
	JR jmp_164

	INCBIN "drmario.gb",$16e,$1e8-$16e
jmp_1e8:
	XOR A
	LD HL,$DFFF
	LD C,$10
	LD B,$00
jmp_1f0:
	LD [HLD],A
	DEC B
	JR NZ,jmp_1f0
	DEC C
	JR NZ,jmp_1f0
	LD A,$0D
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
	LD [$D000],A
	LD [$D001],A
	LD A,$80
	LDH [$40],A
jmp_215:
	LDH A,[$44]
	CP $94
	JR NZ,jmp_215
	LD A,$03
	LDH [$40],A
	LD A,$E1
	LDH [$47],A
	LDH [$48],A
	LD A,$E5
	LDH [$49],A
	LD HL,$FF26
	LD A,$80
	LD [HLD],A
	LD A,$FF
	LD [HLD],A
	LD [HL],$77
	LD HL,$FF06
	LD A,$BF
	LD [HLI],A
	LD A,$04
	LD [HL],A
	LD A,$01
	NOP
	NOP
	NOP
	LD SP,$CFFF
	XOR A
	LD HL,$DFFF
	LD B,$00
jmp_24b:
	LD [HLD],A
	DEC B
	JR NZ,jmp_24b
	LD HL,$CFFF
	LD C,$10
	LD B,$00
jmp_256:
	LD [HLD],A
	DEC B
	JR NZ,jmp_256
	DEC C
	JR NZ,jmp_256
	LD HL,$9FFF
	LD C,$20
	XOR A
	LD B,$00
jmp_265:
	LD [HLD],A
	DEC B
	JR NZ,jmp_265
	DEC C
	JR NZ,jmp_265
	LD HL,$FEFF
	LD B,$00
jmp_271:
	LD [HLD],A
	DEC B
	JR NZ,jmp_271
	LD HL,$FFFE
	LD B,$80
jmp_27a:
	LD [HLD],A
	DEC B
	JR NZ,jmp_27a
	LD C,$B6
	LD B,$0A
	LD HL,$2386
jmp_285:
	LD A,[HLI]
	LD [C],A
	INC C
	DEC B
	JR NZ,jmp_285
	CALL sub_20a3
	CALL $7FF3
	CALL sub_20bb
	LD A,$0D
	LDH [$FF],A
	LD A,$80
	LDH [$40],A
	XOR A
	LDH [$0F],A
	LDH [$4A],A
	LDH [$4B],A
	LDH [$E1],A
	LDH [$9C],A
	EI
jmp_2a8:
	DI
	LD A,[$C4F1]
	AND A
	JR Z,jmp_2c0
	LDH A,[$CC]
	AND A
	LD A,$00
	LDH [$CC],A
	JR NZ,jmp_2bc
	LD A,$E0
	JR jmp_2be
jmp_2bc:
	LDH A,[$F2]
jmp_2be:
	LDH [$D0],A
jmp_2c0:
	EI
	CALL sub_434
	CALL sub_231c
	LD HL,$DF89
	LD A,[HL]
	AND A
	JR Z,jmp_2d3
	XOR A
	LD [HL],A
	CALL sub_3002
jmp_2d3:
	CALL $0455
	LDH A,[$E4]
	AND A
	JR NZ,jmp_33a
	LDH A,[$B1]
	AND A
	JR Z,jmp_311
	LD A,[$C4F1]
	AND A
	JR Z,jmp_2f1
	LDH A,[$AB]
	AND A
	JR NZ,jmp_2f1
	CALL sub_364
	CALL sub_3e4
jmp_2f1:
	LDH A,[$DE]
	LD [$DF8F],A
	AND A
	JR Z,jmp_305
	CP $01
	JR Z,jmp_309
	CP $02
	JR Z,jmp_30d
	LD A,$D8
	JR jmp_30f
jmp_305:
	LD A,$BF
	JR jmp_30f
jmp_309:
	LD A,$C8
	JR jmp_30f
jmp_30d:
	LD A,$D0
jmp_30f:
	LDH [$06],A
jmp_311:
	LDH A,[$CB]
	CP $60
	JR Z,jmp_33a
	LDH A,[$80]
	AND $0F
	CP $0F
	JR NZ,jmp_33a
	LDH A,[$B1]
	AND A
	JP Z,jmp_1e8
	RST $08
	RST $08
	XOR A
	LDH [$CC],A
	LD A,$F0
	LDH [$01],A
	LD A,$81
	LDH [$02],A
jmp_332:
	LDH A,[$CC]
	AND A
	JR Z,jmp_332
	JP jmp_1e8
jmp_33a:
	LD HL,$FFA6
	LD B,$02
jmp_33f:
	LD A,[HL]
	AND A
	JR Z,jmp_344
	DEC [HL]
jmp_344:
	INC L
	DEC B
	JR NZ,jmp_33f
	LD HL,$FFE2
	INC [HL]
	LD HL,$FFE5
	INC [HL]
	LD HL,$FFE6
	INC [HL]
	LD HL,$FFE7
	INC [HL]
jmp_358:
	NOP ; rgbasm could compile halt instruction properly
	LDH A,[$85]
	AND A
	JR Z,jmp_358
	XOR A
	LDH [$85],A
	JP jmp_2a8
sub_364:
	LDH A,[$DC]
	AND A
	JR NZ,jmp_3a3
	LDH A,[$D0]
	CP $FD
	JR Z,jmp_379
	CP $F8
	JR Z,jmp_394
	CP $55
	RET NC
	LDH [$D3],A
	RET

jmp_379:
	XOR A
	LDH [$D0],A
	LD A,$F8
	LDH [$F4],A
	LD B,$17
	LDH A,[$B1]
	AND A
	JR NZ,jmp_38c
	LD A,$01
	LDH [$A9],A
	RET

jmp_38c:
	LD A,$10
	LDH [$A6],A
	LD A,B
	LDH [$E1],A
	RET

jmp_394:
	LD A,$FD
	LDH [$F4],A
	LD B,$0F
	LDH A,[$B1]
	AND A
	JR Z,jmp_38c
	LD B,$17
	JR jmp_38c
jmp_3a3:
	LD A,[$C0A4]
	AND A
	RET Z
	CP $E0
	RET Z
	LD B,A
jmp_3ac:
	LD A,B
	AND $C0
	JR NZ,jmp_3b7
	SLA B
	SLA B
	JR jmp_3ac
jmp_3b7:
	LD C,B
	LDH A,[$D9]
	LD D,A
jmp_3bb:
	LD A,D
	AND $C0
	JR NZ,jmp_3d1
jmp_3c0:
	LD A,C
	AND $C0
	JR Z,jmp_3de
	SLA C
	RL D
	SLA C
	RL D
	LD A,C
	AND A
	JR NZ,jmp_3bb
jmp_3d1:
	LD A,D
	LDH [$D9],A
	LD [$D016],A
	XOR A
	LDH [$DC],A
	LD [$C0A4],A
	RET

jmp_3de:
	SLA C
	SLA C
	JR jmp_3c0
sub_3e4:
	LD DE,$D008
	LD HL,$FFDD
	LD A,[HL]
	CP $02
	JR Z,jmp_416
	CP $01
	JR Z,jmp_409
	LD A,[DE]
	AND A
	RET NZ
	LD A,[$D00E]
	AND A
	JR NZ,jmp_400
	LDH A,[$D1]
	JR jmp_403
jmp_400:
	LD A,[$D046]
jmp_403:
	LDH [$CF],A
	LD A,[DE]
	INC A
	LD [DE],A
	RET

jmp_409:
	LD A,[DE]
	AND A
	RET NZ
	LD A,$FE
	LDH [$CF],A
	LD A,[DE]
	INC A
	LD [DE],A
	LD [HL],$02
	RET

jmp_416:
	LD A,[DE]
	AND A
	RET NZ
	LDH A,[$D8]
	LDH [$CF],A
	LD [$D046],A
	XOR A
	LDH [$D8],A
	LD [HL],A
	INC A
	LD [DE],A
	LDH A,[$CB]
	CP $60
	LD A,$0B
	JR Z,jmp_430
	LD A,$08
jmp_430:
	LD [$DFE0],A
	RET

sub_434:
	LD A,[$D03A]
	AND A
	RET NZ
	LDH A,[$CB]
	CP $30
	RET NZ
	LDH A,[$CF]
	LDH [$01],A
	CALL sub_153
	XOR A
	LD [$D008],A
	LD A,[$C4F1]
	AND A
	JR NZ,jmp_450
	RST $08
jmp_450:
	LD A,$81
	LDH [$02],A
	RET

	INCBIN "drmario.gb",$455,$1e40-$455
sub_1e40:
	LD HL,$C000
	LD B,$A0
	XOR A
jmp_1e46:
	LD [HLI],A
	DEC B
	JR NZ,jmp_1e46
	RET


	INCBIN "drmario.gb",$1e4b,$20a3-$1e4b
sub_20a3:
	LD HL,$9BFF
	LD BC,$0400
jmp_20a9:
	LD A,$FF
	LD [HLD],A
	DEC BC
	LD A,B
	OR C
	JR NZ,jmp_20a9
	RET

jmp_20b2:
sub_20b2:
	LD A,[HLI]
	LD [DE],A
	INC DE
	DEC BC
	LD A,B
	OR C
	JR NZ,jmp_20b2
	RET

sub_20bb:
	LD HL,$3D9E
	LD DE,$8000
	LD BC,$17FF
	CALL sub_20b2
	RET


	INCBIN "drmario.gb",$20c8,$20c9-$20c8
sub_20c9:
	LD HL,$9800
	LD B,$12
jmp_20ce:
	PUSH HL
	LD C,$14
jmp_20d1:
	LD A,[DE]
	LD [HLI],A
	INC DE
	DEC C
	JR NZ,jmp_20d1
	POP HL
	PUSH DE
	LD DE,$0020
	ADD HL,DE
	POP DE
	DEC B
	JR NZ,jmp_20ce
	RET


	INCBIN "drmario.gb",$20e2,$231c-$20e2
sub_231c:
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


	INCBIN "drmario.gb",$2359,$3002-$2359
sub_3002:
	LDH A,[$E4]
	AND A
	JR NZ,jmp_3013
	LD B,$01
	LDH A,[$C1]
	AND A
	JR Z,jmp_3019
	INC B
	CP $01
	JR Z,jmp_3019
jmp_3013:
	LD A,$07
	LD [$DFE8],A
	RET

jmp_3019:
	LD A,B
	LD [$DFE8],A
	RET
	
	INCBIN "drmario.gb",$301E,$4000-$301E

SECTION "bank1",DATA,BANK[$1]
INCBIN "drmario.gb",$4000,$4000

