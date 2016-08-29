#include<pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <sys/times.h>

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
		times(&exec_time);
	}
}

void* thread1_func(){
	printf("Hello from thread 1!\n" );
	busy_wait_delay(5);
	printf("Hello from thread 1 again!\n" );
}
void* thread2_func(){
	printf("Hello from thread 2!\n" );
	busy_wait_delay(5);
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
