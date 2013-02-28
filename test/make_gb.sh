#!/bin/bash
# $Id$

# This scripts creates a ROM from assembly

if [ $# -ne 1 ]; then
    echo "Usage: $0 <GAME>"
    echo "where GAME is GAME.asm without .asm"
    exit 1
fi

# if file exists
if [ -f "$1.asm" ]; then
rgbasm $1.asm -o $1_reasm.o && rgblink -o $1_reasm.gb $1_reasm.o && rgbfix -v -p 0 $1_reasm.gb && rm $1_reasm.o
else
    echo "File $1.asm doesn't exist"
    exit 2
fi
