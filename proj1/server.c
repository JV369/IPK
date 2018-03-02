#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

typedef enum {NAME = 30, FILEDIR, LIST}message_type;

typedef struct Message{
    message_type type;
    char *argument;
}TMessage;

int check_arg(char **arguments,int lenght, long *socket){
    if(lenght == 3){
        if(strcmp(arguments[1],"-p") == 0){
            *socket = strtol(arguments[2],NULL,10);
        }
        else{
            return 1;
        }
    }
    else{
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    long server_socket;
    if(check_arg(argv,argc,&server_socket)){
        perror("ERROR: arguments");
        exit(EXIT_FAILURE);
    }


    return 0;
}