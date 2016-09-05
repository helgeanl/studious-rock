#include<pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>



pthread_mutex_t fork1;
pthread_mutex_t fork2;
pthread_mutex_t fork3;
pthread_mutex_t fork4;
pthread_mutex_t fork5;


void* thread1_func(){
	pthread_mutex_lock(&fork1);
	pthread_mutex_lock(&fork2);
	printf("Philosopher 1 is eating\n");
	pthread_mutex_unlock(&fork2);
	pthread_mutex_unlock(&fork1);
}

void* thread2_func(){
	pthread_mutex_lock(&fork3);
	pthread_mutex_lock(&fork2);
	printf("Philosopher 2 is eating\n");
	pthread_mutex_unlock(&fork3);
	pthread_mutex_unlock(&fork2);
}

void* thread3_func(){
	pthread_mutex_lock(&fork3);
	pthread_mutex_lock(&fork4);
	printf("Philosopher 3 is eating\n");
	pthread_mutex_unlock(&fork4);
	pthread_mutex_unlock(&fork3);
}

void* thread4_func(){
	pthread_mutex_lock(&fork5);
	pthread_mutex_lock(&fork4);
	printf("Philosopher 4 is eating\n");
	pthread_mutex_unlock(&fork5);
	pthread_mutex_unlock(&fork4);
}

void* thread5_func(pthread_mutex_t *fork5){
	pthread_mutex_lock(&fork5);
	pthread_mutex_lock(&fork1);
	printf("Philosopher 5 is eating\n");
	pthread_mutex_unlock(&fork1);
	pthread_mutex_unlock(&fork5);
}

int main() {

	if (pthread_mutex_init(&fork1, NULL) != 0){
		perror("mutex initilization");
		exit(0);
	}
	if (pthread_mutex_init(&fork2, NULL) != 0){
		perror("mutex initilization");
		exit(0);
	}
	if (pthread_mutex_init(&fork3, NULL) != 0){
		perror("mutex initilization");
		exit(0);
	}
	if (pthread_mutex_init(&fork4, NULL) != 0){
		perror("mutex initilization");
		exit(0);
	}

	if (pthread_mutex_init(&fork5, NULL) != 0){
		perror("mutex initilization");
		exit(0);
	}


    pthread_t thread1;
    pthread_create(&thread1,NULL,thread1_func,NULL);
	pthread_t thread2;
    pthread_create(&thread2,NULL,thread2_func,NULL);
	pthread_t thread3;
    pthread_create(&thread3,NULL,thread3_func,NULL);
	pthread_t thread4;
    pthread_create(&thread4,NULL,thread4_func,NULL);
	pthread_t thread5;
	pthread_create(&thread5,NULL,thread5_func,NULL);


    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	pthread_join(thread5,NULL);

    pthread_exit;
    return 0;
}
