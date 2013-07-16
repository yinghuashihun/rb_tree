#include<pthread.h>
#include<string.h>

#define THREADNUMBER 5

#define QUENENUMBER 50

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int queue[QUENENUMBER];
int current = 0;

void* readQueue(void* arg){
	printf("THREADNUMBER is %d",(unsigned int)pthread_self());
	pthread_mutex_lock(&mutex);
	if( 0 ==  queue[current])
	{
		pthread_cond_wait(&cond,&mutex); 
	}else{
		printf("queue index %d  avalid",current);
		current++;
	}
	pthread_mutex_unlock(&mutex);
    
}


int main()
{
	pthread_t tid1,tid2,tid3,tid4,tid5;//pid[THREADNUMBER];
    bzero(queue,THREADNUMBER*sizeof(pthread_t));
    int i = 0;
    printf("this is here\n");
    pthread_create(&tid1, NULL, readQueue ,NULL);
    pthread_create(&tid2, NULL, readQueue ,NULL);
    pthread_create(&tid3, NULL, readQueue ,NULL);
    pthread_create(&tid4, NULL, readQueue ,NULL);
    pthread_create(&tid5, NULL, readQueue ,NULL);
    printf("this is here1\n");
    /*
    while(i < THREADNUMBER)
    {
    	printf("i is %d\n",pthread_create(&(pid[i]), NULL, readQueue ,NULL));
    	i++;
    }*/
    printf("this is here3\n");	
    i = 0;
    while (i < QUENENUMBER)
    {   

    	queue[i] = 1;
        printf(" now is looping %d\n",i);
    	pthread_cond_broadcast(&cond);
    	sleep(500);
    	i++;
    }
    return 1;
}