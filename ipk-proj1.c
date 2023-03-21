//import libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>


#define BUFSIZE 512


/// @brief Check if imput arguments are simmilar to the format "-h <host IPv4> -p <port number> -m <mode>"
/// @param argc 
/// @param argv 
/// @return 
bool checkArgs(int argc, char *argv[]){
    if(argc != 7){
        fprintf(stderr, "Wrong number of arguments!\n");
        return false;
    }
    if(argv[1][0] != '-' || argv[1][1] != 'h'){
        fprintf(stderr, "Wrong format of arguments!\n");
        return false;
    }


    if(argv[3][0] != '-' || argv[3][1] != 'p'){
        fprintf(stderr, "Wrong format of arguments!\n");
        return false;
    }

    char *endptr;
    long port = strtol(argv[4], &endptr, 10);
    if(*endptr != '\0' || port < 0 || port > 65535){
        fprintf(stderr, "Wrong format of port number!\n");
        return false;
    }

    if(argv[5][0] != '-' || argv[5][1] != 'm'){
        fprintf(stderr, "Wrong format of arguments!\n");
        return false;
    }

    if(strcmp(argv[6], "tcp") != 0 && strcmp(argv[6], "udp") != 0){
        fprintf(stderr, "Wrong format of arguments!\n");
        return false;
    }

    return true;
    
}

void handle_sigint(int sig){
}

void UDP_cominucation(struct sockaddr_in server_address){

    int client_socket, bytestx, bytesrx;
    socklen_t serverlen;
    struct hostent *server;
    char buf[BUFSIZE];
    while (true)
    {
    
        /* tiskne informace o vzdalenem soketu */ 
        printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
        
        /* Vytvoreni soketu */
        if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
        {
            perror("ERROR: socket");
            exit(EXIT_FAILURE);
        }
            
        /* nacteni zpravy od uzivatele */
        bzero(buf, BUFSIZE);
        printf("Please enter msg: ");
        char *in_char = fgets(buf+2, BUFSIZE-2, stdin);

        buf[0] = 0x00;
        buf[1] = strlen(buf + 2);

        /* odeslani zpravy na server */
        serverlen = sizeof(server_address);
        bytestx = sendto(client_socket, buf, strlen(buf + 2) + 2, 0, (struct sockaddr *) &server_address, serverlen);
        if (bytestx < 0) 
        perror("ERROR: sendto");
        
        /* prijeti odpovedi a jeji vypsani */
        bytesrx = recvfrom(client_socket, buf, BUFSIZE, 0, (struct sockaddr *) &server_address, &serverlen);
        if (bytesrx < 0) 
        perror("ERROR: recvfrom");
        buf[5] = '\0';
        printf("Echo from server: %s", buf+3);
        
    }

}

void TCP_cominucation(struct sockaddr_in server_address){

    signal(SIGINT, handle_sigint);
    while (1);

    int client_socket, bytestx, bytesrx;
    socklen_t serverlen;
    struct hostent *server;
    char buf[BUFSIZE];
    while (true)
    {
    
        /* tiskne informace o vzdalenem soketu */ 
        printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
    
        /* Vytvoreni soketu */
        if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
        {
            perror("ERROR: socket");
            exit(EXIT_FAILURE);
        }
            
        /* nacteni zpravy od uzivatele */
        bzero(buf, BUFSIZE);
        printf("Please enter msg: ");
        fgets(buf, BUFSIZE, stdin);
        
        if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
        {
            perror("ERROR: connect");
            exit(EXIT_FAILURE);        
        }

        /* odeslani zpravy na server */
        bytestx = send(client_socket, buf, strlen(buf), 0);
        if (bytestx < 0) 
        perror("ERROR in sendto");
        
        /* prijeti odpovedi a jeji vypsani */
        bytesrx = recv(client_socket, buf, BUFSIZE, 0);
        if (bytesrx < 0) 
        perror("ERROR in recvfrom");
        
        printf("Echo from server: %s", buf);
            

        
    }
}

int main(int argc, char *argv[]){
    if(!checkArgs(argc, argv)){
        return 1;
    }


    int port_number;
    const char *server_hostname;
    struct hostent *server;
    struct sockaddr_in server_address;
     
   
    server_hostname = argv[2];
    port_number = atoi(argv[4]);
    
    /* 2. ziskani adresy serveru pomoci DNS */
    
    if ((server = gethostbyname(server_hostname)) == NULL) {
        fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
        exit(EXIT_FAILURE);
    }
    
    /* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);
   
    if(strcmp(argv[6], "tcp") == 0){
        TCP_cominucation(server_address);
    }
    else{
        UDP_cominucation(server_address);
    }
    
    
}