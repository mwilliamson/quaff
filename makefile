.PHONY: test py_test

quaff: *.h *.c
	gcc *.c -o quaff -Wall -Werror -std=c99

quaff.o: *.h *.c
	gcc -fPIC -c quaff.c -o quaff.o -Wall -Werror -std=c99

libquaff.so: quaff.o
	gcc -shared -o libquaff.so quaff.o

libquaff.a: quaff.o
	rm -f libquaff.a
	ar -cvq libquaff.a quaff.o

test: quaff
	./quaff

quaff_py.c: quaff_py.pyx
	_cython/bin/cython quaff_py.pyx

quaff_py.so: quaff_py.c libquaff.a
	rm -f quaff_py.so
	_cython/bin/pip install -e .

py_test: quaff_py.so
	nosetests quaff_tests.py
