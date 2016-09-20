#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<iostream>

using namespace std;

int main(int argc, char* argv[]){
  
  if (argc < 2){
    cout << "Usage: " << argv[0] << " <file_name>" << endl;
    return 0;
  }
  int PID, ret;
printf("Parent PID %d\n",getpid());

if(fork()==0) {
    ret = execl ("/bin/ls", "ls", "-1", (char *)0);
    printf("child 1 \n");
    if(fork()==0) {
        ret = execl ("/bin/ps", "ps", "-ef", (char *)0);
        printf("child 2 \n");
        if(fork()==0) {
            ret = execl ("/bin/cat", "cat", argv[1], (char *)0);
            printf("child 3 \n");
        }
    }
}
return 0;
}    
