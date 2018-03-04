#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>


int check_arg(char **arguments,int lenght, char **socket, char **address, char **message){
    if(lenght > 7 || lenght < 6){
        return 1;
    }
    for (int i = 1; i < lenght; i+=2) {
        if(strcmp(arguments[i], "-h") == 0){
            *address = (char *)malloc(strlen(arguments[i+1])+1);
            strcpy(*address,arguments[i+1]);
        }
        else if(strcmp(arguments[i],"-p") == 0){
            *socket = (char *)malloc(strlen(arguments[i+1])+1);
            strcpy(*socket,arguments[i+1]);
        }
        else{
            if(strcmp(arguments[i],"-n") == 0){
                strcpy(*message,"NAME/");
            }
            else if(strcmp(arguments[i],"-f") == 0){
                strcpy(*message,"FILEDIR/");
            }
            else if(strcmp(arguments[i],"-l") == 0){
                strcpy(*message,"LIST/");
            }
            else {
                return 1;
            }

            if(lenght%2 != 0){
                strcat(*message,arguments[i+1]);
            }
        }
    }
    return 0;
}




int main(int argc, char* argv[]) {
    int client_socket;
    char* server_address;
    char *server_socket;
    char*message = (char *)malloc(1024);
    if(check_arg(argv,argc,&server_socket,&server_address,&message)){
        perror("ERROR: arguments");
        exit(EXIT_FAILURE);
    }
    struct addrinfo hints, *servinfo;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(server_address, server_socket, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    printf("Address aquired\n");

    if ((client_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket aquired\n");

    if (connect(client_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }
    printf("Connected\n");

    ssize_t bytestx = send(client_socket, message, sizeof(message), 0);
    if (bytestx < 0)
        perror("ERROR:sendto");

    char *message_recv = (char *)malloc(sizeof(1024));

    ssize_t bytesrx = recv(client_socket, message_recv, sizeof(message_recv), 0);
    if (bytesrx < 0)
        perror("ERROR:recvfrom");

    printf("%s\n",message_recv);

    close(client_socket);
    free(server_address);
    free(message);
    free(message_recv);
    return 0;
}