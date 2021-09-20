#ifndef HANDLERS_H_INCLUDED
#define HANDLERS_H_INCLUDED
#include <ulfius.h>
#include <string.h>
#include <sqlite3.h>

int callback_hello_world (const struct _u_request* request, struct _u_response* response, void* user_data);
int callback_add_coffee(const struct _u_request* request, struct _u_response* response, void* user_data);


#endif

