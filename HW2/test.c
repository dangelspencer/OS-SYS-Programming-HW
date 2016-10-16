#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/times.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define FILE_SIZE 11
#define NO_PROC   10

int DelayCount = 0;

int readerID = 0;
int writerID = 0;

char* shared_buffer;

int Delay100ms = 0;

typedef union
  {
    int val;
    struct semid_ds *buf;
    ushort *array;
  }smn_t;


void setVal(int semid){
  smn_t arg;
  arg.val = 1;
  
  if (semctl(semid,0,SETVAL, arg)<0 )
  {
    perror("semctl failure Reason:");
    exit(-1);
  }
}

int startVal(int nsems){

  key_t key = (shared_buffer, 1);
  int semid = semget(key, nsems, IPC_CREAT | 0666); 
  if(semid<0)
  {
    perror("semget failed Reason:");
    exit(-1);
  }
  
  setVal(semid);
  return semid;
}

void lockThread(int semid){
  struct sembuf op1;   
  op1.sem_num = 0;
  op1.sem_op = -1;
  op1.sem_flg = SEM_UNDO;
  if (semop(semid,&op1,1) == -1)
  {
    perror("Thread1:semop failure Reason:");
    exit(-1);
  }
}

void unlockThread(int semid){
  struct sembuf op1;
  op1.sem_num = 0;
  op1.sem_op = 1;
  op1.sem_flg = SEM_UNDO;
  if (semop(semid,&op1,1) == -1)
  {
    perror("Thread1:semop failure Reason:");
    exit(-1);
  }
}

void removeSemID(int semid){
  if (semctl(semid,0,IPC_RMID)<0 ) 
  {
    perror("semctl failure Reason:");
    exit(-1);
  }
}

/*-------------------------------------------
    Delay routines
 --------------------------------------------*/
void basic_delay()
{
   long i,j,k;
   for (i=0;i<200L;i++) {
       for (j=0;j<400L;j++) {
          k = k+i;
       }
   }
}

/* do some delays (in 100 ms/tick) */
void delay(int delay_time)
{
   int i,j;

   for (i=0; i<delay_time; i++) {
      for (j=0;j<Delay100ms;j++) {
          basic_delay();
      }
   }
}


int stop_alarm = 0;

static void
sig_alrm(int signo)
{
    stop_alarm = 1;
}


/*------------------------------------------
 * Since the speed of differet systems vary,
 * we need to calculate the delay factor 
 *------------------------------------------
 */
void calcuate_delay()
{
    int i;
    struct  tms t1;
    struct  tms t2;
    clock_t t;
    long    clktck;
    double  td;

    printf(".... Calculating delay factor ......\n");
    stop_alarm = 0;
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
       perror("Set SIGALRM");
    alarm(5);  /* stop the following loop after 5 seconds */

    times(&t1);
    while (stop_alarm == 0) {
        DelayCount++;
        basic_delay();
    }    
    times(&t2);
    alarm(0);   /* turn off the timer */

    /* Calcluate CPU time */
    t = t2.tms_utime - t1.tms_utime; 

    /* fetch clock ticks per second */
    if ( (clktck = sysconf(_SC_CLK_TCK)) < 0 )
       perror("sysconf error");

    /* actual delay in seconds */
    td = t / (double)clktck;
    
    Delay100ms = DelayCount/td/10;

    if (Delay100ms == 0)
       Delay100ms++;

    printf(".... End calculating delay factor\n");
}




/*-------------------------------------------
   The reader
 --------------------------------------------*/
void reader(int semid)
{
  int i,j,n;
  char results[FILE_SIZE];

  srand(2);
  for (i=0; i<1; i++) {
      printf("Reader %d (pid = %d) arrives\n", readerID, getpid()); 

      lockThread(semid);

      /* read data from shared data */
      for (j=0; j<FILE_SIZE; j++) {
         results[j] = shared_buffer[j]; 
         delay(4);  
      }

      unlockThread(semid);
 
      /* display result */
      results[j] = 0;
      printf("      Reader %d gets results = %s\n", 
              readerID, results);
  }
}




/*-------------------------------------------
   The writer. It tries to fill the buffer
   repeatly with the same digit 
 --------------------------------------------*/
void writer(int semid)
{
  int i,j,n;
  char data[FILE_SIZE];

  srand(1);

  for (j=0; j<FILE_SIZE-1; j++) {
      data[j]= writerID + '0';
  }
  data[j]= 0;

  for (i=0; i<1; i++) {
      printf("Writer %d (pid = %d) arrives, writing %s to buffer\n", 
              writerID, getpid(), data);
      
      lockThread(semid);

      /* write to shared buffer */
      for (j=0; j<FILE_SIZE-1; j++) {
          shared_buffer[j]= data[j]; 
          delay(3);  
      }
     
      unlockThread(semid);

      printf("Writer %d finishes\n", writerID);
  }
}


/*-------------------------------------------

      Routines for creating readers and writers

*-------------------------------------------*/
void create_reader(int semid)
{
    if (0 == fork()) {
        reader(semid);
        exit(0);
    }

    readerID++;
}

void create_writer(int semid)
{
    if (0 == fork()) {
        writer(semid); 
        exit(0);
    }

    writerID++;
}



/*-------------------------------------------
 --------------------------------------------*/
void main() 
{ 
  int return_value;
  char InitData[]="0000000000\n";
  int i;
  int fd;

  calcuate_delay();

  int semid = startVal(1);

  /*-------------------------------------------------------
   
       The following code segment creates a memory
     region shared by all child processes
       If you can't completely understand the code, don't
     worry. You don't have to understand the detail
     of mmap() to finish the homework
  
  -------------------------------------------------------*/ 

  fd = open("race.dat", O_RDWR | O_CREAT | O_TRUNC, 0600);
  if ( fd < 0 ) {
     perror("race.dat ");
     exit(1);
  }

  write(fd, InitData, FILE_SIZE);

  unlink("race.dat");

  shared_buffer = mmap(0, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if ( shared_buffer == (caddr_t) -1) {
     perror("mmap");
     exit(2);
  }
     

  /*------------------------------------------------------- 
  
       Create some readers and writes (processes)
  
  -------------------------------------------------------*/ 

  create_reader(semid);
  delay(1);
  create_writer(semid);
  delay(1);
  create_reader(semid);
  create_reader(semid);
  create_reader(semid);
  delay(1);
  create_writer(semid);
  delay(1);
  create_reader(semid);

  /* delay 15 seconds so all previous readers/writes can finish.
   * This is to prevent writer starvation
   */
  delay(150);


  create_writer(semid);
  delay(1);
  create_writer(semid);
  delay(1);
  create_reader(semid);
  create_reader(semid);
  create_reader(semid);
  delay(1);
  create_writer(semid);
  delay(1);
  create_reader(semid);
  
  /*------------------------------------------------------- 
  
      Wait until all children terminate
  
  --------------------------------------------------------*/
  for (i=0; i<(readerID+writerID); i++) {
      wait(NULL);
  }

  removeSemID(semid);
}

