/**
 * Kod pro reflektor
 * Autor: Jan Vávra
 * Login: xvavra20
 */

#include "reflect.h"
struct addrinfo *res;
char *message;
/**
 * @brief odchytávač SIGINT
 */
void sighandler(){
    free(message);
    freeaddrinfo(res);
    exit(0);
}


void reflect(char *port){
    //inicializace proměnných
    struct addrinfo hints;
    int sockfd;
    signal(SIGINT, sighandler);
    //pripravíme se strukturu pro info o serveru
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if((getaddrinfo(NULL, port, &hints, &res)) != 0){
        fprintf(stderr,"Cant get address\n");
        exit(EXIT_FAILURE);
    }

    //vytvoříme socket
    if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) <=0){
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    //bindnem socket
    if((bind(sockfd, res->ai_addr, res->ai_addrlen)) < 0){
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }
    message = (char *) malloc(1024);
    long alloc;
    //hlavní smyčka
    while(1) {
        if (recvfrom(sockfd, message, 1024, 0, res->ai_addr, &res->ai_addrlen) < 0) {
            perror("ERROR: recvfrom");
            break;
        }
        char *token = strtok(message,"#");
        if(strcmp(token,"CONNECT") != 0){
            fprintf(stderr,"Connection error\n");
            break;
        }
        token = strtok(NULL,"#");
        alloc = strtol(token,NULL,10);
        message = (char *)realloc(message,alloc);
        //smyčka pro reflektování probe packetů o velikosti alloc, dokud nepříjmem kontrolní zprávu END
        while(strcmp(message,"END") != 0){
            if (sendto(sockfd, message, alloc, 0, res->ai_addr, res->ai_addrlen) < 0) {
                perror("ERROR: sendto");
                break;
            }
            bzero(message, (size_t) alloc);
            if (recvfrom(sockfd, message, alloc, 0, res->ai_addr, &res->ai_addrlen) < 0) {
                perror("ERROR: recvfrom");
                break;
            }
        }
        //konec smyčky pro reflektování

        //realokování zprávy a čekání na další měření
        message = (char *)realloc(message,1024);
    }
    free(message);
    freeaddrinfo(res);
    return;
}
