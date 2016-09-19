#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<iostream>

using namespace std;

int main(int argc, char* argv[]){
  
  if (argc < 2){
    cout << "Usage: " << argv[0] << " N" << endl;
    return 0;
  }
  
  int PID;
  int data = 10;
  int N = atoi(argv[1]);
  
  for (int i = 0; i < 3; i++){
    PID = fork();
  }
  
  if (PID == 0){
    //parent process 
    for (int i = 0; i < N; i++){
      printf("This is the main process, my PID is %d\n", PID);
      sleep(2);
    }
  } else {
    //child process
    for (int i = 0; i < N; i++){
      printf("This is a child process, my PID is %d, my parent PID is %d\n", PID, getppid());
    }
  }

  return 0;
}
