#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <native/task.h>
#include <sys/mman.h>
#include <rtdk.h>



void* threadFunc(int *channel){
  io_write(*channel,1);

  while(1){
    rt_task_wait_period(NULL);
    int read = io_read(*channel);
    io_write(*channel, read);
    rt_task_yield();
  /*  if(!read){
      io_write(*channel,0);
      rt_task_sleep(5000);
      io_write(*channel,1);
    }*/
  }

}
void* threadPrint(){
  while(1){
    rt_task_wait_period(NULL);
    rt_printf("Hello \n");

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

void* distFunc(){
  set_cpu(1);
  int i;
  int b = 0;
  while(1){

    for(i = 0; i<10000;i++){
      b = i/3;
    }
  }

}

int main(){
  mlockall(MCL_CURRENT | MCL_FUTURE); // lock the current memory allocations and future memory allocations to the main memory
  rt_print_auto_init(1);
  io_init();
  int A = 1;
  int B = 2;
  int C = 3;

  //RT_TASK print;
  //rt_task_create(&print,"print", 0, 0,T_CPU(1)|T_JOINABLE);
  //rt_task_set_periodic(&print, TM_NOW, 5000000000);
  //rt_task_start(&print, threadPrint,&print);

  RT_TASK testA;
  rt_task_create(&testA,"A", 0, 0,T_CPU(1)|T_JOINABLE);
  rt_task_set_periodic(&testA, TM_NOW, 1000000);
  rt_task_start(&testA, threadFunc,&A);

  RT_TASK testB;
  rt_task_create(&testB,"B", 0, 0,T_CPU(1)|T_JOINABLE);
  rt_task_set_periodic(&testB, TM_NOW, 1000000);
  //rt_task_start(&testB, threadFunc,&B);

  RT_TASK testC;
  rt_task_create(&testC,"C", 0, 0,T_CPU(1)|T_JOINABLE);
  rt_task_set_periodic(&testC, TM_NOW, 1000000);
  //rt_task_start(&testC, threadFunc,&C);

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

  while(1);
  return 0;
}
