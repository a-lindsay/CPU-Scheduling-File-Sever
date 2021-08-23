#include "scheduler_factory.h"

Scheduler *MakeScheduler(const char *scheduler) {
	/* If any, default to FIFO */
	if (strncmp(scheduler, "ANY", 3) == 0) {
		return new Scheduler_FIFO();
	}

	/* If FIFO, return FIFO scheduler */
	if (strncmp(scheduler, "FIFO", 4) == 0) {
		return new Scheduler_FIFO();
	}

	/* We have not implemented other schedulers */
	if (strncmp(scheduler, "RANDOM", 6) == 0)  {
		return new Scheduler_RANDOM();
	}
	
	if (strncmp(scheduler, "SJF", 3) == 0) {
		return new Scheduler_SJF();
	} else {
		return nullptr;
	}
}