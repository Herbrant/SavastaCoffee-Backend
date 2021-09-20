#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int createDatabaseIfNotExists();
int createDatabaseTables(sqlite3* db);
int getCoffeeCounterByDate(sqlite3* db, const char* date);
int addCoffee(sqlite3* db, const char* date);
int initCoffeeDay(sqlite3* db);


#endif