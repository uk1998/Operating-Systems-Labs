/* Program to generate fibonacci sequence using pthread */

#include <pthread.h>
#include <stdio.h>

int seq[0]; /* this sequence data is shared by the thread(s) */

void *fibonacci(void *param); /* the thread */

int main()
{
	int n;
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */

	printf("Enter number to generate fibonacci sequence: ");
	scanf("%d", &n);
	getchar();
	
	/* get the default attributes */
	pthread_attr_init(&attr);

	/* create the thread */
	pthread_create(&tid, &attr, fibonacci, &n);

	/* now wait for the thread to exit */
	pthread_join(tid,NULL);

	printf("seq =");
	for(int i=0; i < n; i++){
		printf(" %d", seq[i]);
	}

	getchar();
}

/**
 * The thread will begin control in this function
 */
void *fibonacci(void *param) 
{
	int *num = param;
	
	seq[0] = 0;
	seq[1] = 1;
	for(int i=2; i < *num; i++){
		seq[i] = seq[i-2] + seq[i-1];
	}
	pthread_exit(0);
}