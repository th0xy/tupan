#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "server.h"

typedef void (*observer_callback_t)(int);

struct observer {
	observer_callback_t callback;
	struct observer* next;
};

typedef struct observer observer_t;

void add_observer(tpx_server_t* server, observer_callback_t callback);
void notify_observers(tpx_server_t* server, int client_fd);
void free_observers(tpx_server_t* server);
#endif

