#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>



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

int find_login(char *login, char **result, int offset){
    FILE *fd;
    char line[255];
    fd = fopen("/etc/passwd","r");
    while((fgets(line,255,fd)) != NULL){
        char *token = strtok(line,":");
        if(strcmp(token,login) == 0){
            for(int i = 0; i < offset; i++) {
                token = strtok(NULL, ":");
            }
            strcpy(*result,"SEND_END/");
            strcat(*result,token);
            fclose(fd);
            return 0;
        }
    }
    strcpy(*result,"NOT_FOUND/");
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
            char *message = (char *) malloc(1024 * sizeof(char));
            char *recv_messager = (char *) malloc(1024 * sizeof(char));
            recv(comm_socket, message, 1024, 0);
            printf("Recieved %s\n",message);
            char *token = strtok(message, "/");
            if (strcmp(token, "NAME") == 0) {
                token = strtok(NULL, "/");
                find_login(token, &recv_messager,4);
                printf("Sending %s\n",recv_messager);
            }
            else if(strcmp(token,"FILEDIR") == 0){
                token = strtok(NULL, "/");
                find_login(token, &recv_messager,5);
                printf("Sending %s\n",recv_messager);
            }
            send(comm_socket, recv_messager, 1024, 0);
            free(message);
            free(recv_messager);
        }
    }

    return 0;
}