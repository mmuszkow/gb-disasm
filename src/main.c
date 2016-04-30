#include "stdinc.h"
#include "rom.h"
#include "sops.h"
#include "state.h"
#include "addr_buff.h"
#include "io.h"

/*
    $FFFF           Interrupt Enable Flag
    $FF80-$FFFE     Zero Page - 127 bytes
    $FF00-$FF7F     Hardware I/O Registers
    $FEA0-$FEFF     Unusable Memory
    $FE00-$FE9F     OAM - Object Attribute Memory
    $E000-$FDFF     Echo RAM - Reserved, Do Not Use
    $D000-$DFFF     Internal RAM - Bank 1-7 (switchable - CGB only)
    $C000-$CFFF     Internal RAM - Bank 0 (fixed)
    $A000-$BFFF     Cartridge RAM (If Available)
    $9C00-$9FFF     BG Map Data 2
    $9800-$9BFF     BG Map Data 1
    $8000-$97FF     Character RAM
    $4000-$7FFF     Cartridge ROM - Switchable Banks 1-xx
    $0150-$3FFF     Cartridge ROM - Bank 0 (fixed)
    $0100-$014F     Cartridge Header Area
    $0000-$00FF     Restart and Interrupt Vectors
*/


/* GLOBALS - start */

/** ROM raw content. */
rom*        r;

/* MBC types. */
#define ROM_ONLY    0x00
#define MBC1        0x01
/** MBC type. */
uint8_t     mbc;

/** Temporary buffer for formatting. */
char        tmp[128];

/** Current memory bank. */
int         bank;
/** Program counter. */
uint16_t    pc;
/** Register A. */
uint8_t     a;
/** 0xFF00-0xFFFF used for LDH operation. TODO: check what LDH really does */
uint8_t     hmem[0xFF];

/** Operations list. */
op*         sops;
/** When branching, state to set back when returning. */
state*      top;
/** Adresses for jmps and calls, used for labelling. */
addr_buff   jmp_addr;
addr_buff   call_addr;
/** Start address. */
uint16_t    start;
/** End address. */
uint16_t    end;

/** Physical address mapping. */
uint32_t phy(uint16_t addr) {
    if(mbc == ROM_ONLY) return addr;
    /* TODO: for the moment we don't check the mbc, and threat MBC1, MBC3 and MBC5 equally */
    if(addr < 0x4000) return addr;
    return ((bank-1) * 0x4000) + addr;
}

/** Relative address (to PC). */
uint16_t rel_addr(uint8_t addr) {
    if(addr & 0x80)
        return pc + (char)addr + 2;
    else
        return pc + (addr & 0x7F) + 2;
}

/** Conditional jump #1. */
void jmp16(uint16_t addr) {
    if(addr < end && phy(addr) < r->total) {
        sops_set_jmp(sops, phy(pc), phy(addr));
        top = state_push(top, pc + 3, bank);
        pc = addr;
    } else {
        printf("[0x%.8X] Warning: Address too high, ignoring 0x%.4X (0x%.8X)\n", 
            phy(pc), addr, phy(addr));
        pc += 3;
    }
}

/** Conditional jump #2. */
uint16_t jmp8(uint8_t addr) {
    uint16_t new_pc = rel_addr(addr);
    if(new_pc < end) {
        sops_set_jmp(sops, phy(pc), phy(new_pc));
        top = state_push(top, pc + 2, bank);
        pc = new_pc;
        return new_pc;
    } else {
        printf("[0x%.8X] Warning: Address too high, ignoring 0x%.4X (0x%.8X)\n", 
            phy(pc), new_pc, phy(new_pc));
        pc += 2;
        return start;
    }
}

/** Unconditional jump #1. */
void jmpu16(uint16_t addr) {
    if(addr < end && phy(addr) < r->total) {
        sops_set_jmp(sops, phy(pc), phy(addr));
        pc = addr;
    } else {
        printf("[0x%.8X] Warning: Address too high, ignoring 0x%.4X (0x%.8X)\n", 
            pc, addr, phy(addr));
        pc = start;
    }
}

