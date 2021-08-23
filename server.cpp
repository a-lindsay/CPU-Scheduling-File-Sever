#include "server.h"
#include "logger.h"

extern Logger logger;

Scheduler *sched = nullptr;

char *ipp = NULL;

void SetIPAddr(struct in_addr, in_port_t);

int main(int argc, char **argv) {
	int 	n;
	int 	sd, new_sd; 
	unsigned int	client_len;
	int port;
	unsigned int threads;
	unsigned int num_bufs;
	struct 	sockaddr_in 	server, client;
	char 	*bp, buf[BUFLEN];
	struct timespec ts;
	memset(&ts, 0, sizeof(struct timespec));
	char time_buff[100] = {0};
	std::string t_string;
	char ip_buff[100] = {0};
	char port_buff[100] = {0};
	pthread_t thread;
	bp = &buf[0];
	// Clear the buffer
	memset(bp, 0, BUFLEN);
	std::string request;

	// Set arguments

	if (argc != 6) {
		fprintf(stderr, "Usage: ./server [portnum] [threads]");
		fprintf(stderr, " [buffers] [sched] [directory]\n");
		exit(1);
	}

	// Convert port, thread, and buffers to integers
	port = (unsigned short) strtol(argv[1], NULL, 10);
	threads = (unsigned int) strtol(argv[2], NULL, 10);
	num_bufs = (unsigned int) strtoul(argv[3], NULL, 10);

	if (!initSemaphores(threads, num_bufs)) {
		fprintf(stderr, "Failed to set semphores!\n");
		exit(1);
	}

	// Use the scheduler factory to set sched
	sched = MakeScheduler(argv[4]);

	if (!sched) {
		fprintf(stderr, "Invalid scheduler\n");
		sem_destroy(a_sem);
		sem_destroy(t_sem);
		exit(1);
	}

	// Check if media directory is valid
	if (!directoryCheck(argv[5])) {
		fprintf(stderr, "Invalid directory\n");
		sem_destroy(a_sem);
		sem_destroy(t_sem);
		exit(1);
	}

	// Set global task info struct's path member
	gti.path = argv[5];
	gti.listing = list();

	/* Run the dispatcher for the scheduler in a separate thread */
	pthread_create(&thread, NULL, dispatch, (void *) &threads);
		
	/* Create a stream socket */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't create a socket\n");
		sem_destroy(a_sem);
		sem_destroy(t_sem);
		exit(1);
	}

	/* Bind an address to the socket */
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	/* Set the ascii form of the ip address and port.
	 * This will be used by the header class
	 */
	SetIPAddr(server.sin_addr, port);


	if (bind(sd, (struct sockaddr *)&server, 
	sizeof(server)) == -1) {
		fprintf(stderr, "Can't bind name to socket\n");
		sem_destroy(a_sem);
		sem_destroy(t_sem);
		exit(1);
	}

	/* queue up to 5 connect requests */
	listen(sd, 1000);

	if (!logger.LogInit(".log")) {
		sem_destroy(a_sem);
		sem_destroy(t_sem);
		exit(1);
	}

	while (1) {
		request = "";
		// Get semaphore for next connection
		sem_wait(a_sem);
		client_len = sizeof(client);
		if ((new_sd = accept(sd, (struct sockaddr *)&client,
		&client_len)) == -1) {
			fprintf(stderr, "Can't accept client\n");
			sem_destroy(a_sem);
			sem_destroy(t_sem);
			exit(1);
		}

		timespec_get(&ts,TIME_UTC);
		inet_ntop(AF_INET, &client.sin_addr,ip_buff,100);
		std::ostringstream oss;
		oss << ip_buff;
		std::string client_ipp(oss.str());
		oss.clear();
		oss.str("");
		oss << client.sin_port;
		client_ipp.append(":");
		client_ipp.append(oss.str());
		oss.clear();
		oss.str("");
		strftime(time_buff, sizeof(time_buff),"%D %T", 
			gmtime(&ts.tv_sec));
		t_string = time_buff;
		t_string.append(" ");
		oss << ts.tv_nsec;
		t_string.append(oss.str());
		t_string.append("\n");
		logger.LogWrite(std::string("Accepted:\n"));
		logger.LogWrite(client_ipp);
		logger.LogWrite("\n");
		logger.LogWrite(t_string);
		memset(ip_buff, 0, strlen(ip_buff));
		memset(time_buff,0,strlen(time_buff));

		// Disable read blocking
		int flags = fcntl(new_sd, F_GETFL, 0);
		fcntl(new_sd, F_SETFL, flags | O_NONBLOCK);
		
		bp = &buf[0];
		request = "";
		while ((n = read(new_sd, bp, BUFLEN)) > 0 || errno == EAGAIN) {
			if (n == -1)
				continue;

			bp[n] = 0;
			request.append(bp);
			memset(bp, 0, BUFLEN);

			if (request.find("\n", 0) != std::string::npos) {
				break;
			}
		}

		if (request.empty()) {
			fprintf(stderr, "Failed to get command!\n");
			fprintf(stderr, "Value of n: %d\n", n);
			close(new_sd);
			sem_post(a_sem);
			continue;
		}

		
		// Make new task
		TaskInfo *t = MakeTask(new_sd, request.c_str(), client_ipp);
		if (t == nullptr) {
			fprintf(stderr, "Failed to make task!\n");
			close(new_sd);
			close(sd);
			sem_destroy(a_sem);
			sem_destroy(t_sem);
			return 1;
		}

		logger.LogWrite(std::string("Request:\n"));
		logger.LogWrite(request);
		// Add task into the queue and let scheduler
		// handle running it
		sched->Add_Task(t);
	}

	delete sched; sched = nullptr;
	sem_destroy(a_sem);
	sem_destroy(t_sem);
	close(sd);
	return(0);
}

void SetIPAddr(struct in_addr addr, unsigned short port) {
	// Set the address
	char *address = inet_ntoa(addr);
	// Get the size
	size_t addr_size = strlen(address);
	// Get the length of the port number
	size_t port_len = snprintf(NULL, 0, "%u", port);
	// Create a buffer and 0 the buffer
	char port_buff[port_len+1];
	char *pb_ptr = &port_buff[0];
	memset(pb_ptr, 0, port_len+1);

	// Copy the port number
	snprintf(pb_ptr, port_len + 1, "%u", port);

	/* Allocate enough memory for the ip address,
	 * the port number, and also a colon
	 */
	ipp = new char[addr_size + port_len + 2];

	// Copy the ip address into ipp
	ipp = strcpy(ipp, address);
	// Add the colon
	ipp[addr_size] = ':';
	// Copy the port into ipp now
	char *ipp_ptr_2 = strcpy(&ipp[addr_size+1], pb_ptr);
}	




