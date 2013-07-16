#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>


#define MAXBUF 1024  
#define MAXEPOLLSIZE 10000  
#define TASKQUEUENUMBER 256
#define PORT 5000
#define THREADNUMBER 5



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
/* 
     set handler as nonblock  
*/  
int setnonblocking(int sockfd)  
{  
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)  
    {  
        return -1;  
    }  
    return 0;  
}
int handle_message(int new_fd)  
{    
    char buf[MAXBUF + 1];  
    int len; 
    bzero(buf, MAXBUF + 1);    
    len = recv(new_fd, buf, MAXBUF, 0);  
    if (len > 0)  
    {  
        printf("%d receive messasge success:%s, total %d byte data\n",new_fd, buf, len);
        char* res ="HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: 14712\r\n\r\n" ;
        write(new_fd, res, strlen(res));
        FILE * fd1 = fopen("./b.jpg","rb");
        int rlen=0;
        bzero(buf, MAXBUF+1 );
        while (1)
        {
            rlen =fread(buf,sizeof(char),MAXBUF,fd1);
            if (rlen > 0)
            {
                write(new_fd, buf, rlen);
            }
            else if (0 == rlen)
            {
                printf("OK1232123\n");
                len = -1;
                break;
            }
            else
            {
                printf("read data error\n");
                len = -1;
                break;
            }       
        }
        fclose(fd1);
    }  
    else  
    {  
        if (len < 0)  
           printf ("message receive failed,code is %d, and message is '%s'\n",errno, strerror(errno));  
        close(new_fd);  
        len = -1;  
    } 

    
    return len;  
}
    


int listener, new_fd, kdpfd, nfds, n, ret, curfds;  
socklen_t len;  
struct sockaddr_in my_addr, their_addr;  
unsigned int  lisnum;  
struct epoll_event ev;  
struct epoll_event events[MAXEPOLLSIZE];  
struct rlimit rt;


pthread_t thread[THREADNUMBER]; 
int task[TASKQUEUENUMBER];
int headindex = 0;
int tailindx = 0;


pthread_cond_t taskCond = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t taskMutex  = PTHREAD_MUTEX_INITIALIZER; 

void* handle_readable_fd(void* parameter)
{
    while (1)
    {
        pthread_mutex_lock(&taskMutex); 
        int myjob = headindex;
        if(headindex == tailindx)
        {
            pthread_cond_wait (&taskCond, &taskMutex); 
        }else{
            int fd = task[headindex];
            headindex ++;
            pthread_mutex_unlock (&taskMutex); 
            handle_message(fd);
            pthread_mutex_lock(&taskMutex); 
            epoll_ctl(kdpfd, EPOLL_CTL_DEL,fd,&ev);  
            curfds--;  
            pthread_mutex_unlock (&taskMutex);
        }
         
        pthread_mutex_unlock (&taskMutex); 
    }
}


void initPthread(void)
{
    bzero(task,TASKQUEUENUMBER*sizeof(int));
    pthread_attr_t threadAttr; 
    pthread_attr_init(&threadAttr);
    int i = 0;
    for(;i<THREADNUMBER;i++)
    {
         pthread_create(&thread[i], &threadAttr, handle_readable_fd, NULL);
    }
}

int main(int argc, char **argv)  
{  
    printf("pid=%d\n", getpid());

    lisnum = 2; 
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;  
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1)   
    {  
        perror("setrlimit");  
        return -1;
    }  
    else   
    {  
        printf("set system resouce parameter sucessful !\n");  
    } 

    /* 创建socket */
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
    {  
        perror("socket");  
        return -1;
    }  
    else  
    {  
        printf("socket create successfully\n");  
    } 
    
    bzero(&my_addr, sizeof(my_addr));

    /* 设置socket属性为非阻塞模式 */
    setnonblocking(listener);

    /* 设置socket */
    my_addr.sin_family = AF_INET;  
    my_addr.sin_port = htons(PORT);  
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listener, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)   
    {  
        perror("bind");  
        return -1;  
    }   
    else  
    {  
        printf("IP address binding sucessfully\n");  
    } 

    if (listen(listener, lisnum) == -1)   
    {  
        perror("listen");  
        return -1;
    }  
    else  
    {  
        printf("service begin\n");  
    } 

    /* 创建epoll */
    kdpfd = epoll_create(MAXEPOLLSIZE);
    len = sizeof(struct sockaddr_in);  
    ev.events = EPOLLIN | EPOLLET;  
    ev.data.fd = listener;


   

    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0)   
    {  
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listener);  
        return -1;  
    }  
    else  
    {  
         printf("listen sockt add epoll\n");  
    }
    curfds = 1; 
    
    
    initPthread();


    while (1)   
    {  
        nfds = epoll_wait(kdpfd, events, curfds, -1);  
        if (nfds == -1)  
        {  
            perror("epoll_wait");  
            break;  
        }    
        for (n = 0; n < nfds; ++n)  
        {  
           if (events[n].data.fd == listener)   
           {  
                new_fd = accept(listener, (struct sockaddr *) &their_addr,&len);  
                if (new_fd < 0)   
                {  
                    perror("accept");  
                    continue;  
                }   
                else  
                {  
                    printf("connection coming from  %d:%d assign  socket is %d\n",  
                           inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);  
                }

                setnonblocking(new_fd);  
                ev.events = EPOLLIN | EPOLLET;  
                ev.data.fd = new_fd;

                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0)  
                {  
                    fprintf(stderr, "add socket '%d' join to epoll failed! %s\n",  
                            new_fd, strerror(errno));
 
                    return -1;  
                }  
                curfds++;  
            }   
            else{  
               task[tailindx] = events[n].data.fd;
               tailindx =  (tailindx+1)%TASKQUEUENUMBER;
               pthread_cond_signal(&taskCond); 
               /*
               ret = handle_message(events[n].data.fd);  
               printf ("return result = %d ",ret);
               if (ret < 1 && errno != 11)  
               {  
                   printf("delefd %d",events[n].data.fd);
                   
               } 
               */ 
            }  
         }  
    }  
    close(listener);  
    return 0;  
}    
