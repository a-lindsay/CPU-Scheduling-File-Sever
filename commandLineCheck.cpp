#include "commandLineCheck.h"

/* Dan: I made a couple of changes to this.
 * Basically, it closes dir if the directory successfully opens
 * and then it returns true.
 *
 * In addition, directoryCheck takes a string as a parameter
 */
bool directoryCheck(char *path)
{
    DIR* dir = opendir(path);
    if (dir) {
        closedir(dir);
	    return true;
    }
    return false;
    
}











