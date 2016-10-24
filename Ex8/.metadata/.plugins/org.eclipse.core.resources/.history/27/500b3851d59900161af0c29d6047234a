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

int main(int argc, char *argv[]) {
	set_priority(4);
	printf("Welcome to Hello, we are creating memory\n");
	int fd = shm_open("/sharepid", O_RDWR |O_CREAT, S_IRWXU);

	ftruncate(fd, sizeof(struct pid_data));
	struct pid_data *ptr = (struct pid_data*)mmap(0,sizeof(struct pid_data),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);

	pthread_mutexattr_t myattr;
	pthread_mutexattr_setpshared(&myattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&ptr->pid_mutex, &myattr );

	pthread_mutex_lock(&ptr->pid_mutex);
	ptr->pid = getpid();
	printf("PID written: %i\n", ptr->pid);
	pthread_mutex_unlock(&ptr->pid_mutex);

	// Assignment B Server


	char data_buffer[100];
	char message_reply[] = "Message reply";
	struct _msg_info msg_info;

	int ch_id = ChannelCreate(0);
	while(1){
		int msg_id = MsgReceive(ch_id, data_buffer, 100*sizeof(char),&msg_info);
		printf("Current server priority: %i\n", get_priority());
		printf("Data buffer received from Client:  %s\n", data_buffer);
		printf("Client PID: %i\nClient TID: %i\n", msg_info.pid, msg_info.tid);
		MsgReply(msg_id, 0, message_reply, sizeof(message_reply));
	}




	return EXIT_SUCCESS;
}

