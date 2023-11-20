CC=g++
CFLAGS=-Wall -g
OUTDIR=$(PWD)

all:
	$(CC) $(CFLAGS) -o $(OUTDIR)/user.o handler.cpp user.cpp

clean:
	rm -f user.o

run:
	$(CC) $(CFLAGS) -o $(OUTDIR)/user.o handler.cpp user.cpp
	clear
	./user.o