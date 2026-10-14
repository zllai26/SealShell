#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define DELIM " \t\r\n\a"

void execute_command(char **args) {
    if (args[0] == NULL) {
        return;
    }

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("SealShell");
        }
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) {
        perror("Fork Failed");
    } 
    else {
        wait(NULL);
    }
}

char **split_line(char *line) {
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    if (!tokens) {
        fprintf(stderr, "SealShell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIM);
    while (token != NULL) {
        tokens[position++] = token;
        if (position >= bufsize) {
            bufsize += 64;
            char **new_tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!new_tokens) {
                free(tokens);
                fprintf(stderr, "SealShell: allocation error\n");
                exit(EXIT_FAILURE);
            }
            tokens = new_tokens;
        }
        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

void sealshell_loop() {
    char *line;
    char **args;

    while (1) {
        line = readline("SealShell V1.0 > ");
        if (!line) {
            break;
        }
        if (strlen(line) > 0) {
            add_history(line);
        }
        args = split_line(line);
        execute_command(args);
        free(line);
        free(args);
    }
}

int main(int argc, char **argv) {
    printf("SealShell. Type 'exit' to quit.\n");
    sealshell_loop();
    return 0;
}
