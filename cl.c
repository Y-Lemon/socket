#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORT 9877
#define BUF_SIZE 1024

void str_cli(FILE *fp, int sockfd);

int
main(int argc, char *argv[])
{
    int sockfd;
    int send_data;
    int recv_data;
    char buffer[BUF_SIZE];
    struct sockaddr_in servaddr;

    if(argc != 2)
    {
        perror("usage: tcpcli <IPaddress>");
        exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
         perror("socket error!");
         exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s", argv[1]);
        exit(1);
    }

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {        perror("connect error!");
    }

    str_cli(stdin, sockfd);

    exit(0);
}

void
str_cli(FILE *fp, int sockfd)
{
    pid_t pid = fork();
    int recv_data, send_data;
    char buf[BUF_SIZE];
    if(pid > 0)
    {
        memset(buf, 0, sizeof(buf));
        while((recv_data = recv(sockfd, buf, BUF_SIZE, 0)) != -1)
        {
            printf("Receive a message:%s\n", buf);
            sleep(1);
        }
        if(recv_data == -1)
        {
            perror("recv error!");
            exit(1);
        }
        exit(0);
    }
    else if(pid == 0)
    {
        memset(buf, 0, sizeof(buf));
        do
        {
            if(fgets(buf, BUF_SIZE, fp) == NULL)
            {
                perror("fgets error!");
                exit(1);
            }
        }while((send_data = send(sockfd, buf, strlen(buf), 0)) != -1);
        if(send_data == -1)
        {
            perror("write error!");
            exit(1);
        }
        putchar(10);
        exit(0);
    }
    else
    {     perror("fork error");
        exit(1);
    }
    close(sockfd);
}
