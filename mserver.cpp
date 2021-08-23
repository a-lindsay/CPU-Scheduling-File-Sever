#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

// Run the server as a new image
void runServer(const char *port, const char *threads, const char *buffers, 
		const char *scheduler, const char *directory);

// Read the config file passed in 
void setConfigCmd(char *filename);

// Read mserver.config
void setConfigDefault();

void doConfigParse(const char *filename);

int main(int argc, char *argv[]) {
	// Main should not return for either of these
	if (argc == 3) {
		setConfigCmd(argv[2]);
	}

	if (argc == 1) {
		setConfigDefault();
	}

	else {
		fprintf(stderr, "Usage: ./mserver -c [config_file]\n");
	}

	return 1;
}

void setConfigCmd(char *filename) {
	std::ifstream config_file;
	config_file.open(filename, std::ios::in);
	
	if (!config_file.is_open()) {
		return setConfigDefault();
	}

	config_file.close();
	doConfigParse(filename);
}

void setConfigDefault() {
	std::ifstream config_file;
	config_file.open("mserver.config", std::ios::in);

	if (!config_file.is_open()) {
		return;
	}

	config_file.close();
	doConfigParse("mserver.config");
}

void doConfigParse(const char *filename) {
	// Try to open file
	std::ifstream config_file;
	config_file.open(filename, std::ios::in);
	std::string port;
	std::string threads;
	std::string buffers;
	std::string scheduler;
	std::string dir;

	if (!config_file.is_open()) {
		// If it doesn't work, return
		return;
	}

	std::string tmp;

	std::getline(config_file, tmp, '#');
	std::getline(config_file, tmp, '\n');

	std::getline(config_file, tmp, ':');
	std::getline(config_file, port);
	std::getline(config_file, tmp, ':');
	std::getline(config_file, threads);
	std::getline(config_file, tmp, ':');
	std::getline(config_file, buffers);
	std::getline(config_file, tmp, ':');
	std::getline(config_file, scheduler);
	std::getline(config_file, tmp, ':');
	std::getline(config_file, dir);

	runServer(port.c_str(), threads.c_str(), buffers.c_str(), 
		scheduler.c_str(), dir.c_str());
}
void runServer(const char *port, const char *threads, const char *buffers, const char *scheduler, const char *directory) {
	execl("server", "./server", port, threads, buffers, scheduler, directory, NULL);
}
