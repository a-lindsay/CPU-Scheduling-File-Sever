#include "scheduler_FIFO.h"
#include "server.h"

Scheduler_FIFO::Scheduler_FIFO() : Scheduler() { }

/* Empty the queue if there is anything remaining */
Scheduler_FIFO::~Scheduler_FIFO() {
	TaskInfo *t = nullptr;
	while (!this->q.empty()) {
		t = this->q.front();
		this->q.pop();
		delete t;
	}
}

size_t Scheduler_FIFO::Task_Count() {
	return this->q.size();
}

TaskInfo *Scheduler_FIFO::Next_Task() {

	TaskInfo *t = this->q.front();
	this->q.pop();
	return t;
}

void Scheduler_FIFO::Add_Task(TaskInfo *t) {
	this->q.push(t);
}
