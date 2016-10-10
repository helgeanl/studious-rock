
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <native/task.h>
#include <native/sem.h>
#include <native/mutex.h>
#include <sys/mman.h>
#include <rtdk.h>

#define TIME_UNIT  1000000

RT_SEM sem,syn;
RT_MUTEX mutex;

/*
The base and current priority of the running task is printed together
with a provided message.
*/
void print_pri(RT_TASK *task, char *s)
{
  struct rt_task_info temp;
  rt_task_inquire(task, &temp);
  rt_printf("b:%i c:%i ", temp.bprio, temp.cprio);
  rt_printf(s);
}

int rt_task_sleep_ms(unsigned long delay){
  return rt_task_sleep(1000*1000*delay);
}

void busy_wait_ms(unsigned long delay){
  unsigned long count = 0;
  while (count <= delay*10){
    rt_timer_spin(1000*100);
    count++;
  }
}

void* threadFunc1(void){
  rt_sem_p(&syn,TM_INFINITE);
  rt_printf("Start task Low\n");
  //rt_sem_p(&sem,TM_INFINITE);
  rt_mutex_acquire(&mutex,TM_INFINITE);
  print_pri(NULL,"Task Low - acquire mutex\n");
  busy_wait_ms(30);
  print_pri(NULL,"Task Low - release mutex\n");
  rt_mutex_release(&mutex);
  rt_printf("End task Low\n");
  return NULL;
}

void* threadFunc2(void){
  rt_sem_p(&syn,TM_INFINITE);
  rt_printf("Start task Medium\n");
  rt_task_sleep_ms(10);
  print_pri(NULL,"Task Medium - run\n");
  busy_wait_ms(50);
  rt_printf("End task Medium\n");
  return NULL;
}

void* threadFunc3(){
  rt_sem_p(&syn,TM_INFINITE);
  rt_printf("Start task High\n");
  rt_task_sleep_ms(20);
  //rt_sem_p(&sem,TM_INFINITE);
  rt_mutex_acquire(&mutex,TM_INFINITE);
  print_pri(NULL,"Task High - acquire mutex\n");
  busy_wait_ms(20);
  //rt_sem_v(&sem);
  print_pri(NULL,"Task High - release mutex\n");
  rt_mutex_release(&mutex);
  rt_printf("End task High\n");
  return NULL;
}


int main(){
  mlockall(MCL_CURRENT | MCL_FUTURE); // lock the current memory allocations and future memory allocations to the main memory
  rt_print_auto_init(1);
  rt_task_shadow (NULL, "Main task", 1, T_CPU(1)|T_JOINABLE);
  rt_sem_create(&syn, "Semaphore sync",0,S_PRIO);
  rt_sem_create(&sem, "Semaphore", 1,S_PRIO);
  rt_mutex_create(&mutex,"Mutty");

  RT_TASK test1;
  rt_task_create(&test1,"Task 1", 0, 3,T_CPU(1)|T_JOINABLE);
  rt_task_start(&test1, threadFunc1,NULL);

  RT_TASK test2;
  rt_task_create(&test2,"Task 2", 0, 4,T_CPU(1)|T_JOINABLE);
  rt_task_start(&test2, threadFunc2,NULL);

  RT_TASK test3;
  rt_task_create(&test3,"Task 3", 0, 5,T_CPU(1)|T_JOINABLE);
  rt_task_start(&test3, threadFunc3,NULL);

  rt_task_sleep(10000000);

  rt_sem_broadcast( &syn);

  rt_task_sleep(1000000000);
  rt_sem_delete( &sem);

  return 0;
}
