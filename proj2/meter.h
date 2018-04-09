/**
 * Hlavičkový soubor pro měřák
 * Autor: Jan Vávra
 * Login: xvavra20
 */
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
#include <math.h>
#include <limits.h>
#include <sys/mman.h>
#include <signal.h>

int meter(char *hostname,char *port,long probeS,long time);
