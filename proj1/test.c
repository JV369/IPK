#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char test[200] = "hue#";
    printf("%s\n",test);
    char *token = strtok(test,"#");
    printf("|%s|\n",token);
    token = strtok(NULL,"#");
    printf("|%s|\n",token);
    return 0;
}