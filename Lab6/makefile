all: static_library
	gcc -Wall -Lstatic -lmysem Prog1.c -o Prog1

static_library:
	gcc -o mysem.o -c mysem.c
	ar rcs static/libmysem.a mysem.o

