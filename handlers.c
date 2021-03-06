#include "handlers.h"

void get_coffee_counter(const char* date, json_t* json_response, void* db){
    int counter = getCoffeeCounterByDate(db, date);
    json_object_set_new(json_response, "status", json_string("success"));
    json_object_set_new(json_response, "counter", json_integer(counter));
}

void callback_get_coffee_counter(char today, json_t* json_request, json_t* json_response, void* db){
    if(today){
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char date[11];

        if(snprintf(date, 11, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday) <= 0){
            json_object_set_new(json_response, "status", json_string("error"));
            return;
        }

        get_coffee_counter(date, json_response, db);
    }
    else{
        json_t* json_date = json_object_get(json_request, "date");
        const char* date = json_string_value(json_date);

        if(!date){
            json_object_set_new(json_response, "status", json_string("error"));
            return;
        }

        get_coffee_counter(date, json_response, db);
    }
}

void add_coffee(const char* date, json_t* json_response, void* db){
    if(addCoffee(db, date) != 0)
        json_object_set_new(json_response, "status", json_string("error"));
    else
        json_object_set_new(json_response, "status", json_string("success"));
}

void callback_add_coffee(char today, json_t* json_request, json_t* json_response, void* db){
    if(today){
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char date[11];

        if(snprintf(date, 11, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday) <= 0){
            json_object_set_new(json_response, "status", json_string("error"));
            return;
        }

        initCoffeeDay(db);
        add_coffee(date, json_response, db);
    }
    else{
        json_t* json_date = json_object_get(json_request, "date");
        const char* date = json_string_value(json_date);

        if(!date)
            json_object_set_new(json_response, "status", json_string("error"));

        add_coffee(date, json_response, db);
    }
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
        callback_get_coffee_counter(0, json_request, json_response, db);
        ulfius_set_json_body_response(response, 200, json_response);
    }
    else if(!strcmp(operation, "add_coffee")){
        callback_add_coffee(0, json_request, json_response, db);
        ulfius_set_json_body_response(response, 200, json_response);
    }
    else if(!strcmp(operation, "add_today_coffee")){
        callback_add_coffee(1, json_request, json_response, db);
        ulfius_set_json_body_response(response, 200, json_response);
    }
    else if(!strcmp(operation, "get_today_coffee")){
        callback_get_coffee_counter(1, json_request, json_response, db);
        ulfius_set_json_body_response(response, 200, json_response);
    }
    else{
        json_object_set_new(json_response, "status", json_string("error"));
        ulfius_set_json_body_response(response, 405, json_response);
    }   
    

    return U_CALLBACK_CONTINUE;
}

int callback_options(const struct _u_request *request, struct _u_response *response, void *user_data){
  u_map_put(response->map_header, "Access-Control-Allow-Origin", "*");
  u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  u_map_put(response->map_header, "Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
  u_map_put(response->map_header, "Access-Control-Max-Age", "1800");
  return U_CALLBACK_COMPLETE;
}

int callback_login(const struct _u_request *request, struct _u_response *response, void* db){
    json_t* json_request;
    json_error_t json_error;
    json_t* json_response = json_object();

    json_request = ulfius_get_json_body_request(request, &json_error);

    if(json_request == NULL){
        json_object_set_new(json_response, "status", json_string("error"));
        json_object_set_new(json_response, "error", json_string(json_error.text));
        ulfius_set_json_body_response(response, 401, json_response);
        return U_CALLBACK_UNAUTHORIZED;
    }

    json_t* json_username = json_object_get(json_request, "username");
    json_t* json_password = json_object_get(json_request, "password");

    if(!json_username || !json_password){
        json_object_set_new(json_response, "status", json_string("error"));
        json_object_set_new(json_response, "error", json_string(json_error.text));
        ulfius_set_json_body_response(response, 401, json_response);
        return U_CALLBACK_UNAUTHORIZED;
    }

    const char* username = json_string_value(json_username);
    const char* password = json_string_value(json_password);
    char* user_password = getUserPassword(db, username);
    char* salt = getUserSalt(db, username);

    if(user_password == NULL){
        json_object_set_new(json_response, "status", json_string("error"));
        ulfius_set_json_body_response(response, 401, json_response);
        return U_CALLBACK_UNAUTHORIZED;
    }

    char* hash = crypt(password, salt);

    if(strcmp(user_password, hash) != 0){
        json_object_set_new(json_response, "status", json_string("error"));
        ulfius_set_json_body_response(response, 401, json_response);
        return U_CALLBACK_UNAUTHORIZED;
    }

    free(user_password);
    free(salt);

    json_object_set_new(json_response, "status", json_string("success"));
    ulfius_set_json_body_response(response, 200, json_response);

    char* auth_token = malloc(sizeof(char)*42);
    getrandom(auth_token, 42, 0);

    for(int i = 0; i < 42; i++){
        if(auth_token[i] < 0)
            auth_token[i] = - auth_token[i];
        auth_token[i] = 32 + (auth_token[i] % 95);
    }
    
    ulfius_add_cookie_to_response(response, "auth_token", auth_token, "1200", 1200, "localhost", "/", 0, 1);
    free(auth_token);
    return U_CALLBACK_CONTINUE;
}