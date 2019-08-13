#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS		0

int store[9];
int buyer[5];
int q=0;
int w=0;
int z=0;

void *buying() 
{
	while(buyer[0]>0 || buyer[1]>0 || buyer[2]>0)
	{
		q=0;
		if(buyer[w]>0 && buyer[w+3]==0)
		{
			buyer[w+3]=1;
			while (q<5)
			{
				if(store[q]>0 && store[q+5]==0)
				{
					store[q+5]=1;
					buyer[w]=buyer[w]-store[q];
					printf("Buyer %d bought from store %d %d products and now he have %d money\n", w,q,store[q],buyer[w]);
					store[q]=0;
					store[q+5]=0;
					sleep(3);
				}
				else 
				{
					q++;
				}
			}
			buyer[w+3]=0;
		}//if buyer>0
		else if(w<3)
		{
			w++;
		}
		else
		{
			w=0;
		}
	}
	printf("I'm done!\n");
	return SUCCESS;
}

void *give()
{
	while(buyer[0]>0 || buyer[1]>0 || buyer[2]>0)
	{
		if(store[z]<=0 && store[z+5]==0)
		{
			store[z+5]=1;
                        store[z]=store[z]+50000;
                        printf("Added 50000 for store %d\n",z);
                        store[z+5]=0;
                        sleep(2);
		}
		else if (z<5)
		{
			z++;
		}
		else
		{
			z=0;
		}
	}
}

int main()
{
	int i,j,k;
	pthread_t threads[3];
	int status;
	int status_addr;
	int counter =0;
	for(j=0;j<5;j++)
	{
		store[j]=rand()%(1100+1-900)+900;
		printf("store %d = %d\n", j,store[j]);
	}

	for(k=0;k<3;k++)
        {
        	buyer[k]=rand()%(11000+1-9000)+9000;
        	printf("buyer %d = %d\n", k, buyer[k]);
        }
	
	for (i = 0; i < 3; i++) 
	{
//status = pthread_create(&threads[i], NULL, buying, (void*) numbers);
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


	for (i = 0; i < 3; i++) 
	{
        	status = pthread_join(threads[i], (void**)&status_addr);
        	if (status != SUCCESS) 
		{
            		printf("main error: can't join thread, status = %d\n", status);
            		exit(ERROR_JOIN_THREAD);
        	}
        	printf("joined with address %d\n", status_addr);
    	}

	status = pthread_join(threads[3], (void**)&status_addr);
                if (status != SUCCESS) 
                {
                        printf("main error: can't join thread, status = %d\n",status);
                        exit(ERROR_JOIN_THREAD);
                }
                printf("joined with address %d\n", status_addr);

	return 0;
}

