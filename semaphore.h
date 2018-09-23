#ifndef __SEMAPHORE_H_SEMAPHORE__
#define __SEMAPHORE_H_SEMAPHORE__


#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>


int create_semaphore(int key);
int open_semaphore(int key);
int remove_semaphore(int id);
int init_semaphore(int id, int val);
int up(int id);
int down(int id);


#endif

