#include "stdlib.h"
#include "string.h"

#include "quaff.h"

void swap(int32_t* a, int32_t* b) {
  int32_t t = *a;
  *a = *b;
  *b = t;
}

QuaffVM* quaff_vm_create() {
  QuaffVM* vm = (QuaffVM*)malloc(sizeof(QuaffVM));
  vm->stack_capacity = 16;
  vm->stack = malloc(sizeof(int32_t) * vm->stack_capacity);
  vm->stack_length = 0;
  return vm;
}

// 0123
// CC.I

QuaffInstruction quaff_inst_const(int16_t value) {
  return value << 16;
}

QuaffInstruction quaff_inst_pop(void) {
  return 1;
}

QuaffInstruction quaff_inst_dup(void) {
  return 2;
}

QuaffInstruction quaff_inst_swap(int16_t depth) {
  return 3 + (depth << 16);
}

QuaffInstruction quaff_inst_cmp(void) {
  return 4;
}

void ensure_capacity(QuaffVM* vm, size_t minimum_capacity) {
  if (minimum_capacity > vm->stack_capacity) {
    int32_t* old_stack = vm->stack;
    size_t old_capacity = vm->stack_capacity;
    size_t new_capacity = 2 * old_capacity;
    vm->stack = malloc(sizeof(int32_t) * new_capacity);
    memcpy(vm->stack, old_stack, old_capacity * sizeof(int32_t));
    vm->stack_capacity = new_capacity;
    free(old_stack);
  }
}

void quaff_vm_run_instruction(QuaffVM* vm, QuaffInstruction instruction) {
  switch (instruction & 0x000000ff) {
  case 0: // const
    ensure_capacity(vm, vm->stack_length + 1);
    vm->stack[vm->stack_length++] = instruction >> 16;
    return;
  case 1: // pop
    vm->stack_length--;
    return;
  case 2: // dup
    ensure_capacity(vm, vm->stack_length + 1);
    size_t top_index = vm->stack_length - 1;
    vm->stack[vm->stack_length++] = vm->stack[top_index];
    return;
  case 3: // swap
    ;
    int depth = instruction >> 16;
    swap(&vm->stack[vm->stack_length - 1], &vm->stack[vm->stack_length - depth - 1]);
    return;
  case 4: // cmp
    ;
    int arg1 = vm->stack[vm->stack_length - 2];
    int arg2 = vm->stack[vm->stack_length - 1];
    int result = arg1 > arg2 ? 1 : arg1 < arg2 ? -1 : 0;
    vm->stack_length--;
    vm->stack[vm->stack_length - 1] = result;
    return;
  }
}

void quaff_vm_run(QuaffVM* vm, QuaffInstruction* instructions, size_t instructions_length) {
  for (int i = 0; i < instructions_length; ++i) {
    quaff_vm_run_instruction(vm, instructions[i]);
  }
}

int32_t quaff_vm_read_stack_int32(QuaffVM* vm, int index) {
  return vm->stack[vm->stack_length - index - 1];
}

void quaff_vm_destroy(QuaffVM* vm) {
  free(vm->stack);
  free(vm);
}
