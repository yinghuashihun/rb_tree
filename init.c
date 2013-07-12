#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

#define MAX 1024

void listallfiles(char * pathname)
{
    DIR * dir;
	struct dirent *ptr;
	char path[MAX];
    bzero(path,MAX);
	dir = opendir(pathname);
	if(NULL == dir)
	{
		exit(1);
	}
    
	while( (ptr = readdir(dir)) !=NULL )
	{
	    if(0 == strcmp(ptr->d_name,".") ||  0 == strcmp(ptr->d_name,".."))
	    {
			continue;
	    }
		if(DT_DIR == ptr->d_type)
		{
		    
			sprintf(path,"%s/%s",pathname,ptr->d_name);
           
			listallfiles(path);
		}
		if(DT_REG == ptr->d_type)
		{
		    char fullname[MAX];
			bzero(fullname,MAX);
		    sprintf(fullname,"%s/%s",pathname,ptr->d_name);
			printf("%s\n",fullname);
		}
	}
	closedir(dir);
}
int main(){
	listallfiles("/opt/apache-tomcat-6.0.36/webapps/website");
	return 1;
}
