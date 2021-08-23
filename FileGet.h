#ifndef	FILE_GET_H
#define	FILE_GET_H
#include <dirent.h>
#include <errno.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include "task.h"

extern struct global_task_info gti;

std::string list();

#endif
