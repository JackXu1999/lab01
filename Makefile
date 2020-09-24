DEBUG = -g

my_malloc: my_malloc.o main_demo.o
	gcc $(DEBUG) -o my_malloc my_malloc.o main_demo.o
my_malloc.o: my_malloc.c my_malloc.h
	gcc $(DEBUG) -c my_malloc.c
main_demo.o: main_demo.c my_malloc.h
	gcc $(DEBUG) -c main_demo.c

