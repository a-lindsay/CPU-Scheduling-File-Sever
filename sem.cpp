#include "sem.h"

sem_t a;
sem_t t;

sem_t *a_sem = &a;
sem_t *t_sem = &t;


bool initSemaphores(unsigned int threads, unsigned int buffers) {
	int result = sem_init(a_sem, 0, buffers);

	if (result < 0) {
		fprintf(stderr, "Failed to initialize accepted ");
		fprintf(stderr, "Connections semaphore!\n");
		return false;
	}

	result = sem_init(t_sem, 0, threads);

	if (result < 0) {
		fprintf(stderr, "Failed to initialize threads semaphore!\n");
		sem_destroy(a_sem);
		return false;
	}

	return true;
}

