#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/select.h>

#include <string.h>

#include "server.h"
#include "observer.h"
#include "client.h"
#include "request.h"

static void 
on_client_connected(int client_fd) 
{
	struct sockaddr_storage ss;
	socklen_t addr_len = sizeof(ss);
	http_request_t req;

	if (getpeername(client_fd, (struct sockaddr*)&ss, &addr_len) == -1) {
		perror("getperrname");
		return;
	}

	tpx_client_t* client = create_client(client_fd, &ss);
	if (client == NULL) {
		return;
	}

	client->rx_bytes = recv(client->fd, 
			client->http_message, 
			MAX_HTTP_MESSAGE_LEN, 0);

	parse_http_request(client->http_message, &req);
}

static void 
tpx_server_loop(tpx_server_t* server)
{
	fd_set current_sockets, ready_sockets;
	int max_fd = server->fd;
	FD_ZERO(&current_sockets);
	FD_SET(server->fd, &current_sockets);
	while (!server->halt) {

		ready_sockets = current_sockets;
		
		if (select(max_fd + 1, &ready_sockets, NULL, NULL, NULL) < 0) {
			perror("select error");
			exit(EXIT_FAILURE);
		}

		if (FD_ISSET(server->fd, &ready_sockets)) {
			accept_command_t command = { .server = server };
			execute_accept_command(&command);
		}

		for (int i = 0; i < max_fd; i++) {
			if (FD_ISSET(i, &ready_sockets)) {
				if (i != server->fd) {
					notify_observers(server, i);
					FD_CLR(i, &current_sockets);
					close(i);
				}
			}
		}
		
	}
	close(server->fd);
	tpx_close_server(server);
}

void
tpx_server_start(int backlog, in_port_t port) 
{
	int family = ISSET_IPV4;
	tpx_server_t* server = server_get_instance(family);
	tpx_set_addr(&server->ss, port, family);
	tpx_set_opt(server->fd, true, 30);

	add_observer(server, on_client_connected);

	if (bind(server->fd, 
		(struct sockaddr*)&server->ss, sizeof(server->ss)) < 0) {
		close(server->fd);
		free(server);
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server->fd, backlog) == -1) {
		close(server->fd);
		free(server);
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	tpx_server_loop(server);

}


int main(void) 
{
	tpx_server_start(10, 8989);
	return 0;
}
