#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char *save;
    char *cmd_token;
    int cmd_count = 0;

    // clear command list
    memset(clist, 0, sizeof(command_list_t));
    clist->num = 0;

    // if no commands
    if (cmd_line == NULL || strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    // split by pipes
    cmd_token = strtok_r(cmd_line, PIPE_STRING, &save);
    while (cmd_token != NULL) {
        
        // trim lead spaces
        while (*cmd_token == SPACE_CHAR) {
            cmd_token++;
        }

        // too many commands
        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        // parse command
        char *arg_token;
        char *arg_save;
        
        arg_token = strtok_r(cmd_token, " ", &arg_save);

        // if command too big
        if (strlen(arg_token) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        // store command
        strcpy(clist->commands[cmd_count].exe, arg_token);

        // store args
        char *args_ptr = clist->commands[cmd_count].args;
        while ((arg_token = strtok_r(NULL, " ", &arg_save)) != NULL) {
            // if args too big
            if (strlen(args_ptr) + strlen(arg_token) + 1 >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            
            // add space to args
            if (*args_ptr) {
                strcat(args_ptr, " ");
            }

            strcat(args_ptr, arg_token);
        }

        cmd_count++;
        cmd_token = strtok_r(NULL, PIPE_STRING, &save);
    }

    clist->num = cmd_count;
    return OK;
}