//
// Created by jan on 28.3.18.
//

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
    message = (char *) malloc(65507 * sizeof(char));
    while(1) {
        if (recvfrom(sockfd, message, 65507, 0, res->ai_addr, &res->ai_addrlen) < 0) {
            perror("ERROR: recvfrom");
            break;
        }
        if (sendto(sockfd, message, 65507, 0, res->ai_addr, res->ai_addrlen) < 0) {
            perror("ERROR: sendto");
            break;
        }
    }
    free(message);
    freeaddrinfo(res);
    return;
}
