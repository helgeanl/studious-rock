#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

void allocate(int value){
	int *ptr = NULL;
	ptr = (int*)malloc(100000000000000 * sizeof(long));
	if(ptr == NULL){
		perror("Malloc");
	}
	*ptr = value;
	printf("test of allocated memory: %i\n");
}

int main(int argc, char *argv[]){
	while(1){
		allocate(10);
	}
	return 0;
}
