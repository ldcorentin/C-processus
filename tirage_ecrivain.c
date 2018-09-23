#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "shmem.h"
#include "semaphore.h"
#include "vol.h"
#include "fonctions.h"
#include "fonctionsDeTest.h"

//variables globales
int id_vols;
VOL* vols;
int mutex;
int sem_affichage;
int empty;

/* fonction pour que affichage se termine proprement dans laquelle on se détache de la memoire partagee, on supprime les semaphores et la memoire partagee  */
void terminate()
{
	printf("Tirage_ecrivain se termine proprement \n");
	detach_shmem(vols);
	remove_shmem(id_vols);
	remove_semaphore(mutex);
	remove_semaphore(empty);
	remove_semaphore(sem_affichage);
	exit(0);
}

int main()
{
	signal(SIGINT, terminate);

	//initialisation de plusieurs variables
	int j, k=1;
	VOL tirage_vol;

	// création tube ordinaire
	int descpipe[2];
	if(pipe(descpipe)==1)
	{
		printf("Erreur de création du tube\n");
		exit(0);
	}

	// clonage du processus pour separer tirage et ecrivain
	int pid = fork();
	if(pid != 0){
		// ------------------ processus tirage ----------------------- //

		// on recupere toutes les destinations possibles et on les stocke dans un tableau
		char liste_dest[23][21] = {0};
		lireDestinations(liste_dest, "Liste_destinations.txt");
		close(descpipe[0]); // je ne m'en sers pas en lecture
		while(1)
		{
			tirage_vol = genererVolRandom(liste_dest);
			write(descpipe[1],&tirage_vol,sizeof(VOL));
			sleep(4);			
		}

	}else{
		// ------------------ processus ecrivain ----------------------- //
		VOL vol_a_ecrire;
		int l;
		
		// creation et initialisation des semaphores
		mutex = create_semaphore(200);
		empty = create_semaphore(201);
		sem_affichage = create_semaphore(202);
		init_semaphore(mutex, 1);
		init_semaphore(empty, 20);
		init_semaphore(sem_affichage, 0);

		// creation, attachement memoire partagee
		id_vols = create_shmem(100, 20*sizeof(VOL));
		vols = (VOL*) shmat(id_vols, 0, 0); //
		
		// initialisation de la memoire partagee (le caractere . a la place de la premiere lettre de la destination correspond a un vol disponible)
		for(l=0; l<20; l++) {
			vols[l].text[0] = '.';
			vols[l].places = 0;
		}

		while(1) {
			// on recupère le vol envoyé par le processus tirage et on l'ecrit dans la memoire partagee
			read(descpipe[0], &vol_a_ecrire, sizeof(VOL));
			ecrireNouveauVol(vol_a_ecrire, vols, mutex, empty, sem_affichage);
		}
		
	}
}
