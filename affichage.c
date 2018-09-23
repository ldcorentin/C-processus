#include <stdio.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include "semaphore.h"
#include "shmem.h"
#include "vol.h"
#include "fonctions.h"

//variables globales
int id_vols;
VOL* vols;
int mutex;
int empty;
int sem_affichage;
int i;

/* fonction pour que affichage se termine proprement dans laquelle on se détache de la memoire partagee, on supprime les semaphores et la memoire partagee  */
void terminate(){
	printf("Affichage se termine proprement \n");
	detach_shmem(vols);
	remove_shmem(id_vols);
	remove_semaphore(mutex);
	remove_semaphore(empty);
	remove_semaphore(sem_affichage);
	exit(0);
}

int main(){
	signal(SIGINT, terminate);
	
	//ouverture des sémaphores
	mutex = open_semaphore(200);
	empty = open_semaphore(201);
	sem_affichage = open_semaphore(202);
	
	//ouverture, attachement mémoire partagée
	id_vols = open_shmem(100, 20*sizeof(VOL));
	vols = (VOL*) shmat(id_vols, 0, 0);

	while(1){
		system("clear"); // permet un affichage plus pratique en evitant de défiler vers le bas sans cesse
		
		printf(" ___ ___ ___ _  ___   _____ _  _ _   _ ___    ___ _  _ ___ ____    _   ___ ___   __  __   _   ___ ___ ___   ___ _____   _    ___ ___  _____   _____ ___  ___ ___ \n");
		printf("| _ )_ _| __| \\| \\ \\ / / __| \\| | | | | __|  / __| || | __|_  /   /_\\ |_ _| _ \\ |  \\/  | /_\\ | _ \\_ _| __| | __|_   _| | |  | __|   \\| __\\ \\ / / __|   \\| __/ __|\n");
		printf("| _ \\| || _|| .` |\\ V /| _|| .` | |_| | _|  | (__| __ | _| / /   / _ \\ | ||   / | |\\/| |/ _ \\|   /| || _|  | _|  | |   | |__| _|| |) | _| \\ V /| _|| |) | _| (__ \n");
		printf("|___/___|___|_|\\_| \\_/ |___|_|\\_|\\___/|___|  \\___|_||_|___/___| /_/ \\_\\___|_|_\\ |_|  |_/_/ \\_\\_|_\\___|___| |___| |_|   |____|___|___/|___| \\_/ |___|___/|___\\___|\n");
		printf("					     _____\n");
		printf("					     \\ U \\__      _____\n");
		printf("					      \\   \\/_______\\___\\_____________\n");
		printf("					      < /_/   .....................  `-.\n");
		printf("					        `-----------,----,--------------'\n");
		printf(" ____________________________________________________/____/\n");
		
		down(mutex);
		/* on supprime les vols de la mémoire partagé si le nombres de places d'un vol est nul ou affiche un vol si la première case est différente d'un '.'
		(comme nous avons initialisé notre mémoire partagé avec des '.' comme premier caractère) */
		for(i=0;i<20;i++){
			if((vols[i].places == 0) && (vols[i].text[0] != '.')){ // on supprime le vol
				vols[i].text[0] = '.';
				up(empty);
			}
			if(vols[i].text[0] != '.'){ // on affiche le vol
				afficherVol(vols[i]);
			}
		}
		printf("|__________________________________|\n");
		up(mutex);
		down(sem_affichage);
	}
}
