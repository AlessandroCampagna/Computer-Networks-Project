#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define PORT "58001"
#define BUFFER_SIZE 128

int main(){

int fd,errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints,*res;
struct sockaddr_in addr;
char buffer[BUFFER_SIZE];

fd = socket(AF_INET, SOCK_DGRAM, 0); //UDP socket
if(fd == -1) {
	printf("Error opening socket \n");
 	exit(1);
}

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_INET; // IPv4
hints.ai_socktype = SOCK_DGRAM; // UDP socket
hints.ai_flags = AI_PASSIVE;

errcode = getaddrinfo(NULL, PORT, &hints, &res);
if(errcode != 0) exit (1);

n = bind(fd, res->ai_addr, res->ai_addrlen);
if(n == -1) {
	printf("Error binding \n");
	exit(1);
}

while(1) {
	printf("Inside While");
	addrlen = sizeof(addr);
	n = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &addr, &addrlen);

	if(n == -1) exit(1);

	write(1, "recived: ", 10);
	write(1, buffer, n);

	n = sendto(fd, buffer, n, 0, (struct sockaddr*) &addr, addrlen);
	if (n == -1) exit(1);
}

printf("out while");
freeaddrinfo(res);
close(fd);

}
