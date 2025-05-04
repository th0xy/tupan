#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "request.h"
void
parse_http_request(const char* http_message, http_request_t* request)
{
	char* saveptr;
	char* token = strtok_r((char*)http_message, "\n\r", &saveptr);

	sscanf((char*)token,"%7s %255s HTTP/%f", 
		request->method,
		request->uri,
		&request->version);

	request->header_count = 0;

	while((token = strtok_r(NULL,"\r\n", &saveptr)) != NULL) {
		if (strcmp(token, "") == 0) break;

		if (request->header_count < MAX_HEADERS) {
			strncpy(request->headers[request->header_count], 
				token, MAX_HEADER_LEN - 1);
			request->headers[request->header_count]
				[MAX_HEADER_LEN -1] = '\0';
			request->header_count++;
		}

	};

	for (int i = 0; i < request->header_count; i++) {
		printf("headers %s\n", request->headers[i]);
	}
}
