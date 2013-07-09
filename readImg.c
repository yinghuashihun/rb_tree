#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXBUF 1024


int main()
{
    
	FILE * fd1 = fopen("./b.jpg","rb");
			
			int rlen=0;
			char buf[MAXBUF ];
			 bzero(buf, MAXBUF );
			while (1)
			{
				rlen =fread(buf,sizeof(char),MAXBUF,fd1);
;
				if (rlen > 0){
					int i = 0;
					for(;i<rlen;i++){
					    printf("%0x ",buf[i]);	
					}
				}else if (rlen == 0){
					printf("OK\n");
					break;
				}else {
					printf("read data error\n");
					return -1;
				}	
				
			}
	
			
}
