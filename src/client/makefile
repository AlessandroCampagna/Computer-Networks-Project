CC=g++
CFLAGS=-Wall -g -std=c++17
OUTDIR=$(PWD)

all:
	$(CC) $(CFLAGS) -o $(OUTDIR)/user.o  handler.cpp user.cpp

clean:
	rm -f user.o

run:
	$(CC) $(CFLAGS) -o $(OUTDIR)/user.o  handler.cpp user.cpp
	clear
	./user.o

test: 
	$(CC) $(CFLAGS) -o user.o  handler.cpp user.cpp