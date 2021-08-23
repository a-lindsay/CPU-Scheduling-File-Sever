#ifndef	LOGGER_H
#define	LOOGER_H
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

class Logger {
public:
	// The logger constructor will be empty
	Logger();
	/* The logger destructor will check to see if the
	 * file stream is open. If so, it will close
	 */
	~Logger();
	/* LogInit will open the given filename in append mode
	 * It will return true if the file was successfully opened,
	 * and false if it did not
	 *
	 * The reason why this is done in log init is because opening a
	 * file may fail. You don't want to include stuff that can fail
	 * in the constructor
	 */
	bool LogInit(std::string filename);
	// This will simply write the string to the file
	void LogWrite(std::string towrite);
private:
	std::ofstream outfile;
};

#endif
