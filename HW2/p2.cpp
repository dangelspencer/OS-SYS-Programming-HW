#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<iostream>
#include<sys/wait.h>

using namespace std;

int main(int argc, char* argv[]){
  for (int i = 0; i < 5; i++){
    
    if (fork() == 0){
      printf("This is a child process, my PID is %d, my parent PID is %d\n", getpid(), getppid());
      break;
    }
  }
  //small pause for all child processes to finish (not required for the program to run correctly)
  sleep(1);
}
