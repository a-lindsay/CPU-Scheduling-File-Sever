#include "excessParser.h"

excessParser::excessParser() : result("") { }

excessParser::~excessParser() { }

void excessParser::doStrip(std::string line) {
	std::string temp;

	// Parse string and set result equal to final output
	temp = stripComments(line);
	temp = tabToSpace(temp);
	temp = stripOuter(temp);
	temp = stripInner(temp);

	result = temp;
}

std::string excessParser::getParsedString() {
	return result;
}

std::string excessParser::stripComments(std::string line) {
	/* Determine whether or not there are any comments to strip.
	 * This means getting the index of the first # found. However,
	 * it must be determined that # is not part of a quote or string
	 * literal.
	 */

	bool found_comment = false;
	bool in_quote = false;
	bool in_literal = false;
	size_t i;

	for (i = 0; i < line.size(); ++i) {
		// If we have found ", but didn't find ' first
		if (line[i] == '\"' && !in_literal) {
			// Reverse whether we are in quote or not.
			// This allows us to track opening vs. closing "
			in_quote = !in_quote;
		}
	
		// Same for here.
		else if (line[i] == '\'' && !in_quote) {
			in_literal = !in_literal;
		}

		// We found a comment and are not inside a quote. Exit
		else if (line[i] == '#' && !in_literal && !in_quote) {
			found_comment = true;
			break;
		}
	}

	// Return the original line if we never found a comment 
	if (!found_comment) {
		return line;
	}

	std::string stripped;

	// strip the comment
	stripped = line.substr(0,i);

	return stripped;
}

std::string excessParser::tabToSpace(std::string line) {
	// Turn all tabs into spaces
	for (size_t i = 0; i < line.size(); ++i) {
		if (line[i] == '\t') {
			line.replace(i, 1, 1, ' ');
		}
	}

	return line;

}

std::string excessParser::stripOuter(std::string line) {
	// Remove leading whitespace
	
	while (line[0] == ' ') {
		line.erase(0,1);
	}

	// Remove trailing whitespace
	while (line[line.size() - 1] == ' ') {
		line.erase(line.size() - 1, 1);
	}

	return line;
}

std::string excessParser::stripInner(std::string line) {
	bool found_whitespace = false;

	for (size_t i = 0; i < line.size(); ++i) {
		/* If we found the first whitespace between words,
		 * then set the bool. We'll ignore the first one
		 */
		if (!found_whitespace && line[i] == ' ') {
			found_whitespace = true;
		}

		/* If we had already found the first whitespace between
		 * words, then we'll erase this whitespace.
		 */
		else if (found_whitespace && line[i] == ' ') {
			line.erase(i, 1);
		}

		/* If we find a non whitespace character, then we are
		 * inside of a word. Go ahead and set found_whitespace to
		 * false. Once we exit the word, we'll go back to stripping
		 * whitespace.
		 */
		else if (line[i] != ' ') {
			found_whitespace = false;
		}
	}

	return line;
}
