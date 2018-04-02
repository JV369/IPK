//
// Created by jan on 28.3.18.
//

#define _DEFAULT_SOURCE
#include "meter.h"

/**
 * @brief odchytávač SIGINT
 */
void sighandlerMeter(){
    exit(0);
}

void fillString(char **strng,long to){
    char *temp = (char *)malloc((to) * sizeof(char));
    bzero(temp,to);
    strcpy(temp,*strng);
    for (int i = 0; i < to-1; i++) {
        if(temp[i] == 0)
            temp[i] = 's';
    }
    strcpy(*strng,temp);
    free(temp);
}
/**
 * @brief simuluje posílání zpráv a snaží se najít správný "sleep time" mezi odesláním
 * @param client_socket
 * @param servinfo
 * @param probeSize
 * @return
 */
struct timespec getSleepTime(int client_socket, struct addrinfo *servinfo,long probeSize){

    int accept = 0;
    int testValue = 0;
    //hodnoty pro stanoveni kvality posilani/obdrzeni paketu
    int treshold = 3;
    int waitTime = 0;
    int *iterSendUDP = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *iterSendUDP = 0;
    struct timespec sleep;
    struct timeval timeout;
    timeout.tv_sec = 3;
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
    TQueue queue;
    QueueInit(&queue);
    struct timeval systime,inTime, outTime;

