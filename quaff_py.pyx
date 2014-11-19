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

     void quaff_vm_run(QuaffVM*, QuaffInstruction* instructions, size_t instructions_length)
     int32_t quaff_vm_read_stack_int32(QuaffVM*, int index)
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

cdef class VirtualMachine(object):
    cdef QuaffVM* _vm

    def __init__(self):
        self._vm = quaff_vm_create()

    def __dealloc__(self):
        quaff_vm_destroy(self._vm)

    def run(self, instructions):
        cdef QuaffInstruction instruction_c
        for instruction in instructions:
            instruction_c = instruction
            quaff_vm_run(self._vm, &instruction_c, 1)

    def read_stack_int32(self, index):
        return quaff_vm_read_stack_int32(self._vm, index)
