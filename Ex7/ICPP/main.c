
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

RT_SEM syn;
RT_MUTEX mutex_A,mutex_B;

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

void* func_low(void){
  rt_sem_p(&syn,TM_INFINITE);
  rt_printf("Start Task Low\n");
  rt_mutex_acquire(&mutex_A,TM_INFINITE);

  rt_task_set_priority(NULL,4);

  print_pri(NULL,"Task Low - acquire Mutex A\n");
  rt_task_sleep(1);
  busy_wait_ms(3);
  rt_mutex_acquire(&mutex_B,TM_INFINITE);
  print_pri(NULL,"Task Low - acquire Mutex B\n");
  busy_wait_ms(3);
  rt_mutex_release(&mutex_B);
  print_pri(NULL,"Task Low - release Mutex B\n");
  rt_mutex_release(&mutex_A);
  print_pri(NULL,"Task Low - release Mutex A\n");

  rt_task_set_priority(NULL,3);

  rt_task_sleep(1);
  busy_wait_ms(1);
  rt_printf("End Task Low\n");
  return NULL;
}

void* func_high(void){
  rt_sem_p(&syn,TM_INFINITE);
  rt_printf("Start Task High\n");
  rt_task_sleep_ms(1);
  rt_mutex_acquire(&mutex_B,TM_INFINITE);
  print_pri(NULL,"Task High - acquire Mutex B\n");
  busy_wait_ms(1);
  rt_mutex_acquire(&mutex_A,TM_INFINITE);
  print_pri(NULL,"Task High - acquire Mutex A\n");
  busy_wait_ms(2);
  rt_mutex_release(&mutex_A);
  print_pri(NULL,"Task High - release Mutex A\n");
  rt_mutex_release(&mutex_B);
  print_pri(NULL,"Task High - release Mutex B\n");
  busy_wait_ms(1);
  rt_printf("End Task High\n");
  return NULL;
}


int main(){
  mlockall(MCL_CURRENT | MCL_FUTURE); // lock the current memory allocations and future memory allocations to the main memory
  rt_print_auto_init(1);
  rt_task_shadow (NULL, "Main task", 1, T_CPU(1)|T_JOINABLE);
  rt_sem_create(&syn, "Semaphore sync",0,S_PRIO);

  rt_mutex_create(&mutex_A,"Mutty A");
  rt_mutex_create(&mutex_B,"Mutty B");

  RT_TASK task_low;
  rt_task_create(&task_low,"Task Low", 0, 3,T_CPU(1)|T_JOINABLE);
  rt_task_start(&task_low, func_low,NULL);

  RT_TASK task_high;
  rt_task_create(&task_high,"Task High", 0, 4,T_CPU(1)|T_JOINABLE);
  rt_task_start(&task_high, func_high,NULL);



  rt_task_sleep(10000000);

  rt_sem_broadcast( &syn);

  rt_task_sleep(1000000000);
  rt_sem_delete( &syn);

  return 0;
}
