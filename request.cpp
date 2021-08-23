#include "request.h"

int sendRequest(std::string req) {
	int sd = GetConnection();
	const char terminator[1] = {'\n'};
	const char *t_ptr = &terminator[0];

	if (sd < 0) {
		fprintf(stderr, "Failed to open connection\n");
	}

	write(sd, req.c_str(), strlen(req.c_str()));
	// Write the terminator to indicate to server that it is finished
	write(sd, t_ptr, 1);

	return sd;
}

void receiveRequest(int sockfd, std::string filename) { 
	//int sockfd = GetConnection();

	if (sockfd == -1) {
		fprintf(stderr, "Failed to open connection for receive\n");
	}
	size_t bufferlen = 256;
	size_t filelength = 0;
	char buffer[bufferlen];
	char *b_ptr = &buffer[0];
	std::stringstream stream;
	std::string temp;
	bool is_text = false;
	bool after_header = false;
	memset(b_ptr, 0, bufferlen);
	std::ofstream output;

	// Read the header first
	int n = read(sockfd, b_ptr, bufferlen);
	stream << b_ptr;
	std::getline(stream, temp, '\n');
	int header_value = 0;
	std::stringstream value(temp);
	value >> header_value;
	std::cout << temp << std::endl;
	std::getline(stream,temp,'\n');
			
	if (header_value == 100) {
		after_header = true;
	}

	std::cout << temp << std::endl;
	std::getline(stream, temp, '\n');
		
	if (temp.find("text") != std::string::npos) {
		is_text = true;
	}

	std::cout << temp << std::endl;

	std::getline(stream, temp, '\n');

	value.clear(); value.str("");

	value << temp;

	value >> filelength;

	std::cout << filelength << std::endl;

	// Prepare to write any remaining info before going back to reading
	int header_size = stream.tellg();
	b_ptr = buffer + header_size;
	
	if (!is_text) {
		output.open(filename, std::ofstream::binary);

		if (!output.is_open()) {
			fprintf(stderr, "Failed to open %s\n", 
				filename.c_str());
			close(sockfd);
			return;
		}
		output << b_ptr;
	} else {
		std::cout << b_ptr;
	}

	b_ptr = &buffer[0];

	memset(b_ptr, 0, bufferlen);

	while ((n = read(sockfd, b_ptr, bufferlen)) > 0) {
		if (is_text) {
			std::cout << b_ptr;
		} else {
			output << b_ptr;
		}

		memset(b_ptr, 0, bufferlen);
	}

	if (is_text) {
		std::cout << std::endl;
	} else {
		output.close();
	}

	close(sockfd);
}
