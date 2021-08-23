#ifndef	SCHEDULER_FIFO_H
#define	SCHEDULER_FIFO_H
#include <errno.h>
#include <cstdio>
#include <pthread.h>
#include <queue>
#include "scheduler.h"
#include "task.h"

/* Scheduler FIFO inherits from schedule */
class Scheduler_FIFO : public Scheduler {
public:
	Scheduler_FIFO();
	~Scheduler_FIFO();
	virtual TaskInfo *Next_Task();
	virtual void Add_Task(TaskInfo *);
	virtual size_t Task_Count();
/* Since we're doing FIFO, we will use a queue */
private:
	std::queue<TaskInfo *> q;
};

#endif
