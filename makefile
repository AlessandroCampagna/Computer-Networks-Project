CC=g++
CFLAGS=-Wall -g

user:
	$(CC) $(CFLAGS) -o user user.cpp
	./run

server:
	$(CC) $(CFLAGS) -o server server.cpp
	./run

clean:
	rm -rf run