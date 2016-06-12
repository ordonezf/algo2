
CFLAGS=-g -Wall -std=c99 -pedantic
EXEC=tp2
CC=gcc
SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
LDFLAGS=

VALGRIND= valgrind --track-origins=yes --show-reachable=yes --leak-check=full
VALGRIND-V= $(VALGRIND) -v

ifneq (,$(shell grep -lm 1 \'^\s*\#.*include.*\<math\.h\>\' *.h *.c ))
	LDFLAGS+=-lm
endif

all: clean $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(EXEC)

valgrind: clean $(EXEC)
	$(VALGRIND) ./$(EXEC)

valgrind-verb: clean $(EXEC)
	$(VALGRIND-V) ./$(EXEC)

clean:
	rm -f *.o $(EXEC)
