#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <thread>
#include <chrono>



struct addrinfo * setup(const char * const t_domain, const char * t_port) {
	struct addrinfo hints, *res;
	int status;


	memset(&hints, 0, sizeof hints);
	// IPv4
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(t_domain, t_port, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
	}

	return res;
}


void connect_with_timeout(struct addrinfo * res) {
	int sockfd;
	// in usec
	const int TIMEOUT = 500000;
	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		fprintf(stderr, "Value of errno: %i\n", errno);
		perror("socket fd failed");
		return;
	}

	// set timeout of 0.5s 
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = TIMEOUT;
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout);


	if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
		struct sockaddr_in *sa; 
		sa = (struct sockaddr_in *)res->ai_addr;
		unsigned int port = sa->sin_port;
		port = ntohs(port);
		fprintf(stdout, "Port %i is open.\n", port);
	}

	if (close(sockfd) == -1) {
		fprintf(stdout, "Could not close the connection succesfully.\n");
	}
	
	return;
}



int main(int argc, char** argv) {

	if (argc != 2 && argc != 3) {
		fprintf(stderr, "usage: %s {domain_name} [work_multiplier]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// return addrinfo struct after passing in domain name

	uint16_t MIN = 1;
	uint16_t MAX = 65535;
	struct addrinfo *list_res[MAX];
	uint8_t WORK_MULTIPLIER;

	argc == 3 ? WORK_MULTIPLIER = atoi(argv[2]) : WORK_MULTIPLIER = 60;
	uint8_t num_threads = std::thread::hardware_concurrency();
	fprintf(stdout, "Current number of threads: %i\n", num_threads);

	// Launch pool with specified threads
	boost::asio::thread_pool pool(WORK_MULTIPLIER * num_threads);
	fprintf(stdout, "Launching %i workers\n", WORK_MULTIPLIER * num_threads);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	// Submit jobs to the pool
	for (int i=MIN; i <= MAX; i++) {
		list_res[i-1] = setup(argv[1], (std::to_string(i)).c_str());
		boost::asio::post(pool, std::bind(connect_with_timeout, list_res[i-1]));
	}

	// Wait for all tasks in pool to complete
	pool.wait();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	fprintf(stdout, "Scanned 65535 ports.\n");
	fprintf(stdout, "Time elapsed: %lis\n", std::chrono::duration_cast<std::chrono::seconds>(end-begin).count());

	// free the dynamic link list given by setup
	for (int i=MIN; i<= MAX; i++) {
		freeaddrinfo(list_res[i-1]);
	}

	return 0;
}
