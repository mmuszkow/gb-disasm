rgbasm drmario.asm -o drmario_reasm.o && rgblink -o drmario_reasm.gb drmario_reasm.o && rgbfix -v -p 0 drmario_reasm.gb && rm drmario_reasm.o
