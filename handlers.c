#include "handlers.h"

void callback_coffee_get_by_date(json_t* json_request, json_t* json_response, void* db){
    json_t* json_date = json_object_get(json_request, "date");
    const char* date = json_string_value(json_date);

    if(!date){
        json_object_set_new(json_response, "status", json_string("error"));
        return;
    }
    
    int counter = getCoffeeCounterByDate(db, date);
    json_object_set_new(json_response, "status", json_string("success"));
    json_object_set_new(json_response, "counter", json_integer(counter));
}

void callback_add_coffee(json_t* json_request, json_t* json_response, void* db){
    json_t* json_date = json_object_get(json_request, "date");
    const char* date = json_string_value(json_date);

    if(!date)
        json_object_set_new(json_response, "status", json_string("error"));
    else if(addCoffee(db, date) != 0)
        json_object_set_new(json_response, "status", json_string("error"));
    else
        json_object_set_new(json_response, "status", json_string("success"));
}

void callback_add_today_coffee(json_t* json_request, json_t* json_response, void* db){
    time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
    char date[11];

    if(snprintf(date, 11, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday) <= 0){
        json_object_set_new(json_response, "status", json_string("error"));
        return;
    }

    printf("%s\n", date);
    
    initCoffeeDay(db);

    if(addCoffee(db, date) != 0)
        json_object_set_new(json_response, "status", json_string("error"));
    else
        json_object_set_new(json_response, "status", json_string("success"));
}

void callback_get_today_coffee(json_t* json_request, json_t* json_response, void* db){
    time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
    char date[11];

    if(snprintf(date, 11, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday) <= 0){
        json_object_set_new(json_response, "status", json_string("error"));
        return;
    }
    
    int counter = getCoffeeCounterByDate(db, date);
    json_object_set_new(json_response, "status", json_string("success"));
    json_object_set_new(json_response, "counter", json_integer(counter));
}

int callback_coffee(const struct _u_request* request, struct _u_response* response, void* db){
    json_t* json_request;
    json_error_t json_error;
    json_t* json_response = json_object();

    json_request = ulfius_get_json_body_request(request, &json_error);

    if(json_request == NULL){
        json_object_set_new(json_response, "status", json_string("error"));
        json_object_set_new(json_response, "error", json_string(json_error.text));
        ulfius_set_json_body_response(response, 301, json_response);
        return U_CALLBACK_CONTINUE;
    }

    json_t* json_operation = json_object_get(json_request, "operation");

    if(!json_operation)
        return U_CALLBACK_CONTINUE;
    
    const char* operation = json_string_value(json_operation);

    if(!strcmp(operation, "get_by_date")){
        callback_coffee_get_by_date(json_request, json_response, db);
        ulfius_set_json_body_response(response, 200, json_response);
    }
    else if(!strcmp(operation, "add")){
        callback_add_coffee(json_request, json_response, db);
        ulfius_set_json_body_response(response, 200, json_response);
    }
    else if(!strcmp(operation, "add_today_coffee")){
        callback_add_today_coffee(json_request, json_response, db);
        ulfius_set_json_body_response(response, 200, json_response);
    }
    else if(!strcmp(operation, "get_today_coffee")){
        callback_get_today_coffee(json_request, json_response, db);
        ulfius_set_json_body_response(response, 200, json_response);
    }
    else{
        json_object_set_new(json_response, "status", json_string("error"));
        ulfius_set_json_body_response(response, 405, json_response);
    }   
    

    return U_CALLBACK_CONTINUE;
}