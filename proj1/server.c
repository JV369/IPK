#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ctype.h>
#include <bits/signum.h>
#include <signal.h>
#include <unistd.h>

int comm_socket;

int isNum(char *arg){
    for (int i = 0; i < strlen(arg); ++i) {
        if(!isdigit(arg[i])){
            return 1;
        }
    }
    return 0;
}

int check_arg(char **arguments,int lenght){
    if(lenght == 3){
        if(strcmp(arguments[1],"-p") == 0){
            if(isNum(arguments[2]))
                return 1;
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
            strcpy(*result,"SEND_END#");
            strcat(*result,token);
            fclose(fd);
            return 0;
        }
    }
    strcpy(*result,"NOT_FOUND#");
    strcat(*result,login);
    fclose(fd);
    return 1;
}


void send_list(char *seed,int socket,char **end){
    FILE *fd;
    char line[255];
    fd = fopen("/etc/passwd","r");
    char *message = (char *)malloc(1024 * sizeof(char));
    char *recv_mess = (char *)malloc(1024 * sizeof(char));
    while((fgets(line,255,fd)) != NULL){

        strcpy(message,"SEND_MORE#");
        char *token = strtok(line,":");
        if(seed == NULL){
            strcat(message,token);
            send(socket,message,1024,0);
        }
        else{
            if(strncmp(seed,token,strlen(seed)) == 0){
                strcat(message,token);
            }
            send(socket,message,1024,0);
        }
        recv(socket,recv_mess,1024,0);
        if(strcmp(recv_mess,"CONTINUE") != 0){
            break;
        }
    }
    free(message);
    free(recv_mess);
    strcpy(*end,"SEND_END#");
}

void sighandler(){
    close(comm_socket);
    exit(0);
}

int main(int argc, char* argv[]) {
    if(check_arg(argv,argc)){
        perror("ERROR: arguments");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd;
    signal(SIGINT, sighandler);
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
    if((bind(sockfd, res->ai_addr, res->ai_addrlen)) < 0){
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    if((listen(sockfd, 5)) < 0){
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

// now accept an incoming connection:

    addr_size = sizeof their_addr;
    while(1) {
        comm_socket = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);

        // ready to communicate on socket descriptor new_fd!
        if (comm_socket > 0) {
            char *message = (char *) malloc(1024 * sizeof(char));
            char *recv_messager = (char *) malloc(1024 * sizeof(char));

            recv(comm_socket,message,1024,0);
            strcpy(recv_messager,"Yes,i am there!");
            send(comm_socket,recv_messager,1024,0);

            recv(comm_socket, message, 1024, 0);
            char *token = strtok(message, "#");
            if (strcmp(token, "NAME") == 0) {
                token = strtok(NULL, "#");
                find_login(token, &recv_messager,4);
            }
            else if(strcmp(token,"FILEDIR") == 0){
                token = strtok(NULL, "#");
                find_login(token, &recv_messager,5);
            }
            else if(strcmp(token,"LIST") == 0){
                token = strtok(NULL, "#");
                send_list(token,comm_socket,&recv_messager);
            }
            send(comm_socket, recv_messager, 1024, 0);
            free(message);
            free(recv_messager);
        }
    }
}