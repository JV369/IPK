/**
 * Main pro ipk-mtrip
 * Autor: Jan Vávra
 * Login: xvavra20
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "meter.h"
#include "reflect.h"

/**
 * @brief zkontroluje jestli je řetězec číslo
 * @param arg řetězec znaků
 * @return 0 pokud je řetězec číslo, jinak 1
 */
int isNum(char *arg){
    for (int i = 0; i < strlen(arg); ++i) {
        if(!isdigit(arg[i])){
            return 1;
        }
    }
    return 0;
}


/**
 * vlastní getopt
 * @param arguments argumenty na vstupu
 * @param lenght počet argumentu
 * @param port port na které bude probíhat připojení
 * @param address adresa reflektoru
 * @param probeSize velikost probe paketu
 * @param time doba probíhání měření
 * @return vrátí 1 pokud se jedná o argumenty pro reflektor, 2 pro meter, jinak 0
 */
int checkArg(char **arguments,int lenght, char **port, char **address, long *probeSize,long *time){
    if (!(lenght == 4 || lenght == 10)){
        return 0;
    }
    int meterFlag = 0;
    int reflFlag = 0;
    int portFlag = 0;
    int probeFlag = 0;
    int timeFlag = 0;
    int addressFlag = 0;

    for (int i = 1; i < lenght ; i += 2) {
        if (strcmp(arguments[i],"reflect" ) == 0 && !reflFlag){
            reflFlag = 1;
            i--;
        }
        else if(strcmp(arguments[i],"meter") == 0 && !meterFlag){
            meterFlag = 1;
            i--;
        }
        else if (strcmp(arguments[i],"-p") == 0 && i+1 < lenght && !portFlag){
            if(isNum(arguments[i+1])) {
                return 0;
            }
            portFlag = 1;
            *port = (char *)malloc(strlen(arguments[i+1])+1);
            strcpy(*port,arguments[i+1]);
        }
        else if (strcmp(arguments[i],"-h") == 0 && i+1 < lenght && !addressFlag){
            addressFlag = 1;
            *address = (char *)malloc(strlen(arguments[i+1])+1);
            strcpy(*address,arguments[i+1]);
        }
        else if(strcmp(arguments[i],"-s" ) == 0 && i+1 < lenght && !probeFlag){
            if(isNum(arguments[i+1])){
                return 0;
            }
            probeFlag = 1;
            *probeSize = strtol(arguments[i+1],NULL,10);
        }
        else if (strcmp(arguments[i],"-t") == 0 && i+1 < lenght && !timeFlag){
            if(isNum(arguments[i+1])){
                return 0;
            }
            timeFlag = 1;
            *time = strtol(arguments[i+1],NULL,10);
        }
        else{
            return 0;
        }

    }
    if(portFlag && reflFlag){
        return 1;
    }
    else if (meterFlag){
        return 2;
    }
    else{
        if(addressFlag){
            free(address);
        }
        if(portFlag){
            free(port);
        }
        return 0;
    }
}

int main(int argc, char* argv[]) {
    long probeSize;
    char *hostname;
    char *port;
    long time;
    int retCode = checkArg(argv,argc,&port,&hostname,&probeSize,&time);
    if(retCode == 1){
        reflect(port);
        return 0;
    }
    else if(retCode == 2){
        if(meter(hostname,port,probeSize,time) != 0)
            return 1;
        return 0;
    }
    else{
        fprintf(stderr,"Wrong arguments\n");
        return 1;
    }
}
