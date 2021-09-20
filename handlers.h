#ifndef HANDLERS_H_INCLUDED
#define HANDLERS_H_INCLUDED
#include <ulfius.h>
#include <string.h>
#include <sqlite3.h>
#include "database.h"

int callback_coffee(const struct _u_request* request, struct _u_response* response, void* db);
void callback_coffee_get_by_date(json_t* json_request, json_t* json_response, void* db);
void callback_add_coffee(json_t* json_request, json_t* json_response, void* db);
void callback_add_today_coffee(json_t* json_request, json_t* json_response, void* db);
void callback_get_today_coffee(json_t* json_request, json_t* json_response, void* db);


#endif

