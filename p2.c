#include "lab7.h"

int fifo_read = -1;
void signal_handler(int signal) {
    const char* template = "p2 received signal %s\n";
    switch (signal) {
    case SIGUSR1:
        fprintf(stderr, template, "SIGUSR1");
        if (fifo_read != -1) {
            lab7_package_t package;
            package.size = 0;
            // check if read ok
            // first read size
            int rd1 = read(fifo_read, &package.size, sizeof(package.size));
            if (rd1 <= 0) {
                perror("fifo read");
                close(fifo_read);
                exit(EXIT_FAILURE);
            }
            // then read the data chunk
            int rd2 = read(fifo_read, package.chunk, package.size);
            if (rd2 <= 0) {
                perror("fifo read");
                close(fifo_read);
                exit(EXIT_FAILURE);
            } else {
                fprintf(stderr, "p2 received %hu bytes from p1\n", (uint16_t)(package.size + sizeof(package.size)));
                for (uint16_t i = 0; i < package.size; i++) {
                    // convert by task
                    if (package.chunk[i] >= '0' && package.chunk[i] <= '9') {
                        fputc('N', stdout);
                    } else if ((package.chunk[i] >= 'A' && package.chunk[i] <= 'Z') || (package.chunk[i] >= 'a' && package.chunk[i] <= 'z')) {
                        fputc('L', stdout);
                    } else {
                        fputc('O', stdout);
                    }
                }
                fputc('\n', stdout);
                fflush(stdout);
                // acknowledge p1 that it has got the data by SIGUSR2
                fprintf(stderr, "p2 %u acknowledged p1 %u ready to send by SIGUSR2\n", getpid(), getppid());
                kill(getppid(), SIGUSR2);
            }
        }
        break;
    case SIGUSR2:
        fprintf(stderr, template, "SIGUSR2");
        break;
    }
}

int main(int argc, char const* argv[]) {
    // if fifo path not given
    if (argc < 2) {
        fprintf(stderr, "Fatal: please give me the fifo path by using arguments\nExample: %s /tmp/test_fifo\n", argv[0]);
        return EXIT_FAILURE;
    }
    if ((fifo_read = open(argv[1], O_RDONLY)) < 1) {
        perror("fifo_open");
    }
    if (signal(SIGUSR1, signal_handler) == SIG_ERR || signal(SIGUSR2, signal_handler) == SIG_ERR) {
        perror("p2 signal register");
        return -1;
    }
    for (;;) {
        sleep(~0);
    }
    return 0;
}
