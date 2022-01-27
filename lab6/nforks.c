/*******************************************************************************
 * Name        : nforks.c
 * Author      : Chancelor Assiamah & Nidhi Parekh
 * Date        : June 10th, 2021
 * Description : Lab on Fork/Exec
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num forks>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int num_forks = 0;
    if (!is_integer(argv[1]) || !get_integer(argv[1], &num_forks) ||
        num_forks <= 0) {
        fprintf(stderr, "Error: Invalid number of forks '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    /*
      TODO:
      Use fork() and execl().

      Create num_forks children processes.
      If any call to fork fails, an error message should be printed and
      the loop should continue.
      Keep track of how many children were actually spawned.
      The error message will be of the form:
      Error: fork[fork_index] failed. %s.
         fork_index will be an integer [1, ..., num_forks]
         %s will be strerror(errno)

      The children should execute randomsleep with no arguments.
      If execl fails, an error message should be printed and
      EXIT_FAILURE should returned.
      The error message will be of the form:
      Error: execl() failed for pid %ld. %s.
         %s will be strerror(errno)
    */
   // Create a pid_t array with its size equal to the number of forks.
   pid_t pid[num_forks];
   // Create an int that tracks how many children have actually been spawned.
   int children_spawned = 0;
   for (int i = 0; i < num_forks; i++) {
       // If the fork fails, print the error message and continue.
       if ((pid[i] = fork()) < 0) {
           fprintf(stderr, "Error: fork[%d] failed. %s.\n", i + 1, strerror(errno));
           continue;
       }
       else if (pid[i] == 0) {
           // We're in the current child process.
           // Execute "randomsleep". If it fails, print the error message and exit.
           if (execl("randomsleep", "randomsleep", (char *)NULL) == -1) {
               fprintf(stderr, "Error: execl() failed for pid %ld. %s.", (long int)pid[i], 
                       strerror(errno));
               return EXIT_FAILURE;
           }
       }
       else {
           // We're in the parent process.
           // Increment the number of children spawned by 1.
           children_spawned++;
       }
   }


    /*
      TODO:
      Use wait().

      We are in the parent here. Wait for all children that were actually
      spawned. As each child terminates, print the message:
      Child with pid <pid> has terminated.
      If an error occurs during waiting, instead print the message:
      Error: wait() failed. %s.
         %s will be strerror(errno)
    */
    for (int i = 0; i < num_forks; i++) {
        int status;

        pid_t terminate = wait(&status);

        if (terminate == -1) {
            fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
        } else {
            printf("Child with pid %d has terminated.\n", terminate);
        }
    }

    printf("All children are done sleeping.\n");
    return EXIT_SUCCESS;
}
