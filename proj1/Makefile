#
#
#
ALL: client  server
client:
	gcc -g -std=c99 -Wall -pedantic -o client client.c -E -D_XOPEN_SOURCE=600 -lpthread


server:
	gcc -g -std=c99 -Wall -pedantic -o server server.c -E -D_XOPEN_SOURCE=600 -lpthread

