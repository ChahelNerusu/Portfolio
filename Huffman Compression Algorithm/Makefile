CC=clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = -lm
HOBJ = huff.o bitwriter.o bitreader.o node.o pq.o
EDUFF = dehuff.o bitreader.o node.o 
BRE = brtest.o bitreader.o
BWE = bwtest.o bitwriter.o
NTE = nodetest.o node.o
PQE = pqtest.o pq.o node.o
all: huff dehuff brtest bwtest nodetest pqtest

huff: $(HOBJ)
	$(CC) $(LFLAGS) -o huff $(HOBJ)
dehuff: $(EDUFF)
	$(CC) $(LFLAGS) -o dehuff $(EDUFF)
brtest: $(BRE)
	$(CC) $(LFLAGS) -o brtest $(BRE)
bwtest: $(BWE)
	$(CC) $(LFLAGS) -o bwtest $(BWE)
nodetest: $(NTE)
	$(CC) $(LFLAGS) -o nodetest $(NTE)
pqtest: $(PQE)
	$(CC) $(LFLAGS) -o pqtest $(PQE)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f pq.o bitreader.o bitwriter.o node.o huff huff.o dehuff dehuff.o brtest brtest.o bwtest bwtest.o nodetest nodetest.o pqtest pqtest.o $(OBJS)

format:
	clang-format -i -style=file *.[ch]
scan-build: clean
	scan-build --use-cc=$(CC) make

.PHONY: all clean format
