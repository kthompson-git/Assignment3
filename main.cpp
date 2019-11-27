#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
// #include <semaphore.h>
// #include <pthread.h>
#include <sys/types.h>

// semaphore
// sem_t sem;

// structure for info
struct Code
{
	public:
    char partialMessage[1000];
    Code *next;
};

// push new node to head of linked list
void push(Code** head_ref, char msg[])  
{ 
  // printf("In push function\n"); 
  Code* newNode = new Code();
  // printf("Created new node\n");  
  int i = 0;
  // printf("Entering while loop\n");
  while (msg[i])
  {
    // printf("%c", msg[i]);
    newNode->partialMessage[i] = msg[i];
    i++;  
  }
  // printf("Copied msg into partialMessage\n");
  newNode->next = (*head_ref);
  // printf("Pointed new node->next node to head\n");
  (*head_ref) = newNode;
  // printf("Made new node the new head\n");  
  return;
} 

void printList(Code *node)  
{  
  while (node != NULL)  
  {  
    std::cout << node->partialMessage << std::endl;  
    node = node->next;  
  } 
  return;
}  

void threadPrint(char sym, std::string code)
{
  if (sym == '\n')
    std::cout << "<EOL> Binary code = " << code << std::endl;
  else
    std::cout << sym << " Binary code = " << code << std::endl;
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

// get message length
int messageSize(char codeArray[])
{
  int i = 0;
  while (codeArray[i])
    i++;
  // printf("message size:\t%d\n", i);
  return i;
}

// replace 1's with character
void replaceOnes(char key, char code[])
{
  int i = 0;
  while (code[i])
  {
    if (code[i] == '1')
      code[i] = key;
    i++;
  }
  return;
}

// decode function test on Windows OS
void decodeWindows(char codeArray[])
{
  int msgSize = messageSize(codeArray);
  char tempArray[msgSize - 2];
  char key = codeArray[0];
  for (int i = 2; i <= msgSize; i++)
  {
    // printf("Char:\t%c\n", codeArray[i]);
    tempArray[i - 2] = codeArray[i];    
  }
  // printf("Key:\t%c\n", key);
  replaceOnes(key, tempArray);
  // std::cout << "Decoded: " << tempArray << std::endl;
  int i = 0;
  while (tempArray[i])
  {
    codeArray[i] = tempArray[i];
    i++;
  }
  codeArray[i] = '\0';
  return;
}

// decoding thread
void *decode(void *codeString)
{
  
  return NULL;
}

int main(int argc, char *argv[]) 
{

  // start linked list for data
  struct Code *head = NULL;
  
  // declare shared memory between threads
  static char sharedArray[1000];
  
  // initialize semaphore
  // sem_init(&sem, 0, 0);

  // create thread array
  // pthread_t tid[128];
  // pthread_attr_t attr;
  // pthread_attr_init(&attr);
  // void *status;
  // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  while (std::cin.getline(sharedArray, 1000))
  {
    // if (pthread_create(&tid[charCount], NULL, decode, data))
    // {
    //   fprintf(stderr, "Error creating thread.\n");
    //   exit(1);
    // }

    // windows OS test
    std::cout << sharedArray << std::endl;
    decodeWindows(sharedArray);    
    push(&head, sharedArray);


  }
  // printf("\nPrinting linked list\n\n");
  printList(head);

  // pthread_attr_destroy(&attr);
  // for (int i = 0; i < charCount; i++ )
  // {
  //   sem_post(&sem);
  //   if (pthread_join(tid[i], &status)) 
  //   {
  //     std::cout << "Error:unable to join thread" << std::endl;
  //     exit(-1);
  //   }
  // }
  // sem_destroy(&sem);


  return 0;
}