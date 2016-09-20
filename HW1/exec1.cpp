/*The Program does the following
It first forks three (3) child processes.
The main process then enters an endless loop that repeatly displays one line of information like "This is the main process, my PID is ...." (remember to use the actual PID here!), then pauses for about one second, then displays the above information again...
Each of the three child processes repeatly displays one line of information like "This is a child process, my PID is ..., my parent PID is ...", then pauses for about two seconds, then displays the above information again...
*/

#include <stdio.h>   /* printf, stderr, fprintf */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno */

int main()
{      
          int PID, ret;

          PID = fork();

          if (PID == 0) {
             printf("The is a parent\n\n");
             // int execl(const char *path, const char *arg0, ... /*, (char *)0 */);
             ret = execl ("/bin/ps", "ps", "-ef", (char *)0);
          } else {
             printf("This is a child  process\n\n");
             ret = execl ("/bin/ls", "ls", "-1", (char *)0);
          }

          
          return 0;
}
