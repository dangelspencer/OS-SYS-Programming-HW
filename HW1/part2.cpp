#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<iostream>
#include<sys/wait.h>

using namespace std;

int main(int argc, char* argv[]){
  
  if (argc < 2){
    cout << "Usage: " << argv[0] << " <file_name>" << endl;
    return 0;
  }
  int PID, ret;
  int status = 0;
  printf("Parent PID %d\n",getpid());

  if(fork()==0) {
      
      if(fork()==0) {
          
          if(fork()==0) {
              
          }else{
            //child process 3
            execl ("/bin/cat", "cat", argv[1], (char *)0);
            printf("child 3 \n");
          }
      }else{
      
        //child process 2
        execl ("/bin/ps", "ps", "-ef", (char *)0);
        printf("child 2 \n");
      }
      
  }else{
    //child process 1
    execl ("/bin/ls", "ls", "-1", (char *)0);
    printf("child 1 \n");
  }

 
 //test code
  /*if (fork() != 0){
    //child process 1
    execl ("/bin/ls", "ls", "-1", (char *)0);
    printf("child 1 \n");
  }
  if (fork() != 0){
    //child process 2
    execl ("/bin/ps", "ps", "-ef", (char *)0);
    printf("child 2 \n");
  }
  if (fork() != 0){
    //child process 3
    execl ("/bin/cat", "cat", argv[1], (char *)0);
    printf("child 3 \n");
  }
  */
  wait(&status);
  return 0;

}    
