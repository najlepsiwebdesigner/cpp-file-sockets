#define PORT 20000 
#define BACKLOG 5
#define LENGTH 512 

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <resolv.h>



int main ()
{
int sockfd; 
int nsockfd; 
int num;
socklen_t sin_size; 
struct sockaddr_in addr_local; /* client addr */
struct sockaddr_in addr_remote; /* server addr */
char revbuf[LENGTH];

/* Get the Socket file descriptor */
if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
{
    fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
    exit(1);
}
else 
    printf("[Server] Obtaining socket descriptor successfully.\n");

/* Fill the client socket address struct */
addr_local.sin_family = AF_INET; // Protocol Family
addr_local.sin_port = htons(PORT); // Port number
addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

/* Bind a special Port */
if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
{
    fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
    exit(1);
}
else 
    printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);

/* Listen remote connect/calling */
if(listen(sockfd,BACKLOG) == -1)
{
    fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
    exit(1);
}
else
    printf ("[Server] Listening the port %d successfully.\n", PORT);

int success = 0;
while(success == 0)
{
    sin_size = sizeof(struct sockaddr_in);

    /* Wait a connection, and obtain a new socket file despriptor for single connection */
    if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
    {
        fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
        exit(1);
    }
    else 
        printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));


            char buffer[256];
            // bzero(buffer,256);
            int n = 0;
            n = read(nsockfd, buffer, 255);
            if (n < 0) printf("ERROR reading from socket");
            printf("msg: %s\n",buffer);

    /*Receive File from Client */
    const char* fr_name = "house-received.jpg";
    FILE *fr = fopen(fr_name, "a");
    if(fr == NULL)
        printf("File %s Cannot be opened file on server.\n", fr_name);
    else
    {
        bzero(revbuf, LENGTH); 
        int fr_block_sz = 0;
        while((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0) 
        {
            int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
            if(write_sz < fr_block_sz)
            {
                printf("File write failed on server.\n");
            }
            bzero(revbuf, LENGTH);
            if (fr_block_sz == 0 || fr_block_sz != 512) 
            {
                break;
            }
        }
        if(fr_block_sz < 0)
        {
            if (errno == EAGAIN)
            {
                printf("recv() timed out.\n");
            }
            else
            {
                fprintf(stderr, "recv() failed due to errno = %d\n", errno);
                exit(1);
            }
        }
        printf("Ok received from client!\n Bye!");
        
        success = 1;
        fclose(fr); 
    }
}
}