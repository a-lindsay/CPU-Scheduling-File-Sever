#ifndef	SCHEDULER_RANDOM_H
#define	SCHEDULER_RANDOM_H
#include <errno.h>
#include <cstdio>
#include <pthread.h>
#include <vector>
#include "scheduler.h"
#include "task.h"

/* Scheduler RANDOM inherits from schedule */
class Scheduler_RANDOM : public Scheduler {
public:
	Scheduler_RANDOM();
	~Scheduler_RANDOM();
	virtual TaskInfo *Next_Task();
	virtual void Add_Task(TaskInfo *);
	virtual size_t Task_Count();
	
/* With RANDOM, we will use a vector */
private:
	std::vector<TaskInfo *> v;
};

#endif
