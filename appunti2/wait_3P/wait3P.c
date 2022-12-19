#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
  if(argc != 2){
    printf("\n\nERRORE PARAMETRI\n\n");
    return -1;
  }
  int status;
  for(int i=0; i<atoi(argv[1]); i++){
    int n=fork();
    if(n == (pid_t)-1){
      printf("\n\nerrore fork\n\n");
    }
    else if(n == (pid_t)0){ //se figlio
      continue;
    }
    else {  //se padre
      waitpid(n, &status, 0);
      if(WIFEXITED(status)){ // vera se figlio finisce normalmente
        printf("P%d\tPID : %d\n", i, getpid());
        break;
      }
    }
  }


  return 0; /* ... ma facciamo male a non verificare errori nelle system calls */
}
