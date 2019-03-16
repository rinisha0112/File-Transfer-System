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
void * send_recv(int newfd,int j);


pthread_mutex_t stdout_lock;
struct sockaddr_in self_addr;
struct sockaddr_in client_addr;
  

int client_len,sock;
//pthread_t input_thread;
//pthread_t output_thread;
pthread_t send_receive[10];
char recvBuff[1000];
char sendBuff[1000];

int main(int argc, char **argv)
{
      //peer_info=self_addr;
    int connfd,n,stop;
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
      printf("\nSERVER ADDRESS: %x %x %x\n",self_addr.sin_family,self_addr.sin_port, self_addr.sin_addr.s_addr);

      if(listen(sock, 10) == -1)
      {
          printf("\nFailed to listen\n");

      }
      else
      {
        printf("\nLISTENING...\n");
      }
  
   
  int j=-1;
  while(1)
    {    
      client_len=sizeof(client_addr);
      connfd = accept(sock, (struct sockaddr *)&client_addr ,&client_len);// accept awaiting request
    printf("New ConnFD is %d\n",connfd);
    
    strcpy(sendBuff, "Hii You are connected to server via PORT 7430!\n");
      write(connfd, sendBuff, strlen(sendBuff));
    
   
   
    
     pthread_create(&send_receive[0], NULL, send_recv(connfd,j), NULL);
      
      
      int x;
    
   
 
  }
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
  
  
}

void * send_recv(int newfd, int j)
{
  char buff[MAX];
  int n, flag1=0, flag2=0;
  
  while(flag1==0)
  {
      bzero(buff,MAX);
     
     read(newfd,buff,sizeof(buff));
     
     
      printf("From client: %s\t To client : ",buff);
    
      bzero(buff,MAX);
      n=0;
      while((buff[n++]=getchar())!='\n');
    
      write(newfd,buff,sizeof(buff));
      if((strncmp(buff,"exit",4))==0)
      {
        printf("Client Exit...\n");
        flag1=1;
      }
    
      
  }
 
  
  
}


