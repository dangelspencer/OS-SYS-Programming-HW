#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<sys/wait.h>
#include<fcntl.h>

using namespace std;

int main (int argc, char* argv[]){

    if (argc != 3){
        cout << "Usage: ./a.out <program_name> <text_file_name>\n";
        return -1;
    }
    
    if (fork() == 0){
        int file = open(argv[2], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        
        dup2(file , 1);
        
        close(file);
        
        execl(argv[1], argv[1], (char *)0);
    }

    return 0;
}

