#include "vol.h"
#include "semaphore.h"
#include "shmem.h"
#include "string.h"
#include <signal.h>
#include <stdio.h>
#include <time.h>

/* fonction pour ecrire un vol donné dans la memoire partagee
   parametres : vol a ecrire, pointeur vers la memoire partagee, entiers designant les 3 semaphores
*/
void ecrireNouveauVol(VOL vol_a_ecrire, VOL* vols, int mutex, int empty, int sem_affichage){
	int k = 0;
	int vol_ecrit = 0; // vol_ecrit sert de booleen pour savoir si on a ecrit dans la memoire, si oui on arretera de la parcourir

	// on parcourt toutes les cases de la memoire tant qu'on a pas ecrit vol_a_ecrire dedans
	down(mutex);
	while((k<20) && (vol_ecrit==0)){
		if(strcmp(vols[k].text, vol_a_ecrire.text)==0) // si cette destination est deja dans la shmem on ajoute juste les places
		{
			vols[k].places = vols[k].places + vol_a_ecrire.places;
			vol_ecrit = 1;
			up(sem_affichage);
		}
		k++;
	}
	if(vol_ecrit==0){ // si on a toujours pas ecrit, cela veut dire que la destination voulue n'est pas dans la memoire, il faut la rajouter
		k=0;
		while((k<20) && (vol_ecrit==0)){
			if(vols[k].text[0] == '.') // on l'écrit dans un emplacement vide
			{
				vols[k] = vol_a_ecrire;
				vol_ecrit = 1;
				up(sem_affichage); // declenche l'affichage
				down(empty); // decremente le nombre de places libres
			}
			k++;
		}
	}
	up(mutex);	
}

/* fonction qui lit les destinations dans un fichier texte et les stocke dans un tableau
   parametres : char[23][21] tableau 2D de caractères vide a remplir par la fonction, char* nom du fichier
*/
void lireDestinations(char liste_dest[23][21], char* nomFichier){
	FILE *fichierDestination; 
	fichierDestination=fopen("Liste_destinations.txt","r");
	char destination[21];
	int l, m, i = 0;

	while(fgets(destination,20,fichierDestination) != NULL)
	{
		for(l=0; l<strlen(destination); l++)
		{
			liste_dest[i][l] = destination[l];
		}
		for(m=strlen(destination)-1; m<21; m++){ // on ajoute les octets de bourrage. Le -1 permet d'ecraser le \n a la fin du mot
			 liste_dest[i][m]='.';
		}
		liste_dest[i][20] = '\0';
		i++;
	}
	fclose(fichierDestination);
} 

/* fonction qui cree un vol aleatoire
   parametre : tableau 2D contenant toutes les destinations possibles
   retourne le vol aléatoire
*/
VOL genererVolRandom(char liste_dest[23][21]){
	srand(time(NULL));
	VOL tirage_vol;
	int ligne=rand()%20;	
	int nbrPlaces=rand()%10;
	strcpy(tirage_vol.text, liste_dest[ligne]);
	tirage_vol.places=nbrPlaces;
	return tirage_vol;
}


/* @fonction pour afficher un vol
   @param VOL vol : parametre */
void afficherVol(VOL vol)
{
	if(vol.places>9) printf("| %s : %d places |\n", vol.text, vol.places);
	else printf("| %s :  %d places |\n", vol.text, vol.places);
}

//fonction pour afficher un message a la reception du signal SIGUSR1 et se reproteger
void handleSIGUSR1()
{
	printf("---------------\nAchat réussi ! \n---------------\n");
	signal(SIGUSR1, handleSIGUSR1);
}

//fonction pour afficher un message a la reception du signal SIGUSR2 et se reproteger
void handleSIGUSR2()
{
	printf("---------------\nAchat impossible...\n---------------\n");
	signal(SIGUSR2, handleSIGUSR2);
}


