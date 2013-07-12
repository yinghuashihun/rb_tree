#include <errno.h>
#include <stdio.h>
ssize_t readn(int fd, void *vptr, size_t n )
{
    size_t   nleft;
    ssize_t  nread;
    char     *ptr = NULL;
    ptr = vptr;
    nleft = n;
    while (nleft > 0)
    {
      if ((nread = read(fd, ptr, nleft )) < 0)
      {
          if ( errno == EINTR )
              nread = 0;
          else
              return  -1 ;
      }
 
      nleft -= nread;
      ptr += nread;
      
      if(nread > 0)
       break;
    }

    return (n - nleft);
}


ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr = NULL;
    ptr = vptr;
    nleft = n;
    while (nleft > 0) 
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) 
        {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
        
        if(nwritten > 0)
           break;
    }
    return n;
}

