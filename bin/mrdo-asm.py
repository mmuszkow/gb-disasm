#!/usr/bin/python

import re

f = open('mrdo.asm', 'r')
lines = f.readlines()
f.close()

m = {}
for line in lines:
    line = re.sub(r'\s+', ' ', line.strip()) # remove whitespaces
    line = re.sub(r';.*', '', line) # remove comments ;
    line = re.sub(r'\*.*', '', line) # remove comments *

    if len(line) == 0:
        continue        
    
    if 'ORG' in line:
        print line
