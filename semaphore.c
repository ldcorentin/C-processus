#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"

/* @fonction pour creer le semphore, plus gestion d'erreur de sa creation */
int create_semaphore(int key)
{
	int variable;
	if( (variable = semget(key, 1, 0660 | IPC_CREAT)) ==-1)
	{
		printf("Erreur de création du semaphore\n");
		exit(0);
	}
	return variable;
}

/* @fonction pour ouvrir le semphore plus gestion d'erreur de son ouverture */
int open_semaphore(int key)
{
	int variable;
	if( (variable = semget(key, 1, 0660)) ==-1)
	{
		printf("Erreur d'ouverture du semaphore\n");
		exit(0);
	}
	return variable;
}

/* @fonction pour supprimer le semphore, plus gestion d'erreur de sa supression */
int remove_semaphore(int id) 
{
	int variable;
	if( (variable = semctl(id, 0, IPC_RMID)) ==-1)
	{
		printf("Erreur pour remove le semaphore\n");
		exit(0);
	}
	return variable;
}

/* @fonction pour initialiser le semphore, plus gestion d'erreur de son initialisation */
int init_semaphore(int id, int val)
{
	int variable;
	if( (variable = semctl(id, 0, SETVAL, val)) ==-1)
	{
		printf("Erreur d'initialisation du semaphore\n");
		exit(0);
	}
	return variable;
}

/* @fonction augmenter la valeur du semphore */
int up(int id)
{
	struct sembuf op ;
	op.sem_num = 0 ;
	op.sem_op = 1 ;
	op.sem_flg = 0 ;
	
	return semop(id, &op, 1) ;
}

/* @fonction diminuer la valeur du semphore */
int down(int id)
{
	struct sembuf op ;
	op.sem_num = 0 ;
	op.sem_op = -1 ;
	op.sem_flg = 0 ;
	
	return semop(id, &op, 1) ;
}
