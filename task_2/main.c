#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int success_attempt_count = 0;
int attempt_count = 0;


void termination_handler(int signum) {
    FILE* f_result = fopen("result.txt", "w");
    fprintf(f_result, "[%d] %d/%d (success/all)\n", getpid(), success_attempt_count, attempt_count);
    fclose(f_result);

    exit(0);
}


int main(int argc, char *argv[])
{

    signal(SIGINT, termination_handler);
    while (1) {
        char *source_filename = argv[1];
        char lockfile[128];
        sleep(0.0003);
        sprintf(lockfile, "%s.lck", source_filename);

        while (access(lockfile, F_OK) == 0) {
             sleep(0.0001);
        }

        printf("[%d] Creating lockfile\n", getpid());
        int lock_fd = open(lockfile, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
        char spid[16];
        sprintf(spid, "%d", getpid());
        write(lock_fd, spid, strlen(spid));
        close(lock_fd);

        int source_fd = open(source_filename, O_WRONLY, S_IRUSR | S_IWUSR);
        lseek(source_fd, 0, SEEK_END);
        write(source_fd, ".", 1);
        sleep(3);
        close(source_fd);

       int stored_pid;
       char input[8];
       lock_fd = open(lockfile, O_RDONLY, S_IRUSR | S_IWUSR);
       read(lock_fd, input, sizeof(input));

       stored_pid = atoi(input);

       if (stored_pid == getpid()) {
           printf("[%d] Removing %s\n", getpid(), lockfile);
           remove(lockfile);
       } else {
           exit(1);
           printf("[%d] Exit...\n", getpid());
       }
    }

    return 0;
}
