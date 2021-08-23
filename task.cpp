#include "task.h"
#include "server.h"
#include "sem.h"
#include "logger.h"
#include <cstdio>

struct global_task_info gti;

extern Logger logger;

TaskInfo::TaskInfo(int sockfd, const char *request, std::string client_ipp)
	: sock_fd(sockfd) {
	
	this->h = MakeHeader(request);

	this->ipp = client_ipp;

	/* Since bad requests only involve sending a header back,
	 * We're going to give it the highest priority.
	 * The reason for this is that sending a bad request
	 * header back to the client will take shorter than
	 * sending a request header along with some file
	 * or listing of the media directory
	 */
	if (this->h->GetResponse() != 100) {
		this->priority = 0;
	}
		
	/* The priority is based on the size
	 */

	else {
		this->priority = this->h->GetLength();
	}
		
}

TaskInfo::~TaskInfo() {
	delete h; h = nullptr;
	close(this->sock_fd);
}

Header *TaskInfo::GetHeader() {
	return h;
}

std::string TaskInfo::GetIPP() {
	return this->ipp;
}

size_t TaskInfo::GetPriority() {
	return this->priority;
}

int TaskInfo::GetSockFD() {
	return this->sock_fd;
}

void TaskInfo::SetThread(pthread_t *thread) {
	this->thread = thread;
}

TaskInfo *MakeTask(int fd, const char *request, std::string client_ipp) {
	TaskInfo *t = new TaskInfo(fd, request, client_ipp);

	if (t->GetHeader()->GetResponse() == 0) {
		delete t; t = nullptr;
	}

	return t;
}

void *DoRequest(void * t) {
	struct timespec ts;
	std::ostringstream oss;
	memset(&ts, 0, sizeof(struct timespec));
	char time_buff[100] = {0};
	std::string t_string;
	// Get the header and the file descriptor
	TaskInfo *task = (TaskInfo *) t;
	Header *h = task->GetHeader();
	int fd = task->GetSockFD();
	// Cal writerequest
	WriteRequest(fd, h);
	timespec_get(&ts, TIME_UTC);
	strftime(time_buff, sizeof(time_buff), "%D %T",
		gmtime(&ts.tv_sec));
	t_string = time_buff;
	t_string.append(" ");
	oss << ts.tv_nsec;
	t_string.append(oss.str());
	t_string.append("\n");
	logger.LogWrite(std::string("Completed:\n"));
	logger.LogWrite(task->GetIPP());
	logger.LogWrite("\n");
	logger.LogWrite(t_string);
	// Delete the task now that we're finished
	delete task;
	// Release semaphore
	sem_post(t_sem);
	return nullptr;
}

void WriteRequest(int fd, Header *h) {
	/* Go ahead and initialize the kinds of strings
	 * that may be used in the function
	 */
	int char_read = 0;
	std::string header = h->GetTotalString(); 
	std::string request = h->GetRequest();
	int n = 0;
	char buffer[250] = {0};
	char *b_ptr = &buffer[0];
	size_t remaining = 0;
	size_t written = 0;

	write(fd, header.c_str(), strlen(header.c_str()));

	/* Exit if the request was not valid */
	if (h->GetResponse() != 100) {
		return;
	}

	/* Print list of directories if the request is list */
	if (strncmp(request.c_str(), "list", strlen(request.c_str())) == 0) {
		write(fd, gti.listing.c_str(), gti.listing.size()+1);
		return;
	}

	
	// Open the file that is to be sent over the network
	int readfd = open(request.c_str(), O_RDONLY);

	if (readfd < 0) {
		fprintf(stderr, "Failed to open %s\n",
				request.c_str(), strerror(errno));
		return;
	}

	while ((n = read(readfd, b_ptr, 250)) > 0) {
		remaining = n;

		while (remaining > 0) {
			written = write(fd,b_ptr, remaining);

			if (written < 0) {
				fprintf(stderr, "Socket send error: %s\n",
					strerror(errno));
				close(readfd);
				return;
			}

			remaining -= written;
			b_ptr += written;
		}

		remaining = 0;
		b_ptr = &buffer[0];
		memset(b_ptr, 0, 250);
	}

	close(readfd);
}