    pid_t pid;
    while (testValue < treshold) {
        switch(testValue){
            case 0:
                waitTime = 3;
                break;
            case 1:
                waitTime = 6;
                break;
            case 2:
                waitTime = 11;
        }
        printf("Testing connection for %d seconds\n",waitTime-1);
        pid = fork();
        gettimeofday(&systime,NULL);
        if (pid == 0){
            waitTime--;
        }
        for (struct timeval actTime = systime; actTime.tv_sec < systime.tv_sec + waitTime; gettimeofday(&actTime,NULL)) {
            if(pid == 0) {
                gettimeofday(&inTime,NULL);
                bzero(message,probeSize);
                fillString(&message,probeSize);
                if(sendto(client_socket,message,probeSize,0,servinfo->ai_addr,servinfo->ai_addrlen)< 0){
                    perror("ERROR: sendto");
                }
                (*iterSendUDP)++;
                nanosleep(&sleep,NULL);
            }
            else if(pid > 0){
                bzero(message_recv,(size_t)probeSize);
                recvfrom(client_socket,message_recv,probeSize,0,servinfo->ai_addr,&servinfo->ai_addrlen);
                gettimeofday(&outTime,NULL);
                QueueUp(&queue,outTime.tv_sec-systime.tv_sec,outTime.tv_usec,message_recv);

                if(strcmp(message,message_recv) == 0){
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
        printf("Send/recieved packets: %d/%d \n",accept,(*iterSendUDP));
        if(accept < (*iterSendUDP)){
            testValue = 0;
            if(sleep.tv_nsec == 0)
                sleep.tv_nsec += 500;
            sleep.tv_nsec *= 4;
            if (sleep.tv_nsec / 1000000000 > 1){
                sleep.tv_sec += 1;
                sleep.tv_nsec = 0;
            }
            printf("Connection problem, upping the sleep time: %lds %ldns\n",sleep.tv_sec,sleep.tv_nsec);
        }
        QueueDestroy(&queue);
        (*iterSendUDP) = 0;
        accept = 0;
    }
    free(message);
    free(message_recv);
    munmap(iterSendUDP, sizeof(int));
    return sleep;
}

void printActualSpeed(float speed){
    if(speed < 1000.0f){
        printf("%.2f b/s\n",speed);
    }
    else if(speed >= 1000.0f && speed < 1000000.0f){
        speed = speed/1000.0f;
        printf("%.2f kb/s\n",speed);
    }
    else if(speed >= 1000000.0f && speed < 1000000000.0f){
        speed = speed/1000000.0f;
        printf("%.2f Mb/s\n",speed);
    }
    else if(speed >= 1000000000.0f){
        speed = speed/1000000000.0f;
        printf("%.2f Gb/s\n",speed);
    }
}

int meter(char *hostname,char *port,long probeSize,long time){
    int client_socket;
    struct addrinfo hints, *servinfo;
    signal(SIGINT, sighandlerMeter);
    char *message = (char *)malloc(1024 * sizeof(char));
    char *message_recv = (char *)malloc(1024 * sizeof(char));

    int digits = (int)log10(time) +1;
    if(probeSize < 9 + digits)
        probeSize = 9 + digits;
    else if (probeSize > 65507){
        probeSize = 65507;
    }

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
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    if (setsockopt (client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0 ||
        setsockopt (client_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("Error: setsockopt\n");
        exit(1);
    }
    if(sendto(client_socket,message,1024,0,servinfo->ai_addr,servinfo->ai_addrlen)< 0){
        perror("ERROR: sendto");
        return 1;
    }
    if(recvfrom(client_socket,message_recv,1024,0,servinfo->ai_addr,&servinfo->ai_addrlen)<0){
        perror("ERROR: recvfrom");
    }
    if(strcmp(message_recv,"CONNECT") == 0){
        printf("Connected\n");
    }
    else {
        fprintf(stderr,"There is a problem on reflector\n");
        free(message);
        free(message_recv);
        free(servinfo);
        return 1;
    }
    free(message);
    free(message_recv);

    struct timespec sleep;
    printf("Testing the connection ... \n");
    sleep = getSleepTime(client_socket,servinfo,probeSize);
    struct timeval systime,inTime, outTime;
    TQueue queue;
    QueueInit(&queue);
    message = (char *)malloc(probeSize * sizeof(char));
    message_recv = (char *)malloc(probeSize * sizeof(char));

    int *numOfPackets = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int accept = 0;
    time++;
    printf("Sending probes ... \n");
    pid_t pid;
    pid = fork();
    if(pid == 0){
        time--;
    }
    gettimeofday(&systime,NULL);

    for (struct timeval actTime = systime; actTime.tv_sec < systime.tv_sec + time; gettimeofday(&actTime,NULL)) {
        if(pid == 0) {
            bzero(message,(size_t)probeSize);
            gettimeofday(&inTime,NULL);
            sprintf(message,"%ld#%ld#",inTime.tv_sec-systime.tv_sec,inTime.tv_usec);
            fillString(&message,probeSize);
            if(sendto(client_socket,message,probeSize,0,servinfo->ai_addr,servinfo->ai_addrlen)< 0){
                perror("ERROR: sendto");
                return 1;
            }
            (*numOfPackets)++;
            nanosleep(&sleep,NULL);
        }
        else if(pid > 0) {
            bzero(message_recv, (size_t) probeSize);
            int rf = recvfrom(client_socket, message_recv, probeSize, 0, servinfo->ai_addr, &servinfo->ai_addrlen);
            gettimeofday(&outTime, NULL);
            if (rf < 0) {
                if(accept < (*numOfPackets))
                    QueueUp(&queue, outTime.tv_sec - systime.tv_sec, outTime.tv_usec, "none");
            } else {
                accept++;
                QueueUp(&queue, outTime.tv_sec - systime.tv_sec, outTime.tv_usec, message_recv);
            }
        }
    }
    if(pid == 0){
        free(message);
        free(message_recv);
        free(servinfo);
        exit(0);
    }
    time--;
    free(message);
    free(message_recv);
    printf("Caculating stats ... \n");
    float minSpeed = INT_MAX;
    float maxSpeed = INT_MIN;
    float avrgSpeed = 0;
    float avrgRtt = 0;
    TQueueMeasure measureResults;
    QueueInitMeasure(&measureResults);
    char *string = (char *)malloc(probeSize);
    while(queue.front != NULL){
        long secSend;
        long usecSend;
        long secRc;
        long usecRc;
        bzero(string,probeSize);
        QueueFrontPop(&queue,&secRc,&usecRc,&string);
        if(strcmp(string,"none") == 0){
            continue;
        }
        char *token = strtok(string,"#");
        secSend = strtol(token,NULL,10);
        token = strtok(NULL,"#");
        usecSend = strtol(token,NULL,10);
        float rtt = (secRc + usecRc/1000000.0f)-(secSend+usecSend/1000000.0f);
        float speed = (probeSize*8)/rtt;
        QueueUpMeasure(&measureResults,speed);
        if(speed > maxSpeed){
            maxSpeed = speed;
        }
        if(speed < minSpeed){
            minSpeed = speed;
        }
        avrgSpeed += speed;
        avrgRtt += rtt;
    }
    avrgSpeed = avrgSpeed/(*numOfPackets);
    avrgRtt = (avrgRtt*1000)/(*numOfPackets);

    float stdevSpeed = 0;
    while (measureResults.front != NULL){
        float speed;
        QueueFrontPopMeasure(&measureResults,&speed);
        stdevSpeed += powf(speed-avrgSpeed,2);
    }
    stdevSpeed = sqrtf(stdevSpeed/(float)(*numOfPackets));

    printf("--------------------------------------------\n");
    printf("Average speed:\t\t\t");
    printActualSpeed(avrgSpeed);
    printf("Maximum speed:\t\t\t");
    printActualSpeed(maxSpeed);
    printf("Minimum speed:\t\t\t");
    printActualSpeed(minSpeed);
    printf("Standart deviation of speed:\t");
    printActualSpeed(stdevSpeed);
    printf("Average RTT:\t\t\t%.2f ms\n",avrgRtt);
    printf("--------------------------------------------\n");
    free(string);
    munmap(numOfPackets,sizeof(int));
    QueueDestroy(&queue);
    free(servinfo);
    return 0;
}

