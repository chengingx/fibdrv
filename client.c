#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

int main(int argc, char *argv[])
{
    struct timespec ts1, ts2;
    char buf[1];
    char write_buf[] = "testing writing";
    int offset = 100; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    unsigned int method = 0;
    method = atoi(argv[1]) & 1;

    for (int i = 0; i <= offset; i++) {
        long long sz, kct;

        lseek(fd, i, SEEK_SET);
        clock_gettime(CLOCK_REALTIME, &ts1);
        sz = read(fd, buf, method);
        clock_gettime(CLOCK_REALTIME, &ts2);
        kct = write(fd, write_buf, strlen(write_buf));

        long uct = ts2.tv_nsec - ts1.tv_nsec;
        printf("%d    %lld    %ld    %lld\n", i, kct, uct, uct - kct);
    }

    close(fd);
    return 0;
}
