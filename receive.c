#include<stdio.h>
#include <arpa/inet.h>
#include <string.h>

#include "iolib.h"

#define SERVERPORT 9001
#define MAXCLIENT 20
#define BUFFERSIZE 1024
#define MAXPATHSIZE 100
#define PATHOFFSET 4



int findpathend(char* buf)
{
    int position = 0;
    char*c = buf;
	while( '\0' != *c && '\n' != *c)
	{
		if(' ' == *c)
		return position;
		position++;
		c += 1;
	}
    return -1;
}


int main()
{
	struct sockaddr_in sin;
	struct sockaddr_in cin;
    int sock_fd,new_fd;
	socklen_t sin_size;
	char buf[BUFFERSIZE];
	char filepath [MAXPATHSIZE];
	bzero(&sin,sizeof(sin));
    bzero(buf,BUFFERSIZE);
	bzero(filepath,MAXPATHSIZE);
	sin.sin_family =AF_INET;
	sin.sin_port = htons(SERVERPORT);
	sin.sin_addr.s_addr = INADDR_ANY;


	if((sock_fd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("setsockopt\n");
    	return -1;
	}
	if(bind(sock_fd,(struct sockaddr *)&sin,sizeof(sin)) == -1)
    {
	     printf("bind error!\n");
	     return -1;
	 }
    if(listen(sock_fd,MAXCLIENT)==-1)
    {
	    printf("listen error!\n");
	    return -1;
    }
   while(1)
   {
        if((new_fd = accept(sock_fd,(struct sockaddr *)&cin,&sin_size)) <=0)
        {
	        perror("accept error\n");
			close(new_fd);
	        continue;
        }
        ssize_t len = readn(new_fd,buf,BUFFERSIZE);
	    if(strcmp(buf,"GET"))
  	    {
     		 int position;
     		 if((position=findpathend(buf+PATHOFFSET)) != -1)
     		 {
         	   	 memcpy(filepath,buf+PATHOFFSET,position);
				 printf("%s\n",filepath);
				 char* res ="HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length=14712\r\n\r\n" ;
				 writen(new_fd, res, strlen(res));



					 FILE * fd1 = fopen("./b.jpg","rb");
					 int rlen=0;
					  bzero(buf, BUFFERSIZE );
					 while (1)
					 {
						 rlen =fread(buf,sizeof(char),BUFFERSIZE,fd1);
		 ;
						 if (rlen > 0){
						 	writen(new_fd, buf, rlen);

						 }else if (rlen == 0){
							 printf("OK\n");
							 break;
						 }else {
							 printf("read data error\n");
							 return -1;
						 }	 
						 
					 }
				 close(new_fd);
				 continue;
				//writeResponse(filepath);
		      }else{
		          printf("get index\n");
		      }
		 }else{
		     printf("it is not get\n");
		     close(new_fd);
		 }
  		 printf("%s",buf);
    }
    return 1;
}


