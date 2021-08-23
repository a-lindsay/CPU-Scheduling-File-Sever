#include "getLine.h"



void getLine(std::string commandFile) {
    std::ifstream inFile;
    std::string commands;
    inFile.open(commandFile);
	
    while(getline(inFile, commands)) {
        if(commandCheck(commands)) {

            lineParser(commands);
        }
    }
    inFile.close();
}

bool commandCheck(std::string commands) 
    for(size_t i = 0; i < commands.length(); i++) {
        if(!std::isspace(commands[i])) {
            if(commands[i] == '#') {
                return false;
            }
            
            return true;
        }
    }
    return false;
}

