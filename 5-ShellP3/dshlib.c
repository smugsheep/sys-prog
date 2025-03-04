#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    command_list_t cmd_list;

    cmd_buff = malloc(SH_CMD_MAX);
    
    if (!cmd_buff) {
        return ERR_MEMORY;
    }

    while (1) {
        // prompt
        printf("%s", SH_PROMPT);

        // read user input
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        // remove the trailing newline
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // if no cmd
        if (strlen(cmd_buff) == 0) {
            printf("%s\n", CMD_WARN_NO_CMD);
            rc = WARN_NO_CMDS;
            continue;
        }

        // attempt to build cmd list
        rc = build_cmd_list(cmd_buff, &cmd_list);
        if (rc != OK) {
            if (rc == ERR_TOO_MANY_COMMANDS) {
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            }
            continue;
        }
    
        // exit cmd
        if (cmd_list.num > 0 && strcmp(cmd_list.commands[0].argv[0], EXIT_CMD) == 0) {
            printf("exiting...\n");
            break;
        }
        // cd cmd
        else if (cmd_list.num > 0 && strcmp(cmd_list.commands[0].argv[0], "cd") == 0) {
            char *dir;
            // if cd has arg or not
            if (cmd_list.commands[0].argc > 1) {
                dir = cmd_list.commands[0].argv[1];
            } else {
                dir = getenv("HOME");
            }

            if (chdir(dir) != 0) {
                perror("cd");
            }

            free_cmd_list(&cmd_list);
            continue;
        }

        // execute pipeline
        execute_pipeline(&cmd_list);
        free_cmd_list(&cmd_list);
    }
    
    free(cmd_buff);
    return rc;
}


int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }

    return OK;
}


int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer != NULL) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }

    return OK;
}


int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    if (cmd_buff->_cmd_buffer != NULL) {
        cmd_buff->_cmd_buffer[0] = '\0';
    }

    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }

    return OK;
}


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (!cmd_line || !cmd_buff) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    clear_cmd_buff(cmd_buff);

    // trim lead / end space from line

    strcpy(cmd_buff->_cmd_buffer, cmd_line);
    char *trimmed_line = cmd_buff->_cmd_buffer;

    while (*trimmed_line == ' ') {
        trimmed_line++;
    }

    size_t len = strlen(trimmed_line);
    while (len > 0 && trimmed_line[len - 1] == ' ') {
        trimmed_line[len - 1] = '\0';
		len--;
    }

    // tokenize (respect quotes)

    char *start_token = trimmed_line;
    int i = 0;
    int inside_quotes = 0;

    for (char *ch = trimmed_line; *ch != '\0'; ch++) {
        if (*ch == ' ' && !inside_quotes) {
            if (start_token != ch) {
                *ch = '\0';
                cmd_buff->argv[i++] = start_token;
            }

            start_token = ch + 1; 
        } else if (*ch == '"') {
            inside_quotes = !inside_quotes;
        }
    }

    // handle token and quotes(if needed)

    if (start_token != trimmed_line + len) {
        if (*start_token == '"') {
            start_token++;
        }
        char *end_token = start_token + strlen(start_token) - 1;
        if (*end_token == '"') {
            *end_token = '\0';
        }
        cmd_buff->argv[i++] = start_token;
    }
    
    cmd_buff->argv[i] = NULL;
    cmd_buff->argc = i;

    return OK;
}


int build_cmd_list(char *cline, command_list_t *clist) {
    if (!cline || !clist) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    clist->num = 0;
    char *cmd;
    char *token = strtok(cline, PIPE_STRING);

    // loop thru

    while (token != NULL && clist->num < CMD_MAX) {
        cmd = token;

        while (*cmd == ' ') {
            cmd++;
        }

        // attempt 2 alloc and build cmd buff

        if (alloc_cmd_buff(&clist->commands[clist->num]) != OK) {
            free_cmd_list(clist);
            return ERR_MEMORY;
        }

        if (build_cmd_buff(cmd, &clist->commands[clist->num]) != OK) {
            free_cmd_list(clist);
            return ERR_MEMORY;
        }

        clist->num++;
        token = strtok(NULL, PIPE_STRING);
    }

    // edge case

    if (token != NULL) {
        return ERR_TOO_MANY_COMMANDS;
    }

    return OK;
}


int free_cmd_list(command_list_t *clist) {
    for (int i = 0; i < clist->num; i++) {
        free_cmd_buff(&clist->commands[i]);
    }

    clist->num = 0;
    return OK;
}


int execute_pipeline(command_list_t *clist) {
    int num_cmds = clist->num;
    int pipe_fds[2 * (num_cmds - 1)];

    // create da pipes
    
    for (int i = 0; i < num_cmds; i++) {
        if (pipe(&pipe_fds[i * 2]) < 0) {
            perror("pipe");
            return ERR_EXEC_CMD;
        }
    }

    // execute pipeline of cmds (refactor last week code)

    int i;
    pid_t pid;

    for (i = 0; i < num_cmds; i++) {
        pid = fork();

        if (pid == 0) { // child
            if (i > 0) { // redir input
                if (dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO) < 0) {
                    perror("dup2 stdin");
                    exit(1);
                }
            }
            
            if (i < num_cmds - 1) { // redir output
                if (dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO) < 0) {
                    perror("dup2 stdout");
                    exit(1);
                }
            }

            for (int j = 0; j < 2*(num_cmds-1); j++) { // close
                close(pipe_fds[j]);
            }

            // exec cmd
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(1);
        }

        else if (pid < 0) {
            perror("fork");
            return ERR_EXEC_CMD;
        }
    }

    int status;

    for (int j = 0; j < 2*(num_cmds-1); j++) {
        close(pipe_fds[j]);
    }

    for (i = 0; i < num_cmds; i++) {
        wait(&status);
    }

    return OK;
}


