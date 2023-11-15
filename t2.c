#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char *cmd = NULL;
    size_t cmd_len = 0;

    while (1) {
        printf("Enter a command (or 'exit' to quit): ");
        if (getline(&cmd, &cmd_len, stdin) == -1) {
            perror("getline failed");
            exit(EXIT_FAILURE);
        }

        // Remove the newline character from the end of the command
        if (cmd[strlen(cmd) - 1] == '\n') {
            cmd[strlen(cmd) - 1] = '\0';
        }

        if (strcmp(cmd, "exit") == 0) {
            free(cmd);
            break;
        }

        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // Child process
            char *args[2];
            args[0] = cmd;
            args[1] = NULL;
            execvp(args[0], args);
            perror(args[0]);
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            int status;
            waitpid(child_pid, &status, 0);
        }
    }

    free(cmd);

    return 0;
}
