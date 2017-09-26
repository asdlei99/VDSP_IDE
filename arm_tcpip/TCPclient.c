// system types
#include <sys/types.h>    
// socket API
#include <sys/socket.h>   
// struct sockaddr_in
#include <netinet/in.h>   
// inet_ntoa()
#include <arpa/inet.h>    
// system call
#include <unistd.h>       
// struct hostent and gethostbyname()
#include <netdb.h>        
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
extern int errno;
#define BUFFSIZE 128
#define PORT 6666

// Print error information
int errexit(const char* format, ...); 
// Print work information
int echo(const char* format, ...);    

char* getMessage(char* buffer, int len, FILE* fp)
{
    echo(">> Input message: ");
    return fgets(buffer, len, fp);
}

// Handle socket session
void process(FILE *fp, int sockfd)
{
    char sendline[BUFFSIZE]="Rokid vsp";
    send(sockfd, sendline, strlen(sendline), 0);
}

// Main function
int main(int argc,char* argv[])
{
    int sock;
    struct hostent *hent;
    struct sockaddr_in server;
    char *host = "127.0.0.1";
    unsigned short port = PORT;
    switch (argc)
    {
        case 1:
            break;
        case 2:
            host = argv[1];
            break;
        case 3:
            host = argv[1];
            port = atoi(argv[2]);
            break;
        default:
            errexit("usage: %s [host [port]]\n", argv[0]);
    }
    // convert decimal IP to binary IP
    if ((hent = gethostbyname(host)) == NULL)  
        errexit("gethostbyname failed.\n");
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        errexit("create socket failed: %s\n", strerror(errno));

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr*)hent->h_addr);
    echo("[CLIENT] server addr: %s, port: %u\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
        errexit("connect to server failed: %s\n", strerror(errno));

    echo("[CLIENT] connected to server %s\n", inet_ntoa(server.sin_addr));
    // Send request to server
    process(stdin, sock);                      
    close(sock);
}

