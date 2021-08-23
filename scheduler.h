#ifndef	SCHEDULER_H
#define	SCHEDULER_H
#include <cstddef>
#include <vector>
#include <pthread.h>
#include "task.h"

/* The server will work with an abstract scheduler */
class Scheduler {
public:
	Scheduler();
	virtual ~Scheduler();
	virtual TaskInfo *Next_Task() = 0;
	virtual void Add_Task(TaskInfo *) = 0;
	virtual size_t Task_Count() = 0;
};

#endif
