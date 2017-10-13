#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define err_exit(m)\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
}
#define SERV_PORT 9877
#define LISTENQ 1024
#define BUFSIZE 4096

void str_echo(int confd);
int main(int argc,char **argv)
{
    int listenfd;
    int confd;
    int status;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    char buff[BUFSIZE];


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
    {
        err_exit("socket");
    }
    status =bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(status == -1)
    {
         err_exit("bind");
    }
    status=listen(listenfd,LISTENQ);
    if(status == -1)
    {
        err_exit("listen");
    }
    while(1)
    {
        clilen = sizeof(cliaddr);
         childpid = sizeof(cliaddr);
        confd = accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);
        if(status == -1)
        {
             err_exit("accept");
        }
        if((childpid = fork())==0)
        {
            printf("connection from %s,port %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,buff,sizeof(buff)),ntohs(cliaddr.sin_port));

             close(listenfd);
             str_echo(confd);
             close(confd);
             exit(0);
        }
        close(confd);
    }
}

void str_echo(int confd)
{
     ssize_t n;
     char buf[BUFSIZE];
     while((n = read(confd,buf,BUFSIZE))>0)
         write(confd,buf,n);
}


