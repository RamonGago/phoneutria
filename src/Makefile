CC = gcc
CFLAGS = -Wall -g -O0

phoneutria : phoneutria.o link_parser.o url_stack.o
	$(CC) $(CFLAGS) phoneutria.o link_parser.o url_stack.o -o phoneutria
	rm *.o

phoneutria.o : phoneutria.c phoneutria.h link_parser.h url_stack.h
	$(CC) $(CFLAGS) -c phoneutria.c

link_parser.o : link_parser.c link_parser.h url_stack.h
	$(CC) $(CFLAGS) -c link_parser.c

url_stack.o : url_stack.c url_stack.h
	$(CC) $(CFLAGS) -c url_stack.c
