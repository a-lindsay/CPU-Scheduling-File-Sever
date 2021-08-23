#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdio>
#include "lineParser.h"
#include "excessParser.h"
#include "request.h"

void lineParser(std::string lineInput){
    excessParser ep;
    std::stringstream stream;
    
    if (lineInput.empty()) {
        return;
    }
    
    ep.doStrip(lineInput);
    lineInput = ep.getParsedString();
    
   int result = sendRequest(lineInput);

   if (result < 0) {
   	fprintf(stderr, "Failed to send request\n");
   	return;
   }

   if (lineInput.find("get") != std::string::npos) {
   	stream << lineInput;
	// Get the file that is to be extracted
	std::getline(stream, lineInput, ' ');
	lineInput = "";
	std::getline(stream, lineInput);
   	receiveRequest(result, lineInput);

	return;
   }

   receiveRequest(result, lineInput);
}
