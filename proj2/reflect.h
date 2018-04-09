/**
 * Hlavičkový soubor pro reflektor
 * Autor: Jan Vávra
 * Login: xvavra20
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>
#include <ctype.h>
#include <signal.h>

void reflect(char *port);