/** Unconditional jump #2. */
uint16_t jmpu8(uint8_t addr) {
    uint16_t new_pc = rel_addr(addr);
    if(new_pc < end && phy(new_pc) < r->total) {
        sops_set_jmp(sops, phy(pc), phy(new_pc));
        pc = new_pc;
        return new_pc;
    } else {
        printf("[0x%.8X] Warning: Address too high, ignoring 0x%.4X (0x%.8X)\n", 
            pc, new_pc, phy(new_pc));
        pc = start;
        return start;
    }
}

/** Return from call or jump. */
void ret(void) {
    if(top) {
        pc = top->pc;        
        bank = top->bank;
        top = state_pop(top);
    } else
        pc = start;
}

/** No-arg operator */
op* op_0(const char* opname) {
    return op_create(phy(pc), &(r->raw[phy(pc)]), 1, opname);
}

op* op_0_2(const char* opname) {
    return op_create(phy(pc), &(r->raw[phy(pc)]), 2, opname);
}

/* Adresses as right arg */
op* op_r16(const char* opname, uint16_t addr) {
    sprintf(tmp, "%s$%.4X", opname, addr);
    return op_create(phy(pc), &(r->raw[phy(pc)]), 3, tmp);
}

op* op_rb16(const char* opname, uint16_t addr) {
    sprintf(tmp, "%s[$%.4X]", opname, addr);
    return op_create(phy(pc), &(r->raw[phy(pc)]), 3, tmp);
}

op* op_r8(const char* opname, uint8_t addr) {
    sprintf(tmp, "%s$%.2X", opname, addr);
    return op_create(phy(pc), &(r->raw[phy(pc)]), 2, tmp);
}

op* op_rb8(const char* opname, uint8_t addr) {
    sprintf(tmp, "%s[$%.2X]", opname, addr);
    return op_create(phy(pc), &(r->raw[phy(pc)]), 2, tmp);
}

/* Adresses as left arg */
op* op_l16(const char* opname, uint16_t addr, const char* right) {
    sprintf(tmp, "%s$%.4X%s", opname, addr, right);
    return op_create(phy(pc), &(r->raw[phy(pc)]), 3, tmp);
}

op* op_lb16(const char* opname, uint16_t addr, const char* right) {
    sprintf(tmp, "%s[$%.4X]%s", opname, addr, right);
    return op_create(phy(pc), &(r->raw[phy(pc)]), 3, tmp);
}

op* op_l8(const char* opname, uint8_t addr, const char* right) {
    /* this function is used only by the LDH instruction */
    sprintf(tmp, "%s$%.2X%s ; %s", opname, addr, right, io_name(addr));
    return op_create(phy(pc), &(r->raw[phy(pc)]), 2, tmp);
}

op* op_lb8(const char* opname, uint8_t addr, const char* right) {
    sprintf(tmp, "%s[$%.2X]%s", opname, addr, right);
    return op_create(phy(pc), &(r->raw[phy(pc)]), 2, tmp);
}

void usage(const char* argv0) {
    printf(
        "Usage: %s <ROM> -s <HEX> -b <BANK> -a -nc -nj\n"
        "<ROM> -> obligatory, ROM file to be disassembled\n"
        "  -s  -> optional, start address (PC), default is 0x100\n"
        "  -e  -> optional, end adress (PC), default is 0x8000\n"
        "  -b  -> optional, memory bank number, default is 1\n"
        "  -a  -> optional, print assembly, default is print binary dump\n"
        "  -nc -> optional, don't follow call instructions, default is to follow\n"
        "  -nj -> optional, don't follow jump instructions, default is to follow\n", 
        argv0);
}

