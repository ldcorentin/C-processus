#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vol.h"
#include "fonctionsDeTest.h"
#include "fonctions.h"
#include "semaphore.h"
#include "shmem.h"
#include <signal.h>
#include <unistd.h>

int main(){
	printf("\n ----------- Test de la fonction lireDestinations avec le fichier Liste_destinations.txt -----------\n\n");
	char liste_dest[23][21] = {0};
	lireDestinations(liste_dest, "Liste_destinations.txt");
	printf("Destinations lues. Affichage de la liste des destinations à l'aide de la fonction test1 :\n\n");
	test1(liste_dest);
	
	printf("\n ----------- Test de la fonction genererVolRandom et de la fonction afficherVol -----------\n\n");
	VOL tirage_vol;
	tirage_vol = genererVolRandom(liste_dest);
	printf("Vol généré. Affichage de celui-ci à l'aide de la fonction afficherVol :\n\n");
	afficherVol(tirage_vol);
	printf("\n");

	printf("\n ----------- Test de la fonction handleSIGUSR1 ----------- \n\n");
	signal(SIGUSR1, handleSIGUSR1);
	printf("Envoi d'un signal SIGUSR1.\n");
	kill(getpid(), SIGUSR1);

	printf("\n ----------- Test de la fonction handleSIGUSR2 ----------- \n\n");
	signal(SIGUSR2, handleSIGUSR2);
	printf("Envoi d'un signal SIGUSR2.\n");
	kill(getpid(), SIGUSR2);
}
	
	
