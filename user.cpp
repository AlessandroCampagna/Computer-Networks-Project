#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>

int main(int argc, char *argv[]) {
    char *ASIP = NULL;
    int ASport = -1;
    int GN = 17;

    int opt;
    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
        case 'n':
            ASIP = strdup(optarg);
            break;
        case 'p':
            ASport = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-n ASIP] [-p ASport]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Default settings
    if (ASIP == NULL) ASIP = strdup("localhost"); // default IP
    if (ASport == -1) ASport = 58000 + GN; // default port

    // rest of your code

    free(ASIP);
    return 0;
}