/* Remember the times when you put everything in main? They are coming back! */
int main(int argc, char** argv) {
    uint8_t     addr8;
    uint16_t    addr16;
    int         i;

    /* Params. */
    int         assembly = 0;
    int         call_follow = 1;
    int         jmp_follow = 1;
    
    /* Default values. */
    bank        = 1;
    start       = 0x100;
    end         = 0x8000;

    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }
    
    /* Parameters parsing. */
    if(argc > 2) {
        int arg = 2;
        while(arg < argc) {
            if(strcmp(argv[arg], "-a") == 0) {
                assembly = 1;
                arg++;
            } else if(strcmp(argv[arg], "-nc") == 0) {
                call_follow = 0;
                arg++;
            } else if(strcmp(argv[arg], "-nj") == 0) {
                jmp_follow = 0;
                arg++;
            } else if (strcmp(argv[arg], "-s") == 0) {
                if(arg+1 < argc) {
                    uint32_t saddr;
                    if(strstr(argv[arg+1], "0x")) {
                        puts("Don't preceed start address with \"0x\"");
                        return -5;
                    }
                    sscanf(argv[arg+1], "%X", &saddr);
                    if(saddr >= 0x4000) {
                        puts("Start address must be in range 0x0000-0x3FFF");
                        return -6;
                    }
                    start = (uint16_t) saddr;
                    arg += 2;
                } else {
                    puts("Specify the start address");
                    usage(argv[0]);
                    return -4;
                }
            } else if (strcmp(argv[arg], "-e") == 0) {
                if(arg+1 < argc) {
                    uint32_t eaddr;
                    if(strstr(argv[arg+1], "0x")) {
                        puts("Don't preceed end address with \"0x\"");
                        return -7;
                    }
                    sscanf(argv[arg+1], "%X", &eaddr);
                    if(eaddr > 0x8000) {
                        puts("End address must be in range 0x0000-0x8000");
                        return -8;
                    }
                    end = (uint16_t) eaddr;
                    arg += 2;
                } else {
                    puts("Specify the start address");
                    usage(argv[0]);
                    return -4;
                }
            } else if (strcmp(argv[arg], "-b") == 0) {
                if(arg+1 < argc) {
                    sscanf(argv[arg+1], "%d", &bank);
                    arg += 2;
                } else {
                    puts("Specify the bank number");
                    usage(argv[0]);
                    return -4;
                }
            } else {
                printf("Uknown argument %s\n", argv[arg]);
                usage(argv[0]);
                return -3;
            }            
        }
    }

    /* Load ROM. */
    r = rom_load(argv[1]);
    if(!r) {
        printf("Could not load ROM file %s\n", argv[1]);
        return -2;
    }
    
    if(!assembly) rom_info(r);
    
    /* Init globals. */
    sops = NULL;
    top = NULL;
    addr_buff_init(&call_addr);
    addr_buff_init(&jmp_addr);
    pc = start;
    switch(rom_header(r)->type) {
        case 0x00: /* ROM ONLY */
        case 0x08: /* ROM+RAM */
        case 0x09: /* ROM+RAM+BATTERY */
            mbc = ROM_ONLY; break;
        default: mbc = MBC1; /* TODO */
    }

    /* Disassembling loop. */
    while(1) {
        /* Limit PC. */
        if(pc >= end) pc = start;
        
        /* Do not visit same instruction twice */
        if(sops_contains(sops, phy(pc))) {
            /* Check if we have any other possible branches to follow */
            if(top) {
                pc = top->pc;
                bank = top->bank;
                top = state_pop(top);
            } else
                goto finish;
        }            
           
        /* big switch interpreting the operations */
        switch(r->raw[phy(pc)]) {
#include "generated.h"
            default:
                printf("Warning: Unknown opcode (0x%.2X) at 0x%.8X\n", r->raw[phy(pc)], phy(pc));
                sops = sops_add(sops, op_0("-"));
                pc = start;
        }
    }

finish:
    /* create labels for followed jumps and calls */
    for(i=0; i<jmp_addr.len; i++)
        sops_set_flag(sops, jmp_addr.addr[i], OP_FLAG_JMP_ADDR);
    for(i=0; i<call_addr.len; i++)
        sops_set_flag(sops, call_addr.addr[i], OP_FLAG_CALL_ADDR);
    
    /* print results */
    if(assembly) 
        sops_asm(sops, stdout, r); 
    else 
        sops_dump(sops, stdout);

    /* Free reources. */
    rom_free(r);
    state_free(top);
    addr_buff_free(&call_addr);
    addr_buff_free(&jmp_addr);
    
    return 0;
}
