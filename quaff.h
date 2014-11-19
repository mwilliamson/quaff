#include "stdint.h"

#ifndef QUAFF_H
#define QUAFF_H

typedef struct QuaffVM_s {
  int32_t* stack;
  size_t stack_length;
  size_t stack_capacity;
} QuaffVM;

QuaffVM* quaff_vm_create();

typedef int32_t QuaffInstruction;

QuaffInstruction quaff_inst_const(int16_t value);
QuaffInstruction quaff_inst_pop(void);
QuaffInstruction quaff_inst_dup(void);
QuaffInstruction quaff_inst_swap(int16_t depth);
QuaffInstruction quaff_inst_cmp(void);

void quaff_vm_run(QuaffVM*, QuaffInstruction* instructions, size_t instructions_length);
int32_t quaff_vm_read_stack_int32(QuaffVM*, int index);
void quaff_vm_destroy(QuaffVM*);

#endif
