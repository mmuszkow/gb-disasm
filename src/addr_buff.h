#ifndef __GB_DASM_ADDR_LIST_H__
#define __GB_DASM_ADDR_LIST_H__

#include "stdinc.h"

typedef struct addr_buff {
    uint32_t*   addr;
    int         len;
    int         reserved;
} addr_buff;

void addr_buff_init(addr_buff* buff) {
    /* Initially 1024 elements. */
    buff->addr = (uint32_t*)malloc(4096);
    buff->len = 0;
    buff->reserved = 1024;
}

void addr_buff_add(addr_buff* buff, uint32_t addr) {
    if(buff->len + 1 < buff->reserved) {
        buff->addr[buff->len++] = addr;
    } else {
        uint32_t* new_addr;
        new_addr = (uint32_t*)malloc(buff->reserved << 3);
        memcpy(new_addr, buff->addr, buff->len<<2);
        free(buff->addr);
        buff->addr = new_addr;
        buff->reserved <<= 1;
        printf("[DEBUG] Resizing to %d\n", buff->reserved);
        addr_buff_add(buff, addr);
    }
}

void addr_buff_free(addr_buff* buff) {
    if(buff->addr) free(buff->addr);
}

#endif
