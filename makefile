CC=g++
CFLAGS=-Wall -g

user:
	$(CC) $(CFLAGS) -o user user.cpp
	./user

server:
	$(CC) $(CFLAGS) -o server server.cpp
	./server

clean:
	rm -rf user server