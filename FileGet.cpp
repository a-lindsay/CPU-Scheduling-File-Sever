#include "FileGet.h"

std::string list() {
	DIR *dir = opendir(gti.path);
	struct dirent* d;
	std::string files = "";
	if (dir) {
		while ((d = readdir(dir)) != NULL) {
			files.append(d->d_name);
			files.append(" ");
		}	 
		closedir(dir);
	}
	else {
		/*Directionary does not exist*/
		fprintf(stderr, "list: %s\n", strerror(errno));
	}
	
	files[files.length()-1] = '\n';
	return files;
}
