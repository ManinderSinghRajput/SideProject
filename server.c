#include <stdio.h>		//Basic i/o
#include <errno.h>		//perror
#include <sys/types.h>          //socket,bind
#include <sys/socket.h>		//socket,bind
#include <stdlib.h>		//exit
#include <string.h>
#include <time.h>		//time_t
#include <netinet/in.h>		//struct sockaddr 
#include <netinet/in.h>		//inet_ntoa()
#include <arpa/inet.h>		//inet_ntoa()
#include <unistd.h>		//close()

#define MAXDATASIZE 500 // max number of bytes we can get at once

int main()
{
  //socket
  int sockfd = -1, connfd = -1;
  struct sockaddr_in my_addr;
  struct sockaddr_in client_addr;
  socklen_t sin_size;
  int numbytes;
  char buf[MAXDATASIZE];
  char tmpbuf[MAXDATASIZE];
  time_t timer;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)			//AF_INET-Ipv4 Communication, SOCK_STREAM-two way connection base reliable connection 0 will use default protocol in this case that is TCP
  {
    perror("socket");
    exit(1);
  }

  /*
    struct sockaddr {
      unsigned short sa_family; 	//Address family (e.g. AF_INET)
      char sa_data[14];			//Family-specific address information(14 bytes)
    }
   
    Particular form of sockaddr used for TCP/IP addresses:
    struct in_addr {
      unsigned long s_addr;		//Internet address (32 bits/4 bytes)
    }
    
    struct sockadd_in {
      unsigned short sin_family;	//Internet protocol (AF_INET)	
      unsigned short sin_port;		//Address port(16bits/2bytes)
      struct in_addr sin_addr;		//Internet address (32bits/8bytes)
      char sin_zero[8];			//Not Used
    }
  */
  my_addr.sin_family = AF_INET; // host byte order
  my_addr.sin_port = htons(18059); // short, network byte order
  my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
  memset(&(my_addr.sin_zero), 0, 8); // zero the rest of the struct

  //bind
  /*
    sockid: integer,socket descriptor
    addrport: struct sockaddr, the (IP) address and port of the machine
      for TCP/IP server ,internet address is usually set t INADDR_ANY i.e., choose any incoming interface
    size: the size (in bytes) of the addrport structure
  */
  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
  {
    perror("bind");
    exit(1);
  }

  //listen
  if (listen(sockfd, 10) == -1)
  {
    perror("listen");
    exit(1);
  }
  while(1)
  {
    //accept
    sin_size = sizeof(struct sockaddr_in);
    if ((connfd = accept(sockfd, (struct sockaddr *)&client_addr,&sin_size)) == -1)
    {
      perror("accept");
      continue;
    } 
  
    printf("Received request from Client: %s\n",inet_ntoa(client_addr.sin_addr));
    if (!fork())
    { // this is the child process
      close(sockfd); // child doesn.t need the listener*/
      while(1)
      {
        if ((numbytes=recv(connfd, buf, MAXDATASIZE-1, 0)) == -1)
        {
          perror("recv");
          exit(1);
        }
   
        printf("Message recieved is [%s]\n",buf);
        //printf("%s",buf);
	
        timer = time(NULL);
        //if (send(connfd, ctime(&timer), 30, 0) == -1)
        sprintf(tmpbuf,"You message is recieved:[%s]",buf);
        if (send(connfd, tmpbuf, 300, 0) == -1)
        {
          perror("send");
          exit(1);
        }
      }
      close(connfd);
      exit(0);    
    }
  }
  return 0;
}

