proc_handler.o: proc_handler.c proc_handler.h
	gcc -g -c proc_handler.c -luqueue -lpthread -o proc_handler.o

.PHONY: test

test: proc_handler.o test.c
	gcc -g test.c proc_handler.o -luqueue -o test.o
