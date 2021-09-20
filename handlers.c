#include "handlers.h"
/**
 * Callback function for the web application on /helloworld url call
 */
int callback_hello_world (const struct _u_request * request, struct _u_response * response, void * user_data) {
    ulfius_set_string_body_response(response, 200, "Hello World!");
    return U_CALLBACK_CONTINUE;
}

int callback_add_coffee(const struct _u_request* request, struct _u_response* response, void* user_data){
    json_t* json_request;
    json_error_t json_error;
    json_t* json_response = json_object();

    json_request = ulfius_get_json_body_request(request, &json_error);

    if(json_request == NULL){
        json_object_set_new(json_response, "status", json_string("error"));
        json_object_set_new(json_response, "error", json_string(json_error.text));
        ulfius_set_json_body_response(response, 301, json_response);
    }
    else{
        json_object_set_new(json_response, "status", json_string("success"));
        ulfius_set_json_body_response(response, 200, json_response);
    }

    return U_CALLBACK_CONTINUE;
}