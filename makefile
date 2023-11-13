CC=g++
CFLAGS=-Wall -g

user:
	$(CC) $(CFLAGS) -o user user.cpp
	
	./user -n 17.17.17 -p 17
	./user -n 17.17.17 
	./user -p 17	
	./user

server:
	$(CC) $(CFLAGS) -o server server.cpp
	./server

clean:
	rm -rf user server