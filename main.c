#include "stdio.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "fcntl.h"

#include "quaff.h"

int main(int argc, char** argv) {
  int file = open(argv[1], O_RDONLY);
  struct stat st;
  fstat(file, &st);
  int file_length = st.st_size;
  QuaffInstruction* instructions =
    (QuaffInstruction*) mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, file, 0);
  int instructions_length = file_length / 4;
  QuaffVM* v = quaff_vm_create();
  quaff_vm_run(v, instructions, instructions_length);
  int top = quaff_vm_read_stack_int32(v, 0);
  printf("%d\n", top);
  quaff_vm_destroy(v);

  return 0;
}
