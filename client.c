#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

void print_uint128(unsigned __int128 n, int count)
{
    if (n == 0 && count == 0) {
        putchar(0 + 0x30);
    }

    if (n == 0) {
        return;
    }

    print_uint128(n / 10, 1);
    putchar(n % 10 + 0x30);
}

int main(int argc, char *argv[])
{
    struct timespec ts1, ts2;
    char write_buf[] = "testing writing";
    int offset = 100; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    unsigned int method = 1;  // 0 : original, 1 : fast doubling
    // method = atoi(argv[1]) & 1;

    for (int i = 0; i <= offset; i++) {
        unsigned __int128 buf;
        long long kct;

        lseek(fd, i, SEEK_SET);
        clock_gettime(CLOCK_REALTIME, &ts1);
        read(fd, &buf, method);
        clock_gettime(CLOCK_REALTIME, &ts2);
        kct = write(fd, write_buf, strlen(write_buf));
        /*
        printf("Reading from " FIB_DEV " at offset %d, returned the sequence ",
               i);
        print_uint128(buf, 0);
        printf(".\n");
        */
        long uct = ts2.tv_nsec - ts1.tv_nsec;
        printf("%d    %lld    %ld    %lld\n", i, kct, uct, uct - kct);
    }


    close(fd);
    return 0;
}
