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
#define BUFSIZETCP 1024

/// @brief Check if imput arguments are simmilar to the format "-h <host> -p <port number> -m <mode>"
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

/// @brief Function realize UDP communication
/// @param server_address 
void UDP_cominucation(struct sockaddr_in server_address){

    int client_socket, bytestx, bytesrx;
    socklen_t serverlen;
    struct hostent *server;
    char buf[BUFSIZE];
    int lenght;
    
    while (true)
    {
         /** 
         * Vladimir Vesely DemoTcp [online]. 
         * Zdroj: Brno University of Technology, Faculty of Information Technology
         * Ziskano z: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoUdp 
         * Volne citovano se zmenami
        */
        
        /* Vytvoreni soketu */
        if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
        {
            perror("ERROR: socket");
            exit(EXIT_FAILURE);
        }
            
        /* nacteni zpravy od uzivatele */
        bzero(buf, BUFSIZE);
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

        /** Konec citace **/

        lenght = buf[2] + 3;
        if(buf[1] == 0x01){
            buf[lenght] = '\0';
            fprintf(stderr, "ERR:%s", buf + 3);
        }
        else{
            buf[lenght] = '\0';
            printf("OK: %s\n", buf+3);
        }
        
    }

}

/// @brief Function realize TCP communication
/// @param server_address 
void TCP_cominucation(struct sockaddr_in server_address){

    

    int client_socket, bytestx, bytesrx;
    socklen_t serverlen;
    struct hostent *server;
    char buf[BUFSIZETCP];
    int lenght;
    
    /** 
     * Vladimir Vesely DemoTcp [online]. 
     * Zdroj: Brno University of Technology, Faculty of Information Technology
     * Ziskano z: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoTcp
    */

    /* Vytvoreni soketu */
        if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
        {
            perror("ERROR: socket");
            exit(EXIT_FAILURE);
        }
        
        if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
        {
            perror("ERROR: connect");
            exit(EXIT_FAILURE);        
        }

    /** Konec citace **/


    while(true){
        
        /** 
         * Vladimir Vesely DemoTcp [online]. 
         * Zdroj: Brno University of Technology, Faculty of Information Technology
         * Ziskano z: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoTcp
         * Volne citovano se zmenami
        */
         
        bzero(buf, BUFSIZETCP);
        char *in_char = fgets(buf, BUFSIZETCP, stdin);
        
        
        

        /* odeslani zpravy na server */
        bytestx = send(client_socket, buf, strlen(buf), 0);
        if (bytestx < 0) 
        perror("ERROR in sendto");

        /* prijeti odpovedi a jeji vypsani */
        bzero(buf, BUFSIZETCP);
        bytesrx = recv(client_socket, buf, BUFSIZETCP, 0);
        if (bytesrx < 0) 
        perror("ERROR in recvfrom");

        /** Konec citace **/

        lenght = strlen(buf) + 1;
        buf[lenght] = '\0';
        
        printf("%s", buf);
        if(strcmp(buf, "BYE\n") == 0){
            break;
        }
        
    }
    close(client_socket);
    
}


int main(int argc, char *argv[]){
    if(!checkArgs(argc, argv)){
        return 1;
    }

    /** 
     * Vladimir Vesely DemoTcp [online]. 
     * Zdroj: Brno University of Technology, Faculty of Information Technology
     * Ziskano z: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoUdp
     *  
    */
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
    /** konec citace **/
    
    if(strcmp(argv[6], "tcp") == 0){
        TCP_cominucation(server_address);
    }
    else{
        UDP_cominucation(server_address);
    }
    
    
}