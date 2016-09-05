#include<pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>

 int global = 0;

void* thread1_func(int *var){
	int i = 0;
	int j;
	for (j=0;j<1000;j++,i++,global++,(*var)++);
	printf("Thread 1: i =%d, global= %d, local=%d\n",i,global,*var);
}
void* thread2_func(int* var){
    int i = 0;
	int j;
	for (j=0;j<1000;j++,i++,global++,(*var)++);
	printf("Thread 2: i =%d, global= %d, local=%d\n",i,global,*var);
}


int main() {
	int local = 0;
    pthread_t thread1;
    pthread_create(&thread1,NULL,thread1_func,&local);
    pthread_t thread2;
    pthread_create(&thread2,NULL,thread2_func,&local);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    pthread_exit;
	printf("Global: %d, local: %d",global,local);
    return 0;
}
