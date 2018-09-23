#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include "vol.h"
#include "fonctions.h" 

// variables globales
int id_BAL;

/*  fonction pour terminer proprement l'utilisateur dans laquelle on supprime la boite aux lettres */
void terminate(){
	printf("Utilisateur se termine proprement \n");
	msgctl(id_BAL, IPC_RMID, NULL);
	exit(0);
}

int main() {
	signal(SIGINT, terminate);
	signal(SIGUSR1, handleSIGUSR1);
	signal(SIGUSR2, handleSIGUSR2);
	
	// declaration de variable
	char dest_achat[21];
	int places_achat;
	int i, j;

	// on definit une structure pour nos messages
	struct msgbuf {
		long mtype;
		VOL vol;
	} message;
	
	// creation de la boite aux lettres de cle 300 et d'option 0660 (lecture et ecriture pour l'utilisateur et le groupe)
	int id_BAL = msgget(300, 0660);

	// le premier message qu'on envoie  est le pid a agence pour qu'elle puisse envoyer des signaux a utilisateur
	message.mtype = 1;
	message.vol.text[0] = '.';
	message.vol.places = getpid();
	msgsnd(id_BAL, &message, sizeof(VOL), 0); 

	while(1) {
		printf("Veuillez entrer la destination souhaitée :\n");
		scanf("%s", &dest_achat);
		printf("Veuillez entrer le nombre de places que vous voulez acheter :\n");
		scanf("%d", &places_achat);

		// on met ensuite à jour le message avec les valeurs entree par l'utilisateur en ajoutant les octets de bourrage puis on envoie
		for(i=0; i<strlen(dest_achat); i++){
			message.vol.text[i] = dest_achat[i];
		}
		for(j=strlen(dest_achat); j<20; j++){
			message.vol.text[j] = '.';
		}
		message.vol.text[20] = '\0';
		message.vol.places = places_achat;
		msgsnd(id_BAL, &message, sizeof(VOL), 0);
		
		pause(); // on attend la reponse de l'agence, pour savoir si l'achat etait possible ou non
	}
}
