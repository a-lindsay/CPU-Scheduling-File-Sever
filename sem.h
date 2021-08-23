#ifndef	SEM_H
#define	SEM_H
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <string.h>
#include <cstdio>
#include <errno.h>

// Names of the semaphores
extern sem_t *a_sem;
extern sem_t *t_sem;

bool initSemaphores(unsigned int threads,
			unsigned int buffer);

#endif
