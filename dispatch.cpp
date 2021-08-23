#include "dispatch.h"
#include "logger.h"

extern Logger logger;

void *dispatch(void *d) {
	std::ostringstream oss;
	TaskInfo *curr_task = nullptr;
	struct timespec ts;
	memset(&ts, 0, sizeof(struct timespec));
	pthread_t thread;
	char time_buff[100] = {0};
	std::string t_string;

	// Wait until a task finishes and then move to next
	while (1) {
		if (sched->Task_Count() > 0) {
			memset(time_buff, 0, strlen(time_buff));
			// Decrement the semaphore
			sem_wait(t_sem);
			timespec_get(&ts, TIME_UTC);
			strftime(time_buff, sizeof(time_buff), "%D %T", 
				gmtime(&ts.tv_sec));
			t_string = time_buff;
			t_string.append(" ");
			oss << ts.tv_nsec;
			t_string.append(oss.str());
			t_string.append("\n");
			oss.clear();
			oss.str("");
			logger.LogWrite(std::string("Started:\n"));
			// Run the new task
			curr_task = sched->Next_Task();
			logger.LogWrite(curr_task->GetIPP());
			logger.LogWrite("\n");
			logger.LogWrite(t_string);
			// Now that we are running thread, allow new connection
			sem_post(a_sem);
			pthread_create(&thread, 
					NULL, 
					DoRequest, 
					(void *) curr_task);
		}
	}

	return nullptr;
}
