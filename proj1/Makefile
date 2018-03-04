#
#
#
ALL: client  server
client:
	gcc -g -std=c99 -Wall -pedantic -o client client.c -D_XOPEN_SOURCE=600 -lpthread


server:
	gcc -g -std=c99 -Wall -pedantic -o server server.c -D_XOPEN_SOURCE=600 -lpthread

test:
	gcc -g -std=c99 -Wall -pedantic -o test test.c -D_XOPEN_SOURCE=600 -lpthread

clean:
	rm client server

