#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h> // Important for error handling with execvp and cd
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
    cmd_buff_t cmd;

    cmd_buff = malloc(SH_CMD_MAX);
    
    if (alloc_cmd_buff(&cmd) != OK) {
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

        if (strlen(cmd_buff) == 0) {
            printf("%s\n", CMD_WARN_NO_CMD);
            rc = WARN_NO_CMDS;
            continue;
        }

        if (build_cmd_buff(cmd_buff, &cmd) != OK) {
            rc = ERR_MEMORY;
            continue;
        }
    
        // exit
        if (strcmp(cmd.argv[0], EXIT_CMD) == 0) {
            break;
        }
        // cd
        else if (strcmp(cmd.argv[0], "cd") == 0) {
            char *dir;

            if (cmd.argc > 1) {
                dir = cmd.argv[1];
            } else {
                dir = getenv("HOME");
            }

            if (chdir(dir) != 0) {
                perror("cd");
            }

            continue;
        }
        // external
        else {
            pid_t pid = fork();

            if (pid == 0) {
                execvp(cmd.argv[0], cmd.argv);
                perror("execvp");
                exit(1);
            }
            else if (pid < 0) {
                perror("fork");
            }
            else {
                int status;
                waitpid(pid, &status, 0); // parent process
            }
        }

    }
    
    free_cmd_buff(&cmd);
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

    // im gonna go crazy
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