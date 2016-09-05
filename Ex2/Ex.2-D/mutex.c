#include<pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>

#include <semaphore.h>

int running = 1;
int var1 = 0;
int var2 = 0;


void* thread1_func(pthread_mutex_t *lock){

	while(running){
		pthread_mutex_lock(lock);
		var1++;
		var2 = var1;
		pthread_mutex_unlock(lock);
		sleep(1);
	}

}

void* thread2_func(pthread_mutex_t *lock){
	for(int i=1;i<=20;i++){
		pthread_mutex_lock(lock);
		printf("number 1 is %d, number 2 is %d\n",var1,var2);
		pthread_mutex_unlock(lock);
		sleep(1);
	}
	running = 0;
}

int main() {
	pthread_mutex_t lock;
	if (pthread_mutex_init(&lock, NULL) != 0){
		perror("mutex initilization");
		exit(0);
	}


    pthread_t thread1;
    pthread_create(&thread1,NULL,thread1_func,&lock);
	pthread_t thread2;
    pthread_create(&thread2,NULL,thread2_func,&lock);


    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    pthread_exit;
    return 0;
}
