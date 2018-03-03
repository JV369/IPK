#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>

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
    TMessage *message = malloc(sizeof(*message));

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
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

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
    comm_socket = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

// ready to communicate on socket descriptor new_fd!
    if (comm_socket > 0)
    {
        char world[150] = "hello,world!";
        printf("Recieving...\n");
        recv(comm_socket,(char*)message,sizeof(message),0);
        if(message->type == NAME){
            printf("Sending...\n");
            send(comm_socket,world,sizeof(world),0);
        }
    }

    free(message);
    return 0;
}