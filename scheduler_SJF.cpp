#include "scheduler_SJF.h"
#include "server.h"

Scheduler_SJF::Scheduler_SJF() {}

Scheduler_SJF::~Scheduler_SJF() { }



TaskInfo *Scheduler_SJF::Next_Task() {
	//set ite to first pair in map
	auto ite = map.begin();

	//set t to second element in pair
    	TaskInfo *t = ite->second;

	//erase the pair from the map
    	map.erase(ite);

    return t;


}

size_t Scheduler_SJF::Task_Count() {
	return map.size();
}

void Scheduler_SJF::Add_Task(TaskInfo *t) {
	
    map.insert(std::pair<int, TaskInfo *>(t->GetHeader()->GetLength(),t));
}
