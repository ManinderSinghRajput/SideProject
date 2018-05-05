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
#include <pthread.h>

#define MAXDATASIZE 2048
#define PORT 14182
#define NUM_THREADS 2

typedef struct _thread_details{
  int port;
  int conn_fd;
  struct sockaddr_in *clientAddr;
}thread_details;

void *talk_to_bot_client(void *comm_essentials)
{
  thread_details *comm_details = (thread_details *) comm_essentials;
  int port = comm_details->port;
  int connFd = comm_details->conn_fd;
  char clientName[1024] = {0};
  char buf[2048] = {0}; 
  int numReadBytes = 0;
  time_t timer;
  struct sockaddr_in *clientAddr = comm_details->clientAddr;
  
  sprintf(buf, "[Server] : May I know who are you ? Name would be fine.\n");

  if ((send(connFd, buf, strlen(buf), 0)) < 0)
  {
    perror("send");
    exit(-1);
  }

  buf[0] = '\0';

  if((numReadBytes = recv(connFd, buf, MAXDATASIZE-1, 0)) < 0)
  {
    perror("recv");
    exit(-1);
  }

  buf[numReadBytes - 1]='\0';

  strcpy(clientName,buf);

  buf[0] = '\0';

  timer = time(NULL); 
  printf("INFO->Connection established with %s from IP %s at %s", clientName, inet_ntoa(clientAddr->sin_addr), ctime(&timer));

  sprintf(buf, "INFO->Connection is successfully established with Server. Please use enter to send the message.\n");

  if ((send(connFd, buf, strlen(buf), 0)) < 0)
  {
    perror("send");
    exit(-1);
  }

  buf[0] = '\0';
  
  while(port)
  {
    if((numReadBytes = recv(connFd, buf, MAXDATASIZE-1, 0)) < 0)
    {
      perror("recv");
      exit(-1);
    }
  
    buf[numReadBytes]='\0';
  
    if(strcmp("CLOSE_CON\n",buf) == 0)
    {
      buf[0] = '\0';
      printf("INFO->Recieved CLOSE_CON from client %s. Hence closing connection.\n", clientName);
      sprintf(buf,"[Server] : Recieved CLOSE_CON from your side so closing connection. Bye!!\n");
      if ((send(connFd, buf, strlen(buf), 0)) < 0)
      {
        perror("send");
        exit(-1);
      }
      break;
    }
    else
    {
      printf("[%s] : %s", clientName, buf);
      buf[0]='\0';
    }
   
    sprintf(buf,"[Server] : Length of message recieved from your side is: %d\n", numReadBytes-1);
    if ((send(connFd, buf, strlen(buf), 0)) < 0)
    {
      perror("send");
      exit(-1);
    }
    buf[0]='\0';
  }
  close(connFd);
  pthread_exit(NULL);
}


int main( int argc, char *argv[])
{
  int sockFd = -1, connFd = -1, enable = 1, i = 0;
  socklen_t sinSize;
  struct sockaddr_in serverAddr;
  struct sockaddr_in clientAddr;
  int port=PORT;
  pthread_t thr[NUM_THREADS];
  thread_details thr_data[NUM_THREADS];

  int thread_used_count = 0;
  

  if(argc > 1)
    port=atoi(argv[1]);

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
  serverAddr.sin_port = htons(port);;
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

  while(port)
  {
    sinSize = sizeof(struct sockaddr);
    if((connFd = accept(sockFd, (struct sockaddr*)&clientAddr, &sinSize)) < 0)
    {
      perror("accept");
      exit(-1);
    }

    thr_data[thread_used_count].port = port;
    thr_data[thread_used_count].conn_fd = connFd;
    thr_data[thread_used_count].clientAddr = &clientAddr;
   
    pthread_create(&(thr[thread_used_count]), NULL, &(talk_to_bot_client), &(thr_data[thread_used_count]));
    thread_used_count++; 
   
    if(thread_used_count == NUM_THREADS)
      break;
  }

  for(i=0;i<thread_used_count;i++)
  {
    pthread_join(thr[i], NULL);
  }

  close(sockFd);
  return 0;
}
