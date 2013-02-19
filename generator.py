#!/usr/bin/python
# Generater code from opcodes from http://pastraiser.com/cpu/gameboy/gameboy_opcodes.html

import re
from HTMLParser import HTMLParser

f = open('opcodes.html', 'r')
lines = f.read()
f.close()

class MyHTMLParser(HTMLParser):    
    def __init__(self):
        HTMLParser.__init__(self)
        self.td = 0
        self.last = []
        self.col = 0
        self.out = []

    def handle_starttag(self, tag, attrs):
        if(tag == 'td'):
            self.td = 1
            self.col += 1
        elif(tag == 'table'):
            self.col = 0

    def handle_endtag(self, tag):
        if(tag == 'td'):
            self.td = 0
            if(self.col > 1):
                self.out.append(self.last)
            self.last = []
        elif(tag == 'tr'):
            self.col = 0

    def handle_data(self, data):
        if(self.td == 1):
            self.last.append(data)

parser = MyHTMLParser()
parser.feed(lines)

# variables
VARS = ['d8', 'd16', 'a8', 'a16', 'r8']

for i in range(0, 0x100):
    op = parser.out[i+0x10]
    if(len(op) == 0): # not used
        continue
    elif(i == 0xCB):
        print '/* bit operations */'
        print 'case 0xcb:'
        print '\tswitch(r->raw[phy(pc+1)]) {'
        for j in range(0, 0x100):
            op2 = parser.out[j+0x120][0]
            print '\t/* %s */' % op2
            print '\tcase ' + hex(j) + ':'
            print '\t\tsops = sops_add(sops, op_0_2("%s"));' % op2
            print '\t\tbreak;'
        print '\t}'
        print '\tpc += 2;'
        print '\tbreak;'
        continue
        
    print '/* ' + op[0] + ' */' # op[0] = operation description
    print 'case ' + hex(i) + ':'
    if(op[1] == '1'): # 1-byte ops
        name = op[0]
        print '\tsops = sops_add(sops, op_0("%s"));' % name
    elif(op[1] == '2'): # 2-byte ops
        print '\taddr8 = r->raw[phy(pc+1)];'
        for var in VARS:
            bvar = '(' + var + ')'
            if op[0].endswith(var):
                name = op[0][0:len(op[0])-len(var)-1]
                print '\tsops = sops_add(sops, op_r("%s", addr8));' % name
                break
            elif op[0].endswith(bvar):
                name = op[0][0:len(op[0])-len(bvar)-1]
                print '\tsops = sops_add(sops, op_rb("%s", addr8));' % name
                break
            elif bvar in op[0]:
                where = op[0].find(bvar)
                name = op[0][0:where].strip()
                r = op[0][where+len(bvar):].strip()
                print '\tsops = sops_add(sops, op_lb("%s", addr8, "%s"));' % (name, r)
                break
            elif var in op[0]:
                where = op[0].find(var)
                name = op[0][0:where].strip()
                r = op[0][where+len(var):].strip()
                print '\tsops = sops_add(sops, op_l("%s", addr8, "%s"));' % (name, r)
                break
    elif(op[1] == '3'): # 3-byte ops
        print '\taddr16 = r->raw[phy(pc+1)] | (r->raw[phy(pc+2)]<<8);'
        for var in VARS:
            bvar = '(' + var + ')'
            if op[0].endswith(var):
                name = op[0][0:len(op[0])-len(var)-1]
                print '\tsops = sops_add(sops, op_r("%s", addr16));' % name
                break
            elif op[0].endswith(bvar):
                name = op[0][0:len(op[0])-len(bvar)-1]
                print '\tsops = sops_add(sops, op_rb("%s", addr16));' % name
                break
            elif bvar in op[0]:
                where = op[0].find(bvar)
                name = op[0][0:where].strip()
                r = op[0][where+len(bvar):].strip()
                print '\tsops = sops_add(sops, op_lb("%s", addr16, "%s"));' % (name, r)
                break
            elif var in op[0]:
                where = op[0].find(var)
                name = op[0][0:where].strip()
                r = op[0][where+len(var):].strip()
                print '\tsops = sops_add(sops, op_l("%s", addr16, "%s"));' % (name, r)
                break
    else:
        raise Exception('Wrong operator length')
        
    name = name.strip()
    if name.startswith('CALL'): # conditional jumps/calls
        print '\tcall_addr.push_back(phy(addr16));'
        print '\tjmp(addr16);'
    elif name.startswith('JP '):
        print '\tjmp_addr.push_back(phy(addr16));'
        print '\tjmp(addr16);'
    elif name.startswith('JR '):
        print '\tjmp_addr.push_back(phy(pc + ((char)addr8)));'
        print '\tjmp(addr8);'
    elif name.startswith('JP'): # unconditional jumps
        print '\tjmp_addr.push_back(phy(addr16));'
        print '\tjmpu(addr16);'
    elif name.startswith('JR'):
        print '\tjmp_addr.push_back(phy(pc + ((char)addr8)));'
        print '\tjmpu(addr8);';
    elif name.startswith('RET '): # conditional ret
        print '\tpc += 1;'
    elif name.startswith('RET'): # unconditional ret
        print '\tret();'
    else:
        print '\tpc += ' + op[1] + ';'
    print '\tbreak;'

