#
#
#
ALL: client  server
client:
	gcc client.c -g -std=c99 -Wall -pedantic -o ipk-client -D_XOPEN_SOURCE=600 -lpthread


server:
	gcc server.c -g -std=c99 -Wall -pedantic -o ipk-server -D_XOPEN_SOURCE=600 -lpthread

test:
	gcc -g -std=c99 -Wall -pedantic -o test test.c -D_XOPEN_SOURCE=600 -lpthread

clean:
	rm -f ipk-client ipk-server

