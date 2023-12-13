CC=g++
CFLAGS=-Wall -g -std=c++17
OUTDIR=$(PWD)
FILES=server.cpp data.cpp handler.cpp sockets.cpp

all:
	$(CC) $(CFLAGS) -o $(OUTDIR)/server.o $(FILES)
clean:
	rm -f server.o
run:
	$(CC) $(CFLAGS) -o $(OUTDIR)/server.o $(FILES)
	clear
	./server.o

test:
	$(CC) $(CFLAGS) -o server.o $(FILES)