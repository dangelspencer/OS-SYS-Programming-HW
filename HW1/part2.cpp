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
        //child process 3
        sleep(2);
        ret = execl ("/bin/cat", "cat", argv[1], (char *)0);
        printf("child 3 \n");
      }
      //child process 2
      sleep(1);
      ret = execl ("/bin/ps", "ps", "-ef", (char *)0);
      printf("child 2 \n");
    }
    //child process 1
    ret = execl ("/bin/ls", "ls", "-1", (char *)0);
    printf("child 1 \n");
  }
  sleep(3);
  cout << "main process terminates" << endl;
  wait(&status);
  return 0;

}    
