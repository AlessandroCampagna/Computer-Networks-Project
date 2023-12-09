CC=g++
CFLAGS=-Wall -g -std=c++17
OUTDIR=$(PWD)

all:
	$(CC) $(CFLAGS) -o $(OUTDIR)/server.o data.cpp handler.cpp server.cpp
clean:
	rm -f server.o
run:
	$(CC) $(CFLAGS) -o $(OUTDIR)/server.o data.cpp handler.cpp server.cpp
	clear
	./server.o

test:
	$(CC) $(CFLAGS) -o server.o data.cpp handler.cpp server.cpp