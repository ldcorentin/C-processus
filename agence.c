#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
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
int empty;
int sem_affichage;
int id_BAL;

/* @fonction pour que affichage se termine proprement dans laquelle on se détache de la memoire partagee, on supprime les semaphores et la memoire partagee  */
void terminate(){
	printf("Agence se termine proprement \n");
	msgctl(id_BAL, IPC_RMID, NULL);
	detach_shmem(vols);
	remove_shmem(id_vols);
	remove_semaphore(mutex);
	remove_semaphore(empty);
	remove_semaphore(sem_affichage);
	if(id_BAL != 0) printf("la boite n'a pas été correctement supprimé");
	exit(0);
}

int main()
{
	signal(SIGINT, terminate);

	//declaration de variables
	int recu, i, pidUtilisateur, taille, places_achat, k, vol, achat;
	char dest_achat[21];
	
	//initialisation de variables
	taille = sizeof(VOL);
	k = 0;
	achat = 0;	
	
	//on ouvre nos semaphores
	mutex = open_semaphore(200);
	empty = open_semaphore(201);
	sem_affichage = open_semaphore(202);
	
	//on récupère l'id de notre memoire partagé pour s'y attacher ensuite
	id_vols = open_shmem(100, 20*sizeof(VOL));
	vols = (VOL*) shmat(id_vols, 0, 0);
	
	//creation de la structure que l'on va utiliser pour nos messages
	MESSAGE message;
	
	//le premier message qu'on reçoit contient le pid dans la variable int vol.places
	//creation de la boite aux lettres
	id_BAL = msgget(300, 0660 | IPC_CREAT);
	if(id_BAL==-1)
	{
		printf ("Erreur de creation de la boite aux lettres");
		exit(0);
	}else{
		recu=msgrcv(id_BAL, &message, taille, 1, 0);
		
		if(recu==-1){
			printf("erreur de reception\n");
		}else{
			//récupération du pid utilisateur
			if(message.vol.text[0]=='.')
			{
				pidUtilisateur = message.vol.places;
			}
		}
	}
	
	while(1)
	{
		recu = msgrcv(id_BAL, &message, taille, 1, 0);
		achat = 0;
		// --------- Test de la reception du message --------- 
		//test2(message);
		
		if(recu==-1)
		{
			printf("erreur de reception\n");
			exit(0);
		}else{
			down(mutex);
			k = 0;
			while((k<20) && (achat==0))
			{
				// on cherche dans la memoire partagée la destination et on enleve le nombre de places voulu si il y en a assez de disponibles
				// on repond par signaux : SIGUSR1 si il y a assez de places disponibles, SIGUSR2 sinon
				if(strcmp(vols[k].text, message.vol.text)==0)
				{
					if(message.vol.places<=vols[k].places)
					{
						vols[k].places = vols[k].places - message.vol.places;
						achat = 1;
						up(sem_affichage);
						kill(pidUtilisateur, SIGUSR1);
					}else{
						achat = 1;
						kill(pidUtilisateur, SIGUSR2);
					}
				}
				// si on a parcouru toute la memoire c'est que le vol entré n'était pas dans la liste, donc on envoie SIGUSR2 a utilisateur
				if(k==19)
				{
					printf("%s n'est pas dans la liste des destinations\n", message.vol.text);
					kill(pidUtilisateur, SIGUSR2);
				}
				k++;
			}
			up(mutex);
		}			
	}				
}
