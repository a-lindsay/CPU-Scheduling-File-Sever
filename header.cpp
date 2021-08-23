#include "server.h"
#include "header.h"
#include "task.h"

Header::Header(int resp, const char *ipp, 
			const char *type, size_t length, 
			const char *request)
	: resp(resp), length(length) {

	if (request == NULL) {
		this->ipp = nullptr;
		this->type = nullptr;
		this->request = nullptr;
		this->length = 0;
		this->resp = 0;
		return;

	}

	// Get the size of ipp and type
	size_t ipp_size = strlen(ipp);
	size_t type_size = strlen(type);
	size_t req_size = strlen(request);

	// Allocate on the heap
	this->ipp = new char [ipp_size+1];
	this->type = new char [type_size+1];
	this->request = new char[req_size+1];
	// Add null terminators to the end
	this->ipp[ipp_size] = 0;
	this->type[ipp_size] = 0;
	this->request[req_size] = 0;

	// Copy 
	this->ipp = strcpy(this->ipp, ipp);
	this->type = strcpy(this->type, type);
	this->request = strcpy(this->request, request);
}

Header::~Header() {
	if (this->ipp != nullptr)
		delete [] this->ipp; this->ipp = nullptr;
	if (this->type != nullptr)
		delete [] this->type; this->type = nullptr;
	if (this->request != nullptr)
		delete [] this->request; this->request = nullptr;
}

int Header::GetResponse() {
	return this->resp;
}

char *Header::GetRequest() {
	return this->request;
}

/* Create a formatted string out the header.
 * This will be given to WriteRequest
 */
std::string Header::GetTotalString() {
	// Convert response and length to strings
	char response_buff[5] = {0};
	char *response_ptr = &response_buff[0];
	int length = snprintf(NULL, 0, "%lu", this->length);
	char length_buff[length+1];
	char *length_ptr = &length_buff[0];
	// Clear the array
	memset(length_ptr, 0, length+1);
	snprintf(response_ptr, 4, "%d", this->resp);
	snprintf(length_ptr, length+1, "%lu", this->length);

	std::string retval(response_ptr);
	retval.append("\n");
	retval.append(ipp);
	retval.append("\n");
	retval.append(type);
	retval.append("\n");
	retval.append(length_ptr);
	retval.append("\n");
	return retval;
}

char *Header::GetType() {
	return this->type;
}

size_t Header::GetLength() {
	return this->length;
}

Header *MakeHeader(const char *req) {
	char *request = strdup(req);
	// Set up header
	Header *h = nullptr;
	// Fill header if list
	if (strncmp(request, "list", 4) == 0) {
		h = new Header(100, 
				ipp, 
				"text", 
				gti.listing.size()+1, 
				"list");

		free(request);
		// Return the header
		return h;
	}

	/* Fill header if get
	 * The request field will have the name of the request
	 * and will not include "get"
	 */
	else if (strncmp(request, "get", 3) == 0) {
		// Get the size of request
		size_t request_size = strlen(request);
		struct stat st;
		// Remove trailing whitespace
		for (size_t i = request_size - 1; i != 0; --i) {
			if (request[i] == '\t' || 
				request[i] == ' ' ||
				request[i] == '\n' ||
				request[i] == '\0') {
				request[i] = 0;
			}

			else {
				break;
			}
		}

		// Convert tabs to spaces
		for (size_t i = 0; i < request_size; ++i) {
			if (request[i] == '\t') {
				request[i] = ' ';
			}
		}

		std::stringstream stream;
		stream << request;
		std::string filename;
		std::getline(stream, filename, ' ');
		std::getline(stream, filename);
		std::string what(gti.path);
		what.append("/");
		what.append(filename);
		// Determine the size of the file
		stat(what.c_str(), &st);

		// Make sure that the file exists
		if (errno != ENOENT) {
			// Fill the necessary information
			h = new Header(100,
					ipp,
					"binary",
					st.st_size,
					what.c_str());

			free(request);
			// Return the new header
			return h;
		}
	}

	// If we got to this point, then that means that somethign went wrong.
	h = new Header(-100, ipp, "invalid", 0, "invalid");

	free(request);
	return h;
}	

