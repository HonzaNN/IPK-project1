#compile ipk-proj1.c
all:
	gcc -std=c99 ipk-proj1.c -o ipkcpc

run:
	gcc -std=c99 ipk-proj1.c -o ipkcpc
	./ipkcpc -h 127.0.0.1 -p 667  -m udp