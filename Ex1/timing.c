#include<pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>


void* thread1_func(){
	printf("Hello from thread 1!\n" );
	sleep(5);
	printf("Hello from thread 1 again!\n" );
}
void* thread2_func(){
	printf("Hello from thread 2!\n" );
	sleep(5);
	printf("Hello from thread 2 again!\n" );
}


int main() {
	pthread_t thread1;
	pthread_create(&thread1,NULL,thread1_func,NULL);
	pthread_t thread2;
	pthread_create(&thread2,NULL,thread2_func,NULL);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	pthread_exit;
	return 0;
}
