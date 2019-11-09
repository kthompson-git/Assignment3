#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>

// semaphore
sem_t sem;

// structure for info
struct Data
{
	public:
    int count;
    char sym;
    char message[1000];
    char code[1000];
};

void threadPrint(char sym, std::string code)
{
  if (sym == '\n')
    std::cout << "<EOL> Binary code = " << code << std::endl;
  else
    std::cout << sym << " Binary code = " << code << std::endl;
  return;
}

void mainPrint(std::string message)
{
  std::cout << "Decompressed file contents:\n" << message;
  return;
}

// checks if the EOL character
bool checkEOL(char first, char second)
{
  if (first == '<' && second == 'E')
    return true;
  else
    return false;
}


// decoding thread
void *decode(void *dataRef)
{
  struct Data *data = (struct Data *) dataRef;
  std::string code = data->code, 
  if (data->message != null) message = data->message;
  char sym = data->sym;
  int count = data->count;
  sem_wait(&sem);
  int msgIndex = 0;
  std::string temp = message;
  for (int i = 0; i < count; i++)
  {
    if (code[i] == 1)
      message[i] = sym;
    else if (code[i] == 0)
    {
      message[i] = temp[msgIndex];
      msgIndex++;
    }
  }
  threadPrint(sym, code);
  data->message = message;
  return NULL;
}

int main(int argc, char *argv[]) 
{
  // printf("declaring variables\n");
  static struct Data *data = new Data();
  std::string line;
  int i, charCount = 0;

  // printf("init semaphore\n");
  sem_init(&sem, 0, 0);

  // printf("create thread\n");
  // create thread
  pthread_t tid[128];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  void *status;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  // printf("entering while loop\n");
  while (std::getline(std::cin, line))
  {
    // printf("in while statement. line read: \n"); std::cout << line << std::endl;

    if (checkEOL(line[0], line[1]))
    {
      // printf("EOL true\n");
      data->sym = '\n';
      i = 6;
    }
    else
    {
      // printf("EOL false\n");
      // std::cout << "line[0] = " << line[0] << std::endl;
      // std::cout << "data->sym = " << data->sym << std::endl;
      data->sym = line[0];
      // printf("symbol set in struct\n");
      i = 2;
    }
    // printf("set data count to zero\n");
    data->count = 0;
    for (; i < line.length(); i++)
    {
      data->code[data->count] = line[i];
      data->count += 1;
    }
    if (pthread_create(&tid[charCount], NULL, decode, data))
    {
      fprintf(stderr, "Error creating thread.\n");
      exit(1);
    }
    charCount++;
  }

  pthread_attr_destroy(&attr);
  for (int i = 0; i < charCount; i++ )
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