#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int block_size = 4096;
    int option;
    char *input_filename = NULL;
    char *output_filename = NULL;

    while ((option = getopt(argc, argv, "i::o:s::")) != -1) {
        switch (option) {
            case 's':
                block_size = atoi(optarg); break;
            case 'i':
                input_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
        }
    }

    int input_file_read = 0;

    if (input_filename != NULL) {
        input_file_read = open(input_filename, O_RDONLY);
    } else {
        input_file_read = fileno(stdin);
    }

    char buffer[block_size];
    ssize_t bytes_read;

    int b_file_write = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    while ((bytes_read = read(input_file_read, buffer, block_size)) > 0) {
        int is_zero_block = 1;
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] != 0) {
                is_zero_block = 0;
                break;
            }
        }

        if (is_zero_block) {
            lseek(b_file_write, block_size, SEEK_CUR);
        } else {
            write(b_file_write, buffer, block_size);
        }
    }

    return 0;
}
