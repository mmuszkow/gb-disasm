./src/generator.py > ./src/generated.h
mkdir -p bin
gcc ./src/main.c -O2 -Wall -o ./bin/gb-disasm
