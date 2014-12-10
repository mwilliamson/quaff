#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "quaff.h"

#define INST_CODE_ADD (5)
#define INST_CODE_MUL (6)
#define INST_CODE_JMP (7)
#define INST_CODE_JLE (8)

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

QuaffInstruction quaff_inst_add(void) {
  return INST_CODE_ADD;
}

QuaffInstruction quaff_inst_mul(void) {
  return INST_CODE_MUL;
}

QuaffInstruction quaff_inst_jmp(int16_t offset) {
  return INST_CODE_JMP + (offset << 16);
}

QuaffInstruction quaff_inst_jle(int16_t offset) {
  return INST_CODE_JLE + (offset << 16);
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

int32_t op_cmp(int32_t arg1, int32_t arg2) {
  return arg1 > arg2 ? 1 : arg1 < arg2 ? -1 : 0;
}

int32_t op_add(int32_t arg1, int32_t arg2) {
  return arg1 + arg2;
}

int32_t op_mul(int32_t arg1, int32_t arg2) {
  return arg1 * arg2;
}

void quaff_vm_binary_operation(QuaffVM* vm, int32_t (*op)(int32_t, int32_t)) {
  int arg1 = vm->stack[vm->stack_length - 2];
  int arg2 = vm->stack[vm->stack_length - 1];
  int result = op(arg1, arg2);
  vm->stack_length--;
  vm->stack[vm->stack_length - 1] = result;
}

size_t quaff_vm_run_instruction(QuaffVM* vm, QuaffInstruction instruction) {
  // 0123
  // CC.I
  switch (instruction & 0x000000ff) {
  case 0: // const
    ensure_capacity(vm, vm->stack_length + 1);
    vm->stack[vm->stack_length++] = instruction >> 16;
    return 0;
  case 1: // pop
    vm->stack_length--;
    return 0;
  case 2: // dup
    ensure_capacity(vm, vm->stack_length + 1);
    size_t top_index = vm->stack_length - 1;
    vm->stack[vm->stack_length++] = vm->stack[top_index];
    return 0;
  case 3: // swap
    ;
    int depth = instruction >> 16;
    swap(&vm->stack[vm->stack_length - 1], &vm->stack[vm->stack_length - depth - 1]);
    return 0;
  case 4: // cmp
    quaff_vm_binary_operation(vm, op_cmp);
    return 0;
  case INST_CODE_ADD:
    quaff_vm_binary_operation(vm, op_add);
    return 0;
  case INST_CODE_MUL:
    quaff_vm_binary_operation(vm, op_mul);
    return 0;
  case INST_CODE_JMP:
    return instruction >> 16;
  case INST_CODE_JLE:
    ;
    int32_t condition = vm->stack[vm->stack_length - 1];
    vm->stack_length--;
    if (condition < 1) {
      return instruction >> 16;
    } else {
      return 0;
    }
  default:
    return 0;
  }
}

void quaff_vm_run(QuaffVM* vm, QuaffInstruction* instructions, size_t instructions_length) {
  for (size_t i = 0; i < instructions_length; ++i) {
    i += quaff_vm_run_instruction(vm, instructions[i]);
  }
}

int32_t quaff_vm_read_stack_int32(QuaffVM* vm, int index) {
  return vm->stack[vm->stack_length - index - 1];
}

size_t quaff_vm_stack_length(QuaffVM* vm) {
  return vm->stack_length;
}

void quaff_vm_destroy(QuaffVM* vm) {
  free(vm->stack);
  free(vm);
}
