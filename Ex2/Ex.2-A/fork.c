#include<stdio.h>
#include<unistd.h>
int global =0;

int main(){
	int i = 10;
	pid_t pid = fork();

	if (pid == 0){
		int j;
		printf("PID: %d\n",pid);
		for(j=0;j<1000;j++,i++,global++);

		printf("Child process: %d, global: %d\n",i,global);
	}else if(pid > 0){
		int j;
		printf("PID: %d\n",pid);
		for (j=0;j<1000;j++,i++,global++);

		printf("Parent process: %d, global: %d\n",i,global);
	}else{
		printf("Error\n");

	}
	//sleep(1000);
	return 0;
}
