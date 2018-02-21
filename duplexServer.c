#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>

void * duplex(void * p);

int listenfd, connfd;

int main(int argc, char** argv)
{
    char str[100], rep[100], temp;
    char * ip;
    int i, port;
    pthread_t t0;
    
    printf("Argc is %d\n", argc);
    if(argc == 2)
    {
        port = atoi(argv[1]);
    }//end if
    
    else
    {
        printf("Usage: ./<program> <port_number>\nContinuing with defaults.\n\n");
        port = 20000;
    }//end else
    
    struct sockaddr_in servaddr;
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);
    
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    
    while(1)
    {
        printf("Waiting for client:\n");
        
        if((listen(listenfd, 5))< 0)
            printf("Error listening\n");
        
        if( (connfd = accept(listenfd, (struct sockaddr*) NULL, NULL)) < 0)
            printf("Error accepting\n");
        
        pthread_create(&t0, NULL, duplex, &connfd);
        
        printf("Connection successful:\n");
        while(1)
        {
            
            bzero(str, 100);
            
            if(read(connfd, str, 100) < 0)
                printf("error reading\n");
            
            
            i = strlen(str);
     
            
            if(strlen(str) == 0)
            {
                printf("Client has disconnected:\n");
                shutdown(connfd, 0);
                break;
            }//end inner if
            
            printf("\nClient: %s", str);
            
        }//end inner while
        
        close(connfd);

    }//end outer while
    
    close(listenfd);
    free(ip);
    
    return 0;
}



// Thread for sending to client
void * duplex(void * connfd)
{
    int descriptor = *(int *)connfd;
    char send[100];
    
    while(1)
    {

        bzero(send, 100);
    
        fgets(send, 100, stdin);
    
        while(strlen(send) == 1)
            fgets(send, 100, stdin);

        write(descriptor, send, strlen(send)+1);
    }//end while
    
    return NULL;
}//end Duplex