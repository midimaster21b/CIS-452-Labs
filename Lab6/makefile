all: static_library
	gcc -Wall -Lstatic Prog1.c -o Prog1 -lmysem

static_library:
	gcc -o mysem.o -c mysem.c
	ar rcs static/libmysem.a mysem.o

