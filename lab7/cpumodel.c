/*******************************************************************************
 * Name        : cpumodel.c
 * Author      : Chancelor Assiamah & Nidhi Parekh
 * Date        : June 15th, 2021
 * Description : Lab on Pipes/popen/pclose
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool starts_with(const char *str, const char *prefix) {
    /* TODO:
       Return true if the string starts with prefix, false otherwise.
       Note that prefix might be longer than the string itself.
     */
    if (strlen(prefix) > strlen(str))
      return false;
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

int main() {
    /* TODO:
       Open "cat /proc/cpuinfo" for reading, redirecting errors to /dev/null.
       If it fails, print the string "Error: popen() failed. %s.\n", where
       %s is strerror(errno) and return EXIT_FAILURE.
     */
    FILE* pipe;
    int status;

    if ((pipe = popen("cat /proc/cpuinfo 2</dev/null", "r")) == NULL) {
      fprintf(stderr, "Error: popen() failed. %s.\n", strerror(errno));
      return EXIT_FAILURE;
    }


    /* TODO:
       Allocate an array of 256 characters on the stack.
       Use fgets to read line by line.
       If the line begins with "model name", print everything that comes after
       ": ".
       For example, with the line:
       model name      : AMD Ryzen 9 3900X 12-Core Processor
       print
       AMD Ryzen 9 3900X 12-Core Processor
       including the new line character.
       After you've printed it once, break the loop.
     */
    // Allocate an array of 256 characters on the stack.
    char buf[256];
    // Use fgets to read line by line.
    while (fgets(buf, 256, pipe) != NULL) {
      // If the line begins with "model name", then print everything that comes 
      // after the colon (": ").
      if (starts_with(buf, "model name")) {
        char* cpu_model = strchr(buf, ':') + 2;
        printf("%s", cpu_model);
        // After you've printed it once, break the loop.
        break;
      }
    }


    /* TODO:
       Close the file descriptor and check the status.
       If closing the descriptor fails, print the string
       "Error: pclose() failed. %s.\n", where %s is strerror(errno) and return
       EXIT_FAILURE.
     */
    // closing pipe 
    if ((status = pclose(pipe) < 0)) { 
      fprintf(stderr, "Error: pclose() failed. %s\n", strerror(errno));
      return EXIT_FAILURE;
    }

    return !(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS);
}
