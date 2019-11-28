#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <fcntl.h>

// semaphore
sem_t *semM, *semT;

void *test(void *number)
{  
  int *i = (int *)number;
  int j = *i;
  printf("Thread Number: %d\n", j);
  sem_post(semM);
  sem_wait(semT);
  printf("Thread Number: %d\n", j);
  
  return NULL;
}

int main(int argc, char *argv[]) 
{
  printf("Begin\n");
  char nameM[] = "KMAIN";
  char nameT[] = "KTHREAD";
  int value=10;
  
  // initialize semaphore
  printf("Create semaphore\n");
  semM = sem_open(nameM, O_CREAT, 0600 , 0);
  semT = sem_open(nameT, O_CREAT, 0600 , 0);

  // create thread array
  printf("Create thread array\n");
  pthread_t tid[128];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  void *status;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  


  static int num = 0;
  while (num < 10)
  {
    if (pthread_create(&tid[num], NULL, test, &num))
    {
      fprintf(stderr, "Error creating thread.\n");
      exit(1);
    }
    sem_wait(semM);
    printf("After sem wait main\n");
    sem_getvalue(semT,&value);	
	  std::cout << "Thread Semaphore Value = " << value << std::endl;
    num++;
  
  }
  printf("Out of While Loop\n");
  
 
  pthread_attr_destroy(&attr);
  for (int i = 0; i < num; i++ )
  {
    sem_post(semT);
    if (pthread_join(tid[i], &status)) 
    {
      std::cout << "Error:unable to join thread" << std::endl;
      exit(-1);
    }
  }

	sem_getvalue(semM,&value);	
	printf("Main Semaphore Value = %d\n", value);
	sem_getvalue(semT,&value);	
	printf("Thread Semaphore Value = %d\n", value);
  sem_unlink(nameM);
  sem_unlink(nameT);


  return 0;
}