#ifndef	TASK_H
#define	TASK_H
#include <dirent.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstdio>
#include <errno.h>
#include <cstring>
#include <sys/sendfile.h>
#include "header.h"
#include "sem.h"

extern sem_t *t_sem;
extern sem_t *a_sem;

/* This is information that is shared by all tasks. 
 * This includes directory information */
struct global_task_info {
	/* This is the path to the media directory.
	 * The reason it is a char * instead of a string
	 * is that having it as a char * allows it to be 
	 * set directly from argv
	 */
	char *path;
	/* A listing of contents in the media directory
	 * The professor did not mention the media directory
	 * ever changing. Hence, we can simplify things by
	 * setting the listing at the beginning.
	 */
	std::string listing;
};

class TaskInfo {
public:
	// The constructor will set priority and header
	TaskInfo(int sock_fd, const char *request, std::string);
	~TaskInfo();
	Header *GetHeader();
	size_t GetPriority();
	int GetSockFD();
	void SetThread(pthread_t *thread);
	std::string GetIPP();
private:
	std::string ipp;
	int sock_fd;
	Header *h;
	size_t priority;
	pthread_t *thread;
};

// The server will set gti
extern struct global_task_info gti;

TaskInfo *MakeTask(int fd, const char *request, std::string client_ipp);

/* DoRequest takes the provided task info object as a parameter.
 * It calls write request, providing the file descriptor and the header.
 *
 * Once the request is complete, DoRequest deletes the task info object
 */
void *DoRequest(void *);

/* write request writes the header to the fd, and then it writes either
 * the file itself or the listing (or nothing if it's a bad request
 */
void WriteRequest(int fd, Header *h);

#endif
