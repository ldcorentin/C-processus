#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vol.h"
#include "fonctionsDeTest.h"

// test de la bonne recuperation des destinations depuis le fichier texte grace a un affichage du tableau a 2 dimensions
void test1(char destination[23][21])
{
	for(int i=0; i<23; i++)
	{
		for(int j=0; j<21; j++) printf("%c", destination[i][j]);
		printf("\n");
	}
}

//test de la bonne reception des messages de la boite aux lettres en les affichants
void test2(MESSAGE message)
{
	printf("Test de la bonne reception du message :\ntype : %ld\n", message.mtype);
	printf("nombre de places : %d\n", message.vol.places);
	printf("destination : ");
	for(int i=0; i<20; i++)  printf("%c", message.vol.text[i]);
	printf("\n");
}

