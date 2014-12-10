cimport cython
from libc.stdlib cimport malloc, free

cdef extern from "stdint.h":
     ctypedef int int16_t
     ctypedef int int32_t

cdef extern from "quaff.h":
     ctypedef struct QuaffVM:
         pass

     QuaffVM* quaff_vm_create()

     ctypedef int QuaffInstruction

     QuaffInstruction quaff_inst_const(int16_t value)
     QuaffInstruction quaff_inst_pop()
     QuaffInstruction quaff_inst_dup()
     QuaffInstruction quaff_inst_swap(int16_t depth)
     QuaffInstruction quaff_inst_cmp()
     QuaffInstruction quaff_inst_add()
     QuaffInstruction quaff_inst_mul()
     QuaffInstruction quaff_inst_jmp(int16_t offset)
     QuaffInstruction quaff_inst_jle(int16_t offset)

     void quaff_vm_run(QuaffVM*, QuaffInstruction* instructions, size_t instructions_length)
     int32_t quaff_vm_read_stack_int32(QuaffVM*, int index)
     size_t quaff_vm_stack_length(QuaffVM*)
     void quaff_vm_destroy(QuaffVM*)

def inst_const(value):
    return quaff_inst_const(value)

def inst_pop():
    return quaff_inst_pop()

def inst_dup():
    return quaff_inst_dup()

def inst_swap(depth):
    return quaff_inst_swap(depth)

def inst_cmp():
    return quaff_inst_cmp()

def inst_add():
    return quaff_inst_add()

def inst_mul():
    return quaff_inst_mul()

def inst_jmp(offset):
    return quaff_inst_jmp(offset)

def inst_jle(offset):
    return quaff_inst_jle(offset)

cdef class VirtualMachine(object):
    cdef QuaffVM* _vm

    def __init__(self):
        self._vm = quaff_vm_create()

    def __dealloc__(self):
        quaff_vm_destroy(self._vm)

    def run(self, instructions):
        cdef int32_t* c_instructions = <int32_t *>malloc(len(instructions) * cython.sizeof(int32_t))
        for i in xrange(len(instructions)):
            c_instructions[i] = instructions[i]
        try:
            quaff_vm_run(self._vm, c_instructions, len(instructions))
        finally:
            free(c_instructions)

    def read_stack_int32(self, index):
        return quaff_vm_read_stack_int32(self._vm, index)

    def stack_length(self):
        return quaff_vm_stack_length(self._vm)
