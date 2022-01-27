/***
 * Name        : head.c
 * Author      : Chancelor Assiamah & Nidhi Parekh
 * Date        : June 7, 2021
 * Description : Lab on File I/O System Calls
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 **/

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 16384
#define DEFAULT_LINE_COUNT 10

/**
 * Determines whether or not the input string represents a valid integer.
 * A valid integer has an optional minus sign, followed by a series of digits
 * [0-9].
 */
bool is_integer(char *input) {
    int start = 0, len = strlen(input);

    if (len >= 1 && input[0] == '-') {
        if (len < 2) {
            return false;
        }
        start = 1;
    }
    for (int i = start; i < len; i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}

/**
 * Takes as input a string and an in-out parameter value.
 * If the string can be parsed, the integer value is assigned to the value
 * parameter and true is returned.
 * Otherwise, false is returned and the best attempt to modify the value
 * parameter is made.
 */
bool get_integer(char *input, int *value) {
    long long long_long_i;
    if (sscanf(input, "%lld", &long_long_i) != 1) {
        return false;
    }
    *value = (int)long_long_i;
    if (long_long_i != (long long)*value) {
        fprintf(stderr, "Warning: Integer overflow with '%s'.\n", input);
        return false;
    }
    return true;
}

/**
 * Displays the usage string for the program.
 */
void display_usage(char *progname) {
    printf("Usage: %s [-n num_lines] <filename>\n", progname);
}

/**
 * This program is a simplified version of head, which prints the first n lines
 * of a file.
 */
int main(int argc, char *argv[]) {
    if (argc == 1) {
        display_usage(argv[0]);
        return EXIT_FAILURE;
    }

    int opt = 0, line_count = DEFAULT_LINE_COUNT;
    char *n_value = NULL, *src_file = NULL;

    while ((opt = getopt(argc, argv, ":n:")) != -1) {
        switch (opt) {
            case 'n':
                n_value = optarg;
                break;
            case '?':
                if (optopt == 'n') {
                    fprintf(stderr,
                            "Error: Option '-%c' requires an argument.\n",
                            optopt);
                } else {
                    fprintf(stderr, "Error: Unknown option '-%c' received.\n",
                            optopt);
                }
                display_usage(argv[0]);
                return EXIT_FAILURE;
            default:
                return EXIT_FAILURE;
        }
    }

    if (n_value) {
        if (!is_integer(n_value) || !get_integer(n_value, &line_count) ||
            line_count < 0) {
            printf("Warning: Invalid line count '%s' supplied. "
                   "Using default of %d instead.\n",
                   n_value, DEFAULT_LINE_COUNT);
            line_count = DEFAULT_LINE_COUNT;
        }
    }
    if (optind + 1 < argc) {
        fprintf(stderr, "Error: Too many file names have been supplied.\n");
        return EXIT_FAILURE;
    } else if (optind + 1 > argc) {
        fprintf(stderr, "Error: No file name has been supplied.\n");
        return EXIT_FAILURE;
    }
    src_file = argv[optind];

    /* Uses the system call open() to open the src_file for reading.
     * If it cannot be opened, it will print an error message with the following format:
     * "Error: Cannot open source file '%s': %s.\n"
     */
    int fd = open(src_file, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: Cannot open source file '%s': %s.\n", src_file, strerror(errno));
        return EXIT_FAILURE;
    }

    printf("==> %s (%d line%s) <==\n", src_file, line_count,
           (line_count == 1 ? "" : "s"));
    fflush(stdout);

    /* Uses read() and write() to display the first n lines on the screen.
     * If n exceeds the line count of the file, it will display the whole file.
     */
    int bytes_read;
    char* buf = (char *)malloc((BUFSIZE) * sizeof(char));
    while ( line_count > 0 && (bytes_read = read(fd, buf, 1)) > 0) {
        if (bytes_read == -1) {
            fprintf(stderr, "Error: Failed to read from file '%s'. %s.\n", src_file, strerror(errno));
            return EXIT_FAILURE;
        }
        else {
            if (*buf == '\n')
                line_count--;
                
            int bytes_written;
            if ( (bytes_written = write(STDOUT_FILENO, buf, bytes_read)) != bytes_read) {
                fprintf(stderr, "Error: Failed to write to STDOUT. %s.\n", strerror(errno));
                return EXIT_FAILURE;
            }
        }
    }

    /* Closes the file and frees up resources, if necessary. */
    free(buf);
    close(fd);

    return EXIT_SUCCESS;
}
