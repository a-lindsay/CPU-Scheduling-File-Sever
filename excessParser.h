#ifndef	EXCESS_PARSER_H
#define	EXCESS_PARSER_H
#include <sstream>
#include <string>

/* Parse and remove excess white space and comments */
class excessParser {
public:
	// Do nothing
	excessParser();
	// Do nothing
	~excessParser();
	void doStrip(std::string line);
	std::string getParsedString();
private:
	// Strip comments
	std::string stripComments(std::string);
	// Convert each tab to a single space
	std::string tabToSpace(std::string);
	// Strip leading and trailing whitespace
	std::string stripOuter(std::string);
	// Strip whitespace between words
	std::string stripInner(std::string);

	std::string result;
};

#endif
