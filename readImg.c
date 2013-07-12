#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXBUF 1024


int main()
{
    
    FILE * fd1 = fopen("./b.jpg","rb");
    int iRet = 0;
    int i = 0;
            
    int rlen=0;
    char buf[MAXBUF ];
    bzero(buf, MAXBUF );

    if (NULL == fd1)
    {
       return iRet;
    }
    while (1)
    {
        rlen =fread(buf, sizeof(char), MAXBUF, fd1);
        if (rlen > 0)
        {
            for(;i<rlen;i++)
            {
                /* ����ط�Ҳ������һ�¸�ʽ */
                printf("%0x ",buf[i]);  
            }
        }
        else if (rlen == 0)
        {
            printf("OK\n");
            break;
        }
        else 
        {
            printf("read data error\n");
            iRet = -1;
            break;
        }   
    }

    fclose(fd1);
    return iRet;
    
            
}
