#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  printf("enter 1o integers:\n");
  int num, total;
  num = 0;
  total = 0;
  for (int i = 0; i < 10; i++){
    cout << ">> ";
    cin >> num;
    total += num;
  }
  int new_total = total / 10;
  
  printf("%d / 10 = %d\n", total, new_total);
  
  int fd[2];

  pipe(fd);

  if (fork() != 0)
  {
    close(fd[0]);
    write(fd[1], &new_total, sizeof(new_total));
    printf("Parent(%d) send value: %d\n", getpid(), new_total);
    close(fd[1]);
  }
  else
  {   
    close(fd[1]);
    read(fd[0], &new_total, sizeof(new_total));
    printf("Child(%d) received value: %d\n", getpid(), new_total);
    close(fd[0]);
  }
  sleep(1);
  return 0;
}
