#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/select.h>

#define SIZE sizeof(fd_set)*8//最多处理这么多

int fds[SIZE];//文件描述符集合的数组

static void usage(const char *proc)
{
   printf("Usage:%s [local_ip] [local_port]\n",proc);
}
int startup(const char* ip,int port)//获得文件描述符
{
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
      perror("socket");
      exit(2);
    }

    int opt=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(port);
    local.sin_addr.s_addr=inet_addr(ip);
    if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
    {
        perror("bind");
        exit(3);
    }
    if(listen(sock,10)<0)
    {
        perror("listen");
        exit(4);
    }
    return sock;
}
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
      usage(argv[0]);
      return 1;
    }
    int listen_sock=startup(argv[1],atoi(argv[2]));//端口号必须是整形
    //链接来了表示读事件就绪
    int nums=sizeof(fds)/sizeof(fds[0]);
    int i=0;
    //将文件描述符置成无效
    for(;i<nums;i++)
    {
       fds[i]=-1;
    }
    fds[0]=listen_sock;
    printf("before while\n");
    while(1)
    {  
        int max_fd=-1;
        fd_set rfds;//读文件描述符集
        FD_ZERO(&rfds);//每次调用select都要对其清空
        for(i=0;i<nums;i++)
        {
             if(fds[i]>0)
             {
                 FD_SET(fds[i],&rfds);
                 if(max_fd<fds[i])
                 {
                     max_fd=fds[i];
                 }
             }
        }
       struct timeval timeout={2,0};
         
      switch(select(max_fd+1,&rfds,NULL,NULL,NULL/*&timeout*/))
      {
          case 0:
              printf("timeout...\n");
              break;
          case -1:
              perror("select");
              break;
         default:
         {   //at least one fd  ready!
             int i = 0;
             for(;i<nums;i++)
              {
                  if( -1 == fds[i])
                  {
                      continue;
                  }
                  if(i==0&&FD_ISSET(fds[i],&rfds))//listen sock ready!get new connect
                  {
                      struct sockaddr_in client;
                      socklen_t len=sizeof(client);
                      int new_sock=accept(listen_sock,(struct sockaddr*)&client,&len);
                      if(new_sock<0)
                      {
                          perror("accept");
                          continue;
                      }
                      //只是链接就绪了，读写事件并不知道
                  printf("get a new client:[%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
                  //从第三方数组中，找一个合法的位置把new_sock保存
                  int j=1;
                  for(;j<nums;j++)
                  {
                      if(fds[j]==-1)
                      {
                          break;    
                      }
                  }
                  if(j==nums)
                  {
                      printf("client is full\n");
                      close(new_sock);
                      continue;
                  }
                  else
                  {
                      fds[j]=new_sock;
                  }
                  
              }
              else if(i!=0&&FD_ISSET(fds[i],&rfds))// normal fd ready
              {
                  char buf[1024];
                  ssize_t s=read(fds[i],buf,sizeof(buf)-1);
                  if(s>0)
                  {
                      buf[s]=0;
                      printf("client# %s\n",buf);
                  }else if(s==0)
                  {
                      close(fds[i]);
                      fds[i]=-1;
                      printf("client is quit!\n");
                   }
                  else
                  {
                      perror("read");
                      close(fds[i]);
                      fds[i]=-1;
                  }

              }
              else//fd not ready！
              {

              }
         }
      }
         break;
      }
   }
    return 0;
}
