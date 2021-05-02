#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define BUFFER_SIZE 100  

int buffer[BUFFER_SIZE]; 
int index_ = 0;  //

void *fb(void *param)
{
    int *p = param;
    int n = *p;

    buffer[0] = 0; 
    buffer[1] = 1;   index_++;
    for (int i = 2; i < n; i++)
    {
        buffer[i] = buffer[i - 1] + buffer[i - 2];
        index_++;
    }

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int n=atoi(argv[1]);

    pthread_t tid; //child thread identifier 

    // create a thread
    pthread_create(&tid, NULL, fb, &n);

    // wait for child thread to finish
    //pthread_join(tid, NULL);

    // printing results stored in global variable
    for (int i = 0; i < n; i++)
    {
        while (index_<i); //busy waiting until child updates index 
        printf("%d ", buffer[i]);
    }
    printf("\n");
}
