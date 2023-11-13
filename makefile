CC=g++
CFLAGS=-Wall -g

all:
	$(CC) $(CFLAGS) -o run *.cpp
	./run

clean:
	rm -rf run