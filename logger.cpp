#include "logger.h"

Logger logger;

Logger::Logger() {}

//checks if ofstream is open, closes if so
Logger::~Logger() {


    if (outfile.is_open()) {
        outfile.close();
    }


}

//makes sure the file can be open and returns true if it does, false if not
bool Logger::LogInit(std::string filename) {
    outfile.open(filename, std::ios_base::app);
    if (outfile.is_open()) {
        return true;
    }

    return false;

}

//writes contents to file
void Logger::LogWrite(std::string towrite) {
    outfile<<towrite;
    outfile.flush();

}
