#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void signal_handler(int signal) {
    switch (signal) {
        case SIGUSR1:
            
            break;
        case SIGUSR2:

            break;
    }
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Fatal: please give me the fifo path\n");
        return -1;
    }
    return 0;
}
