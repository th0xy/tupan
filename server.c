#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>
#include "server.h"
#include "observer.h"

tpx_server_t*
server_get_instance(address_family_t family) {
	
	static tpx_server_t* instance = NULL;

	if (instance == NULL) {
		instance = (tpx_server_t*)malloc(sizeof(tpx_server_t));
		if (instance == NULL) { perror("socket");
			exit(1);
		}

		instance->fd = socket(family == ISSET_IPV6 ? AF_INET6
							   : AF_INET, 
							   SOCK_STREAM, 0);
		if (instance->fd == -1) {
			perror("socket");
			free(instance);
			exit(2);
		}

		instance->halt = 0;
	}

	return instance;
}


void
tpx_set_opt(int s, bool so_linger, int linger_time)
{
	int i = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) == -1) {
		close(s);
		perror("so reuseaddr");
		exit (EXIT_FAILURE);
	}

	if (setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &i, sizeof(i)) == -1) {
		close(s);
		perror("so reuseport");
		exit (EXIT_FAILURE);
	}

	if (so_linger) {
		struct linger li = { .l_onoff = true, .l_linger = linger_time };
		if (setsockopt(s, SOL_SOCKET, 
			SO_LINGER, &li, sizeof(li)) == -1) {
			perror("so linger");
			exit (EXIT_FAILURE);
		}
	}
}

void 
tpx_set_addr(struct sockaddr_storage* ss, 
		in_port_t port,  
		address_family_t family)
{
	if (family == ISSET_IPV4) {
		struct sockaddr_in *addr_in = (struct sockaddr_in*)ss;
		addr_in->sin_family = AF_INET;
		addr_in->sin_port = htons(port);
		addr_in->sin_addr.s_addr = INADDR_ANY;
	} else if (family == ISSET_IPV6) {
		struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6*)ss;
		addr_in6->sin6_family = AF_INET6;
		addr_in6->sin6_port = htons(port);
		addr_in6->sin6_addr = in6addr_any;
	} else {
		fprintf(stdout, "Unknown address type.\n");
		exit(EXIT_FAILURE);
	}
}



void
tpx_close_server(tpx_server_t* server)
{
	free_observers(server);
	free(server);
}
