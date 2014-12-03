from nose.tools import assert_equal

import quaff_py as quaff


def test_const_1():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(1)])
    assert_stack_elements(vm, [1])

def test_const_2():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(2)])
    assert_stack_elements(vm, [2])

def test_multiple_consts():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(42), quaff.inst_const(64)])
    assert_stack_elements(vm, [64, 42])

def test_lots_consts():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(i) for i in range(32)])
    assert_equal(31, vm.read_stack_int32(0))
    assert_equal(1, vm.read_stack_int32(30))
    assert_equal(0, vm.read_stack_int32(31))

def test_const_then_pop():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(42), quaff.inst_const(64), quaff.inst_pop()])
    assert_stack_elements(vm, [42])

def test_const_then_dup():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(42), quaff.inst_dup()])
    assert_stack_elements(vm, [42, 42])

def test_const_then_swap():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(42), quaff.inst_const(64), quaff.inst_swap(1)])
    assert_stack_elements(vm, [42, 64])

def test_const_then_swap_2():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(42), quaff.inst_const(7), quaff.inst_const(64), quaff.inst_swap(2)])
    assert_stack_elements(vm, [42, 7, 64])

def test_cmp_ints_equal():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(42), quaff.inst_const(42), quaff.inst_cmp()])
    assert_stack_elements(vm, [0])

def test_cmp_ints_big_to_small():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(42), quaff.inst_const(7), quaff.inst_cmp()])
    assert_stack_elements(vm, [1])

def test_cmp_ints_small_to_big():
    vm = quaff.VirtualMachine()
    vm.run([quaff.inst_const(7), quaff.inst_const(42), quaff.inst_cmp()])
    assert_stack_elements(vm, [-1])


def assert_stack_elements(vm, elements):
    actual = map(vm.read_stack_int32, range(len(elements)))
    assert_equal(elements, actual)
