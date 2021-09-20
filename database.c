#include "database.h"

int createDatabaseIfNotExists(char* db_path){
	if(!access(db_path, F_OK))
		return 0;
	
	FILE* f = fopen(db_path, "w");

	if(f == NULL){
		fprintf(stderr, "Database error: creating %s file", db_path);
		return 1;
	}
	
	fclose(f);
	return 0;
}

int createDatabaseTables(sqlite3* db){
	char* err_msg = 0;
	char* sql = "CREATE TABLE IF NOT EXISTS Coffee(date DATE DEFAULT CURRENT_DATE PRIMARY KEY, counter INT DEFAULT 0);";

	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if(rc != SQLITE_OK){
		sqlite3_free(err_msg);    
        sqlite3_close(db);
		fprintf(stderr, "SQL error: %s\n", err_msg);
	}

	return rc;
}