#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>

int createDatabaseIfNotExists();
int createDatabaseTables(sqlite3* db);


#endif