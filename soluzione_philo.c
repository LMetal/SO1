#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>

/* 5 filosofi con possibile deadlock */

#define NFILOSOFI 5


sem_t *sem, *mutex;
int shm_fd1, shm_fd2, shm_fd3;


char *stato; /* puntera' ad un array di 5 caratteri in
	        memoria condivisa che serve solo per illustrare
		all'utente lo stato dei 5 filosofi,
		T=thinking, H=hungry, E=eating */

void cleanup(int sig)
{
    munmap(stato, NFILOSOFI*sizeof(char));
    close(shm_fd1);
    munmap(sem, NFILOSOFI*sizeof(sem_t));
    close(shm_fd2);
    munmap(mutex, sizeof(sem_t));
    close(shm_fd3);
    exit(sig);
}

void print()	/* stampa lo stato dei filosofi */
{
    int i;
    for(i=0;i<NFILOSOFI;i++) putchar(stato[i]);
    printf("\n");
}

void test(int i){
    if(stato[i] == 'H' && stato[(i-1)%NFILOSOFI] != 'E' && stato[(i+1)%NFILOSOFI] != 'E'){
        stato[i] = 'E';
        printf("FILOSOFO %d HA PRESO LA FORCHETTA %d \n",i,i);
        printf("FILOSOFO %d HA PRESO LA FORCHETTA %d \n",i,(i+1)%NFILOSOFI);
        printf("FILOSOFO %d MANGIA:\t\t",i);
        print();
        sem_post(&sem[i]);
    }
}

void take_forks(int i){
    sem_wait(mutex);
    stato[i] = 'H';
    printf("FILOSOFO %d E' AFFAMATO:\t\t",i);
    print();
    test(i);

    sem_post(mutex);

    sem_post(&sem[i]);
}

void put_forks(int i){
    sem_wait(mutex);
    stato[i]='T';
    printf("FILOSOFO %d SMETTE:\t\t",i);
    print();
    test((i+1)%NFILOSOFI);
    test((i-1)%NFILOSOFI);
    sem_post(mutex);
}

void* proc(void * iPtr)
{
    int j,n;
    int m;
    int i = *(int*)iPtr;

    m = 50000000 + 30000000 * i; /* tempo per cui usera' le forchette;
    i filosofi con i piu' alto sono piu' lenti a mangiare, questo differenzia
    un pochino il loro comportamento e su qualche sistema favorisce
    il verificarsi del deadlock */


    for (n=0;n<10;n++)
    {
        take_forks(i);

        for(j=0;j<m;j++); /* ogni processo usa le risorse per un tempo diverso */

        put_forks(i);

    }
}

int main()
{
    int i;
    struct sigaction sa;


    shm_fd1 = shm_open("/myshm1", O_CREAT|O_RDWR,0600);
    if (shm_fd1 == -1) perror("Creazione memoria condivisa: semafori");
    ftruncate(shm_fd1, NFILOSOFI*sizeof(sem_t));
    sem = mmap(0, NFILOSOFI*sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED, shm_fd1,0);
    if (sem == MAP_FAILED) perror("Creazione memoria condivisa: semafori");

    shm_fd2 = shm_open("/myshm2", O_CREAT|O_RDWR,0600);
    if (shm_fd2 == -1) perror("Creazione memoria condivisa: stato");
    ftruncate(shm_fd2, NFILOSOFI*sizeof(char));
    stato = mmap(0, NFILOSOFI*sizeof(char),PROT_READ|PROT_WRITE,MAP_SHARED, shm_fd2,0);
    if (stato == MAP_FAILED) perror("Creazione memoria condivisa: stato");

    shm_fd3 = shm_open("/myshm3", O_CREAT|O_RDWR,0600);
    if (shm_fd3 == -1) perror("Creazione memoria condivisa: mutex");
    ftruncate(shm_fd3, sizeof(sem_t));
    mutex = mmap(0, sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED, shm_fd3,0);
    if (mutex == MAP_FAILED) perror("Creazione memoria condivisa: mutex");

    sem_init(mutex,1,1);

    pthread_t threads[NFILOSOFI];
    int* n;

    for (i=0;i<NFILOSOFI;i++)
    {
        sem_init(&sem[i],1,0);
        stato[i]='T';
    }

    for(i=0;i<NFILOSOFI;i++) {
        n=malloc(sizeof(int));
        *n = i;

        if (pthread_create(&threads[i], NULL, &proc, n) != 0){
            perror("Errore creazione thread");
            exit(0);
        }
    }

    sa.sa_handler = cleanup;
    sigaction(SIGINT,&sa,NULL);
     /* se vanno in deadlock, bisogna interrompere tutto,
    il thread padre associa cleanup a SIGINT per rimuovere
    semafori e memoria condivisa in questo caso */

    for(i=0;i<NFILOSOFI;i++) {
        pthread_join(threads[i], NULL);
        printf("Terminato processo %d\n", i);
    }

    cleanup(0);
}
