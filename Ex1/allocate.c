#include<stdio.h>
#include<stdlib.h>

void allocate(int value){
	int *ptr = NULL;
	ptr = malloc(100000 * sizeof(int));
	*ptr = value;
	printf("test of allocated memory: %i\n");
}

void main(int argc, char *argv[]){
	while(1){
		allocate(10);
	}
}
