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
      printf("Child Process -> (PID =%d, PPID = %d)\n", getpid(), getppid());
      break;
    }
  }
  sleep(1);
}
