Programming Assignment 3 
Due date: Monday, 2 December 2019, 11:59 PM 
Requested files: main.cpp (Download) 
Type of work: Individual work 
Reduction by automatic evaluation: 5 Free evaluations: 10 
COSC 3360 – Operating System Fundamentals 
Assignment #3 for Fall 2019: The decompression algorithm using synchronization mechanisms. 
Similarity Threshold: 85%

### Objective  
This assignment will test your knowledge about synchronization mechanisms in UNIX.

### Specifications:  
You must write a c++ program to implement the decompression process of the compression algorithm proposed by Rincon et al. in 2008 (A Probabilistic Compression Algorithm based on Symbols Position). Your program must use threads to implement the decompression process.

##### The main thread:

This thread will read the compressed file using input redirection:

` ./exec_filename < input_filename `

where exec_filename is the name of your executable file and input_filename is the name of the file with the compressed message.

The format of the input file is as follow:

a 11110001110000111110  
n 11100000  
k 11000  
l 110  
p 1  

Each line of the input file represents a symbol and its binary code. The symbols will appear in the file based on their priority (highest frequency + ASCII Value if the symbols have the same frequency). The possible symbols are letters, numbers, white space, and end-of-line (represented by the string <EOL>).

After reading the information from the input file, the main thread will create n child threads (where n is the number of symbols). It will wait for all the child threads to end their execution, before printing the original message.
 
##### The child threads:

Each child thread will execute the following steps:

1. Receives the information from the parent thread (you must guarantee mutual exclusion).
2. Waits for its time to execute the decompression process of the part of the original message assigned to the thread.
3. Prints the information about the symbol and binary code used for the decompression process.
4. Saves its part of the decompressed message into an address space that is shared with the main thread (you must guarantee mutual exclusion).
5. Finishes its execution.

##### Restrictions:

1. You can only declare as global variables the semaphores and condition variables used to guarantee mutual exclusion.
2. You must use the same memory address space to pass the parameters from the main thread to the child threads.
3. You must guarantee that the threads are executed in the right order.
4. You must guarantee that more than one child thread is in the system at a particular time of the execution of your program.

##### Example:

For the previous input file, the corresponding output is:    

p Binary code = 1  
l Binary code = 110  
k Binary code = 11000  
n Binary code = 11100000  
a Binary code = 11110001110000111110  
Decompressed file contents:   
aaaannnaaakkllaaaaap   
 
### HINTS: 

- You can safely assume that the input files will always be in the proper format and that the maximum number of symbols used to create a message is 100.
- You must use the format from the previous example when writing information into STDOUT.


These specifications were written on Wednesday, November 6, 2019. Please refer to the course web site for corrections and updates.  

