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
    TMessage *message = malloc(sizeof(*message));

    if(check_arg(argv,argc,&server_socket)){
        perror("ERROR: arguments");
        exit(EXIT_FAILURE);
    }

    struct sockaddr addr, client_addr;
    memset(&addr,0,sizeof(addr));
    addr.sa_family = AF_INET;
    if ((bind(server_socket, &addr, sizeof(addr))) < 0)
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    if ((listen(server_socket, 5)) < 0) {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    memset(&addr,0,sizeof(client_addr));
    client_addr.sa_family = AF_INET;
    socklen_t client_addr_len = sizeof client_addr;
    int comm_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (comm_socket > 0)
    {
        char world[150] = "hello,world!";
        recv(comm_socket,(char*)message,sizeof(message),0);
        if(message->type == NAME){
            send(comm_socket,world,sizeof(world),0);
        }
    }

    free(message);
    return 0;
}