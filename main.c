#include "stdio.h"

#include "quaff.h"
#include "test.h"

#define VM_TEST(NAME, INSTRUCTIONS, INSTRUCTIONS_LENGTH, EXPECTED) \
  void NAME(void) { \
    QuaffVM* v = quaff_vm_create(); \
    QuaffInstruction instructions[] = INSTRUCTIONS; \
    quaff_vm_run(v, instructions, INSTRUCTIONS_LENGTH); \
    int actual = quaff_vm_read_stack_int32(v, 0); \
    test_assert_equal(#NAME, EXPECTED, actual); \
    quaff_vm_destroy(v); \
  }


VM_TEST(
  test_const_1,
  {quaff_inst_const(1)}, 1,
  1
)

VM_TEST(
  test_const_2,
  {quaff_inst_const(2)}, 1,
  2
)


void test_assert_stack_element(char* message, QuaffVM* v, int index, int expected) {
  int actual = quaff_vm_read_stack_int32(v, index);
  test_assert_equal(message, expected, actual);
}

void test_vm(QuaffInstruction instructions[], size_t instructions_length, void (*assertions)(QuaffVM*)) {
  puts("");
  QuaffVM* v = quaff_vm_create();
  quaff_vm_run(v, instructions, instructions_length);
  assertions(v);
  quaff_vm_destroy(v);
}

void multiple_consts_asserts(QuaffVM* v) {
  test_assert_stack_element("top is 64", v, 0, 64);
  test_assert_stack_element("second is 42", v, 1, 42);
}

void lots_consts_asserts(QuaffVM* v) {
  test_assert_stack_element("top is 31", v, 0, 31);
  test_assert_stack_element("second to bottom is 1", v, 30, 1);
  test_assert_stack_element("bottom is 0", v, 31, 0);
}

void const_then_pop_asserts(QuaffVM* v) {
  test_assert_stack_element("top is 42", v, 0, 42);
}

void const_then_dup_asserts(QuaffVM* v) {
  test_assert_stack_element("top is 42", v, 0, 42);
  test_assert_stack_element("second is 42", v, 1, 42);
}

void const_then_swap_asserts(QuaffVM* v) {
  test_assert_stack_element("top is 42", v, 0, 42);
  test_assert_stack_element("second is 64", v, 1, 64);
}

void const_then_swap_2_asserts(QuaffVM* v) {
  test_assert_stack_element("top is 42", v, 0, 42);
  test_assert_stack_element("middle is 7", v, 1, 7);
  test_assert_stack_element("second is 64", v, 2, 64);
}

void cmp_ints_equal_asserts(QuaffVM* v) {
  test_assert_stack_element("top is 0", v, 0, 0);
}

void cmp_ints_big_to_small_asserts(QuaffVM* v) {
  test_assert_stack_element("top is 1", v, 0, 1);
}

void cmp_ints_small_to_big_asserts(QuaffVM* v) {
  test_assert_stack_element("top is -1", v, 0, -1);
}

int main(void) {

  test_const_1();
  test_const_2();

  QuaffInstruction multiple_consts_insts[] = { quaff_inst_const(42), quaff_inst_const(64) };
  test_vm(multiple_consts_insts, 2, &multiple_consts_asserts);

  QuaffInstruction lots_consts_insts[32];
  for (int i = 0; i < 32; ++i) { lots_consts_insts[i] = quaff_inst_const(i); }
  test_vm(lots_consts_insts, 32, &lots_consts_asserts);

  QuaffInstruction const_then_pop_inst[] = { quaff_inst_const(42), quaff_inst_const(64), quaff_inst_pop() };
  test_vm(const_then_pop_inst, 3, &const_then_pop_asserts);

  // TODO: Decide how to handle pop when stack is empty

  QuaffInstruction const_then_dup_inst[] = {
    quaff_inst_const(42), quaff_inst_dup()
  };
  test_vm(const_then_dup_inst, 2, &const_then_dup_asserts);

  QuaffInstruction const_then_swap_inst[] = {
    quaff_inst_const(42), quaff_inst_const(64), quaff_inst_swap(1)
  };
  test_vm(const_then_swap_inst, 3, &const_then_swap_asserts);

  QuaffInstruction const_then_swap_2_inst[] = {
    quaff_inst_const(42), quaff_inst_const(7), quaff_inst_const(64), quaff_inst_swap(2)
  };
  test_vm(const_then_swap_2_inst, 4, &const_then_swap_2_asserts);

  QuaffInstruction cmp_ints_equal_inst[] = {
    quaff_inst_const(42), quaff_inst_const(42), quaff_inst_cmp()
  };
  test_vm(cmp_ints_equal_inst, 3, &cmp_ints_equal_asserts);

  QuaffInstruction cmp_ints_big_to_small_inst[] = {
    quaff_inst_const(42), quaff_inst_const(7), quaff_inst_cmp()
  };
  test_vm(cmp_ints_big_to_small_inst, 3, &cmp_ints_big_to_small_asserts);

  QuaffInstruction cmp_ints_small_to_big_inst[] = {
    quaff_inst_const(7), quaff_inst_const(42), quaff_inst_cmp()
  };
  test_vm(cmp_ints_small_to_big_inst, 3, &cmp_ints_small_to_big_asserts);

  return 0;
}
