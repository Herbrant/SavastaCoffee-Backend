#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int createDatabaseIfNotExists();
int createDatabaseTables(sqlite3* db);
int getCoffeeCounterByDate(sqlite3* db, const char* date);
int addCoffee(sqlite3* db, const char* date);
int initCoffeeDay(sqlite3* db);
char* getUserPassword(sqlite3* db, const char* username);
int setUserAuthToken(sqlite3* db, const char* username, const char* token);
char* getUserAuthToken(sqlite3* db, const char* username);
char* getUserSalt(sqlite3* db, const char* username);


#endif