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
  sem_wait(&sem);
  if (data->message == NULL)
  {
    for (int i = 0; i < data->count; i++)
    {
      data->message[i] = data->sym;
    }
  }
  else
  {
    int msgIndex = 0;
    std::string temp = data->message;
    for (int i = 0; i < data->count; i++)
    {
      if (data->code[i] == 1)
        data->message[i] = data->sym;
      else if (data->code[i] == 0)
      {
        data->message[i] = temp[msgIndex];
        msgIndex++;
      }
    }
  }
  threadPrint(data->sym, data->code);
  return;
}

int main(int argc, char *argv[]) 
{
  static struct Data *data = NULL;
  std::string line;
  int i, charCount = 0;

  sem_init(&sem, 0, 0);

  // create thread
  pthread_t tid[128];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  void *status;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  while (std::getline(std::cin, line))
  {
    if (checkEOL(line[0], line[1]))
    {
      data->sym = '\n';
      i = 6;
    }
    else
    {
      data->sym = line[0];
      i = 2;
    }
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
    sem_post&sem);
    if (pthread_join(tid[i], &status)) 
    {
      std::cout << "Error:unable to join thread" << std::endl;
      exit(-1);
    }
  }
  sem_unlink(&sem);


  return 0;
}