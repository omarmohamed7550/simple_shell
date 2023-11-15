#include "shell.h"

void execute_command(char *cmdline) {
    int status;
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        execl(cmdline, cmdline, (char *)NULL);
        printf("Failed to execute %s\n", cmdline);
        exit(1);
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void handle_PATH(char *cmdline, char *PATH, char *cmd) {
    char *token = strtok(cmdline, " ");
    char *path = strtok(PATH, ":");

    while (path != NULL) {
      snprintf(cmd, MAX_COMMAND_LENGTH, "%s/%s", path, token);

        if (access(cmd, F_OK) != -1) {
            return;
        }

        path = strtok(NULL, ":");
    }

    strcpy(cmd, "");
}

void print_env(void) {
    extern char **environ;
    char **var;

    for (var = environ; *var != 0; var++) {
        printf("%s\n", *var);
    }
}

void print_prompt() {
    printf("%s", PROMPT);
    fflush(stdout);
}

void tokenize_command(char *command, char **arguments) {
    char *token = strtok(command, " ");
    int i = 0;
    while (token != NULL) {
        arguments[i++] = token;
        token = strtok(NULL, " ");
    }
    arguments[i] = NULL;
}

void run_command(char *command, char **arguments) {
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(command, arguments) == -1) {
            perror("Error executing command");
            exit(1);
        }
    } else if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else {
        do {
          waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

