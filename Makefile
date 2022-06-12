CC = gcc
CFLAGS = -g -Wall -Wshadow 
DEPS = token.h character.h testTree.h scanner.h parser.h testTree.c scanner.c parser.c p2.c
OBJ = testTree.o scanner.o parser.o p2.o
LDFLAGS = -static
OUTPUT = frontEnd

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
frontEnd: $(OBJ)
	$(CC)  $(CFLAGS) -o $@ $^

.PHONY: all clean
clean:
	/bin/rm -f $(OUTPUT) *.o *.log

