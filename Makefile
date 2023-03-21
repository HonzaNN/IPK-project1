#compile ipk-proj1.c
all:
	gcc -std=c99 ipk-proj1.c -o ipkcpc

run:
	gcc -std=c99 ipk-proj1.c -o ipkcpc
	./ipkcpc -h merlin.fit.vutbr.cz -p 10002  -m udp