#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>

int main(int argc, char *argv[]) {
    int ASport = -1;
    int GN = 17;
    bool Verbose = false;

    int opt;
    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
        case 'p':
            ASport = atoi(optarg);
            break;
        case 'v':
            Verbose = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-n ASIP] [-p ASport]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Default settings
    if (ASport == -1) ASport = 58000 + GN; // default port
    if (Verbose) printf("ASport = %d\n", ASport);

    // rest of your code

    return 0;
}