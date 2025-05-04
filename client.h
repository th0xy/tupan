#ifndef _CLIENT_H
#define _CLIENT_H

#include "server.h"

#define MAX_HTTP_MESSAGE_LEN 4096

struct client
{
	int fd;
	struct sockaddr_storage ss;
	char ip_str[INET6_ADDRSTRLEN];
	char http_message[MAX_HTTP_MESSAGE_LEN];
	int rx_bytes;
	int port;
};

typedef struct client tpx_client_t;

tpx_client_t* create_client(int,struct sockaddr_storage*); 
void clear_client(tpx_client_t* client);

struct accept_command {
	tpx_server_t* server;
};

typedef struct accept_command accept_command_t;
void execute_accept_command(accept_command_t* command);

tpx_client_t* create_client(int,struct sockaddr_storage*);
void clear_client(tpx_client_t* client);
tpx_client_t* create_client(int , struct sockaddr_storage*);
#endif
