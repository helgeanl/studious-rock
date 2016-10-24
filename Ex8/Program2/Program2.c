#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
};

struct thread_args{
	char message[100];
	int priority;
};
static int server_PID;

int set_priority(int priority){
	int policy;
	struct sched_param param;
	// check priority in range
	if (priority < 1 || priority > 63) return -1;
	// set priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	param.sched_priority = priority;
	return pthread_setschedparam(pthread_self(), policy, &param);
}

int get_priority(){
	int policy;
	struct sched_param param;
	// get priority
	pthread_getschedparam(pthread_self(), &policy, &param);
	return param.sched_curpriority;
}

void *send_message(struct thread_args *args){
	set_priority(args->priority);
	char data_buffer[100];

	int channel_id = ConnectAttach(0,server_PID,1,0,0);
	int message_status = MsgSend(channel_id,args->message, 100*sizeof(char),&data_buffer, 100*sizeof(char));
	printf("Server reply from %s: %s\n", args->message ,data_buffer);
	ConnectDetach(channel_id);
}
int main(int argc, char *argv[]) {
	set_priority(10);
	printf("Welcome to Program2, we are reading memory\n");
	int fd = shm_open("/sharepid", O_RDWR, S_IRWXU);

	struct pid_data *ptr = (struct pid_data*)mmap(0,sizeof(struct pid_data),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);




	pthread_mutexattr_t myattr;
	pthread_mutexattr_setpshared(&myattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&ptr->pid_mutex, &myattr );



	pthread_mutex_lock(&ptr->pid_mutex);
	server_PID = ptr->pid;
	printf("PID read: %i\n",ptr->pid);
	pthread_mutex_unlock(&ptr->pid_mutex);


	//Assignment B Client

	int data = 42;
	int data_buffer;

	int channel_id = ConnectAttach(0,server_PID,1,0,0);
	int message_status = MsgSend(channel_id,&data, sizeof(int),&data_buffer, sizeof(int));

	ConnectDetach(channel_id);

	printf("Data buffer received from Server:  %i\n", data_buffer);


	struct thread_args t1 = {"thread1", 2};
	struct thread_args t2 = {"thread2", 3};
	struct thread_args t3 = {"thread3", 5};
	struct thread_args t4 = {"thread4", 6};

	//Assignment C Client

	pthread_t thread1;
	pthread_create(&thread1,NULL,send_message,&t1);

	pthread_t thread2;
	pthread_create(&thread2,NULL,send_message,&t2);

	pthread_t thread3;
	pthread_create(&thread3,NULL,send_message,&t3);

	pthread_t thread4;
	pthread_create(&thread4,NULL,send_message,&t4);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);

return EXIT_SUCCESS;
}

