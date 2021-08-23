#ifndef	SERVER_H
#define	SERVER_H
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <ctime>
#include "scheduler.h"
#include "dispatch.h"
#include "commandLineCheck.h"
#include "scheduler_factory.h"
#include "sem.h"
#include "FileGet.h"

extern sem_t *t_sem;
extern sem_t *a_sem;

#define	SERVER_TCP_PORT	3000
#define	BUFLEN		256

/* ip address of server, as well as its port */
extern char *ipp;

void SetIPAddr(struct in_addr, unsigned short);

#endif

