#define PORT 20000
#define LENGTH 512 

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>



int main(int argc, char *argv[]){
int sockfd; 
int nsockfd;
char revbuf[LENGTH]; 
struct sockaddr_in remote_addr;

/* Get the Socket file descriptor */
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
{
    fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",1);
    std::exit(1);
}

/* Fill the socket address struct */
remote_addr.sin_family = AF_INET; 
remote_addr.sin_port = htons(PORT); 
inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr); 
bzero(&(remote_addr.sin_zero), 8);

/* Try to connect the remote */
if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
{
    fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",2);
    exit(1);
}
else 
    printf("[Client] Connected to server at port %d...ok!\n", PORT);

/* Send File to Server */
//if(!fork())
//{
    const char* fs_name = "house.jpg";
    char sdbuf[LENGTH]; 

            char buffer[256];
            int n;
            fgets(buffer,255,stdin);
            // bzero(buffer,256);
            n = write(sockfd,buffer, strlen(buffer));
            if(n<0) printf("Error: sending filename");

    printf("[Client] Sending %s to the Server... ", fs_name);
    FILE *fs = fopen(fs_name, "r");
    if(fs == NULL)
    {
        printf("ERROR: File %s not found.\n", fs_name);
        exit(1);
    }

    bzero(sdbuf, LENGTH); 
    int fs_block_sz;
    while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
    {
        if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
        {
            fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
            break;
        }
        bzero(sdbuf, LENGTH);
    }
    printf("Ok File %s from Client was Sent!\n", fs_name);
//}


close (sockfd);
printf("[Client] Connection lost.\n");
return (0);
}