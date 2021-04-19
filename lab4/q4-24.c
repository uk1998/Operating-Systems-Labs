/* Program to compute Pi using Monte Carlo methods using pthread */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int count; /* Count of points in Circle is shared by the thread(s) */

void *randPoints(void *nPoints); /* the thread */

int main(int argc, char *argv[])
{
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */
	
	if (argc != 2) {
		fprintf(stderr,"usage: a.out <integer value>\n");
		return -1;
	}

	/* get the default attributes */
	pthread_attr_init(&attr);

	/* create the thread */
	pthread_create(&tid, &attr, randPoints, argv[1]);

	/* now wait for the thread to exit */
	pthread_join(tid,NULL);
	
	printf("Monte carlo estimate of pi : %f", (double) 4*count/atoi(argv[1]));

	getchar();
}

/* The thread will begin control in this function */
void *randPoints(void *nPoints)
{
	int total = atoi(nPoints);
	double x, y;
    for (int i = 0; i < total; i++)
    {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;

        if ((x*x+y*y) < 1)
        {
            count++;
        }
    }
	
	pthread_exit(0);
}
