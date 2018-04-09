/**
 * Kod pro měřák
 * Autor: Jan Vávra
 * Login: xvavra20
 */

#define _DEFAULT_SOURCE
#include "meter.h"
struct addrinfo *servinfo;
long probeSize;
int client_socket;
/**
 * @brief odchytávač SIGINT
 */
void sighandlerMeter(){
    char *message_err = (char *)malloc(1024 * sizeof(char));
    strcpy(message_err,"END");
    sendto(client_socket, message_err, probeSize, 0, servinfo->ai_addr, servinfo->ai_addrlen);
    free(servinfo);
    free(message_err);
    exit(0);
}

/**
 * naplní zprávu daty na délku řetězce - 1
 * @param strng řetězec, který se má naplnit
 * @param to délka řetězce
 */
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
 * @param client_socket socket na který se mají zasílat zprávy
 * @return
 */
struct timespec getSleepTime(int client_socket){

    long accept = 0;
    int testValue = 0;
    //hodnoty pro stanoveni kvality posilani/obdrzeni paketu
    int treshold = 3;
    int waitTime = 0;
    int *iterSendUDP = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *iterSendUDP = 0;
    struct timespec sleep;
    struct timeval timeout;
    timeout.tv_sec = 1;
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
    struct timeval systime;

    pid_t pid;
    //musí 3x projít bez toho aniž by se ztratila značná část packetů
    while (testValue < treshold) {
        waitTime = 3;
        printf("Testing connection for %d seconds\n",waitTime-1);
        pid = fork();
        gettimeofday(&systime,NULL);
        if (pid == 0){
            waitTime--;
        }
        //zasílání probe packetů po 2 sekundách
        for (struct timeval actTime = systime; actTime.tv_sec < systime.tv_sec + waitTime; gettimeofday(&actTime,NULL)) {
            if(pid == 0) {
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
            }
        }
        // konec zasílání
        if (pid == 0) {
            free(message);
            free(message_recv);
            free(servinfo);
            exit(0);
        }
        bzero(message, (size_t) probeSize);
        strcpy(message,"PACKET_COUNT");
        if (sendto(client_socket, message, probeSize, 0, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
            perror("ERROR: sendto");
        }
        recvfrom(client_socket, message, probeSize, 0, servinfo->ai_addr, &servinfo->ai_addrlen);
        accept = strtol(message,NULL,10);
        testValue++;
        printf("Send/recieved packets: %ld/%d \n",accept,(*iterSendUDP));
        //kontrola kolik přišlo
        if(((*iterSendUDP) - accept) > 10){
            testValue = 0;
            if(sleep.tv_nsec == 0)
                sleep.tv_nsec += 500;
            sleep.tv_nsec *= 2;
            if (sleep.tv_nsec / 1000000000 > 1){
                sleep.tv_sec += 1;
                sleep.tv_nsec = 0;
            }
            if(sleep.tv_sec == 2){
				printf("Connection problem\n");
				exit(1);
			}
            printf("Connection problem, upping the sleep time: %lds %ldns\n",sleep.tv_sec,sleep.tv_nsec);
        }
        (*iterSendUDP) = 0;
        accept = 0;
    }
    free(message);
    free(message_recv);
    munmap(iterSendUDP, sizeof(int));
    return sleep;
}

/**
 * převede rychlost na větší jednotky a vypíše
 * @param speed rychlost, která se má vypsat
 */
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

int meter(char *hostname,char *port,long probeS,long time){
    //inicializace proměnných
    probeSize = probeS;
    struct addrinfo hints;
    signal(SIGINT, sighandlerMeter);
    char *message = (char *)malloc(1024 * sizeof(char));
    char *message_recv = (char *)malloc(1024 * sizeof(char));

    //pokud je menší než časová hlavička, zvětší velikost podle toho kolik bude potřebovat místa na uložení času
    //pokud je větší než UDP_MAX tak zmenší na UDP_MAX
    int digits = (int)log10(time) +1;
    if(probeSize < 9 + digits){
        probeSize = 9 + digits;
        printf("Probe size is too small, upping to %ld\n",probeSize);
	}
    else if (probeSize > 65507){
		printf("Probe size is too big, lowering to 65507\n");
        probeSize = 65507;
    }

    //zjištění adresy
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if ((getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }
    free(hostname);
    free(port);

    //získání socketu
    if ((client_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
    bzero(message,1024);
    bzero(message_recv,1024);
    sprintf(message,"CONNECT#%ld",probeSize);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt (client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0 ||
        setsockopt (client_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("Error: setsockopt\n");
        exit(1);
    }
    //poslání prvotní zprávy
    if(sendto(client_socket,message,1024,0,servinfo->ai_addr,servinfo->ai_addrlen)< 0){
        perror("ERROR: sendto");
        return 1;
    }
    if(recvfrom(client_socket,message_recv,1024,0,servinfo->ai_addr,&servinfo->ai_addrlen)<0){
        perror("ERROR: recvfrom");
    }
    //kontrola na doručenou zprávu
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

    //získání času na uspání
    struct timespec sleep;
    printf("Testing the connection ... \n");
    sleep = getSleepTime(client_socket);
    struct timeval systime,inTime, outTime;
    TQueue queue;
    QueueInit(&queue);
    message = (char *)malloc(probeSize * sizeof(char));
    message_recv = (char *)malloc(probeSize * sizeof(char));

    //anulace counteru na reflektoru
    bzero(message, (size_t) probeSize);
    strcpy(message,"PACKET_COUNT");
    if (sendto(client_socket, message, probeSize, 0, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        perror("ERROR: sendto");
        return 1;
    }
    recvfrom(client_socket, message, probeSize, 0, servinfo->ai_addr, &servinfo->ai_addrlen);

    float minSpeed = INT_MAX;
    float maxSpeed = INT_MIN;
    float avrgSpeed = 0;
    TQueueMeasure measureResults;
    QueueInitMeasure(&measureResults);
    int recievedPackets = 0;
    int iteration = 0;
    int *numOfPackets = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    float iterTime = time/10.0f;
    iterTime++;
    printf("Sending probes ... \n");

    //smyčka intervalů měření
    while(iteration < 10) {
        long accept = 0;
        pid_t pid;
        pid = fork();
        if(pid == 0){
            iterTime--;
        }
        gettimeofday(&systime,NULL);
        //smyčka měření na intervalu
        for (struct timeval actTime = systime; (actTime.tv_sec*1000000.0 + actTime.tv_usec) < ((systime.tv_sec*1000000.0 + systime.tv_usec) + iterTime*1000000.0); gettimeofday(&actTime, NULL)) {
            if (pid == 0) {
                bzero(message, (size_t) probeSize);
                gettimeofday(&inTime, NULL);
                sprintf(message, "%ld#%ld#", inTime.tv_sec - systime.tv_sec, inTime.tv_usec);
                fillString(&message, probeSize);
                if (sendto(client_socket, message, probeSize, 0, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
                    perror("ERROR: sendto");
                    return 1;
                }
                (*numOfPackets)++;
                nanosleep(&sleep, NULL);
            } else if (pid > 0) {
                bzero(message_recv, (size_t) probeSize);
                int rf = recvfrom(client_socket, message_recv, probeSize, 0, servinfo->ai_addr, &servinfo->ai_addrlen);
                gettimeofday(&outTime, NULL);
                if (rf < 0 || strlen(message_recv) < probeSize-1) {
                    if (accept < (*numOfPackets))
                        QueueUp(&queue, outTime.tv_sec - systime.tv_sec, outTime.tv_usec, "none");
                } else {
                    accept++;
                    QueueUp(&queue, outTime.tv_sec - systime.tv_sec, outTime.tv_usec, message_recv);
                }
            }
        }
        //konec měření na intervalu
        if (pid == 0) {
            QueueDestroy(&queue);
            QueueDestroyMeasure(&measureResults);
            free(message);
            free(message_recv);
            free(servinfo);
            exit(0);
        }
            //zjištění kolik paketů se úspěšně odeslalo
        else if (pid > 0){
            bzero(message, (size_t) probeSize);
            strcpy(message,"PACKET_COUNT");
            if (sendto(client_socket, message, probeSize, 0, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
                perror("ERROR: sendto");
                return 1;
            }
            bzero(message, (size_t) probeSize);
            recvfrom(client_socket, message, probeSize, 0, servinfo->ai_addr, &servinfo->ai_addrlen);
            accept = strtol(message,NULL,10);
            iteration++;
            //pokud je počet odeslaných paketů menší než přijatých tak zvedneme sleep time jinak zmenšíme
            if(accept < (*numOfPackets)){
				if(sleep.tv_nsec == 0){
					sleep.tv_nsec = 100;
				}
				else{
                sleep.tv_nsec += sleep.tv_nsec*0.1;
				}
            }
            else{
                sleep.tv_nsec -= sleep.tv_nsec*0.05;
            }
            (*numOfPackets) = 0;
            float speed = ((accept*probeSize*8.0f)/(iterTime-1.0f));
            avrgSpeed += speed;
            if(speed < minSpeed){
                minSpeed = speed;
            }
            if(speed> maxSpeed){
                maxSpeed = speed;
            }
            recievedPackets += accept;
            QueueUpMeasure(&measureResults,speed);
        }
    }
    bzero(message, (size_t) probeSize);
    strcpy(message,"END");
    //zaslání zprávy pro vyskočení ze smyčky pro reflektování
    if (sendto(client_socket, message, probeSize, 0, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        perror("ERROR: sendto");
        return 1;
    }
    avrgSpeed = avrgSpeed/10.0f;
    free(message);
    free(message_recv);
    printf("Caculating stats ... \n");
    double avrgRtt = 0;

    char *string = (char *)malloc(probeSize);
    //vypočet rtt
    while(queue.front != NULL){
        long secSend;
        long usecSend;
        long secRc;
        long usecRc;
        bzero(string,probeSize);
        QueueFrontPop(&queue,&secRc,&usecRc,&string);
        if(strcmp(string,"none")== 0){
            recievedPackets--;
            continue;
        }
        char *token = strtok(string,"#");
        secSend = strtol(token,NULL,10);
        token = strtok(NULL,"#");
        usecSend = strtol(token,NULL,10);
        double rtt = (secRc + usecRc/1000000.0)-(secSend+usecSend/1000000.0);
        avrgRtt += rtt;
    }
    avrgRtt = (avrgRtt*1000.0)/recievedPackets;
    //printf("recieved ... %d\n",recievedPackets);
    float stdevSpeed = 0;
    //výpočet směrodatné odchylky
    while (measureResults.front != NULL){
        float speed;
        QueueFrontPopMeasure(&measureResults,&speed);
        stdevSpeed += powf(speed-avrgSpeed,2);
    }
    stdevSpeed = sqrtf(stdevSpeed/10.0f);
    //vypsání na výstup
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

