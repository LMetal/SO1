#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>


void f_intr(int sig)
{
  printf("\n\nOKOK\n\n");
}
//11
int main(int argc, char *argv[]) {
  int m,n;
  struct sigaction sa;

  sa.sa_handler = f_intr;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGSEGV, &sa,NULL);

  for (m=0;m<1000;m++) {
    for (n=0;n<200000000;n++); //22
    printf("working %d PID: %d\n",m, getpid());
  }

  return 0;
}


// SIGINT Ctrl + c
// SIGTERM
