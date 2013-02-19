#ifndef __GB_DASM_STATE_H__
#define __GB_DASM_STATE_H__

/* stack.h: State stack implementation. */

/** Bank and pc state after return from control instruction. */
typedef struct state {
    uint16_t    pc;
    int            bank;
    struct state* prev;
} state;

state* state_push(state* top, uint16_t pc, int bank) {
    state* el;
    el = (state*)malloc(sizeof(state));
    el->pc = pc;
    el->bank = bank;
    el->prev = top;
    return el;
}

state* state_pop(state* top) {
    state* ret;
    ret = top->prev;
    free(top);
    return ret;
}

void state_free(state* top) {
    state* tmp;
    while(top) {
        tmp = top->prev;
        free(top);
        top = tmp;
    }
}

#endif

