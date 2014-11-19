.PHONY: test py_test

quaff: *.h *.c
	gcc *.c -o quaff -Wall -Werror -std=c99

quaff.o: *.h *.c
	gcc -fPIC -c quaff.c -o quaff.o -Wall -Werror -std=c99

quaff.so: quaff.o
	gcc -shared -o libquaff.so quaff.o

quaff.a: quaff.o
	ar -cvq libquaff.a quaff.o

test: quaff
	./quaff

quaff_py.c: quaff_py.pyx
	_cython/bin/cython quaff_py.pyx

_quaff_py_install: quaff_py.c
	_cython/bin/pip install -e .
	touch _quaff_py_install

py_test: _quaff_py_install
	# TODO
