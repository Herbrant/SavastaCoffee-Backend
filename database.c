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
		return rc;
	}

	char* sql2 = "CREATE TABLE IF NOT EXISTS Users(username TEXT PRIMARY KEY, salt TEXT NOT NULL, password TEXT NOT NULL, auth_token TEXT)";

	rc = sqlite3_exec(db, sql2, 0, 0, &err_msg);

	if(rc != SQLITE_OK){
		sqlite3_free(err_msg);    
        sqlite3_close(db);
		fprintf(stderr, "SQL error: %s\n", err_msg);
	}

	return rc;
}

int getCoffeeCounterByDate(sqlite3* db, const char* date){
	sqlite3_stmt* res;
	char* sql = "SELECT counter FROM Coffee WHERE date = ?";
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK) 
        sqlite3_bind_text(res, 1, date, strlen(date), NULL);
    else{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	int step = sqlite3_step(res);

	if(step == SQLITE_ROW){
		int counter = sqlite3_column_int(res, 0);
		sqlite3_finalize(res);
		return counter;
	}
	
	return -1;
}

int initCoffeeDay(sqlite3* db){
	char* err_msg = 0;
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
	char sql[64];

	if(snprintf(sql, 64, "INSERT INTO Coffee(date, counter) VALUES(\"%d-%02d-%02d\", 0)", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday) <= 0)
		return 1;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);


    if (rc != SQLITE_OK ) {
        //fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    
	return 0;
}

int addCoffee(sqlite3* db, const char* date){
	char* err_msg = 0;
	char* template_sql = "UPDATE Coffee SET counter = counter+1 WHERE date = ";
	char sql[64];

	if(snprintf(sql, 64, "%s\"%s\"", template_sql, date) <= 0)
		return 1;

	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    } 
    
	return 0;
}

char* getUserPassword(sqlite3* db, const char* username){
	sqlite3_stmt* res;
	char* sql = "SELECT password FROM Users WHERE username = ?";
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK)
        sqlite3_bind_text(res, 1, username, strlen(username), NULL);
    else{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		return NULL;
	}
	
	int step = sqlite3_step(res);

	char* p = malloc(256*sizeof(char));

	if(step == SQLITE_ROW){
		const char* password = sqlite3_column_text(res, 0);
		strncpy(p, password, 256);
		sqlite3_finalize(res);
		return p;
	}
	
	return NULL;
}

char* getUserSalt(sqlite3* db, const char* username){
	sqlite3_stmt* res;
	char* sql = "SELECT salt FROM Users WHERE username = ?";
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK)
        sqlite3_bind_text(res, 1, username, strlen(username), NULL);
    else{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		return NULL;
	}
	
	int step = sqlite3_step(res);

	char* p = malloc(256*sizeof(char));

	if(step == SQLITE_ROW){
		const char* password = sqlite3_column_text(res, 0);
		strncpy(p, password, 256);
		sqlite3_finalize(res);
		return p;
	}
	
	return NULL;
}

int setUserAuthToken(sqlite3* db, const char* username, const char* token){
	sqlite3_stmt* res;
	char* err_msg = 0;
	char* template_sql = "UPDATE Users SET auth_token = %s WHERE username = %s";
	char sql[1024];

	if(snprintf(sql, 1024, template_sql, token, username) <= 0)
		return 1;

	return 0;
}

char* getUserAuthToken(sqlite3* db, const char* username){
	sqlite3_stmt* res;
	char* sql = "SELECT auth_token FROM Users WHERE username = ?";
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK)
        sqlite3_bind_text(res, 1, username, strlen(username), NULL);
    else{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		return NULL;
	}
	
	int step = sqlite3_step(res);

	char* p = malloc(256*sizeof(char));

	if(step == SQLITE_ROW){
		const char* auth_token = sqlite3_column_text(res, 0);
		strncpy(p, auth_token, 256);
		sqlite3_finalize(res);
		return p;
	}
	
	return NULL;
}

int checkAuthToken(sqlite3* db, const char* username, const char* token){
	char* settedToken = getUserAuthToken(db, username);
	return strcmp(token, settedToken);
}