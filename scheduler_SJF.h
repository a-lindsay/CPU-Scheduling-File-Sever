#ifndef	SCHEDULER_SJF_H
#define	SCHEDULER_SJF_H
#include <errno.h>
#include <cstdio>
#include <pthread.h>
#include <map>
#include "scheduler.h"
#include "task.h"


/* Scheduler SJF inherits from schedule */
class Scheduler_SJF : public Scheduler {
public:
	Scheduler_SJF();
	~Scheduler_SJF();
	virtual TaskInfo *Next_Task();
	virtual void Add_Task(TaskInfo *);
	size_t Task_Count();
	
/* Since we're doing SJF, we will use a multimap */
private:
	std::multimap<int, TaskInfo *> map;
  
};

#endif
