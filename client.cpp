#include <string>
#include <vector>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include "getLine.h"
#include "lineParser.h"
#include "request.h"

std::string ipp;

void doInteractive() {
    ///interactive
    std::string interactiveCommand;
    std::cerr << ">>";
    while (std::getline(std::cin, interactiveCommand)) {

        if (interactiveCommand.compare("quit") == 0){
			break;
		}

        lineParser(interactiveCommand);
		std::cout << ">>";
    }
}

int GetConnectionDefault(int sd) {
	// Check clientrc file in order to determine if 
	// ip address and port are valid
	std::ifstream client(".clientrc", std::ifstream::in);

	if (!client.is_open()) {
		return -1;
	}

	std::string ip; std::string p;

	getline(client, ip, ':');
	getline(client, p);

	struct sockaddr_in server;
	memset((char *) &server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	unsigned short port = (unsigned short) atoi(p.c_str());
	server.sin_port = htons(port);
	int result = inet_aton(ip.c_str(), &server.sin_addr);

	if (!result) {
		return -1;
	}

	if (connect(sd, (struct sockaddr *) &server, sizeof(server)) < 0) {
		close(sd);
		return -1;
	}

	return sd;
}

int GetConnection() {
	std::stringstream ipp_stream;
	ipp_stream << ipp;
	std::string ip;
	std::string p;
	std::getline(ipp_stream, ip, ':');
	std::getline(ipp_stream, p);
	// Check if the provided port and ip address is valid
	struct sockaddr_in server;
	memset((char *) &server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	unsigned short port = (unsigned short) atoi(p.c_str());
	server.sin_port = htons(port);
	int result = inet_aton(ip.c_str(), &server.sin_addr);

	int sd = socket(AF_INET, SOCK_STREAM, 0);

	if (sd < -1) {
		fprintf(stderr, "%s\n", strerror(errno));
		return -1;
	}

	if (!result) {
		return GetConnectionDefault(sd);
	}

	if (connect(sd, (struct sockaddr *) &server, sizeof(server)) < 0) {
		return -1;
	}

	return sd;
}


int main(int argc, char *argv[]) {	
    if (argc != 2 && argc != 3) {
    	fprintf(stderr, "Usage: ./client [host:port] [script]\n");
		return 1;
    }

    ipp.append(argv[1]);
	
    if (argc == 2){
        doInteractive();
    } else {
        getLine(argv[2]);
    }

    return 0;
}

