#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

/* create and initialize the mutex lock */

pthread_mutex_t my_mutex_lock;

int hits = 0;


// Struct that will contain information about each thread
// Mutex Lock Initialization

typedef struct{
    unsigned long long int start;
    unsigned long long int end;
}thread_args;

double frand()
{
    return (double)rand() / (double)RAND_MAX;
}

void *cal_pi(void *param)
{
    double x, y;
    thread_args *args = (thread_args*)param;
    for (int i = args -> start; i < args -> end; i++)
    {
        x = frand();
        y = frand();
        //printf("%d\n", i);
        if ((x * x + y * y) < 1)
        {
            pthread_mutex_lock (&my_mutex_lock);
            hits++;
            pthread_mutex_unlock(&my_mutex_lock);
        }
    }
    pthread_exit(0);
}



int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("should include your argument\n");
        return 1;
    }

    if (0 != pthread_mutex_init(&my_mutex_lock, NULL))
    {
        printf("mutex init failed");
        return -1;
    }
    srand(time(NULL));
    //pthread_t tid;
    //pthread_attr_t attr;

    // Multi thread init
    int n_threads = 10;
    pthread_t thread_id[n_threads];
    // Is threads args necessary ? Yes, cause we want to make different limits for for-loop
    thread_args *thread_args_arr = (thread_args*)calloc(n_threads, sizeof(thread_args));
    unsigned long long int first = 1;
    unsigned long long int end = atoi(argv[1]);
    // get thread attributes
    //pthread_attr_init(&attr);
    // create a thread
    int i;
    for (i = 0; i < n_threads; i++) {
        thread_args_arr[i].start = first;
        thread_args_arr[i].end = first + (unsigned long long int)ceil((double)end/(double)n_threads) - 1 <= end  ? first + (unsigned long long int)ceil((double)end/(double)n_threads) - 1 : end;
        pthread_create(&(thread_id[i]), NULL, cal_pi, &(thread_args_arr[i]));
        first =  first + (unsigned long long int)ceil((double)end/(double)n_threads);
    }
    //pthread_create(&tid, &attr, cal_pi, argv[1]);

    // wait for each thread to finish
    for (i = 0; i < n_threads; i++) {
        pthread_join(thread_id[i], NULL);
    }
    //pthread_join(tid, NULL);
    printf("Hits = %d\n", hits);
    printf("Monte Carlo Pi: %f\n", 4 * (double)hits / (double)end);
}
