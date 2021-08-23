#include "scheduler_RANDOM.h"
#include "server.h"

Scheduler_RANDOM::Scheduler_RANDOM() : Scheduler() { 

    srand(time(NULL));

}

Scheduler_RANDOM::~Scheduler_RANDOM() { }



TaskInfo *Scheduler_RANDOM::Next_Task() {

	int n = this->v.size();
    int randNum = rand() % n;

    TaskInfo *t = this->v[randNum];
    this->v.erase(this->v.begin()+randNum);
    return t;
}

size_t Scheduler_RANDOM::Task_Count() {
	return this->v.size();
}

void Scheduler_RANDOM::Add_Task(TaskInfo *t) {
	this->v.push_back(t);
}
