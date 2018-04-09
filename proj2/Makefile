#
#
#
ALL: ipk-mtrip

ipk-mtrip:
	gcc -g -std=gnu99 -Wall -pedantic -o $(@) ipk-mtrip.c meter.c reflect.c queue.c -lpthread -lm

zip:
	zip xvavra20.zip meter.c meter.h reflect.c reflect.h queue.c queue.h ipk-mtrip.c Makefile dokumentace.pdf README.md

clean:
	rm -f ipk-mtrip

