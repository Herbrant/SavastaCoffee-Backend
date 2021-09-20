#include <stdio.h>
#include <ulfius.h>
#include <sqlite3.h>
#include "handlers.h"
#include "database.h"

#define PORT 8888
#define DB_PATH "./database.db"


int main(void) {
	struct _u_instance instance;

	// Initialize instance with the port number
	if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
		fprintf(stderr, "Error ulfius_init_instance, abort\n");
		return(1);
	}

	//Initialize sqlite3
	if(createDatabaseIfNotExists(DB_PATH))
		return 1;
	
	sqlite3 *db;
	sqlite3_stmt *res;
	int rc = sqlite3_open(DB_PATH, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}

	if(createDatabaseTables(db) != SQLITE_OK)
		return(1);

	// Endpoint list declaration
	ulfius_add_endpoint_by_val(&instance, "POST", "/", NULL, 0, &callback_coffee, db);

	// Start the framework
	if (ulfius_start_framework(&instance) == U_OK) {
		printf("Start framework on port %d\n", instance.port);

		// Wait for the user to press <enter> on the console to quit the application
		getchar();
	} 
	else {
		fprintf(stderr, "Error starting framework\n");
	}

	printf("End framework\n");

	ulfius_stop_framework(&instance);
	ulfius_clean_instance(&instance);

	sqlite3_close(db);
	return 0;
}
