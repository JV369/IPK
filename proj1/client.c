#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

typedef enum {NAME = 30, FILEDIR, LIST}message_type;

typedef struct Message{
    message_type type;
    char *argument;
}TMessage;


int check_arg(char **arguments,int lenght, long *socket, char **address, TMessage *message){
    if(lenght > 7 || lenght < 6){
        return 1;
    }
    for (int i = 1; i < lenght; i+=2) {
        if(strcmp(arguments[i], "-h") == 0){
            *address = (char *)malloc(strlen(arguments[i+1])+1);
            strcpy(*address,arguments[i+1]);
        }
        else if(strcmp(arguments[i],"-p") == 0){
            *socket = strtol(arguments[i+1],NULL,10);
        }
        else{
            if(lenght%2 != 0){
                *(&message->argument) = (char *)malloc(strlen(arguments[i+1])+1);
                strcpy(message->argument, arguments[i+1]);
            }

            if(strcmp(arguments[i],"-n") == 0){
                message->type = NAME;
            }
            else if(strcmp(arguments[i],"-f") == 0){
                message->type = FILEDIR;
            }
            else if(strcmp(arguments[i],"-l") == 0){
                message->type = LIST;
            }
            else {
                return 1;
            }
        }
    }
    return 0;
}




int main(int argc, char* argv[]) {
    int client_socket;
    char* server_address;
    long server_socket = 0;
    struct hostent *server_addr;
    struct sockaddr_in sin;
    TMessage *message = malloc(sizeof(*message));
    if(check_arg(argv,argc,&server_socket,&server_address,message)){
        perror("ERROR: arguments");
        exit(EXIT_FAILURE);
    }
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    server_addr = gethostbyname(server_address);
    if(server_addr == NULL){
        fprintf(stderr,"Error no address found");
        exit(EXIT_FAILURE);
    }
    memset(&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server_socket);
    memcpy(&sin.sin_addr, server_addr->h_addr_list[0],server_addr->h_length);

    if (connect(client_socket, (const struct sockaddr *) &sin, sizeof(sin)) != 0) {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }

    ssize_t bytestx = send(client_socket, (char*)message, sizeof(message), 0);
    if (bytestx < 0)
        perror("ERROR:sendto");
    char world[150];
    ssize_t bytesrx = recv(client_socket, world, sizeof(world), 0);
    if (bytesrx < 0)
        perror("ERROR:recvfrom");

    printf("%s\n",world);

    close(client_socket);
    free(server_address);
    free(message->argument);
    free(message);
    return 0;
}