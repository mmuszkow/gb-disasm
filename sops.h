#pragma once

/** This operation is destination of a jump instruction. */
#define OP_FLAG_JMP_ADDR	0x01
/** This operation is destination of a call instruction. */
#define OP_FLAG_CALL_ADDR	0x02

/** Operation. */
typedef struct op {
	uint32_t	off;
	char		name[32];	
	uint8_t		code[3];
	uint8_t		len;
	uint8_t		flags;
	struct op*	next;
} op;

op* op_create(uint32_t off, const uint8_t* code, uint8_t len, const char* name) {
	op* oper;

	oper = (op*)malloc(sizeof(op));
	oper->off = off;
	for(int i=0; i<len; i++) 
		oper->code[i] = code[i];
	oper->len = len;
	strcpy(oper->name, name);
	oper->flags = 0;
	oper->next = NULL;
	return oper;
}

/** Operations sorted by address. Sorted linked list. */

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

void sops_dump(op* head, FILE* f) {
	op* tmp = head;
	while(tmp) {
		switch(tmp->len) {
			case 1:
				fprintf(f, "[0x%.8X] 0x%.2X           %s\n", 
					tmp->off, tmp->code[0], tmp->name);
				break;
			case 2:
				fprintf(f, "[0x%.8X] 0x%.2X 0x%.2X      %s\n", 
					tmp->off, tmp->code[0], tmp->code[1], tmp->name);
				break;
			case 3:
				fprintf(f, "[0x%.8X] 0x%.2X 0x%.2X 0x%.2X %s\n", 
					tmp->off, tmp->code[0], tmp->code[1], tmp->code[2], tmp->name);
				break;
		}

		tmp = tmp->next;
	}
}

void sops_asm(op* head, FILE* f) {
	op* tmp = head;
	uint32_t prev = 0;

	while(tmp) {
		if(tmp->off != prev) {
			// TODO
			fprintf(f, "\t; %d data bytes\n", tmp->off - prev);
		}
		if(prev > tmp->off) {
			fprintf(f, "\t; Something is wrong here\n");
		}
		if(tmp->flags & OP_FLAG_JMP_ADDR) 
			fprintf(f, "jmp_%x:\n", tmp->off);
		if(tmp->flags & OP_FLAG_CALL_ADDR)
			fprintf(f, "call_%x:\n", tmp->off);
		fprintf(f, "\t%s\n", tmp->name);
		if(strcmp(tmp->name, "RET") == 0) fprintf(f, "\n");
		prev = tmp->off + tmp->len;
		tmp = tmp->next;
	}
}
