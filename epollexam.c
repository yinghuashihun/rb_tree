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

#define MAXBUF 1024
#define MAXEPOLLSIZE 10000


/* 设置socket 非阻塞模式 */
int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) 
    {
        return -1;
    }
    return 0;
}

/* 从socket接受数据，进行处理 */
int handle_message(int new_fd)
{
    char buf[MAXBUF + 1];
    char sendbuf[MAXBUF+1];
    int len;
   
    bzero(buf, MAXBUF + 1);
   
    len = recv(new_fd, buf, MAXBUF, 0);
    if (len > 0)
    {
        printf("%d接收消息成功:'%s'，共%d个字节的数据\n",
               new_fd, buf, len);
    }
    else 
    {
        if (len < 0)
            printf("消息接收失败！错误代码是%d，错误信息是'%s'\n",
                   errno, strerror(errno));
        else
            printf("客户端%d退出!\n",new_fd);
                 
        return -1;
    }
   
    return len;
}

int main(int argc, char **argv)
{
    int listener, new_fd, nfds, n, ret;
    struct epoll_event ev;
    int kdpfd, curfds;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int myport, lisnum;
    struct epoll_event events[MAXEPOLLSIZE];
    struct rlimit rt;
    if (argc > 1)
        myport = atoi(argv[1]);
    else
        myport = 1234;
    if (argc > 2)
        lisnum = atoi(argv[2]);
    else
        lisnum = 10;
   
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) 
    {
        perror("setrlimit");
        exit(1);
    }
    else printf("设置系统资源参数成功！\n");
   
    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    else
        printf("socket 创建成功！\n");
   
    int opt=SO_REUSEADDR;
    setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
   
    setnonblocking(listener);
    bzero(&my_addr, sizeof(my_addr));

    /* 设置socket 绑定的地址 */
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    if (argc>3)
        my_addr.sin_addr.s_addr = inet_addr(argv[3]);
    else
        my_addr.sin_addr.s_addr = INADDR_ANY;
        
    if (bind(listener, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1)
    {
        perror("bind");
        exit(1);
    }
    else
        printf("IP 地址和端口绑定成功\n");
        
    if (listen(listener, lisnum) == -1)
    {
        perror("listen");
        exit(1);
    } 
    else
        printf("开启服务成功！\n");

    /* 创建epoll，设置epoll最大关心socket的个数  */
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
        printf("监听 socket 加入 epoll 成功！\n");

    curfds = 1;

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
            /* 有connect请求 */
            if (events[n].data.fd == listener) 
            {
                new_fd = accept(listener, (struct sockaddr *)&their_addr,
                                &len);
                if (new_fd < 0)
                {
                    perror("accept");
                    continue;
                }
                else
                    printf("有连接来自于： %s:%d， 分配的 socket 为:%d\n",
                           inet_ntoa(their_addr.sin_addr), 
                           ntohs(their_addr.sin_port), new_fd);
                           
                setnonblocking(new_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;

                /* 通信socket加入epoll */
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0) 
                {
                    fprintf(stderr, "把 socket '%d' 加入 epoll 失败！%s\n",
                            new_fd, strerror(errno));
                    return -1;
                }
                curfds++;
            } 
            else 
            {
                /* 处理socket消息 */
                ret = handle_message(events[n].data.fd);
                if (ret < 1 && errno != 11) 
                {
                    if(epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev) < 0)
                    {
                        fprintf(stderr, "把 socket '%d' 从 epoll 删除失败！%s\n",
                                events[n].data.fd, strerror(errno));
                    }
                    
                    curfds--;
                    close(events[n].data.fd);
                }
            }
        }
    }
    close(listener);
    return 0;
}

