#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int f = fork();

  printf("\n\n%d\n", f);
  printf("Hello World\n");
  return 0; /* dovremmo verificare errore fork */
}
