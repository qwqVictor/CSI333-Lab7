#ifndef HKS_SPP_CSI333_LAB7
#define HKS_SPP_CSI333_LAB7
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
    uint16_t size;
    char chunk[BUFSIZ-16];
} lab7_package_t;

#endif // !HKS_SPP_CSI333_LAB7