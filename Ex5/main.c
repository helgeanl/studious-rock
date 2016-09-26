#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>


void timespec_add_us(struct timespec *t, long us)
{
  // add microseconds to timespecs nanosecond counter
  t->tv_nsec += us*1000;

  // if wrapping nanosecond counter, increment second counter
  if (t->tv_nsec > 1000000000)
  {
    t->tv_nsec = t->tv_nsec - 1000000000;
    t->tv_sec += 1;
  }
}

int set_cpu(int cpu_number)
{
  // setting cpu set to the selected cpu
  cpu_set_t cpu;
  CPU_ZERO(&cpu);
  CPU_SET(cpu_number, &cpu);
  // set cpu set to current thread and return
  return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);
}

void* threadFunc(int channel){
  set_cpu(1);
  io_write(channel,1);
  struct timespec next;
  clock_gettime(CLOCK_REALTIME, &next);

  while(1){
    int read = io_read(channel);
    pthread_yield();
    if(!read){
      io_write(channel,0);
      usleep(5);
      io_write(channel,1);
      timespec_add_us(&next,1000);
      clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
    }
  }

}

void* distFunc(){
  set_cpu(1);
  int i;
  int b = 0;
  while(1){

    for(i = 0; i<1000;i++){
      b = i/3;
    }
  }

}

int main(){
  io_init();
  pthread_t threadA;
  pthread_t threadB;
  pthread_t threadC;
  pthread_t threadDist1;
  pthread_t threadDist2;
  pthread_t threadDist3;
  pthread_t threadDist4;
  pthread_t threadDist5;
  pthread_t threadDist6;
  pthread_t threadDist7;
  pthread_t threadDist8;
  pthread_t threadDist9;
  pthread_t threadDist10;

  pthread_create(&threadA, NULL, threadFunc,1);
  pthread_create(&threadB, NULL, threadFunc,2);
  pthread_create(&threadC, NULL, threadFunc,3);
  /*
  pthread_create(&threadDist1, NULL, distFunc,NULL);
  pthread_create(&threadDist2, NULL, distFunc,NULL);
  pthread_create(&threadDist3, NULL, distFunc,NULL);
  pthread_create(&threadDist4, NULL, distFunc,NULL);
  pthread_create(&threadDist5, NULL, distFunc,NULL);
  pthread_create(&threadDist6, NULL, distFunc,NULL);
  pthread_create(&threadDist7, NULL, distFunc,NULL);
  pthread_create(&threadDist8, NULL, distFunc,NULL);
  pthread_create(&threadDist9, NULL, distFunc,NULL);
  pthread_create(&threadDist10, NULL, distFunc,NULL);
*/
  pthread_join(threadA,NULL);
  pthread_join(threadB,NULL);
  pthread_join(threadC,NULL);


  pthread_exit;

  return 0;
}
