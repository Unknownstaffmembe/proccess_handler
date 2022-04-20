proc_handler.o: proc_handler.c proc_handler.h
	gcc -g -c proc_handler.c -luqueue -lpthread -o proc_handler.o

.PHONY: test global

test: proc_handler.o test.c
	gcc -g test.c proc_handler.o -luqueue -o test.o

global: proc_handler.c proc_handler.h
	gcc -O2 -c proc_handler.c -luqueue -o proc_handler.o
	cp proc_handler.h /usr/local/include/proc_handler.h
	cp proc_handler.o /usr/local/lib/libproc_handler.a
