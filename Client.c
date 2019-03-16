#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <netdb.h>
#define MAX 80




void parsed_args(int argc, char **argv);
void * read_input(void *ptr);
void * write_output(void *ptr);
void send_recv(int newfd);


pthread_mutex_t stdout_lock;
struct sockaddr_in self_addr;
struct sockaddr_in server_info;
  

pthread_t input_thread;
pthread_t output_thread;
char recvBuff[1000];
char sendBuff[1000];

int main(int argc, char **argv)
{
  //peer_info=self_addr;
  
  int sock,n,connfd;
  sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
  {
    printf("\nERROR IN CREATING SOCKET.\n");
	}
  else
  {
    printf("\nSOCKET CREATED.\n");
  }
  
   parsed_args(argc,argv);
  
  if(bind(sock,(struct sockaddr *)&self_addr,sizeof(self_addr))<0)
  {
    printf("\nERROR IN BINDING\n");
  }
  else
  {
    printf("\n Binded to Port %x \n", self_addr.sin_port);
  }
  printf("\nCLIENT ADDRESS: %x %x %x\n",self_addr.sin_family,self_addr.sin_port, self_addr.sin_addr.s_addr);
  
  connfd=connect(sock, (struct sockaddr *)&server_info, sizeof(server_info));
  
  if(connfd<0)
    {
      printf("\n Error : Connect Failed \n");
     
    }
  else
  {
    printf("\nConnected to Server...\n");
  }
  
  read(sock, recvBuff, sizeof(recvBuff)-1);
   printf("From Server : %s",recvBuff);
 
   send_recv(sock);
  
}

 void parsed_args(int argc, char **argv)
{
    if(argc!=2)
    {
      printf("\nAURGUMENT COUNT IS INCORRECT.\n");
    }

    short self_port=atoi(argv[1]);
    self_addr.sin_family=AF_INET;
    self_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    self_addr.sin_port=htons(self_port);
  
    short server_port=7430;
    server_info.sin_family=AF_INET;
    server_info.sin_addr.s_addr=htonl(INADDR_ANY);
    server_info.sin_port=htons(server_port);
  
   printf("\n SERVER INFO : %x %x %x\n",server_info.sin_family,server_info.sin_port, server_info.sin_addr.s_addr);
  
}

void send_recv(int newfd)
{
  char buff[MAX];
  int n, flag1=0;
  
  while(flag1==0)
  {
    bzero(buff,sizeof(buff));
    
    printf("Enter the string : ");
    n=0;
    while((buff[n++]=getchar())!='\n');
   
    write(newfd,buff,sizeof(buff));
    
    bzero(buff,sizeof(buff));
    read(newfd,buff,sizeof(buff));
    
    printf("From Server : %s",buff);
    if((strncmp(buff,"exit",4))==0)
    {
      printf("Client Exit...\n");
      flag1=1;
    }

  }
  
  close(newfd);
}


