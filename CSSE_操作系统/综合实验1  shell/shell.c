#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 10
#define MAX_LINE 1024

// 内部命令处理函数
int handle_internal_command(char **args) {
    if (strcmp(args[0], "help") == 0) {
        printf("Available commands: help, exit, external commands like ls, cp etc.\n");
        return 1;
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    return 0;
}

// 处理重定向
void handle_redirection(char **args, int *in_fd, int *out_fd) {
    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            if (args[i + 1] == NULL) {
                perror("Missing input file for redirection");
                return;
            }
            *in_fd = open(args[i + 1], O_RDONLY);
            if (*in_fd == -1) {
                perror("Failed to open input file");
                return;
            }
            args[i] = NULL;
            args[i + 1] = NULL;
        } else if (strcmp(args[i], ">") == 0) {
            if (args[i + 1] == NULL) {
                perror("Missing output file for redirection");
                return;
            }
            *out_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*out_fd == -1) {
                perror("Failed to open output file");
                return;
            }
            args[i] = NULL;
            args[i + 1] = NULL;
        }
        i++;
    }
}

// 外部命令处理函数
void handle_external_command(char **args) {
    int in_fd = STDIN_FILENO;
    int out_fd = STDOUT_FILENO;

    handle_redirection(args, &in_fd, &out_fd);

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
    } else if (pid == 0) {
        // 子进程
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        if (execvp(args[0], args) == -1) {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // 父进程
        if (in_fd != STDIN_FILENO) {
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            close(out_fd);
        }
        wait(NULL);
    }
}

// 处理管道命令
void handle_pipe_command(char *line) {
    char *commands[2];
    char *token = strtok(line, "|");
    int i = 0;
    while (token != NULL && i < 2) {
        commands[i++] = token;
        token = strtok(NULL, "|");
    }

    if (i != 2) {
        perror("Invalid pipe command");
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("Fork failed");
    } else if (pid1 == 0) {
        // 第一个子进程
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        char *args1[MAX_ARGS];
        parse_command(commands[0], args1);
        handle_external_command(args1);
        exit(EXIT_SUCCESS);
    } else {
        pid_t pid2 = fork();
        if (pid2 < 0) {
            perror("Fork failed");
        } else if (pid2 == 0) {
            // 第二个子进程
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            char *args2[MAX_ARGS];
            parse_command(commands[1], args2);
            handle_external_command(args2);
            exit(EXIT_SUCCESS);
        } else {
            // 父进程
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
    }
}

// 解析命令行输入
void parse_command(char *line, char **args) {
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        // 显示提示符
        printf("$ ");

        // 读取用户输入
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            continue;
        }

        // 检查是否有管道
        if (strstr(line, "|") != NULL) {
            handle_pipe_command(line);
            continue;
        }

        // 解析命令
        parse_command(line, args);

        if (args[0] == NULL) {
            continue;
        }

        // 处理内部命令
        if (handle_internal_command(args)) {
            continue;
        }

        // 处理外部命令
        handle_external_command(args);
    }

    return 0;
}
