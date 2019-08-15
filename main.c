#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS		0

int store[4];
int q=0;
pthread_mutex_t lock;
int counter=0;

void *buying()
{
	srand(time(NULL));
        int buyer=rand()%(11000+1-9000)+9000;
	while(buyer>0 && counter<3)
	{
		srand(time(NULL));
        	q=rand()%(4+1-0)+0;//random store
		if(store[q]>0)
		{
			pthread_mutex_lock(&lock);
			buyer=buyer-store[q];
			printf("Buyer bought from store %d %d products and now he have %d money\n", q,store[q],buyer);
			store[q]=0;
			pthread_mutex_unlock(&lock);
			sleep(3);
		}
	}
	printf("1 thread done!\n");
	counter++;
	printf("Counter=%d\n",counter);
	return SUCCESS;
}

void *give()
{
	while(counter<3)
	{
		srand(time(NULL));
                q=rand()%(4+1-0)+0;//random store
		if(store[q]<=0)
		{
			pthread_mutex_lock(&lock);
                        store[q]=store[q]+500;
                        printf("Added 500 for store %d\n",q);
			pthread_mutex_unlock(&lock);
                        sleep(2);
		}
	}
}

int main()
{
	int i;
	int j;
	pthread_t threads[3];
	int status;
	int status_addr;
	srand(time(NULL));
	for(j=0;j<5;j++)
	{
		store[j]=rand()%(1100+1-900)+900;
		printf("store %d = %d\n", j,store[j]);
	}

	if (pthread_mutex_init(&lock, NULL) != 0)
    	{
        	printf("\n mutex init failed\n");
        	return 1;
    	}

	for (i = 0; i < 3; i++)
	{
		status = pthread_create(&threads[i], NULL, buying, NULL);
        	if (status != 0)
		{
            		printf("main error: can't create thread, status = %d\n", status);
            		exit(ERROR_CREATE_THREAD);
        	}
    	}

	status = pthread_create(&threads[3], NULL, give, NULL);
                if (status != 0)
                {
                        printf("main error: can't create thread, status = %d\n", status);
                        exit(ERROR_CREATE_THREAD);
                }


	for (i = 0; i < 4; i++)
	{
        	status = pthread_join(threads[i], (void**)&status_addr);
        	if (status != SUCCESS)
		{
            		printf("main error: can't join thread, status = %d\n", status);
            		exit(ERROR_JOIN_THREAD);
        	}
        	printf("joined with address %d\n", status_addr);
    	}

	return 0;
}

