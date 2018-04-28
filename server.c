#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>
#include <errno.h>

#define MAXDATASIZE 2048


int main( int argc, char *argv[])
{
  int sockFd = -1, connFd = -1, numReadBytes = -1, enable = 1;
  socklen_t sinSize;
  struct sockaddr_in serverAddr;
  struct sockaddr_in clientAddr;
  char buf[MAXDATASIZE] = {0};
  //char msgBuf[MAXDATASIZE] = {0};
  time_t timer;

  if((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Socket");
    exit(-1); 
  }

  if(setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
  {
    perror("setsockopt");
    exit(-1);
  }

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(14182);;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  memset(&(serverAddr.sin_zero), 0, 8);


  if(bind(sockFd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
  {
    perror("bind");
    exit(-1);
  }

  if(listen(sockFd, 10) < 0)
  {
    perror("listen");
    exit(-1);
  }

  int i=1;
  while(i--)
  {
    sinSize = sizeof(struct sockaddr);
    if((connFd = accept(sockFd, (struct sockaddr*)&clientAddr, &sinSize)) < 0)
    {
      perror("accept");
      exit(-1);
    }

    timer = time(NULL); 
    printf("Connection established with %s at %s", inet_ntoa(clientAddr.sin_addr), ctime(&timer));

    sprintf(buf, "Connection is successfully established with Server. Please use enter to send the message.\n");

    if ((numReadBytes=send(connFd, buf, strlen(buf), 0)) < 0)
    {
      perror("send");
      exit(-1);
    }

    buf[0] = '\0';

    int j=1;
    while(j)
    {
      if((numReadBytes = recv(connFd, buf, MAXDATASIZE-1, 0)) < 0)
      {
        perror("recv");
        exit(-1);
      }

      buf[numReadBytes]='\0';

      if(strcmp("CLOSE_CON",buf) == 0)
      {
        printf("Recieved CLOSE_CON from client %s. Hence closing connection.\n", inet_ntoa(clientAddr.sin_addr));
        break;
      }
      else
      {
        printf("[%s] : %s", inet_ntoa(clientAddr.sin_addr), buf);
        buf[0]='\0';
      }
     
      sprintf(buf,"[Server] : Length of message recieved from your side is: %d\n", numReadBytes-1);
      if ((numReadBytes=send(connFd, buf, strlen(buf), 0)) < 0)
      {
        perror("send");
        exit(-1);
      }
      buf[0]='\0';
    }
    close(connFd);
  }
  close(sockFd);
  return 0;
}
