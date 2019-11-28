#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

// semaphore
sem_t *semM, *semT;

// structure for temp storage of input lines
struct Code
{
	public:
    char code[1000];
    Code *next;
};

// push new node to head of linked list
void push(Code **headRef, char msg[])  
{ 
  Code* newNode = new Code();
  int i = 0;
  while (msg[i])
  {
    newNode->code[i] = msg[i];
    i++;  
  }
  newNode->next = (*headRef);
  (*headRef) = newNode;
  return;
} 

// pop node from front of linked list
void pop(Code **head) 
{ 
    if (head == NULL) 
        return NULL; 
    Code *temp = head; 
    head = head->next;  
    delete temp;   
    return; 
} 

// function to write to file
// void writeToFile(std::string input, std::string fileName)
// {
//   std::ofstream outFile;
//   outFile.open(fileName, std::ios::app);
//   outFile << input << std::endl;
//   outFile.close();
//   return;
// }

// // concatenate multiple file contents to one
// void readFile(std::string fileName)
// {
//   std::string input;
//   std::ifstream inFile;
//   inFile.open(fileName, std::ios::in);
//   std::getline(inFile, input);
//   inFile.close();
//   writeToFile(input, "input");
//   return;
// }

// // reverse reading order for main function
// void reverseFile()
// {
//   int fileLineCnt = 0;
//   std::string temp, fileName;
//   while (std::getline(std::cin, temp))
//   {
//     fileName = "input" + std::to_string(fileLineCnt);
//     writeToFile(temp, fileName);
//     fileLineCnt++;
//   }
//   for (int i = fileLineCnt - 1; i >= 0; i--)
//   {
//     fileName = "input" + std::to_string(i);
//     readFile(fileName);
//   }
//   return;
// }

// print message received by thread
void threadPrint(char sym, char code[])
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

// decoding thread
void *decode(void *codeArray)
{
  char *tempArray = (char *)codeArray;
  int msgSize = messageSize(tempArray);
  char  newArray[msgSize];
  char key;
  int i;
  if (checkEOL(tempArray[0], tempArray[1]))
  {
    key = '\n';
    for (i = 6; i <= msgSize; i++)
      newArray[i - 6] = tempArray[i];
  }
  else
  {
    key = tempArray[0];
    for (i = 2; i <= msgSize; i++)
      newArray[i - 2] = tempArray[i];
  }  
  sem_post(semM);
  sem_wait(semT);
  threadPrint(key, newArray);
  replaceOnes(key, newArray);
  i = 0;
  while (newArray[i])
  {
    tempArray[i] = newArray[i];
    i++;
  }
  tempArray[i] = '\0';
  sem_post(semM);
  return NULL;
}

// copy array
void copyArray(char sharedMem[], char temp[])
{
  int i = 0;
  while (sharedMem[i])
  {
    temp[i] = sharedMem[i];
    i++;
  }
  return;
}

// combine returned decoded messages
void combineMessages(char sharedMem[], char temp[])
{
  int i = 0, j = 0;
  while (sharedMem[i])
  {
    if (sharedMem[i] == '0')
    {
      sharedMem[i] = temp[j];
      j++;
    }
    i++;
  }
  copyArray(sharedMem, temp);
  return;
}

int main(int argc, char *argv[]) 
{
  // set up semaphores
  char nameM[] = "KMAIN"; // semaphore for main thread
  char nameT[] = "KTHREAD"; // semaphore for child threads
  semM = sem_open(nameM, O_CREAT, 0600 , 0);
  semT = sem_open(nameT, O_CREAT, 0600 , 0);

  // start linked list for data
  struct Code *head = NULL;
  
  // declare shared memory between threads
  static char sharedArray[1000];
  
  // copy of sharedArray when combing decoded messages
  char tempMsg[1000];

  // create thread properties
  int threadNum = 0; // keep track of num threads to join later
  pthread_t tid[128]; // account for all ASCII characters
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  void *status;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  // reverse file contents
  // reverseFile();
  
  std::ifstream inFile;
  inFile.open("input", std::ios::in);
  while (inFile.getline(sharedArray, 1000))
  {
    if (pthread_create(&tid[threadNum], NULL, decode, &sharedArray))
    {
      fprintf(stderr, "Error creating thread.\n");
      exit(1);
    }
    sem_wait(semM);
    threadNum++;
  }

  pthread_attr_destroy(&attr);
  for (int i = 0; i < threadNum; i++ )
  {
    sem_post(semT);
    sem_wait(semM);
    if (i == 0)
      copyArray(sharedArray, tempMsg);
    else
      combineMessages(sharedArray, tempMsg);
    if (pthread_join(tid[i], &status)) 
    {
      std::cout << "Error:unable to join thread" << std::endl;
      exit(-1);
    }
  }
  
  std::cout << "Decompressed file contents:\n" << sharedArray << std::endl;
  
  system("rm input*");
  sem_unlink(nameM);
  sem_unlink(nameT);

  return 0;
}