#
#
#
ALL: ipk-mtrip

ipk-mtrip:
	gcc -g -std=c99 -Wall -pedantic -o $(@) ipk-mtrip.c meter.c reflect.c queue.c -D_XOPEN_SOURCE=600 -lpthread -lm

zip:
	zip xvavra20.zip meter.c meter.h reflect.c reflect.h ipk-mtrip.c Makefile dokumentace.pdf README.md

clean:
	rm -f ipk-mtrip

