#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>

void * duplex(void * sockfd);

int main(int argc, char ** argv)
{
    int sockfd, n, port;
    char sendln[100], recvln[100];
    char * ipaddr;
    struct sockaddr_in servaddr;
    pthread_t t0;
    
    
    if(argc == 3)
    {
        ipaddr = malloc(sizeof(char) * strlen(argv[1]));
        strcpy(ipaddr, argv[1]);
        port = atoi(argv[2]);
    }
    
    else
    {
        ipaddr = malloc(sizeof(char) * 16);
        printf("Usage: ./<program> <ip_address> <port_number>\nContinuing with defaults.\n\n");
        strcpy(ipaddr,"127.0.0.1");
        port = 20000;
    }
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr) );
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    
    inet_pton(AF_INET, ipaddr, &(servaddr.sin_addr) );
    
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        printf("Error connecting\n");
        close(sockfd);
        exit(1);
        
    }
    
    pthread_create(&t0, NULL, duplex, &sockfd);
    
    printf("Connected to server\n");
    printf("The server is waiting for client message\n");
    while(1)
    {
        bzero(recvln, 100);

        read(sockfd, recvln, 100);
        if (strlen(recvln) == 0)
        {
            printf("Server disconnected\n");
            break;
        }
        
        
        printf("\nServer: %s", recvln);
        
    }
    
    close(sockfd);
}

void * duplex(void * sockfd)
{
    int descriptor = *(int *)sockfd;
    char send[100];
    
    while(1)
    {

        bzero(send, 100);
    
        fgets(send, 100, stdin);
    
        while(strlen(send) == 1)
            fgets(send, 100, stdin);
        
        //write(descriptor, "Client: ", strlen("Client: ")+ 1);
        write(descriptor, send, strlen(send)+1);
    }
    return NULL;
}
