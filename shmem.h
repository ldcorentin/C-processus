#ifndef __SHMEM_H_SHMEM__
#define __SHMEM_H_SHMEM__


#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>


int create_shmem(int key, int memsize);
int open_shmem(int key, int memsize);
int remove_shmem(int id);
void* attach_shmem(int id);
int detach_shmem(const void* buffer);


#endif

