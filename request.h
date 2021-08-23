#ifndef	REQUEST_H
#define	REQUEST_H
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

extern int GetConnection();

int sendRequest(std::string req);

void receiveRequest(int sockfd, std::string filename);

#endif
