CC=g++
CFLAGS=-Wall -g -std=c++17
OUTDIR=$(PWD)
FILES = connection.cpp handler.cpp user.cpp

all:
	$(CC) $(CFLAGS) -o $(OUTDIR)/user.o $(FILES)

clean:
	rm -f user.o

run:
	$(CC) $(CFLAGS) -o $(OUTDIR)/user.o $(FILES)
	clear
	./user.o

test: 
	$(CC) $(CFLAGS) -o user.o $(FILES)