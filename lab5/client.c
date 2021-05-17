/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "threadpool.h"

struct data {
    int a;
    int b;
};

void add(void *param) {
    struct data *temp;
    temp = (struct data *)param;
    printf("%d + %d = %d\n", temp->a, temp->b, temp->a + temp->b);
}

int main(void) {
    int n = 0;
	printf("Please input the number of tasks: ");
	scanf("%d", &n);
	if(n <= 0){
		printf("Input should be a positive integer\n");
		return 0;
	}
	
    // initialize the thread pool
    pool_init();
    
	// submit the work to the queue
    struct data *Data = malloc(n * sizeof(struct data));
    for(int i = 0; i < n; i++) {
        Data[i].a = i;
        Data[i].b = i;
        while(pool_submit(&add, &Data[i]) != 0);// busy waiting...queue is full
    }
    printf("All tasks have been submitted.\n");
    pool_shutdown();
    free(Data);
    return 0;
}
