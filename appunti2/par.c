#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

  int i,j;
  pid_t n;

  if(argc != 3){
    perror("\n\nERRORE INSERIMENTO PARAMETRI\n\n");
    exit(1);
  }


  n=fork();
  //wait();
  if (n==(pid_t)-1)
	{perror("fork fallita");
	 exit(1);
	};

  if (n==(pid_t)0) {
      for (j=0;j<15;j++) {
        for (i=0; i< 100000000; i++);
        printf("  Figlio %d di %d giro %d, n=%d ",getpid(),getppid(),j, n );
        //fflush(NULL);
      }
  }

  else {
    printf("\n\n Figlio \n\n");
      for (j=0;j<5;j++) {
        for (i=0; i< 100000000; i++);
        printf("Padre %d di n=%d giro %d ",getpid(),n,j);
        //fflush(NULL);
      }
  }
  return 0;
}
