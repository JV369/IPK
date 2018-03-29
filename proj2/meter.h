//
// Created by jan on 28.3.18.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include "queue.h"

int meter(char *hostname,char *port,long probeSize,long time);
