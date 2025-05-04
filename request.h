#ifndef REQUEST_H_
#define REQUEST_H_

#define MAX_METHOD_LEN  8
#define MAX_URI_LEN     256
#define MAX_VERSION_LEN 8
#define MAX_HEADERS	100
#define MAX_HEADER_LEN  512

struct http_request
{
	char  method[MAX_METHOD_LEN];
	char  uri[MAX_URI_LEN];
	float version;
	char  headers[MAX_HEADERS][MAX_HEADER_LEN];
	int   header_count;
};

typedef struct http_request http_request_t;
void parse_http_request(const char* http_message, http_request_t* request);

#endif
