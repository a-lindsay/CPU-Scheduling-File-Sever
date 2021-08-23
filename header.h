#ifndef	HEADER_H
#define	HEADER_H
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <errno.h>
#include "task.h"

class Header {
public:
	Header(int resp, const char *ipp, const char 
			*type, size_t length, const char *request);
	~Header();
	int GetResponse();
	std::string GetTotalString();
	char *GetType();
	char *GetRequest();
	size_t GetLength();
private:
	int resp;
	char *ipp;
	char *type;
	size_t length;
	char *request;
};


/* When the task is created, it calls make header.
 * Make header determines what kind of request is being made.
 * MakeTask will use GetResponse in order to determine what
 * kind of task is being performed
 */
Header *MakeHeader(const char *request);

#endif
