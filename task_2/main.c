#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("My pid:%d\n", getpid());

    char *source_filename = argv[1];
    char lockfile[128];

    sprintf(lockfile, "%s.lck", source_filename);

    printf("Source filename is %s\n", source_filename);
    printf("Lock filename is %s\n", lockfile);

    int fd = open(lockfile, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);

    if (fd < 0) {
        if (errno == EEXIST) {
            printf("Lockfile file exits\n");

            while (access(lockfile, F_OK) == 0) {
                printf("Pending for deletion\n");
                sleep(1);
            }
        }

    } else {
        printf("Lock file doesn't exist\n");
    }

    printf("Creating .lck");
    fd = open(lockfile, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);

    char spid[16];
    sprintf(spid, "%d", getpid());
    write(fd, spid, strlen(spid));

    sleep(3);

    return 0;
}
