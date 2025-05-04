#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <unistd.h>
#include "client.h"
#include "observer.h"
#include "request.h"

tpx_client_t* 
create_client(int client_fd, struct sockaddr_storage* addr)
{
	tpx_client_t* new_client = (tpx_client_t*)malloc(sizeof(tpx_client_t));

	if (new_client == NULL) {
		perror("Failed to create memory for client");
		return NULL;
	}

	new_client->fd = client_fd;
	new_client->ss = *addr;

	if (addr->ss_family == AF_INET) {
		struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
		new_client->port = ntohs(addr_in->sin_port);
		inet_ntop(AF_INET, &addr_in->sin_addr, 
				new_client->ip_str, sizeof(new_client->ip_str));
	} else if (addr->ss_family == AF_INET6)
	{
		struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)addr;
		new_client->port = ntohs(addr_in6->sin6_port);
		inet_ntop(AF_INET, &addr_in6->sin6_addr, 
				new_client->ip_str, sizeof(new_client->ip_str));
	} else{
		fprintf(stderr, "Unkown address_family");
		free(new_client);
		return NULL;
	}

	return new_client;
}

void 
execute_accept_command(accept_command_t* command)
{
	int client_fd = accept(command->server->fd, NULL, NULL);
	if (client_fd >= 0) {
		notify_observers(command->server, client_fd);
	} else {
		perror("accept failed");
	}
}

void 
close_client(tpx_client_t* client) 
{
	if (client) {
		close(client->fd);
		free(client);
	}
}
