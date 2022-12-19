#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct dati{
  unsigned int seconds;
  char message[64];
} Sveglia;

void *tbody(void *arg)
{
    Sveglia * dati_sveglia = (Sveglia *)arg;

    sleep(dati_sveglia->seconds);
    printf("(%d) %s\n", dati_sveglia->seconds, dati_sveglia->message);

    free(dati_sveglia);
}


int main(int argc, char *argv[]){
  char line[128];
  pthread_t t;
  Sveglia * sveglia;

  while(1){
    sveglia = malloc(sizeof(Sveglia));
    printf("Allarme >");

    if(fgets(line, sizeof(line), stdin)==NULL) exit(0);
    if(strlen(line)<=1) continue;

    if(sscanf(line, "%d %64[^\n]", &(sveglia->seconds), sveglia->message)<2) {
      fprintf(stderr, "Comando sconosciuto\n");
    } else {
      pthread_create(&t, NULL, tbody, sveglia);
      }

    }

}
