rgbasm $1.asm -o $1_reasm.o && rgblink -o $1_reasm.gb $1_reasm.o && rgbfix -v -p 0 $1_reasm.gb && rm $1_reasm.o
