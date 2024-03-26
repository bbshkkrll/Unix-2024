#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    char *source_filename = argv[1];
    int fd = open(source_filename, O_WRONLY);
    lseek(fd, 0, SEEK_SET);
    write(fd, "1", 1);
    lseek(fd, 10000, SEEK_CUR);
    write(fd, "1", 1);
    lseek(fd, 0, SEEK_END);
    write(fd, "1", 1);

    close(fd);
    return 0;
}
