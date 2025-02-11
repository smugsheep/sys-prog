#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
int  count_words(char *, int, int);
int  print_words(char *, int);
char *get_reverse(char *, int);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int str_len = 0;
    int last_was_space = 1;

    // copy string by looping thru each char
    while (*user_str) {
        if (str_len > len) return -1; // error: provided string is too long
        
        if (*user_str == ' ' || *user_str == '\t') { // handle whitespaces
            if (!last_was_space) { // add whitespace if not duplicate
                *buff++ = ' ';
                str_len++;
            }
            last_was_space = 1;
        } else { // else copy char to buffer
            *buff++ = *user_str;
            str_len++;
            last_was_space = 0;
        }

        user_str++;
    }

    // delete any trailing space
    if (str_len > 0 && *(buff - 1) == ' ') {
        buff--;
        str_len--;
    }

    // fill rest of buffer with periods
    int cur_str_len = str_len;
    while (cur_str_len < len) {
        *buff++ = '.';
        cur_str_len++;
    }

    return str_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    if (str_len > len) return -1;
    int word_count = 0;
    
    // count spaces in buff
    for (int i = 0; i < str_len; i++) {
        if (*buff == ' ') {
            word_count++;
        }
        buff++;
    }

    return ++word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

char *get_reverse(char *buff, int str_len) {
    char *reverse = (char *) malloc(str_len+1 * sizeof(char));

    if (reverse == NULL) {
        return NULL;
    }

    // copy end of buff letters to start of new reverse string
    char *start = reverse;
    char *end = buff + str_len - 1;
    while (end >= buff) {
        *start = *end;
        start++;
        end--;
    }

    *start = '\0';
    return reverse;
}

int print_words(char *buff, int str_len) {
    int wc = 1;
    int cur_len = 0;

    if (str_len < 0) return 0;

    printf("Word Print\n");
    printf("----------\n");
    printf("1. ");

    // print each non-whitespace character in buff one by one,
    // keep track of length, print length and go to new line if space
    for (int i = 0; i < str_len; i++) {
        if (*buff != ' ') {
            putchar(*buff);
            cur_len++;
        } else {
            printf(" (%d)", cur_len);
            printf("\n%d. ", ++wc);
            cur_len = 0;
        }
        buff++;
    }
    
    printf(" (%d)\n", cur_len);
    return wc;
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    
    /* This code is safe because this if statement checks that there are
    enough arguments before proceeding. If argv[1] does not exist, aka
    if argc < 2, then we exit. We also exit if the argument does not
    begin with a hyphen. This ensures proper usage. */

    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below

    /* The purpose of this if statement is to check that the user has
    given enough arguments to run the code. All the valid options
    besides -h need at least 3 arguments to run. We exit if this is
    not the case. */

    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    buff = malloc(BUFFER_SZ);
    if (buff == NULL) {
        printf("Error allocating buffer space");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options

        case 'r':
            char *reversed = get_reverse(buff, user_str_len);
            if (reversed == NULL) {
                printf("Error reversing string");
                exit(2);
            }
            printf("Reversed String: %s\n", reversed);
            break;
        case 'w':
            print_words(buff, user_str_len);
            break;
        case 'x':
            if (argc < 5) {
                usage(argv[0]);
                exit(1);
            }
            printf("Not Implemented!\n");
            exit(8);
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?

/* I think that providing both the pointer and its length is good
practice as it provides safety within the code. Explicitly passing
the length lets you use it to detect any errors. */