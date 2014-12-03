.PHONY: test py_test

quaff: *.h *.c
	gcc *.c -o quaff -Wall -Werror -std=c99

quaff.o: *.h *.c
	gcc -fPIC -c quaff.c -o quaff.o -Wall -Werror -std=c99

libquaff.so: quaff.o
	gcc -shared -o libquaff.so quaff.o

libquaff.a: quaff.o
	rm libquaff.a
	ar -cvq libquaff.a quaff.o

test: quaff
	./quaff

quaff_py.c: quaff_py.pyx
	_cython/bin/cython quaff_py.pyx

# TODO: This could be quaff_py.so
_quaff_py_install: quaff_py.c libquaff.a
	rm quaff_py.so
	_cython/bin/pip install -e .
	touch _quaff_py_install

py_test: _quaff_py_install
	nosetests quaff_tests.py
