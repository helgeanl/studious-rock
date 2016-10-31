#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>



int main(int argc, char *argv[]) {
	printf("Reading from file example \n");
	int res;
	char buf[255];
	int fd = open("/dev/myresource", O_RDONLY); // blocking
	//int fd = open("/dev/myresource", O_NONBLOCK); // non-blocking

	res = read(fd, buf, 255);
	buf[res] = '\0';

	printf("Read: %s\n", buf);

	close(fd);
	return EXIT_SUCCESS;
}
