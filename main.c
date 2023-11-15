#include "shell.h"

int main() {
    char cmdline[MAX_COMMAND_LENGTH];
    char PATH[] = "/bin:/usr/bin:/usr/local/bin";
    char cmd[MAX_COMMAND_LENGTH];
    char *arguments[MAX_ARGUMENTS];

    while (1) {
        print_prompt();
        if (fgets(cmdline, MAX_COMMAND_LENGTH, stdin) == NULL) {
            break;
        }
        cmdline[strcspn(cmdline, "\n")] = '\0';

        if (strcmp(cmdline, "exit") == 0) {
            exit(0);
        }

        if (strcmp(cmdline, "env") == 0) {
            print_env();
        } else {
            tokenize_command(cmdline, arguments);
            handle_PATH(arguments[0], PATH, cmd);

            if (strlen(cmd) > 0) {
                run_command(cmd, arguments);
            } else {
                printf("Command not found\n");
            }
        }
    }

    return 0;
}
