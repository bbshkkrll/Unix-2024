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
    FILE* f_result = fopen("result.txt", "a");
    fprintf(f_result, "[%d] %d/%d (lock/all_attempts)\n", getpid(), success_attempt_count, attempt_count);
    fclose(f_result);

    exit(0);
}


int main(int argc, char *argv[])
{
    signal(SIGINT, termination_handler);

    char *source_filename = argv[1];
    char lockfile[128];
    sprintf(lockfile, "%s.lck", source_filename);


    while (1) {
        FILE* lock_w;

        if ((lock_w = fopen(lockfile, "wx")) == NULL) {
            continue;
        }

//        printf("[%d] Creating lockfile\n", getpid());
        fprintf(lock_w, "%d", getpid());
        fflush(lock_w);

        FILE* source_fd = fopen(source_filename, "a");
        fprintf(source_fd, "[%d]", getpid());
        sleep(1);
        fclose(source_fd);

        FILE* lock_r = fopen(lockfile, "r");
        char input[128];
        fgets(input, sizeof(input), lock_r);
        int stored_pid = atoi(input);

        fclose(lock_w);
        fclose(lock_r);
        stored_pid = atoi(input);
        attempt_count++;

        if (stored_pid == getpid()) {
//            printf("[%d] Removing %s\n", getpid(), lockfile);
            remove(lockfile);
            success_attempt_count++;
        } else {
//            printf("[%d] Exit...\n", getpid());
            FILE* f_result = fopen("result.txt", "a");
            fprintf(f_result, "[%d] %d/%d (lock/all_attempts)\n", getpid(), success_attempt_count, attempt_count);
            fclose(f_result);
            exit(1);
        }
    }

    return 0;
}
