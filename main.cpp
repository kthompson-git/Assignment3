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

// function to write to file
void writeToFile(std::string input, std::string fileName)
{
  std::ofstream outFile;
  outFile.open(fileName, std::ios::app);
  outFile << input << std::endl;
  outFile.close();
  return;
}

void readFile(std::string fileName)
{
  std::string input;
  std::ifstream inFile;
  inFile.open(fileName, std::ios::in);
  std::getline(inFile, input);
  inFile.close();
  writeToFile(input, "input");
  return;
}

void reverseFile()
{
  int fileLineCnt = 0;
  std::string temp, fileName;
  while (std::getline(std::cin, temp))
  {
    fileName = "input" + std::to_string(fileLineCnt);
    writeToFile(temp, fileName);
    fileLineCnt++;
  }
  for (int i = fileLineCnt - 1; i >= 0; i--)
  {
    fileName = "input" + std::to_string(i);
    readFile(fileName);
  }

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
  if (checkEOL(tempArray[0], tempArray[1]))
  {
    for (int i = 6; i <= msgSize; i++)
    {
      // printf("Char:\t%c\n", codeArray[i]);
      tempArray[i - 6] = codeArray[i];    
    }
    // printf("Key:\t%c\n", key);
    replaceOnes(key, tempArray);
    // std::cout << "Decoded: " << tempArray << std::endl;
  }
  else
  {
    for (int i = 2; i <= msgSize; i++)
    {
      // printf("Char:\t%c\n", codeArray[i]);
      tempArray[i - 2] = codeArray[i];    
    }
    // printf("Key:\t%c\n", key);
    replaceOnes(key, tempArray);
    // std::cout << "Decoded: " << tempArray << std::endl;
  }
    
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
void *decode(void *codeArray)
{
  char *tempArray = (char *)codeArray;
  int msgSize = messageSize(tempArray);
  char  newArray[msgSize - 2];
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
  
  std::cout << "Encoded: " << newArray << std::endl;
  replaceOnes(key, newArray);
  std::cout << "Decoded: " << newArray << std::endl;
  
  
  sem_post(semM);
  return NULL;
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

  // create thread properties
  int threadNum = 0; // keep track of num threads to join later
  pthread_t tid[128]; // account for all ASCII characters
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  void *status;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  reverseFile();
  
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

    // windows OS test
    // std::cout << sharedArray << std::endl;
    // decodeWindows(sharedArray);    
    // push(&head, sharedArray);


  }
  // printf("\nPrinting linked list\n\n");
  printList(head);

  pthread_attr_destroy(&attr);
  for (int i = 0; i < threadNum; i++ )
  {
    sem_post(semT);
    if (pthread_join(tid[i], &status)) 
    {
      std::cout << "Error:unable to join thread" << std::endl;
      exit(-1);
    }
  }
  sem_unlink(nameM);
  sem_unlink(nameT);

  return 0;
}