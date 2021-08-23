#ifndef	DISPATCH_H
#define	DISPATCH_H
#include <pthread.h>
#include <string>
#include <sstream>
/* The dispatcher is what actually runs the scheduler */
#include "scheduler.h"
#include "sem.h"

extern sem_t *t_sem;
extern sem_t *a_sem;

extern Scheduler *sched;

/* The dispatcher takes the number of threads as a parameter.
 * Pass the address of threads and type cast to void *
 *
 * Nothing is returned
 */
void *dispatch(void *);

#endif
