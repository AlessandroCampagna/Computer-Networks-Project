CC=g++
CFLAGS=-Wall -g -std=c++17
OUTDIR=$(PWD)
FILES=data.cpp handler.cpp sockets.cpp connection.cpp server.cpp

all:
	$(CC) $(CFLAGS) -o $(OUTDIR)/server.o $(FILES)
clean:
	rm -f server.o
run:
	$(CC) $(CFLAGS) -o $(OUTDIR)/server.o $(FILES)
	clear
	./server.o -v