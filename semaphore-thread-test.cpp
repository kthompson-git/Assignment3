#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>

// semaphore
sem_t sem;

void *test(void *number)
{
  int i = number;
  printf("Thread Number: %d\n", i);
  return NULL;
}

int main(int argc, char *argv[]) 
{

  
  // initialize semaphore
  sem_init(&sem, 0, 0);

  // create thread array
  pthread_t tid[128];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  void *status;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  int num = 0;

  while (num < 10)
  {
    if (pthread_create(&tid[i], NULL, test, num))
    {
      fprintf(stderr, "Error creating thread.\n");
      exit(1);
    }
    num++;
  }
 
  pthread_attr_destroy(&attr);
  for (int i = 0; i < num; i++ )
  {
    sem_post(&sem);
    if (pthread_join(tid[i], &status)) 
    {
      std::cout << "Error:unable to join thread" << std::endl;
      exit(-1);
    }
  }
  sem_destroy(&sem);


  return 0;
}