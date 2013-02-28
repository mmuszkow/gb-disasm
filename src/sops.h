#ifndef __GB_DASM_SOPS_H__
#define __GB_DASM_SOPS_H__

/** sops.h: Operations sorted by address. Sorted linked list. */

#include "rom.h"

/** This operation is destination of a jump instruction. */
#define OP_FLAG_JMP_ADDR    0x01
/** This operation is destination of a call instruction. */
#define OP_FLAG_CALL_ADDR   0x02
/** Jump/call instruction. */
#define OP_FLAG_IS_JUMP     0x04

/** Operation. */
typedef struct op {
    /** Offset in ROM. */
    uint32_t    off;
    /** Operation name and parameters. */
    char        name[32];    
    /** Operation binary code. */
    uint8_t     code[3];
    /** Operation binary code length in bytes, max 3. */
    uint8_t     len;
    /** Flags, used for labelling. */
    uint8_t     flags;
    /** If jump or call, physical destination address. */
    uint32_t    addr;
    /** Linked list. */
    struct op*  next;
} op;

op* op_create(uint32_t off, const uint8_t* code, uint8_t len, const char* name) {
    op* oper;
    int i;

    oper = (op*)malloc(sizeof(op));
    oper->off = off;
    for(i=0; i<len; i++) 
        oper->code[i] = code[i];
    oper->len = len;
    strcpy(oper->name, name);
    oper->addr = 0;
    oper->flags = 0;
    oper->next = NULL;
    return oper;
}

void sops_free(op* head) {
    if(head) {
        op* sw, *tmp = head;
        while(tmp) {
            sw = tmp;
            tmp = tmp->next;
            free(sw);
        }
    }
}

op* sops_add(op* head, op* oper) {
    op* new_op = op_create(oper->off, oper->code, oper->len, oper->name);
    if(head) {
        op* tmp, *prev;
        tmp = head;
        prev = NULL;

        while(tmp) {
            if(oper->off < tmp->off) {
                if(prev) {
                    prev->next = new_op;
                    new_op->next = tmp;
                    return head;
                } else {
                    new_op->next = head;
                    head = new_op;
                    return head;
                }
            } else if(oper->off == tmp->off) {
                return head; /* already on list */
            }
            prev = tmp;
            tmp = tmp->next;
        }

        prev->next = new_op;
    } else
        head = new_op;

    return head;
}

