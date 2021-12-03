/*
	Scrivere un programma in C in cui un processo padre P0 genera 10 processi figli.
	P0 dopo aver generato generato i figli invia ciclicamente per 10 volte un segnale SIGUSR1 ad ogni figlio.

	Ogni figlio si mette in attesa di un segnale, quando riceve il segnale SIGUSR1, stampa a video il proprio PID e
	si rimette in attesa di un segnale.
	
	Quando riceve SIGUSR2 termina.

	P0 dopo aver concluso il ciclo, invia a tutti i figli il segnale USR2, attende la terminazione di tutti i figli e
	termina esso stesso.
*/

//non va

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>

#define N 5

void fSIGUSR1(int sig){
    printf("Sono il figlio con PID %d, ho ricevuto SIGUSR1\n",getpid());
    //sleep(1);
    //dopo la chiamata a funzione SIGUSR1, si mette di nuovo in pausa per SIGUSR2
    pause();

}

void fSIGUSR2(int sig){
    printf("Sono il figlio con PID %d, ho ricevuto SIGUSR2 - sto terminando\n",getpid());
    exit(EXIT_SUCCESS);
}

int main(){
    int i,status; //status per wait
    pid_t pid[N];
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    act.sa_handler=fSIGUSR1;
    sigaction(SIGUSR1,&act,0);

    act.sa_handler=fSIGUSR2;
    sigaction(SIGUSR2,&act,0);

    for(int i=0;i<N;i++){
        pid[i]=fork();
        switch(pid[i]){
            case -1:
                perror("Fork failed\n");
                break;
            case 0:
                //child
                sleep(1);
                printf("Sono il figlio %d, sono stato creato. Attendo un segnale\n",i);
                pause();
                //si sblocca quando il padre manda SIGUSR1
                
                //exit avviene in f SIGUSR2
            default: 
                
                break; 
        }
    }
    
    //padre manda SIGUSR1 ad ogni figlio fatto, finito il ciclo manda SIGUSR2
    for(i=0;i<N;i++){
        kill(pid[i],SIGUSR1);
    }
    //padre, i child sono in attesa con pause
    for(i=0;i<N;i++){
        kill(pid[i],SIGUSR2);
    }

    //il padre aspetta tutti i figli
    waitpid(0,&status,0); //con 0 li aspetta tutti, con pid al primo argomento aspettava solamente il primo a finire
    printf("Tutti sono morti. Vado anche io\n");
    exit(EXIT_SUCCESS);
}
