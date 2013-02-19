#include "stdinc.h"
#include "ROM.h"
#include "Dissasm.h"

int main(int argc, char** argv) {
	rom* r;

	argc = 2;
	argv[1] = "drmario.gb";

	if(argc != 2) {
		printf("Usage: %s <ROM>\n", argv[0]);
		return -1;
	}

	r = rom_load(argv[1]);
	if(!r) {
		puts("Could not load file");
		return -2;
	}

	rom_info(r);

	Dissasm dasm(r);
	dasm.print_asm("out.asm");

	rom_free(r);
	
	return 0;
}
