#!/usr/bin/env python2

# This is unfinished script to convert Mr.Do! source code to format acceptable by rgbasm


import re

f = open('mrdo.asm', 'r')
lines = f.readlines()
f.close()

m = {}
defs = {}

stupid_names = { 'DIV': 'DIVV', 'IF': 'IFF', 'L': 'LEFT', 'D': 'DOWN' }
KEYWORDS = ['LDH', 'CPL', 'SUB', 'DI', 'RRCA', 'RES', 'STOP', 'JR', 'POP', 'EI', 'CCF',
    'LD', 'DEC', 'DAA', 'RR', 'RETI', 'SCF', 'RRC', 'RRA', 'RET', 'SWAP', 'PUSH', 'BIT', 
    'RLCA', 'SRA', 'XOR', 'HALT', 'JP', 'SRL', 'RL', 'ADD', 'SET', 'ADC', 'CP', 'SLA', 
    'AND', 'RLA', 'RLC', 'SBC', 'CALL', 'RST', 'NOP', 'OR', 'INC', 'DB', 'DW', 'DS', 
    'HEX', 'ORG', 'END' ]

wram = False
for line in lines:
    line = re.sub(r'\s+', ' ', line.strip()) # remove whitespaces
    #line = re.sub(r';.*', '', line) # remove comments ;
    line = re.sub(r'\*.*', '', line) # remove comments *

    if len(line) == 0:
        continue        
	
    # reserved variable names
    for name in stupid_names:
        if line.startswith(name + ' EQU '):
            line = line.replace(name + ' EQU ', stupid_names[name] + ' EQU ')
        elif '(' + name + ')' in line:
            line = line.replace('(' + name + ')', '(' +stupid_names[name] + ')')

    # Z80 synthax
    line = line.replace('DEFB ', 'DB ')
    line = line.replace('DEFW ', 'DW ')	
    line = line.replace('DEFS ', 'DS ')
	
    # HEX
    if 'HEX ' in line:
        sp = line.split(' ')
        for i in range(0, len(sp)):
            if sp[i] == 'HEX':
                a = 'DB '
                for j in range(0, (len(sp[i+1])/2) - 1):
                    a += '$' + sp[i+1][j*2:j*2+2] + ','
                a += '$' + sp[i+1][len(sp[i+1])-2:len(sp[i+1])]
                line = a
		
    if line.startswith('DB'):
        line = line.replace("D+L", "DOWN+LEFT")
        line = line.replace(",L", ",LEFT")
        line = line.replace(",D", ",LEFT")
	
    # labels	
    first = line.split(' ')[0]
    if ' EQU ' in line:
        defs[first] = line.split(' ')[2]
    elif not first in KEYWORDS:
        line = line.replace(first, first + ':')
		
	# () to []
    line = line.replace('(', '[')
    line = line.replace(')', ']')
	
    # wtf?
    if '<' in line or '>' in line:
        line = ';' + line + ' ; WTF?'

    if line.startswith('ORG WORKRAM+'):
        line = 'SECTION "' + line + '",BSS[' + line.replace('ORG WORKRAM+', '') + ']'
        wram = True
    elif line.startswith('ORG '):
        line = 'SECTION "' + line + '",HOME[' + line.split(' ')[1] + ']'
        wram = False
    if not wram:
        print line
