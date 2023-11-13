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

int main() {

int fd, errcode;
ssize_t n;
socklen_t addrlen;
struct addrinfo hints, *res;
struct sockaddr_in addr;
char buffer[BUFFER_SIZE];

    /* Cria um socket TCP (SOCK_STREAM) para IPv4 (AF_INET).
    É devolvido um descritor de ficheiro (fd) para onde se deve comunicar. */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    errcode = getaddrinfo("Minho", PORT, &hints, &res);
    if (errcode != 0) {
        exit(1);
    }

    /* Em TCP é necessário estabelecer uma ligação com o servidor primeiro (Handshake).
       Então primeiro cria a conexão para o endereço obtido através de `getaddrinfo()`. */
    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
	printf("Error making connection \n");
        exit(1);
    }

    /* Escreve a mensagem "Hello!\n" para o servidor, especificando o seu tamanho */
    n=write(fd, "The mitocondria is the powerhouse of the cell! #chumbados\n", 100);
    if (n == -1) {
        exit(1);
    }

    /* Lê 128 Bytes do servidor e guarda-os no buffer. */
    n=read(fd, buffer, BUFFER_SIZE);
    if (n == -1) {
        exit(1);
    }

    /* Imprime a mensagem "echo" e o conteúdo do buffer (ou seja, o que foi recebido
    do servidor) para o STDOUT (fd = 1) */
    write(1, "echo: ", 6);
    write(1, buffer, n);

    /* Desaloca a memória da estrutura `res` e fecha o socket */
    freeaddrinfo(res);
    close(fd);
}
