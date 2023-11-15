CC=gcc
CXX=g++
CFLAGS=-Wall -g

user:
    $(CXX) $(CFLAGS) -o user user.cpp

server: 
    $(CXX) $(CFLAGS) -o server.o server/*.cpp
    $(CC) $(CFLAGS) -o server.o server/*.c

clean:
    rm -f user.o server.o