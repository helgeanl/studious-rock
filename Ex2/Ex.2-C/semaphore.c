#include<pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <semaphore.h>


 int global = 0;

void* thread1_func(sem_t*sem){

	sem_wait(sem);
	int j;
	for (j=0;j<4;j++,global++){
		printf("Thread number 1\n");
		sleep(1);
	}
	sem_post(sem);
}
void* thread2_func(sem_t*sem){
	sem_wait(sem);
	int j;
	for (j=0;j<4;j++,global++){
		printf("Thread number 2\n");
		sleep(1);
	}
	sem_post(sem);
}
void* thread3_func(sem_t*sem){

	sem_wait(sem);
	int j;
	for (j=0;j<4;j++,global++){
		printf("Thread number 3\n");
		sleep(1);
	}
	sem_post(sem);
}
void* thread4_func(sem_t*sem){

	sem_wait(sem);
	int j;
	for (j=0;j<4;j++,global++){
		printf("Thread number 4\n");
		sleep(1);
	}
	sem_post(sem);
}
void* thread5_func(sem_t*sem){

	sem_wait(sem);
	int j;
	for (j=0;j<4;j++,global++){
		printf("Thread number 5\n");
		sleep(1);
	}
	sem_post(sem);
}

int main() {
	sem_t *sem;
	sem_init( sem, 0, 3);


    pthread_t thread1;
    pthread_create(&thread1,NULL,thread1_func,sem);
	pthread_t thread2;
    pthread_create(&thread2,NULL,thread2_func,sem);
	pthread_t thread3;
    pthread_create(&thread3,NULL,thread3_func,sem);
	pthread_t thread4;
    pthread_create(&thread4,NULL,thread4_func,sem);
	pthread_t thread5;
    pthread_create(&thread5,NULL,thread5_func,sem);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	pthread_join(thread5,NULL);

    pthread_exit;
    return 0;
}
