//
// Created by jan on 28.3.18.
//


#include "meter.h"

void fillString(char **strng,long to){
    char *temp = (char *)malloc((to) * sizeof(char));
    bzero(temp,to);
    for (int i = 0; i < to-1; i++) {
        temp[i] = 's';
    }
    strcpy(*strng,temp);
    free(temp);
}

long getSleepTime(int client_socket, struct addrinfo *servinfo,long probeSize){

    int accept = 0;
    int testValue = 0;
    //hodnoty pro stanoveni kvality posilani/obdrzeni paketu
    int treshold = 5;
    int iterSendUDP = 10;

    struct timespec sleep;
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    char *message = (char *)malloc((probeSize) * sizeof(char));
    char *message_recv = (char *)malloc((probeSize) * sizeof(char));
    bzero(message,probeSize);
    fillString(&message,probeSize);
    bzero(message_recv,probeSize);
    sleep.tv_sec = 0;
    sleep.tv_nsec = 0;
    if (setsockopt (client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0 ||
        setsockopt (client_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("Error: setsockopt\n");
        exit(1);
    }
    pid_t pid;
    while (testValue < treshold) {
        pid = fork();
        for (int i = 0; i < iterSendUDP; ++i) {
            if (pid == 0) {
                nanosleep(&sleep,NULL);
                if (sendto(client_socket, message, probeSize, 0, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
                    perror("ERROR: sendto");
                }
            } else if (pid > 0) {
                if (recvfrom(client_socket, message_recv, probeSize, 0, servinfo->ai_addr, &servinfo->ai_addrlen) < 0) {
                    break;
                }
                //printf("%s\n",message);
                //printf("%s\n\n",message_recv);
                if(strcmp(message,message_recv) == 0) {
                    accept++;
                }
            }
        }
        if (pid == 0) {
            free(message);
            free(message_recv);
            free(servinfo);
            exit(0);
        }
        testValue++;
        if(accept < iterSendUDP){
            testValue = 0;
            sleep.tv_nsec += 1000;
            printf("Connection problem, upping the sleep time: %ld ns\n",sleep.tv_nsec);
        }
        accept = 0;
    }
    free(message);
    free(message_recv);
    return sleep.tv_nsec;
}

int meter(char *hostname,char *port,long probeSize,long time){
    int client_socket;
    struct addrinfo hints, *servinfo;
    char *message = (char *)malloc(1024 * sizeof(char));
    char *message_recv = (char *)malloc(1024 * sizeof(char));

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if ((getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }
    free(hostname);
    free(port);
    if ((client_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
    bzero(message,1024);
    bzero(message_recv,1024);
    strcpy(message,"CONNECT");
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt (client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0 ||
        setsockopt (client_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("Error: setsockopt\n");
        exit(1);
    }
    if(sendto(client_socket,message,1024,0,servinfo->ai_addr,servinfo->ai_addrlen)< 0){
        perror("ERROR: sendto");
    }
    if(recvfrom(client_socket,message_recv,1024,0,servinfo->ai_addr,&servinfo->ai_addrlen)<0){
        perror("ERROR: recvfrom");
    }
    if(strcmp(message_recv,"CONNECT") == 0){
        printf("Connected\n");
    }
    else {
        fprintf(stderr,"There is a problem on reflector\n");
        return 1;
    }
    free(message);
    free(message_recv);

    long sleeptime = getSleepTime(client_socket,servinfo,probeSize);
    printf("%ld\n",sleeptime);
    struct timeval systime;
    gettimeofday(&systime,NULL);
    printf("%ld %ld\n",(long)systime.tv_sec,(long)systime.tv_usec);
    free(servinfo);
    return 0;
}