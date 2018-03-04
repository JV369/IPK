#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    FILE *fd;
    char line[255];
    char *login = "rysavy";
    char result[1024];
    fd = fopen("/etc/passwd","r");
    while((fgets(line,255,fd)) != NULL){
        char *token = strtok(line,":");
        if(strcmp(token,login) == 0){
            for(int i = 0; i < 3; i++) {
                token = strtok(NULL, ":");
            }
            strcpy(result,"SEND_END/");
            strcat(result,token);
            printf("%s\n",result);
            fclose(fd);
            return 0;
        }
    }
    strcpy(result,"NOT_FOUND/");
    printf("%s\n",result);
    fclose(fd);
    return 1;
}