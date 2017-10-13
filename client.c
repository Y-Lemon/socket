 #include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define err_exit(m)\
        {\
                perror(m);\
                exit(EXIT_FAILURE);\
        }

#define SERV_PORT 9877

#define BUFSIZE 4096

//�ͻ��˾����������
void str_cli(FILE *fp, int sockfd);

int main(int argc, char **argv)
{
        if (argc != 2)
        {
                printf("argument error\n");
                exit(0);
        }
        int sockfd;
        struct sockaddr_in servaddr;
        int status;

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);//�����ʮ����IP��ַת��Ϊ�����ֽ���Ķ����Ƶ�ַ

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
                err_exit("socket");
        status = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));//���ӷ�����
        if (sockfd == -1)
                err_exit("connect");
        str_cli(stdin, sockfd);
        exit(0);
}

void str_cli(FILE *fp, int sockfd)
{
        printf("connection success! \n");
        char send[BUFSIZE], recive[BUFSIZE];
        while (fgets(send, BUFSIZE, fp) != NULL)
        {
                write(sockfd, send, strlen(send));
                read(sockfd, recive, BUFSIZE);
                fputs(recive, stdout);
                bzero(recive, BUFSIZE);
        }
}
