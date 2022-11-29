#include "lab7.h"

void signal_handler(int signal) {
    const char* template = "p1 received signal %s\n";
    switch (signal) {
        case SIGUSR1:
            fprintf(stderr, template, "SIGUSR1");
            break;
        case SIGUSR2:
            fprintf(stderr, template, "SIGUSR2");
            break;
    }
}

int main(int argc, char const *argv[]) {
    // if fifo path not given
    if (argc < 2) {
        fprintf(stderr, "Fatal: please give me the fifo path by using arguments\nExample: %s /tmp/test_fifo\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char* fifo_path = argv[1];
    struct stat fifo_stat;
    // stat if fifo exists, unlink it first
    if (!stat(fifo_path, &fifo_stat) && S_ISFIFO(fifo_stat.st_mode)) {
        unlink(fifo_path);
    }
    // make fifo
    if (mkfifo(fifo_path, 0644) < 0) {
        perror("mkfifo");
        return -1;
    }
    // fork program
    pid_t childpid = -1;
    switch (childpid = fork()) {
    // error case
    case -1:
        perror("fork");
        break;
    // sub case
    case 0:
        execlp("./p2", "p2", fifo_path, NULL);
        fprintf(stderr, "fatal: unable to run subprocess.\n");
        exit(EXIT_FAILURE);
    // main case
    default:
        // register signal
        signal(SIGUSR1, signal_handler);
        signal(SIGUSR2, signal_handler);
        // open write fifo
        int fifo_write;
        if ((fifo_write = open(fifo_path, O_WRONLY)) < 0) {
            perror("fifo open");
            return EXIT_FAILURE;
        }
        lab7_package_t package;
        // while stdin is open
        for (; !feof(stdin);) {
            // prompt
            fprintf(stderr, "p1 %u enter data: ", getpid());
            fgets(package.chunk, sizeof(package.chunk), stdin);
            package.size = strlen(package.chunk);
            // check if write ok
            // for -1 is all 1 in binary, use bitwise NOT and logical NOT to determine if they're both -1.
            if (!(write(fifo_write, &package.size, sizeof(package.size)) > 0 && write(fifo_write, package.chunk, package.size) > 0)) {
                fprintf(stderr, "p1 write error!\n");
                perror("fifo write");
                close(fifo_write);
                return EXIT_FAILURE;
            }
            else {
                fprintf(stderr, "p1 sent %hu bytes to p2\n", (uint16_t)(package.size + sizeof(package.size)));
            }
            // I use SIGUSR1 to tell subprocess to read.
            fprintf(stderr, "p1 %u send SIGUSR1 to p2 %u\n", getpid(), childpid);
            kill(childpid, SIGUSR1);
            usleep(100);
        }
        fprintf(stderr, "p1 stdin eofed! exiting\n");
        close(fifo_write);
        break;
    }
    kill(childpid, SIGTERM);
    return 0;
}