op* sops_get(op* head, uint32_t addr) {
    op* tmp = head;
    while(tmp) {
        if(tmp->off == addr) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

int sops_contains(op* head, uint32_t addr) {
    return (sops_get(head, addr) != NULL);
}

void sops_set_flag(op* head, uint32_t addr, uint8_t flag) {
    op* tmp = head;
    while(tmp) {
        if(tmp->off == addr) {
            tmp->flags |= flag;
            return;
        }
        tmp = tmp->next;
    }
}

/** Use this after call or jump instruction. */
void sops_set_jmp(op* head, uint32_t addr, uint32_t daddr) {
    op* tmp = head;
    while(tmp) {
        if(tmp->off == addr) {
            tmp->flags |= OP_FLAG_IS_JUMP;
            tmp->addr = daddr;
            return;
        }
        tmp = tmp->next;
    }
}

/** Hex dump. */
void sops_dump(op* head, FILE* f) {
    op* tmp = head;
    while(tmp) {
        switch(tmp->len) {
            case 1:
                fprintf(f, "[0x%.8X] 0x%.2X           %s\n", 
                    tmp->off, tmp->code[0], tmp->name);
                break;
            case 2:
                if(strstr(tmp->name, "JR")) {
                    fprintf(f, "[0x%.8X] 0x%.2X 0x%.2X      %s ; 0x%X\n", 
                        tmp->off, tmp->code[0], tmp->code[1], tmp->name, tmp->addr);
                } else {
                    fprintf(f, "[0x%.8X] 0x%.2X 0x%.2X      %s\n", 
                        tmp->off, tmp->code[0], tmp->code[1], tmp->name);
                }
                break;
            case 3:
                fprintf(f, "[0x%.8X] 0x%.2X 0x%.2X 0x%.2X %s\n", 
                    tmp->off, tmp->code[0], tmp->code[1], tmp->code[2], tmp->name);
                break;
        }

        tmp = tmp->next;
    }
}

/** Disassembled code. */
void sops_asm(op* head, FILE* f, rom* r) {
    char buff[16];
    op* tmp = head;
    uint32_t prev = 0;

    fprintf(f, "SECTION \"rom0\", HOME[0]\n");
    
    while(tmp) {
        /* print data */
        if(tmp->off != prev) {
            if(prev > tmp->off)
                fprintf(f, "\t; Something is wrong here\n");
            else {
                int prev_bank = prev / 0x4000;
                int tmp_bank = tmp->off / 0x4000;
                if(prev_bank == tmp_bank) 
                    fprintf(f, "\n\tINCBIN \"%s\",$%x,$%x-$%x\n", r->filename, prev, tmp->off, prev);
                else {
                    fprintf(f, "\n\tINCBIN \"%s\",$%x,$%x-$%x\n\n", 
                        r->filename, prev, (prev_bank+1)*0x4000, prev);
                    fprintf(f, "SECTION \"bank%d\",DATA,BANK[$%d]\n", 
                        tmp_bank, tmp_bank);
                    fprintf(f, "\n\tINCBIN \"%s\",$%x,$%x-$%x\n", 
                        r->filename, (prev_bank+1)*0x4000, tmp->off, (prev_bank+1)*0x4000);
                }
            }
        }
        
        /* print jump or call label */
        if(tmp->flags & OP_FLAG_JMP_ADDR) 
            fprintf(f, "jmp_%x:\n", tmp->off);
        if(tmp->flags & OP_FLAG_CALL_ADDR)
            fprintf(f, "sub_%x:\n", tmp->off);

        /* print jump or call instruction */
        if(tmp->flags & OP_FLAG_IS_JUMP) {
            if(strstr(tmp->name, "CALL"))
                fprintf(f, "\tCALL sub_%x\n", tmp->addr);
            else {
                char* sep = strstr(tmp->name, ",");
                if(sep) {
                    strcpy(buff, tmp->name);
                    buff[sep-tmp->name] = 0;
                    if(strstr(buff, " ")) /* conditional jump */
                        fprintf(f, "\t%s,jmp_%x\n", buff, tmp->addr);
                    else
                        fprintf(f, "\t%s jmp_%x\n", buff, tmp->addr);
                } else {
                    sep = strstr(tmp->name, " ");
                    if(sep) {
                        strcpy(buff, tmp->name);
                        buff[sep-tmp->name] = 0;
                        fprintf(f, "\t%s jmp_%x\n", buff, tmp->addr);
                    } else
                        fprintf(f, "\t%s ; Something went wrong\n", tmp->name);
                }
            }
        } else {
            fprintf(f, "\t%s\n", tmp->name);
        }

        if(strcmp(tmp->name, "RET") == 0) fprintf(f, "\n");
        
        prev = tmp->off + tmp->len;
        tmp = tmp->next;
    }
    
    /* add anything whats left */
    if(prev % 0x4000 != 0) {
        int prev_bank = prev / 0x4000;
        fprintf(f, "\n\tINCBIN \"%s\",$%x,$%x-$%x\n", r->filename, prev, (prev_bank+1)*0x4000, prev);
        
        /* fill to 32KB */
        if(prev_bank == 0) {
            fprintf(f, "\nSECTION \"bank1\",DATA,BANK[$1]\n");
            fprintf(f, "\tINCBIN \"%s\",$4000,$4000\n", r->filename);
        }
    }
}

#endif

