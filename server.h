#ifndef SERVER_H_
#define SERVER_H_

#include <netinet/in.h>
#include <stdbool.h>


enum address_family {
	ISSET_IPV4,
	ISSET_IPV6
}; 

typedef enum address_family address_family_t;

struct server {
	int fd;
	struct sockaddr_storage ss;
	struct observer* observers;
	unsigned char halt;
};

typedef struct server tpx_server_t;

tpx_server_t* server_get_instance(address_family_t);
void tpx_set_opt(int, bool, int);
void tpx_set_addr(struct sockaddr_storage*, in_port_t, address_family_t);
void tpx_close_server(tpx_server_t* server);
void tpx_server_start(int backlog, in_port_t port);
#endif
