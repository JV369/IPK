#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>



typedef enum {NAME = 30, FILEDIR, LIST, SEND_END, NOT_FOUND}message_type;



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

int find_login(char *login, char **result){
    FILE *fd;
    char *line;
    size_t lenght;
    fd = fopen("/etc/passwd","r");
    while((getline(&line,&lenght,fd)) != -1){
        char *token = strtok(line,":");
        if(strcmp(token,login) == 0){
            for(int i = 0; i < 3; i++) {
                token = strtok(NULL, ":");
            }
            *result = (char *)malloc(sizeof(lenght));
            strcpy(*result,token);
            fclose(fd);
            return 0;
        }
    }
    fclose(fd);
    return 1;
}


int main(int argc, char* argv[]) {
    long server_socket;

    if(check_arg(argv,argc,&server_socket)){
        perror("ERROR: arguments");
        exit(EXIT_FAILURE);
    }
    int comm_socket;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd;

// first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if((getaddrinfo(NULL, argv[2], &hints, &res)) != 0){
        fprintf(stderr,"Cant get address\n");
        exit(EXIT_FAILURE);
    }

// make a socket, bind it, and listen on it:

    if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) <=0){
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket aquired\n");
    if((bind(sockfd, res->ai_addr, res->ai_addrlen)) < 0){
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }
    printf("Bind succesfull\n");
    if((listen(sockfd, 5)) < 0){
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }
    printf("Accepting...\n");
// now accept an incoming connection:

    addr_size = sizeof their_addr;
    while(1) {
        comm_socket = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);

        // ready to communicate on socket descriptor new_fd!
        if (comm_socket > 0) {
            TMessage *message = malloc(sizeof(*message));
            TMessage *send_message = malloc(sizeof(*message));
            recv(comm_socket, (char *) message, sizeof(message), 0);
            if (message->type == NAME) {
                if(find_login(message->argument,&(send_message->argument)) == 0){
                    send_message->type = SEND_END;
                }
                else{
                    send_message->type = NOT_FOUND;
                }
            }
            send(comm_socket, (char *)send_message, sizeof(send_message), 0);
        }
    }


    return 0;
}