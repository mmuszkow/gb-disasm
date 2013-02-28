#ifndef __GB_DASM_ROM__H__
#define __GB_DASM_ROM__H__

/* rom.h: GameBoy ROM content loading. */

#include "stdinc.h"
#include "header.h"

typedef struct rom {
    char*       filename;
    uint8_t*    raw;
    uint32_t    total;
} rom;

rom* rom_load(const char* filename) {
    rom*        r;
    FILE*       f;
    uint32_t    total;

    r = NULL;
    
    /* Read total file content, shouldn't exceed 4GB ;) */
    f = fopen(filename, "rb");
    if(f) {
        fseek(f, 0, SEEK_END);
        total = ftell(f);
        if(total > 0) {
            r = (rom*)malloc(sizeof(rom));
            r->raw = (uint8_t*)malloc(total);
            r->total = total;
            fseek(f, 0, SEEK_SET);
            if(fread(r->raw, 1, total, f) != total) {
                free(r->raw);
                free(r);
                fclose(f);
                return NULL;
            }
            r->filename = strdup(filename);
        }
        fclose(f);
    }

    return r;
}

void rom_free(rom* r) {
    if(r->filename) free(r->filename);
    if(r->raw) free(r->raw);
    free(r);
}

cart_header* rom_header(rom* r) {
    return (cart_header*)r->raw;
}

void rom_info(rom* r) {
    uint8_t h_checksum;
    cart_header* header;

    if(r->total <= sizeof(cart_header)) {
        printf("Wrong ROM size (%d)\n", r->total);
        return;
    }    
    
    header = rom_header(r);
    printf("Title: %s\n", header->title);
    switch(header->cgb) {
        case CGB_OR_OLD:
            puts("CGB flag: works GameBoy and GameBoy Color"); break;
        case CGB_ONLY:
            puts("CGB flag: works only on GameBoy Color"); break;
        case 0x00:
            puts("CGB flag: Not used, old cartridge"); break;
        default:
            printf("CGB flag: 0x%.2X\n", header->cgb);
    }
    switch(header->sgb) {
        case NO_SGB:
            puts("SGB flag: SuperGameBoy not supported"); break;
        case SGB:
            puts("SGB flag: SuperGameBoy supported"); break;
        default:
            printf("SGB flag: 0x%.2X\n", header->sgb);
    }
    if(header->type < MAX_TYPE)
        printf("Type: %s\n", TYPES[header->type]);
    else
        printf("Type: Unknown (0x%.2X)\n", header->type);
    if(header->rom < MAX_ROMS_LO)
        printf("ROM: %s\n", ROMS_LO[header->rom]);
    else if(header->rom >= 0x52 && header->rom <= 0x54)
        printf("ROM: %s\n", ROMS_HI[header->rom-0x52]);
    else
        printf("ROM: Unknown (0x%.2X)\n", header->rom);
    if(header->ram < MAX_RAMS)
        printf("RAM: %s\n", RAMS[header->ram]);
    else
        printf("RAM: Unknown (0x%.2X)\n", header->ram);
    switch(header->dest) {
        case JAP: puts("Destination: Japanese"); break;
        case NON_JAP: puts("Destination: non-Japanese"); break;
        default: printf("Destination: Unknown (0x%.2X)\n", header->dest);
    }
    printf("Version: 0x%.2X\n", header->version);
    if(header->h_checksum == (h_checksum = header_checksum(header)))
        puts("Header checksum: OK");
    else
        printf("Header checksum: Wrong (0x%.2X != 0x%.2X)\n", header->h_checksum, h_checksum);
}

#endif

