
/********  N produttori, 1 consumatore con condizioni e thread ********/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5    		/* dim. buffer */
#define NPROD 8    	/* numero produttori */
#define NGIRI 20 	/* quanti caratteri prodotti da ogni produttore */

struct buffer{
        char pool[N];
        int in,out,count;
        } buf;

pthread_mutex_t m;
pthread_cond_t empty,full;


void printstatus()	/* stampa contenuto del buffer */
{
  int i;
 
  printf("Contenuto del buffer:");
  for (i=0;i<buf.count;i++)
      putchar(buf.pool[(buf.out+i)%N]);
}

void put(char i)
{
 
  buf.pool[buf.in]=i;
  buf.in = ((buf.in)+1)%N;
  buf.count++;
 
  printstatus();
  printf(" dopo put del carattere %c\n",i);
  fflush(stdout); /* cosi' va bene anche se si ridirige l'output su disco */
}

void get(char *ip)
{

  *ip=buf.pool[buf.out];
  buf.out = ((buf.out)+1)%N;
  buf.count--;
  
  printstatus();
  printf(" dopo get del carattere %c\n",*ip);
  fflush(stdout); /* cosi' va bene anche se si ridirige l'output su disco */
}
 

void *producer(void *p)	/* produce NGIRI volte il carattere *p */
{
  int i;

  for (i=0;i<NGIRI;i++)
        {
 
	int j;

for (j=0;j<100000;j++); /* per rallentare */
	pthread_mutex_lock(&m);
//        put(*(char *)p);
	printf("Thread %c in sez critica\n", *((char *)p));
        
	pthread_mutex_unlock(&m);
        }
  return NULL;
}

 
int main(int argc, char *argv[])
{

int i;
char c[NPROD];
pthread_t t[NPROD+1];

pthread_mutex_init(&m,NULL);




for(i=0;i<NPROD;i++)
	{
	  c[i]='0'+i;
	  pthread_create(&t[i], NULL, producer, &c[i]);
	}

for(i=0;i<NPROD;i++)
	pthread_join(t[i], NULL);


}

