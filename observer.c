#include <stdlib.h>
#include "observer.h"
#include "server.h"

void
add_observer(tpx_server_t* server, observer_callback_t callback) 
{
	observer_t* new_observer = (observer_t*)malloc(sizeof(observer_t));
	new_observer->callback = callback;
	new_observer->next = server->observers;
	server->observers = new_observer;
}

void
notify_observers(tpx_server_t* server, int client_fd)
{
	observer_t* current = server->observers;
	while(current != NULL) {
		current->callback(client_fd);
		current = current->next;
	}
}


void 
free_observers(tpx_server_t* server) {
    observer_t* current = server->observers;
    while (current != NULL) {
        observer_t* next = current->next;
        free(current);
        current = next;
    }
    server->observers = NULL;
}
