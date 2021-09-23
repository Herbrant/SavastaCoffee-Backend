#ifndef HANDLERS_H_INCLUDED
#define HANDLERS_H_INCLUDED
#include <ulfius.h>
#include <string.h>
#include <sqlite3.h>
#include "database.h"

int callback_coffee(const struct _u_request* request, struct _u_response* response, void* db);
void get_coffee_counter(const char* date, json_t* json_response, void* db);
void callback_get_coffee_counter(char today, json_t* json_request, json_t* json_response, void* db);
void add_coffee(const char* date, json_t* json_response, void* db);
void callback_add_coffee(char today, json_t* json_request, json_t* json_response, void* db);
int callback_options(const struct _u_request *request, struct _u_response *response, void *user_data);


#endif

