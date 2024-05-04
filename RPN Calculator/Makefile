CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = -lm
EXEC = calc 
OBJS = calc.o stack.o operators.o mathlib.o
TEST_OBJS = tests.o mathlib.o

all: calc tests

calc: $(OBJS)
	$(CC) $(LFLAGS) -o calc $(OBJS)

tests: $(TEST_OBJS)
	$(CC) $(LFLAGS) -o tests $(TEST_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f calc calc.o tests $(OBJS) $(TEST_OBJS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make


.PHONY: all clean format

