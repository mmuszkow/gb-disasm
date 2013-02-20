#ifndef __GB_DASM_HEADER_H__
#define __GB_DASM_HEADER_H__

/* header.h: GameBoy ROM header. */

#include "stdinc.h"

/** Game supports CGB functions, but works on old gameboys also. */
#define CGB_OR_OLD  0x80
/** Game works on CGB only (physically the same as 80h). */
#define CGB_ONLY    0xC0
/** No SGB functions (Normal Gameboy or CGB only game). */
#define NO_SGB      0x00
/** Game supports SGB functions. */
#define SGB         0x03
/** Destination Japanese. */
#define JAP         0x00
/** Destination non-Japanese. */
#define NON_JAP     0x01

#define MAX_TYPE 0x1F
const char* TYPES[MAX_TYPE] = {
    "ROM ONLY", /* 0x00 */
    "MBC1", /* 0x01 */
    "MBC1+RAM", /* 0x02 */
    "MBC1+RAM+BATTERY", /* 0x03 */
    "Unknown", /* 0x04 */
    "MBC2", /* 0x05 */
    "MBC2+BATTERY", /* 0x06 */
    "Unknown", /* 0x07 */
    "ROM+RAM", /* 0x08 */
    "ROM+RAM+BATTERY", /* 0x09 */
    "Unknown", /* 0x0A */
    "MMM01", /* 0x0B */
    "MMM01+RAM", /* 0x0C */            
    "MMM01+RAM+BATTERY", /* 0x0D */
    "Unknown", /* 0x0E */
    "MBC3+TIMER+BATTERY", /* 0x0F */
    "MBC3+TIMER+RAM+BATTERY", /* 0x10 */
    "MBC3", /* 0x11 */
    "MBC3+RAM", /* 0x12 */
    "MBC3+RAM+BATTERY", /* 0x13 */
    "Unknown", /* 0x14 */
    "MBC4", /* 0x15 */
    "MBC4+RAM", /* 0x16 */
    "MBC4+RAM+BATTERY", /* 0x17 */
    "Unknown", /* 0x18 */
    "MBC5", /* 0x19 */
    "MBC5+RAM", /* 0x1A */
    "MBC5+RAM+BATTERY", /* 0x1B */
    "MBC5+RUMBLE", /* 0x1C */
    "MBC5+RUMBLE+RAM", /* 0x1D */
    "MBC5+RUMBLE+RAM+BATTERY" /* 0x1E */
};

#define MAX_ROMS_LO 0x08
const char* ROMS_LO[MAX_ROMS_LO] = {
    "32KByte",
    "64KByte",
    "128KByte",
    "256KByte",
    "512KByte",
    "1MByte",
    "2MByte",
    "4MByte"
};

const char* ROMS_HI[3] = {
    "1.1MByte",
    "1.2MByte",
    "1.5MByte"
};

#define MAX_RAMS 0x05
const char* RAMS[MAX_RAMS] = {
    "None", /* 0x00 */
    "2 KBytes", /* 0x01 */
    "8 Kbytes", /* 0x02 */
    "32 KBytes", /* 0x03 */
    "128 KBytes" /* 0x04 */
};

#pragma pack(push, 1)

/** Cartridge header. */
typedef struct {
    /* Chuj go wie. */
    uint8_t hgw[0x100];
    /** Entry point. */
    uint8_t jmp[0x4];
    /** Nintendo logo. */
    uint8_t logo[0x30];
    /** Title. */
    uint8_t title[0xB];
    /** Manufacturer code. */
    uint8_t manufacturer[0x4];
    /** CGB Flag. */
    uint8_t cgb;
    /** New Licensee Code. */
    uint8_t nlc[2];
    /** SGB Flag. */
    uint8_t sgb;
    /** Cartridge type. */
    uint8_t type;
    /** ROM Size. */
    uint8_t rom;
    /** RAM Size. */
    uint8_t ram;
    /** Destination Code. */
    uint8_t dest;
    /** Old Licensee Code. */
    uint8_t license;
    /** Mask ROM Version number. */
    uint8_t version;
    /** Header Checksum. */
    uint8_t h_checksum;
    /**  Global Checksum. */
    uint8_t g_checksum[2];
} cart_header;

#pragma pack(pop)

uint8_t header_checksum(cart_header* header) {
    uint8_t cs;
    uint8_t* raw;
    int i;
    
    raw = (uint8_t*)header;
    cs = 0;
    for(i=0x134; i<=0x14C; i++) cs = cs-raw[i]-1;
    return cs;
}

#endif

