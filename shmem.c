#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "shmem.h"

/* @fonction pour creer la memoire partagee, plus gestion d'erreur de sa creation */
int create_shmem(int key, int memsize)
{
	int variable = shmget(key, memsize, 0660 | IPC_CREAT);
	if (variable==-1)
	{
		printf("Erreur de création de la shmem\n");
		exit(0);
	}
	return variable;
}

/* @fonction pour ouvrir la memoire partagee, plus gestion d'erreur de son ouverture */
int open_shmem(int key, int memsize)
{
	int variable = shmget(key, memsize, 0660);
	if (variable==-1)
	{
		printf("Erreur d'ouverture de la shmem\n");
		exit(0);
	}
	return variable;
}

/* @fonction pour supprimer la memoire partagee, plus gestion d'erreur de sa supression */
int remove_shmem(int id) 
{
	int variable = shmctl(id, IPC_RMID, 0);
	if (variable==-1)
	{
		printf("Erreur de suppression de la shmem\n");
		exit(0);
	}
	return variable;
}

/* @fonction pour s'attacher à la memoire partagee, plus gestion d'erreur de son attachement */
void* attach_shmem(int id)
{
	return shmat(id, 0, 0) ;
}

/* @fonction pour se detacher de la memoire partagee, plus gestion d'erreur du desatachement */
int detach_shmem(const void* buffer)
{
	int variable = shmdt(buffer);
	if (variable==-1)
	{
		printf("Erreur de detachement de la shmem\n");
		exit(0);
	}
	return variable;
}
