#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <signal.h>

#include "miniproject.h"

char udp_message[128];
pthread_mutex_t udp_message_mutex;
struct udp_conn conn;

#define REFERENCE 1
#define KP 10
#define KI 800
#define PERIOD 1000

void sighandler(int signum){
	printf("exiting, closing simulator and udp connection..\n");
	char* msg = "STOP";
	udp_send(&conn, msg, strlen(msg)+1);
	udp_close(&conn);
	exit(1);

}

void *udp_listen(void){
	char *GET = "GET";

	while(1){
		if (udp_send(&conn, GET, sizeof(GET)) == -1) {
			printf("send failed\n");
			exit(1);
		}
		pthread_mutex_lock(&udp_message_mutex);
		if (udp_receive(&conn,udp_message,sizeof(udp_message))==-1){
			printf("receive failed\n");
			exit(1);
		}
		pthread_mutex_unlock(&udp_message_mutex);
	}
	return NULL;
}

void *controller(){
	float y;
	char m[128];
	struct timespec time;
	double error;
	double integral = 0;
	double u;
	double period = 0.005;

	char msg[128];
	while(1){
		clock_gettime(CLOCK_REALTIME, &time);
		timespec_add_us(&time, period*1000000);
		clock_nanosleep(&time);
		pthread_mutex_lock(&udp_message_mutex);
		memcpy(m,&udp_message[8],7);
		m[7] = '\0';
		y = atof(m);
		pthread_mutex_unlock(&udp_message_mutex);

		error = REFERENCE - y;
		integral += error*period;
		u = KP * error + KI * integral;

		sprintf(msg,"SET:%3.3f", u);

		if (udp_send(&conn, msg, sizeof(msg)+1) == -1) {
			printf("send failed\n");
			exit(1);
		}
	}
	return NULL;
}

int main(){

	signal(SIGINT, sighandler);

	if (pthread_mutex_init(&udp_message_mutex, NULL) != 0){
		perror("mutex initilization error");
		exit(1);
	}

	if (udp_init_client(&conn, 9999, "192.168.0.1")) {
		printf("init failed\n");
		exit(1);
	}
	char *msg = "START";
	if (udp_send(&conn, msg, sizeof(msg)+1) == -1) {
		printf("send failed\n");
		exit(1);
	}
	printf("Simulator started\n");

	pthread_t udp_thread;
	pthread_create(&udp_thread,NULL,udp_listen,NULL);


	pthread_t controller_thread;
	pthread_create(&controller_thread,NULL,controller,NULL);

	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	timespec_add_us(&time, 495000);
	clock_nanosleep(&time);

	udp_send(&conn, "STOP", 5);
	udp_close(&conn);
	return 0;
}



int udp_init_client(struct udp_conn *udp, int port, char *ip)
{
	struct hostent *host;

	if ((host = gethostbyname(ip)) == NULL) return -1;

	udp->client_len = sizeof(udp->client);
	// define servers
	memset((char *)&(udp->server), 0, sizeof(udp->server));
	udp->server.sin_family = AF_INET;
	udp->server.sin_port = htons(port);
	bcopy((char *)host->h_addr, (char *)&(udp->server).sin_addr.s_addr, host->h_length);

	// open socket
	if ((udp->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) return udp->sock;

	return 0;
}

int udp_send(struct udp_conn *udp, char *buf, int len)
{
	return sendto(udp->sock, buf, len, 0, (struct sockaddr *)&(udp->server), sizeof(udp->server));
}

int udp_receive(struct udp_conn *udp, char *buf, int len)
{
	int res = recvfrom(udp->sock, buf, len, 0, (struct sockaddr *)&(udp->client), &(udp->client_len));

	return res;
}

void udp_close(struct udp_conn *udp)
{
	close(udp->sock);
	return;
}

int clock_nanosleep(struct timespec *next)
{
	struct timespec now;
	struct timespec sleep;

	// get current time
	clock_gettime(CLOCK_REALTIME, &now);

	// find the time the function should sleep
	sleep.tv_sec = next->tv_sec - now.tv_sec;
	sleep.tv_nsec = next->tv_nsec - now.tv_nsec;

	// if the nanosecon is below zero, decrement the seconds
	if (sleep.tv_nsec < 0)
	{
		sleep.tv_nsec += 1000000000;
		sleep.tv_sec -= 1;
	}

	// sleep
	nanosleep(&sleep, NULL);

	return 0;
}

void timespec_add_us(struct timespec *t, long us)
{
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;

	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000)
	{
		t->tv_nsec -= 1000000000;
		t->tv_sec += 1;
	}
}
