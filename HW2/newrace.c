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

/********************
  Additional inlcudes
********************/
#include <sys/sem.h>
#include <sys/ipc.h> /******** this one you did not have ********/

#define FILE_SIZE 11
#define NO_PROC   10

/******************
   semun after union is not necessary
  This is your code that I copied over
******************/
typedef union
  {
    int val;
    struct semid_ds *buf;
    ushort *array;
  }semun_t;

/******************
  Here you want to set the semaphore to equal 1 something about arrays
  You want an array size one equal to one
  Your code copied over
******************/
void set(int semid){
  /* Literally 4 lines of code here
    1.
    2.
    3.
    4.
  */
  if (semctl(semid,0,SETVAL,arg)<0 )
  {
    perror("semctl failure Reason:"); /* New reason now */
    exit(-1);
  }
}

/******************
  Here you want to create the key and SET the id and return semid
  Hint: look at ftok("","")
  This is your code that I copied over
******************/
int initial(int nsems){
  /*
    Need to make key here
  */
  int semid = semget(IPC_PRIVATE,2,0666|IPC_CREAT); /* this is what i have inside semget; key, nsems, IPC_CREAT | 0666 */
  if(semid<0)
  {
    perror("semget failed Reason:");
    exit(-1);
  }
  /* Call here */
  /* return here */
}

/******************
  This was your code that i copied over
  This is correct
******************/
void lock(int semid){
  struct sembuf op1;   
  op1.sem_num = 0;
  op1.sem_op = -1;
  op1.sem_flg = 0; /* Should not be zero, try SEM_UNDO */
  if (semop(semid,&op1,1) == -1)
  {
    perror("Thread1:semop failure Reason:");
    exit(-1);
  }
}

/******************
  This was your code that I copied over
  This is correct
******************/
void unlock(int semid){
  struct sembuf op1;
  op1.sem_num = 0;
  op1.sem_op = 1;
  op1.sem_flg = 0;/* Needs to be the same as lock */
  if (semop(semid,&op1,1) == -1)
  {
    perror("Thread1:semop failure Reason:");
    exit(-1);
  }
}

/******************
  Somewhat correct
******************/
void remove(int semid){
  if (semctl(semid,0,SETVAL,arg)<0 ) /* This is what i have: SETVAL --> IPC_RMID; get rid of arg;*/
  {
    perror("semctl failure Reason:");
    exit(-1);
  }
}


int DelayCount = 0;

int readerID = 0;
int writerID = 0;

char* shared_buffer;

int Delay100ms = 0;

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
void reader()
{
  int i,j,n;
  char results[FILE_SIZE];

  srand(2);
  for (i=0; i<1; i++) {
      printf("Reader %d (pid = %d) arrives\n", readerID, getpid()); 

      /*
          LOCK THE FOR LOOP HERE
      */

      /* read data from shared data */
      for (j=0; j<FILE_SIZE; j++) {
         results[j] = shared_buffer[j]; 
         delay(4);  
      }

      /*
          UNLOCK THE FOR LOOP HERE
      */
 
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
void writer()
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
      /*
          LOCK THE FOR LOOP HERE
      */

      /* write to shared buffer */
      for (j=0; j<FILE_SIZE-1; j++) {
          shared_buffer[j]= data[j]; 
          delay(3);  
      }
      /*
          UNLOCK THE FOR LOOP HERE
      */

      printf("Writer %d finishes\n", writerID);
  }
}


/*-------------------------------------------

      Routines for creating readers and writers

*-------------------------------------------*/
void create_reader()
{
    if (0 == fork()) {
        reader(); /* HMMMMM I WONDER WHAT GOES IN HERE */
        exit(0);
    }

    readerID++;
}

void create_writer()
{
    if (0 == fork()) {
        writer(); /* HMMMMM I WONDER WHAT GOES IN HERE */
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

  /******************************** HMMM YOU NEED TO INITIAL HERE TO INT SEMID ************************/

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

/***********************************
  Each create_reader and create_writer needs an ID
***********************************/

  create_reader();
  delay(1);
  create_writer();
  delay(1);
  create_reader();
  create_reader();
  create_reader();
  delay(1);
  create_writer();
  delay(1);
  create_reader();

  /* delay 15 seconds so all previous readers/writes can finish.
   * This is to prevent writer starvation
   */
  delay(150);


  create_writer();
  delay(1);
  create_writer();
  delay(1);
  create_reader();
  create_reader();
  create_reader();
  delay(1);
  create_writer();
  delay(1);
  create_reader();
  
  /*------------------------------------------------------- 
  
      Wait until all children terminate
  
  --------------------------------------------------------*/
  for (i=0; i<(readerID+writerID); i++) {
      wait(NULL);
  }

/* WELL THE SEMID NEEDS TO DISAPPEAR HERE I WONDER WHAT FUNCTION THAT COULD BE */
}

