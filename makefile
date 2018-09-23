all : tirage_ecrivain affichage utilisateur agence mainTest clean

utilisateur : utilisateur.o fonctions.o semaphore.o shmem.o
	gcc -g utilisateur.o fonctions.o semaphore.o shmem.o -o utilisateur

agence : agence.o shmem.o semaphore.o fonctions.o fonctionsDeTest.o
	gcc -g agence.o shmem.o semaphore.o fonctions.o fonctionsDeTest.o -o agence

tirage_ecrivain : tirage_ecrivain.o fonctions.o shmem.o semaphore.o fonctionsDeTest.o
	gcc -g tirage_ecrivain.o -Wall fonctions.o shmem.o semaphore.o fonctionsDeTest.o -o tirage_ecrivain

affichage : affichage.o semaphore.o shmem.o fonctions.o
	gcc -g affichage.o semaphore.o shmem.o fonctions.o -o affichage

mainTest : mainTest.o fonctions.o fonctionsDeTest.o shmem.o semaphore.o
	gcc -g mainTest.o fonctions.o fonctionsDeTest.o semaphore.o shmem.o -o mainTest

mainTest.o : mainTest.c
	gcc -g -c mainTest.c -o mainTest.o

utilisateur.o : utilisateur.c
	gcc -g -c utilisateur.c -o utilisateur.o

affichage.o : affichage.c
	gcc -g -c affichage.c -o affichage.o

fonctions.o : fonctions.c
	gcc -g -c fonctions.c -o fonctions.o

tirage_ecrivain.o : tirage_ecrivain.c
	gcc -g -c tirage_ecrivain.c -o tirage_ecrivain.o

semaphore.o : semaphore.c
	gcc -g -c semaphore.c -o semaphore.o

shmem.o : shmem.c
	gcc -g -c shmem.c -o shmem.o

fonctionsDeTest.o : fonctionsDeTest.c
	gcc -g -c fonctionsDeTest.c -o fonctionsDeTest.o

clean : 
	rm -f *.o



