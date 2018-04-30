#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXDATASIZE 2048

int main(int argc, char *argv[])
{

  int sockFd = -1, close_con_flag = 0, numReadBytes = 0;
  struct sockaddr_in serverAddr;
  char msg_buf[MAXDATASIZE];

  if(argc != 3)
  {
    printf("Error: Wrong usage\nEx:\n\t./bot_client <server_ip> <port>\n");
    exit(-1);
  }

  if((sockFd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket");
    exit(-1);
  }

  serverAddr.sin_family = AF_INET;
  if((inet_aton(argv[1], (struct in_addr *)&(serverAddr.sin_addr)) == 0))
  {
    printf("Error:  In converting IP adderess [%s] to binary format", argv[1]);
  }
  serverAddr.sin_port = htons(atoi(argv[2]));
  memset(&(serverAddr.sin_zero), 0, 8);

  if(connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
  {
    perror("connect");
    exit(-1);
  }

  while(argv[2])
  {

    if((numReadBytes=recv(sockFd, msg_buf, MAXDATASIZE - 1, 0)) < 0)
    {
      perror("recv");
      exit(-1);
    }

    msg_buf[numReadBytes]='\0';
    printf("%s",msg_buf);
    msg_buf[0]='\0';

    if(close_con_flag == 1)
    {
      break;
    }

    fgets(msg_buf, MAXDATASIZE, stdin);

    if((numReadBytes=send(sockFd, msg_buf, strlen(msg_buf), 0)) < 0)
    {
      perror("send");
      exit(-1);
    }

    if(!strcmp(msg_buf,"CLOSE_CON\n"))
    {
      close_con_flag=1;
    }

    msg_buf[0]='\0';
  }
  close(sockFd);
}
